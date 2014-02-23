#ifndef  _APPAPPMACROS_H_
#define  _APPAPPMACROS_H_

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];

} Resource;

static Resource smallResource ={ cocos2d::CCSizeMake( 270,  480), "S"};
static Resource mediumResource ={ cocos2d::CCSizeMake(540,  888), "M"};
static Resource largeResource ={ cocos2d::CCSizeMake(1080, 1776), "L"};

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(540, 888);
#define NUMBER_FONT_SIZE (cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / mediumResource.size.width * 48)

#endif // _APPAPPMACROS_H_

