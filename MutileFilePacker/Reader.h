//
//  Reader.h
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#ifndef __MutileFilePacker__Reader__
#define __MutileFilePacker__Reader__

#include <stdio.h>
class Reader{
private:
    FILE *m_bFd;
public:
    Reader();
    ~Reader();
    void load(const char *fileName);
};
#endif /* defined(__MutileFilePacker__Reader__) */
