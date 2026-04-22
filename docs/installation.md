# 1tracker_pi — Installation Guide (beta testers)

Thank you for testing **1tracker_pi**, an OpenCPN plugin that sends your boat
position (and optionally wind data) to HTTP endpoints — such as NoForeignLand or
your own server.

---

## Requirements

- OpenCPN **5.6 or newer**
- Internet connection during installation

---

## Step 1 — Download the plugin

Pick the build that matches your operating system:

| Operating system | Download |
|---|---|
| **macOS** (Apple Silicon & Intel) | [1tracker-v0.1.0-beta1_darwin-universal.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-beta/raw/names/1tracker-v0.1.0-beta1-darwin-wx32-universal-10.15-tarball/versions/v0.1.0-beta1/1tracker-v0.1.0-beta1_darwin-wx32-10.15-arm64-x86_64.tar.gz) |
| **Windows** | [1tracker-v0.1.0-beta1_windows.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-beta/raw/names/1tracker-v0.1.0-beta1-msvc-wx32-10-tarball/versions/v0.1.0-beta1/1tracker-v0.1.0-beta1_msvc-wx32-10-x86.tar.gz) |
| **Linux** (Debian/Ubuntu 64-bit) | [1tracker-v0.1.0-beta1_debian12-x86_64.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-beta/raw/names/1tracker-v0.1.0-beta1-debian-x86_64-12-tarball/versions/v0.1.0-beta1/1tracker-v0.1.0-beta1_debian-x86_64-12-x86_64.tar.gz) |
| **Android** (64-bit, most phones) | [1tracker-v0.1.0-beta1_android-arm64.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-beta/raw/names/1tracker-v0.1.0-beta1-android-arm64-A64-16-tarball/versions/v0.1.0-beta1/1tracker-v0.1.0-beta1_android-arm64-16-arm64.tar.gz) |
| **Android** (32-bit, older phones) | [1tracker-v0.1.0-beta1_android-armhf.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-beta/raw/names/1tracker-v0.1.0-beta1-android-armhf-A32-16-tarball/versions/v0.1.0-beta1/1tracker-v0.1.0-beta1_android-armhf-16-armhf.tar.gz) |

---

## Step 2 — Install the plugin

1. Open OpenCPN
2. Go to **Options** → **Plugins** tab
3. Click **Install plugin from file**
4. Select the downloaded `.tar.gz` file
5. Restart OpenCPN when prompted

---

## Step 3 — Enable the plugin

1. Go back to **Options** → **Plugins**
2. Find **1tracker** in the list and click **Enable**
3. Click **OK** and restart OpenCPN

---

## Step 4 — Configure for NoForeignLand

After restarting OpenCPN a new **1tracker** icon appears in the toolbar.

### The toolbar icon

The icon doubles as a status indicator — its color reflects the result of the
most recent send attempt across all enabled endpoints:

| Color | Meaning |
|---|---|
| 🟢 Green | Last send succeeded |
| 🔴 Red | Last send failed |
| ⚪ Grey | Inactive or nothing sent yet |

Click the icon at any time to open the configuration window.

### Get your NoForeignLand boat key

Before configuring the endpoint you need a boat key from NoForeignLand:

1. Log in at [noforeignland.com](https://www.noforeignland.com/)
2. Go to **Boat Tracking Settings** → **API Key**
3. Create (or copy) the key for the boat you want to track

### Add the NoForeignLand endpoint

1. Click the **1tracker icon** in the toolbar to open the configuration window
2. Click **Add endpoint**
3. Fill in the fields:

| Field | Value |
|---|---|
| **Name** | Free choice, e.g. `nfl-my-boat` |
| **Type** | `noforeignland` |
| **My NFL boat key** | The API key you copied from NoForeignLand |
| **Send interval** | Minutes between sends (minimum 10, default 15) |

The URL, timeout and header name are filled in automatically for the
NoForeignLand type — you do not need to change them.

4. Set the endpoint toggle to **On**
5. Click **Save**

Your position is now being sent to NoForeignLand on every interval tick while
OpenCPN is running and has a GPS fix.

---

## Step 5 — Configure another endpoint (optional)

You can add as many endpoints as you like; each one is sent to independently
with its own type, URL, credentials, and interval. Typical reasons to add more
endpoints:

- Track on a **second NoForeignLand boat** (e.g. a tender or a shared account)
- Send the same fixes to **your own server** or a third-party service that
  accepts JSON over HTTP
- Mirror traffic to a **test endpoint** while you are experimenting

### Add another NoForeignLand endpoint

Repeat Step 4 with a different name and a different boat key. Both endpoints
will run on their own schedules.

### Add a generic JSON endpoint

1. Open the configuration window from the toolbar icon
2. Click **Add endpoint**
3. Fill in:

| Field | Value |
|---|---|
| **Name** | Free choice, e.g. `my-server` |
| **Type** | `http_json_with_header_key` |
| **URL** | The full HTTPS URL that will receive the JSON payload |
| **Header name** | Name of the auth header (e.g. `Authorization`, `X-API-Key`) |
| **Header value** | The token or key the server expects |
| **Send interval** | Minutes between sends (default 1) |
| **Timeout** | Request timeout in seconds |

4. Set the toggle to **On**
5. Click **Save**

Every enabled endpoint is sent to on its own schedule. The toolbar icon color
reflects the worst result across them all, so a single failing endpoint will
turn the icon red even if the others succeeded.

---

## Issues or feedback?

Please report bugs and comments on GitHub:
[github.com/pa2wlt/1tracker_pi/issues](https://github.com/pa2wlt/1tracker_pi/issues)

Or contact the developer directly.

---

*Version 0.1.0-beta1 — April 2026*
