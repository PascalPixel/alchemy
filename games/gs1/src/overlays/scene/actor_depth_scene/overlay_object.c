#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_0200110e
#define RunOverlayObjectCommand0  Func_02001144
#define RunOverlayObjectCommand1  Func_0200115c
#define RunOverlayObjectCommand14 Func_02001244
#define CreateOverlayObject Func_02001166
#define SetOverlayObjectMode Func_020011a0
#define SetOverlayObjectSlot Func_020012a0
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_TurnStateByEighth Func_02000da8
#define OverlayObject_WaitForHeight Func_02000db8

#include "create_configured_overlay_object.h"

void *Func_0200110e(s32, s32, s32, s32);
void Func_02001144(void *, s32);
void Func_0200115c(void *, s32);
void Func_02001244(void *, s32);
/* Wait at most sixty frames for the object to reach the requested height. */
extern void Func_02001e54(s32 cnt);

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth) {
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

/* Turn the object's attached presentation state by one eighth-turn. */
void OverlayObject_TurnStateByEighth(u8 *obj)
{
    u8 *state = *(u8 **)(obj + 80);
    s32 v = *(u16 *)(state + 30) - 0x800;

    *(u16 *)(state + 30) = v;
}

void OverlayObject_WaitForHeight(u8 *obj, s32 height)
{
    s32 cnt = 60;
    while (cnt != 0) {
        Func_02001e54(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}
