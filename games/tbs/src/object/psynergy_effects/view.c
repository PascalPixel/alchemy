// Part of psynergy_effects.c, compiled only through it.
// The view bounds, zoom transitions and objects linked to an actor.

#include "../psynergy_effects_internal.h"

// The high byte of the first sprite part's OAM attribute 2.
// Bits 2 and 3 are its priority.
#define SPRITE_ATTR2_HIGH(sprite) (((u8 *)(sprite))[9])

// The event command "set view bounds": the camera keeps the screen inside
// these map coordinates (Camera_MoveWithinMapBounds).
void Map_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY) {
  struct MapScrollWork *work = gMapWork[0];

  work->minX = minX;
  work->minY = minY;
  work->maxX = maxX;
  work->maxY = maxY;
}

// Scheduler callback: moves the zoom one step from its previous value toward
// its target, and removes itself on the last step. It waits while the camera
// is paused.
// The scheduler calls void(void). Plain void changes the native return from
// pop {r1}; bx r1 to pop {r0}; bx r0 (two instructions in tbs-en).
#ifdef UBFIX
void FieldFx_StepRatioTransition(void) {
#else
s32 FieldFx_StepRatioTransition(void) {
#endif
  struct PerspectiveWork *work;
  s16 *frames;
  s32 *from;
  s16 *step;
  s32 zoom;
  s32 delta;

  work = gMapWork[0];
  if (((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                 sizeof(struct FieldWork)))
          ->camera->movementState != 0)
    return;
  frames = (s16 *)&work->zoomTransitionFrames;
  if (*frames == 0)
    return;
  from = &work->previousZoom;
  delta = work->zoom - *from;
  step = &work->zoomTransitionStep;
  (*step)++;
  /* FAKEMATCH(wrapper): the division written as a call to __divsi3, an
   * ordinary call where the operator makes a libcall block; as
   * delta * *step / *frames, 93 of 87 instructions differ in tbs-en. */
  zoom = *from + __divsi3(delta * *step, *frames);
  work->distance = Iwram_MulQ16(work->farPlane, zoom);
  gScanlinePitch = work->pitch + 1;
  if (*step == *frames) {
    *frames = 0;
#ifdef UBFIX
    Scheduler_RemoveCallback(FieldFx_StepRatioTransition);
#else
    Scheduler_RemoveCallback((SchedulerCallback)FieldFx_StepRatioTransition);
#endif
  }
}

// Starts a zoom to zoom (16.16) over frames frames, on the world map (event
// mode 3) only.
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames) {
  struct PerspectiveWork *work = gMapWork[0];
  s32 inverse;

  if (((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                 sizeof(struct FieldWork)))
          ->mode != MODE_WORLD_MAP)
    return;
  {
    inverse = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
  }
  work->previousZoom = work->zoom;
  work->zoom = inverse;
  work->zoomTransitionFrames = frames;
  work->zoomTransitionStep = 0;
#ifdef UBFIX
  Scheduler_AddOrUpdateCallback(FieldFx_StepRatioTransition, 0xc94);
#else
  Scheduler_AddOrUpdateCallback((SchedulerCallback)FieldFx_StepRatioTransition,
                                0xc94);
#endif
}

// Waits for a zoom transition to end (on the world map only, at most 300
// frames).
void Event_WaitForZoomTransition(void) {
  s32 frames;
  struct PerspectiveWork *work = gMapWork[0];

  if (((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                 sizeof(struct FieldWork)))
          ->mode == MODE_WORLD_MAP) {
    frames = 0;
    if (*(s16 *)&work->zoomTransitionFrames != 0) {
      do {
        WaitFrames(1);
        frames++;
      } while (frames < 300 && *(s16 *)&work->zoomTransitionFrames != 0);
    }
  }
}

