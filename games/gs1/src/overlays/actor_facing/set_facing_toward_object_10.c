#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s16 Func_0200150e(s32, s32);
void *Func_02001564(s32);

s32 OverlayObject_SetFacingTowardObject10(void *self) {
    void *obj;

    obj = Func_02001564(0xA);
    FIELD_AT_OFFSET(self, s16 *, 6) = Func_0200150e(FIELD_AT_OFFSET(obj, s32 *, 0x10) - FIELD_AT_OFFSET(self, s32 *, 0x10), FIELD_AT_OFFSET(obj, s32 *, 8) - FIELD_AT_OFFSET(self, s32 *, 8));
    return 0;
}
