#include "OBJECT_LOOKUP.H"
#include "TYPES.H"

struct ObjectPairPosition {
    u8 unknown_00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
};

s32 Func_080044d0(s32, s32);
#define ArcTan2 Func_080044d0
void Func_08092878(void *, void *);
void Battle_WaitMode0(s32);

void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait)
{
    struct ObjectPairPosition *first;
    struct ObjectPairPosition *second;

    first = ObjectTable_Get(first_id);
    second = ObjectTable_Get(second_id);
    if (first != 0 && second != 0) {
        first->angle = ArcTan2(second->z - first->z,
            second->x - first->x);
        Battle_WaitMode0(wait);
    }
}

void Object_LinkPair(s32 first_id, s32 second_id, s32 wait)
{
    void *first = ObjectTable_Get(first_id);
    void *second = ObjectTable_Get(second_id);

    if (first != NULL && second != NULL) {
        Func_08092878(first, second);
        Battle_WaitMode0(wait);
    }
}
