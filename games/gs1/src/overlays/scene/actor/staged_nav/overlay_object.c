#include "types.h"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02002b26
#define SetOverlayObjectMode Func_02002bb0
#define SetOverlayObjectSlot Func_02002c50
#define OverlayObject_PrepareObject Func_02000048
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_WaitUntilSettledAndReset Func_02000d78
#define OverlayObject_SetCallbackAndMode2 Func_02002694

#include "create_configured_overlay_object.h"

void *Func_02002ace(s32, s32, s32, s32);
void Func_02002b54(void *, s32);
void Func_02002b6c(void *, s32);
void Func_02002bf4(void *, s32);
void Func_020037be();
u8 *Func_020051b4(void);

/*
 * Poll an overlay object until it settles, then reset it -- resource_3b3.
 */

/* Declared without a prototype; the call site passes one argument. */

void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02002ace(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02002b54(obj, 0);
        Func_02002bf4(obj, 14);
        Func_02002b6c(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

/*
 * Wait at most sixty polls for the object's +12 to reach its +20, then clear
 * +0x28, set +0x3c, and mirror +20 back into +12. Func_020037be(1) is taken
 * to be a one-frame wait, and the field offsets are named by position and
 * not verified.
 */
void OverlayObject_WaitUntilSettledAndReset(u8 *obj)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Func_020037be(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

void OverlayObject_SetCallbackAndMode2(void)
{
    u8 *obj = Func_020051b4();
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = 0x02009aa9;
}
