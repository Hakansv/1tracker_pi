#!/usr/bin/env bash
# Build the core library and test binaries with clang's source-based coverage
# instrumentation, run the tests, and emit an HTML report under build-coverage/html/.
#
# Open the report:  open build-coverage/html/index.html
set -e

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${REPO_ROOT}/build-coverage"
PROFILE_DIR="${BUILD_DIR}/profiles"
REPORT_DIR="${BUILD_DIR}/html"

COV_FLAGS="-fprofile-instr-generate -fcoverage-mapping -O0 -g"

LLVM_PROFDATA="${LLVM_PROFDATA:-$(xcrun -f llvm-profdata 2>/dev/null || command -v llvm-profdata)}"
LLVM_COV="${LLVM_COV:-$(xcrun -f llvm-cov 2>/dev/null || command -v llvm-cov)}"
if [ -z "$LLVM_PROFDATA" ] || [ -z "$LLVM_COV" ]; then
    echo "error: llvm-profdata / llvm-cov not found on PATH" >&2
    exit 1
fi

echo "==> Configuring $BUILD_DIR"
rm -rf "$BUILD_DIR"
cmake -S "$REPO_ROOT" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS="$COV_FLAGS" \
    -DCMAKE_C_FLAGS="$COV_FLAGS" \
    -DCMAKE_EXE_LINKER_FLAGS="-fprofile-instr-generate" \
    -DCMAKE_SHARED_LINKER_FLAGS="-fprofile-instr-generate" \
    > /dev/null

echo "==> Building core + test targets"
TEST_TARGETS=(
    onetracker_core
    test_atomic_file_writer
    test_config_loader
    test_endpoint_error_summary
    test_endpoint_identity
    test_endpoint_sender
    test_endpoint_type_behavior
    test_payload_builder
    test_scheduler
    test_secret_masking
    test_state_store
)
cmake --build "$BUILD_DIR" --target "${TEST_TARGETS[@]}" -j > /dev/null

mkdir -p "$PROFILE_DIR"
rm -f "$PROFILE_DIR"/*.profraw

echo "==> Running test binaries"
TEST_BINARIES=()
for target in "${TEST_TARGETS[@]}"; do
    case "$target" in
        test_*)
            bin="$BUILD_DIR/test/$target"
            if [ -x "$bin" ]; then
                TEST_BINARIES+=("$bin")
                LLVM_PROFILE_FILE="$PROFILE_DIR/$target.profraw" "$bin"
                echo "   - $target OK"
            else
                echo "   ! missing $bin" >&2
            fi
            ;;
    esac
done

echo "==> Merging profiles"
MERGED="$PROFILE_DIR/merged.profdata"
"$LLVM_PROFDATA" merge -sparse "$PROFILE_DIR"/*.profraw -o "$MERGED"

echo "==> Generating HTML report"
# -object args: one per binary, llvm-cov combines coverage across them.
OBJ_ARGS=()
for bin in "${TEST_BINARIES[@]}"; do
    OBJ_ARGS+=(-object "$bin")
done

# Only include project sources under src/ and include/ — skip opencpn-libs/, tests, system headers.
SRC_FILTER='^.*/1tracker_pi/(src|include)/'

rm -rf "$REPORT_DIR"
"$LLVM_COV" show "${OBJ_ARGS[@]}" \
    --instr-profile="$MERGED" \
    --format=html \
    --output-dir="$REPORT_DIR" \
    --show-line-counts-or-regions \
    --show-expansions \
    --show-branches=count \
    --sources "$REPO_ROOT/src" "$REPO_ROOT/include" \
    > /dev/null

echo "==> Summary (project sources only)"
# llvm-cov report writes project paths relative to the repo root, and opencpn-libs/tests/include
# headers are bundled in. Filter to src/ only, then aggregate.
"$LLVM_COV" report "${OBJ_ARGS[@]}" \
    --instr-profile="$MERGED" 2>/dev/null \
    | awk '
        BEGIN { h = 0 }
        /^Filename/ && h == 0 { print; h = 1; next }
        /^---+$/ && h == 1 { print; next }
        $1 ~ /^src\// {
            print
            regions_total += $2; regions_miss += $3
            funcs_total += $5;   funcs_miss   += $6
            lines_total += $8;   lines_miss   += $9
            branches_total += $11; branches_miss += $12
        }
        END {
            if (lines_total > 0) {
                br_pct = 0.0
                if (branches_total > 0) br_pct = 100.0*(branches_total-branches_miss)/branches_total
                printf "%-60s  lines %d/%d = %.2f%%   funcs %d/%d = %.2f%%   branches %d/%d = %.2f%%\n",
                    "PROJECT-TOTAL (src/ only)",
                    lines_total - lines_miss, lines_total, 100.0*(lines_total-lines_miss)/lines_total,
                    funcs_total - funcs_miss, funcs_total, 100.0*(funcs_total-funcs_miss)/funcs_total,
                    branches_total - branches_miss, branches_total, br_pct
            }
        }
      '

echo ""
echo "HTML report: $REPORT_DIR/index.html"
echo "Open with:   open '$REPORT_DIR/index.html'"
