/// @file
/// A Venus Djinni joining the party, and a Mercury Djinni's droplets.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field_bank.h"

struct FieldEffectWork;

/// A Venus Djinni joining the party, as The Broken Seal's
/// Djinn_RunVenusJoinEffect.
void Djinn_RunVenusJoinEffect(s32 djinniId) {
  struct Actor *leader;
  struct Actor *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(4, 16, 4));
  wait(30);
  djinni->paused = 0;
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 15);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 15);
  wait(30);
  djinni->update = (void (*)(void))BattleEffect_AdvanceSpinAngle;
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(djinniId, 8, 22);
  Sound_PlayFar(SE_SPIN);
  MapGrid_SetScreenShakeFar(px(13) / 10, px(13) / 10, Q16_ONE);
  djinni->update = (void (*)(void))FieldEffect_ShrinkActorAndDestroyFast;
  Actor_SelectAnimationFar(djinni, 3);
  wait(90);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(30);
  MapGrid_SetScreenShakeFar(px(8) / 5, px(8) / 5, Q16_ONE);

  position[0] = djinni->x.fixed;
  position[1] = djinni->y.fixed;
  position[2] = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize(slot, SPRITE_PARTICLE, position[0], position[2]);
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
    position[0] = leader->x.fixed;
    position[1] = leader->y.fixed + px(120);
    position[2] = leader->z.fixed;
    djinni =
        Actor_Spawn(SPRITE_PARTICLE, position[0], position[1], position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + Q16_ONE;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->orbitFrames = 100;
      djinni->orbitAngle = (remaining << 16) / 24;
      djinni->update = (void (*)(void))FieldEffect_UpdateDescendingOrbitActor;
      djinni->motionFlags = 0;
      Actor_SelectAnimationFar(djinni, 7);
      Actor_SetSpriteColorModeFar(djinni, SPRITE_COLOR_GREEN_TINT);
    }
  }

  wait(100);
  Sound_PlayFar(SE_STOP_MUSIC_PLAYER_5);
  wait(1);
  Sound_PlayFar(SE_LIGHT_APPEAR);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed + px(18);
  position[2] = leader->z.fixed;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni =
        Actor_Spawn(SPRITE_PARTICLE, position[0], position[1], position[2]);
    if (djinni == NULL)
      break;
    djinni->scaleY = Q16_ONE * 3 / 5;
    djinni->scaleX = Q16_ONE * 3 / 5;
    djinni->motionFlags = 2;
    djinni->velocityY = px(5);
    djinni->groundHeight = djinni->y.fixed;
    djinni->speedLimit = Random16() + px(7) / 5;
    Actor_SetTargetPositionFromMagnitudeAngle(djinni, px(32), Random16());
    Actor_SetSpriteColorModeFar(djinni, SPRITE_COLOR_GREEN_TINT);
    djinni->wait = 8;
    Actor_StartScriptFar(djinni, gBattleEffect_SharedParticleScript);
  }

  wait(15);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

struct FieldEffectWork;

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

  source = Actor_GetById(gMainRoutineWork.leaderActorId);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x.fixed;
    position.y = source->y.fixed + Random16() * 5 + px(15);
    {
      struct EffectVector *p = &position;

      position.z = source->z.fixed;
      Camera_WorldToScreen(p);
    }
    Vector_AddPolarOffset(Random16() * 6 + px(2), Random16(), &position);

    {
      s32 accel = 0x30000;

      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->x = position.x;
      effect->z = position.z - px(100);
      effect->acceleration = accel;
    }
    effect->maxSpeed = Random16() * 3 + px(3);
    effect->scaleX = Q16_ONE;
    effect->scaleY = Q16_ONE;
    effect->limitTurnStep = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(px(12), Random16(), &position);
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
        effect->state--;
      else
        effect->state++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    FieldEffect_ClearOwnedSlot(effect);
  }
}
