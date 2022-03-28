#include "FestUtils.hpp"

uint32_t GetHighBytes(uint64_t titleID) {
    return (uint32_t)(titleID >> 32);
}

uint32_t GetLowBytes(uint64_t titleID) {
    return (uint32_t)(titleID & 0xFFFFFFFF);
}
    
namespace FestUtils
{
    const char* g_TempPath = "sfdltemp/";

    enum Version : uint8_t
    {
        Version_USA = 1 << 0,
        Version_EUR = 1 << 1,
        Version_JPN = 1 << 2,
    };

    constexpr uint64_t TitleID_USA = 0x0005000010176900;
    constexpr uint64_t TitleID_EUR = 0x0005000010176A00;
    constexpr uint64_t TitleID_JPN = 0x0005000010162B00;


    FestInstaller::FestInstaller()
    {
        FindInstalledVersions();
    }

    FestInstaller::~FestInstaller()
    {
    }

    void FestInstaller::FindInstalledVersions()
    {
        #ifdef WIIU
        m_InstalledGameVersions = 0;
        if (IsFoundUSA())
            m_InstalledGameVersions |= Version_USA;
        if (IsFoundEUR())
            m_InstalledGameVersions |= Version_EUR;
        if (IsFoundJPN())
            m_InstalledGameVersions |= Version_JPN;
        #else
        m_InstalledGameVersions =  Version_USA | Version_EUR | Version_JPN;
        #endif
    }

    bool FestInstaller::IsFoundUSA()
    {
        #ifdef WIIU
        return SYSCheckTitleExists(TitleID_USA);
        #else
        return true;
        #endif
    }

    bool FestInstaller::IsFoundEUR()
    {
        #ifdef WIIU
        return SYSCheckTitleExists(TitleID_EUR);
        #else
        return true;
        #endif
    }

    bool FestInstaller::IsFoundJPN()
    {
        #ifdef WIIU
        return SYSCheckTitleExists(TitleID_JPN);
        #else
        return true;
        #endif
    }


    constexpr FestFileIdx festFileIdx_USA {
        .festivalByamlID = 0x00000544,
        .panelTextureID = 0x00000545,
        .hapTextureID = 0x00000543,
    };

    constexpr FestFileIdx festFileIdx_EUR {
        .festivalByamlID = 0x0000054d,
        .panelTextureID = 0x0000054c,
        .hapTextureID = 0x0000054e,
    };

    constexpr FestFileIdx festFileIdx_JPN {
        .festivalByamlID = 0x000005d2,
        .panelTextureID = 0x000005d4,
        .hapTextureID = 0x000005d3,
    };



    void inline GetFestFilePathFromStorage(char* buffer, const char* storagePath, uint64_t titleID, uint32_t fileID)
    {
        snprintf(buffer, 0x100, "%s:/usr/boss/%08X/%08X/user/common/data/optdat2/%08x", 
                storagePath, GetHighBytes(titleID), GetLowBytes(titleID), fileID);
    }
    void GetMLCFestFilePath(char* buffer, uint64_t titleID, uint32_t fileID) {
        GetFestFilePathFromStorage(buffer, "storage_mlc", titleID, fileID);
    }
    void GetUSBFestFilePath(char* buffer, uint64_t titleID, uint32_t fileID) {
        GetFestFilePathFromStorage(buffer, "storage_usb", titleID, fileID);
    }

    bool CheckForFestFilesInStorage(char* storage, uint64_t titleID, FestFileIdx idxStruct)
    {
        bool allFound = true;
        char path[0x100];
        GetFestFilePathFromStorage(path, storage, titleID, idxStruct.festivalByamlID);
        // ScrUtils::printf("Checking for %s\n", path);

        if (!FsUtils::CheckFileExists(path))
            allFound = false;

        GetFestFilePathFromStorage(path, storage, titleID, idxStruct.panelTextureID);
        if (!FsUtils::CheckFileExists(path))
            allFound = false;

        GetFestFilePathFromStorage(path, storage, titleID, idxStruct.hapTextureID);
        if (!FsUtils::CheckFileExists(path))
            allFound = false;

        return allFound;
    }


