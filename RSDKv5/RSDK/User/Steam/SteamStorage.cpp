#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02
namespace fs = std::filesystem;

void SKU::SteamUserStorage::CopySave(bool save, const char *filename) {
    char steamFolderPath[0x100];
    if (SteamUser()->GetUserDataFolder(steamFolderPath, sizeof(steamFolderPath))) {
        /*
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
        */
#if RETRO_PLATFORM == RETRO_LINUX
        const char *local = "/local"; // linux stores in .local so we need the /
        const char *remote = "/remote/";
#else
        const char *local = "local";
#if RETRO_PLATFORM == RETRO_WIN
        const char *remote = "remote\\"; // windows and its stupid backslashes
#else
        const char *remote = "local/";
#endif
#endif

        char steamFilePath[0x100];
        char localFilePath[0x100];
        int i = 0;

        int localLen = strlen(local);
        int steamFolderLen = strlen(steamFolderPath);

        while (i < localLen) {
            if (strstr(&steamFolderPath[i], local) == &steamFolderPath[i]) {
                steamFolderLen -= localLen;

                for (int j = i; j < steamFolderLen; j++) {
                    steamFolderPath[j] = steamFolderPath[j + localLen];
                }
            }
            else
                i++;
        }

        sprintf_s(steamFilePath, sizeof(steamFilePath), "%s%s", steamFolderPath, remote);
        sprintf_s(localFilePath, sizeof(localFilePath), "%s%s", SKU::userFileDir, filename);

        FileIO *steamFileRead = fOpen(steamFilePath, "r");
        FileIO *localFileRead = fOpen(localFilePath, "r");

        if (steamFileRead) {
            if (!save && localFileRead) {
                FileIO *steamFileWrite = fOpen(steamFilePath, "w");
                
                while ((char data = fgetc(localFileRead)) != EOF)
                    fputc(data, steamFileWrite);

                fClose(steamFileWrite);
            }
            else if (save) {
                FileIO *localFileWrite = fOpen(localFilePath, "w");

                while ((char data = fgetc(steamFileRead)) != EOF)
                    fputc(data, localFileWrite);

                fClose(localFileWrite);
            }
        }

        fClose(steamFileRead);
        fClose(localFileRead);
    }
    else
        PrintLog(PRINT_ERROR, "Steam userdata folder not found.");
}
#endif
