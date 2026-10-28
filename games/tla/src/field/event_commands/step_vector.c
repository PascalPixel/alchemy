#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

void BattleEffect_StepVector2(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector2(vector, damping, gravity);
}

void BattleEffect_StepVector3(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector3(vector, damping, gravity);
}
