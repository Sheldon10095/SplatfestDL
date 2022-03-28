#include "SpecialFestUtils.hpp"


// XML
/*
<!-- Needs a list of files, each with sourceURL, destPath, and CRC32 -->
<!-- If the file is split, the crc32 will be the CRC32 of the whole file -->
<root>
    <files>
        <file>
            <sourceURL>http://www.example.com/file1.txt</sourceURL>
            <destPath>file1.txt</destPath>
            <crc32>12345678</crc32>
        </file>
        <file>
            <sourceURL>http://www.example.com/file2.txt</sourceURL>
            <destPath>file2.txt</destPath>
            <crc32>12345678</crc32>
        </file>
        <file>
            <sourceURL>http://www.example.com/file3.txt</sourceURL>
            <destPath>file3.txt</destPath>
            <crc32>12345678</crc32>
        </file>
    </files>
    <info>
        <startTime>1648166400</startTime>
        <endTime>1648598400</endTime>
        <isSpecial>0</isSpecial>
    </info>
</root>
*/


namespace SpecialFestUtils
{
    SpecialFestInstaller::SpecialFestInstaller()
    {
        // m_specialFiles.clear();
        // m_isFestSpecial = false;
        // m_startTime = 0;
        // m_endTime = 0;
        // m_xml = "";
    }

    SpecialFestInstaller::~SpecialFestInstaller()
    {
    }

    void inline SpecialFestInstaller::GetFestInfoXML()
    {
        Tools::Downloader dl;
        std::string webResponse;
        
        dl.DownloadToString(URL_FEST_INFO_XML, webResponse);

        m_xml = webResponse;
    }

