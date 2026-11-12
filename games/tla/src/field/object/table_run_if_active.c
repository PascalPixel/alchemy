/// @file
/// Field helpers, in ROM order: ending the window of an actor's speaker, the
/// field camera (following an actor, its speed, the map bounds it keeps inside,
/// waiting for it to arrive), copying a watched object's position, spawning an
/// effect object that follows another, and a test of an actor's animation 108.

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "input.h"

s32 Window_CloseMatchingIdFar();
s32 ObjectTable_ReadActiveValue(s32 key);
s32 Speaker_GetVoice(s32 sprite);
s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 packed);
void Func_080380f8(s32 sprite, s32 zero, s32 x, s32 y);
s32 Message_AllTypedFar(void);
s32 Window_IsClosedFar(s32 window);
void Message_CloseFinishedWindowsFar(void);
void wait(s32 frames);

/// If the actor has graphics, ends the window of the speaker its graphics ID
/// names.
/// id: actor ID.
void ObjectTable_RunIfActive(s32 id) {
  s32 no;

  no = ObjectTable_ReadActiveValue(id);
  if (no != -1)
    Window_CloseMatchingIdFar(no);
}

/// The event command "show two messages and wait": two actors speak at once,
/// each message at its own (x, y) with its portrait at (portraitX,
/// portraitY), all in tiles. When delay is nonzero, the second message waits
/// for the first to finish, checking every delay frames. Waits for A, B, L or
/// R, then closes both.
// As ☀️'s function of the same name, which has no delay.
void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 firstX, s32 firstY,
                                      s32 firstPortraitX, s32 firstPortraitY,
                                      s32 second, s32 secondX, s32 secondY,
                                      s32 secondPortraitX, s32 secondPortraitY,
                                      s32 delay) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 firstSprite = ObjectTable_ReadActiveValue(first);
  s32 secondSprite = ObjectTable_ReadActiveValue(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = Message_OpenWindowFar((s16)event->messageId++, firstX, firstY,
                                      Speaker_GetVoice(firstSprite) << 16);
  Func_080380f8(firstSprite, 0, firstPortraitX, firstPortraitY);
  if (delay != 0) {
    while (!Message_AllTypedFar())
      wait(delay);
  }

  secondWindow =
      Message_OpenWindowFar((s16)event->messageId++, secondX, secondY,
                            Speaker_GetVoice(secondSprite) << 16);
  Func_080380f8(secondSprite, 0, secondPortraitX, secondPortraitY);

  while (!Message_AllTypedFar())
    wait(1);

  wait(1);
  while (!(gInput.pressed & 0x303))
    wait(1);

  wait(1);
  Window_CloseMatchingIdFar(firstSprite);
  Window_CloseMatchingIdFar(secondSprite);
  Message_CloseFinishedWindowsFar();
  wait(1);

  while (!Window_IsClosedFar(firstWindow))
    wait(1);
  while (!Window_IsClosedFar(secondWindow))
    wait(1);

  wait(1);
}

// Sets the message voice from the object `key` names (The Broken Seal's
// Message_SetVoiceFromActor): the voice sound from its value and the
// countdown from the event mode's table; 0x80000000 clears both.
#include "main_routine_work.h"
#include "window.h"

extern const u8 gData_080f330c[];

void Func_080d4330(s32 key) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u8 value;
  s32 id;

  if (key == 0x80000000) {
    *(s16 *)&work->speakerVoiceSound = (value = 0);
    *(s16 *)&work->speakerVoiceCountdown = value;
  } else {
    id = Speaker_GetVoice(ObjectTable_ReadActiveValue(key));
    value = gData_080f330c[gMainRoutineWork.eventMode];
    *(s16 *)&work->speakerVoiceSound = id;
    *(s16 *)&work->speakerVoiceCountdown = value;
  }
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "map.h"
#include "actor.h"
#include "macros.h"

void Camera_FollowActor(s32 id, s32 flag) {
  struct Actor *obj;
  struct Actor *target;
  struct EventWork *work;
  struct MapWork *map;

  obj = ObjectTable_Get(id);
  work = Ram_HeapSlots->eventWork;
  target = work->viewCenter;
  map = Ram_HeapSlots->mapWork;
  if (obj != 0) {
    map->origin = &target->x;
    Actor_StartCameraFar(target, obj);
    if (flag == 0) {
      SET_POSITION(target, obj->x, obj->y, obj->z);
      wait(1);
      if (work->controlMode != 3)
        Map_CenterOnFollowedPointFar();
    }
  }
}

