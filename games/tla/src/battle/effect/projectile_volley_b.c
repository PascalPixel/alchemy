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
#include "battle_effect_work.h"
#include "motobj.h"
#include "field_event.h"

#define ObjectDispatch_SetAnimationSpeedFar                                 \
  ((s32 (*)(struct FieldActor *, s32))ObjectDispatch_SetAnimationSpeedFar)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode0, BattleEffect_RunProjectileVolleyB, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode12, BattleEffect_RunProjectileVolleyB, 12)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode2, BattleEffect_RunProjectileVolleyB, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode6, BattleEffect_RunProjectileVolleyB, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode3, BattleEffect_RunProjectileVolleyB, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode5, BattleEffect_RunProjectileVolleyB, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode23, BattleEffect_RunProjectileVolleyB, 23)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode7, BattleEffect_RunProjectileVolleyB, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode21, BattleEffect_RunProjectileVolleyB, 21)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode4, BattleEffect_RunProjectileVolleyB, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode8, BattleEffect_RunProjectileVolleyB, 8)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode24, BattleEffect_RunProjectileVolleyB, 24)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode22, BattleEffect_RunProjectileVolleyB, 22)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode9, BattleEffect_RunProjectileVolleyB, 9)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode13, BattleEffect_RunProjectileVolleyB, 13)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode14, BattleEffect_RunProjectileVolleyB, 14)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode15, BattleEffect_RunProjectileVolleyB, 15)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode16, BattleEffect_RunProjectileVolleyB, 16)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode17, BattleEffect_RunProjectileVolleyB, 17)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode18, BattleEffect_RunProjectileVolleyB, 18)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode19, BattleEffect_RunProjectileVolleyB, 19)

DEFINE_EFFECT_KIND(BattleEffect_RunVolleyBMode20, BattleEffect_RunProjectileVolleyB, 20)

void BattleEffect_RunVolleyBMode10WithAction(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolleyB(effect, 10);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunVolleyBMode11WithAction(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolleyB(effect, 11);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x10);
}
