#include "../../../../games/THE BROKEN SEAL/SRC/DEBUG/ITEM_LEVEL/LEVEL.H"
void ItemLevel_RunMotionTest(void)
{
    Battle_Reset();
    Engine_ActorSetAnimation(10, 2);
    Engine_ActorSetAnimation(11, 4);
    ObjectMotion_WaitForAnimationChange(10);
    ObjectMotion_SetHorizontalPositionWithTerrain( 11, 312 << 16, 416 << 16);
    BattleFx_FinishAction();
}
