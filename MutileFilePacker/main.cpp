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
    //for test
//    argc = 5;
//    argv[1] = "-i";
//    argv[2] = "/Users/Razer/Documents/MutileFilePacker/MutileFilePacker/Resources";
//    argv[3] = "-o";
//    argv[4] = "/Users/Razer/Documents/MutileFilePacker/data.bin";
    if (argc>=5) {
        std::string inputDir;
        std::string outputFile;
        for (int i =0; i < 5; ++i) {
            if (strcmp(argv[i], "-i") ==0 ) {
                inputDir = argv[++i];
            }else if(strcmp(argv[i], "-o") ==0 ){
                outputFile = argv[++i];
            }
        }
        
        if (inputDir!="" && outputFile!="") {
            Writer *wr = new Writer();
            wr->saveTo(inputDir.c_str(), outputFile.c_str());
            delete wr;
        }
        
    }else{
        fprintf(stderr, "usage: mfp -i INPUT_DIR -o OUTPUT_FILE\n");
    }
    
    return 0;
}
