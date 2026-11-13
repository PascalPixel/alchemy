/// @file
/// A Venus Djinni joining the party, and a Mercury Djinni's droplets.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/field/field.h"

struct BattleFxScene;

/// A Venus Djinni joining the party, as The Broken Seal's
/// Djinn_RunVenusJoinEffect.
void Djinn_RunVenusJoinEffect(s32 arg) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = (struct CaptureObject *)Actor_GetById(gMainRoutineWork.currentOwner);
  djinni = (struct CaptureObject *)Actor_GetById(arg);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_SaveAndTintFieldPaletteFar(0x201204);
  wait(30);
  djinni->visible = 0;
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(arg, 4, 15);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(arg, 4, 15);
  wait(30);
  djinni->callback = (void *)BattleEffect_AdvanceSpinAngle;
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(arg, 8, 22);
  Sound_PlayFar(SE_SPIN);
  MapGrid_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = (void *)FieldEffect_ShrinkActorAndDestroyFast;
  Actor_SelectAnimationFar((struct Actor *)djinni, 3);
  wait(90);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(30);
  MapGrid_SetScreenShakeFar(0x19999, 0x19999, 0x10000);

  position[0] = djinni->x;
  position[1] = djinni->y;
  position[2] = djinni->z;
  Camera_WorldToScreen(position);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize(slot, 672, position[0], position[2]);
    FieldParticle_SetCallback(slot, (void *)FieldEffect_UpdateRadialSpread);
    FieldParticle_SetActorMode(slot, 7);
    Actor_SetChildValueUnlessFifteenFar(*(s32 *)slot, 11);
    ((struct FieldParticle *)slot)->scaleX = Q16_ONE / 2;
    ((struct FieldParticle *)slot)->scaleY = Random16() + Q16_ONE * 3 / 2;
    remaining--;
    wait(1);
    slot += 72;
  } while (remaining >= 0);

  wait(140);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  wait(20);
  MapGrid_SetScreenShakeFar(1, 1, 1);
  wait(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position[0] = leader->x;
    position[1] = leader->y + 0x780000;
    position[2] = leader->z;
    djinni = (struct CaptureObject *)Actor_Spawn(672, position[0], position[1],
                                                 position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + Q16_ONE;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = (void *)FieldEffect_UpdateDescendingOrbitActor;
      djinni->mode = 0;
      Actor_SelectAnimationFar((struct Actor *)djinni, 7);
      Actor_SetSpriteColorModeFar(djinni, 11);
    }
  }

  wait(100);
  Sound_PlayFar(SE_298);
  wait(1);
  Sound_PlayFar(SE_LIGHT_APPEAR);
  position[0] = leader->x;
  position[1] = leader->y + 0x120000;
  position[2] = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Actor_Spawn(672, position[0], position[1],
                                                 position[2]);
    if (djinni == NULL)
      break;
    djinni->scaleY = 0x9999;
    djinni->scaleX = 0x9999;
    djinni->mode = 2;
    djinni->gravity = px(5);
    djinni->baseY = djinni->y;
    djinni->speed = Random16() + 0x16666;
    Actor_SetTargetPositionFromMagnitudeAngle(djinni, 0x200000, Random16());
    Actor_SetSpriteColorModeFar(djinni, 11);
    djinni->lifetime = 8;
    Actor_StartScriptFar((struct Actor *)djinni,
                         gBattleEffect_SharedParticleScript);
  }

  wait(15);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

struct BattleFxScene;

/// Field effect sprite callback of a Mercury Djinni's droplets: each falls from
/// 100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
/// to 20 pixels over the leader, at 3 to 6 pixels a frame, and where it lands
/// splashes 12 pixels in a random direction, at half size, with sound 143. It
/// then falls again 6 frames later, until Djinn_RunMercuryJoinEffect
/// sets its state to 2: its next splash is its last.
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect) {
  struct Actor *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = Actor_GetById(gMainRoutineWork.currentOwner);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y + Random16() * 5 + 0xf0000;
    {
      struct EffectVector *p = &position;

      position.z = source->z;
      Camera_WorldToScreen(p);
    }
    Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

    {
      s32 accel = 0x30000;

      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->x = position.x;
      effect->z = position.z - 0x640000;
      effect->acceleration = accel;
    }
    effect->maxSpeed = Random16() * 3 + 0x30000;
    effect->scaleX = Q16_ONE;
    effect->scaleY = Q16_ONE;
    effect->limitTurnStep = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = Q16_ONE;
      effect->acceleration = 0;
      effect->maxSpeed = Random16() + 0x23333;
      effect->scaleX = Q16_ONE / 2;
      effect->scaleY = Q16_ONE / 2;
      Sound_PlayFar(SE_CRACK);

      if (*statePointer == 1)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    FieldEffect_ClearOwnedSlot(effect);
  }
}
