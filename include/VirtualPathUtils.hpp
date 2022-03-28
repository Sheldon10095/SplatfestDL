#pragma once

#include <stdint.h>
#include <string.h>
#include <dirent.h>

#ifdef WIIU
#include <malloc.h>
#include <iosuhax_devoptab.h>
#else
#include <stdlib.h>
#endif

namespace VirtualPathUtils
{
    typedef struct
    {
        char *name;
        char *alias;
        char *prefix;
        bool inserted;
    } VIRTUAL_PARTITION;

    extern VIRTUAL_PARTITION *VIRTUAL_PARTITIONS;
    extern uint8_t MAX_VIRTUAL_PARTITIONS;

    void VirtualMountDevice(const char *devicepath);
    void AddVirtualPath(const char *name, const char *alias, const char *prefix);
    void MountVirtualDevices();
    void UnmountVirtualPaths();

    
    // MLC
    
    // global variables
    extern int mFSA;
    extern bool mMLCMounted;
    extern bool mUSBMounted;

    void MountMLC();
    void UnmountMLC();
}