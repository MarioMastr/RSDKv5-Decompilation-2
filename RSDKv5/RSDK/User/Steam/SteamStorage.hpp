#pragma once

#include <RSDK/Core/RetroEngine.hpp>
using namespace RSDK;

#if RETRO_REV02

struct SteamUserStorage : SKU::UserStorage {
    ISteamRemoteStorage *remoteStorage;

    int32 TryAuth()
    {
        authStatus = SKU::STATUS_OK;
        return authStatus;
    }
    int32 TryInitStorage()
    {
        remoteStorage = SteamRemoteStorage();
        storageStatus = SKU::STATUS_OK;
        return storageStatus;
    }
    bool32 GetUsername(String *userName)
    {
        const char* name = SteamFriends()->GetPersonaName();
        if (name)
        {
            SetString(userName, name);
            return true;
        }
        return false;
    }
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
    {
        bool32 success = false;

        if (!remoteStorage->FileExists(filename)) {
            PrintLog(PRINT_NORMAL, "File '%s' not found, creating...", filename);
            TrySaveUserFile(filename, buffer, size, callback, false);
        }

        if (!noSaveActive) {
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
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compressed)
    {
        bool32 success = false;
        if (!noSaveActive) {
            success = remoteStorage->FileWrite(filename, buffer, size);
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
    bool32 TryDeleteUserFile(const char *filename, void (*callback)(int32 status))
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
    void ClearPrerollErrors() {}
};

#endif