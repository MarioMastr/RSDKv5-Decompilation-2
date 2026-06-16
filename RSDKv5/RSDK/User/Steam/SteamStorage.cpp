#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02
#include <stdlib.h>

void SKU::SteamUserStorage::CopySave(bool save, const char *filename) {
    char steamFolderPath[0x100];
    if (SteamUser()->GetUserDataFolder(steamFolderPath, sizeof(steamFolderPath))) {
#if RETRO_PLATFORM == RETRO_WIN
        const char *remote = "\\..\\remote\\"; // windows and its stupid backslashes
#else
        const char *remote = "/../remote/";
#endif

        char steamFilePath[0x100];
        char localFilePath[0x100];
        char steamFilePathFake[0x100];

        sprintf_s(steamFilePathFake, sizeof(steamFilePathFake), "%s%s%s", steamFolderPath, remote, filename);
        sprintf_s(localFilePath, sizeof(localFilePath), "%s%s", SKU::userFileDir, filename);

        realpath(steamFilePathFake, steamFilePath);

        FILE *steamFileRead = fopen(steamFilePath, "r");
        FILE *localFileRead = fopen(localFilePath, "r");

        if (steamFileRead) {
            if (!save && localFileRead) {
                FILE *steamFileWrite = fopen(steamFilePath, "w");
                while (1) {
                    char data = fgetc(localFileRead);
                    if (data != EOF)
                        fputc(data, steamFileWrite);
                    else
                        break;
                }

                fclose(steamFileWrite);
            }
            else if (save) {
                FILE *localFileWrite = fopen(localFilePath, "w");

                while (1) {
                    char data = fgetc(steamFileRead);
                    if (data != EOF)
                        fputc(data, localFileWrite);
                    else
                        break;
                }

                fclose(localFileWrite);
            }
        }

        fclose(steamFileRead);
        fclose(localFileRead);
    }
    else
        PrintLog(PRINT_ERROR, "Steam userdata folder not found.");
}
#endif
