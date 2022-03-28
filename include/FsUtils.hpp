#pragma once

// #ifndef WIIU
// #else
// #endif

#include <filesystem>
#include <iostream>
#include <zlib.h>
#include "ScrUtils.hpp"

#ifdef WIIU
#include <whb/file.h>
#include <whb/sdcard.h>
#include <iosuhax.h>
#include <coreinit/mcp.h>
#include "VirtualPathUtils.hpp"
#include <iosfwd>
#include <fstream>
#include <mxml.h>
#endif

namespace FsUtils
{
    /* File functions */

    int RemoveFile(const char *fileName);                               // Remove a file
    int RenameFile(const char *oldFileName, const char *newFileName);   // Rename / move a file
    int CopyFile(const char *srcFileName, const char *dstFileName);     // Copy a file
    bool CheckFileExists(const char *fileName);                         // Check if a file exists
    
    
    /* Directory functions */

    int CreateDir(const char *dirName);                           // Create a directory
    bool CheckDirExists(const char *path);                        // Check if a directory exists
    bool EnsureDirExists(const char *path);                       // Ensure a directory exists
    bool IsDirEmpty(const char *path);                            // Check if a directory is empty
    bool SafeRemoveDir(const char *dirName);                      // Remove a directory (if empty)
    unsigned int RemoveDirRecursive(const char *dirName);         // Remove a directory recursively


    /* WiiU functions */

    bool IsExploitedIOSU(); // Check if the IOSU exploit has been applied
    bool OpenIOSU();        // Open FileSystem Access
    bool CloseIOSU();       // Close FileSystem Access
    bool IsMLCMounted();    // Check if the MLC is mounted
    bool IsUSBMounted();    // Check if the USB is mounted


    /* Misc functions */

    uint32_t GetFileCrc32(const char *fileName);    // Get the CRC32 of a file
}