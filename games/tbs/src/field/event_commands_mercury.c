/// @file
/// A Mercury Djinni joining the party.
// Part of event_commands.c.

#include "event_commands_internal.h"

#define MERCURY_DROPLETS 24      ///< droplets (one a field effect sprite)
#define MERCURY_AFTERIMAGE_MAX 8 ///< afterimages of the rising Djinni

/// A Mercury Djinni joins the party: the screen is tinted blue, the Djinni
/// bounces twice and hops three times ever higher, then rises 120 pixels out of
/// sight trailing eight afterimages; the leader raises its arms as droplets
/// rain round it.
void Djinn_RunMercuryJoinEffect(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *copy;
  struct Actor *previous;
  struct Sprite *sharedSprite;
  struct FieldEffectState *effectState;
  struct Actor **cursor;
  struct Actor **list;
  struct FieldParticle *effect;
  s8 *slotState;
  s32 spriteId;
  s32 i;
  s32 sharedSlot;
  struct Actor *copies[MERCURY_AFTERIMAGE_MAX];
  struct Vector3 position;

  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;

  // Tint the screen blue.
  FieldEffect_InitializeSlots();
  effectState = gFieldEffectState;
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(4, 4, 16));
  wait(30);
  // Unpause the Djinni, so its script and motion run.
  djinni->paused = 0;
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  wait(20);

  // Three hops, at 3, 5 and 7 pixels a frame.
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 3, 14);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 5, 16);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 7, 18);
  wait(20);

  // Eight afterimages of the same sprite at 15/16 size, each closing half
  // the distance to the one before it every frame, sharing one VRAM cache
  // entry, as in Djinn_RunBattleStartHop.
  spriteId = djinni->sprite->entries[0]->animationId;
  sharedSprite = NULL;
  previous = djinni;
  cursor = copies;
  for (i = 0; i < MERCURY_AFTERIMAGE_MAX; i++) {
    copy = Actor_CreateFar(spriteId, djinni->x.fixed, djinni->y.fixed,
                           djinni->z.fixed);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = AFTERIMAGE_SCALE;
      copy->scaleX = AFTERIMAGE_SCALE;
      copy->motionFlags = 0;
      copy->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      copy->steeringFlags |= ACTOR_FACES_MOTION;
      copy->update = FieldEffect_HalveDistanceToTarget;
      copy->facing = djinni->facing;
      Actor_SetSpriteColorModeFar(copy, SPRITE_COLOR_BLUE_TINT);
      Actor_SelectAnimationFar(copy, ANIM_STILL);
      Actor_SetSpriteFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->sprite, sharedSprite);
      copy->watchedActor = (union ActorOrEffect *)previous;
      previous = copy;
    }
  }

  // Rise 8 pixels a frame for 15 frames, then remove the Djinni and its
  // afterimages, and free their shared VRAM cache entry
  // (VRAM_CACHE_ENTRY_COUNT is none).
  sharedSlot = sharedSprite->slot;
  Sound_PlayFar(SE_HIGH_JUMP);
  djinni->motionFlags = 0;
  for (i = 0; i < 15; i++) {
    djinni->y.fixed += toFIXED(8);
    wait(1);
  }
  Actor_ReleaseFar(djinni);
  list = copies;
  for (i = 0; i < MERCURY_AFTERIMAGE_MAX; i++)
    Actor_ReleaseFar(*list++);
  if (sharedSlot != VRAM_CACHE_ENTRY_COUNT)
    VramCache_FreeEntry(sharedSlot);
  wait(10);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  wait(20);
  Actor_SelectAnimationFar(Actor_GetById(gMainRoutineState.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  wait(20);

  // The droplets (SPRITE_DROPLET), one a frame, from where the Djinni was on
  // screen (FieldEffect_UpdateRandomTargetParticle moves them).
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  effect = effectState->particles;
  i = MERCURY_DROPLETS - 1;
  do {
    FieldParticle_Initialize(effect, SPRITE_DROPLET, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRandomTargetParticle);
    FieldParticle_SetAnimation(effect, 7);
    Sprite_SetAnimationColorModesFar(effect->sprite, SPRITE_COLOR_BLUE_TINT);
    i--;
    wait(1);
    effect++;
  } while (i >= 0);

  // After 120 frames every droplet still in use takes its last fall.
  wait(120);
  FIELD_PARTICLES_SET_ACTIVE_STATE(slotState, i, effectState, particles,
                                   MERCURY_DROPLET_LAST_FALL);

  // Restore the colours.
  wait(50);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ReleaseSlots();
}

#undef MERCURY_DROPLETS
#undef MERCURY_AFTERIMAGE_MAX
