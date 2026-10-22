#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "battle_effect_work.h"
#include "types.h"
#include "motobj.h"
#include "field_event.h"

#define ObjectDispatch_SetAnimationSpeedFar                                 \
  ((s32 (*)(struct FieldActor *, s32))ObjectDispatch_SetAnimationSpeedFar)

void BattleEffect_RunCanvasModeByCount(s32 *effect) {
  if (*effect > 199)
    Func_0818f620(effect, 2);
  else
    Func_0818f620(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunCanvasMode0, Func_0818f620, 0)

void BattleEffect_RunCanvasMode3WithAction(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x1c);
  Func_0818f620(effect, 3);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x10);
}
