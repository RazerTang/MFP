//
//  Writer.h
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#ifndef __MutileFilePacker__Writer__
#define __MutileFilePacker__Writer__

#include <stdio.h>
#include <iostream>
#include <map>
#include "Constant.h"

class Writer{
private:
    FILE *m_bFd;
    ccpFileInfo m_FileInfo;
    std::map<std::string,std::string> m_fileMap;
    std::map<std::string,ccpDataInfo> m_ccpDataInfoMap;
public:
    Writer();
    ~Writer();
    bool saveTo(const char *fromFileFolder,const char *toFileName);
private:
    bool saveToFd(const char *fileNmae,FILE *fd);
    
    void indexFiles(const char *fromFileFolder,const char *relativePath);
    
    bool compress(const char * DestName,const char *SrcName);
};

#endif /* defined(__MutileFilePacker__Writer__) */
