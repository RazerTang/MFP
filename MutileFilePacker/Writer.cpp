//
//  Writer.cpp
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Writer.h"
#include "Constant.h"
#include <assert.h>

static bool IsDir(const char *path){
     DIR *dir;
    if ((dir=opendir(path)) == nullptr) {
        return false;
    }
    closedir(dir);
    return true;
}


Writer::Writer():m_bFd(nullptr){
    memset(&m_FileInfo, 0,sizeof(ccpFileInfo));
}
Writer::~Writer(){
    
}
bool Writer::saveTo(const char *fromFileFolder,const char *toFileName){
    if (!m_bFd) {
        m_bFd = fopen(toFileName, "wb+");
    }
   
    
    indexFiles(fromFileFolder,"");
    m_FileInfo.version   = 1;
    m_FileInfo.fileCount = (unsigned int)m_fileMap.size();
    uint64_t offset = KCCP_FILEINFO_LEN +m_FileInfo.fileCount *KCCP_DATAINFO_LEN;
    fprintf(stderr,"%lu\n",(KCCP_FILEINFO_LEN +m_FileInfo.fileCount *KCCP_DATAINFO_LEN));
    fprintf(stderr,"%lu %lu\n",KCCP_FILEINFO_LEN,KCCP_DATAINFO_LEN);
    //write index info
    int count = 0;
    fseek(m_bFd, KCCP_FILEINFO_LEN, SEEK_SET);
    auto it = m_fileMap.begin();
    while (it != m_fileMap.end()) {
        FILE *fd = fopen(it->first.c_str(), "rb+");
        printf("%s\n",it->first.c_str());
        assert(fd);
        fseek(fd, 0, SEEK_END);
        uint64_t size = ftell(fd);

        int ret =fclose(fd);
        assert(ret == 0);
        //写入文件基本信息
        ccpDataInfo data(it->second.c_str(),offset,size);
         assert(sizeof(data) == KCCP_DATAINFO_LEN);
        fwrite(&data, KCCP_DATAINFO_LEN, 1, m_bFd);
        offset += size;
        m_ccpDataInfoMap.insert(std::make_pair(it->first, data));
        ++count;
        ++it;
    }
    if (count != m_FileInfo.fileCount) {
        assert(0);
    }
    const int  BUFFSIZE_SIZE = 1024*1024*8;
    //malloc 1024*1024*8 cause exec bad access
    char buff[1024*1024*6] = {0};//6mb
    auto iter = m_ccpDataInfoMap.begin();
    while (iter != m_ccpDataInfoMap.end()) {
        FILE *fd = fopen(iter->first.c_str(), "rb+");
        assert(fd);
      
        //到文件末尾
        fseek(fd, 0, SEEK_END);
        uint64_t size = ftell(fd);
        if(size >BUFFSIZE_SIZE){
            assert(0);
        }
        //到文件头
        fseek(fd, 0, SEEK_SET);
        memset(&buff, 0, sizeof(buff));
        //读取
        fread(&buff, size, 1, fd);
        fclose(fd);
        //写入
        fwrite(&buff, size, 1, m_bFd);
  
        ++iter;
    }
    
    fseek(m_bFd, 0, SEEK_SET);
    assert(sizeof(m_FileInfo)==KCCP_FILEINFO_LEN);
    fwrite(&m_FileInfo, KCCP_FILEINFO_LEN, 1, m_bFd);
    
    fclose(m_bFd);
    m_bFd=nullptr;
    //SIGABRIT 可能开的buff不够大
    return true;
    
}

bool Writer::saveToFd(const char *fileNmae,FILE *fd){
    return true;
}

void Writer::indexFiles(const char *fromFileFolder,const char *relativePath){
    DIR *dir;
    struct dirent *ptr;
    
    if ((dir=opendir(fromFileFolder)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
    // readdir() return next enter point of directory dir
    char absolutePath[1024];
    char fileName[KFILENAME_LEN];
    while ((ptr=readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") ==0 || strcmp(ptr->d_name, "..")==0 || strcmp(ptr->d_name, ".DS_Store")==0) {
            continue;
        }
        if(strcmp(relativePath, "")==0){
            sprintf(fileName, "%s",ptr->d_name);
        }else{
            sprintf(fileName, "%s/%s",relativePath,ptr->d_name);
        }
        sprintf(absolutePath, "%s/%s",fromFileFolder,ptr->d_name);
        if (IsDir( absolutePath)) {
            indexFiles(absolutePath,fileName);
        }else{
        
            m_fileMap.insert(std::make_pair(absolutePath, fileName));
        }
    }
     closedir(dir);
}
