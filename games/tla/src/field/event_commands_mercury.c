/// @file
/// A Mercury Djinni joining the party.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/shop.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "system/system.h"
#include "field/scene_call.h"
#include "macros.h"

#define MERCURY_PARTICLE_MAX 24  ///< droplets (one a field effect sprite)
#define MERCURY_AFTERIMAGE_MAX 8 ///< afterimages of the rising Djinni

/// Moves the actor halfway to the actor it watches; a copy of
/// Actor_MoveHalfwayTowardTarget.
void Actor_MoveHalfwayTowardTargetCopy(struct Actor *actor) {
  struct Actor *target = actor->watchedActor;

  actor->x.fixed += (target->x.fixed - actor->x.fixed) / 2;
  actor->y.fixed += (target->y.fixed - actor->y.fixed) / 2;
  actor->z.fixed += (target->z.fixed - actor->z.fixed) / 2;
}

/// A Mercury Djinni joins the party: the screen is tinted blue, the Djinni
/// bounces twice and hops three times ever higher, then rises 120 pixels out of
/// sight trailing eight afterimages.
void Djinn_RunMercuryJoinEffect(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *copy;
  struct Actor *previous;
  struct Sprite *resource;
  struct FieldEffectWork *effectWork;
  struct Actor **cursor;
  struct Actor **list;
  struct FieldParticle *effect;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct Actor *copies[MERCURY_AFTERIMAGE_MAX];
  struct {
    s32 x;
    s32 y;
    s32 z;
  } position;

  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;

  FieldEffect_InitializeSlots();
  effectWork = Ram_HeapSlots->fieldEffectWork;
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(4, 4, 16));
  wait(30);
  // Unpause the Djinni, so its script and motion run.
  djinni->paused = 0;
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  wait(20);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 3, 14);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 5, 16);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 7, 18);
  wait(20);

  kind = ((struct Sprite *)djinni->sprite)->entries[0]->animationId;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = 7; remaining >= 0; remaining--) {
    copy = Actor_CreateFar(kind, djinni->x.fixed, djinni->y.fixed,
                           djinni->z.fixed);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = AFTERIMAGE_SCALE;
      copy->scaleX = AFTERIMAGE_SCALE;
      copy->motionFlags = 0;
      copy->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      copy->steeringFlags |= ACTOR_FACES_MOTION;
      copy->update = (void (*)(void))Actor_MoveHalfwayTowardTargetCopy;
      copy->facing = djinni->facing;
      Actor_SetSpriteColorModeFar(copy, SPRITE_COLOR_BLUE_TINT);
      Actor_SelectAnimationFar(copy, ANIM_STILL);
      Actor_SetSpriteFlagsFar(copy, 0);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
      copy->watchedActor = previous;
      previous = copy;
    }
  }

  resourceId = resource->slot;
  Sound_PlayFar(SE_HIGH_JUMP);
  djinni->motionFlags = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y.fixed += toFIXED(8);
    wait(1);
  }
  Actor_ReleaseFar(djinni);
  list = copies;
  for (remaining = 7; remaining >= 0; remaining--)
    Actor_ReleaseFar((*list++));
  if (resourceId != VRAM_CACHE_ENTRY_COUNT)
    VramCache_FreeEntry(resourceId);
  wait(10);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineWork.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(20);

  SET_POSITION(&position, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
  Camera_WorldToScreen(&position.x);

  effect = effectWork->particles;
  remaining = MERCURY_PARTICLE_MAX - 1;
  do {
    FieldParticle_Initialize(effect, SPRITE_DROPLET, position.x, position.z);
    FieldParticle_SetCallback(effect,
                              (void *)FieldEffect_UpdateRandomTargetParticle);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->actor, SPRITE_COLOR_BLUE_TINT);
    remaining--;
    wait(1);
    effect++;
  } while (remaining >= 0);

  // After 120 frames every droplet still in use takes its last fall.
  wait(120);
  FIELD_PARTICLES_SET_ACTIVE_STATE(slotState, remaining, effectWork, particles,
                                   MERCURY_DROPLET_LAST_FALL);

  wait(50);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

#undef MERCURY_PARTICLE_MAX
#undef MERCURY_AFTERIMAGE_MAX
