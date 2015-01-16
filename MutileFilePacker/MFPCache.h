//
//  MFPCache.h
//  MyGame
//
//  Created by Razer on 15/1/7.
//
//

#ifndef __MyGame__MFPCache__
#define __MyGame__MFPCache__

#include <stdio.h>
#include "cocos2d.h"
#include "Constant.h"


class MFPCache: public cocos2d::Object{
private:
    char m_buffData[1024*1024*6];//buffer for tmp store
    FILE *m_bFd;
    std::map<std::string,ccpDataInfo> m_dataMap;
protected:
    MFPCache(){}
    
    bool init();
    
    void parseXml(char *data,uint64_t size);
    
public:
    static MFPCache* getInstance(void);
    
    static void destroyInstance();
    
    //expand zip to documents
    void expand(const char *zipFile);
    
    //add png to texturecache
    void addImage(const char *file);
    
    //add .plist .png to cache
    void addPlist(const char *file);
    
    //get raw data
    char *getBlockData(const char *file,uint64_t *size);
    
    //update block file,write to local
    bool updateBlockData(const char *file);
};

#endif /* defined(__MyGame__MFPCache__) */
