//
//  main.c
//  obliterator
//
//  Created by Sem Voigtländer on 11/11/19.
//  Copyright © 2019 kernelprogrammer. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <CoreFoundation/CoreFoundation.h>

void fail_and_exit(void){
    puts("Obliterate call could not be completed, perhaps Busy");
    exit(-1);
}

int (*mobile_obliterate_internal)(CFMutableDictionaryRef options);

int mobile_obliterate(CFMutableDictionaryRef options){
    void *obliterateref = dlopen("/System/Library/PrivateFrameworks/MobileObliteration.framework/MobileObliteration", RTLD_LAZY);
    if(!obliterateref){
        perror("Failed to load MobileObliteration.framework");
        fail_and_exit();
    }
    
    mobile_obliterate_internal = dlsym(obliterateref, "Mobile_Obliterate");
    if(!mobile_obliterate_internal){
        perror("Failed to find Mobile_Obliterate() \n");
    }

    if(mobile_obliterate_internal(options)){
        return -1;
    }
    else {
        return 0;
    }
}



int main(int argc, const char * argv[]) {
    
    CFMutableDictionaryRef options = CFDictionaryCreateMutable(0, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    if(!options){
        fwrite("Could not create options dictionary.\n", 0x25, 1, stderr);
        fail_and_exit();
    }
    CFStringRef nuke = CFStringCreateWithCString(kCFAllocatorDefault, "Minerva Mobile Security has power, nuclear power. Your device has been nuked.", kCFStringEncodingUTF8);
    CFDictionarySetValue(options, CFSTR("ObliterationMessage"), nuke);
    
   
    if(nuke){
        CFRelease(nuke);
        nuke = NULL;
    }
    
    CFDictionarySetValue(options,CFSTR("ObliterationType"), CFSTR("ObliterationTypeWipeAndBrick"));
    CFDictionarySetValue(options, CFSTR("DisplayProgressBar"), kCFBooleanTrue);
    CFDictionarySetValue(options, CFSTR("IgnoreMissingPath"), kCFBooleanTrue);
    
    printf("WARNING!!!!!\nThis tool will wipe your device and put it into DFU mode.\nYou will have to do a clean restore via iTunes.\nDo you wish to continue (y/n): ");
    
    int _continue = getchar();
    if(_continue != 'y'){
        printf("Aborted.");
        exit(1);
    }
    
    int err = mobile_obliterate(options);
    
    if(err){
        fprintf(stderr, "Could not obliterate: %d.\n", err);
        CFRelease(options);
        fail_and_exit();
    }
    puts("Obliterate call connection successful");
    return 0;
}
