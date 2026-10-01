/* Draft: Japanese debug motion command with an ordinary direct call.
 * 2026-10-01: 50 instruction bytes plus 2 bytes alignment, matching owner size;
 * three argument-scheduling halfwords differ before the terrain-position call.
 * Ordinary TBS compiler and options; no scene placement or credit claimed.
 */
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
