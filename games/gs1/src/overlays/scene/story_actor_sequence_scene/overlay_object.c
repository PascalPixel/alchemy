#include "types.h"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02001a2a
#define SetOverlayObjectMode Func_02001a74
#define SetOverlayObjectSlot Func_02001b4c
#define OverlayObject_CreateConfigured Func_02000048
#define OverlayObject_CreateConfiguredObject Func_020000a0

#include "create_configured_overlay_object.h"

void *Func_020019d2(s32, s32, s32, s32);
void Func_02001a18(void *, s32);
void Func_02001a30(void *, s32);
void Func_02001af0(void *, s32);

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_020019d2(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02001a18(obj, 0);
        Func_02001af0(obj, 14);
        Func_02001a30(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}
