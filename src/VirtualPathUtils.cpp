#include "VirtualPathUtils.hpp"

namespace VirtualPathUtils
{

    uint8_t MAX_VIRTUAL_PARTITIONS = 0;
    VIRTUAL_PARTITION *VIRTUAL_PARTITIONS = NULL;

    void VirtualMountDevice(const char *path)
    {
        if (!path)
            return;

        int i = 0;
        char name[255];
        char alias[255];
        char prefix[255];
        bool namestop = false;

        alias[0] = '/';

        do
        {
            if (path[i] == ':')
                namestop = true;

            if (!namestop)
            {
                name[i] = path[i];
                name[i + 1] = '\0';
                alias[i + 1] = path[i];
                alias[i + 2] = '\0';
            }

            prefix[i] = path[i];
            prefix[i + 1] = '\0';
            i++;
        } while (path[i - 1] != '/');

        AddVirtualPath(name, alias, prefix);
    }

    void AddVirtualPath(const char *name, const char *alias, const char *prefix)
    {
        if (!VIRTUAL_PARTITIONS)
            VIRTUAL_PARTITIONS = (VIRTUAL_PARTITION *)malloc(sizeof(VIRTUAL_PARTITION));

        VIRTUAL_PARTITION *tmp = (VIRTUAL_PARTITION*)realloc(VIRTUAL_PARTITIONS, sizeof(VIRTUAL_PARTITION) * (MAX_VIRTUAL_PARTITIONS + 1));
        if (!tmp)
        {
            free(VIRTUAL_PARTITIONS);
            MAX_VIRTUAL_PARTITIONS = 0;
            return;
        }

        VIRTUAL_PARTITIONS = tmp;

        VIRTUAL_PARTITIONS[MAX_VIRTUAL_PARTITIONS].name = strdup(name);
        VIRTUAL_PARTITIONS[MAX_VIRTUAL_PARTITIONS].alias = strdup(alias);
        VIRTUAL_PARTITIONS[MAX_VIRTUAL_PARTITIONS].prefix = strdup(prefix);
        VIRTUAL_PARTITIONS[MAX_VIRTUAL_PARTITIONS].inserted = true;

        MAX_VIRTUAL_PARTITIONS++;
    }

    void MountVirtualDevices()
    {
        VirtualMountDevice("sd:/");
        VirtualMountDevice("slccmpt01:/");
        VirtualMountDevice("storage_odd_tickets:/");
        VirtualMountDevice("storage_odd_updates:/");
        VirtualMountDevice("storage_odd_content:/");
        VirtualMountDevice("storage_odd_content2:/");
        VirtualMountDevice("storage_slc:/");
        VirtualMountDevice("storage_mlc:/");
        VirtualMountDevice("storage_usb:/");
        VirtualMountDevice("usb:/");
    }

    void UnmountVirtualPaths()
    {
        uint32_t i = 0;
        for (i = 0; i < MAX_VIRTUAL_PARTITIONS; i++)
        {
            if (VIRTUAL_PARTITIONS[i].name)
                free(VIRTUAL_PARTITIONS[i].name);
            if (VIRTUAL_PARTITIONS[i].alias)
                free(VIRTUAL_PARTITIONS[i].alias);
            if (VIRTUAL_PARTITIONS[i].prefix)
                free(VIRTUAL_PARTITIONS[i].prefix);
        }

        if (VIRTUAL_PARTITIONS)
            free(VIRTUAL_PARTITIONS);
        VIRTUAL_PARTITIONS = NULL;
        MAX_VIRTUAL_PARTITIONS = 0;
    }


    // MLC

    int mFSA = -1;
    bool mMLCMounted = false;
    bool mUSBMounted = false;

    #ifdef WIIU
    void MountMLC()
    {
        mount_fs("storage_mlc", mFSA, NULL, "/vol/storage_mlc01");
        VirtualMountDevice("storage_mlc:/");
        mMLCMounted = true;

        mount_fs("storage_usb", mFSA, NULL, "/vol/storage_usb01");
        VirtualMountDevice("storage_usb:/");

        if (opendir("storage_usb:/") != NULL)
            mUSBMounted = true;
    }

    void UnmountMLC()
    {
        if (mMLCMounted)
        {
            unmount_fs("storage_mlc");
            // VirtualUnmountPath("storage_mlc:/");
            mMLCMounted = false;
        }

        if (mUSBMounted)
        {
            unmount_fs("storage_usb");
            // VirtualUnmountPath("storage_usb:/");
            mUSBMounted = false;
        }
    }
    #else
    void MountMLC()
    {
        mMLCMounted = true;
    }

    void UnmountMLC()
    {
        mMLCMounted = false;
    }
    #endif
}