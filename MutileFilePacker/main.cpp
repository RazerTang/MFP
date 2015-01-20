//
//  main.cpp
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#include <iostream>
#include "Writer.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    Writer *wr = new Writer();
    wr->saveTo("/Users/regina/Documents/MFP/MutileFilePacker/Resources", "/Users/regina/Documents/MFP/data.bin");
    delete wr;
    return 0;
}