void Camera_SetSpeedParameters(s32 first, s32 second) {
  struct EventWork *work = HeapSlot_AllocEwram(108, 0xd50);

  work->viewCenter->speedLimit = first;
  work->viewCenter->acceleration = second;
}

void Camera_MoveWithinMapBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                                s32 useSetter) {
  s32 shouldUseSetter;
  struct EventWork *runtimeBlock;
  s32 minimumX;
  s32 minimumZ;
  s32 maximumX;
  s32 objectZOffset;
  s32 maximumZ;
  s32 positionX;
  s32 positionZ;
  s32 positionY;
  struct MapWork *map;
  struct Actor *object;

  positionX = requestedX;
  positionY = requestedY;
  shouldUseSetter = useSetter;
  positionZ = requestedZ;
  runtimeBlock = HeapSlot_AllocEwram(108, 0xd50);
  object = runtimeBlock->viewCenter;
  map = Ram_HeapSlots->mapWork;
  minimumX = map->minX + 0x780000;
  objectZOffset = object->y;
  minimumZ = map->minZ + objectZOffset + 0x600000;
  maximumX = map->maxX + 0xFF880000;
  maximumZ = map->maxZ + objectZOffset + 0xFFC00000;
  map->origin = &object->x;
  Actor_ResetMotion(object);
  if (positionX == -1) {
    positionX = object->x;
  }
  if (positionY == -1) {
    positionY = object->y;
  }
  if (positionZ == -1) {
    positionZ = object->z;
  }
  if (positionX < minimumX) {
    positionX = minimumX;
  }
  if (positionZ < minimumZ) {
    positionZ = minimumZ;
  }
  if (positionX > maximumX) {
    positionX = maximumX;
  }
  if (positionZ > maximumZ) {
    positionZ = maximumZ;
  }
  if (shouldUseSetter == 0) {
    object->x = positionX;
    object->y = positionY;
    object->z = positionZ;
    wait(1);
    if (runtimeBlock->controlMode != 3) {
      Map_CenterOnFollowedPointFar();
    }
  } else {
    Actor_SetTargetPosition(object, positionX, positionY, positionZ);
  }
}

#include "heap_release.h"
#include "iwram_call.h"
#include "task.h"

s32 __divsi3(s32, s32);
extern u32 gData_03001144;

void Func_080d450c(s32 id, s32 use_setter) {
  struct Actor *obj = ObjectTable_Get(id);

  if (obj != NULL)
    Camera_MoveWithinMapBounds(obj->x, -1, obj->z, use_setter);
}

void Func_080d452c(s32 id, s32 use_setter) {
  struct Actor *obj = ObjectTable_Get(id);

  if (obj != NULL)
    Camera_MoveWithinMapBounds(obj->x, obj->y, obj->z, use_setter);
}