// Per-frame update of a riding object: keeps it on the actor it rides (its
// linked object), raised by the actor's sprite height (its metadata's boxX) and
// 8 pixels. Its work holds the actor's id.
// The hook consumer ignores the result. Plain void removes the native zero
// result and changes its return register (three instructions in tbs-en).
#ifdef UBFIX
void BattleFx_CopyLinkedObjectPosition(union FieldObject *fieldObject) {
#else
s32 BattleFx_CopyLinkedObjectPosition(union FieldObject *fieldObject) {
#endif
  void *self = (void *)fieldObject;
  struct FieldActor *obj = self;
  struct FieldActor *host;

  host = obj->linkedObject;
  if (host != NULL) {
    obj->motionFlags = 0;
    obj->x.fixed = host->x.fixed;
    obj->y.fixed =
        host->y.fixed +
        ((s8)Resource_GetMetadataRecordFar(
             (s16)Scene_FindActorPlacement(*(s16 *)&obj->work)->sprite)
             ->boxX
         << 16) +
        (8 << 16);
    obj->terrainHeight = host->terrainHeight;
    obj->z.fixed = host->z.fixed;
  }
#ifndef UBFIX
  return 0;
#endif
}

// The event command "actor show emote": shows an emote bubble (object 21)
// over an actor, then waits wait frames. The low 4 bits of emote pick the
// bubble's animation (6 also plays sound 110); with 0x100 the bubble takes
// sprite priority 1, otherwise the actor's.
void BattleFx_SpawnLinked(s32 actor, s32 emote, s32 wait) {
  struct FieldActor *target;

  if ((emote & 0xff) == 6) {
    AudioCommand_PlayFar(SE_110);
  }

  target = ObjectTable_Get(actor);
  if (target != 0) {
    struct FieldActor *bubble =
        Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

    if (bubble != 0) {
      struct AnimationObject *sprite;

      ObjectDispatch_InitializeFar((struct DispatchObject *)bubble,
                                   gBattleFx_LinkedObjectScript);
      ObjectDispatch_SelectAnimationFar(bubble, emote & 15);
      bubble->motionFlags = 0;
      bubble->timer = 0;
      bubble->work = actor;
      sprite = bubble->sprite;
#ifdef UBFIX
      bubble->update = BattleFx_CopyLinkedObjectPosition;
#else
      bubble->update =
          (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
#endif
      sprite->flags = 0;
      bubble->linkedObject = target;

      // Replace the bubble's priority bits (2 and 3) with 1 or the actor's.
      if ((emote & 0x100) != 0) {
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
    FieldEvent_WaitFrames(wait);
  }
}

// The event command "actor set attached effect": with mode (the low 2 bits)
// 0 removes the object attached to an actor; 1 attaches one (object 209,
// animation 1) that stays until removed; 2 shows a one-off one (animation 2)
// that ends by itself. With 0x100 the object takes sprite priority 1,
// otherwise the actor's.
void BattleFx_ConfigureLinkedObject(s32 actor, s32 flags) {
  struct FieldActor *object = ObjectTable_Get(actor);
  struct FieldActor *effect;
  struct AnimationObject *visual;
  s32 mode;

  effect = 0;
  visual = 0;

  if (object == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == 2 || object->linkedObject == 0) {
      effect = Object_CreateFar(209, object->x.fixed, object->y.fixed,
                                object->z.fixed);
    }
  } else {
    effect = object->linkedObject;
    if (effect == 0)
      return;
    ObjectDispatch_ReleaseFar(effect);
    object->linkedObject = 0;
    return;
  }

  if (effect == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case 1:
    ObjectDispatch_SelectAnimationFar(effect, 1);
    object->linkedObject = effect;
    effect->timer = 1;
    break;
  case 2:
    ObjectDispatch_SelectAnimationFar(effect, 2);
    ObjectDispatch_InitializeFar((struct DispatchObject *)effect,
                                 (u32)gObjectMotion_WaitScript);
    effect->timer = 1;
    break;
  }

  effect->work = actor;
  effect->motionFlags = 0;
#ifdef UBFIX
  effect->update = BattleFx_CopyLinkedObjectPosition;
#else
  effect->update =
      (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
#endif
  visual = effect->sprite;
  visual->flags = 0;
  effect->linkedObject = object;

  // Replace the effect's priority bits (2 and 3) with 1 or the actor's.
  if (flags & 0x100) {
    s32 mask = 13;
    u8 attr = SPRITE_ATTR2_HIGH(visual);

    mask = -mask;
    mask &= attr;
    mask |= 4;
    SPRITE_ATTR2_HIGH(visual) = mask;
  } else {
    s32 priority = 12;
    u8 src = SPRITE_ATTR2_HIGH(object->sprite);
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
