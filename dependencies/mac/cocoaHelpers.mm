#ifdef __APPLE__

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>
#include "cocoaHelpers.hpp"
#include <sys/stat.h>

const char* getResourcesPath()
{
    @autoreleasepool
    {
        NSString* appFolder = [NSBundle.mainBundle.bundlePath stringByDeletingLastPathComponent];
        NSString* dataFile = [appFolder stringByAppendingString:@"/Data.rsdk"];
        NSString* dataFolder = [appFolder stringByAppendingString:@"/Data"];
        NSString* settingsFile = [appFolder stringByAppendingString:@"/settings.ini"];
        if([NSFileManager.defaultManager fileExistsAtPath:dataFile] ||
        [NSFileManager.defaultManager fileExistsAtPath:settingsFile] ||
        [NSFileManager.defaultManager fileExistsAtPath:dataFolder]) {
            return [appFolder UTF8String];
        }
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
        NSString *applicationSupportDirectory = [paths firstObject];
        NSString* gameData = [applicationSupportDirectory stringByAppendingString:@"/RSDKv5"];
        if(![NSFileManager.defaultManager fileExistsAtPath:gameData]) {
            mkdir([gameData cStringUsingEncoding:NSUTF8StringEncoding], 0777);
        }
        return [gameData UTF8String];
    }
}

const char *getBundleResourcePath()
{
    return [[[NSBundle mainBundle] resourcePath] UTF8String];
}

const float getMacScreenScale()
{
    return [[NSScreen mainScreen] backingScaleFactor];
}
#endif
