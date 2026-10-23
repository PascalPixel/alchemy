/// @file
/// The view bounds, zoom transitions and objects linked to an object event.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// The high byte of the first sprite part's OAM attribute 2.
/// Bits 2 and 3 are its priority.
#define SPRITE_ATTR2_HIGH(sprite) (((u8 *)(sprite))[9])

/// The script command "set view bounds": the camera keeps the screen inside
/// these map coordinates (Camera_MoveWithinMapBounds).
void MapGrid_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
  struct MapGridScrollWork *grid = gMapGridWork[0];

  grid->minX = minX;
  grid->minY = minY;
  grid->maxX = maxX;
  grid->maxY = maxY;
}

// Scheduler callback: moves the zoom one step from its previous value toward
// its target, and removes itself on the last step. It waits while the camera
// is paused.
// The scheduler calls void(void). Plain void changes the native return from
// pop {r1}; bx r1 to pop {r0}; bx r0 (two instructions in tbs-en).
#ifdef UBFIX
void FieldEffect_StepRatioTransition(void) {
#else
s32 FieldEffect_StepRatioTransition(void) {
#endif
  struct PerspectiveWork *perspective;
  s16 *frames;
  s32 *from;
  s16 *step;
  s32 zoom;
  s32 delta;

  perspective = gMapGridWork[0];
  if (((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                 sizeof(struct FieldWork)))
          ->camera->movementState != 0)
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
    Scheduler_RemoveCallback(FieldEffect_StepRatioTransition);
#else
    Scheduler_RemoveCallback(
        (SchedulerCallback)FieldEffect_StepRatioTransition);
#endif
  }
}

/// Starts a zoom to zoom (16.16) over frames frames, on the world map only. The
/// perspective work keeps the inverse, which scales the view's distance
/// (farPlane times it, FieldEffect_StepRatioTransition): a zoom of 2 halves the
/// distance. FieldScript_WaitForZoomTransition waits for it.
void FieldEffect_ScheduleRatioTransition(s32 zoom, s32 frames) {
  struct PerspectiveWork *perspective = gMapGridWork[0];
  s32 inverse;

  if (((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                 sizeof(struct FieldWork)))
          ->mode != MODE_WORLD_MAP)
    return;
  {
    inverse = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
  }
  perspective->previousZoom = perspective->zoom;
  perspective->zoom = inverse;
  perspective->zoomTransitionFrames = frames;
  perspective->zoomTransitionStep = 0;
  // 0xc94: a main-loop callback (high byte 0xc, as SCHEDULER_KEY_MAIN_LOOP)
  // at priority 0x94.
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(FieldEffect_StepRatioTransition, 0xc94);
#else
  Scheduler_AddOrUpdateCallback(
      (SchedulerCallback)FieldEffect_StepRatioTransition, 0xc94);
#endif
}

/// Waits for a zoom transition to end (on the world map only, at most 300
/// frames).
void FieldScript_WaitForZoomTransition(void) {
  s32 frames;
  struct PerspectiveWork *perspective = gMapGridWork[0];

  if (((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                 sizeof(struct FieldWork)))
          ->mode == MODE_WORLD_MAP) {
    frames = 0;
    if (*(s16 *)&perspective->zoomTransitionFrames != 0) {
      do {
        WaitFrames(1);
        frames++;
      } while (frames < 300 && *(s16 *)&perspective->zoomTransitionFrames != 0);
    }
  }
}

