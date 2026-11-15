/// @file
/// A Jupiter Djinni joining the party.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/shop.h"
#include "field/event_commands.h"
#include "field/field_bank.h"
#include "field/object.h"
#include "battle/object_effect_scripts.h"
#include "macros.h"

/// Ends a heavy impact: object id spins, leaps and vanishes in 24 sparks that
/// circle and then close on the current owner, which turns and takes mode 28.
void Djinn_RunJupiterJoinEffect(s32 id) {
  struct EffectVector position;
  struct MotionObject *object;
  struct FieldEffectWork *scene;
  struct FieldParticle *slot;
  s32 scale;
  s32 count;

  object = Actor_GetById(id);
  if (object == NULL)
    return;
  FieldEffect_InitializeSlots();
  scene = Ram_HeapSlots->fieldEffectWork;
  Shop_SaveAndTintFieldPaletteFar(0x20118c);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(id, 1);
  Sound_PlayFar(SE_NOTE_MID);
  Actor_RunRepeatedMotion(id, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(id, 1);
  wait(20);

  // Spin, then leap high.
  Sound_PlayFar(SE_SPIN);
  object->update = BattleEffect_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(SE_HIGH_JUMP);
  Actor_Jump(id, 12, 22);

  // 24 sparks from the object's place on screen, one a frame.
  SET_POSITION(&position, object->x, object->y, object->z);
  Camera_WorldToScreen(&position);
  Actor_ReleaseFar(object);
  Sound_PlayFar(SE_SCATTER);
  slot = scene->particles;
  count = 23;
  do {
    FieldParticle_Initialize(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, Random_Below(7));
    scale = Random16() / 3 + Q16_ONE; // 1.0 to 1.33
    slot->scaleY = scale;
    slot->scaleX = scale;
    count--;
    wait(1);
    slot++;
  } while (count >= 0);

  // The current owner turns to 0x4000 and takes mode 28.
  wait(60);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.leaderActorId), 28);
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