/// Waits (at most 600 frames) for the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrival(void) {
  Actor_CommitPosition(
      ((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->viewCenter);
  Event_Wait(2);
}

struct Actor *Func_080d4568(void) {
  return ((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->viewCenter;
}

void Func_080d4580(struct Actor *target, s32 keepCurrentPosition) {
  struct Actor *object;

  object = ((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->viewCenter;
  if (target != NULL) {
    Actor_StartCameraFar(object, NULL);
    object->watchedObject = target;
    if (keepCurrentPosition == 0) {
      SET_POSITION(object, target->x, target->y, target->z);
    }
  }
}

/// Sets the map coordinates the camera keeps the screen inside.
void MapGrid_SetCameraBounds(s32 minX, s32 minZ, s32 maxX, s32 maxZ) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  map->minX = minX;
  map->minZ = minZ;
  map->maxX = maxX;
  map->maxZ = maxZ;
}

void Func_080d45d8(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 *duration;
  s32 *from;
  s16 *step;
  s32 offset;
  s32 delta;

  if (((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))
          ->viewCenter->movementState != 0)
    return;
  duration = &map->zoomFrames;
  if (*duration == 0)
    return;
  from = &map->zoomFrom;
  delta = map->zoomTo - *from;
  step = &map->zoomStep;
  (*step)++;
  offset = *from + __divsi3(delta * *step, *duration);
  map->zoom = Iwram_MulQ16(map->farPlane, offset);
  gData_03001144 = *(u16 *)((u8 *)map + 0x11c) + 1;
  if (*step == *duration) {
    *duration = 0;
    Task_Remove(Func_080d45d8);
  }
}

void Func_080d46a4(s32 zoom, s32 frames) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 result;

  if (((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->controlMode != 3)
    return;
  {
    s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14;
    result = ratio(zoom, 0x10000);
  }
  map->zoomFrom = map->zoomTo;
  map->zoomTo = result;
  map->zoomFrames = frames;
  map->zoomStep = 0;
  Task_Add(Func_080d45d8, TASK_PASS_VBLANK + 20);
}

void Func_080d4714(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 frames;

  if (((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->controlMode == 3) {
    frames = 0;
    if (map->zoomFrames != 0) {
      do {
        wait(1);
        frames++;
      } while (frames <= 0x12b && map->zoomFrames != 0);
    }
  }
}

#include "sprite_metadata.h"
#include "scene.h"

s32 BattleEffect_CopyWatchedObjectPosition(struct Actor *object) {
  struct Actor *link;

  link = object->watchedObject;
  if (link != NULL) {
    object->motionFlags = 0;
    object->x = link->x;
    object->y = link->y +
                ((s8)Resource_GetMetadataRecordFarFar(
                     *(s16 *)Scene_FindActorPlacement((s16)object->unk66))
                     ->boxX
                 << 16) +
                0x80000;
    object->terrainHeight = link->terrainHeight;
    object->z = link->z;
  }
  return 0;
}

#include "objdisp.h"

struct BattleEffectVisual {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[16];
  u8 value26;
};

struct BattleEffectResource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[60];
  struct BattleEffectVisual *visual;
};

struct BattleEffectWatchedObject {
  u8 unk00[80];
  struct BattleEffectVisual *visual;
  u8 value54;
  u8 value55;
  u8 unk56[14];
  u16 counter;
  u16 resourceId;
  struct BattleEffectResource *resource;
  void (*callback)(void);
};

struct BattleEffectWatchedObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
extern const u8 gData_080f3310[];

/// The Broken Seal's BattleEffect_SpawnLinked with this game's object kind
/// (488): spawns an effect object at object resourceId that follows it,
/// waits state, and returns the new object.
struct BattleEffectWatchedObject *Func_080d47b4(s32 resourceId, s32 flags,
                                                s32 state) {
  struct BattleEffectResource *resource;
  struct BattleEffectWatchedObject *object;

  if ((flags & 0xff) == 6) {
    Sound_PlayFar(110);
  }

  resource = ObjectTable_Get(resourceId);
  if (resource != 0) {
    object = Func_080200c0(488, resource->x, resource->y, resource->z);

    if (object != 0) {
      Actor_StartScriptFar((struct Actor *)object, (u32)gData_080f3310);
      Actor_SetMode(object, flags & 15);
      object->value55 = 0;
      object->counter = 0;
      object->resourceId = resourceId;
      object->callback = BattleEffect_CopyWatchedObjectPosition;
      object->resource = resource;
      object->visual->value26 = 0;

      if ((flags & 0x100) != 0) {
        s32 mask = 13;
        u8 visualFlags = object->visual->flags;

        mask = -mask;
        mask &= visualFlags;
        mask |= 4;
        object->visual->flags = mask;
      } else {
        s32 copiedFlags = 12;
        u8 sourceFlags = resource->visual->flags;
        u8 flags;
        s32 clearMask = 13;

        copiedFlags &= sourceFlags;
        flags = object->visual->flags;
        clearMask = -clearMask;
        clearMask &= flags;
        clearMask |= copiedFlags;
        object->visual->flags = clearMask;
      }
    }
    Event_Wait(state);
  }
  return object;
}

s32 Actor_IsOutsideAnimation108(struct Actor *object) {
  struct ObjectAnimationState *state;
  s32 value;

  if (object == 0 || object->animationKind != 1 ||
      (state = object->sprite) == 0 || state->value28 == 0)
    return 1;
  value = *state->value28;
  value ^= 0x108;
  if (value != 0)
    return 1;
  return 0;
}
