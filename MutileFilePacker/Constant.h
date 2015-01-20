//
//  Constant.h
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#ifndef MutileFilePacker_Constant_h
#define MutileFilePacker_Constant_h

#include <stdint.h>

#define KFILENAME_LEN 128
#define KKEY_LEN 32
typedef struct _ccpDataInfo
{
    char name[KFILENAME_LEN];//文件的名字 ui/aa/bb
    uint64_t offset;//在文件中的开始位置
    uint64_t size;
    _ccpDataInfo(){}
    _ccpDataInfo(const char *name,uint64_t offset,uint64_t size){
        strcpy(this->name, name);
        this->offset = offset;
        this->size   = size;
    }
}ccpDataInfo;

typedef struct _ccpFileInfo
{
    uint32_t version;
    uint32_t fileCount;//所有文件数量
}ccpFileInfo;

#define KCCP_DATAINFO_LEN sizeof(ccpDataInfo)
#define KCCP_FILEINFO_LEN sizeof(ccpFileInfo)
#endif
