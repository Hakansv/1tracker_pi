# Toolbar Icons

The `1tracker_pi` toolbar button uses one source SVG. The plugin renders
three in-memory color variants (neutral / green / red) from that template
at runtime. Nothing is written to disk.

## Required In A Release

The package/install must include:

- `Contents/PlugIns/1tracker_pi.dylib` (macOS) / `1tracker_pi.dll` (Windows) /
  `lib1tracker_pi.so` (Linux/Android)
- `data/icons/1tracker_toolbar_icon.svg` — the template SVG, containing the
  placeholder tokens `__MARKER_FILL__`, `__MARKER_STROKE__`, `__BOAT_FILL__`

Install paths per platform:

- macOS: `Contents/SharedSupport/plugins/1tracker_pi/data/icons/`
- Other: `share/opencpn/plugins/1tracker_pi/data/icons/`

## How It Works

At plugin `Init()`:

1. The template SVG is read once into memory via `wxFile::ReadAll`.
2. For each toolbar state (`Neutral`, `Green`, `Red`) the plugin does three
   plain `std::string::replace` passes on the template to substitute the
   color tokens, then passes the resulting string to
   `wxBitmapBundle::FromSVG(const char*, wxSize)` to produce a `wxBitmap`.
3. `InsertPlugInTool` registers the neutral bitmap as the toolbar icon.

When status changes (`updateEndpointStatus` / `applyRuntimeConfig`):

- `refreshToolbarIcon()` recomputes the state, renders the matching bitmap
  in memory, and calls `SetToolbarToolBitmaps`.

There are no runtime files in
`~/Library/Preferences/opencpn/plugins/1tracker_pi/` other than `config.json`.
There is no OpenCPN SVG cache interaction for the toolbar — OpenCPN only
caches SVGs it reads from disk by path.

## Dev Flow

On macOS:

```sh
cmake --build build-wx32 --target deploy_opencpn_dev
```

Then restart OpenCPN. Because the color variants live only in memory, a
changed template SVG takes effect on the next OpenCPN launch; there are no
stale disk caches to clear.

## Diagnostics

If the toolbar button is missing or the status colors are wrong:

1. Confirm the template is deployed:
   - macOS: `~/Library/Application Support/OpenCPN/Contents/SharedSupport/plugins/1tracker_pi/data/icons/1tracker_toolbar_icon.svg`
   - Windows: `%LOCALAPPDATA%\opencpn\plugins\1tracker_pi\data\icons\1tracker_toolbar_icon.svg`
2. In `opencpn.log`, look for these plugin log lines:
   - `1tracker_pi: loaded toolbar template from <path> (<N> bytes)` — template
     load succeeded.
   - `1tracker_pi: toolbar template not found for 1tracker_toolbar_icon.svg` —
     asset missing from the install.
   - `1tracker_pi: toolbar tool id=<N>, template_bytes=<N>` — `InsertPlugInTool`
     was called.
3. Verify the template still contains the three placeholder tokens
   (`__MARKER_FILL__`, `__MARKER_STROKE__`, `__BOAT_FILL__`). Without them
   the variants all render identically to the template's literal colors.
