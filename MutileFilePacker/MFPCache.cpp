//
//  MFPCache.cpp
//  MyGame
//
//  Created by Razer on 15/1/7.
//
//

#include "MFPCache.h"
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>


static std::string ReplaceString(const char *src,const char *from,const char *to){
    std::string srcString(src);
    std::string::size_type   pos(0);
    if( (pos=srcString.find(from))!=std::string::npos){
        return srcString.replace(pos,std::string(from).length(),to);
    }else{
        assert(0);
        return nullptr;
    }
}

//往下拉找到Search Paths在里面找到Header Search Paths
//开启编辑，并按下+然后输入${SDK_ROOT}/usr/include/libxml2

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
    return true;
}

bool MFPCache::unCompress(const char * DestName,const char *SrcName)
{
    FILE * fp_out = NULL;
    bool re = true;
    
    gzFile in;int len = 0;char buf[16384];
    
    in = gzopen(SrcName,"rb");
    
    if(in == NULL)
    {
        return false;
    }
    
    if(NULL == (fp_out = fopen(DestName,"wb")))
    {
        gzclose(in);
        return false;
    }
    
    for (;;)
    {
        len = gzread(in,buf,sizeof(buf));
        
        if(len < 0)
        {
            re = false;
            break;
        }
        
        if(len == 0) break;
        
        if(fwrite(buf,1,(unsigned)len,fp_out)!=len)
        {
            re = false;
            break;
        }
    }
    
    fclose(fp_out);
    gzclose(in);
    
    return re;
}

//expand zip to documents
void MFPCache::expand(const char *zipFile){
    std::string fromPath   = FileUtils::getInstance()->fullPathForFilename(zipFile);
    std::string toPath     = FileUtils::getInstance()->getWritablePath();
    std::string newFileName= ReplaceString(zipFile, ".zip", "");
    std::string newFilePath= toPath +"/"+newFileName;
    log("%s\n",newFilePath.c_str());
    //exist
    if (0 == access(newFilePath.c_str(), 0)) {
        log("exist=%s",newFilePath.c_str());
    }else{
       assert(unCompress(newFilePath.c_str(), fromPath.c_str()));
    }
    
    //open it
    m_bFd =fopen(newFilePath.c_str(), "rb");
    if(m_bFd){
        ccpFileInfo fileInfo;
        memset(&fileInfo, 0, KCCP_FILEINFO_LEN);
        fread(&fileInfo, KCCP_FILEINFO_LEN, 1, m_bFd);
        for (int i=0; i < fileInfo.fileCount; ++i) {
            ccpDataInfo data;
            fread(&data, KCCP_DATAINFO_LEN, 1, m_bFd);
            m_dataMap.insert(std::make_pair(data.name, data));
            printf("%s %llu %llu %s \n ",data.name,data.size,data.offset,data.md5);
            
        }
    }else{
        assert(m_bFd);
    }
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
    }else{
        assert(0);
    }

}

//for test
void parseValueMap(ValueMap &dict){
    auto iter = dict.begin();
    while (iter != dict.end()) {
        printf("key=%s ",iter->first.c_str());
        if (iter->first == "rotated") {
            auto value = iter->second;
            printf("%d",value.getType());
        }
        if (iter->second.getType() == Value::Type::MAP) {
            printf("map start");
            parseValueMap(iter->second.asValueMap());
            printf("map end");
        }else if (iter->second.getType() == Value::Type::STRING){
            printf(" %s ",iter->second.asString().c_str());
        }else if (iter->second.getType() == Value::Type::DOUBLE){
            printf(" %f ",iter->second.asDouble());
        }else if (iter->second.getType() == Value::Type::INTEGER){
            printf(" %c ",iter->second.asByte());
        }else if (iter->second.getType() == Value::Type::BOOLEAN){
            printf(" %d ",iter->second.asBool());
        }
        else{
            printf("error=%d\n",iter->second.getType());
            assert(0);
        }
        printf("\n");
        ++iter;
    }
}

//add .plist .png to cache
void MFPCache::addPlist(const char *file){
//    printf("/////////////////////\n");
//    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(file);
//    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
//
//    parseValueMap(dict);
//  
//    printf("/////////////////////\n");
//    std::string plistName(file);
//    std::string pngName;
//    std::string::size_type   pos(0);
//    if( (pos=plistName.find(".plist"))!=std::string::npos){
//        pngName=plistName.replace(pos,std::string(".plist").length(),".png");
//    }else{
//        assert(0);
//    }
    
    std::string png = ReplaceString(file, ".plist", ".png");
   
    ValueMap map;
    uint64_t size = 0;
    char *data = this->getBlockData(file,&size);
    parseXml(data,size,map);
//    parseValueMap(map);
    addImage(png.c_str());
    Texture2D *texture= Director::getInstance()->getTextureCache()->getTextureForKey(png);
    SpriteFrameCache::getInstance()->addSpriteFramesWithDictionary(map,texture);

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

#pragma mark xml parser
void MFPCache::parseXml(char *data,uint64_t size,ValueMap &dict){
    
    xmlDocPtr doc;
    xmlNodePtr curNode;
    doc = doc = xmlReadMemory(data, size,NULL, "UTF-8", XML_PARSE_RECOVER); // open a xml doc.
    curNode = xmlDocGetRootElement(doc); // get root element.
    
    if (curNode == NULL){
        fprintf(stderr, "open file failed. \n");
        xmlFreeDoc (doc);
        return ;
    }
    curNode = curNode->xmlChildrenNode;
    while(curNode != NULL){
        //取出节点中的内容
        if ((xmlStrcmp(curNode->name, BAD_CAST"dict")) == 0){
             dict = parseDict(curNode);
        }
        
        curNode = curNode->next;
    }
    xmlFreeDoc(doc);
}

cocos2d::ValueMap MFPCache::parseDict(xmlNodePtr node){
    xmlNodePtr curNode = node->children;
    std::string key;
    ValueMap map;
    while (curNode) {
        if ((xmlStrcmp(curNode->name, BAD_CAST"dict")) == 0){
            map[key.c_str()] = this->parseDict(curNode);
        }else if((xmlStrcmp(curNode->name, BAD_CAST"text")) != 0){
            if ((xmlStrcmp(curNode->name, BAD_CAST"key")) == 0) {
                key = parseKey(curNode);
//                printf("key= %s %s\n",curNode->name,key);
            }else{
                const char *ret = parseKey(curNode);
//              printf("ret= %s %s\n",curNode->name,ret);
                if (key == "rotated") {
                    if (xmlStrcmp(curNode->name, BAD_CAST"false")==0) {
                         map[key.c_str()] = Value(false);
                    }else if (xmlStrcmp(curNode->name, BAD_CAST"true")==0) {
                        map[key.c_str()] = Value(true);
                    }else{
                        assert(0);
                    }   
                }else{
                    map[key.c_str()] = Value(ret); 
                }
               
            }
            
        }
        curNode = curNode->next;
    }
    return map;
}

const char *MFPCache::parseKey(xmlNodePtr node){
     return (const char*)xmlNodeGetContent(node);
}