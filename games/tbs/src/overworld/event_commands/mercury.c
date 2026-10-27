/// @file
/// A Mercury Djinni joining the party.
// Part of event_commands.c.

#include "../event_commands_internal.h"

#define MERCURY_PARTICLE_MAX 24  ///< droplets (one a field effect sprite)
#define MERCURY_AFTERIMAGE_MAX 8 ///< afterimages of the rising Djinni
/// A Mercury Djinni joins the party: the screen is tinted blue, the Djinni
/// bounces twice and hops three times ever higher, then rises 120 pixels out
/// of sight trailing eight afterimages. The leader turns to the camera and
/// plays ANIM_LEADER_ARMS_RAISED while 24 droplets rain round it, one a frame
/// (FieldEffect_UpdateRandomTargetParticle); then the colours return. Called
/// by Djinn_JoinParty for a Mercury Djinni, which then clears the Djinni's
/// actor slot; the Djinni's actor and its afterimages are released
/// here. Uses all 24 field effect sprites (FieldEffect_InitializeSlots).
void FieldEffect_RunMercuryDjinnCapture(s32 djinniId) {
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
  struct Vector3 position;

  djinni = Actor_GetById(djinniId);
  if (djinni == NULL)
    return;

  // Tint the screen blue (red 4, green 4, blue 16).
  FieldEffect_InitializeSlots();
  effectWork = gFieldEffectWork;
  Shop_SaveAndTintFieldPaletteFar(PALETTE_TINT_CUSTOM | RGB(4, 4, 16));
  WaitTicks(30);
  // Unpause the Djinni, so its script and motion run.
  djinni->paused = 0;
  Sound_PlaySongFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlaySongFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  WaitTicks(20);

  // Three hops, at 3, 5 and 7 pixels a frame.
  Sound_PlaySongFar(SE_JUMP);
  Actor_Jump(djinniId, 3, 14);
  Sound_PlaySongFar(SE_JUMP);
  Actor_Jump(djinniId, 5, 16);
  Sound_PlaySongFar(SE_JUMP);
  Actor_Jump(djinniId, 7, 18);
  WaitTicks(20);

  // Eight afterimages of the same sprite at 15/16 size, each closing half
  // the distance to the one before it every frame, sharing one VRAM cache
  // entry, as in Djinn_RunBattleStartHop.
  kind = djinni->sprite->entries[0]->animationId;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = MERCURY_AFTERIMAGE_MAX - 1; remaining >= 0; remaining--) {
    copy = Actor_CreateFar(kind, djinni->x.fixed, djinni->y.fixed,
                           djinni->z.fixed);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->steeringFlags |= 1;
      copy->update = FieldEffect_HalveDistanceToTarget;
      copy->facing = djinni->facing;
      Anim_ApplyChildValuesFar(copy, SPRITE_COLOR_BLUE_TINT);
      Actor_SelectAnimationFar(copy, 0);
      Actor_SetSpriteFlagsFar(copy, 0);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
      copy->linkedActor = previous;
      previous = copy;
    }
  }

  // Rise 8 pixels a frame for 15 frames, then remove the Djinni and its
  // afterimages, and free their shared VRAM cache entry (96,
  // VRAM_CACHE_ENTRY_COUNT, is none).
  resourceId = resource->slot;
  Sound_PlaySongFar(SE_HIGH_JUMP);
  djinni->motionFlags = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y.fixed += 8 << 16;
    WaitTicks(1);
  }
  Actor_ReleaseFar(djinni);
  list = copies;
  for (remaining = MERCURY_AFTERIMAGE_MAX - 1; remaining >= 0; remaining--)
    Actor_ReleaseFar((*list++));
  if (resourceId != 96)
    VramCache_FreeEntry(resourceId);
  WaitTicks(10);
  Actor_ArmCallback(gCommon.leaderActorId, FACING_SOUTH, 0);
  WaitTicks(20);
  Actor_SelectAnimationFar(Actor_GetById(gCommon.leaderActorId),
                           ANIM_LEADER_ARMS_RAISED);
  WaitTicks(20);

  // The droplets (sprite 240), one a frame, from where the Djinni was on
  // screen (FieldEffect_UpdateRandomTargetParticle moves them).
  position.x = djinni->x.fixed;
  position.y = djinni->y.fixed;
  position.z = djinni->z.fixed;
  Camera_WorldToScreen(&position);

  effect = effectWork->particles;
  remaining = MERCURY_PARTICLE_MAX - 1;
  do {
    FieldParticle_Initialize(effect, 240, position.x, position.z);
    FieldParticle_SetCallback(effect, FieldEffect_UpdateRandomTargetParticle);
    FieldParticle_SetActorMode(effect, 7);
    Sprite_SetAnimColorModesFar(effect->actor, SPRITE_COLOR_BLUE_TINT);
    remaining--;
    WaitTicks(1);
    effect++;
  } while (remaining >= 0);

  // After 120 frames every droplet still in use takes its last fall.
  // Walking whole slots instead of their state bytes grows native English
  // from 0x23c to 0x240 bytes.
  WaitTicks(120);
  {
    s32 nextState;

    nextState = MERCURY_DROPLET_LAST_FALL;
    slotState = (s8 *)effectWork;
    slotState += offsetof(struct FieldEffectWork, particles) +
                 offsetof(struct FieldParticle, state);
    remaining = MERCURY_PARTICLE_MAX - 1;
    do {
      if (slotState[5] != 0) // active
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  // Restore the colours.
  WaitTicks(50);
  Shop_RestoreFieldPaletteFar();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

#undef MERCURY_PARTICLE_MAX
#undef MERCURY_AFTERIMAGE_MAX