    void SpecialFestInstaller::ParseXML()
    {
        // Objectives:
        // Get the list of files to download
        // Get the start and end times
        // Get the isSpecial flag

        m_specialFiles.clear();

        #ifdef WIIU
        // Parse the XML using mxml
        mxml_node_t *tree = mxmlLoadString(NULL, m_xml.c_str(), MXML_TEXT_CALLBACK);
        if (tree == NULL)
        {
            return;
        }

        // Get the list of files
        mxml_node_t *files = mxmlFindElement(tree, tree, "files", NULL, NULL, MXML_DESCEND);
        if (files == NULL)
        {
            return;
        }

        // Get the list of files
        mxml_node_t *file = mxmlFindElement(files, files, "file", NULL, NULL, MXML_DESCEND);
        while (file != NULL)
        {
            SpecialFileData sfd;

            // Get the sourceURL
            mxml_node_t *sourceURL = mxmlFindElement(file, file, "sourceURL", NULL, NULL, MXML_DESCEND);
            if (sourceURL != NULL)
            {
                sfd.sourceURL = mxmlGetOpaque(sourceURL);
            }

            // Get the destPath
            mxml_node_t *destPath = mxmlFindElement(file, file, "destPath", NULL, NULL, MXML_DESCEND);
            if (destPath != NULL)
            {
                sfd.destPath = mxmlGetOpaque(destPath);
            }

            // Get the crc32
            mxml_node_t *crc32 = mxmlFindElement(file, file, "crc32", NULL, NULL, MXML_DESCEND);
            if (crc32 != NULL)
            {
                sfd.crc32 = strtoul(mxmlGetOpaque(crc32), NULL, 0);
            }

            m_specialFiles.push_back(sfd);

            file = mxmlFindElement(file, files, "file", NULL, NULL, MXML_DESCEND_FIRST);
        }

        // Get the startTime
        mxml_node_t *info = mxmlFindElement(tree, tree, "info", NULL, NULL, MXML_DESCEND);
        if (info != NULL)
        {
            mxml_node_t *startTime = mxmlFindElement(info, info, "startTime", NULL, NULL, MXML_DESCEND);
            if (startTime != NULL)
            {
                m_startTime = strtoul(mxmlGetOpaque(startTime), NULL, 0);
            }

            mxml_node_t *endTime = mxmlFindElement(info, info, "endTime", NULL, NULL, MXML_DESCEND);
            if (endTime != NULL)
            {
                m_endTime = strtoul(mxmlGetOpaque(endTime), NULL, 0);
            }

            mxml_node_t *isSpecial = mxmlFindElement(info, info, "isSpecial", NULL, NULL, MXML_DESCEND);
            if (isSpecial != NULL)
            {
                m_isFestSpecial = (strtoul(mxmlGetOpaque(isSpecial), NULL, 0) == 1);
            }
        }

        #else

        // Parse the XML using libxml
        xmlDocPtr doc = xmlParseMemory(m_xml.c_str(), m_xml.size());
        if (doc == NULL) {
            return;
        }

        // Root
        xmlNodePtr root = xmlDocGetRootElement(doc);
        if (root == NULL) {
            xmlFreeDoc(doc);
            return;
        }

        // Get the files element
        xmlNodePtr node = root->children;
        while (node != NULL)
        {
            if (xmlStrcmp(node->name, (const xmlChar*)"files") == 0)
            {
                // Get the file element
                xmlNodePtr file = node->children;
                while (file != NULL)
                {
                    SpecialFileData sfd;

                    // Get the sourceURL
                    xmlNodePtr sourceURL = file->children;
                    while (sourceURL != NULL)
                    {
                        if (xmlStrcmp(sourceURL->name, (const xmlChar*)"sourceURL") == 0)
                        {
                            sfd.sourceURL = (char*)xmlNodeGetContent(sourceURL);
                        }

                        sourceURL = sourceURL->next;
                    }

                    // Get the destPath
                    xmlNodePtr destPath = file->children;
                    while (destPath != NULL)
                    {
                        if (xmlStrcmp(destPath->name, (const xmlChar*)"destPath") == 0)
                        {
                            sfd.destPath = (char*)xmlNodeGetContent(destPath);
                        }

                        destPath = destPath->next;
                    }

                    // Get the crc32
                    xmlNodePtr crc32 = file->children;
                    while (crc32 != NULL)
                    {
                        if (xmlStrcmp(crc32->name, (const xmlChar*)"crc32") == 0)
                        {
                            sfd.crc32 = strtoul((char*)xmlNodeGetContent(crc32), NULL, 0);
                        }

                        crc32 = crc32->next;
                    }

                    m_specialFiles.push_back(sfd);

                    file = file->next;
                }
            }
            else if (xmlStrcmp(node->name, (const xmlChar*)"info") == 0)
            {
                // Get the startTime
                xmlNodePtr startTime = node->children;
                while (startTime != NULL)
                {
                    if (xmlStrcmp(startTime->name, (const xmlChar*)"startTime") == 0)
                    {
                        m_startTime = std::atoi((char*)xmlNodeGetContent(startTime));
                    }

                    startTime = startTime->next;
                }

                // Get the endTime
                xmlNodePtr endTime = node->children;
                while (endTime != NULL)
                {
                    if (xmlStrcmp(endTime->name, (const xmlChar*)"endTime") == 0)
                    {
                        m_endTime = std::atoi((char*)xmlNodeGetContent(endTime));
                    }

                    endTime = endTime->next;
                }

                // Get the isSpecial flag
                xmlNodePtr isSpecial = node->children;
                while (isSpecial != NULL)
                {
                    if (xmlStrcmp(isSpecial->name, (const xmlChar*)"isSpecial") == 0)
                    {
                        m_isFestSpecial = std::atoi((char*)xmlNodeGetContent(isSpecial)) > 0;
                    }

                    isSpecial = isSpecial->next;
                }
            }

            node = node->next;
        }
        #endif
    }

    void SpecialFestInstaller::GetLatestFestInfo()
    {
        GetFestInfoXML();
        ParseXML();
    }

    bool SpecialFestInstaller::IsDownloadActive()
    {
        // Check if the current time is between the start and end times
        time_t currentTime = time(NULL);
        // ScrUtils::printf("Current time: %d\n", currentTime);
        return (currentTime >= m_startTime && currentTime <= m_endTime);
    }

    bool SpecialFestInstaller::IsSpecialFest()
    {
        return m_isFestSpecial;
    }
}