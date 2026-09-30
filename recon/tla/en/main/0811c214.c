#include "TYPES.H"
#include "MOTION_OBJECT.H"

void BattleMotion_ReservedNoOp83B0(void *, s32);

void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id)
{
    struct BattleObjectSlot *second_slot;
    struct MotionObject *first;
    struct MotionObject *second;
    s32 x1;
    s32 x2;
    s32 z1;
    s32 z2;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;

    first = GetBattleObjectSlot(first_id)->object;
    second_slot = GetBattleObjectSlot(second_id);
    second = second_slot->object;

    x1 = first->target_x;
    if (x1 == (s32)0x80000000) {
        x1 = first->x;
    }
    z1 = first->target_z;
    if (z1 == (s32)0x80000000) {
        z1 = first->z;
    }
    x2 = second->target_x;
    if (x2 == (s32)0x80000000) {
        x2 = second->x;
    }
    z2 = second->target_z;
    if (z2 == (s32)0x80000000) {
        z2 = second->z;
    }

    pos.x = (x1 + x2) / 2;
    pos.y = 0;
    pos.z = (z1 + z2) / 2;

    BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}
