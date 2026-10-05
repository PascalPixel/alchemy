#include "EVENTWRK.H"
#include "OBJECT_LOOKUP.H"
#include "TYPES.H"

struct ObjectPairPosition {
    u8 unknown_00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
};

s32 ArcTan2(s32, s32);

void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait)
{
    struct ObjectPairPosition *first;
    struct ObjectPairPosition *second;

    first = ObjectTable_Get(first_id);
    second = ObjectTable_Get(second_id);
    if (first != 0 && second != 0) {
        first->angle = ArcTan2(second->z - first->z,
            second->x - first->x);
        EventRuntime_Wait(wait);
    }
}