    bool FestInstaller::InstallFestToTitle(uint64_t titleID, FestFileIdx idxStruct)
    {
        // Check if IOSU has been exploited
        if (!FsUtils::IsExploitedIOSU()) {
            ScrUtils::printf("IOSU has not been exploited.\n");
            return false;
        }
    
        // ScrUtils::printf("Full titleID: %016llX\n", titleID);
        // ScrUtils::printf("High bytes: %08X\n", GetHighBytes(titleID));
        // ScrUtils::printf("Low bytes:  %08X\n", GetLowBytes(titleID));
        // ScrUtils::printf("Festival Byaml ID: %08x\n", idxStruct.festivalByamlID);
        // ScrUtils::printf("Panel Texture ID:  %08x\n", idxStruct.panelTextureID);
        // ScrUtils::printf("Hap Texture ID:    %08x\n", idxStruct.hapTextureID);
        // ScrUtils::AskForAPressToContinue();

        // Check if USB storage is mounted before checking MLC
        // If not, just check path on MLC
        enum StorageType {
            StorageType_NOT_FOUND,
            StorageType_USB,
            StorageType_MLC,
        };

        StorageType storageType = StorageType_NOT_FOUND;
        bool isFestOnUSB = false;

        if (FsUtils::IsUSBMounted()) // USB storage mounted
        {
            // ScrUtils::printf("USB storage is mounted.\n");
            if (CheckForFestFilesInStorage("storage_usb", titleID, idxStruct))
            {
                // ScrUtils::printf("Files found on USB storage.\n");
                isFestOnUSB = true;
                storageType = StorageType_USB;  // Splatfest is on USB
            }
        }
        if (!isFestOnUSB) // Fest is not on USB, check MLC
        {
            if (CheckForFestFilesInStorage("storage_mlc", titleID, idxStruct))
            {
                // ScrUtils::printf("Files found on MLC storage.\n");
                storageType = StorageType_MLC; // Splatfest is on MLC
            }
            else // Files not found anywhere
            {
                ScrUtils::printf("Splatfest Files couldn't be found.\n");
                ScrUtils::printf("Have you launched the game yet?\n");
            }
        }

        const char* storageDevice = "";
        switch (storageType)
        {
            case StorageType_USB:
                storageDevice = "storage_usb";
                break;
            case StorageType_MLC:
                storageDevice = "storage_mlc";
                break;
            case StorageType_NOT_FOUND:
                ScrUtils::printf("Splatfest Files couldn't be found anywhere.\n");
                return false;
                break;
        }

        // Copy from SD temp folder to storage

        char path[0x100];
        std::string sdPathBase = g_TempPath;
        std::string sdPath = sdPathBase;

        // ScrUtils::printf("Copying...\n");

        GetFestFilePathFromStorage(path, storageDevice, titleID, idxStruct.festivalByamlID);
        sdPath = sdPathBase + "Festival.byaml";
        FsUtils::CopyFile(sdPath.c_str(), path);

        GetFestFilePathFromStorage(path, storageDevice, titleID, idxStruct.panelTextureID);
        sdPath = sdPathBase + "PanelTexture.bfres";
        FsUtils::CopyFile(sdPath.c_str(), path);

        GetFestFilePathFromStorage(path, storageDevice, titleID, idxStruct.hapTextureID);
        sdPath = sdPathBase + "HapTexture.bfres";
        FsUtils::CopyFile(sdPath.c_str(), path);

        // ScrUtils::printf("Done!\n");
        return true;
    }

    bool FestInstaller::InstallFest()
    {
        if (m_InstalledGameVersions == 0) // No games found
            return false;

        if (m_InstalledGameVersions & Version_USA)
            InstallFestToTitle(TitleID_USA, festFileIdx_USA);
        if (m_InstalledGameVersions & Version_EUR)
            InstallFestToTitle(TitleID_EUR, festFileIdx_EUR);
        if (m_InstalledGameVersions & Version_JPN)
            InstallFestToTitle(TitleID_JPN, festFileIdx_JPN);

        return true;
    }
}