#pragma once

// #include <string.h>

#define GITHUB_USER "Sheldon10095"
#define GITHUB_REPO "FestFilesTest"

#define URL_BASE "https://raw.githubusercontent.com/" GITHUB_USER "/" GITHUB_REPO "/master/"

#define URL_MAINTENANCE     URL_BASE "m"
#define URL_VERSION         URL_BASE "v"

// Splatfest Downloader
#define URL_SFDL_APPRPX          URL_BASE "app/SFDL/sfdl"
#define URL_SFDL_APPICON         URL_BASE "app/SFDL/icon.png"
#define URL_SFDL_APPMETA         URL_BASE "app/SFDL/meta.xml"

// Splatfest Modloader
#define URL_SFML_APPEXEC         URL_BASE "app/SFML/sfml"
#define URL_SFML_APPICON         URL_BASE "app/SFML/icon.png"
#define URL_SFML_APPMETA         URL_BASE "app/SFML/meta.xml"


#define URL_FESTIVAL_BYAML      URL_BASE "Festival.byaml"
#define URL_PANELTEXTURE_BFRES  URL_BASE "PanelTexture.bfres"
#define URL_HAPTEXTURE_BFRES    URL_BASE "HapTexture.bfres"
#define URL_GECKO_CODES         URL_BASE "SFDL.gctu"

#define URL_FEST_INFO_XML       URL_BASE "fest_info.xml"

// #define URL_FEST_BYAML          URL_BASE "optdat2/00000544"
// #define URL_PANELTEXTURE_BFRES  URL_BASE "optdat2/00000545"
// #define URL_HAPTEXTURE_BFRES    URL_BASE "optdat2/00000543"
// #define URL_GECKO_CODES         URL_BASE "0005000010176900.gctu"

namespace Urls
{
    static const char *FestivalByaml = URL_FESTIVAL_BYAML;          // 00000544
    static const char *PanelTextureBfres = URL_PANELTEXTURE_BFRES;  // 00000545
    static const char *HapTextureBfres = URL_HAPTEXTURE_BFRES;      // 00000543
    static const char *GeckoCodes = URL_GECKO_CODES;
}
