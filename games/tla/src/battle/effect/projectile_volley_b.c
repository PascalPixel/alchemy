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

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode0, BattleFx_RunProjectileVolleyB, 0)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode12, BattleFx_RunProjectileVolleyB, 12)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode2, BattleFx_RunProjectileVolleyB, 2)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode6, BattleFx_RunProjectileVolleyB, 6)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode3, BattleFx_RunProjectileVolleyB, 3)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode5, BattleFx_RunProjectileVolleyB, 5)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode23, BattleFx_RunProjectileVolleyB, 23)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode7, BattleFx_RunProjectileVolleyB, 7)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode21, BattleFx_RunProjectileVolleyB, 21)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode4, BattleFx_RunProjectileVolleyB, 4)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode8, BattleFx_RunProjectileVolleyB, 8)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode24, BattleFx_RunProjectileVolleyB, 24)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode22, BattleFx_RunProjectileVolleyB, 22)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode9, BattleFx_RunProjectileVolleyB, 9)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode13, BattleFx_RunProjectileVolleyB, 13)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode14, BattleFx_RunProjectileVolleyB, 14)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode15, BattleFx_RunProjectileVolleyB, 15)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode16, BattleFx_RunProjectileVolleyB, 16)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode17, BattleFx_RunProjectileVolleyB, 17)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode18, BattleFx_RunProjectileVolleyB, 18)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode19, BattleFx_RunProjectileVolleyB, 19)

DEFINE_EFFECT_KIND(BattleFx_RunVolleyBMode20, BattleFx_RunProjectileVolleyB, 20)

void BattleFx_RunVolleyBMode10WithAction(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x30);
  BattleFx_RunProjectileVolleyB(effect, 10);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x10);
}

void BattleFx_RunVolleyBMode11WithAction(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x30);
  BattleFx_RunProjectileVolleyB(effect, 11);
  ObjectDispatch_SetAnimationSpeedFar(object, 0x10);
}
