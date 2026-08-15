#include "overlay_object.h"

extern struct OverlayObject *Func_0200491e(s32, s32, s32, s32);
extern void Func_0200497c(struct OverlayObject *, s32);
extern void Func_02004994(struct OverlayObject *, s32);
extern void Func_02004ad4(struct OverlayObject *, s32);

#define AcquireOverlayObject Func_0200491e
#define RunOverlayObjectCommand0 Func_0200497c
#define RunOverlayObjectCommand1 Func_02004994
#define RunOverlayObjectCommand14 Func_02004ad4
#define PrepareOverlayObject Func_02000048

struct OverlayObject *PrepareOverlayObject(
    s32 first, s32 second, s32 third, s32 fourth) {
    struct OverlayObject *object;
    struct OverlayObjectRecord *record;
    s32 flags_mask;

    object = AcquireOverlayObject(fourth, first, second, third);
    if (object != 0) {
        record = object->record;
        flags_mask = -0xD;
        record->flags = (u8)(flags_mask & record->flags);
        object->unknown_55 = 0;
        object->unknown_59 = 8;
        RunOverlayObjectCommand0(object, 0);
        RunOverlayObjectCommand14(object, 0xE);
        RunOverlayObjectCommand1(object, 1);
        return object;
    }
    return 0;
}
