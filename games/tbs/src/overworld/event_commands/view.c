/// @file
/// The view bounds, zoom transitions and actors that ride another.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// The high byte of the first sprite part's OAM attribute 2.
/// Bits 2 and 3 are its priority.
#define SPRITE_ATTR2_HIGH(sprite) (((u8 *)(sprite))[9])

/// The event command "set view bounds": the camera keeps the screen inside
/// these map coordinates (FieldCamera_MoveWithinMapBounds).
void MapGrid_SetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
  struct MapGridScrollWork *grid = gMapGridWork[0];

  grid->minX = minX;
  grid->minY = minY;
  grid->maxX = maxX;
  grid->maxY = maxY;
}

// Task: moves the zoom one step from its previous value toward
// its target, and removes itself on the last step. It waits while the camera
// is paused.
// The task runner calls void(void). Plain void changes the native return from
// pop {r1}; bx r1 to pop {r0}; bx r0 (two instructions in tbs-en).
#ifdef UBFIX
void FieldEffect_StepRatioTransition(void) {
#else
s32 FieldEffect_StepRatioTransition(void) {
#endif
  struct WorldMapView *perspective;
  s16 *frames;
  s32 *from;
  s16 *step;
  s32 zoom;
  s32 delta;

  perspective = gMapGridWork[0];
  if (((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                               sizeof(struct FieldWork)))
          ->camera->paused != 0)
    return;
  frames = (s16 *)&perspective->zoomTransitionFrames;
  if (*frames == 0)
    return;
  from = &perspective->previousZoom;
  delta = perspective->zoom - *from;
  step = &perspective->zoomTransitionStep;
  (*step)++;
  /* FAKEMATCH(wrapper): the division written as a call to __divsi3, an
   * ordinary call where the operator makes a libcall block; as
   * delta * *step / *frames, 93 of 87 instructions differ in tbs-en. */
  zoom = *from + __divsi3(delta * *step, *frames);
  perspective->distance = Iwram_MulQ16(perspective->farPlane, zoom);
  gScanlinePitch = perspective->pitch + 1;
  if (*step == *frames) {
    *frames = 0;
#ifdef UBFIX
    Task_Remove(FieldEffect_StepRatioTransition);
#else
    Task_Remove((TaskFunc)FieldEffect_StepRatioTransition);
#endif
  }
}

/// Starts a zoom to zoom (16.16) over frames frames, on the World Map only. The
/// perspective work keeps the inverse, which scales the view's distance
/// (farPlane times it, FieldEffect_StepRatioTransition): a zoom of 2 halves the
/// distance. Event_WaitForZoomTransition waits for it.
void FieldEffect_ScheduleRatioTransition(s32 zoom, s32 frames) {
  struct WorldMapView *perspective = gMapGridWork[0];
  s32 inverse;

  if (((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                               sizeof(struct FieldWork)))
          ->mode != ACTOR_SYSTEM_WORLD_MAP)
    return;
  {
    inverse = Iwram_RatioMulQ14(zoom, Q16_ONE); // 1 / zoom
  }
  perspective->previousZoom = perspective->zoom;
  perspective->zoom = inverse;
  perspective->zoomTransitionFrames = frames;
  perspective->zoomTransitionStep = 0;
#ifdef UBFIX
  Task_Add(FieldEffect_StepRatioTransition, TASK_PASS_FRAME + 20);
#else
  Task_Add((TaskFunc)FieldEffect_StepRatioTransition, TASK_PASS_FRAME + 20);
#endif
}

/// Waits for a zoom transition to end (on the World Map only, at most 300
/// frames).
void Event_WaitForZoomTransition(void) {
  s32 frames;
  struct WorldMapView *perspective = gMapGridWork[0];

  if (((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                               sizeof(struct FieldWork)))
          ->mode == ACTOR_SYSTEM_WORLD_MAP) {
    frames = 0;
    if (*(s16 *)&perspective->zoomTransitionFrames != 0) {
      do {
        WaitFrames(1);
        frames++;
      } while (frames < 300 && *(s16 *)&perspective->zoomTransitionFrames != 0);
    }
  }
}

// Per-frame update of a riding actor: keeps it on the actor it rides
// (its watched actor), raised by the actor's sprite height (its
// metadata's boxX) and 8 pixels. Its work holds the actor's id. Its
// caller ignores the result. Plain void removes the native zero result and
// changes its return register (three instructions in tbs-en).
#ifdef UBFIX
void FieldEffect_CopyWatchedActorPosition(struct Actor *updated) {
#else
s32 FieldEffect_CopyWatchedActorPosition(struct Actor *updated) {
#endif
  void *self = (void *)updated;
  struct Actor *actor = self;
  struct Actor *host;

  host = actor->watchedActor;
  if (host != NULL) {
    actor->motionFlags = 0;
    actor->x.fixed = host->x.fixed;
    actor->y.fixed =
        host->y.fixed +
        ((s8)Resource_GetMetadataRecordFar(
             (s16)Scene_FindActorPlacement(*(s16 *)&actor->userValue)->sprite)
             ->boxX
         << 16) +
        (8 << 16);
    actor->groundHeight = host->groundHeight;
    actor->z.fixed = host->z.fixed;
  }
#ifndef UBFIX
  return 0;
#endif
}

/// The event command "actor show emote": shows an emote bubble (sprite
/// 21) over an actor, then waits wait frames. The low 4 bits of emote
/// pick the bubble (enum Emote; EMOTE_LIGHT_BULB also plays SE_CHIME); with
/// EMOTE_IN_FRONT the bubble takes sprite priority 1, otherwise the
/// actor's. The bubble rides the actor and releases itself after 46 frames
/// (gFieldEffect_WatchedActorScript: 16 frames of squash and stretch, then 30),
/// whatever wait is.
void FieldEffect_SpawnAttached(s32 actorId, s32 emote, s32 wait) {
  struct Actor *target;

  if ((emote & 0xff) == EMOTE_LIGHT_BULB) {
    Sound_PlayFar(SE_CHIME);
  }

  target = Actor_Lookup(actorId);
  if (target != 0) {
    struct Actor *bubble =
        Actor_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

    if (bubble != 0) {
      struct Sprite *sprite;

      Actor_StartScriptFar(bubble, gFieldEffect_WatchedActorScript);
      Actor_SelectAnimationFar(bubble, emote & 15);
      bubble->motionFlags = 0;
      bubble->timer = 0;
      bubble->userValue = actorId;
      sprite = bubble->sprite;
#ifdef UBFIX
      bubble->update = FieldEffect_CopyWatchedActorPosition;
#else
      bubble->update =
          (void (*)(struct Actor *))FieldEffect_CopyWatchedActorPosition;
#endif
      sprite->flags = 0;
      bubble->watchedActor = target;

      // Replace the bubble's priority bits (2 and 3) with 1 or the
      // actor's.
      if ((emote & EMOTE_IN_FRONT) != 0) {
        s32 mask = 13;
        u8 attr = SPRITE_ATTR2_HIGH(sprite);

        mask = -mask;
        mask &= attr;
        mask |= 4;
        SPRITE_ATTR2_HIGH(sprite) = mask;
      } else {
        s32 priority = 12;
        u8 src = SPRITE_ATTR2_HIGH(target->sprite);
        u8 dst;
        s32 mask = 13;

        priority &= src;
        dst = SPRITE_ATTR2_HIGH(sprite);
        mask = -mask;
        mask &= dst;
        mask |= priority;
        SPRITE_ATTR2_HIGH(sprite) = mask;
      }
    }
    Event_WaitFrames(wait);
  }
}

/// The event command "actor set attached effect", by enum
/// AttachedEffectFlag: removes the actor attached to an actor, attaches
/// one (SPRITE_ATTACHED_EFFECT, animation 1) or shows a one-off one (animation
/// 2) that ends by itself (gActor_WaitScript, 40 frames). The attached
/// actor is kept as the actor's watchedActor, so ATTACHED_EFFECT_ATTACH
/// does nothing while the actor has one, and ATTACHED_EFFECT_REMOVE
/// releases whatever it is; a one-off is never recorded there. The leader's
/// walking update runs all three for hazard ground (overworld/walking.c).
void FieldEffect_SetAttached(s32 actorId, s32 flags) {
  struct Actor *actor = Actor_Lookup(actorId);
  struct Actor *effect;
  struct Sprite *visual;
  s32 mode;

  effect = 0;
  visual = 0;

  if (actor == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == ATTACHED_EFFECT_ONCE || actor->watchedActor == 0) {
      effect = Actor_CreateFar(SPRITE_ATTACHED_EFFECT, actor->x.fixed,
                               actor->y.fixed, actor->z.fixed);
    }
  } else {
    effect = actor->watchedActor;
    if (effect == 0)
      return;
    Actor_ReleaseFar(effect);
    actor->watchedActor = 0;
    return;
  }

  if (effect == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case ATTACHED_EFFECT_ATTACH:
    Actor_SelectAnimationFar(effect, 1);
    actor->watchedActor = effect;
    effect->timer = 1;
    break;
  case ATTACHED_EFFECT_ONCE:
    Actor_SelectAnimationFar(effect, 2);
    Actor_StartScriptFar(effect, (const s32 *)gActor_WaitScript);
    effect->timer = 1;
    break;
  }

  effect->userValue = actorId;
  effect->motionFlags = 0;
#ifdef UBFIX
  effect->update = FieldEffect_CopyWatchedActorPosition;
#else
  effect->update =
      (void (*)(struct Actor *))FieldEffect_CopyWatchedActorPosition;
#endif
  visual = effect->sprite;
  visual->flags = 0;
  effect->watchedActor = actor;

  // Replace the effect's priority bits (2 and 3) with 1 or the actor's.
  if (flags & ATTACHED_EFFECT_IN_FRONT) {
    s32 mask = 13;
    u8 attr = SPRITE_ATTR2_HIGH(visual);

    mask = -mask;
    mask &= attr;
    mask |= 4;
    SPRITE_ATTR2_HIGH(visual) = mask;
  } else {
    s32 priority = 12;
    u8 src = SPRITE_ATTR2_HIGH(actor->sprite);
    u8 dst;
    s32 mask = 13;

    priority &= src;
    dst = SPRITE_ATTR2_HIGH(visual);
    mask = -mask;
    mask &= dst;
    mask |= priority;
    SPRITE_ATTR2_HIGH(visual) = mask;
  }
}

#undef SPRITE_ATTR2_HIGH
