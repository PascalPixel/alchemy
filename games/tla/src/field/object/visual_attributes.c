#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "actor.h"
#include "objdisp.h"
#include "field_sprite.h"
#include "edition.h"
#include "actor_view.h"

struct Actor *Actor_GetById(u32 objectId);

void Actor_ResetTargetAndSetMode1(u32 objectId) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    Actor_ResetMotion(object);
    Actor_SetMode(object, 1);
  }
}

void Actor_ArmCallback(s32 objectId, s32 angle, s32 frames) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->timer = angle;
    Actor_StartScriptFar(object, (u32)gActor_StepAngleScript);
    Event_Wait(frames);
  }
}

void Actor_ArmCallbackAndRefresh(s32 objectId, s32 angle) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->timer = angle;
    Actor_StartScriptFar(object, (u32)gActor_StepAngleScript);
    Actor_RefreshSelectorById(objectId);
  }
}

void Actor_SetSpritePriority(u32 objectId, s32 priority) {
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL && (object->animationKind & 0xF) == 1) {
    struct FieldSprite *sprite = object->sprite;

    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= ~1;
  }
}

void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId) {
  struct FieldSprite *sprite;
  u8 block;
  u32 tile;

  sprite = (struct FieldSprite *)Actor_GetById(sourceId);
  sprite = ((struct Actor *)sprite)->sprite;
  block = sprite->vramBlock;
  tile = sprite->tile;

  sprite = (struct FieldSprite *)Actor_GetById(targetId);
  sprite = ((struct Actor *)sprite)->sprite;
  sprite->vramBlock = block;
  sprite->tile = tile;
}

void Func_080d390c(u32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlagsFar(object, 0);
}

void Func_080d3928(u32 id, s32 rotation) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;

  if (object != NULL && sprite != NULL)
    sprite->rotation = rotation;
}

#include "types.h"
#include "dma.h"

void SetMapCellCollisionFar(s32 terrain, s32 x, s32 z, s32 value);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void Func_08038248(s32 resource, u8 *buffer);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void ResourceMetadata_ClearRecordFar(void *record);

struct ObjectSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 palette256 : 1;
  u16 shape : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 vramBlock;
  u8 unk11;
  u16 rotation;
  u8 unk14[2];
  s8 offsetX;
  s8 offsetY;
  u8 frame;
  u8 frameTimer;
  u8 nextFrame;
  u8 unk1b;
  u8 unk1c[0xc];
  void *record;
};

void Func_080d3940(u32 id, u32 angle) {
  s32 position[3];
  struct Actor *object = Actor_GetById(id);
  struct ObjectSprite *sprite = object->sprite;

  if (object != NULL && sprite != NULL) {
    Actor_SetMode(object, 0);
    angle = (angle + 0x2000) & 0xc000;
    position[0] = object->x;
    position[2] = object->z;
    SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
    Vector_AddPolarOffset(0x100000, angle, position);
    SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
    switch (angle) {
    case 0:
      sprite->offsetX -= 3;
      sprite->offsetY += 5;
      break;
    case 0x8000:
      sprite->offsetX += 2;
      sprite->offsetY += 4;
      break;
    case 0xc000:
      sprite->offsetY += 1;
      break;
    case 0x4000:
      sprite->offsetY += 5;
      break;
    }
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    sprite->nextFrame = 0;
    sprite->rotation = angle + 0x4000;
  }
}

