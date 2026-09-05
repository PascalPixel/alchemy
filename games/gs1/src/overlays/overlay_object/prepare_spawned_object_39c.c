#include "types.h"
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_02005a2e(s32, s32, s32, s32);
void Func_02005aac(void *, s32);
void Func_02005ac4(void *, s32);
void Func_02005bfc(void *, s32);

#define OverlayObject_PrepareSpawnedObject      Func_02000048
#define AcquireOverlayObject      Func_02005a2e
#define RunOverlayObjectCommand0  Func_02005aac
#define RunOverlayObjectCommand1  Func_02005ac4
#define RunOverlayObjectCommand14 Func_02005bfc

void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth) {
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
