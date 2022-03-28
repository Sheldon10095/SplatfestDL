#pragma once

#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

#ifdef WIIU
#include <whb/log.h>
#include <whb/log_console.h>
#include <vpad/input.h>
#include <whb/log_udp.h>
#else
#include <iostream>
#endif

namespace ScrUtils
{
    int printf(const char *format, ...);
    bool waitForKeyPress();
    void AskForAPressToContinue();
    bool AskForYesNo(const char *question);
}