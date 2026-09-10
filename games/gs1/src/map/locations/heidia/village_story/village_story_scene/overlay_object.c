#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSpawnedObject      Func_02000048
#define AcquireOverlayObject      Func_020036ca
#define RunOverlayObjectCommand0  Func_02003710
#define RunOverlayObjectCommand1  Func_02003728
#define RunOverlayObjectCommand14 Func_020037e8
#define CreateOverlayObject Func_02003722
#define SetOverlayObjectMode Func_0200376c
#define SetOverlayObjectSlot Func_02003844
#define SetOverlayObjectRecordField1 Func_02000030
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_ApplyVelocity Func_02000104

#include "overlay_object.h"
#include "create_configured_overlay_object.h"

struct OverlayObjectRecordFieldView {
    u8 unknown_00[9];
    u8 unknown_00_0 : 2;
    u8 field_01 : 2;
    u8 unknown_04_0 : 4;
};

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

void *Func_020036ca(s32, s32, s32, s32);
void Func_02003710(void *, s32);
void Func_02003728(void *, s32);
void Func_020037e8(void *, s32);

void SetOverlayObjectRecordField1(struct OverlayObject *object, s32 value)
{
    struct OverlayObjectRecordFieldView *record;
    record = (struct OverlayObjectRecordFieldView *)object->record;
    record->field_01 = value;
}

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

void OverlayObject_ApplyVelocity(union Slot *object)
{
    u16 *record;

    object[2].w += object[17].w;
    object[3].w += object[18].w;
    object[4].w += object[19].w;
    object[6].w += object[12].w;
    object[7].w += object[13].w;
    record = (u16 *)object[20].p;
    record[15] += object[25].h[0];
}
