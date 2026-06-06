#ifdef __APPLE__

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include "cocoaHelpers.hpp"
#include <sys/stat.h>

const char* getResourcesPath()
{
    static char pathStorage[1024] = {0};
    
    if (!strlen(pathStorage)) {
        @autoreleasepool
        {
            NSString* appFolder = [NSBundle.mainBundle.bundlePath stringByDeletingLastPathComponent];
            NSString* dataFile = [appFolder stringByAppendingString:@"/Data.rsdk"];
            NSString* dataFolder = [appFolder stringByAppendingString:@"/Data"];
            NSString* settingsFile = [appFolder stringByAppendingString:@"/settings.ini"];
            if([NSFileManager.defaultManager fileExistsAtPath:dataFile] ||
            [NSFileManager.defaultManager fileExistsAtPath:settingsFile] ||
            [NSFileManager.defaultManager fileExistsAtPath:dataFolder]){
                strncpy(pathStorage, [appFolder UTF8String], sizeof(pathStorage));
                return pathStorage;
            }
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
            NSString *applicationSupportDirectory = [paths firstObject];
            NSString* gameData = [applicationSupportDirectory stringByAppendingString:@"/RSDKv5"];
            if(![NSFileManager.defaultManager fileExistsAtPath:gameData]){
                mkdir([gameData cStringUsingEncoding:NSUTF8StringEncoding], 0777);
            }
            strncpy(pathStorage, [gameData UTF8String], sizeof(pathStorage));
            return pathStorage;
        }
    }

    return pathStorage;
}

const char *getBundleResourcePath()
{
    static char pathStorage[1024] = {0};
    
    if (!strlen(pathStorage)) {
        @autoreleasepool
        {
            NSString* resourceFolder = NSBundle.mainBundle.resourcePath;
            strncpy(pathStorage, [resourceFolder UTF8String], sizeof(pathStorage));
        }
    }

    return pathStorage;
}

const float getMacScreenScale(void){
    return NSScreen.mainScreen.backingScaleFactor;
}
#endif