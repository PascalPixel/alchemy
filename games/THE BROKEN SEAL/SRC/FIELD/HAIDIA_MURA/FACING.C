#include "FIELD_EVENT.H"

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
u16 ArcTan2(s32 z, s32 x);

s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force)
{
    s32 result;
    u32 angle;
    u32 left;
    u32 right;
    u32 dir;

    result = 0;
    if (obj->unknown_5b == 1) {
        if (obj->rise_counter == 0) {
            Object_SetMode(obj, 1);
            return 1;
        }
    }
    if (Runtime_ComputeFixedPointDistance(&target->x.fixed, &obj->x.fixed) < range || force != 0) {
        angle = (u16)ArcTan2(target->z.fixed - obj->z.fixed,
                                target->x.fixed - obj->x.fixed);
        left = (angle - 0x1000) & 0xf000;
        right = (angle + 0x1000) & 0xf000;
        angle &= 0xf000;
        dir = obj->facing & 0xf000;
        if (angle == dir || right == dir || left == dir || force != 0) {
            /* FAKEMATCH: plain-byte publication avoids synthetic QI masks. */
            *(u8 *)&obj->unknown_5b = 1;
            Object_SetMode(obj, 1);
            result = 1;
            *(u8 *)&obj->rise_counter = result;
        } else {
            *(u8 *)&obj->unknown_5b = 0;
            Object_SetMode(obj, 2);
            *(u8 *)&obj->rise_counter = 0;
        }
    } else {
        *(u8 *)&obj->unknown_5b = 0;
        Object_SetMode(obj, 2);
        *(u8 *)&obj->rise_counter = 0;
    }
    return result;
}