void Func_080d3a10(s32 id, u32 angle, s32 mode) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->sprite;
  s32 position[3];

  if (object == NULL || sprite == NULL)
    return;
  if (mode != -1)
    Actor_SetMode(object, mode);
  position[0] = object->x;
  angle = (angle + 0x2000) & 0xc000;
  position[2] = object->z;
  SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
  Vector_AddPolarOffset(0x100000, angle, position);
  SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
  switch (angle) {
  case 0:
    object->x += 0x80000;
    object->z += 0x10000;
    ((s8 *)sprite->unk14)[2] -= 4;
    ((s8 *)sprite->unk14)[3] += 15;
    break;
  case 0x8000:
    object->x -= 0x80000;
    object->z += 0x10000;
    ((s8 *)sprite->unk14)[2] += 3;
    ((s8 *)sprite->unk14)[3] += 16;
    break;
  case 0xc000:
    ((s8 *)sprite->unk14)[3] += 1;
    break;
  case 0x4000:
    object->z += 0x80000;
    ((s8 *)sprite->unk14)[3] += 13;
    break;
  }
  object->collisionFlags = 0;
  object->priorityFlags |= 2;
  sprite->nextFrame = 0;
  sprite->rotation = angle + 0x4000;
}

#include "ram_buffer.h"
#include "heap_release.h"
#include "resource.h"

/// Gives an actor's sprite a still picture: resource, drawn by Func_08038248
/// into a cleared 128-byte buffer, or a blank one when bit 15 is set, loaded
/// into the sprite's VRAM block as its tile.
void Func_080d3b28(u32 id, s32 resource) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite;
  u8 *buffer;
  // One zero for the fill and the stores, set between the allocation and its
  // 0x400 offset; a literal 0 at each use differs.
  s32 zero;
  s32 tile;
  u8 kind;

  if (object != NULL) {
    kind = object->animationKind;
    if (kind == 1) {
      sprite = object->sprite;
      if (resource & 0x8000)
        resource = 0;
      buffer = HeapSlot_Alloc(68, 0x608);
      zero = 0;
      buffer += 0x400;
      Dma_Fill32(zero, buffer, 0x80);
      Func_08038248(resource, buffer);
      tile = VramCache_Load(sprite->vramBlock, 0x80, buffer);
      HeapSlot_Release(68);
      object->unk5c[0] = kind;
      ResourceMetadata_ClearRecordFar(sprite->record);
      sprite->record = NULL;
      sprite->unk1b[0] = zero;
      sprite->fullColor = zero;
      sprite->tile = tile;
      sprite->unk19 = zero;
      sprite->nextFrame = zero;
    }
  }
}

void Event_SetMessageId(s32 message) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->messageId = message;
}

s32 ObjectTable_ReadActiveValue(s32 key) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 result = -1;
  struct Actor *object = event->objects[key & 0x0fff];

  if (object != NULL && object->animationKind == 1)
    result = *((struct ObjectAnimationState *)object->sprite)->value28;
  return result;
}

s32 ObjectTable_FindActiveByValue(s32 value) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 result = -1;
  s32 index = 8;
  struct Actor *object = event->objects[index];

  if (object != NULL && object->animationKind == 1 &&
      *((struct ObjectAnimationState *)object->sprite)->value28 == value) {
    result = index;
  } else {
  next:
    index++;
    if (index <= 79) {
      object = event->objects[index];
      if (object == NULL || object->animationKind != 1 ||
          *((struct ObjectAnimationState *)object->sprite)->value28 != value)
        goto next;
      result = index;
    }
  }
  return result;
}

struct Actor;

s32 Func_080d5bec(s32 objectId, s32 *position);
void Text_GetMessageSizeAltFar(s32 message, s32 *left, s32 *top, u32 *width,
                               u32 *height);
void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top, u32 *width,
                            u32 *height);
s32 Portrait_GetForSpeakerFar(s32 sprite);
s32 Speaker_GetVoice(s32 sprite);
s32 Func_080380f8(s32 sprite, s32 zero, s32 x, s32 y);
s32 Message_AllTypedFar(void);

#define SPEAKER_PORTRAIT_LEFT 0x1000
#define SPEAKER_PORTRAIT_RIGHT 0x2000
#define SPEAKER_WINDOW_ABOVE 0x4000
#define SPEAKER_WINDOW_BELOW 0x8000

