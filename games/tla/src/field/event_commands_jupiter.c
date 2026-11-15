/// @file
/// A Jupiter Djinni joining the party.
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

/// A Jupiter Djinni joins the party: it bounces three times, spins and leaps
/// away, leaving 24 light particles that circle and then close on the leader,
/// who turns to the camera and raises its arms.
void Djinn_RunJupiterJoinEffect(s32 djinniId) {
  struct Vector3 position;
  struct Actor *djinni;
  struct FieldEffectWork *effectWork;
  struct FieldParticle *effect;
  s32 scale;
  s32 remainingParticles;

  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;
  FieldEffect_InitializeSlots();
  effectWork = Ram_HeapSlots->fieldEffectWork;
  // Tint the screen yellow: PALETTE_TINT_CUSTOM | RGB(12, 12, 4).
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
  djinni->update = (void (*)(void))BattleEffect_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(djinniId, 12, 22);

  // 24 light particles from where the Djinni is on screen.
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);
  Actor_ReleaseFar(djinni);
  Sound_PlayFar(SE_SCATTER);
  effect = effectWork->particles;
  remainingParticles = 23;
  // One a frame, each with a random sprite param of 0 to 6.
  do {
    FieldParticle_Initialize(effect, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(effect, 7);
    Animation_ApplyChildValuesToRecordFar(effect->object,
                                          (Random16() * 7) >> 16);
    scale = Random16() / 3 + Q16_ONE; // 1.0 to 1.33
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
void FieldEffect_PulseActorScale(u8 *object) {
  u32 *vblankCount = &gVBlankCount;
  u32 *table;
  u32 index;
  u32 value;

  table = gBattleEffect_PulseScales;
  index = (*vblankCount >> 2) & 1;
  value = index[table];
  ((struct MotionObject *)object)->scaleX = value;
  ((struct MotionObject *)object)->scaleY = value;
}

/// Per-frame update: spins an actor ever faster, from 1/16 turn a frame up to
/// just over 1/8 (TRIG_TURN / 8 + 64), 80 more each frame for 52 frames.
void FieldEffect_AdvanceAngleWithRamp(struct Actor *actor) {
  u32 step;

  step = actor->spinRampFrames * 80;
  actor->facing = actor->facing + step + FACING_STEP;
  if (step < FACING_STEP) {
    actor->spinRampFrames = actor->spinRampFrames + 1;
  }
}

/// Spins an object 1/8 turn and shrinks it by 0x1c0 a frame, destroying it once
/// its scale falls below 0x3000.
void FieldEffect_ShrinkActorAndDestroySlow(void *obj) {
  s32 step = 0xFFFFFE40;
  s32 scaleY;
  s32 scale;
  s32 angle;

  scaleY = ((struct MotionObject *)obj)->scaleY + step;
  scale = ((struct MotionObject *)obj)->scaleX;
  ((struct MotionObject *)obj)->scaleY = scaleY;
  angle = ((struct MotionObject *)obj)->angle;
  scale += step;
  ((struct MotionObject *)obj)->angle = angle + TRIG_TURN / 8;
  ((struct MotionObject *)obj)->scaleX = scale;
  if (scale < 0x3000)
    Actor_ReleaseFar(obj);
}

struct FieldEffectWork;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define gEffectWork Ram_HeapSlots->fieldEffectWork

/// Field effect sprite callback of a Mars Djinni's sparks: the spark flies 120
/// pixels up the screen (give or take 3/32 turn) from its origin at up to 5
/// pixels a frame and starts over, until it stops where it arrives, hidden; 40
/// frames later it swarms to points within 4 pixels of one 20 pixels over the
/// leader, until it is freed once it arrives.
void FieldEffect_UpdateMarsSpark(struct FieldParticle *effect) {
  struct EffectOwner *owner;
  struct Vector3 position;
  s32 state;

  owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  state = effect->state;

  if (state == 0) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(px(120),
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + FACING_NORTH,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = px(5);
    effect->maxSpeed = px(5);
    effect->limitTurnStep = state;
    effect->state++;

    ((struct Sprite *)effect->object)->image.priority =
        owner->sprite->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_HEAVY_IMPACT);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct Sprite *)effect->object)->image.priority = 0;
      effect->flags = 4;
    }
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct Sprite *)effect->object)->image.priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == 3) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    SET_POSITION(&position, owner->position.x, owner->position.y + px(20),
                 owner->position.z);
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(px(4), Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(SE_CRASH);
  } else if (state == 4) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
}

#undef Actor_SetChildValueUnlessFifteenFar
#undef gEffectWork
