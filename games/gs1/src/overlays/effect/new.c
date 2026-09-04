#include "types.h"
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_020043e6(s32, s32, s32, s32);
void Func_02004424(void *, s32);
void Func_0200443c(void *, s32);
void Func_0200451c(void *, s32);

#define NewEffectObject           Func_02000048
#define AcquireOverlayObject      Func_020043e6
#define RunOverlayObjectCommand0  Func_02004424
#define RunOverlayObjectCommand1  Func_0200443c
#define RunOverlayObjectCommand14 Func_0200451c
void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth) {
    void *overlay_object;
    void *object_record;
    s32 flags_mask;

    overlay_object = AcquireOverlayObject(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = FIELD_AT_OFFSET(overlay_object, void *, 0x50);
        flags_mask = -0xD;
        FIELD_AT_OFFSET(object_record, u8, 9) = (u8)(flags_mask & FIELD_AT_OFFSET(object_record, u8, 9));
        FIELD_AT_OFFSET(overlay_object, u8, 0x55) = 0;
        FIELD_AT_OFFSET(overlay_object, u8, 0x59) = 8;
        RunOverlayObjectCommand0(overlay_object, 0);
        RunOverlayObjectCommand14(overlay_object, 0xE);
        RunOverlayObjectCommand1(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}
