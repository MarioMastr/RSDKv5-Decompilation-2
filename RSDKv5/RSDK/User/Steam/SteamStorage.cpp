#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02
#include <stdlib.h>

void SKU::SteamUserStorage::CopySave(bool save, const char *filename)
{
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

        if (steamFileRead) {
            if (!save) {
                FILE *localFileRead = fopen(localFilePath, "r");
                if (localFileRead) {
                    FILE *steamFileWrite = fopen(steamFilePath, "w");
                    while (1) {
                        char data = fgetc(localFileRead);
                        if (data != EOF)
                            fputc(data, steamFileWrite);
                        else
                            break;
                    }

                    fclose(steamFileWrite);
                    fclose(localFileRead);
                }
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
    }
    else
        PrintLog(PRINT_ERROR, "Steam userdata folder not found.");
}

int32 SKU::SteamUserStorage::TryAuth()
{
    authStatus = SKU::STATUS_OK;
    return authStatus;
}

int32 SKU::SteamUserStorage::TryInitStorage()
{
    remoteStorage = SteamRemoteStorage();
    storageStatus = SKU::STATUS_OK;
    return storageStatus;
}

bool32 SKU::SteamUserStorage::GetUsername(String *userName)
{
    const char* name = SteamFriends()->GetPersonaName();
    if (name)
    {
        SetString(userName, name);
        return true;
    }
    return false;
}

bool32 SKU::SteamUserStorage::TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
{
    bool32 success = false;

    if (!noSaveActive) {
#if !RETRO_REV0U
        CopySave(false, filename);
#endif
        if (!remoteStorage->FileExists(filename)) {
            PrintLog(PRINT_ERROR, "File '%s' not found, creating...", filename);
            TrySaveUserFile(filename, buffer, size, callback, false);
        }

        int32 bytes = remoteStorage->FileRead(filename, buffer, size);
        if (bytes == 0) {
            PrintLog(PRINT_ERROR, "Failed to load user file.");
            return false;
        }

        if (callback)
            callback(SKU::STATUS_OK);

        PrintLog(PRINT_NORMAL, "Loaded Replay DB");

        success = true;
    }
    else {
        std::string str = __FILE__;
        str += ": TryLoadUserFile() # TryLoadUserFile(";
        str += filename;
        str += ", ...) failing due to noSave \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        if (callback)
            callback(SKU::STATUS_ERROR);
    }

    return success;
}

bool32 SKU::SteamUserStorage::TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compressed)
{
    bool32 success = false;
    if (!noSaveActive) {
        success = remoteStorage->FileWrite(filename, buffer, size);

#if !RETRO_REV0U
        CopySave(true, filename);
#endif

        if (callback)
            callback(SKU::STATUS_OK);
    }
    else {
        std::string str = __FILE__;
        str += ": TrySaveUserFile() # TrySaveUserFile(";
        str += filename;
        str += ", ...) failing due to noSave \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        if (callback)
            callback(SKU::STATUS_ERROR);
    }

    return success;
}

bool32 SKU::SteamUserStorage::TryDeleteUserFile(const char *filename, void (*callback)(int32 status))
{
    if (!noSaveActive) {
        remoteStorage->FileDelete(filename);
        if (callback)
            callback(SKU::STATUS_OK);
    }
    else {
        std::string str = __FILE__;
        str += ": TryDeleteUserFile() # TryDeleteUserFile(";
        str += filename;
        str += ", ...) failing due to noSave \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        if (callback)
            callback(SKU::STATUS_ERROR);
    }

    return true;
}

void SKU::SteamUserStorage::ClearPrerollErrors() {}
#endif
