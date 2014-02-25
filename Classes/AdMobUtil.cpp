#include "AdMobUtil.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "net/yohei2112/dropMan/dropMan"

using namespace std;
using namespace cocos2d;

void AdMobUtil::hideAdView()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "hideAdView", "()V"))
    {
        t.env->CallStaticObjectMethod(t.classID, t.methodID);
    }

}

void AdMobUtil::showAdView()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showAdView", "()V"))
    {
        t.env->CallStaticObjectMethod(t.classID, t.methodID);
    }

}
