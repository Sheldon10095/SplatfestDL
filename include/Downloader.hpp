#pragma once

#include <string>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <ScrUtils.hpp>

#ifdef WIIU
#include <openssl/ssl.h>
#include "WU_CAcert.h"
#endif


namespace Tools
{
    class Downloader
    {
    private:
        unsigned int m_maxStringSize;
    public:
        Downloader();
        ~Downloader();

        unsigned int GetMaxStringSize() const;
        void SetMaxStringSize(unsigned int maxStringSize);

        /**
         * @brief Download a file from a URL, and save it to a file.
         *
         * @param url
         * @param fileName
         * @return int
         */
        int DownloadToFile(std::string url, std::string fileName);

        /**
         * @brief Download content from a URL, and try to store it in str.
         * 
         * @param url Input URL
         * @param str Reference to string to store the content.
         * @return int
         */
        int DownloadToString(std::string url, std::string &str);
    };
}