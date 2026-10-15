#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "system.h"
#include "types.h"
#include "battle_effect_work.h"

void BattleFx_LongApproachThenSpark(struct BattleEffectArgument *effect) {
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 0x18,
                                 0xc3333);
  WaitFrames(26);
  effect->variant = 5;
  BattleFx_RunSparkGroups(effect, 2);
}
