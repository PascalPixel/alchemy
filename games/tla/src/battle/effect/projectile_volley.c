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

#define ObjectDispatch_ApplyValueToChildrenFar                                 \
  ((s32 (*)(struct FieldActor *, s32))ObjectDispatch_ApplyValueToChildrenFar)

DEFINE_EFFECT_KIND(BattleFx_RunSlash, BattleFx_RunProjectileVolley, 0)

DEFINE_EFFECT_KIND(BattleFx_RunShuriken, BattleFx_RunProjectileVolley, 1)

DEFINE_EFFECT_KIND(BattleFx_RunCrazyVoice, BattleFx_RunProjectileVolley, 2)

DEFINE_EFFECT_KIND(BattleFx_RunWingFlutter, BattleFx_RunProjectileVolley, 3)

DEFINE_EFFECT_KIND(BattleFx_RunWingStroke, BattleFx_RunProjectileVolley, 11)

DEFINE_EFFECT_KIND(BattleFx_RunWingBeat, BattleFx_RunProjectileVolley, 4)

void BattleFx_RunCuttingEdge(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 5);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunHeatWave(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 6);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleFx_RunSleet, BattleFx_RunProjectileVolley, 7)

void BattleFx_RunBlizzard(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 9);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunProjectileVolleyMode12WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 12);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunProjectileVolleyMode13WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 13);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunProjectileVolleyMode14WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 14);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleFx_RunProjectileVolleyMode15,
                   BattleFx_RunProjectileVolley, 15)

DEFINE_EFFECT_KIND(BattleFx_RunProjectileVolleyMode16,
                   BattleFx_RunProjectileVolley, 16)

DEFINE_EFFECT_KIND(BattleFx_RunProjectileVolleyMode17,
                   BattleFx_RunProjectileVolley, 17)

// Named, as The Broken Seal names it, for Ice Blessing; here kind 402 is Ice
// Breath and Blue Dragon.
DEFINE_EFFECT_KIND(BattleFx_RunIceBlessing, BattleFx_RunProjectileVolley, 10)

DEFINE_EFFECT_KIND(BattleFx_RunProjectileVolleyMode18,
                   BattleFx_RunProjectileVolley, 18)
