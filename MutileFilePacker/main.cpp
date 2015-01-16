//
//  main.cpp
//  MutileFilePacker
//
//  Created by Razer on 15/1/6.
//  Copyright (c) 2015年 Razer. All rights reserved.
//

#include <iostream>
#include "Writer.h"
#include "Reader.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Writer *wr = new Writer();
    wr->saveTo("/Users/Razer/Desktop/shells", "/Users/Razer/Desktop/aa.bin");
    delete wr;
//     int size = sizeof(long);
//    Reader *read = new Reader();
//    read->load("/Users/Razer/Desktop/aa.bin");
//    delete read;
    
    return 0;
}
