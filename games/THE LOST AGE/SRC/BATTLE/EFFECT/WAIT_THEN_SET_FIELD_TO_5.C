#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"


void BattleFx_LongApproachThenSpark(struct BattleEffectArgument *effect)
{
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 0x18, 0xc3333);
    WaitFrames(26);
    effect->variant = 5;
    BattleFx_RunSparkGroups(effect, 2);
}
