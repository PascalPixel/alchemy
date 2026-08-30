#include "types.h"
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_02001992(s32, s32, s32, s32);
void Func_020019d8(void *, s32);
void Func_020019f0(void *, s32);
void Func_02001ad0(void *, s32);

#define AcquireOverlayObject      Func_02001992
#define RunOverlayObjectCommand0  Func_020019d8
#define RunOverlayObjectCommand1  Func_020019f0
#define RunOverlayObjectCommand14 Func_02001ad0
#define PrepareSceneObject        Func_02000048

void *PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth) {
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
