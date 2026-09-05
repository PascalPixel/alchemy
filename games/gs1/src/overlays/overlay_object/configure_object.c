#include "overlay_object.h"
#define NULL ((void *)0)

void Func_02004e8c(void *, s32);
void Func_02004f8c(void *, s32);

#define RunOverlayObjectCommand0 Func_02004e8c
#define RunOverlayObjectCommand1 Func_02004f8c

void ConfigureOverlayObject(struct OverlayObject *object, s32 parameter) {
    object->unknown_55 = 0;
    object->unknown_59 = 8;
    RunOverlayObjectCommand0(object, 0);
    RunOverlayObjectCommand1(object, parameter);
}

#undef RunOverlayObjectCommand0
#undef RunOverlayObjectCommand1
