#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSpawnedObject      Func_02000048
#define AcquireOverlayObject      Func_02005a2e
#define RunOverlayObjectCommand0  Func_02005aac
#define RunOverlayObjectCommand1  Func_02005ac4
#define RunOverlayObjectCommand14 Func_02005bfc
#define CreateOverlayObject Func_02005a86
#define SetOverlayObjectMode Func_02005b08
#define SetOverlayObjectSlot Func_02005c58
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_SpawnKind24AtObject Func_02005158
#define OverlayObject_StepScaleUpSixteenFrames Func_02005218
#define OverlayObject_ReleasePublishedAttachmentB Func_02005948

#include "create_configured_overlay_object.h"

void *Func_02005a2e(s32, s32, s32, s32);
void Func_02005aac(void *, s32);
void Func_02005ac4(void *, s32);
void Func_02005bfc(void *, s32);
u8 *Func_0200ab38(s32 kind, s32 arg1, s32 arg2, s32 arg3);
void Func_0200ab3e(u8 *src, void *table);
void Func_0200ab50(u8 *rec, s32 arg1);
u8 **Func_0200b304(s32 group, s32 slot);
void Func_0200b33c(u8 *obj);

/*
 * Spawns a kind-24 object from three fields of the caller's object, sets
 * three of its byte fields, then ORs two low bits into its owner record's
 * flag byte -- a sibling routine in this overlay clears those bits instead,
 * and the two are deliberately not unified. The 88-byte owner includes its
 * one pool word, 0x0200de08, which is never dereferenced here and so stays
 * a raw literal. The field offsets are named by position only.
 */

void *OverlayObject_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth)
{
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

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void OverlayObject_SpawnKind24AtObject(u8 *src)
{
    u8 *obj;
    u8 *rec;

    obj = Func_0200ab38(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
    if (obj == 0) {
        return;
    }

    rec = *(u8 **)(obj + 0x50);
    Func_0200ab3e(obj, (void *)0x0200de08);

    obj[0x55] = 0;
    obj[0x22] = 1;
    obj[0x23] = 2;

    if (rec != 0) {
        Func_0200ab50(rec, 2);
        rec[0x26] = 0;
        rec[9] |= 0x0C;
    }
}

s32 OverlayObject_StepScaleUpSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n << 11) + 0x10000;
    *(s32 *)(o + 28) = (n << 11) + 0x10000;
    return 1;
}

/* Release the scene object's optional attachment and clear its slot. */
void OverlayObject_ReleasePublishedAttachmentB(void)
{
    u8 **slot = Func_0200b304(35, 4);
    u8 *state;
    u8 *obj;

    if (slot == 0)
        return;

    state = *slot;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;

    Func_0200b33c(obj);
    *(u8 **)(state + 20) = 0;
}
