#pragma once

#ifdef WIIU
#include <sysapp/title.h>
#endif
#include "FsUtils.hpp"

namespace FestUtils
{
    struct FestFileIdx
    {
        uint32_t festivalByamlID;
        uint32_t panelTextureID;
        uint32_t hapTextureID;
    };

    extern const char* g_TempPath;

    class FestInstaller
    {
    private:
        // bool InstallFestToTitle(uint64_t titleID);
        bool InstallFestToTitle(uint64_t titleID, FestFileIdx idxStruct);

        uint8_t m_InstalledGameVersions; // Installed versions (bitfield)

    public:
        FestInstaller();
        ~FestInstaller();

        /* Maybe move this to private? */
        void FindInstalledVersions(); // Find installed versions and set m_InstalledGameVersions
        static bool IsFoundUSA();     // Check if USA is installed
        static bool IsFoundEUR();     // Check if EUR is installed
        static bool IsFoundJPN();     // Check if JPN is installed

        bool InstallFest();   // Install Splatfest to found versions
        bool UninstallFest(); // Uninstall Splatfest from found versions
    };
}