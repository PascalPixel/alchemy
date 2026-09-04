#include "types.h"

#define OverlayObject_PrepareSpawnedObject Func_02000048
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_02001aca(s32, s32, s32, s32);
void Func_02001b20(void *, s32);
void Func_02001b38(void *, s32);
void Func_02001bd0(void *, s32);
#define AcquireOverlayObject      Func_02001aca
#define RunOverlayObjectCommand0  Func_02001b20
#define RunOverlayObjectCommand1  Func_02001b38
#define RunOverlayObjectCommand14 Func_02001bd0

void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind) {
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(kind, x, y, z);
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