// Per-frame update of a riding object: keeps it on the object event it rides
// (its linked object), raised by the object event's sprite height (its
// metadata's boxX) and 8 pixels. Its work holds the object event's id. The hook
// consumer ignores the result. Plain void removes the native zero result and
// changes its return register (three instructions in tbs-en).
#ifdef UBFIX
void FieldEffect_CopyLinkedObjectPosition(union FieldObject *fieldObject) {
#else
s32 FieldEffect_CopyLinkedObjectPosition(union FieldObject *fieldObject) {
#endif
  void *self = (void *)fieldObject;
  struct ObjectEvent *objectEvent = self;
  struct ObjectEvent *host;

  host = objectEvent->linkedObject;
  if (host != NULL) {
    objectEvent->motionFlags = 0;
    objectEvent->x.fixed = host->x.fixed;
    objectEvent->y.fixed =
        host->y.fixed +
        ((s8)Resource_GetMetadataRecordFar(
             (s16)Map_FindObjectEventTemplate(*(s16 *)&objectEvent->work)
                 ->sprite)
             ->boxX
         << 16) +
        (8 << 16);
    objectEvent->terrainHeight = host->terrainHeight;
    objectEvent->z.fixed = host->z.fixed;
  }
#ifndef UBFIX
  return 0;
#endif
}

/// The script command "object event show emote": shows an emote bubble (object
/// 21) over an object event, then waits wait frames. The low 4 bits of emote
/// pick the bubble (enum Emote; EMOTE_LIGHT_BULB also plays SE_110); with
/// EMOTE_IN_FRONT the bubble takes sprite priority 1, otherwise the object
/// event's. The bubble rides the object event and frees itself after 46 frames
/// (gFieldEffect_LinkedObjectScript: 16 frames of squash and stretch, then 30),
/// whatever wait is.
void FieldEffect_SpawnLinked(s32 localId, s32 emote, s32 wait) {
  struct ObjectEvent *target;

  if ((emote & 0xff) == EMOTE_LIGHT_BULB) {
    AudioCommand_PlayFar(SE_110);
  }

  target = ObjectTable_Get(localId);
  if (target != 0) {
    struct ObjectEvent *bubble = (struct ObjectEvent *)Object_CreateFar(
        21, target->x.fixed, target->y.fixed, target->z.fixed);

    if (bubble != 0) {
      struct AnimationObject *sprite;

      ObjectDispatch_InitializeFar((struct DispatchObject *)bubble,
                                   gFieldEffect_LinkedObjectScript);
      ObjectDispatch_SelectAnimationFar(bubble, emote & 15);
      bubble->motionFlags = 0;
      bubble->timer = 0;
      bubble->work = localId;
      sprite = bubble->sprite;
#ifdef UBFIX
      bubble->update = FieldEffect_CopyLinkedObjectPosition;
#else
      bubble->update =
          (void (*)(union FieldObject *))FieldEffect_CopyLinkedObjectPosition;
#endif
      sprite->flags = 0;
      bubble->linkedObject = target;

      // Replace the bubble's priority bits (2 and 3) with 1 or the object
      // event's.
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
    FieldScript_WaitFrames(wait);
  }
}

/// The script command "object event set attached effect", by enum
/// AttachedEffectFlag: removes the object attached to an object event, attaches
/// one (SPRITE_ATTACHED_EFFECT, animation 1) or shows a one-off one (animation
/// 2) that ends by itself (gObjectMotion_WaitScript, 40 frames). The attached
/// object is kept as the object event's linkedObject, so ATTACHED_EFFECT_ATTACH
/// does nothing while the object event has one, and ATTACHED_EFFECT_REMOVE
/// releases whatever it is; a one-off is never recorded there. The leader's
/// step runs all three for hazard ground (object/step.c).
void FieldEffect_ConfigureLinkedObject(s32 localId, s32 flags) {
  struct ObjectEvent *objectEvent = ObjectTable_Get(localId);
  struct ObjectEvent *effect;
  struct AnimationObject *visual;
  s32 mode;

  effect = 0;
  visual = 0;

  if (objectEvent == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == ATTACHED_EFFECT_ONCE || objectEvent->linkedObject == 0) {
      effect = (struct ObjectEvent *)Object_CreateFar(
          SPRITE_ATTACHED_EFFECT, objectEvent->x.fixed, objectEvent->y.fixed,
          objectEvent->z.fixed);
    }
  } else {
    effect = objectEvent->linkedObject;
    if (effect == 0)
      return;
    ObjectDispatch_ReleaseFar((struct DispatchObject *)effect);
    objectEvent->linkedObject = 0;
    return;
  }

  if (effect == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case ATTACHED_EFFECT_ATTACH:
    ObjectDispatch_SelectAnimationFar(effect, 1);
    objectEvent->linkedObject = effect;
    effect->timer = 1;
    break;
  case ATTACHED_EFFECT_ONCE:
    ObjectDispatch_SelectAnimationFar(effect, 2);
    ObjectDispatch_InitializeFar((struct DispatchObject *)effect,
                                 (const s32 *)gObjectMotion_WaitScript);
    effect->timer = 1;
    break;
  }

  effect->work = localId;
  effect->motionFlags = 0;
#ifdef UBFIX
  effect->update = FieldEffect_CopyLinkedObjectPosition;
#else
  effect->update =
      (void (*)(union FieldObject *))FieldEffect_CopyLinkedObjectPosition;
#endif
  visual = effect->sprite;
  visual->flags = 0;
  effect->linkedObject = objectEvent;

  // Replace the effect's priority bits (2 and 3) with 1 or the object event's.
  if (flags & ATTACHED_EFFECT_IN_FRONT) {
    s32 mask = 13;
    u8 attr = SPRITE_ATTR2_HIGH(visual);

    mask = -mask;
    mask &= attr;
    mask |= 4;
    SPRITE_ATTR2_HIGH(visual) = mask;
  } else {
    s32 priority = 12;
    u8 src = SPRITE_ATTR2_HIGH(objectEvent->sprite);
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