s32 FieldMessage_ShowAtActor(s32 localId, s32 unused) {
  u8 *win;
  struct EventWork *work;
  s32 sprite;
  s32 extra;
  s32 portraitTop;
  s32 margin;
  u32 height;
  u32 width;
  s32 top;
  s32 left;
  s32 pos[3];
  s32 handle;
  s32 portrait;
  s32 flags;
  s32 message;
  struct Actor *object;
  s32 x;
  s32 y;
  s32 found;
  s32 column;

  win = Ram_HeapSlots->windowTiles;
  work = Ram_HeapSlots->eventWork;
  handle = 0;
  portrait = 0;
  sprite = ObjectTable_ReadActiveValue(localId);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = localId & 0xf000;
  message = (s16)work->messageId;
  localId &= 0xfff;
  x = 0;
  object = ObjectTable_Get(localId);
  work->currentOwner = localId;
  y = 0;
  found = 0;
  if (work->waitMode == 0) {
    if (object != 0) {
      if (work->controlMode == 3) {
        Render_ProjectPoint((u8 *)object + 8, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
        y -= 2;
      } else {
        if (Func_080d5bec(localId, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (localId <= 7) {
      sprite = localId;
      object = ObjectTable_Get(Party_GetLeaderActor());
      if (work->controlMode == 3) {
        Render_ProjectPoint((u8 *)object + 8, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (Func_080d5bec(Party_GetLeaderActor(), pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    }

    if (!found) {
      left = 15;
      top = 10;
    } else {
      left = 0;
      top = 0;
      Text_GetMessageSizeAltFar(message, &left, &top, &width, &height);
      left = x - (s32)width / 2;
      if (flags & SPEAKER_WINDOW_ABOVE)
        top = y - height - 1;
      else if (!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    if (win[4] != 0)
      margin = 5;
    column = x;
    if (flags & SPEAKER_PORTRAIT_LEFT) {
      column = x - margin - 2;
      if (column < 0)
        column = 0;
    } else if (flags & SPEAKER_PORTRAIT_RIGHT) {
      column += 2;
      if (column + margin > 29)
        column = 29 - margin;
    } else if (column <= 15) {
      column = column - margin - 2;
      if (column < 0)
        column = x + 2;
    } else {
      column += 2;
      if (column + margin > 29)
        column = x - margin - 2;
    }

    found = Portrait_GetForSpeakerFar(sprite);
    if (found != -1) {
      Text_GetMessageSizeAltFar(message, &left, &top, &width, &height);
      message = -1;
      if (top > y)
        portraitTop = top - 5;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + 5 > 19)
        portraitTop = top - 5;
      if (top < portraitTop) {
        s32 lines = height;

        Text_GetMessageSizeFar(message, &left, &top, &width, &height);
        message = -1;
        extra = lines - height + 1;
      }
    } else if (top < y) {
      s32 lines = height;

      Text_GetMessageSizeFar(message, &left, &top, &width, &height);
      extra = lines - height + 1;
      message = found;
    }
    if (column < 0)
      column = 0;
    else if (column + margin > 29)
      column = 29 - margin;

    if (win[4] != 0) {
      wait(8);
      if (extra != 0)
        handle = Message_OpenWindowFar(message, left, top + extra - 1, 18);
      else
        handle = Message_OpenWindowFar(message, left, top, 2);
    } else {
      s32 voice = Speaker_GetVoice(sprite);

      if (extra != 0)
        handle = Message_OpenWindowFar(message, left, top + extra - 1,
                                       (voice << 16) | 17);
      else
        handle = Message_OpenWindowFar(message, left, top, (voice << 16) | 1);
    }
    if (win[4] != 0)
      portrait = Func_080380f8(sprite, 0, column, portraitTop);
    else
      portrait = Func_080380f8(sprite, 0, column, portraitTop);
    while (!Message_AllTypedFar())
      wait(1);
  }
  work->objectStatus = handle;
  *(s32 *)((u8 *)work + 0x1e8) = portrait;
  work->messageId++;
  return handle;
}

#include "input.h"

void Window_CloseMatchingIdFar(s32 id);
void Message_CloseFinishedWindowsFar(void);
void Func_080d4330(s32 key);

/// The held keys, read again at each test through a volatile pointer; as
/// plain reads, combined into one test of 0x305, 62 of 82 instructions of
/// Func_080d3fb0 differ in tla-en.
#define HELD (*(volatile u32 *)&gInput.held)

/// Opens the message at the object and waits for its window; after ten
/// seconds, or with Select, R, L and A held, the text is finished at once.
void Func_080d3fb0(s32 object, s32 message) {
  s32 masked;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  void *window = FieldMessage_ShowAtActor(object, message);
  s32 id;
  u32 frames;

  frames = 0;
  wait(1);
  id = ObjectTable_ReadActiveValue(object);
  if (object <= 7) {
    masked = object & 0x0fff;
    if (Scene_FindActorPlacement(masked) == 0)
      id = masked;
  }
  Window_CloseMatchingIdFar(id);

  if (work->waitMode == 0) {
    while (Window_IsClosedFar((s32)window) == 0) {
      wait(1);
      frames++;
      if (frames > 600 ||
          ((HELD & 4) && (HELD & 0x100) && (HELD & 0x200) && (HELD & 1)))
        Message_CloseFinishedWindowsFar();
    }
  }

  Func_080d4330(0x80000000);
  wait(1);
}

void Event_ShowMessageThenWait(s32 object, s32 message, s32 frames) {
  Func_080d3fb0(object, message);
  Event_Wait(frames);
}

void Event_ShowMessageAndWait(s32 object, s32 message) {
  Func_080d3fb0(object, message);
}

s32 Event_AskYesNoWithActorAnimation(s32 owner, s32 mode);

s32 Event_AskYesNoAndShowResponse(s32 object, s32 message) {
  s32 result;

  FieldMessage_ShowAtActor(object, message);
  result = Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0);
  if (result == 0) {
    Func_080d3fb0(object, message);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Func_080d3fb0(object, message);
  }
  return result;
}

void Event_NoOpUpdate(void) {}

/// Opens the next message for the speaker and waits until its window is idle.
void Func_080d40dc(s32 speaker) {
  // BUG: the position is never set; the window is placed from whatever the
  // two registers hold.
  struct {
    s32 x, y;
  } pos;
  struct EventWork *work = Ram_HeapSlots->eventWork;

  speaker &= 0xfff;
  ObjectTable_Get(speaker);
  work->currentOwner = speaker;
  if (work->waitMode == 0) {
    s32 px = pos.x;
    s32 py = pos.y;
    s32 window;

    if (py > 119)
      py += 32;
    else
      py -= 32;
    px = px < 8 ? 8 : px;
    if (px > 312)
      px = 312;
    py = py < 20 ? 20 : py;
    if (py > 220)
      py = 220;
    window = Message_OpenWindowFar((s16)work->messageId, px, py, 1);
    work->objectStatus = window;
    while (Window_IsClosedFar(window) == 0)
      wait(1);
  }
  work->messageId++;
}

#include "debug.h"
#include "macros.h"

void Event_ShowCounterAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);

void Event_ShowCounter(s32 a, s32 b, s32 c, s32 d) {
  Event_ShowCounterAtPosition(a, b, c, d);
}

void Event_ShowCounterAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
  s32 x0 = x;
  struct EventWork *state = Ram_HeapSlots->eventWork;
  s32 py = y;
  s32 px = x0;
  s32 minX = 8;
  s32 minY = 20;
  s32 ret;

  if (py > 119)
    py += 32;
  else
    py -= 32;

  if (x0 < minX)
    px = minX;
  if (px > 312)
    px = 312;
  CLAMP(py, minY, 220);

  ret = Message_OpenWindowFar((s16)state->messageId, px, py, 1);
  while (Window_IsClosedFar(ret) == 0)
    wait(1);
  state->messageId++;
}

// Field helpers, in ROM order: ending the window of an actor's speaker, the
// field camera (following an actor, its speed, the map bounds it keeps inside,
// waiting for it to arrive), copying a watched object's position, spawning an
// effect object that follows another, and a test of an actor's animation 108.

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

#include "map.h"

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
