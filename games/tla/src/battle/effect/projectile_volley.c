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

#define Actor_SetAnimationSpeedFar                                 \
  ((s32 (*)(struct FieldActor *, s32))Actor_SetAnimationSpeedFar)

DEFINE_EFFECT_KIND(BattleEffect_RunSlash, BattleEffect_RunProjectileVolley, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunShuriken, BattleEffect_RunProjectileVolley, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunCrazyVoice, BattleEffect_RunProjectileVolley, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunWingFlutter, BattleEffect_RunProjectileVolley, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunWingStroke, BattleEffect_RunProjectileVolley, 11)

DEFINE_EFFECT_KIND(BattleEffect_RunWingBeat, BattleEffect_RunProjectileVolley, 4)

void BattleEffect_RunCuttingEdge(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 5);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunHeatWave(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 6);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleEffect_RunSleet, BattleEffect_RunProjectileVolley, 7)

void BattleEffect_RunBlizzard(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 9);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode12WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 12);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode13WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 13);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode14WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 14);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode15,
                   BattleEffect_RunProjectileVolley, 15)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode16,
                   BattleEffect_RunProjectileVolley, 16)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode17,
                   BattleEffect_RunProjectileVolley, 17)

/// Kind 402: Ice Breath and Blue Dragon.
// Named, as The Broken Seal names it, for Ice Blessing.
DEFINE_EFFECT_KIND(BattleEffect_RunIceBlessing, BattleEffect_RunProjectileVolley, 10)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode18,
                   BattleEffect_RunProjectileVolley, 18)
