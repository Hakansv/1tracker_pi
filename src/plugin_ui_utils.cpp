#include "plugin_ui_utils.h"

#include <filesystem>
#include <vector>

#if wxCHECK_VERSION(3,1,6)
#include <wx/bmpbndl.h>
#endif
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/platform.h>

#include "ocpn_plugin.h"

namespace tracker_plugin_ui {
namespace {

const char* kPluginName = "1tracker_pi";

wxString existingAssetPath(const std::filesystem::path& path) {
  std::error_code ec;
  return std::filesystem::exists(path, ec)
             ? wxString::FromUTF8(path.string().c_str())
             : wxString("");
}

std::vector<std::filesystem::path> pluginAssetSearchRoots() {
  std::vector<std::filesystem::path> roots;
  try {
    const wxString pluginDataDir = GetPluginDataDir(kPluginName);
    if (!pluginDataDir.empty()) {
      roots.emplace_back(pluginDataDir.ToStdString());
    }
  } catch (...) {
    // GetPluginDataDir can return unexpected data on some OCPN hosts; if
    // constructing the path throws, skip this root rather than crash.
  }

  // The source-tree fallback uses __FILE__, which on a release build can be
  // a path that no longer exists (or that contains characters that trip
  // std::filesystem on some locales). Never let that take the process down.
  try {
    const std::filesystem::path sourceDataDir =
        std::filesystem::path(__FILE__).parent_path().parent_path() / "data" /
        "icons";
    std::error_code ec;
    if (std::filesystem::exists(sourceDataDir, ec)) {
      roots.push_back(sourceDataDir);
    }
  } catch (...) {
    // Ignored by design.
  }
  return roots;
}

std::vector<std::filesystem::path> candidateAssetPaths(
    const std::filesystem::path& root, const std::filesystem::path& fileNamePath) {
  return {root / "data" / fileNamePath, root / "data" / "icons" / fileNamePath,
          root / fileNamePath};
}

}  // namespace

bool IsWindowsPlatform() {
  return wxPlatformInfo::Get().GetOperatingSystemId() & wxOS_WINDOWS;
}

wxBitmap TintBitmap(const wxBitmap& bitmap, const wxColour& colour) {
  if (!bitmap.IsOk()) {
    return wxNullBitmap;
  }

  wxImage image = bitmap.ConvertToImage();
  if (!image.IsOk()) {
    return wxNullBitmap;
  }

  if (!image.HasAlpha()) {
    image.InitAlpha();
  }

  unsigned char* data = image.GetData();
  unsigned char* alpha = image.GetAlpha();
  if (data == nullptr) {
    return wxNullBitmap;
  }

  const int pixelCount = image.GetWidth() * image.GetHeight();
  for (int i = 0; i < pixelCount; ++i) {
    if (alpha != nullptr && alpha[i] == 0) {
      continue;
    }
    data[i * 3] = colour.Red();
    data[i * 3 + 1] = colour.Green();
    data[i * 3 + 2] = colour.Blue();
  }

  return wxBitmap(image);
}

wxString FindPluginAssetPath(const wxString& fileName) {
  const auto roots = pluginAssetSearchRoots();
  const std::filesystem::path fileNamePath(fileName.ToStdString());
  for (const auto& root : roots) {
    for (const auto& candidate : candidateAssetPaths(root, fileNamePath)) {
      const wxString path = existingAssetPath(candidate);
      if (!path.empty()) {
        return path;
      }
    }
  }

  return "";
}

wxBitmap LoadBitmapFromPluginAsset(const wxString& fileName, const wxSize& size) {
  const wxString path = FindPluginAssetPath(fileName);
  if (path.empty()) {
    return wxNullBitmap;
  }

  wxFileName assetPath(path);
  if (assetPath.GetExt().CmpNoCase("svg") == 0) {
#if wxCHECK_VERSION(3,1,6)
    return wxBitmapBundle::FromSVGFile(path, size).GetBitmap(size);
#else
    return wxNullBitmap;
#endif
  }

  wxImage image;
  if (!image.LoadFile(path)) {
    return wxNullBitmap;
  }
  if (size.IsFullySpecified() && size.GetWidth() > 0 && size.GetHeight() > 0 &&
      (image.GetWidth() != size.GetWidth() ||
       image.GetHeight() != size.GetHeight())) {
    image.Rescale(size.GetWidth(), size.GetHeight(), wxIMAGE_QUALITY_HIGH);
  }
  return wxBitmap(image);
}

wxBitmap LoadBitmapFromPluginAssetWidth(const wxString& fileName,
                                        int targetWidth) {
  const wxString path = FindPluginAssetPath(fileName);
  if (path.empty() || targetWidth <= 0) {
    return wxNullBitmap;
  }

  wxFileName assetPath(path);
  if (assetPath.GetExt().CmpNoCase("svg") == 0) {
    return LoadBitmapFromPluginAsset(fileName, wxSize(targetWidth, targetWidth));
  }

  wxImage image;
  if (!image.LoadFile(path) || image.GetWidth() <= 0 || image.GetHeight() <= 0) {
    return wxNullBitmap;
  }

  const int targetHeight =
      std::max(1, (image.GetHeight() * targetWidth) / image.GetWidth());
  if (image.GetWidth() != targetWidth || image.GetHeight() != targetHeight) {
    image.Rescale(targetWidth, targetHeight, wxIMAGE_QUALITY_HIGH);
  }

  return wxBitmap(image);
}

}  // namespace tracker_plugin_ui
