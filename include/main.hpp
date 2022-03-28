#include <iostream>

#ifdef WIIU
#include <wut.h>
#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>
#include <iosuhax.h>
#include <coreinit/mcp.h>
#endif

#include "Downloader.hpp"
#include "FestUtils.hpp"
#include "SpecialFestUtils.hpp"
#include "FsUtils.hpp"      // FileSystem Utilities
#include "ScrUtils.hpp"     // Screen Utilities
#include "Utilities.hpp"    // Utilities (maintenance, version, etc)
#include "Urls.hpp"