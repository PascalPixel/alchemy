#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "BATTLE_EFFECT_WORK.H"
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect)
{
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 0x18, 0xC3333);
    WaitFrames(0x1D);
    effect->variant = 4;
    BattleFx_RunSparkGroups(effect, 2);
}
