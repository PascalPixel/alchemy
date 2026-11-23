/// @file
/// A Jupiter Djinni joining the party, and per-frame updates the field effects
/// share: a pulse, a quickening spin, a slow shrink, and a Mars Djinni's
/// sparks.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/shop.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "battle/object_effect_scripts.h"
#include "macros.h"

/// A Jupiter Djinni joins the party: it bounces three times, spins, leaps out
/// of sight and bursts into PSYNERGY_EFFECT_SLOT_COUNT lights, which circle
/// and close in on the leader, who turns to the camera and raises its arms
/// (ANIM_LEADER_ARMS_RAISED).
void Djinn_RunJupiterJoinEffect(s32 djinniId) {
  struct Vector3 position;
  struct Actor *djinni;
  struct FieldEffectWork *effectWork;
  struct FieldParticle *effect;
  FIXED scale;
  s32 remainingParticles;

  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;
  FieldEffect_InitializeSlots();
  effectWork = Ram_HeapSlots->fieldEffectWork;
  // Tint the screen yellow.
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(12, 12, 4));
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_MID);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  wait(20);

  // Spin, then leap high.
  Sound_PlayFar(SE_SPIN);
  djinni->update = (ObjectUpdate)Djinn_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(djinniId, 12, 22);

  // A light particle in every slot the effect work has, from where the
  // Djinni is on screen.
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);
  Actor_ReleaseFar(djinni);
  Sound_PlayFar(SE_SCATTER);
  effect = effectWork->particles;
  remainingParticles = ARRAY_COUNT(effectWork->particles) - 1;
  // One a frame, each in a random colour mode from SPRITE_COLOR_NORMAL to
  // SPRITE_COLOR_GREY.
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimationColorModesFar(
        effect->actor, (Random16() * (SPRITE_COLOR_GREY + 1)) >> 16);
    scale = Random16() / 3 + FIXED_ONE; // 1.0 to 1.33
    effect->scaleY = scale;
    effect->scaleX = scale;
    remainingParticles--;
    wait(1);
    effect++;
  } while (remainingParticles >= 0);

  wait(60);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(40);
  Sound_PlayFar(SE_SCATTER);
  wait(100);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

/// Per-frame update: makes an actor pulse, switching its scale between the two
/// values of gBattleEffect_PulseScales every 4 frames.
void FieldEffect_PulseActorScale(struct Actor *actor) {
  const u32 *table = gBattleEffect_PulseScales;
  u32 index = ((u32)gVBlankCount >> 2) & 1;
  u32 scale = table[index];

  actor->scaleX = scale;
  actor->scaleY = scale;
}

/// Per-frame update: spins an actor ever faster, from 1/16 turn a frame up to
/// just over 1/8 (DEGtoANG(45) + 64), 80 more each frame for 52 frames.
void FieldEffect_AdvanceAngleWithRamp(struct Actor *actor) {
  u32 step;

  step = actor->spinRampFrames * 80;
  actor->facing = actor->facing + step + FACING_STEP;
  if (step < FACING_STEP) {
    actor->spinRampFrames = actor->spinRampFrames + 1;
  }
}

/// Per-frame update: spins an actor 1/8 turn and shrinks it by 7/1024 a frame,
/// releasing it once its scale falls below 3/16, 119 frames from full size.
/// The Mars Djinni's joining shrinks the Djinni away with it.
void FieldEffect_ShrinkActorAndDestroySlow(void *obj) {
  struct Actor *actor = obj;
  s32 step = -(FIXED_ONE * 7 / 1024);
  s32 scaleY;
  s32 scale;
  s32 angle;

  scaleY = actor->scaleY + step;
  scale = actor->scaleX;
  actor->scaleY = scaleY;
  angle = actor->facing;
  scale += step;
  actor->facing = angle + DEGtoANG(45);
  actor->scaleX = scale;
  if (scale < FIXED_ONE * 3 / 16)
    Actor_ReleaseFar(actor);
}

/// Field effect sprite callback of a Mars Djinni's sparks (enum
/// MarsSparkState): the spark flies 120 pixels up the screen (give or take 3/32
/// turn) from its origin at up to 5 pixels a frame and starts over, until it
/// stops where it arrives, hidden; 40 frames later it swarms to points within 4
/// pixels of one 20 pixels over the leader, until it is freed once it arrives.
void FieldEffect_UpdateMarsSpark(struct FieldParticle *effect) {
  struct Actor *owner;
  struct Vector3 position;
  s32 state;

  owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  state = effect->state;

  if (state == MARS_SPARK_AIM) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(toFIXED(120),
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + FACING_NORTH,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = toFIXED(5);
    effect->speedMax = toFIXED(5);
    effect->turnStepLimited = 0;
    effect->state++;

    // At the leader's sprite priority, until MARS_SPARK_RISING brings it in
    // front after 3 frames.
    ((struct Sprite *)effect->actor)->image.priority =
        (*(struct Sprite **)&owner->sprite)->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_HEAVY_IMPACT);
  } else if (state == MARS_SPARK_RISING) {
    if ((s16)effect->age == 3) {
      ((struct Sprite *)effect->actor)->image.priority = 0;
      effect->flags = FIELD_PARTICLE_LIFTED_BY_Z;
    }
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == MARS_SPARK_STOPPING) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct Sprite *)effect->actor)->image.priority = 0;
      effect->flags = FIELD_PARTICLE_LIFTED_BY_Z;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == MARS_SPARK_AIM_LEADER) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    SET_POSITION(&position, owner->x.fixed, owner->y.fixed + toFIXED(20),
                 owner->z.fixed);
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(toFIXED(4), Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_CRASH);
  } else if (state == MARS_SPARK_SWARMING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == MARS_SPARK_LEAVING) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}
