#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "1tracker_pi/state_store.h"

namespace {

void expect(bool condition, const char* message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

}  // namespace

int main() {
  tracker_pi::StateStore store;
  expect(!store.hasValidPosition(), "position should start invalid");

  store.updateTimevalue(1710000000);
  store.updateLatLon(52.12345, 4.98765);
  store.updateAWA(38.2);
  store.updateAWS(14.6);

  const auto snapshot = store.getSnapshot();
  expect(snapshot.hasValidPosition(), "position should be valid");
  expect(snapshot.hasTimestamp(), "timestamp should be present");
  expect(snapshot.awa.has_value() && *snapshot.awa == 38.2, "awa mismatch");
  expect(snapshot.aws.has_value() && *snapshot.aws == 14.6, "aws mismatch");

  // Invalid updates should leave the previous value intact
  const double nan = std::numeric_limits<double>::quiet_NaN();
  const double inf = std::numeric_limits<double>::infinity();

  store.updateLatLon(nan, 5.0);
  store.updateLatLon(91.0, 5.0);    // lat out of range
  store.updateLatLon(10.0, 181.0);  // lon out of range
  store.updateLatLon(inf, inf);
  const auto afterBadLatLon = store.getSnapshot();
  expect(afterBadLatLon.lat.has_value() && *afterBadLatLon.lat == 52.12345,
         "lat overwritten by invalid value");
  expect(afterBadLatLon.lon.has_value() && *afterBadLatLon.lon == 4.98765,
         "lon overwritten by invalid value");

  store.updateAWA(nan);
  store.updateAWA(-1.0);
  store.updateAWA(361.0);
  store.updateAWS(nan);
  store.updateAWS(-0.1);
  const auto afterBadWind = store.getSnapshot();
  expect(afterBadWind.awa.has_value() && *afterBadWind.awa == 38.2,
         "awa overwritten by invalid value");
  expect(afterBadWind.aws.has_value() && *afterBadWind.aws == 14.6,
         "aws overwritten by invalid value");

  // Boundary values should be accepted
  store.updateLatLon(-90.0, -180.0);
  store.updateAWA(0.0);
  store.updateAWA(360.0);
  store.updateAWS(0.0);
  const auto afterBoundary = store.getSnapshot();
  expect(afterBoundary.lat.has_value() && *afterBoundary.lat == -90.0,
         "-90 lat should be accepted");
  expect(afterBoundary.lon.has_value() && *afterBoundary.lon == -180.0,
         "-180 lon should be accepted");
  expect(afterBoundary.awa.has_value() && *afterBoundary.awa == 360.0,
         "360 awa should be accepted");
  expect(afterBoundary.aws.has_value() && *afterBoundary.aws == 0.0,
         "0 aws should be accepted");

  return EXIT_SUCCESS;
}
