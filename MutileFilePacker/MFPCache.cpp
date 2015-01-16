//
//  MFPCache.cpp
//  MyGame
//
//  Created by Razer on 15/1/7.
//
//

#include "MFPCache.h"
#include <stdio.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>


using namespace cocos2d;

static MFPCache *_sharedMFPCache = nullptr;

MFPCache* MFPCache::getInstance(void){
    if (! _sharedMFPCache){
        _sharedMFPCache = new MFPCache();
        _sharedMFPCache->init();
    }
    return _sharedMFPCache;
}

void MFPCache::destroyInstance(){
    fclose(_sharedMFPCache->m_bFd);
    CC_SAFE_RELEASE_NULL(_sharedMFPCache);
}

bool MFPCache::init(){
    int size = sizeof(long);
   std::string path = FileUtils::getInstance()->fullPathForFilename("aa.bin");
    m_bFd =fopen(path.c_str(), "rb");
    ccpFileInfo fileInfo;
    memset(&fileInfo, 0, KCCP_FILEINFO_LEN);
    fread(&fileInfo, KCCP_FILEINFO_LEN, 1, m_bFd);
    for (int i=0; i < fileInfo.fileCount; ++i) {
        ccpDataInfo data;
        fread(&data, KCCP_DATAINFO_LEN, 1, m_bFd);
        m_dataMap.insert(std::make_pair(data.name, data));
        printf("%s %llu %llu %s \n ",data.name,data.size,data.offset,data.md5);
        
    }
    return true;
}

//expand zip to documents
void MFPCache::expand(const char *zipFile){
    
}

void MFPCache::addImage(const char *file){
    // npc/1.png 50702 23801075 aaa
    uint64_t size = 0;
    unsigned char *data = (unsigned char *)this->getBlockData(file,&size);
    if (data !=nullptr) {
        Image *image = new Image();
        image->initWithImageData((unsigned char*)data, size);
        Director::getInstance()->getTextureCache()->addImage(image, file);
        CC_SAFE_DELETE(image);
    }

}

void parseValueMap(ValueMap &dict){
    auto iter = dict.begin();
    while (iter != dict.end()) {
        printf("key=%s ",iter->first.c_str());
        if (iter->second.getType() == Value::Type::MAP) {
            parseValueMap(iter->second.asValueMap());
        }else if (iter->second.getType() == Value::Type::STRING){
            printf(" %s ",iter->second.asString().c_str());
        }else if (iter->second.getType() == Value::Type::DOUBLE){
            printf(" %f ",iter->second.asDouble());
        }
        printf("\n");
        ++iter;
    }
}

void MFPCache::parseXml(char *data,uint64_t size){
   
    xmlDocPtr doc;
    xmlNodePtr curNode;
    doc = doc = xmlReadMemory(data, size, "noname.xml", NULL, 0); // open a xml doc.
    curNode = xmlDocGetRootElement(doc); // get root element.
    
    if (curNode == NULL){
        fprintf(stderr, "open file failed. \n");
        xmlFreeDoc (doc);
        return ;
    }
    curNode = curNode->xmlChildrenNode;
    while(curNode != NULL){
        //取出节点中的内容
        if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1"))){
            xmlChar *szKey = xmlNodeGetContent(curNode);
            printf("newNode1: %s", szKey);
            xmlFree(szKey);
        }
        
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
}


//add .plist .png to cache
void MFPCache::addPlist(const char *file){
    uint64_t size = 0;
    char *data = this->getBlockData(file,&size);
//    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(file);
//    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
//    parseValueMap(dict);
    parseXml(data,size);
//    SpriteFrameCache::getInstance()->addSpriteFramesWithDictionary(dict, pobTexture);
}

//get raw data
char *MFPCache::getBlockData(const char *file,uint64_t *size){
    if(m_dataMap.find(file) != m_dataMap.end())
    {
        ccpDataInfo *dataInfo = &m_dataMap.find(file)->second;
        fseek(m_bFd, dataInfo->offset , SEEK_SET);//int fseek(FILE *stream, long offset, int fromwhere);fseek 用于二进制方式打开的文件,移动文件读写指针位置.
        memset(m_buffData, 0, sizeof(m_buffData));
        *size=fread(m_buffData, 1, dataInfo->size, m_bFd);
        return m_buffData;
    }
    return nullptr;
}

//update block file,write to local
bool MFPCache::updateBlockData(const char *file){
    return true;
}