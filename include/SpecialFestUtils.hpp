#pragma once

#include <string>
#include <vector>
#include <time.h>

#include "Downloader.hpp"
#include "Urls.hpp"

#ifdef WIIU
#include <mxml.h>
#else
#include <libxml/parser.h>
#endif


namespace SpecialFestUtils
{
    class SpecialFestInstaller
    {
    private:
        struct SpecialFileData
        {
            std::string sourceURL;
            std::string destPath;
            uint32_t crc32;
        };

        void GetFestInfoXML();
        void ParseXML();

        // XML String
        std::string m_xml;
        
        std::vector<SpecialFileData> m_specialFiles;
        time_t m_startTime;
        time_t m_endTime;
        bool m_isFestSpecial;

    public:
        SpecialFestInstaller();
        ~SpecialFestInstaller();

        // Steps:
        // 1. Get fest info xml
        // 2. Parse xml to get custom file list
        // 3. Download custom files
        // 4. Check the crc32 of custom files

        // Steps:
        // 1. Download Fest info & parse xml
        // 2. Check if we are within the start and end times
        // 3. If we are, check if splatfest is special
        // 4. If true, download the special mod files.
        // 5. Otherwise, do not download the special splatfest files.
        // If we are past the endTime, we can remove the special fest files.
        // (If we are not in the open period, we can also remove the special fest files.)

        void GetLatestFestInfo();
        bool IsDownloadActive();
        bool IsSpecialFest();

    };
}