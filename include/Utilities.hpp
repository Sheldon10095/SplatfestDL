#pragma once

#include <string>
#include "Downloader.hpp"
#include "FsUtils.hpp"
#include "Urls.hpp"

namespace Utilities
{
    bool CheckMaintenance();
    int GetLatestVersionNum();
    void DownloadLatestVersion();
    bool DownloadModLoader();
}