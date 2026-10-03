#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"

void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 mode)
{
    struct MotionObject *left = GetBattleObjectSlot(first)->object;
    struct MotionObject *right = GetBattleObjectSlot(second)->object;
    s32 left_x = left->x;
    s32 right_x = right->x;
    s32 left_z = left->z;
    s32 right_z = right->z;
    s32 x = (right_x + left_x) / 2;
    s32 z = (right_z + left_z) / 2;

    BattlePres_SetupTransitionScene(x, 0, z, mode);
}
