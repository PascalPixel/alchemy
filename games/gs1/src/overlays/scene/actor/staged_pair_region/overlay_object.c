#include "types.h"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02001cf6
#define SetOverlayObjectMode Func_02001d58
#define SetOverlayObjectSlot Func_02001df0
#define OverlayObject_CreateConfigured Func_02000048
#define OverlayObject_CreateConfiguredB Func_020000a0

#include "create_configured_overlay_object.h"

void *Func_02001c9e(s32, s32, s32, s32);
void Func_02001cfc(void *, s32);
void Func_02001d14(void *, s32);
void Func_02001d94(void *, s32);

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02001c9e(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *object = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        object[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02001cfc(obj, 0);
        Func_02001d94(obj, 14);
        Func_02001d14(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}
