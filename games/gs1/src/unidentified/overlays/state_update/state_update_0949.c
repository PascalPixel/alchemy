#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_02005a2e(s32, s32, s32, s32);
void Func_02005aac(void *, s32);
void Func_02005ac4(void *, s32);
void Func_02005bfc(void *, s32);

#define AcquireOverlayObject      Func_02005a2e
#define RunOverlayObjectCommand0  Func_02005aac
#define RunOverlayObjectCommand1  Func_02005ac4
#define RunOverlayObjectCommand14 Func_02005bfc
#define PrepareOverlayObject      Func_02000048

void *PrepareOverlayObject(s32 first, s32 second, s32 third, s32 fourth) {
    void *overlay_object;
    void *object_record;
    s32 flags_mask;

    overlay_object = AcquireOverlayObject(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = M2C_FIELD(overlay_object, void *, 0x50);
        flags_mask = -0xD;
        M2C_FIELD(object_record, u8, 9) = (u8)(flags_mask & M2C_FIELD(object_record, u8, 9));
        M2C_FIELD(overlay_object, u8, 0x55) = 0;
        M2C_FIELD(overlay_object, u8, 0x59) = 8;
        RunOverlayObjectCommand0(overlay_object, 0);
        RunOverlayObjectCommand14(overlay_object, 0xE);
        RunOverlayObjectCommand1(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}
