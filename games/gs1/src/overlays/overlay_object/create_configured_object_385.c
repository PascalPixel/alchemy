#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_020010da(s32, s32, s32, s32);
void Func_02001120(void *, s32);
void Func_02001138(void *, s32);
void Func_020011e8(void *, s32);
#define AcquireOverlayObject      Func_020010da
#define RunOverlayObjectCommand0  Func_02001120
#define RunOverlayObjectCommand1  Func_02001138
#define RunOverlayObjectCommand14 Func_020011e8

void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth) {
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
