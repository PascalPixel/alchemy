#include "FACING_OBJECT.H"
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
void Battle_WaitMode0(s32);
s32 WaitFrames(s32 frames);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second);

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
        FacingObject_TurnPairToFaceEachOther(first, second);
        Battle_WaitMode0(wait);
    }
}

/* Turns two objects toward each other by at most 0x1000 per frame, for up
   to sixty frames, until both face along the line between them. */
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second)
{
    s32 toward;
    s32 away;
    s32 frame;
    s32 turning;
    s32 delta;

    if (first == NULL || second == NULL)
        return;
    toward = (u16)ArcTan2(second->position_z - first->position_z,
        second->position_x - first->position_x);
    away = toward + 0x8000;
    for (frame = 0; frame < 60; frame++) {
        turning = 2;
        delta = (s16)(toward - first->facing);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            first->facing += delta;
        } else {
            turning = 1;
        }
        delta = (s16)(away - second->facing);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            second->facing += delta;
        } else {
            turning--;
        }
        if (turning == 0)
            break;
        WaitFrames(1);
    }
}
