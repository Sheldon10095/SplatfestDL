#include "ScrUtils.hpp"

namespace ScrUtils
{
    // extern uint32_t g_ConsoleColor;

    int printf(const char *format, ...)
    {
        uint bufSize = 4096;

        #ifdef WIIU
        va_list args;
        va_start(args, format);
        char buffer[bufSize];
        vsnprintf(buffer, bufSize, format, args);
        va_end(args);
        // WHBLogPrint(buffer);
        buffer[bufSize - 1] = '\0';
        WHBLogWrite(buffer);
        WHBLogConsoleSetColor(0x008040FF);
        WHBLogConsoleDraw();
        return 0;
        
        #else
        int ret;
        va_list args;
        va_start(args, format);
        ret = vprintf(format, args);
        va_end(args);
        return ret;
        #endif
    }

    bool waitForKeyPress()
    {
        #ifdef WIIU

        VPADStatus vpad;
        VPADReadError err;
        while (true)
        {
            VPADRead(VPAD_CHAN_0, &vpad, 1, &err);
            if (err == VPAD_READ_SUCCESS)
            {
                if (vpad.trigger & VPAD_BUTTON_A)
                    return true;
                // if (vpad.trigger & VPAD_BUTTON_HOME)
                //     return false;
            }
        }

        #else
        
        // std::cout << "Press any key to continue..." << std::endl;
        std::cin.get();
        return false;
        
        #endif
    }

    void AskForAPressToContinue()
    {
        ScrUtils::printf("Press A to proceed.\n");
        waitForKeyPress();
    }

    bool AskForYesNo(const char *question)
    {
        #ifdef WIIU
        ScrUtils::printf("%s (A=OK, B=No)", question);

        VPADStatus vpad;
        VPADReadError err;
        while (true)
        {
            VPADRead(VPAD_CHAN_0, &vpad, 1, &err);
            if (err == VPAD_READ_SUCCESS)
            {
                if (vpad.trigger & VPAD_BUTTON_A)
                    return true;
                if (vpad.trigger & VPAD_BUTTON_B)
                    return false;
            }
        }
        #else
        ScrUtils::printf("%s (Y/N): ", question);
        char c = std::cin.get();
        return c == 'Y' || c == 'y';
        #endif
    }
}