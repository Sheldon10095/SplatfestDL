#include "FsUtils.hpp"

namespace FsUtils
{
    int RemoveFile(const char *fileName)
    {
        #ifdef WIIU
        return std::filesystem::remove(fileName);
        #else
        return std::filesystem::remove(fileName);
        #endif
    }

    int RenameFile(const char *oldFileName, const char *newFileName)
    {
        std::filesystem::rename(oldFileName, newFileName);
        return 0;
    }

    int CopyFile(const char *srcFileName, const char *dstFileName)
    {
        #ifdef WIIU
        std::ifstream src(srcFileName, std::ios::binary);
        std::ofstream dst(dstFileName, std::ios::binary);
        
        std::streamsize bufferSize = 2 * 1024 * 1024; // 2 MB
        char buffer[bufferSize];
        
        while (src)
        {
            src.read(buffer, bufferSize);
            dst.write(buffer, src.gcount());
        }

        return 1;
        #else
        return std::filesystem::copy_file(srcFileName, dstFileName); // Doesn't work on WiiU
        #endif
    }
    int CopyFile(std::string srcFileName, std::string dstFileName)
    {
        std::filesystem::copy_file(srcFileName, dstFileName);
        // std::filesystem::copy(srcFileName.c_str(), dstFileName.c_str());
        return 0;
    }

    bool CheckFileExists(const char *fileName)
    {
        return std::filesystem::exists(fileName);
    }


    int CreateDir(const char *dirName)
    {
        std::filesystem::create_directory(dirName);
        return 0;
    }

    bool CheckDirExists(const char *path)
    {
        // if (!std::filesystem::is_directory(path))
        //     return false;
        // return std::filesystem::exists(path);
        
        return std::filesystem::is_directory(path);
    }

    bool EnsureDirExists(const char *path)
    {
        if (CheckDirExists(path))
            return true;

        CreateDir(path);
        return CheckDirExists(path);
    }

    bool IsDirEmpty(const char *path)
    {
        if (!CheckDirExists(path))
            return false;

        return std::filesystem::is_empty(path);
    }

    bool SafeRemoveDir(const char *dirName)
    {
        if (!CheckDirExists(dirName)) return false;

        try
        {
            std::filesystem::remove(dirName);
        }
        catch (std::filesystem::filesystem_error &e)
        {
            ScrUtils::printf("[Warning] %s\n", e.what());
            return false;
        }
        return true;
    }

    unsigned int RemoveDirRecursive(const char *dirName)
    {
        if (!CheckDirExists(dirName)) return 0;
        
        unsigned int count = 0;
        try {
            #ifdef WIIU
            
            for (auto &path : std::filesystem::directory_iterator(dirName))
            {
                std::filesystem::remove(path);
                count++;
            }
            std::filesystem::remove(dirName) ? count++ : 0;
            
            #else
            count = std::filesystem::remove_all(dirName);
            #endif
        } 
        catch (std::filesystem::filesystem_error &e) {
            ScrUtils::printf("[Warning] %s\n", e.what());
            return 0;
        }

        return count;
    }


    /* WiiU functions */
    namespace MCP
    {
        static int mcp_hook_fd = -1;
        
        bool MCPHookOpen()
        {
            #ifdef WIIU
            // take over mcp thread
            mcp_hook_fd = MCP_Open();
            if (mcp_hook_fd < 0)
                return false;
            // IOS_IoctlAsync(mcp_hook_fd, 0x62, (void*)0, 0, (void*)0, 0, (IOSAsyncCallbackFn)someFunc, (void*)0);
            if (IOSUHAX_Open("/dev/mcp") < 0)
            {
                MCP_Close(mcp_hook_fd);
                mcp_hook_fd = false;
                return false;
            }
            #endif
            return true;
        }

        void MCPHookClose()
        {
            #ifdef WIIU
            if (mcp_hook_fd < 0)
                return;
            // close down wupserver, return control to mcp
            IOSUHAX_Close();
            // wait for mcp to return
            sleep(1);
            MCP_Close(mcp_hook_fd);
            mcp_hook_fd = -1;
            #endif
        }
    }


    // IOSU

    bool openedIOSU = false;

    bool IsExploitedIOSU()
    {
        return openedIOSU;
    }

    bool OpenIOSU()
    {
        if (IsExploitedIOSU())
            return true;

        #ifdef WIIU
        int res = IOSUHAX_Open(NULL);
        if (res < 0) // res = MCPHookOpen();
            res = MCP::MCPHookOpen();
        if (!res)
        {
            ScrUtils::printf("[Error] Failed to open IOSU\n");
            return false;
        }
        
        VirtualPathUtils::mFSA = IOSUHAX_FSA_Open();
        if (VirtualPathUtils::mFSA < 0)
        {
            CloseIOSU(); // Don't leave IOSU open if we can't open the FSA (?)

            ScrUtils::printf("[Error] Failed to open FSA\n");
            return false;
        }
        #endif

        openedIOSU = true;
        // ScrUtils::printf("[Info] IOSU opened\n");
        return true;
    }

    bool CloseIOSU()
    {
        #ifdef WIIU
        if (!IsExploitedIOSU()) // No need to close if it isn't open
            return false;

        if (VirtualPathUtils::mFSA != 0)
            IOSUHAX_FSA_Close(VirtualPathUtils::mFSA);

        if (VirtualPathUtils::mUSBMounted)
            unmount_fs("storage_usb:/");

        if (VirtualPathUtils::mMLCMounted)
            unmount_fs("storage_mlc:/");

        MCP::MCPHookClose();
        IOSUHAX_Close();
        #endif

        // ScrUtils::printf("[Info] IOSU closed\n");
        openedIOSU = false;
        return true;
    }


    #ifdef WIIU
    bool IsMLCMounted() {
        return VirtualPathUtils::mMLCMounted;
    }

    bool IsUSBMounted() {
        return VirtualPathUtils::mUSBMounted;
    }
    #else    
    bool IsMLCMounted() {
        return false;
    }

    bool IsUSBMounted() {
        return false;
    }
    #endif


    /* Misc Functions */

    uint32_t GetFileCrc32(const char *filePath)
    {
        uint32_t crc = 0;
        FILE *f = fopen(filePath, "rb");
        if (!f)
            return 0;

        // Get file size
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        fseek(f, 0, SEEK_SET);

        // Read file
        uint8_t *buf = new uint8_t[size];
        fread(buf, 1, size, f);
        fclose(f);

        // Calculate CRC32
        crc = crc32(crc, buf, size);
        delete[] buf;

        return crc;
    }
}