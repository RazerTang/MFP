mfp is a tools to pack png together  
  
usage:  
  mfp -i INPUTDIR -o OUTPUTFILE  
like:
   mfp -i aaa/Resources -o bbb/data.bin  
it will generate file:data.bin.zip  
  
  
1.then drag data.bin.zip and MFPCache.h,MFPCache.cpp,Constant.h to your project   
2.change build setting,set header seachr path include ${SDK_ROOT}/usr/include/libxml2  
3.user like this:  
 MFPCache::getInstance()->expand("data.bin.zip");
    
         MFPCache::getInstance()->addPlist("ui.plist");
		 Sprite *spr1 = Sprite::createWithSpriteFrameName("ui/+30s.png");	
		 spr1->setPosition(Point(100,100));
		 addChild(spr1);

		 MFPCache::getInstance()->addImage("ken.png");
		 Sprite *spr2 = Sprite::create("ken.png");
		 spr2->setPosition(Point(200,200));
		 addChild(spr2);

