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

void BattleFx_RunSlash(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 0);
}

void BattleFx_RunShuriken(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 1);
}

void BattleFx_RunCrazyVoice(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 2);
}

void BattleFx_RunWingFlutter(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 3);
}

void BattleFx_RunWingStroke(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 11);
}

void BattleFx_RunWingBeat(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 4);
}

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

void BattleFx_RunSleet(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 7);
}

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

void BattleFx_RunProjectileVolleyMode15(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 15);
}

void BattleFx_RunProjectileVolleyMode16(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 16);
}

void BattleFx_RunProjectileVolleyMode17(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 17);
}

// Named, as The Broken Seal names it, for Ice Blessing; here kind 402 is Ice
// Breath and Blue Dragon.
void BattleFx_RunIceBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 10);
}

void BattleFx_RunProjectileVolleyMode18(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 18);
}
