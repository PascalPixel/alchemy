#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_0200491e
#define RunOverlayObjectCommand0  Func_0200497c
#define RunOverlayObjectCommand1  Func_02004994
#define RunOverlayObjectCommand14 Func_02004ad4
#define CreateOverlayObject Func_02004976
#define SetOverlayObjectMode Func_020049d8
#define SetOverlayObjectSlot Func_02004b30
#define CalculateAngleFromCoordinateDelta Func_02004bb2
#define OverlayObject_SetHandleFlagBits2And3 Func_02000030
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define UpdateOverlayObjectAngle Func_02000314

#include "create_configured_overlay_object.h"
#include "overlay_object.h"

typedef struct {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} S;

void *Func_0200491e(s32, s32, s32, s32);
void Func_0200497c(void *, s32);
void Func_02004994(void *, s32);
void Func_02004ad4(void *, s32);
u16 Func_02004bb2(s32, s32);

void OverlayObject_SetHandleFlagBits2And3(s32 obj, u32 v)
{
    S *h = *(S **)(obj + 0x50);
    h->f1 = v;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth) {
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

s32 UpdateOverlayObjectAngle(struct OverlayObject *object)
{
    struct OverlayObject *linked_object = object->linked_object;
    if (linked_object != NULL) {
        s32 angle_delta;
        u16 angle;
        object->unknown_5a = object->unknown_5a & 0xFE;
        angle_delta = CalculateAngleFromCoordinateDelta(
            linked_object->coordinate_10 - object->coordinate_10,
            linked_object->coordinate_08 - object->coordinate_08);
        angle = object->angle;
        angle_delta -= angle;
        angle_delta <<= 16;
        angle_delta >>= 16;
        if (angle_delta != 0) {
            if (angle_delta > 0x1000) {
                angle_delta = 0x1000;
            }
            if (angle_delta < -0x1000) {
                angle_delta = -0x1000;
            }
            object->angle = angle + angle_delta;
        }
    }
    return 1;
}
