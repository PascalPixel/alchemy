/// @file
/// The view bounds, zoom transitions and actors that ride another.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "sounds.h"
#include "field/field.h"
#include "field/perspective.h"
#include "field/emote.h"
#include "sprite_ids.h"

/// Actor_ShowEmoteById's emote: the bubble (enum Emote) in its low byte, of
/// which the low four bits pick its animation, then flags (EMOTE_IN_FRONT).
#define EMOTE_BUBBLE_MASK 0xff
#define EMOTE_ANIMATION_MASK 15

/// Sets the map coordinates the camera keeps the screen inside.
void MapGrid_SetCameraBounds(s32 minX, s32 minZ, s32 maxX, s32 maxZ) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  map->minX = minX;
  map->minZ = minZ;
  map->maxX = maxX;
  map->maxZ = maxZ;
}

/// Each frame of a World Map zoom: moves the zoom from zoomFrom toward zoomTo
/// over zoomFrames frames.
void WorldMap_StepZoom(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s16 *duration;
  s32 *from;
  s16 *step;
  s32 offset;
  s32 delta;

  if (((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                               sizeof(struct FieldWork)))
          ->camera->paused != 0)
    return;
  duration = &map->zoomFrames;
  if (*duration == 0)
    return;
  from = &map->zoomFrom;
  delta = map->zoomTo - *from;
  step = &map->zoomStep;
  (*step)++;
  offset = *from + __divsi3(delta * *step, *duration);
  map->zoom = Iwram_MulFX(map->farPlane, offset);
  gScanlinePitch = ((struct WorldMapView *)map)->pitch + 1;
  if (*step == *duration) {
    *duration = 0;
    Task_Remove((TaskFunc)WorldMap_StepZoom);
  }
}

/// Starts a zoom to zoom (16.16) over frames frames, on the World Map only.
void WorldMap_StartZoom(FIXED zoom, s32 frames) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 result;

  if (((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                               sizeof(struct FieldWork)))
          ->mode != ACTOR_SYSTEM_WORLD_MAP)
    return;
  {
    s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14Entry;
    result = ratio(zoom, FIXED_ONE);
  }
  map->zoomFrom = map->zoomTo;
  map->zoomTo = result;
  map->zoomFrames = frames;
  map->zoomStep = 0;
  Task_Add(WorldMap_StepZoom, TASK_PASS_FRAME + 20);
}

/// Waits for a zoom transition to end (on the World Map only, at most 300
/// frames).
void Event_WaitForZoomTransition(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 frames;

  if (((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                               sizeof(struct FieldWork)))
          ->mode == ACTOR_SYSTEM_WORLD_MAP) {
    frames = 0;
    if (map->zoomFrames != 0) {
      do {
        wait(1);
        frames++;
      } while (frames < 300 && map->zoomFrames != 0);
    }
  }
}

/// Per-frame update: keeps the actor on the actor it watches, raised by the
/// height of that actor's sprite and 8 pixels more.
s32 FieldEffect_CopyWatchedActorPosition(struct Actor *actor) {
  struct Actor *host;

  host = actor->watchedActor;
  if (host != NULL) {
    actor->motionFlags = 0;
    actor->x.fixed = host->x.fixed;
    actor->y.fixed =
        host->y.fixed +
        toFIXED((s8)Resource_GetMetadataRecordFarFar(
                    ((struct ActorPlacement *)Scene_FindActorPlacement(
                         (s16)actor->userValue))
                        ->sprite)
                    ->boxX) +
        toFIXED(8);
    actor->groundHeight = host->groundHeight;
    actor->z.fixed = host->z.fixed;
  }
  return 0;
}

extern const u8 gFieldEffect_WatchedActorScript[];

/// The event command "actor show emote": shows an emote bubble (SPRITE_EMOTE)
/// over an actor, then waits the given number of frames. Returns the bubble.
struct Actor *Actor_ShowEmoteById(s32 actorId, s32 emote, s32 frames) {
  struct Actor *target;
  struct Actor *bubble;

  if ((emote & EMOTE_BUBBLE_MASK) == EMOTE_LIGHT_BULB) {
    Sound_PlayFar(SE_CHIME);
  }

  target = Actor_Lookup(actorId);
  if (target != 0) {
    bubble = Actor_CreateFar(SPRITE_EMOTE, target->x.fixed, target->y.fixed,
                             target->z.fixed);
    if (bubble != 0) {
      struct Sprite *sprite;

      Actor_StartScriptFar(bubble,
                           (const s32 *)gFieldEffect_WatchedActorScript);
      Actor_SelectAnimationFar(bubble, emote & EMOTE_ANIMATION_MASK);
      bubble->motionFlags = 0;
      bubble->timer = 0;
      bubble->userValue = actorId;
      bubble->update =
          (void (*)(struct Actor *))FieldEffect_CopyWatchedActorPosition;
      bubble->watchedActor = target;
      sprite = bubble->sprite;
      sprite->flags = 0;

      // The bubble takes sprite priority 1, or the actor's.
      if ((emote & EMOTE_IN_FRONT) != 0) {
        s32 mask = ATTR2_HIGH_PRIORITY_MASK + 1;
        u8 attr = SPRITE_ATTR2_HIGH(sprite);

        /* FAKEMATCH(temporary): the mask's complement built as -(mask + 1) in
         * a local. */
        mask = -mask;
        mask &= attr;
        mask |= OBJ_PRIORITY(1) >> 8;
        SPRITE_ATTR2_HIGH(sprite) = mask;
      } else {
        s32 priority = ATTR2_HIGH_PRIORITY_MASK;
        u8 src = SPRITE_ATTR2_HIGH(target->sprite);
        u8 dst;
        s32 mask = ATTR2_HIGH_PRIORITY_MASK + 1;

        priority &= src;
        dst = SPRITE_ATTR2_HIGH(sprite);
        /* FAKEMATCH(temporary): the mask's complement built as -(mask + 1) in
         * a local. */
        mask = -mask;
        mask &= dst;
        mask |= priority;
        SPRITE_ATTR2_HIGH(sprite) = mask;
      }
    }
    Event_Wait(frames);
  }
  return bubble;
}

/// Returns 0 while the actor's sprite is an attached effect
/// (SPRITE_ATTACHED_EFFECT), else 1.
s32 Actor_IsNotAttachedEffect(struct Actor *actor) {
  struct Sprite *sprite;
  s32 value;

  if (actor == 0 || actor->spriteStorageKind != ACTOR_SPRITE_ONE ||
      (sprite = actor->sprite) == 0 || sprite->entries[0] == 0)
    return 1;
  value = sprite->entries[0]->animationId;
  value ^= SPRITE_ATTACHED_EFFECT;
  if (value != 0)
    return 1;
  return 0;
}
