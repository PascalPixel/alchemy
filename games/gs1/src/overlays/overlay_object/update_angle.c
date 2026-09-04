#include "overlay_object.h"

#define NULL ((void *)0)
#define UpdateOverlayObjectAngle Func_02000314
#define CalculateAngleFromCoordinateDelta Func_02004bb2

u16 CalculateAngleFromCoordinateDelta(s32, s32);

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
            if (angle_delta < (s32)0xf9b4f001) {
                angle_delta = (s32)0xf9b4f001;
            }
            object->angle = angle + angle_delta;
        }
    }
    return 1;
}
