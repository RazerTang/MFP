//
//  Reader.cpp
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#include <string.h>
#include "Reader.h"
#include "Constant.h"

Reader::Reader():m_bFd(nullptr){
    
}

Reader::~Reader(){
    
}

void Reader::load(const char *fileName){
    m_bFd =fopen(fileName, "rb");
    ccpFileInfo fileInfo;
    memset(&fileInfo, 0, KCCP_FILEINFO_LEN);
    fread(&fileInfo, KCCP_FILEINFO_LEN, 1, m_bFd);
    for (int i=0; i < fileInfo.fileCount; ++i) {
        ccpDataInfo data;
        fread(&data, KCCP_DATAINFO_LEN, 1, m_bFd);
        printf("%s %lu %lu %s \n ",data.name,data.size,data.offset,data.md5);
        
    }
    fclose(m_bFd);
}