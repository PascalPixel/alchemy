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

void BattleFx_RunProjectileVolleyMode0(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 0);
}

void BattleFx_RunProjectileVolleyMode1(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 1);
}

void BattleFx_RunProjectileVolleyMode2(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 2);
}

void BattleFx_RunProjectileVolleyMode3(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 3);
}

void BattleFx_RunProjectileVolleyMode11(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 11);
}

void BattleFx_RunProjectileVolleyMode4(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 4);
}

void BattleFx_RunProjectileVolleyMode5WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 5);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunProjectileVolleyMode6WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Object_SetMode(object, 2);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x30);
  BattleFx_RunProjectileVolley(effect, 6);
  ObjectDispatch_ApplyValueToChildrenFar(object, 0x10);
}

void BattleFx_RunProjectileVolleyMode7(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 7);
}

void BattleFx_RunProjectileVolleyMode9WithAction(
    struct BattleEffectArgument *effect) {
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

void BattleFx_RunProjectileVolleyMode15(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 15);
}

void BattleFx_RunProjectileVolleyMode16(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 16);
}

void BattleFx_RunProjectileVolleyMode17(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 17);
}

void BattleFx_RunProjectileVolleyMode10(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 10);
}

void BattleFx_RunProjectileVolleyMode18(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 18);
}
