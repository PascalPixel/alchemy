#include "types.h"
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_0200491e(s32, s32, s32, s32);
void Func_0200497c(void *, s32);
void Func_02004994(void *, s32);
void Func_02004ad4(void *, s32);

#define OverlayObject_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_0200491e
#define RunOverlayObjectCommand0  Func_0200497c
#define RunOverlayObjectCommand1  Func_02004994
#define RunOverlayObjectCommand14 Func_02004ad4

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
