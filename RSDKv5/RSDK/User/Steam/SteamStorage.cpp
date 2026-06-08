#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02
namespace fs = std::filesystem;

void SKU::SteamUserStorage::CopySave(bool save, const char *filename) {
    char steamFolderPath[0x100];
    if (SteamUser()->GetUserDataFolder(steamFolderPath, sizeof(steamFolderPath))) {
        std::string steamFolderString = steamFolderPath;
        size_t pos = steamFolderString.find("/local"); // linux stores in .local so we need the /
        std::string steamFilePath = steamFolderString.erase(pos).append("/remote/").append(filename);

        if (fs::exists(steamFilePath)) {
            fs::path steamFile = steamFilePath;
            std::string localFilePath = std::string(SKU::userFileDir).append(filename);

            if ((!save && fs::exists(localFilePath)) || (save && !fs::exists(localFilePath))) {
                fs::path localFile = localFilePath;
                fs::copy_file(save ? steamFile : localFile, save ? localFile : steamFile);
            }
        }
    }
    else
        PrintLog(PRINT_ERROR, "Steam userdata folder not found.");
}
#endif
