// ---- games/tla/src/field/object/visual_attributes.c ----
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
#undef SPEAKER_PORTRAIT_LEFT
#undef SPEAKER_PORTRAIT_RIGHT
#undef SPEAKER_WINDOW_ABOVE
#undef SPEAKER_WINDOW_BELOW
#undef HELD

// ---- games/tla/src/field/event_commands/f0d489c.c ----
struct LinkedEffectVisual {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[16];
  u8 value26;
};

struct LinkedEffectObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[60];
  struct LinkedEffectVisual *visual;
  u8 value54;
  u8 value55;
  u8 unk56[14];
  u16 counter;
  u16 resourceId;
  struct LinkedEffectObject *resource;
  void (*callback)(void);
};

void Actor_StartScriptFar(struct Actor *object, const s32 *script);
extern const u8 gData_080f341c[];

/// The Broken Seal's BattleEffect_ConfigureWatchedObject with this game's
/// calls: with flags & 3 zero it destroys the object's linked effect;
/// otherwise (mode 2, or when the linked effect is outside animation 0x108)
/// it creates effect object 264 at the object and links it, mode 1 keeping it
/// as the object's resource and mode 2 running gData_080f341c on it.
void Func_080d489c(s32 id, s32 flags) {
  struct LinkedEffectObject *object = ObjectTable_Get(id);
  struct LinkedEffectObject *child;
  struct LinkedEffectVisual *visual;
  s32 mode;

  child = 0;
  visual = 0;

  if (object == 0)
    return;

  if ((flags & 3) != 0) {
    if ((flags & 3) == 2 || Actor_IsOutsideAnimation108(object->resource)) {
      child = Func_080200c0(264, object->x, object->y, object->z);
    }
  } else {
    child = object->resource;
    if (child == 0)
      return;
    Actor_Destroy(child);
    object->resource = NULL;
    return;
  }

  if (child == 0)
    return;

  mode = flags & 3;
  switch (mode) {
  case 1:
    Actor_SetMode(child, 1);
    object->resource = child;
    child->counter = 1;
    break;
  case 2:
    Actor_SetMode(child, 2);
    Actor_StartScriptFar(child, gData_080f341c);
    child->counter = 1;
    break;
  }

  child->resourceId = id;
  child->value55 = 0;
  child->callback = BattleEffect_CopyWatchedObjectPosition;
  visual = child->visual;
  visual->value26 = 0;
  child->resource = object;

  if (flags & 0x100) {
    s32 mask = 13;
    u8 visualFlags = visual->flags;

    mask = -mask;
    mask &= visualFlags;
    mask |= 4;
    visual->flags = mask;
  } else {
    s32 copiedFlags = 12;
    u8 sourceFlags = object->visual->flags;
    u8 destinationFlags;
    s32 clearMask = 13;

    copiedFlags &= sourceFlags;
    destinationFlags = visual->flags;
    clearMask = -clearMask;
    clearMask &= destinationFlags;
    clearMask |= copiedFlags;
    visual->flags = clearMask;
  }
}


s32 __divsi3(s32, s32);

s32 Actor_ClearChildAndCollisionFlags(struct Actor *obj) {
  Actor_SetSingleChildFlagsFar((struct Actor *)obj, 0);
  obj->collisionFlags = 0;
  return 0;
}

s32 Actor_ClearChildFlags(struct Actor *obj) {
  Actor_SetSingleChildFlagsFar((struct Actor *)obj, 0);
  return 0;
}

struct ObjectFlags23 {
  u8 unk00[0x23];
  u8 flags23;
};

s32 Actor_SetPriorityBit5(struct ObjectFlags23 *obj) {
  obj->flags23 |= 0x20;
  return 0;
}

s32 Actor_MoveTowardTarget(struct Actor *object) {
  struct Actor *target;
  s32 deltaX;
  s32 deltaY;
  s32 cellX;
  s32 cellY;
  s32 newX;
  s32 distance;
  s32 step;

  target = object->watchedObject;
  if (target != 0) {
    deltaX = target->x - object->x;
    if (deltaX < 0)
      deltaX += 0xffff;
    cellX = deltaX >> 16;
    deltaY = target->z - object->z;
    if (deltaY < 0)
      deltaY += 0xffff;
    cellY = deltaY >> 16;
    distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
    step = object->timer;
    if (distance >= step) {
      newX = object->x + __divsi3(cellX << 20, step);
      Actor_SetTargetPosition(object, newX, object->y,
                              object->z + __divsi3(cellY << 20, step));
      Actor_SetMode(object, 2);
    } else {
      Actor_SetMode(object, 1);
    }
  }
  return 1;
}

// Objects: turn toward the linked target.

#include "facing_object.h"

s32 ArcTan2(s32, s32);

/// Turns the object toward its facing target, if it has one, by at most
/// 0x1000 (22.5 degrees).
/// obj: object to turn.
s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *obj) {
  s32 turn;
  u16 old;
  s32 dir;
  struct FacingObject *target;

  target = obj->facingTarget;
  if (target != NULL) {
    obj->facingFlags = 0xfe & obj->facingFlags;
    dir = (u16)ArcTan2(target->positionZ - obj->positionZ,
                       target->positionX - obj->positionX);
    old = obj->facing;
    turn = (s16)(dir - old);
    if (turn != 0) {
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      obj->facing = old + turn;
    }
  }
  return 1;
}

/// Makes the object watch target and keeps it facing it.
void Func_080d4aa8(struct Actor *object, struct Actor *target) {
  object->watchedObject = target;
  object->update = SceneActor_UpdateFacingTowardTarget;
}


struct ObjectLayerAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[27];
  u8 unused25 : 2;
  u8 shadowLayer : 2;
};

/// Gives the object the party leader's display layer.
s32 Func_080d4ab4(struct Actor *object) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  s32 layer = ((struct ObjectLayerAnimation *)leader->sprite)->layer;

  if ((object->animationKind & 0xf) == 1) {
    struct ObjectLayerAnimation *animation = object->sprite;

    animation->layer = layer;
    animation->shadowLayer = layer;
    object->priorityFlags &= 0xfe;
  }
  return 0;
}

struct Actor *Actor_GetById(u32 objectId);

extern const u8 gData_080f34e4[];
extern const u8 gData_080f3428[];
extern const u8 gData_080f350c[];
extern const u8 gData_080f35c8[];
extern const u8 gData_080f373c[];
extern const u8 gData_080f3750[];
extern const u8 gData_080f34e8[];
extern const u8 gData_080f3614[];
extern const u8 gData_080f3764[];
extern const u8 gData_080f3500[];
extern const u8 gData_080f34f4[];

void Actor_SetActionCallback(struct Actor *object, s32 kind) {
  switch (kind) {
  case 1:
    kind = (s32)gData_080f34e4;
    break;
  case 2:
    kind = (s32)gData_080f3428;
    break;
  case 3:
    kind = (s32)gData_080f350c;
    break;
  case 4:
    kind = (s32)gData_080f35c8;
    break;
  case 5:
    kind = (s32)gData_080f373c;
    break;
  case 6:
    object->watchedObject = Actor_GetById(gMainRoutineWork.currentOwner);
    kind = (s32)gData_080f3750;
    break;
  case 7:
    kind = (s32)gData_080f34e8;
    break;
  case 8:
    kind = (s32)gData_080f3614;
    break;
  case 9:
    kind = (s32)gData_080f3764;
    break;
  case 10:
    kind = (s32)gData_080f3500;
    break;
  case 11:
    kind = (s32)gData_080f34f4;
    break;
  }
  Actor_StartScriptFar(object, (const void *)kind);
}


u32 Func_080202f0(s32 terrain, s32 x, s32 z);

#include "fixed_math.h"

struct ObjectAnimationEntry {
  s16 animId;
};

struct ObjectAnimation {
  u8 unk00[0x28];
  struct ObjectAnimationEntry *entries[1];
};

/// The Broken Seal's Actor_FindNearestFacingTarget: the nearest object
/// within 40 pixels in the direction self faces, if its first animation is id.
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 id) {
  struct Actor *entry;
  struct Actor *found;
  s32 cnt;
  s32 best;
  s32 dy;
  s32 dx;
  s32 dz;
  s32 dist;
  s32 angle;
  s32 turn;

  found = NULL;
  best = 40;
  entry = Ram_HeapSlots->scriptObjects;
  for (cnt = 0; cnt < 64; cnt++, entry++) {
    if (entry->script == NULL)
      continue;
    if (entry == self)
      continue;
    if (entry->animationKind != 1)
      continue;
    dy = entry->y - self->y;
    if (dy >= 0) {
      if (dy > 0x2fffff)
        continue;
    } else {
      if (self->y - entry->y > 0x2fffff)
        continue;
    }
    dx = (entry->x - self->x) / Q16_ONE;
    dz = (entry->z - self->z) / Q16_ONE;
    dist = Iwram_Sqrt(dx * dx + dz * dz);
    if (dist >= best)
      continue;
    angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
    if (dist > 23) {
      turn = (s16)(angle - self->facing);
      if (turn < -0x2fff)
        continue;
      if (turn > 0x2fff)
        continue;
    }
    found = entry;
    best = dist;
  }
  if (found == NULL)
    return NULL;
  if (((struct ObjectAnimation *)found->sprite)->entries[0]->animId != id)
    return NULL;
  return found;
}

s32 Func_080d4ccc(struct Actor *object, s32 *position) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s8 *event = Ram_HeapSlots->eventWork;

  if (map == 0 || event[0x18a] == 3 ||
      Func_080202f0(object->terrainId, position[0], position[2]) <= 2)
    return 0;
  return 1;
}


struct JumpSprite {
  u8 unk00[26];
  u8 flags;
};

struct JumpEventWork {
  u8 unk000[0x19c];
  s32 walkingDistanceScale;
  s32 walkingDistance;
};

extern struct MapCell gMapCellBuffer[];

void Event_Start(void);
void Event_End(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
s32 Func_08020210(struct Actor *object, s32 *position);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void Func_080203a0(struct Actor *object, s32 kind);
s32 GetMapCellCollisionFar(s32 layer, s32 x, s32 z);

/// The leader jumps two tiles forward, over the tile in
/// front, when it faces a quarter turn, the map allows it (Func_080d4ccc and
/// the step check) or a platform object (flag 0x80) waits 16 pixels below the
/// landing, and no solid object stands there. A platform underfoot (sprite 258
/// or 259 with flag 2) dips 2 pixels with the leader. Each jump advances
/// walking distance, and the leader keeps jumping while it lands on a jump spot
/// (collision 254). Returns 0 after a jump, -1 when the leader cannot jump.
s32 Func_080d4d08(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 result = -1;
  s32 angle = leader->facing;
  u8 *flags = &leader->motionFlags;
  u8 saved = *flags;
  struct JumpEventWork *work = Ram_HeapSlots->eventWork;
  s16 spriteFlags = 1;
  s32 position[3];
  s32 *positionWords;
  struct Actor *entry;
  struct Actor *platform;
  s32 canJump;
  s32 i;

  if (gMainRoutineWork.renderMode == 2)
    return result;
  if (gMainRoutineWork.renderMode == 3)
    return result;
  if ((angle + 0x1000) & 0x2000) // not facing a quarter turn
    return result;
  angle = (angle + 0x2000) & 0xc000;
  positionWords = position;
again:
  // The middle of the tile in front, then of the tile beyond.
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x100000, angle, position);
  if (Func_08020210(leader, position) == 1)
    return -1;
  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffset(0x200000, angle, position);
  canJump = 0;
  if (Func_080d4ccc(leader, position) == 0 &&
      Func_08020210(leader, position) == 0)
    canJump = 1;

  // A platform 16 pixels below the landing makes it a jump spot.
  if (leader->priorityFlags & 0x40) {
    entry = Ram_HeapSlots->scriptObjects;
    for (i = 0; i < 64; i++, entry++) {
      s32 bound = 0x7ffff;
      s32 delta;
      s32 dy;

      if (entry->script == NULL)
        continue;
      if (!(entry->priorityFlags & 0x80))
        continue;
      if (entry->unk56 != 0)
        continue;
      if (entry == leader)
        continue;
      if ((position[0] - entry->x + 0x80000) >> 20 != 0)
        continue;
      if ((position[2] - entry->z + 0x80000) >> 20 != 0)
        continue;
      delta = position[1] - entry->y;
      dy = delta - 0x100000;
      if (dy >= 0) {
        if (dy <= bound)
          goto hit;
      } else {
        dy = 0x100000 - delta;
        if (dy <= bound) {
        hit:
          canJump = 1;
          break;
        }
      }
    }
  }

  // A solid object at the landing blocks it.
  entry = Ram_HeapSlots->scriptObjects;
  for (i = 0; i < 64; i++, entry++) {
    if (entry->script != NULL && !(entry->priorityFlags & 0x80) &&
        entry != leader && entry->collisionFlags != 0 &&
        (position[0] - entry->x + 0x80000) >> 20 == 0 &&
        (position[2] - entry->z + 0x80000) >> 20 == 0) {
      s32 d = position[1] - entry->y;
      if (d >= 0) {
        if (d <= 0x7ffff)
          goto hit2;
      } else if (entry->y - position[1] <= 0x7ffff) {
      hit2:
        canJump = 0;
        break;
      }
    }
  }

  if (canJump) {
    struct MapWork *map;
    u8 attr;

    // Crouch, then jump there.
    if (leader->animationKind == 1)
      spriteFlags = ((struct JumpSprite *)leader->sprite)->flags;
    Event_Start();
    Actor_SetMode(leader, 6);
    wait(6);
    Sound_PlayFar(152);
    Actor_SetMode(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    *flags &= 0x7e;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags & 0xfe);
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner,
                               ((s16 *)positionWords)[1],
                               ((s16 *)positionWords)[5]);
    Actor_SetMode(leader, 6);
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);

    map = Ram_HeapSlots->mapWork;
    attr = map->unk017;
    if (attr & 12) {
      s32 cellX = leader->x >> 20;
      s32 cellZ = leader->z >> 20;
      struct MapCell *cells;

      if (map == NULL)
        cells = gMapCellBuffer;
      else
        cells = map->layers[leader->terrainId & 3].cells;
      cells += cellX + (cellZ << 7);
      if (cells->attributeB & 16)
        Func_080203a0(leader, (u32)(attr << 28) >> 30);
    }

    // A platform underfoot dips 2 pixels with the leader and comes back up.
    if (((platform = Actor_FindNearestFacingTarget(leader, 258)) != NULL ||
         (platform = Actor_FindNearestFacingTarget(leader, 259)) != NULL) &&
        (platform->priorityFlags & 2)) {
      Actor_SetMode(platform, 7);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      wait(2);
      leader->y -= 0x10000;
      leader->terrainHeight -= 0x10000;
      wait(10);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
      wait(4);
      leader->y += 0x10000;
      leader->terrainHeight += 0x10000;
    } else {
      wait(6);
    }
    *flags = saved;
    Event_End();
    if (work != NULL)
      work->walkingDistance +=
          Iwram_MulQ16(work->walkingDistanceScale, 0x200000);
    if (GetMapCellCollisionFar(leader->terrainId, position[0], position[2]) ==
        254) {
      Actor_SetMode(leader, 1);
      wait(6);
      goto again;
    }
    result = 0;
  }
  leader->timer = 8;
  leader->unk66 = 0;
  return result;
}

struct ClimbSprite {
  u8 unk00[26];
  u8 flags;
};

void Event_Wait(s32 frames);
s32 Func_08020258(s32 *position);
s32 Field_CheckSpheresOverlapFar(s32 *position, s32 radius, s32 *other,
                                 s32 otherRadius);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_WaitForMoveById(s32 id);

s32 Func_080d50f8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 gridX;
  s16 spriteFlags;
  struct MapCell *ground;
  s32 gridY;
  s32 gridZ;
  s32 position[3];

  spriteFlags = 1;
  gridX = 8 + ((leader->x >> 16) & 0xfff0);
  gridY = 8 + ((leader->y >> 16) & 0xfff0);
  gridZ = 8 + ((leader->z >> 16) & 0xfff0);
  Event_Start();
  if (leader->animationKind == 1)
    spriteFlags = ((struct ClimbSprite *)leader->sprite)->flags;
  if (gMainRoutineWork.renderMode == 0) {
    struct Actor *entry;
    struct MapCell *cell;
    struct MapCell *south;
    s32 cellX = gridX / 16;
    s32 cellY = gridY / 16;
    s32 cellZ = gridZ / 16;
    s32 radius;
    s32 i;

    ground = &map->layers[leader->terrainId].cells[cellX + cellZ * 128];
    cell = ground;
    south = cell + 128;
    if (!(cell->attributeB & 0x40) || !(south->attributeB & 0x40))
      goto failure;
    cell = &map->layers[2].cells[cellX + (cellZ - cellY) * 128];
    south = cell + 128;
    if (south->collisionCode == 255)
      goto failure;
    if (Map_GetTerrainHeightFar(leader->terrainId, gridX << 16, gridZ << 16) <=
        Map_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                (gridZ + 16) << 16))
      goto failure;
    position[0] = leader->x;
    position[1] = leader->y - 0x100000;
    position[2] = leader->z;
    if (Func_08020258(position) != 0)
      goto failure;
    radius = leader->radius - 2;
    position[0] = leader->x;
    position[1] = leader->y - 0x200000;
    position[2] = leader->z + 0x100000;
    entry = Ram_HeapSlots->scriptObjects;
    for (i = 0; i < 64; i++, entry++) {
      if (entry->script != NULL && (entry->collisionFlags & 1) &&
          entry != leader &&
          Field_CheckSpheresOverlapFar(&entry->x, radius, position,
                                       entry->radius - 2) >= 0)
        goto failure;
    }

    // Step to the middle of the tile, face north and climb down a tile.
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, gridX, gridZ);
    leader->speedLimit = 0x10000;
    Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
    Actor_RefreshSelectorById(gMainRoutineWork.currentOwner);
    leader->actionFlags = 1;
    leader->motionFlags = 0;
    spriteFlags &= 0xfe;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    Actor_SetMode(leader, 13);
    Actor_SetTargetPosition(leader, gridX << 16, leader->y - 0x100000,
                            (gridZ << 16) + 0x100000);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    gMainRoutineWork.renderMode = 2;
    leader->z = (leader->z & 0xfff00000) + 0x80000;
  } else {
    // Let go of the wall and fall.
    Actor_SetMode(leader, 10);
    leader->motionFlags = 3;
    leader->velocityY = 0x40000;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    Event_Wait(6);
    gMainRoutineWork.renderMode = 0;
    leader->actionFlags = 1;
    leader->facing = 0xc000;
    leader->timer = 0;
    leader->unk66 = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

s32 Func_080d53b8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 spriteFlags = 1;
  struct MapCell *ground;
  s32 gridX = 8 + ((leader->x >> 16) & 0xfff0);
  s32 gridY = 8 + ((leader->y >> 16) & ~15);
  s32 gridZ = 8 + ((leader->z >> 16) & 0xfff0);
  s32 position[3];
  s32 result;

  Event_Start();
  if (leader->animationKind == 1)
    spriteFlags = ((struct ClimbSprite *)leader->sprite)->flags;
  if (gMainRoutineWork.renderMode == 0) {
    struct MapCell *cell;
    struct MapCell *north;
    s32 cellX = gridX >> 4;
    s32 cellY = gridY >> 4;
    s32 cellZ = gridZ >> 4;

    ground = &map->layers[leader->terrainId].cells[cellX + cellZ * 128];
    cell = ground;
    north = cell - 128;
    if (!(cell->attributeB & 0x40) || !(north->attributeB & 0x40))
      goto failure;
    cell = &map->layers[2].cells[cellX + (cellZ - cellY) * 128];
    north = cell - 128;
    if (north->collisionCode == 255)
      goto failure;
    if (Map_GetTerrainHeightFar(leader->terrainId, gridX << 16, gridZ << 16) >=
        Map_GetTerrainHeightFar(leader->terrainId, gridX << 16,
                                (gridZ - 16) << 16))
      goto failure;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    result = Func_08020258(position);
    if (result != 0)
      goto failure;

    // Step to the middle of the tile, crouch, jump and grab the wall.
    leader->actionFlags = result;
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, gridX, gridZ);
    Actor_SetMode(leader, 6);
    wait(4);
    Actor_SetMode(leader, 7);
    leader->velocityY = 0x40000;
    wait(4);
    leader->motionFlags = result;
    spriteFlags &= 0xfe;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    leader->speedLimit = 0x10000;
    leader->velocityY = result;
    Actor_SetMode(leader, 12);
    wait(4);
    gMainRoutineWork.renderMode = 2;
    leader->actionFlags = 1;
    wait(8);
    leader->z = (leader->z & 0xfff00000) + 0x80000;
  } else {
    struct Actor *entry;
    s32 radius = leader->radius - 2;
    s32 i;

    position[0] = leader->x;
    position[1] = leader->y + 0x100000;
    position[2] = leader->z - 0x100000;
    i = 0;
    entry = Ram_HeapSlots->scriptObjects;
    for (; i < 64; i++, entry++) {
      if (entry->script != NULL && (entry->collisionFlags & 1) &&
          entry != leader &&
          Field_CheckSpheresOverlapFar(&entry->x, radius, position,
                                       entry->radius - 2) >= 0)
        goto failure;
    }

    // Climb over the top: up 8 pixels and a tile north.
    leader->motionFlags = 0;
    Actor_SetMode(leader, 11);
    Actor_SetTargetPosition(leader, gridX << 16, leader->y + 0x80000,
                            (gridZ << 16) - 0x100000);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    leader->motionFlags = 3;
    leader->terrainHeight = leader->y;
    spriteFlags |= 1;
    Actor_SetSingleChildFlagsFar(leader, spriteFlags);
    Event_Wait(4);
    gMainRoutineWork.renderMode = 0;
    leader->actionFlags = 1;
    leader->timer = 0;
    leader->unk66 = 0;
  }
  Event_End();
  return 0;

failure:
  Event_End();
  return -1;
}

#include "work9c.h"

struct Work9cItemPosition {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

struct Work9cItem *Func_080d5670(s32 *position) {
  struct Work9cItem *found;
  struct Work9c *work;
  struct Work9cItem *item;
  s32 best;
  s32 i;

  found = NULL;
  work = WORK_9C;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH(barrier): an empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) after the
   * workspace test, where if-conversion would hoist it; without it, 29 of 85
   * instructions differ in tla-en. */
  asm("");
  item = work->items;
  best = 0x10000000;
  for (i = 0; i < 128; i++, item++) {
    if (item->used == 1) {
      struct Work9cItemPosition *at = (struct Work9cItemPosition *)item;
      s32 dx = (position[0] - at->x) >> 8;
      s32 dy = (position[1] - at->y) >> 8;
      s32 dz = (position[2] - at->z) >> 8;
      s32 distance =
          Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

      if (distance <= 0x400 && best > distance) {
        best = distance;
        found = item;
      }
    }
  }
  return found;
}


struct GrabSave {
  u8 unk00[98];
  u8 priority;
};

struct EventWorkGrab {
  u8 unk000[388];
  s16 grabState;
};

s32 Func_080d5720(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  struct Work9cItemPosition *item = Func_080d5670(&leader->x);
  u8 *flags;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  if (item == NULL)
    return 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->watchedObject = (struct Actor *)item;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, item->x, item->y, item->z);
  Actor_CommitPosition(leader);
  leader->timer = leader->motionFlags;
  flags = &leader->priorityFlags;
  leader->unk66 = sprite->nextFrame;
  ((struct GrabSave *)leader)->priority = sprite->priority;
  leader->priorityFlags = *flags & ~1;
  sprite->nextFrame = 0;
  sprite->priority = 1;
  gMainRoutineWork.renderMode = 3;
  return 1;
}


s32 Func_080201d0(s32 layer, s32 x, s32 z);

extern const u16 Data_080f0890[];

s32 Func_080d57e4(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  s32 position[3];
  u32 angle;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  angle = Data_080f0890[(gInput.held >> 4) & 15];
  if (angle == 0xffff)
    return 0;
  leader->x = (leader->x & 0xfff00000) + 0x80000;
  leader->z = (leader->z & 0xfff00000) + 0x80000;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  leader->terrainHeight = leader->y - 0x80000;
  if (Func_080201d0(leader->terrainId, position[0], position[2]) ||
      Func_08020210(leader, position))
    return 0;
  leader->facing = angle;
  leader->priorityFlags |= 1;
  leader->motionFlags = leader->timer;
  leader->scaleX = 0x10000;
  sprite->nextFrame = leader->unk66;
  sprite->rotation = 0;
  Sound_PlayFar(152);
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->acceleration = 0x10000;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, position[0], position[1], position[2]);
  Actor_CommitPosition(leader);
  sprite->priority = ((u8 *)leader)[98];
  gMainRoutineWork.renderMode = 0;
  return 1;
}

#include "worka0.h"

struct WorkA0Node *Func_080d5930(s32 *position) {
  struct WorkA0Node *found;
  struct WorkA0 *work;
  struct WorkA0Node *item;
  s32 best;
  s32 i;

  found = NULL;
  work = WORK_A0;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH(barrier): an empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) after the
   * workspace test, where if-conversion would hoist it; without it, 29 of 84
   * instructions differ in tla-en. */
  asm("");
  item = work->items;
  best = 0x10000000;
  for (i = 0; i < 128; i++, item++) {
    if (item->used == 1) {
      s32 dx = (position[0] - item->x) >> 8;
      s32 dy = (position[1] - item->y) >> 8;
      s32 dz = (position[2] - item->z) >> 8;
      s32 distance =
          Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

      if (distance <= 0x400 && best > distance) {
        best = distance;
        found = item;
      }
    }
  }
  return found;
}

s32 Func_080d59dc(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  struct Actor *leader = ObjectTable_Get(work->currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  struct Work9cItemPosition *item = Func_080d5930(&leader->x);
  u8 *flags;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  if (item == NULL)
    return 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->watchedObject = (struct Actor *)item;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, item->x, item->y, item->z);
  Actor_CommitPosition(leader);
  leader->timer = leader->motionFlags;
  leader->unk66 = sprite->nextFrame;
  ((struct GrabSave *)leader)->priority = sprite->priority;
  flags = &leader->priorityFlags;
  leader->priorityFlags = *flags & ~1;
  sprite->nextFrame = 0;
  sprite->priority = 1;
  work->renderMode = 4;
  return 1;
}

s32 Func_080d5aa0(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->sprite;
  s32 position[3];
  u32 angle;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  angle = Data_080f0890[(gInput.held >> 4) & 15];
  if (angle == 0xffff)
    return 0;
  leader->x = (leader->x & 0xfff00000) + 0x80000;
  leader->z = (leader->z & 0xfff00000) + 0x80000;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  leader->terrainHeight = leader->y - 0x80000;
  if (Func_080201d0(leader->terrainId, position[0], position[2]) ||
      Func_08020210(leader, position))
    return 0;
  leader->facing = angle;
  leader->priorityFlags |= 1;
  leader->motionFlags = leader->timer;
  sprite->nextFrame = leader->unk66;
  sprite->rotation = 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, position[0], position[1], position[2]);
  Actor_CommitPosition(leader);
  sprite->priority = ((u8 *)leader)[98];
  gMainRoutineWork.renderMode = 0;
  return 1;
}


/// Writes the object's position on screen, in pixels, to position[0] and
/// position[1]; an animated object is raised by its frame box. Returns -1 when
/// the object does not exist.
s32 Func_080d5bec(s32 objectId, s32 *position) {
  struct Actor *object = ObjectTable_Get(objectId);
#ifdef UBFIX
  struct MapWork *map;
#else
  s32 *camera; // the view's top left, x then z
#endif
  s32 cameraX;
  s32 cameraZ;
  s32 x;
  s32 z;

  if (object == 0)
    return -1;
  // UB: camera[1] crosses the viewX scalar; UBFIX reads the named pair.
#ifdef UBFIX
  map = Ram_HeapSlots->mapWork;
  cameraX = map->viewX & 0xffff0000;
  cameraZ = map->viewZ & 0xffff0000;
#else
  camera = &Ram_HeapSlots->mapWork->viewX;
  cameraX = camera[0] & 0xffff0000;
  cameraZ = camera[1] & 0xffff0000;
#endif
  x = object->x - cameraX;
  z = object->z - cameraZ - object->y;
  *position++ = x / Q16_ONE;
  *position = z / Q16_ONE;
  if ((object->animationKind & 15) == 1)
    *position -= (s8)Resource_GetMetadataRecordFarFar(
                     *((struct ObjectAnimationState *)object->sprite)->value28)
                     ->boxX;
  return 0;
}

#include "display_transition.h"
#include "io_reg.h"
#include "io_write.h"

/// The Broken Seal's Scene_FadeColorFromWhite, playing cue 298 where that
/// plays 288: turns the backdrop (on the world map, a colour of palette 15)
/// white, finishes the screen transition and fades that colour to black
/// over 16 frames.
void Func_080d5c70(s32 unused) {
  struct EventWork *work;
  s32 i;
  s32 c;

  work = Ram_HeapSlots->eventWork;
  Sound_PlayFar(gMainRoutineWork.sceneCue);
  Sound_PlayFar(298);
  Sound_PlayFar(147);
  if (work->controlMode == 3) {
    Io_Write16(RGB(31, 31, 31), (u16 *)&BG_PLTT_COLOR(15, 3));
    ScreenTransition_Finish(0x401, 16);
    work->screenStatus = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), (u16 *)&BG_PLTT_COLOR(15, 3));
      wait(1);
    }
  } else {
    Io_Write16(RGB(31, 31, 31), &PLTT_BACKDROP);
    ScreenTransition_Finish(0x207, 16);
    work->screenStatus = 0;
    wait(16);
    for (i = 0; i < 16; i++) {
      c = 30 - i * 2;
      Io_Write16(RGB(c, c, c), &PLTT_BACKDROP);
      wait(1);
    }
  }
}

struct ContextAnimation {
  u8 unk00[0x1a];
  u8 flags;
};

struct ContextEffect {
  u8 unk00[5];
  u8 param;
};

void *ResourceMetadata_RegisterFar(void *, s32);

void Func_080d5d70(s32 id, s32 side) {
  u32 zero;
  u8 kind;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  s32 mode;

  object = ObjectTable_Get(id);
  context = object->sprite;
  mode = (side != 1);
  mode = 26 - mode;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  // Through a byte pointer: as context->flags the kind is set first.
  *&context->flags = zero;
  kind = 15;
  effect->param = kind;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SetMode(object, mode);
  wait(18);
}

// ---- games/tla/src/field/object/context_start.c ----


void GameFlag_SetBit(s32 flag);

/// Starts the leader's animation for leaving through an exit: centres it on
/// its tile, at the tile's south edge, stops it and gives it the animation's
/// mode, then waits 18 frames. The leader's sprite gains a second part
/// (metadata 267) for it.
// As ☀️'s function of the same name.
void ObjectEffect_PrepareContextEffect(s32 animation) {
  u32 zero;
  u8 param;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  struct MainRoutineWork *party;
  s32 mode;

  party = &gMainRoutineWork;
  mode = animation;
  object = ObjectTable_Get(party->currentOwner);
  context = object->sprite;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  context->flags = zero;
  // An empty once-only loop keeps the cleared flags' store before the kind's
  // load; without it, 8 of 47 instructions differ in tla-en.
  do {
  } while (0);
  param = 15;
  effect->param = param;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SetMode(object, mode);
  wait(18);
}

void ObjectEffect_StartContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBit(0x120);
}

void ObjectEffect_StartContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBit(0x121);
}

struct ContextSprite {
  u8 unk00[0xc];
  s32 scale;
  u8 unk10[0xa];
  u8 flags;
  u8 count;
  u8 unk1c[0xc];
  void *entries[4];
};

struct ContextEffectEntry {
  u8 unk00[5];
  u8 param;
};

void ResourceMetadata_ClearRecordFar(void *record);

void ObjectEffect_EndContextEffect(s32 mode) {
  s32 zero;
  struct Actor *obj = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct ContextSprite *ctx = obj->sprite;
  struct ContextEffectEntry *eff = ResourceMetadata_RegisterFar(ctx, 0x10b);

  zero = 0;
  ctx->flags = zero;
  eff->param = 15;
  obj->x = (obj->x & 0xfff00000) + 0x80000;
  obj->z &= 0xfff00000;
  Actor_SetMode(obj, mode);
  wait(30);
  ctx->count = 1;
  ResourceMetadata_ClearRecordFar(ctx->entries[1]);
  {
    /* FAKEMATCH(pin): the sprite copied into r1 for its two stores, leaving
     * r3 to the zero's copy from sl; as ctx in both stores, 8 of 69
     * instructions differ in tla-en. */
    register struct ContextSprite *pinnedCtx asm("r1") = ctx;

    pinnedCtx->flags = 1;
    pinnedCtx->entries[1] = (void *)zero;
  }
  obj->acceleration = 0x10000;
  obj->speedLimit = 0x10000;
  Actor_SetTargetPosition(obj, obj->x, obj->y, obj->z + 0x80000);
  Actor_CommitPosition(obj);
}


s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Camera_MoveWithinMapBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ObjectEffect_RunPendingFlagEvent(void) {
  s32 result = 0;
  s32 flag = 0x120;

  if (GameFlag_Test(flag) != 0) {
    ObjectEffect_EndContextEffect(24);
    GameFlag_ClearBit(flag);
    result = 1;
  } else {
    flag = 0x121;
    if (GameFlag_Test(flag) != 0) {
      ObjectEffect_EndContextEffect(23);
      GameFlag_ClearBit(flag);
      result = 2;
    } else {
      flag = 0x122;
      if (GameFlag_Test(flag) != 0) {
        s32 id;
        struct Actor *object;

        GameFlag_ClearBit(flag);
        id = gMainRoutineWork.currentOwner;
        object = ObjectTable_Get(id);
        object->y += 0x00a00000;
        Camera_MoveWithinMapBounds(-1, -1, -1, 0);
        while (object->y + object->velocityY > object->terrainHeight) {
          wait(1);
        }
        Sound_PlayFar(159);
        object->y = object->terrainHeight;
        Actor_SetMode(object, 22);
        Event_Wait(15);
        Camera_FollowActor(id, 1);
        result = 3;
      }
    }
  }
  return result;
}

void Func_080d5fd4(void) {
  struct Actor *object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  u8 *metadata =
      ResourceMetadata_RegisterFar(*(void **)((u8 *)object + 0x50), 268);

  metadata[5] = 15;
}


struct DmaFxPage {
  u32 lines[0x1e0];
};

struct DmaFxPages {
  struct DmaFxPage pages[2];
  u8 page;
};

/// Writes the current page's first entry each frame; HBlank DMA writes the
/// rest.
void Func_080d5ff8(void) {
  struct DmaFxPages *work = Ram_HeapSlots->unk84[1];
  u32 *line = work->pages[work->page].lines;
  volatile u32 *dest = (volatile u32 *)&REG_BG1HOFS;
  u8 *event = Ram_HeapSlots->eventWork;
  volatile u32 *target;

  if (event != NULL) {
    if (*(s8 *)(event + 0xca4) != 0)
      return;
    if (*(s16 *)(event + 0x164) != 0)
      return;
  }
  {
    volatile u16 *dma = REG_DMA0;
    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    target = dest;
    (void)dma[5];
  }
  *dest = *line++;
  *dest = *line++;
  *dest = *line++;
  Dma_Set(
      line, (void *)target,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_DEST_RELOAD | DMA_32BIT)
              << 16 |
          3,
      REG_DMA0);
}

#include "bg_scroll.h"

struct DisplayScrollWork {
  struct BgScroll rows[2][160][3];
  u8 page;
  u8 mode;
  u16 frame;
  u8 unkf04[4];
  s32 freqX;
  s32 freqY;
  s32 stepX;
  s32 stepY;
  s32 ampX;
  s32 ampY;
};

extern const s16 Data_080f08b0[];

void Func_080d607c(void) {
  struct DisplayScrollWork *work = Ram_HeapSlots->unk84[1];
  u8 mode = work->mode;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  u8 *event = Ram_HeapSlots->eventWork;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amplitude;
  s32 scanline;
  u16 wave;
  u16 waveX3;
  u16 waveY3;

  if (event != NULL && *(s8 *)(event + 0xca4) != 0)
    return;
  if (mode == 0) {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      u16 a = x1;
      u16 b = x2;

      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
        row[0] = a + wave;
        row[2] = b + wave;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      u16 a = y1;
      u16 b = y2;
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
        *row = a + wave;
        row += 2;
        *row = b + wave;
        row += 2;
        *row = waveY3 + wave;
        row += 2;
        phase += step;
      }
    }
  } else {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
        row[0] = x1;
        row[2] = x2;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave =
            Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) / 256;
        row[0] = y1;
        row[2] = y2;
        row[4] = waveY3 + wave;
        row += 6;
        phase += step;
      }
    }
  }
  work->frame++;
  work->page ^= 1;
}

struct DmaFxWork {
  u8 unk000[0xf01];
  u8 kind;
  u8 unkf02[6];
  s32 unkf08;
  s32 unkf0c;
  s32 unkf10;
  s32 unkf14;
  s32 unkf18;
  s32 unkf1c;
};

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA_ENABLE << 16) != 0) {
  }
}

/// Starts the line scroll: its work (heap slot 136) holds the mode and, for x
/// and y, the wave's frequency, its step each frame and its amplitude; two
/// tasks, one in the vertical blank pass and one in the frame pass, build and
/// send each frame's line offsets.
void LineScroll_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                         s32 stepY, s32 ampY) {
  struct DmaFxWork *work = HeapSlot_AllocEwram(136, sizeof(struct DmaFxWork));
  volatile u32 zero;

  zero = 0;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  WaitDma((volatile struct DmaRegisters *)&REG_DMA3SAD);
  work->kind = mode;
  work->unkf08 = freqX;
  work->unkf0c = freqY;
  work->unkf18 = ampX;
  work->unkf1c = ampY;
  work->unkf10 = stepX;
  work->unkf14 = stepY;
  Task_Add(Func_080d607c, TASK_PASS_VBLANK);
  Task_Add(Func_080d5ff8, TASK_PASS_FRAME);
}

/// Stops the line scroll: removes its two tasks and stops DMA 0.
void LineScroll_Stop(void) {
  Task_Remove(Func_080d5ff8);
  Task_Remove(Func_080d607c);
  {
    volatile u16 *dma = REG_DMA0;

    u16 control;

    control = dma[5];
    dma[5] = control & 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff;          // Stop.
    (void)dma[5];
  }
}

#include "flag_call.h"

struct GroundParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 attr0 : 6;
  u16 shape : 2;
  u16 x : 9;
  u16 attr1 : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  u8 unk00[4];
  s32 tile;
  struct GroundParticle particles[64];
};

void Func_080140d8(struct GroundParticle *sprite, s32 depth);

extern const s16 Data_080f0ab0[];

void Func_080d64f0(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  s32 originZ = origin[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(358))
      particle->timer++;
    frame = &Data_080f0ab0[particle->timer * 5];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Func_080140d8(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x800000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
      particle->timer = 16;
    }
  }
}

// ---- games/tla/src/field/field_effect/f0d6674.c ----

/* VIEW(f0d6674): tag struct GroundParticleWork as struct GroundParticleWork__f0d6674; visual_attributes.c has another definition; tried with the definition the rest of the object has, Func_080d6674 moves: 1 of 167 instructions in tla-ja. */
struct GroundParticleWork__f0d6674 {
  u8 unk00[4];
  // long, not s32: its own alias set lets the sprite stores pass its load.
  long tile;
  struct GroundParticle particles[64];
};

u32 Random16(void);
void Func_080140d8(struct GroundParticle *sprite, s32 depth);

extern const s16 Data_080f0b50[];

/// The Broken Seal's Weather_UpdateRain with fixed sprites, and
/// Func_080d64f0's sibling for the same 64 ground particles: each counts down
/// its frame (held while flag 358 is set), is drawn at its picture's offset
/// (Data_080f0b50, pairs) with the work's tile, shape 1 and size 2 when on
/// screen, and when its count runs out is placed again at a random point (x
/// within -64 to +192, z within -128 to +128 pixels of the map's focus), on
/// the ground, for 12 frames.
void Func_080d6674(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct GroundParticleWork__f0d6674 *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  struct GroundParticle *particle = work->particles;
  s32 originZ = origin[1];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(358))
      particle->timer++;
    frame = &Data_080f0b50[2 * particle->timer];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile;
      particle->shape = 1;
      particle->size = 2;
      Func_080140d8(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x400000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
      particle->timer = 12;
    }
  }
}

#include "palette_buffers.h"
#include "field_blend.h"

void BattleEffect_ApplyColorToTargetBuffer(void *buffer, s32 mode);

void Sound_PlayFar(s32 cue);

// Lightning: selects the palette according to the remaining duration.
void Func_080d67e8(void) {
  struct FieldBlendWork *work = Ram_HeapSlots->unk70[2];
  struct BattleEffectBuffers *target = Ram_HeapSlots->block32;
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_Test(358))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    if (work->enabled != 0) {
      u32 a = Random16();
      u32 b = Random16();
      u32 draw = a * 24;
      u32 jitter = b * 100;
      work->timer = (((draw + a) << 4) >> 16) - (jitter >> 16) + 150;
      if (work->loud != 0)
        Sound_PlayFar(172);
      else
        Sound_PlayFar(171);
    }
  case 5:
  case 10:
    BattleEffect_ApplyColorToTargetBuffer(work, 1);
    Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    BattleEffect_ApplyColorToTargetBuffer(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}

// ---- games/tla/src/field/field_effect/ground_particles.c ----

/* VIEW(ground_particles): tag struct GroundParticleWork as struct GroundParticleWork__ground_particles; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:2771: structure has no member named `entry'. */
struct GroundParticleWork__ground_particles {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[64];
  s32 unk808;
  s32 stopped;
  s32 unk810;
};

void Func_080d2c64(s32 sound);
void Func_080d64f0(void);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
extern const u8 gData_080f3798[];

/// Starts the ground particles: 64 sprites placed at the leader's position on
/// the terrain, their tiles decoded into VRAM, the blend set, and
/// Func_080d64f0 scheduled at priority 0x480 to move them.
void Func_080d690c(void) {
  struct GroundParticleWork__ground_particles *work;
  s32 *leader;
  struct GroundParticle *particle;
  // words is the decode buffer, then each particle's attribute cursor.
  u32 *words;
  u32 i;
  // The fill's value; as a literal 0 the fill differs.
  u32 zero = 0;

  work = HeapSlot_AllocEwram(116, sizeof(struct GroundParticleWork__ground_particles));
  leader = Ram_HeapSlots->mapWork->origin;
  Func_080d2c64(170);
  particle = work->particles;
  Dma_Fill32(zero, work, sizeof(struct GroundParticleWork__ground_particles));
  words = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f3798, words);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, words);
  HeapSlot_Release(56);
  for (i = 0; i < 64; i++) {
    s32 x;
    s32 z;

    words = (u32 *)particle;
    *words++ = 0;
    *words++ = 0x40000400;
    *words = 0xd400;
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(Func_080d64f0, TASK_PASS_VBLANK);
}

#include "field_coordinate.h"
#include "frame.h"

/// One rising particle: its sprite, world position, drift and frames left.
struct RisingParticle {
  /// The sprite: set as three words by the start, drawn field by field.
  union {
    u32 words[3];
    struct {
      u8 unk00[4];
      /// OAM attributes 0 to 2.
      ///@{
      u16 y : 8; ///< screen pixels
      u16 attr0 : 2;
      u16 blendMode : 2;
      u16 attr0b : 2;
      u16 shape : 2;
      u16 x : 9; ///< screen pixels
      u16 attr1 : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      ///@}
      u8 unk0a[2];
    } attributes;
  } sprite;
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 drift;
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the rising particles run.
struct RisingParticleWork {
  s32 vramEntry;
  s32 tile;
  struct RisingParticle particles[64];
};

u32 Random16(void);
void Camera_WorldToScreen(union FieldCoordinate *position);

extern const u8 gData_080f385e[];

/// Updates the 64 rising particles: each starts on the ground at a random
/// point of the strip for 120 frames, drifts right by a random amount each
/// frame, rises with its age, and is drawn at priority 3, its tile stepping
/// back by 8 at 90 and 60 frames left and flickering between two tiles every 8
/// frames.
void Func_080d69f4(void) {
  struct RisingParticleWork *work = Ram_HeapSlots->unk70[1];
  struct RisingParticle *particle = work->particles;
  union FieldCoordinate screen[3];
  u32 i;
  s32 x;
  s32 y;

  for (i = 0; i < 64; i++, particle++) {
    if (particle->timer == 0) {
      x = Random16() * 200 + Q16_ONE * 288;
      y = Q16_ONE * 80 - Random16() * 20;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
      particle->timer = 120;
      particle->drift = 0;
    }
    screen[0].fixed = particle->posX + particle->drift;
    screen[1].fixed = particle->posY;
    screen[2].fixed = particle->posZ;
    Camera_WorldToScreen(screen);
    x = screen[0].part.pixel;
    y = screen[2].part.pixel - particle->timer - 8;
    particle->drift += Random16() + Q16_ONE / 2;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      if (particle->timer < 60)
        particle->sprite.attributes.tile = work->tile + 16;
      else if (particle->timer < 90)
        particle->sprite.attributes.tile = work->tile + 8;
      else
        particle->sprite.attributes.tile = work->tile;
      particle->sprite.attributes.tile =
          ((gFrameCount >> 3) & 1) * 4 + particle->sprite.attributes.tile;
      particle->sprite.attributes.x = x;
      particle->sprite.attributes.shape = 0;
      particle->sprite.attributes.size = 1;
      particle->sprite.attributes.priority = 3;
      particle->sprite.attributes.blendMode = 1;
      particle->sprite.attributes.y = y;
      Func_080140d8(particle, 240);
    }
    particle->timer--;
  }
}

/// Starts the rising particles: allocates and clears heap slot
/// 0x74, decodes the particle tiles into VRAM, sets each of the 64 particles'
/// sprite words and places it on the ground at a random point of the strip
/// (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2 frames, and
/// schedules the update.
void Func_080d6b90(void) {
  struct RisingParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct RisingParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r2, built after the copy of
   * work; as a plain local, 8 of 85 instructions differ in tla-en. */
  register u32 zero asm("r2");
  s32 priority;
  u8 *tiles;
  u32 i;

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  for (i = 0; i < 64; i++, particle++) {
    u32 *sprite = particle->sprite.words;
    s32 x;
    s32 z;

    *sprite++ = 0;
    *sprite++ = 0x40000400;
    *sprite = 0xd400;
    x = Random16() * 200 + 0x1200000;
    z = 0x500000 - Random16() * 20;
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->drift = 0;
    particle->timer = i * 2;
  }
  priority = 0x480;
  Task_Add(Func_080d69f4, priority);
}

struct MoteParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 attr3[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
};

struct MoteWork {
  s32 entry;
  s32 tileBase;
  struct MoteParticle particles[64];
};

s32 GameFlag_Test(s32 flag);

/// The Broken Seal's FieldMotes_Update with 64 motes in the heap slot 0x74
/// workspace, started by FieldMotes_Start.
void Func_080d6c60(void) {
  struct MoteWork *work = Ram_HeapSlots->unk70[1];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u32 spawned = 0;
  u32 i;
  struct MoteParticle *mote;

  for (i = 0, mote = work->particles; i < 64; i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != 0xffff) {
      s32 *camera = &map->viewX;
      s32 cameraX = camera[0];
      s32 cameraZ = camera[1];
      s16 age = mote->timer;
      u32 firstJitterDraw;
      u32 secondJitterDraw;

      if (GameFlag_Test(0x166)) {
        mote->timer++;
        mote->delay--;
      }
      firstJitterDraw = Random16();
      secondJitterDraw = Random16();
      x = ((mote->posX - cameraX) >> 16) +
          (((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
      y = ((mote->posZ - mote->posY - cameraZ) >> 16) - (u16)age;
      if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
        if (mote->timer < 60) {
          mote->tile = work->tileBase + 16;
          mote->delay += 3;
        } else if (mote->timer < 90) {
          mote->tile = work->tileBase + 8;
          mote->delay++;
        } else {
          mote->tile = work->tileBase;
        }
        if ((gFrameCount >> 3) & 1)
          mote->tile += 4;
        mote->x = x;
        mote->y = y - (mote->delay >> 2);
        mote->shape = 0;
        mote->size = 1;
        Func_080140d8(mote, 240);
      } else {
        mote->timer = 0;
      }
    }
    if (spawned < 8 && mote->timer == 0) {
      s32 *leader = map->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      mote->posX = x;
      mote->posZ = y;
      mote->posY = Map_GetTerrainHeightFar(0, x, y);
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}

// ---- games/tla/src/field/field_effect/motes.c ----
#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)


extern const u8 gData_080f385e[];

union MoteWord {
  u32 value;
  void *link;
};

/// One floating mote: its sprite's attribute words and its world position.
struct Mote {
  union MoteWord link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

/// Heap slot 0x74 while the motes run.
/* VIEW(motes): tag struct MoteWork as struct MoteWork__motes; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:3089: structure has no member named `motes'. */
struct MoteWork__motes {
  s32 vramEntry;
  s32 tileBase;
  struct Mote motes[64];
  u8 pad808[12];
};

void Func_080d6c60(void);

/// Starts the 64 floating motes (Func_080d6c60): loads their tiles and puts
/// each under the leader with staggered timers, drawn semi-transparent.
// As ☀️'s function of the same name.
void FieldMotes_Start(void) {
  struct MoteWork__motes *work;
  struct Mote *mote;
  u8 *tiles;
  u32 i;
  u32 size;
  volatile u32 zero;
  volatile u16 *reg;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6, which keeps its zero set inside the
   * loop on every pass and ready for the last blend register; as a plain
   * local, 4 of 92 instructions differ in tla-en. */
  register s32 clear asm("r6");

  size = sizeof(struct MoteWork__motes);
  work = HeapSlot_AllocEwram(116, size);
  mote = work->motes;
  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x205, REG_DMA3);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++) {
    struct MapWork *view = slots->mapWork;
    s32 *pos;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 18 of 92 instructions differ in tla-en. */
    u32 p;
    s32 z;

    p = (u32)mote;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    pos = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = Map_GetTerrainHeightFar(0, (s32)p, z);
    mote->timer = (i & 15) + 1;
    mote++;
  }
  // The blend writes in a once-only block keep the task routine's address
  // ahead of the call's other argument; as a plain block, 4 of 92 instructions
  // differ in tla-en.
  do {
    /* FAKEMATCH(temporary): the values through v and the registers through
     * one stepped pointer, which builds each value from a small constant and
     * steps the address by 2; as REG_BLDCNT, REG_BLDALPHA and REG_BLDY
     * written directly, 31 of 92 instructions differ in tla-en. */
    s32 v;
    v = 0x3f00;
    reg = &REG_BLDCNT;
    *reg = v;
    v = 0x1008;
    reg++;
    *reg = v;
    reg++;
    *reg = clear;
  } while (0);
  Task_Add(Func_080d6c60, TASK_PASS_VBLANK);
}

#undef Resource_DecodeByteLzInRam


/// One of Func_080d6674's particles: its sprite's attribute words, its world
/// position and its frames left.
struct SparkParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the particles run.
struct SparkParticleWork {
  s32 vramEntry;
  s32 tile;
  struct SparkParticle particles[64];
};

void Func_080d6674(void);
void Func_080d2c64(s32 cue);

extern const u8 Data_080f3901[];

/// Starts Func_080d6674's particles: allocates and clears heap slot 0x74,
/// decodes the particle tiles into VRAM, sets each of the 64 particles' sprite
/// words and places it on the ground at the map's focus with a timer of
/// (i & 11) + 1, turns on alpha blending of all layers at 16/16, schedules the
/// update and, when asked, plays cue 137 (Func_080d2c64).
void Func_080d6f34(s32 withCue) {
  struct SparkParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct SparkParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r1, built after the copy of
   * work; as a plain local, 8 of 100 instructions differ in tla-en. */
  register u32 zero asm("r1");
  u8 *tiles;
  u32 i;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6, which keeps its zero set inside the
   * loop on every pass and ready for the last blend register; as a plain
   * local, 4 of 100 instructions differ in tla-en. */
  register s32 clear asm("r6");

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(Data_080f3901, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++, particle++) {
    struct MapWork *view = slots->mapWork;
    s32 *focus;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 18 of 100 instructions differ in tla-en. */
    u32 p;
    s32 z;

    p = (u32)particle;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    focus = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = focus[0];
    z = focus[2];
    particle->posX = (s32)p;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, (s32)p, z);
    particle->timer = (i & 11) + 1;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  Io_Write16(clear, &REG_BLDY);
  Task_Add(Func_080d6674, TASK_PASS_VBLANK);
  if (withCue != 0)
    Func_080d2c64(137);
}

void Func_080d7024(void) { Task_Remove(Func_080d6674); }


struct Sparkle {
  u8 padding00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 padding0a[2];
  s32 posX;
  s32 height;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 padding1e[2];
};

struct SparkleWork {
  u8 padding00[4];
  s32 tileBase;
  struct Sparkle sparkles[64];
  u8 padding808[8];
  s32 stopped;
};

struct SparklePicture {
  s16 dy;
  u16 tile;
};

extern const struct SparklePicture gData_080f0b7c[];


void FieldEffect_UpdateSparkles(void) {
  struct SparkleWork *work;
  u32 spawned;
  u32 i;
  struct MapWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct Sparkle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = Ram_HeapSlots->unk70[1];
  spawned = 0;
  map = Ram_HeapSlots->mapWork;
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->sparkles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_Test(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gData_080f0b7c[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->height - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
#ifdef UBFIX
        frames = gFrameCount;
#else
        // UB: this reads a u32 as unsigned long. The unsigned long alias
        // combines the size and flip stores; as gFrameCount or a u32 alias,
        // 29 of 232 instructions differ in tla-en. UBFIX reads the declared
        // type.
        frames = *(unsigned long *)&gFrameCount;
#endif
        sparkle->y = sy;
        sparkle->matrix = ((frames >> 1) & 1) << 3;
        Func_080140d8(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = Map_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = Map_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 63);
}

void Func_080d67e8(void);

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames);

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

union DustWord {
  u32 value;
  void *link;
};

struct DustParticle {
  union DustWord link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

struct DustWork {
  s32 vramEntry;
  s32 tileBase;
  struct DustParticle particles[64];
  u8 pad808[12];
};

extern const u8 gData_080f38b2[];

void FieldEffect_InitializeSparkles(void) {
  u32 size = sizeof(struct DustWork);
  struct DustWork *work = HeapSlot_AllocEwram(116, size);
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;
  volatile u32 zero;
  struct HeapSlots *slots;

  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set(&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (sizeof(struct DustWork) / 4),
          REG_DMA3);
  buf = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f38b2, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x200, buf);
  HeapSlot_Release(56);

  i = 0;
  slots = Ram_HeapSlots;
  clear = 0;
loop:
  {
    struct MapWork *view = slots->mapWork;
    s32 *leader;
    /* FAKEMATCH(temporary): one integer cursor for the stores and the x keeps
     * the leader in r2; as a u32 * cursor and an x of its own, 16 of 79
     * instructions differ in tla-en. */
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = Map_GetTerrainHeightFar(0, attr, z);
    p->timer = (i & 15) + 1;
  }
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_Add(FieldEffect_UpdateSparkles, TASK_PASS_VBLANK);
}

void BattleEffect_StartTwelveFrameBlend(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;

  work = HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  BattleEffect_BuildBuffer(0x10003, (u16 *)buffers, (u16 *)work, 1);
  BattleEffect_BuildBuffer(0x10005, (u16 *)buffers, (u16 *)work->to, 1);
  BattleEffect_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Task_Add(Func_080d67e8, TASK_PASS_VBLANK);
}

void BattleEffect_SetBlock30ValuesMaxZero(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 0x7fff;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values12Zero(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values128One(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

void BattleEffect_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;
  s16 *target;

  work = HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  BattleEffect_BuildBuffer(from, (u16 *)buffers, (u16 *)work, 1);
  target = work->to;
  BattleEffect_BuildBuffer(to, (u16 *)buffers, (u16 *)target, 1);
  BattleEffect_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Task_Add(Func_080d67e8, TASK_PASS_VBLANK);
}

#include "motobj.h"

/// Per-frame update: spins an object 1/8 turn a frame.
void FieldEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

// Objects: follow a watched object.


/// Halves the distance to the watched object along x, y and z.
/// obj: object to move.
void Actor_MoveHalfwayTowardTarget(struct Actor *obj) {
  struct Actor *target = obj->watchedObject;

  obj->x += (target->x - obj->x) / 2;
  {
    s32 position = obj->y;

    position += (target->y - position) / 2;
    obj->y = position;
  }
  {
    s32 position = obj->z;

    position += (target->z - position) / 2;
    obj->z = position;
  }
}

#include "version.h"
#include "sprite.h"

void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags);

#if TLA_PATCH_1
#define LEAP_SOUND 153
#else
#define LEAP_SOUND 136
#endif

struct Actor *Actor_GetById(u32 objectId);
void Event_Start(void);
void Event_End(void);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_Jump(s32 id, s32 speed, s32 frames);
void Animation_ApplyChildValuesFar(struct Actor *object, u32 value);
void *Sprite_ShareVramCacheEntry(void *sprite, void *last);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);

void Func_080d7524(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *leader;
  struct Actor *copy;
  struct Actor *prev;
  void *sharedSprite;
  s32 dir;
  s32 back;
  s32 sprite;
  s32 tint;
  s32 i;
  struct Actor *copies[8];

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  dir = (leader->facing + 0x2000) & 0xc000;
  Event_Start();
  wait(10);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  wait(10);
  back = dir + 0x8000;
  Actor_ArmCallback(djinniId, back, 0);
  wait(10);
  djinni->facing = back;
  Func_080d47b4(djinniId, 0x103, 50);
  Actor_GetById(djinniId)->actionFlags &= ~1;
  djinni->motionFlags = 2;
  djinni->update = (void (*)(void))FieldEffect_AdvanceSpinAngle;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->gravity = 0x4000;
  djinni->velocityY = 0x48000;
  wait(33);
  ((struct AnimationObject *)djinni->sprite)->image.priority = 0;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4cccc;
  wait(35);
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4e666;
  wait(38);

  djinni->update = NULL;
  djinni->facing = back;
  sprite = ((struct AnimationObject *)djinni->sprite)->entries[0]->animId;
  tint = 9;
  if (sprite == 243)
    tint = 2;
  if (sprite == 245)
    tint = 10;
  // DEAD: tint is already 9 for sprite 244; the source keeps the test.
  if (sprite == 244)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < 8; i++) {
    copy = Func_080200c0(sprite, djinni->x, djinni->y, djinni->z);
    // DEAD: nothing reads copies; the source keeps the store.
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->actionFlags |= 1;
      copy->update = (void (*)(void))Actor_MoveHalfwayTowardTarget;
      copy->facing = djinni->facing;
      ((struct AnimationObject *)copy->sprite)->image.priority = 0;
      Animation_ApplyChildValuesFar(copy, tint);
      Actor_SetMode(copy, 0);
      Actor_SetSingleChildFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->sprite, sharedSprite);
      copy->watchedObject = prev;
      prev = copy;
    }
  }

  djinni->speedLimit = 0x4ccc;
  djinni->acceleration = 0x4ccc;
  Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x100000, dir + 0x8000);
  Sound_PlayFar(LEAP_SOUND);
  Actor_Jump(djinniId, 6, 5);
  wait(24);
  for (i = 0; i < 120; i++) {
    if (ABS(djinni->y - leader->y) < 0x140000)
      break;
    wait(1);
  }
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = 0x80000000;
  djinni->targetZ = 0x80000000;
  djinni->targetY = 0x80000000;
  Actor_SetMode(djinni, 0);
  ((struct AnimationObject *)djinni->sprite)->image.priority = 2;
#if TLA_PATCH_1
  djinni->gravity = 0x10000;
#endif
  Event_End();
}

s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinni);
void Func_080d82e0(s32 actor);
void BattleEffect_RunMercuryDjinnCapture(s32 actor);
void Djinn_RunMarsJoinEffect(s32 actor);
void Djinn_RunJupiterJoinEffect(s32 actor);
void Func_080cb82c(void);
void Func_08038328(s32 owner, s32 element, s32 djinni);
void Func_080cb8a4(void);

extern s8 gData_02001000[];

struct EventActors {
  u8 unk00[20];
  void *actors[64];
};

/// A Djinni joins. Djinni 19 means the one recorded for the element in the
/// save (none: nothing happens). With flag 366 set it joins the current owner
/// by its flag (48 + element * 20 + djinni), otherwise the least loaded owner;
/// if it found one, the element's join effect plays at the actor (none for
/// actor -1), the Djinni is added, and the actor's event slot is cleared
/// (from patch 1; the Japanese release leaves it set).
void Func_080d7788(s32 actor, s32 element, s32 djinni) {
  s32 owner;

  if (djinni == 19) {
    s32 recorded = gData_02001000[element + 0x1950];

    if (recorded == 0)
      return;
    djinni = recorded - 1;
  }
  if (GameFlag_Test(366)) {
    owner = gMainRoutineWork.currentOwner;
    GameFlag_SetBit(element * 20 + djinni + 48);
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinni);
  }
  if (owner < 0)
    return;
  Event_Start();
  if (actor != -1) {
    if (element == 0)
      Func_080d82e0(actor);
    else if (element == 1)
      BattleEffect_RunMercuryDjinnCapture(actor);
    else if (element == 2)
      Djinn_RunMarsJoinEffect(actor);
    else if (element == 3)
      Djinn_RunJupiterJoinEffect(actor);
  }
  Func_080cb82c();
  Func_08038328(owner, element, djinni);
  Func_080cb8a4();
#if TLA_PATCH_1
  ((struct EventActors *)Ram_HeapSlots->eventWork)->actors[actor] = NULL;
#endif
  Event_End();
}

struct ActorDescriptor {
  s16 action;
  s16 scene;
  u8 unk04[4];
  s32 x;
  s32 y;
  s32 z;
};

void Func_080d784c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinnEvent;
  s32 flags = party->pendingDjinnEvent & 0xf000;
  u32 scene = 0xfff;
  s32 id;

  scene &= raw;
  if (GameFlag_Test(0x109) != 0 && flags == 0) {
    flags = scene & 0x800;
    scene &= 0x7ff;
    if (scene - 300 <= 80 && party->sequenceResult > 0) {
      s32 *owner;

      for (id = 8, owner = &party->currentOwner; id <= 79; id++) {
        struct ActorDescriptor *desc = Scene_FindActorPlacement(id);
        struct Actor *obj;

        if (desc == NULL || desc->scene - 48 != scene - 300)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->motionFlags = 3;
          Actor_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct Actor *selection = ObjectTable_Get(*owner);

          Actor_SetPositionAndResetMotionFar(obj, selection->x, selection->y,
                                             selection->z - (32 << 16));
        }
        Actor_SetMode(obj, 1);
      }
    }
  }
}

struct MapActor {
  s16 id;
  s16 event;
};

/// The Broken Seal's Djinn_ResolvePendingEvent, with this game's actors up to
/// 79, the plain flag calls, the event at gMainRoutineWork's 0x254 and its
/// 0x1000 kind tested like the first, falling into the common clear.
void Func_080d793c(s32 capture) {
  struct MainRoutineWork *st = &gMainRoutineWork;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (capture == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < 300 || flag > 380)
        return;
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        return;
      GameFlag_SetBit(flag - 172);
      *pending = kind;
      return;
    } else if (kind == 0x1000) {
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        GameFlag_SetBit(flag);
    } else
      return;
  } else if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= 300 && flag <= 380) {
      flag &= 0x7ff;
      if (st->sequenceResult > 0) {
        s32 djinn = flag - 300;
        s32 element = djinn / 20;
        s32 index = djinn % 20;
        s32 actor;

        for (actor = 8; actor <= 79; actor++) {
          struct MapActor *found =
              (struct MapActor *)Scene_FindActorPlacement(actor);
          if (found != 0 && found->event - 48 == flag - 300) {
            wait(40);
            Func_080d7788(actor, element, index);
            break;
          }
        }
      }
    }
  }
  gMainRoutineWork.pendingDjinnEvent = 0;
}

#include "field_particle.h"

/* VIEW(motes): tag struct BattleFxScene as struct BattleFxScene__motes; field_psynergy_work.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:3869: dereferencing pointer to incomplete type. */
struct BattleFxScene__motes {
  u8 unk00[0x50];
  struct FieldParticle slots[24];
  u8 unk710[0xe4];
};

#define BATTLE_EFFECT_SLOT 224

void BattleEffect_UpdateAllSlots(void) {
  struct BattleFxScene__motes *scene;
  struct FieldParticle *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->effectWork;
  remainingSlots = 0x17;
  slot = scene->slots;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

void BattleEffect_InitializeSlots(void) {
  void *work;

  work = HeapSlot_Alloc(BATTLE_EFFECT_SLOT, sizeof(struct BattleFxScene__motes));
  Dma_Fill32(0, work, sizeof(struct BattleFxScene__motes));
  Task_Add((s32)BattleEffect_UpdateAllSlots, TASK_PASS_VBLANK);
}

void BattleEffect_ClearOwnedSlot(struct FieldParticle *);

void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void) {
  struct BattleFxScene__motes *scene = Ram_HeapSlots->effectWork;
  struct FieldParticle *slot;
  s32 i;

  Task_Remove((u32)BattleEffect_UpdateAllSlots);
  for (i = 0; i < 24; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
  HeapSlot_Release(BATTLE_EFFECT_SLOT);
  wait(1);
}

void BattleEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}


struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

/// Field effect sprite callback of a Jupiter Djinni's light particles: each
/// circles its centre while it widens for 60 frames, circles 40, drifts toward
/// a point 20 pixels over the leader for 40, closes in for 60 and is freed. It
/// turns 1/32 turn a frame throughout, and its radius grows, then shrinks, by 1
/// pixel a frame. The slot's origin is the centre, in screen coordinates
/// (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *owner;
  struct EffectVector position;

  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  // Each age test reads age signed (ldrsh); as particle->age == 60 and so on,
  // 54 of 122 instructions differ in tla-en.
  if (particle->state == 0) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 1) {
    particle->orbitAngle += 1;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 2) {
    particle->orbitAngle += 1;
    position.x = owner->x;
    position.y = owner->y + 20 * Q16_ONE;
    position.z = owner->z;
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 3) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 4) {
    BattleEffect_ClearOwnedSlot(particle);
  }
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset(particle->orbitRadius << 16, particle->orbitAngle << 11,
                        &position);
  particle->x = position.x;
  particle->z = position.z;
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];

void Shop_RestoreSceneTilesFar(s32 value);
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
void Shop_InitializeEffectFar(void);

/// Ends a heavy impact: object id spins, leaps and vanishes in 24 sparks that
/// circle and then close on the current owner, which turns and takes mode 28.
void Djinn_RunJupiterJoinEffect(s32 id) {
  struct EffectVector position;
  struct MotionObject *object;
  struct BattleFxScene__motes *scene;
  struct FieldParticle *slot;
  s32 scale;
  s32 count;

  object = Actor_GetById(id);
  if (object == NULL)
    return;
  BattleEffect_InitializeSlots();
  scene = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x20118c);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(id, 1);
  Sound_PlayFar(174);
  Motion_SetVarCbAndRefresh(id, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(id, 1);
  wait(20);

  // Spin, then leap high.
  Sound_PlayFar(140);
  object->update = BattleEffect_AdvanceSpinAngle;
  wait(40);
  Sound_PlayFar(153);
  Actor_Jump(id, 12, 22);

  // 24 sparks from the object's place on screen, one a frame.
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  Camera_WorldToScreen(&position);
  Actor_Destroy(object);
  Sound_PlayFar(164);
  slot = scene->slots;
  count = 23;
  do {
    FieldParticle_Initialize(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, FieldEffect_RunAngledApproachPhases);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
    scale = Random16() / 3 + Q16_ONE;
    slot->scaleY = scale;
    slot->scaleX = scale;
    count--;
    wait(1);
    slot++;
  } while (count >= 0);

  // The current owner turns to 0x4000 and takes mode 28.
  wait(60);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(40);
  Sound_PlayFar(164);
  wait(100);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

void BattleEffect_SetObjectAlternatingWords(u8 *object) {
  u32 *vblankCount = &gVBlankCount;
  u32 *table;
  u32 index;
  u32 value;

  table = gData_080f0bfc;
  index = (*vblankCount >> 2) & 1;
  value = index[table];
  ((struct MotionObject *)object)->scaleX = value;
  ((struct MotionObject *)object)->scaleY = value;
}

void BattleEffect_SpinUpAngle(void *obj) {
  u32 step;

  step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
  FIELD_AT_OFFSET(obj, u16 *, 6) =
      (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
  if (step < 0x1000U) {
    FIELD_AT_OFFSET(obj, s16 *, 0x64) =
        (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
  }
}

/// Spins an object 1/8 turn and shrinks it by 0x1c0 a frame, destroying it once
/// its scale falls below 0x3000.
void BattleEffect_ShrinkObjectAndDestroySlow(void *obj) {
  s32 step = 0xFFFFFE40;
  s32 scaleY;
  s32 scale;
  s32 angle;

  scaleY = ((struct MotionObject *)obj)->scaleY + step;
  scale = ((struct MotionObject *)obj)->scaleX;
  ((struct MotionObject *)obj)->scaleY = scaleY;
  angle = ((struct MotionObject *)obj)->angle;
  scale += step;
  ((struct MotionObject *)obj)->angle = angle + 0x2000;
  ((struct MotionObject *)obj)->scaleX = scale;
  if (scale < 0x3000)
    Actor_Destroy(obj);
}


struct BattleFxScene__motes;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

void Map_SetScreenShakeFar(s32, s32, s32);

struct OrbitPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 angle;
};

struct SpreadPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern const s32 gBattleEffect_SharedParticleScript[];

struct CaptureObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 gravity;
  u8 unk2c[4];
  s32 speed;
  u8 unk34[33];
  u8 mode;
  u8 unk56[5];
  u8 visible;
  u8 unk5c[2];
  u16 lifetime;
  u8 unk60[4];
  u16 timer;
  u16 angle;
  u8 unk68[4];
  void *callback;
};

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);

struct LinkedCaptureObject {
  u8 padding000[8];
  s32 x;
  s32 y;
  s32 z;
  u8 padding014[84];
  struct LinkedCaptureObject *target;
};

void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect) {
  struct EffectOwner *owner;
  struct EffectVector position;
  s32 state;

  owner = (struct EffectOwner *)Actor_GetById(gMainRoutineWork.currentOwner);
  state = effect->state;

  if (state == 0) {
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = effect->x;
    position.z = effect->z;
    Vector_AddPolarOffset(0x780000,
                          ((Random16() * 3 << 11) >> 16) -
                              ((Random16() * 3 << 11) >> 16) + 0xc000,
                          &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = 0x50000;
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    ((struct AnimationObject *)effect->object)->image.priority =
        owner->sprite->image.priority;
    effect->flags = 0;
    effect->age = 0;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(134);
  } else if (state == 1) {
    if ((s16)effect->age == 3) {
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
    }
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->originX = effect->x;
      effect->originZ = effect->z;
      ((struct AnimationObject *)effect->object)->image.priority = 0;
      effect->flags = 4;
      effect->render = 0;
      effect->state++;
      effect->callbackDelay = 40;
    }
  } else if (state == 3) {
    effect->render = 1;
    effect->x = effect->originX;
    effect->z = effect->originZ;
    position.x = owner->position.x;
    position.y = owner->position.y + 0x140000;
    position.z = owner->position.z;
    Camera_WorldToScreen(&position);
    Vector_AddPolarOffset(0x40000, Random16(), &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->state++;
    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(145);
  } else if (state == 4) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 5) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      BattleEffect_ClearOwnedSlot(effect);
  }
}

void BattleEffect_ShrinkObjectAndDestroyFast(void *obj);

void BattleEffect_UpdateDescendingOrbitObject(
    struct DescendingOrbitObject *arg);

void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);
#undef Resource_DecodeByteLzInRam
#undef LEAP_SOUND
#undef BATTLE_EFFECT_SLOT
#undef Actor_SetChildValueUnlessFifteenFar
#undef Actor_ReleaseFar
#undef gBattleEffect_PulseScales
#undef gEffectWork

// ---- games/tla/src/field/event_commands/mars_join.c ----

struct DjinniObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[68];
  u16 timer;
  u8 unk66[6];
  void *update;
};

struct Actor *Actor_GetById(u32 objectId);
void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32 value);
void Sound_PlayFar(s32 cue);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void wait(s32 frames);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x, s32 z);
u32 Random16(void);
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);
void BattleEffect_SetObjectAlternatingWords(u8 *object);
void BattleEffect_SpinUpAngle(void *obj);
void BattleEffect_ShrinkObjectAndDestroySlow(void *obj);
void BattleEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect);

/// A Mars Djinni joins the party: it turns to the camera, bounces three times,
/// spins ever faster and shrinks away in a fountain of 24 sparks
/// (BattleEffect_UpdatePhasedRadialParticle). The leader turns to the camera
/// and plays animation 28, the sparks swarm the leader, who pulses for 100
/// frames, and the sparks vanish.
void Djinn_RunMarsJoinEffect(s32 djinniId) {
  s8 *objectPointer;
  struct EffectVector position;
  s32 value;
  s32 remaining;
  struct DjinniObject *djinni;
  void *effectSlots;
  struct DjinniObject *leader;
  void *slot;
  s8 *slotState;

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x201090);
  wait(30);
  Actor_ArmCallback(djinniId, 0x4000, 0);
  wait(20);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(174);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  wait(20);
  Sound_PlayFar(140);
  djinni->update = BattleEffect_SpinUpAngle;
  djinni->timer = 0;
  wait(80);
  djinni->update = BattleEffect_ShrinkObjectAndDestroySlow;
  Actor_SetMode(djinni, 3);

  // The sparks start where the Djinni is on screen.
  position.x = djinni->x;
  {
    void *p = &position;
    s32 y;

    y = djinni->y;
    remaining = 23;
    position.y = y;
    position.z = djinni->z;
    Camera_WorldToScreen(p);
  }

  slot = (u8 *)effectSlots + 80;
  do {
    FieldParticle_Initialize(slot, 672, position.x, position.z);
    FieldParticle_SetCallback(slot, BattleEffect_UpdatePhasedRadialParticle);
    FieldParticle_SetActorMode(slot, 7);
    objectPointer = slot;
    value = *(s32 *)objectPointer;
    Animation_ApplyChildValuesToRecordFar(value, 10);
    value = Random16() / 3 + Q16_ONE; // 1.0 to 1.33
    ((struct FieldParticle *)slot)->scaleY = value;
    ((struct FieldParticle *)slot)->scaleX = value;
    remaining--;
    wait(1);
    slot = (u8 *)slot + sizeof(struct FieldParticle);
  } while (remaining >= 0);

  wait(60);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);

  // Every spark still in use stops its fountain (state 2). The cursor walks
  // the slots' state bytes; a slot's active flag is 5 bytes after its state.
  {
    s32 nextState;

    wait(20);
    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  wait(60);
  leader->update = BattleEffect_SetObjectAlternatingWords;
  wait(100);

  // Every spark still in use goes (state 5).
  {
    s32 nextState;

    slotState = effectSlots;
    nextState = 5;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += sizeof(struct FieldParticle);
    } while (remaining >= 0);
  }

  wait(10);
  slot = NULL;
  leader->update = slot;
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  wait(30);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Spins an object 1/8 turn and shrinks it by 0x400 a frame, destroying it once
/// its scale falls below 0x3000.
void BattleEffect_ShrinkObjectAndDestroyFast(void *obj) {
  s32 step = 0xFFFFFC00;
  s32 scaleY;
  s32 scale;
  s32 angle;

  scaleY = ((struct MotionObject *)obj)->scaleY + step;
  scale = ((struct MotionObject *)obj)->scaleX;
  ((struct MotionObject *)obj)->scaleY = scaleY;
  angle = ((struct MotionObject *)obj)->angle;
  scale += step;
  ((struct MotionObject *)obj)->angle = angle + 0x2000;
  ((struct MotionObject *)obj)->scaleX = scale;
  if (scale < 0x3000)
    Actor_Destroy(obj);
}


struct BattleFxScene;

#define Actor_SetChildValueUnlessFifteenFar                                    \
  ((void (*)(s32, s32))Animation_ApplyChildValuesToRecordFar)
#define Actor_ReleaseFar Actor_Destroy
#define gBattleEffect_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

extern u32 gVBlankCount;
extern u32 gData_080f0bfc[];


void BattleEffect_ClearOwnedSlot(struct FieldParticle *effect);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
void BattleEffect_AdvanceSpinAngle();
void Map_SetScreenShakeFar(s32, s32, s32);
void Motion_SetTargetPositionFromMagnitudeAngle();




extern const s32 gBattleEffect_SharedParticleScript[];


void Actor_Jump(s32 id, s32 height, s32 frames);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);


/// Per-frame update of a Venus Djinni's falling motes: each circles the
/// leader on a shrinking spiral (radius timer * 0.4 pixels) while it sinks a
/// pixel a frame, and is released 20 pixels above the leader's feet. Set by
/// Func_080d82e0, the Venus Djinni's effect.
void FieldEffect_UpdateDescendingOrbitActor(
    struct DescendingOrbitObject *actor) {
  struct OrbitPosition local;
  struct DescendingOrbitObject *other;
  s32 raw;
  s16 value;
  s32 y;

  other = (struct DescendingOrbitObject *)Actor_GetById(
      gMainRoutineWork.currentOwner);
  raw = actor->timer - 1;
  actor->timer = raw;
  value = actor->timer;
  local.x = other->x;
  local.z = other->z;
  Vector_AddPolarOffset(value * 0x6666, (value << 11) + actor->angle, &local);
  actor->x = local.x;
  actor->z = local.z;
  y = actor->y + 0xFFFF0000;
  actor->y = y;
  if (y < other->y + 0x140000)
    Actor_ReleaseFar(actor);
}

void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect);

void BattleEffect_RunVenusDjinnCapture(s32 arg);

void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);

/// Places a spark at random around the current owner and sends it 120 pixels
/// along angle 0xc000. Each time FieldParticle_HasPendingTarget returns 0,
/// state 1 starts it over and state 2 clears its slot.
void BattleEffect_UpdateRadialSpread(struct FieldParticle *effect) {
  struct EffectVector position;
  struct Actor *source;
  s32 state;
  u32 random;
  s32 x;
  s32 magnitude;
  s32 angle;

  source = Actor_GetById(gMainRoutineWork.currentOwner);
  state = effect->state;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y;
    position.z = source->z;

    random = Random16() * 10 + 0xa0000;
    Vector_AddPolarOffset(random, Random16(), &position);
    Camera_WorldToScreen(&position);

    x = position.x;
    magnitude = 240;
    effect->originX = x;
    angle = 192;
    effect->originZ = position.z;
    effect->x = position.x;
    effect->z = position.z;
    position.x = effect->x;
    position.z = effect->z;

    Vector_AddPolarOffset(magnitude << 15, angle << 8, &position);
    effect->targetX = position.x;
    effect->targetZ = position.z;
    effect->acceleration = Q16_ONE;
    effect->maxSpeed = 0x50000;
    effect->limitTurnStep = state;
    effect->state++;

    if ((gVBlankCount & 1) != 0)
      Sound_PlayFar(0x90);
  } else if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      effect->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(effect) == 0)
      BattleEffect_ClearOwnedSlot(effect);
  }
}

struct BattleFxScene;

void Func_080d82e0(s32 arg) {
  struct CaptureObject *leader;
  struct CaptureObject *djinni;
  void *effectSlots;
  u8 *slot;
  s8 *slotState;
  s32 remaining;
  s32 scale;
  s32 position[3];

  leader = (struct CaptureObject *)Actor_GetById(gMainRoutineWork.currentOwner);
  djinni = (struct CaptureObject *)Actor_GetById(arg);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = gEffectWork;
  Shop_RestoreSceneTilesFar(0x201204);
  wait(30);
  djinni->visible = 0;
  Sound_PlayFar(152);
  Actor_Jump(arg, 4, 15);
  Sound_PlayFar(152);
  Actor_Jump(arg, 4, 15);
  wait(30);
  djinni->callback = (void *)BattleEffect_AdvanceSpinAngle;
  Sound_PlayFar(153);
  Actor_Jump(arg, 8, 22);
  Sound_PlayFar(140);
  Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
  djinni->callback = (void *)BattleEffect_ShrinkObjectAndDestroyFast;
  Actor_SetMode((struct Actor *)djinni, 3);
  wait(90);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(30);
  Map_SetScreenShakeFar(0x19999, 0x19999, 0x10000);

  position[0] = djinni->x;
  position[1] = djinni->y;
  position[2] = djinni->z;
  Camera_WorldToScreen(position);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize(slot, 672, position[0], position[2]);
    FieldParticle_SetCallback(slot, (void *)BattleEffect_UpdateRadialSpread);
    FieldParticle_SetActorMode(slot, 7);
    Actor_SetChildValueUnlessFifteenFar(*(s32 *)slot, 11);
    ((struct FieldParticle *)slot)->scaleX = 0x8000;
    ((struct FieldParticle *)slot)->scaleY = Random16() + 0x18000;
    remaining--;
    wait(1);
    slot += 72;
  } while (remaining >= 0);

  wait(140);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  wait(20);
  Map_SetScreenShakeFar(1, 1, 1);
  wait(30);

  for (remaining = 0; remaining <= 23; remaining++) {
    position[0] = leader->x;
    position[1] = leader->y + 0x780000;
    position[2] = leader->z;
    djinni = (struct CaptureObject *)Actor_Spawn(672, position[0], position[1],
                                                 position[2]);
    if (djinni != NULL) {
      scale = Random16() / 3 + 0x10000;
      djinni->scaleY = scale;
      djinni->scaleX = scale;
      djinni->timer = 100;
      djinni->angle = (remaining << 16) / 24;
      djinni->callback = (void *)FieldEffect_UpdateDescendingOrbitActor;
      djinni->mode = 0;
      Actor_SetMode((struct Actor *)djinni, 7);
      Animation_ApplyChildValuesFar(djinni, 11);
    }
  }

  wait(100);
  Sound_PlayFar(298);
  wait(1);
  Sound_PlayFar(151);
  position[0] = leader->x;
  position[1] = leader->y + 0x120000;
  position[2] = leader->z;
  for (remaining = 0; remaining <= 7; remaining++) {
    djinni = (struct CaptureObject *)Actor_Spawn(672, position[0], position[1],
                                                 position[2]);
    if (djinni == NULL)
      break;
    djinni->scaleY = 0x9999;
    djinni->scaleX = 0x9999;
    djinni->mode = 2;
    djinni->gravity = 0x50000;
    djinni->baseY = djinni->y;
    djinni->speed = Random16() + 0x16666;
    Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x200000, Random16());
    Animation_ApplyChildValuesFar(djinni, 11);
    djinni->lifetime = 8;
    Actor_StartScriptFar((struct Actor *)djinni,
                         gBattleEffect_SharedParticleScript);
  }

  wait(15);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}

struct BattleFxScene;

void BattleEffect_UpdateDescendingOrbitObject(
    struct DescendingOrbitObject *arg);

/// Field effect sprite callback of a Mercury Djinni's droplets: each falls from
/// 100 pixels above (on screen) to a random point 2 to 8 pixels round a spot 15
/// to 20 pixels over the leader, at 3 to 6 pixels a frame, and where it lands
/// splashes 12 pixels in a random direction, at half size, with sound 143. It
/// then falls again 6 frames later, until BattleEffect_RunMercuryDjinnCapture
/// sets its state to 2: its next splash is its last.
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect) {
  struct Actor *source;
  struct EffectVector position;
  s8 *statePointer;
  s32 state;

  source = Actor_GetById(gMainRoutineWork.currentOwner);
  statePointer = &effect->state;
  state = *statePointer;

  if (state == 0) {
    position.x = source->x;
    position.y = source->y + Random16() * 5 + 0xf0000;
    {
      struct EffectVector *p = &position;

      position.z = source->z;
      Camera_WorldToScreen(p);
    }
    Vector_AddPolarOffset(Random16() * 6 + 0x20000, Random16(), &position);

    {
      s32 accel = 0x30000;

      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->x = position.x;
      effect->z = position.z - 0x640000;
      effect->acceleration = accel;
    }
    effect->maxSpeed = Random16() * 3 + 0x30000;
    effect->scaleX = Q16_ONE;
    effect->scaleY = Q16_ONE;
    effect->limitTurnStep = state;
    effect->stopAtTarget = 1;
    (*statePointer)++;
  } else if ((u8)(effect->state - 1) <= 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      position.x = effect->x;
      position.z = effect->z;
      Vector_AddPolarOffset(0xc0000, Random16(), &position);
      effect->targetX = position.x;
      effect->targetZ = position.z;
      effect->stopAtTarget = 0;
      effect->speed = Q16_ONE;
      effect->acceleration = 0;
      effect->maxSpeed = Random16() + 0x23333;
      effect->scaleX = 0x8000;
      effect->scaleY = 0x8000;
      Sound_PlayFar(143);

      if (*statePointer == 1)
        (*statePointer)--;
      else
        (*statePointer)++;
      effect->callbackDelay = 6;
    }
  } else if (state == 3) {
    BattleEffect_ClearOwnedSlot(effect);
  }
}

void Actor_MoveHalfwayTowardTargetCopy(struct Actor *object) {
  struct Actor *target = object->watchedObject;

  object->x += (target->x - object->x) / 2;
  object->y += (target->y - object->y) / 2;
  object->z += (target->z - object->z) / 2;
}
#undef Actor_SetChildValueUnlessFifteenFar
#undef Actor_ReleaseFar
#undef gBattleEffect_PulseScales
#undef gEffectWork

// ---- games/tla/src/field/work9c/f0d8740.c ----

struct CaptureSprite {
  u8 unk00[40];
  s16 *resource;
};

/* VIEW(f0d8740): tag struct CaptureObject as struct CaptureObject__f0d8740; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:4840: structure has no member named `sprite'. */
struct CaptureObject__f0d8740 {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[44];
  struct CaptureSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 unk56[4];
  u8 collisionFlags;
  u8 visible;
  u8 unk5c[12];
  struct CaptureObject__f0d8740 *linked;
  void *callback;
};

struct CaptureResource {
  u8 unk00[16];
  u8 id;
};

void BattleEffect_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32 value);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_ArmCallback(s32 id, s32 value, s32 flags);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x, s32 z);
void FieldParticle_SetCallback(struct FieldParticle *effect, EffectCallback callback);
void FieldParticle_SetActorMode(struct FieldParticle *effect, s32 mode);
void Shop_InitializeEffectFar(void);
void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void);

void BattleEffect_RunMercuryDjinnCapture(s32 arg) {
  struct CaptureObject__f0d8740 *djinni;
  struct CaptureObject__f0d8740 *copy;
  struct CaptureObject__f0d8740 *previous;
  struct CaptureResource *resource;
  void *effectSlots;
  struct CaptureObject__f0d8740 **cursor;
  struct CaptureObject__f0d8740 **list;
  u8 *slot;
  s8 *slotState;
  s32 kind;
  s32 remaining;
  s32 resourceId;
  struct CaptureObject__f0d8740 *copies[8];
  struct {
    s32 x;
    s32 y;
    s32 z;
  } position;

  djinni = Actor_GetById(arg);
  if (djinni == NULL)
    return;

  BattleEffect_InitializeSlots();
  effectSlots = Ram_HeapSlots->effectWork;
  Shop_RestoreSceneTilesFar(0x204084);
  wait(30);
  djinni->visible = 0;
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(arg, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(arg, 1);
  wait(20);
  Sound_PlayFar(152);
  Actor_Jump(arg, 3, 14);
  Sound_PlayFar(152);
  Actor_Jump(arg, 5, 16);
  Sound_PlayFar(152);
  Actor_Jump(arg, 7, 18);
  wait(20);

  kind = *djinni->sprite->resource;
  resource = NULL;
  previous = djinni;
  cursor = copies;
  for (remaining = 7; remaining >= 0; remaining--) {
    copy = Func_080200c0(kind, djinni->x, djinni->y, djinni->z);
    *cursor++ = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->mode = 0;
      copy->priorityFlags = 2;
      copy->collisionFlags |= 1;
      copy->callback = (void *)Actor_MoveHalfwayTowardTargetCopy;
      copy->facing = djinni->facing;
      Animation_ApplyChildValuesFar((struct Actor *)copy, 9);
      Actor_SetMode(copy, 0);
      Actor_SetSingleChildFlagsFar((struct Actor *)copy, 0);
      resource = Sprite_ShareVramCacheEntry(copy->sprite, resource);
      copy->linked = previous;
      previous = copy;
    }
  }

  resourceId = resource->id;
  Sound_PlayFar(153);
  djinni->mode = 0;
  for (remaining = 14; remaining >= 0; remaining--) {
    djinni->y += 0x80000;
    wait(1);
  }
  Actor_Destroy(djinni);
  list = copies;
  for (remaining = 7; remaining >= 0; remaining--)
    Actor_Destroy(*list++);
  if (resourceId != 96)
    Resource_ResetEntry(resourceId);
  wait(10);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x4000, 0);
  wait(20);
  Actor_SetMode(Actor_GetById(gMainRoutineWork.currentOwner), 28);
  wait(20);

  position.x = djinni->x;
  position.y = djinni->y;
  position.z = djinni->z;
  Camera_WorldToScreen(&position.x);

  slot = (u8 *)effectSlots + 80;
  remaining = 23;
  do {
    FieldParticle_Initialize(slot, 301, position.x, position.z);
    FieldParticle_SetCallback(slot,
                              (void *)FieldEffect_UpdateRandomTargetParticle);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(*(struct ActorSpriteView **)slot, 9);
    remaining--;
    wait(1);
    slot += 72;
  } while (remaining >= 0);

  wait(120);
  {
    s32 nextState;

    nextState = 2;
    slotState = effectSlots;
    slotState += 144;
    remaining = 23;
    do {
      if (slotState[5] != 0)
        slotState[0] = nextState;
      remaining--;
      slotState += 72;
    } while (remaining >= 0);
  }

  wait(50);
  Shop_InitializeEffectFar();
  BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}


// XXX: what the work block in heap slot 39 holds, and what this family does;
// its callers and the block's readers would settle it.

extern const u8 gData_080f0c04[];

void Func_080d8a00(void);
void Func_080d8c10(void);

void Func_080d897c(void) {
  struct Work9c *work = HeapSlot_AllocEwram(156, sizeof(struct Work9c));
  void *buffer = Heap_Alloc(0x100);

  Dma_Fill32(0, work, sizeof(struct Work9c));
  work->vramEntry = Resource_FindFreeEntry();
  Resource_DecodeType01(gData_080f0c04, buffer);
  VramCache_Load(work->vramEntry, 0x100, buffer);
  Task_Add(Func_080d8a00, TASK_PASS_VBLANK);
  Task_Add(Func_080d8c10, TASK_PASS_VBLANK + 15);
  Heap_Release(buffer);
}

#include "vram_tab.h"

struct ChainTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 unk06 : 16;
};

struct ChainNode {
  struct ChainNode *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 used;
  u8 unk13;
  struct AnimationSpritePart sprite;
};

struct ChainSlot {
  s16 id;
  s16 value;
  s16 depth;
  s16 priority;
  s16 flag;
  s16 state;
  u8 unk0c[16];
  struct ChainNode *head;
};

struct ChainWork {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot slots[8];
};

s32 ArcTan2(s32 y, s32 x);

/// The heap slot 0x9c work's per-frame callback (Func_080d897c schedules it).
/// For each of the eight slots it draws a sprite at the midpoint of each pair
/// of neighbouring nodes in the slot's chain, 4 pixels up, turned along the
/// pair and given the slot's priority, culled to the screen and sorted into the
/// OAM buckets by depth; the chain's first node takes the last angle.
void Func_080d8a00(void) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct ChainSlot *slot = work->slots;
  s32 i;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  s32 prevZ = 0;
  s32 started = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;
  struct ChainNode *node;
  u32 angle;

  for (i = 7; i >= 0; i--, slot++) {
    for (node = slot->head; node != NULL; node = node->next) {
      s32 priority = slot->priority;
      s32 y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y += 0xfffc0000;

      if (started) {
        s32 screenX;
        s32 depth;
        s32 screenY;

        angle = ((u16)ArcTan2(prevX - x, (z - prevZ) + (prevY - y)) + 0x4000) &
                0xffff;
        screenX = (prevX + x) / 2 - viewX;
        // prevY is reused for the midpoint's height, then its depth key; a
        // local of its own differs.
        prevY = (prevY + y) / 2 - originY;
        depth = (prevZ + z) / 2 - viewZ - originY;
        screenY = depth - prevY;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 spriteX = ((screenX >> 16) - 4) & 0x1ff;
          s32 spriteY = ((screenY >> 16) - 4) & 0xff;
          struct AnimationSpritePart *sprite;

          prevY = ((prevY + depth) >> 16) + 58;
          sprite = &node->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = spriteY | (spriteX << 16) | 0x2000;
          ((u32 *)&node->sprite)[2] = 0x800 | tile;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            node->sprite.affine = 1;
            node->sprite.affineIndex = AffineMatrix_BuildForEffect(&transform);
          }
          node->sprite.priority = priority;
          node->angle = angle;
          Func_080140d8(sprite, prevY);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
      started = 1;
    }
    prevX = 0;
    prevY = 0;
    prevZ = 0;
    started = 0;
    if (slot->head != NULL)
      slot->head->angle = angle;
  }
}


struct SwingSlot {
  s16 id;
  s16 value;
  u8 unk04[8];
  s32 amplitude;
  u8 unk10[4];
  s32 phase;
  s32 active;
  u8 unk1c[4];
};

struct SwingEvent {
  u8 unk000[362];
  s16 prompt;
  u8 unk16c[24];
  s16 timer;
};

struct SwingWork {
  u8 unk000[12];
  struct SwingSlot slots[8];
};

s32 Field_FindNearbyTalkTarget(s32 owner);
s32 Func_080d99b8(s32 id, s32 value);
s32 Trig_Sin(s32 angle);
void Func_080d8fa8(s32 index, s32 id, s32 value, s32 offset);

void Func_080d8c10(void) {
  struct SwingEvent *event = Ram_HeapSlots->eventWork;
  struct SwingWork *work = Ram_HeapSlots->unk84[6];
  struct SwingSlot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->timer > 12) {
    if (gMainRoutineWork.renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++)
          if (work->slots[i].id == target || work->slots[i].value == target)
            break;
        if (i < 8 && Func_080d99b8(work->slots[i].id, work->slots[i].value))
          event->prompt = 0xfc85;
      }
    } else if (gMainRoutineWork.renderMode == 3) {
      event->prompt = 0xfc86;
    }
  }
  slot = work->slots;
  for (i = 0; i < 8; i++, slot++) {
    if (slot->active != 0) {
      s32 swing = Iwram_MulQ16(Trig_Sin(slot->phase), slot->amplitude);

      slot->phase += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->active = 0;
      }
      Func_080d8fa8(i, slot->id, slot->value, swing);
    }
  }
}

struct Work9cItem *Func_080d8d40(void) {
  struct Work9c *work = WORK_9C;
  struct Work9cItem *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}


s32 Func_080d8e08(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2);

void Func_080d8d68(s32 index, s32 id, s32 other) {
  struct Work9c *work = WORK_9C;
  s32 priority = 2;
  struct Actor *first = ObjectTable_Get(id);
  struct Actor *second = ObjectTable_Get(other);

  if (first != NULL && second != NULL) {
    struct Work9cSlot *slot;
    s32 lift;

    if (first->animationKind == 1)
      priority = ((struct FieldSprite *)first->sprite)->priority;
    slot = &work->slots[index];
    slot->id = id;
    slot->value = other;
    *(s16 *)slot->unk06 = priority;
    lift = 128;
    /* FAKEMATCH(barrier): an empty asm on lift keeps its 128 built after the
     * priority's store and ahead of the coordinates' loads; without it, 10
     * of 75 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d8e08(index, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
  Actor_SetMode(first, 2);
  Actor_SetMode(second, 2);
}


struct RopeNode {
  struct RopeNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  u8 kind;
  u8 slot;
};

struct RopeSlot {
  s16 id;
  s16 value;
  s16 count;
  u8 unk06[2];
  s16 flag;
  s16 state;
  s32 amplitude;
  u8 unk10[4];
  s32 phase;
  s32 active;
  struct RopeNode *head;
};

struct RopeWork {
  u8 unk000[12];
  struct RopeSlot slots[8];
};

s32 Func_080d8e08(s32 index, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2) {
  struct RopeWork *work = Ram_HeapSlots->unk84[6];
  s32 result = 0;
  s32 dx = x2 - x1;
  s32 sx = dx >> 8;
  s32 dy = y2 - y1;
  s32 sy = dy >> 8;
  s32 dz = z2 - z1;
  s32 sz = dz >> 8;
  s32 count;
  struct RopeSlot *slot;
  struct RopeNode **link;
  struct RopeNode *node;
  u16 angle;
  s32 sin;
  s32 cos;
  s32 i;

  count = Iwram_Sqrt(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                     Iwram_MulQ16(sz, sz)) /
              6 +
          1;
  slot = &work->slots[index];
  link = &slot->head;
  angle = ArcTan2(sx, sy);
  sin = Trig_Sin(angle);
  cos = Trig_Cos(angle);
  slot->count = count;
  for (i = 0; i <= count; i++) {
    s32 bow;

    node = Func_080d8d40();
    if (node == NULL) {
      result = -1;
      break;
    }
    bow = Iwram_MulQ16(Trig_Sin((i << 16) / count), 0x10000);
    node->x = x1 + i * dx / count + Iwram_MulQ16(bow, cos);
    node->y = y1 + i * dy / count + Iwram_MulQ16(bow, sin);
    node->z = z1 + i * dz / count;
    node->slot = index + 1;
    if (i == count)
      node->kind = 2;
    else
      node->kind = 1;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  slot->state = 1;
  slot->active = 1;
  slot->amplitude = 0x10000;
  slot->phase = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  return result;
}

s32 Func_080d8ff8(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Func_080d8fa8(s32 index, s32 from, s32 to, s32 mode) {
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Func_080d8ff8(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z,
                  mode);
}

// ---- games/tla/src/field/work9c/f0d8ff8.c ----
// A slot's chain of nodes laid along a line between two points, and
// Func_080d9104, which links two objects.


/* VIEW(f0d8ff8): routine ArcTan2 as s16 ArcTan2(s32 y, s32 x); f0d8740 has s32 ArcTan2(s32 y, s32 x); tried as the definition's s32 ArcTan2(s32 x, s32 y), which the object already declares, Func_080d8ff8 moves: 1 of 120 instructions in tla-ja. */
extern s16 ArcTan2__f0d8ff8(s32 y, s32 x) __asm__("ArcTan2");

/* VIEW(f0d8ff8): tag struct ChainNode as struct ChainNode__f0d8ff8; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5338: structure has no member named `visible'. */
struct ChainNode__f0d8ff8 {
  struct ChainNode__f0d8ff8 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
};

/* VIEW(f0d8ff8): tag struct ChainSlot as struct ChainSlot__f0d8ff8; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5330: structure has no member named `segments'. */
struct ChainSlot__f0d8ff8 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[22];
  struct ChainNode__f0d8ff8 *head;
};


/// Lays a slot's chain of nodes along the line between two points, bowed
/// sideways by a half sine of the given height.
s32 Func_080d8ff8(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ax = dx >> 8;
  s32 dy = toY - y;
  struct ChainSlot__f0d8ff8 *slot = &work->slots[index];
  struct ChainNode__f0d8ff8 *node = slot->head;
  u16 angle = ArcTan2__f0d8ff8(ax, dy >> 8);
  s32 sin = Trig_Sin(angle);
  s32 cos = Trig_Cos(angle);
  s32 segments = slot->segments;
  s32 i;
  s32 visible = 1; // One constant for its whole life.
  {
    for (i = 0; i <= segments; i++) {
      s32 bow = Iwram_MulQ16(Trig_Sin((i << 15) / segments), height);

      node->x = x + i * dx / segments + Iwram_MulQ16(bow, cos);
      node->y = y + i * dy / segments + Iwram_MulQ16(bow, sin);
      node->z = z + i * (toZ - z) / segments;
      node->visible = visible;
      node = node->next;
    }
  }
  return 0;
}


struct LinkAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

struct LinkEnds {
  s16 first;
  s16 second;
  s16 unk04;
  s16 layer;
};

struct LinkEntry {
  u8 unk00[12];
  struct LinkEnds ends;
  u8 unk14[12];
};


// Link two objects.
void Func_080d9104(s32 slot, s32 firstId, s32 secondId) {
  struct LinkEntry *entries = Ram_HeapSlots->unk84[6];
  s32 layer = 2;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != 0 && second != 0) {
    struct LinkEnds *ends;
    s32 lift;

    if (first->animationKind == 1)
      layer = ((struct LinkAnimation *)first->sprite)->layer;
    ends = &entries[slot].ends;
    ends->first = firstId;
    ends->second = secondId;
    ends->layer = layer;
    lift = 128;
    /* FAKEMATCH(barrier): an empty asm on lift keeps its 128 built after the
     * layer's store and ahead of the coordinates' loads; without it, 10 of
     * 69 instructions differ in tla-en. */
    asm volatile("" : "+l"(lift));
    lift <<= 12;
    Func_080d9194(slot, first->x, first->y + lift, first->z, second->x,
                  second->y + lift, second->z);
  }
}

// ---- games/tla/src/field/work9c/f0d9194.c ----
// A slot's chain of nodes built around a point, and Func_080d92a4, which gives
// the position of the node at the slot's depth.



/* VIEW(f0d9194): tag struct ChainNode as struct ChainNode__f0d9194; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5452: structure has no member named `visible'. */
struct ChainNode__f0d9194 {
  struct ChainNode__f0d9194 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
};

/* VIEW(f0d9194): tag struct ChainSlot as struct ChainSlot__f0d9194; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5442: structure has no member named `segments'. */
struct ChainSlot__f0d9194 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[22];
  struct ChainNode__f0d9194 *head;
};



/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point.
s32 Func_080d9194(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct ChainSlot__f0d9194 *slot = &work->slots[index];
  struct ChainNode__f0d9194 **link = &slot->head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 visible = 2; // One constant for its whole life.

  slot->segments = segments;
  for (i = 0; i <= segments; i++) {
    struct ChainNode__f0d9194 *node = Func_080d8d40();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
    node->y = y;
    node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
    node->visible = visible;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    link = &node->next;
    radius += step * 4;
  }
  *link = NULL;
  return result;
}


void Func_080d92a4(s32 index, s32 *position) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = &work->slots[index];
  struct Work9cNode *node = slot->head;
  s32 depth;

  depth = slot->depth;
  if (depth > 0) {
    do {
      depth--;
      node = node->next;
    } while (depth != 0);
  }
  SET_VECTOR(position, node->x, node->y, node->z);
}

// ---- games/tla/src/field/work9c/f0d92d4.c ----
/* VIEW(f0d92d4): tag struct ChainSlot as struct ChainSlot__f0d92d4; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5526: structure has no member named `segments'. */
struct ChainSlot__f0d92d4 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[18];
  s32 unk18;
  void *head;
};


typedef void (*TargetHook)(struct Actor *, s32 *, s32 *);

extern s32 gData_080f393c;

void wait(s32 frames);

void Func_080d92d4(s32 index, s32 fromId, s32 toId) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct Actor *from = ObjectTable_Get(fromId);
  struct Actor *to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    s32 toX = to->x;
    s32 toY = to->y;
    s32 toZ = to->z;
    struct ChainSlot__f0d92d4 *slot = &work->slots[index];
    s32 x, y = 0, z; // Default: y.
    s32 xx, yy, length, segments, count;
    s32 (*mul)(s32, s32);

    if (((u8 *)to)[99] == 99)
      (*(TargetHook *)((u8 *)to + 104))(to, &toX, &toZ);
    x = (toX - from->x) >> 8;
    y = (toY - from->y) >> 8;
    z = (toZ - from->z) >> 8;
    mul = Iwram_MulQ16;
    xx = mul(x, x);
    yy = mul(y, y);
    length = xx + yy + mul(z, z);
    segments = Iwram_SqrtEntry(length) / 6 + 1;
    count = slot->segments;
    if (segments > count) {
      count++;
      x = from->x + (toX - from->x) * count / segments;
      y = toY;
      z = from->z + (toZ - from->z) * count / segments;

      Func_080d9498(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
      slot->unk18 = 0;
      wait(1);
      Func_080d9790(index, from->x, from->y + 0x80000, from->z, x, y + 0x80000,
                    z, Trig_Sin(gData_080f393c) << 2);
    } else {
      Func_080d9498(index, from->x, from->y + 0x80000, from->z, to->x,
                    to->y + 0x80000, to->z, Trig_Sin(gData_080f393c) << 2);
      gData_080f393c += 0x1000;
      wait(6);
      Actor_SetMode(from, 2);
      Actor_SetMode(to, 2);
    }
  }
}

// ---- games/tla/src/field/work9c/f0d9498.c ----
s32 ArcTan2(s32 y, s32 x);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);

/* VIEW(f0d9498): tag struct ChainNode as struct ChainNode__f0d9498; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5634: structure has no member named `visible'. */
struct ChainNode__f0d9498 {
  struct ChainNode__f0d9498 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

/* VIEW(f0d9498): tag struct ChainSlot as struct ChainSlot__f0d9498; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5591: structure has no member named `active'. */
struct ChainSlot__f0d9498 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  s32 unk0c;
  u8 unk10[4];
  s32 unk14;
  s32 unk18;
  struct ChainNode__f0d9498 *head;
};


s32 Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 segments;
  s32 frame;
  s32 sine;
  s32 cosine;
  s32 lastX;
  s32 lastY;
  s32 lastZ;
  struct ChainSlot__f0d9498 *slot = &work->slots[index];
  struct ChainNode__f0d9498 *node = slot->head;
  void **cursor;
  s32 i;
  u16 angle;
  struct ChainNode__f0d9498 **nodes;

  if (slot->active == 1)
    return;
  slot->active = 1;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->unk13 = index + 1;
      if (i == segments) {
        ex = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);

        node->x = x + dx * frame / 2 / i + Iwram_MulQ16(ex, cosine);
        lastX = node->x;
        node->y = y + dy * frame / 2 / i + Iwram_MulQ16(ex, sine);
        lastY = node->y;
        node->z = z + dz * frame / 2 / i;
        lastZ = node->z;
      } else {
        s32 ez;

        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        ex = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                             Iwram_MulQ16(ez, ez));

        if (ex > 6) {
          node->x = lastX + (node->x - lastX) * 6 / ex;
          lastX = node->x;
          node->y = lastY + (node->y - lastY) * 6 / ex;
          lastY = node->y;
          node->z = lastZ + (node->z - lastZ) * 6 / ex;
          node->visible = 1;
          lastZ = node->z;
        }
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  slot->unk18 = 1;
  slot->unk0c = 0x40000;
  slot->unk14 = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  Heap_Release(nodes);
  return 0;
}

// ---- games/tla/src/field/work9c/f0d9710.c ----
/* VIEW(f0d9710): tag struct ChainWork as struct ChainWork__f0d9710; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5671: structure has no member named `from'. */
struct ChainWork__f0d9710 {
  struct Actor *from;
  struct Actor *to;
};

extern s32 gData_080f3940;


void Func_080d9710(s32 index, s32 fromId, s32 toId) {
  struct ChainWork__f0d9710 *work;
  struct Actor *from;
  struct Actor *to;

  work = Ram_HeapSlots->unk84[6];
  from = ObjectTable_Get(fromId);
  to = ObjectTable_Get(toId);

  if (from != NULL && to != NULL) {
    work->from = from;
    work->to = to;
    Func_080d9790(index, from->x, from->y + 0x80000, from->z, to->x,
                  to->y + 0x80000, to->z, Trig_Sin(gData_080f3940) * 4);
    gData_080f3940 += 0x1000;
  }
}

// ---- games/tla/src/field/work9c/f0d9790.c ----
s32 ArcTan2(s32 y, s32 x);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);

/* VIEW(f0d9790): tag struct ChainNode as struct ChainNode__f0d9790; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5764: structure has no member named `visible'. */
struct ChainNode__f0d9790 {
  struct ChainNode__f0d9790 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

/* VIEW(f0d9790): tag struct ChainSlot as struct ChainSlot__f0d9790; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5725: structure has no member named `active'. */
struct ChainSlot__f0d9790 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  u8 unk0c[12];
  s32 unk18;
  struct ChainNode__f0d9790 *head;
};

/* VIEW(f0d9790): tag struct ChainWork as struct ChainWork__f0d9790; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5749: structure has no member named `from'. */
struct ChainWork__f0d9790 {
  struct Actor *from;
  struct Actor *to;
  u8 unk08[4];
  struct ChainSlot__f0d9790 slots[8];
};

s32 Func_080d9790(s32 index, s32 x, s32 y, s32 z) {
  struct ChainWork__f0d9790 *work = Ram_HeapSlots->unk84[6];
  s32 zero = 0;
  struct ChainSlot__f0d9790 *slot = &work->slots[index];
  struct ChainNode__f0d9790 *node = slot->head;
  struct ChainNode__f0d9790 **cursor;
  s32 i;
  s32 angle;
  s32 radius;
  s32 lastZ;
  s32 segments;
  s32 frame;
  s32 lastX;
  s32 lastY;
  struct ChainNode__f0d9790 **nodes;

  if (slot->active == 0)
    return;
  slot->active = zero;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  slot->unk18 = zero;
  if (slot->flag != 0)
    GameFlag_ClearBit(slot->flag);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 225 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SetMode(work->from, 3);
  Actor_SetMode(work->to, 3);
  wait(4);
  for (frame = 0; frame <= segments; frame++) {
    angle = frame << 12;
    radius = 0x40000;
    for (i = 0; i <= segments; i++) {
      node = nodes[i];
      node->unk13 = 0;
      if (i <= frame) {
        u16 step;

        node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
        lastX = node->x;
        node->y = y;
        lastY = node->y;
        node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
        lastZ = node->z;
        node->visible = 2;
        step = ArcTan2(0x60000, radius);
        angle += step;
        radius += step * 4;
      } else {
        s32 dx = (lastX - node->x) >> 8;
        s32 dy = (lastY - node->y) >> 8;
        s32 dz = (lastZ - node->z) >> 8;

        radius = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                                 Iwram_MulQ16(dz, dz));

        if (radius <= 6)
          break;
        node->x = lastX + (node->x - lastX) * 6 / radius;
        lastX = node->x;
        node->y = lastY + (node->y - lastY) * 6 / radius;
        lastY = node->y;
        node->z = lastZ + (node->z - lastZ) * 6 / radius;
        node->visible = 2;
        lastZ = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}

void Func_080d9974(void) {}


extern s32 gData_080f3944;

void Func_080d8fa8(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080d9978(struct Actor *object) {
  u8 *parts = object->sprite;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gData_080f3944 = 1;
  if (gData_080f3944 != 0)
    Func_080d8fa8(0, 12, 13, 0);
}

struct Actor *Actor_GetById(u32 objectId);

s32 Func_080d99b8(s32 firstId, s32 secondId) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  s32 x = owner->x >> 20;
  s32 y = owner->y >> 20;
  s32 z = owner->z >> 20;
  s32 firstX = first->x >> 20;
  s32 firstY = first->y >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondY = second->y >> 20;
  s32 secondZ = second->z >> 20;

  if (y == firstY) {
    if (x == firstX) {
      if (z == firstZ - 1 || z == firstZ + 1 || z == firstZ)
        return 1;
    } else if (z == firstZ) {
      if (x == firstX - 1 || x == firstX + 1 || x == firstX)
        return 1;
    }
  }
  if (y == secondY) {
    if (x == secondX) {
      if (z == secondZ - 1 || z == secondZ + 1 || z == secondZ)
        return 1;
    } else if (z == secondZ) {
      if (x == secondX - 1 || x == secondX + 1 || x == secondX)
        return 1;
    }
  }
  return 0;
}


s32 Func_080d9a74(s32 id, s32 *value, s32 *index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = work->slots;
  s32 i;

  for (i = 0; i <= 7; i++) {
    if (slot->head != NULL && slot->id == id) {
      *value = slot->value;
      *index = i;
      return 0;
    }
    slot++;
  }
  return -1;
}

/// Returns the state of slot index of the heap slot 0x9c work, or -1 while the
/// work is not allocated.
s32 Func_080d9ab0(s32 index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  if (work == NULL)
    return -1;
  slot = &work->slots[index];
  return slot->state;
}

s32 GameFlag_Test(s32 flag);
void Func_080d8d68(s32 index, s32 a1, s32 a2);
void Func_080d9104(s32 index, s32 a1, s32 a2);

void Func_080d9ad0(s32 index, s32 flag, s32 a2, s32 a3) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  slot = &work->slots[index];
  slot->flag = flag;
  if (!GameFlag_Test(flag))
    Func_080d9104(index, a2, a3);
  else
    Func_080d8d68(index, a2, a3);
}


// XXX: what the work block in heap slot 40 holds, and what this family does;
// its callers and the block's readers would settle it.

extern const u8 gData_080f0c04[];

void Func_080d9b80(void);
void Func_080d9d40(void);

void Func_080d9b08(void) {
  struct WorkA0 *work = HeapSlot_AllocEwram(160, sizeof(struct WorkA0));
  void *buffer = Heap_Alloc(0x900);

  Dma_Fill32(0, work, sizeof(struct WorkA0));
  work->vramEntry = Resource_FindFreeEntry();
  Resource_DecodeType01(gData_080f0c04, buffer);
  VramCache_Load(work->vramEntry, 64, buffer);
  Task_Add(Func_080d9b80, TASK_PASS_VBLANK);
  Task_Add(Func_080d9d40, TASK_PASS_VBLANK + 15);
  Heap_Release(buffer);
}

// ---- games/tla/src/field/worka0/f0d9b80.c ----


/* VIEW(f0d9b80): tag struct ChainSlot as struct ChainSlot__f0d9b80; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5953: structure has no member named `angle'. */
struct ChainSlot__f0d9b80 {
  s16 first;
  s16 second;
  u32 angle;
  s32 amplitude;
  u8 unk0c[4];
  s32 wobble;
  struct ChainNode *tail;
  struct ChainNode *head;
};

/* VIEW(f0d9b80): tag struct ChainWork as struct ChainWork__f0d9b80; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5958: structure has no member named `angle'. */
struct ChainWork__f0d9b80 {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot__f0d9b80 slots[8];
};


void Func_080d9b80(void) {
  struct ChainWork__f0d9b80 *work = Ram_HeapSlots->unk84[7];
  s32 i;
  s32 prevX = 0;
  s32 prevY = 0;
  s32 prevZ = 0;
  s32 started = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;

  for (i = 0; i < 8; i++) {
    u32 angle = work->slots[i].angle;
    struct ChainNode *node = work->slots[i].head;
    s32 affine;
    s32 matrix;

    if (angle != 0) {
      transform.angle = angle;
      transform.scaleX = 0x100;
      transform.scaleY = 0x100;
      affine = 1;
      matrix = AffineMatrix_BuildForEffect(&transform);
    } else {
      affine = 0;
      matrix = 0;
    }
    for (; node != NULL; node = node->next) {
      s32 y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y += 0xfffc0000;

      if (started) {
        s32 screenX = (prevX + x) / 2 - viewX;
        s32 screenY;

        prevY = (prevY + y) / 2 - originY;
        prevZ = (prevZ + z) / 2 - viewZ - originY;
        screenY = prevZ - prevY;

        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 left = ((screenX >> 16) - 4) & 0x1ff;
          s32 top = ((screenY >> 16) - 4) & 0xff;
          s32 key = ((prevY + prevZ) >> 16) + 58;

          ((u32 *)&node->sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = top | (left << 16) | 0x2000;
          ((u32 *)&node->sprite)[2] = tile | 0x800;
          node->sprite.affine = affine;
          node->sprite.affineIndex = matrix;
          Func_080140d8(&node->sprite, key);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
      started = 1;
    }
    prevX = 0;
    prevY = 0;
    prevZ = 0;
    started = 0;
  }
}


struct WobbleEvent {
  u8 unk000[362];
  u16 cue;
  u8 unk16c[24];
  s16 counter;
};

s32 GameFlag_Test(s32 flag);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Func_080d99b8(s32 first, s32 second);
void Func_080da060(s32 index, s32 first, s32 second, s32 wobble);

/// The heap slot 0xa0 work's per-frame callback (Func_080d9b08 adds it).
/// Unless flag 260 is set, once the event work's counter passes 12 it picks
/// the event work's cue: with render mode 0 the slot linking the leader's
/// nearby talk target sets 0xfc83 when Func_080d99b8 accepts its two ends, and
/// render mode 4 sets 0xfc84. Then every active slot wobbles: the amplitude
/// decays by 0.95 a frame until it drops to 0x1998, and Func_080da060 redraws
/// the slot with the sine of its angle times it.
void Func_080d9d40(void) {
  struct WobbleEvent *event = Ram_HeapSlots->eventWork;
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Slot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->counter > 12) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(party->currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++) {
          if (work->slots[i].first == target || work->slots[i].second == target)
            break;
        }
        if (i < 8 && Func_080d99b8(work->slots[i].first, work->slots[i].second))
          event->cue = 0xfc83;
      }
    } else if (party->renderMode == 4) {
      event->cue = 0xfc84;
    }
  }

  slot = work->slots;
  for (i = 0; i < 8; i++, slot++) {
    if (slot->tail != NULL) {
      s32 wobble = Iwram_MulQ16(Trig_Sin(slot->angle), slot->amplitude);

      slot->angle += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->tail = NULL;
      }
      Func_080da060(i, slot->first, slot->second, wobble);
    }
  }
}

struct WorkA0Node *Func_080d9e74(void) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}


s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080d9e98(s32 slotNo, s32 firstId, s32 secondId) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    struct WorkA0Slot *slot = &work->slots[slotNo];

    slot->first = firstId;
    slot->second = secondId;
    Func_080d9f1c(slotNo, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z);
    Actor_SetMode(first, 4);
    Actor_SetMode(second, 4);
  }
}

s32 ArcTan2(s32 x, s32 y);

struct LineNode {
  struct LineNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 kind;
  u8 slot;
};

struct LineSlot {
  s16 first;
  s16 second;
  s32 direction;
  s32 amplitude;
  u8 unk0c[4];
  s32 angle;
  s32 active;
  struct LineNode *head;
};

struct LineWork {
  u8 unk00[12];
  struct LineSlot slots[8];
};

s32 Func_080d9f1c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 result = 0;
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(sx, sx) + Iwram_MulQ16(sy, sy) +
                                 Iwram_MulQ16(sz, sz)) /
                     6 +
                 1;
  struct LineNode **link = &work->slots[index].head;
  s32 i;

  work->slots[index].direction = (u16)ArcTan2(sz, sx);
  for (i = 0; i <= segments; i++) {
    struct LineNode *node = Func_080d9e74();

    if (node == NULL) {
      result = -1;
      break;
    }
    node->slot = index + 1;
    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments;
    node->z = z + i * dz / segments;
    if (i == segments)
      node->kind = 2;
    else
      node->kind = 1;
    *link = node;
    link = &node->next;
  }
  *link = NULL;
  work->slots[index].active = 1;
  return result;
}

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 to_x, s32 to_y, s32 to_z,
                  s32 mode);

void Func_080da060(s32 index, s32 from, s32 to, s32 mode) {
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL)
    Func_080da0b0(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000, b->z,
                  mode);
}

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

s32 Func_080da0b0(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ,
                  s32 height) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 dx = toX - x;
  s32 sx = dx >> 8;
  s32 dy = toY - y;
  s32 sy = dy >> 8;
  s32 dz = toZ - z;
  s32 sz = dz >> 8;
  s32 segments = Iwram_SqrtEntry(IwramMulQ16(sx, sx) + IwramMulQ16(sy, sy) +
                                 IwramMulQ16(sz, sz)) /
                     6 +
                 1;
  struct LineNode *node = work->slots[index].head;
  s32 i;
  s32 one = 1;

  for (i = 0; i <= segments; i += one) {
    s32 bow = IwramMulQ16(Trig_Sin((i << 15) / segments), height);

    node->x = x + i * dx / segments;
    node->y = y + i * dy / segments + bow;
    node->z = z + i * dz / segments;
    node->kind = one;
    node = node->next;
  }
  return 0;
}

s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ);

void Func_080da1b8(s32 index, s32 from, s32 to) {
  struct WorkA0 *work = WORK_A0;
  struct Actor *a = ObjectTable_Get(from);
  struct Actor *b = ObjectTable_Get(to);

  if (a != NULL && b != NULL) {
    struct WorkA0Slot *slot = &work->slots[index];

    slot->first = from;
    slot->second = to;
    Func_080da22c(index, a->x, a->y + 0x80000, a->z, b->x, b->y + 0x80000,
                  b->z);
  }
}

// Builds a chain of nodes around a point.


/// Builds a slot's chain of nodes, one for each 6 units of the distance between
/// two points, laid out as a flat spiral around the first point, every node of
/// kind 2.
s32 Func_080da22c(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct LineWork *work = Ram_HeapSlots->unk84[7];
  s32 result = 0;
  s32 dx = (toX - x) >> 8;
  s32 dy = (toY - y) >> 8;
  s32 dz = (toZ - z) >> 8;
  s32 length = Iwram_SqrtEntry(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) +
                               Iwram_MulQ16(dz, dz));
  s32 segments = length / 6 + 1;
  struct LineNode **link = &work->slots[index].head;
  s32 angle = 0;
  s32 radius = 0x40000;
  s32 i;
  s32 kind = 2; // One constant for its whole life.

  for (i = 0; i <= segments; i++) {
    struct LineNode *node = Func_080d8d40();
    u16 step;

    if (node == NULL) {
      result = -1;
      break;
    }
    node->x = x + Iwram_MulQ16(Trig_Cos(angle), radius);
    node->y = y;
    node->z = z + Iwram_MulQ16(Trig_Sin(angle), radius);
    node->kind = kind;
    *link = node;
    step = ArcTan2(0x60000, radius);
    angle += step;
    link = &node->next;
    radius += step * 4;
  }
  *link = NULL;
  return result;
}


extern s32 gData_080f3948;

void Func_080da334(s32 arg, s32 firstId, s32 secondId) {
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    Func_080da3bc(arg, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z,
                  Trig_Sin(gData_080f3948) << 2);
    gData_080f3948 += 0x1000;
    wait(6);
    Actor_SetMode(first, 2);
    Actor_SetMode(second, 2);
  }
}

// ---- games/tla/src/field/worka0/f0da3bc.c ----
s32 ArcTan2(s32 y, s32 x);
void wait(s32 frames);

s32 Func_080da3bc(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct WorkA0 *work = WORK_A0;
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 ez = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez)) /
                     6 +
                 1;
  s32 frame;
  struct WorkA0Node *node = work->slots[index].head;
  s32 sine;
  s32 height;
  s32 cosine;
  s32 lastX, lastY, lastZ;
  struct WorkA0Node **nodes;
  struct WorkA0Node **cursor;
  struct WorkA0Slot *slot;
  s32 i;
  u16 angle;

  cursor = nodes = Heap_Alloc(0x200);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 319 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      // XXX: byte 17 of a node is the slot it belongs to, plus one; likely
      // what the per-frame callbacks read to draw it.
      node->unk11[0] = index + 1;
      if (i == segments) {
        s32 offsetX;
        s32 offsetY;

        height = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);
        offsetX = Iwram_MulQ16(height, cosine);
        lastX = node->x = x + dx * frame / 2 / i + offsetX;
        offsetY = Iwram_MulQ16(height, sine);
        lastY = node->y = y + dy * frame / 2 / i + offsetY;
        lastZ = node->z = z + dz * frame / 2 / i;
      } else {
        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        height = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez));

        if (height <= 6)
          break;
        lastX = node->x = lastX + (node->x - lastX) * 6 / height;
        lastY = node->y = lastY + (node->y - lastY) * 6 / height;
        lastZ = node->z = lastZ + (node->z - lastZ) * 6 / height;
        node->used = 1;
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  // XXX: the field worka0.h calls tail is tested as a flag by Func_080d9d40;
  // a pointer here only because the adopted header types it so.
  slot->tail = (struct WorkA0Node *)1;
  slot->amplitude = 0x40000;
  slot->angle = 0x4000;
  Heap_Release(nodes);
  return 0;
}


extern s32 gData_080f394c;
s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave);

void Func_080da660(s32 arg, s32 firstId, s32 secondId) {
  struct Actor **work;
  struct Actor *first;
  struct Actor *second;

  work = (struct Actor **)WORK_A0;
  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    work[0] = first;
    work[1] = second;
    Func_080da6e0(arg, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z,
                  Trig_Sin(gData_080f394c) << 2);
    gData_080f394c += 0x1000;
  }
}


s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *tail = NULL;
  struct WorkA0Node *node;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 x, y, z;
  struct WorkA0Node **nodes;
  struct WorkA0Node **entry;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
  count = Iwram_Sqrt(dist) / 6 + 1;
  node = work->slots[slot].head;
  entry = nodes = Heap_Alloc(0x200);
  work->slots[slot].tail = tail;
  if (count >= 0) {
    i = count + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 259 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *entry++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SetMode(((void **)work)[0], 3);
  Actor_SetMode(((void **)work)[1], 3);
  wait(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame << 12;
    dist = 0x40000;
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->unk11[0] = 0;
      if (i <= frame) {
        x = node->x = x1 + Iwram_MulQ16(Trig_Cos(angle), dist);
        y = node->y = y1;
        z = node->z = z1 + Iwram_MulQ16(Trig_Sin(angle), dist);
        node->used = 2;
        {
          u16 step = ArcTan2(0x60000, dist);

          angle += step;
          dist += step * 4;
        }
      } else {
        dx = (x - node->x) >> 8;
        dy = (y - node->y) >> 8;
        dz = (z - node->z) >> 8;
        dist =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
        dist = Iwram_Sqrt(dist);
        if (dist <= 6)
          break;
        x = node->x = x + (node->x - x) * 6 / dist;
        y = node->y = y + (node->y - y) * 6 / dist;
        node->z = z + (node->z - z) * 6 / dist;
        node->used = 2;
        z = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}


extern s32 gData_080f3950;

void Func_080da060(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080da908(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 0;
  work->slots[index].head = NULL;
}

void Func_080da938(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 2;
  work->slots[index].tail = NULL;
}

void Func_080da968(struct Actor *object) {
  u8 *parts = object->sprite;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gData_080f3950 = 1;
  if (gData_080f3950 != 0)
    Func_080da060(0, 12, 13, 0);
}


// The four 256-byte pictures of each set (type-01 compressed).
extern const u8 Data_080f0c1c[], Data_080f0c63[], Data_080f0cb8[];
extern const u8 Data_080f0d07[], Data_080f0d5e[], Data_080f0dbb[];
extern const u8 Data_080f0e00[];

s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_080daab0(void);
void Func_080dacb8(void);

struct TileEffectWork {
  s32 vramEntry;
  u8 unk04[0x10e0];
};

#define TILE_EFFECT_SLOT 164
#define TILE_EFFECT_KIND_ALT 372 // the kind that takes the first picture set
#define PICTURE_SIZE 0x100

/// Starts a field effect that animates sixteen tiles of its own: takes a
/// 0x10e4-byte work block at heap offset 164 (worka0.h's WorkA4 lies there),
/// clears it, decodes four 256-byte tile pictures into a scratch buffer (one
/// set for kind 372, another for the rest; the second set repeats one
/// picture), loads them into a VRAM slot and schedules the two per-frame
/// routines Func_080daab0 and Func_080dacb8.
struct TileEffectWork *Func_080da9a8(s32 kind) {
  struct TileEffectWork *work;
  u8 *pictures;
  s32 size;

  work = HeapSlot_AllocEwram(TILE_EFFECT_SLOT, sizeof(struct TileEffectWork));
  size = 4 * PICTURE_SIZE;
  pictures = Heap_Alloc(size);
  Dma_Fill32(0, work, sizeof(struct TileEffectWork));
  work->vramEntry = Resource_FindFreeEntry();
  if (kind == TILE_EFFECT_KIND_ALT) {
    Resource_DecodeType01(Data_080f0c1c, pictures);
    Resource_DecodeType01(Data_080f0c63, pictures + PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0cb8, pictures + 2 * PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0d07, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(dead): an empty once-only block closing the arm keeps each
     * decode's destination set before its source loads; without it, 4 of 95
     * instructions differ in tla-en. */
    do {
    } while (0);
  } else {
    Resource_DecodeType01(Data_080f0d5e, pictures);
    Resource_DecodeType01(Data_080f0dbb, pictures + PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0dbb, pictures + 2 * PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0e00, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(barrier): an empty asm closing the arm keeps each decode's
     * destination set before its source loads; without it, 4 of 95
     * instructions differ in tla-en. */
    asm("");
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_080daab0, priority);
  }
  Task_Add(Func_080dacb8, 0x48f);
  Heap_Release(pictures);
  return work;
}

#include "affine.h"
#include "graphics/vram_tab.h"


// The transform as this function builds it, a word at a time.

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 kind; ///< 0 or 1: not drawn; else its picture, from 2
  u8 unk13;
  struct AnimationSpritePart sprite;
};

/* VIEW(f0da3bc): tag struct ChainSlot as struct ChainSlot__f0da3bc; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6634: structure has no member named `root'. */
struct ChainSlot__f0da3bc {
  s16 id;
  u8 unk02[22];
  struct ChainLink *root;
};

/* VIEW(f0da3bc): tag struct ChainWork as struct ChainWork__f0da3bc; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6624: structure has no member named `sheet'. */
struct ChainWork__f0da3bc {
  s32 sheet; ///< the resource entry of the links' tiles
  struct ChainSlot__f0da3bc slots[8];
};

void Func_080daab0(void) {
  struct ChainWork__f0da3bc *work = Ram_HeapSlots->unk84[8];
  s32 i;
  u32 angle;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tiles = gResourceTableEntries[work->sheet].offset >> 5;
  s32 prevZ = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 height = map->origin[1];
  struct ChainTransform transform;
  struct ChainLink *link;

  for (i = 0; i < 8; i++) {
    for (link = work->slots[i].root; link != NULL; link = link->next) {
      s32 x = link->x;
      s32 y = link->y;
      s32 z = link->z;

      if (link->kind > 1) {
        s32 dz = prevZ - z;
        s32 absZ = dz >= 0 ? dz : z - prevZ;
        s32 dy = prevY - y;
        s32 screenX, screenY;
        u32 tile;

        if (dy >= 0 ? absZ < dy : absZ < y - prevY)
          angle = (u16)ArcTan2(prevX - x, dy);
        else
          angle = (u16)ArcTan2(prevX - x, dz);
        tile = tiles + link->kind * 8 - 16;
        screenX = (prevX + x) / 2 - viewX;
        prevY = (prevY + y) / 2 - height;
        prevZ = (prevZ + z) / 2 - viewZ - height;
        screenY = prevZ - prevY;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 sx = ((screenX >> 16) - 8) & 0x1ff;
          s32 sy = ((screenY >> 16) - 8) & 0xff;
          s32 priority = ((prevY + prevZ) >> 16) + 34;
          struct AnimationSpritePart *sprite = &link->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&link->sprite)[1] = sy | (sx << 16) | 0x40002000;
          ((u32 *)&link->sprite)[2] = tile | 0x800;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            link->sprite.affine = 1;
            link->sprite.affineIndex = AffineMatrix_BuildForEffect(
                (struct AffineTransform *)&transform);
          }
          link->angle = angle;
          Func_080140d8(sprite, priority);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
    }
    prevX = 0;
    prevY = 0;
    link = work->slots[i].root;
    prevZ = 0;
    if (link != NULL)
      link->angle = angle;
  }
}
#undef TILE_EFFECT_SLOT
#undef TILE_EFFECT_KIND_ALT
#undef PICTURE_SIZE

// ---- games/tla/src/field/worka0/f0dacb8.c ----

/* VIEW(f0dacb8): tag struct ChainSlot as struct ChainSlot__f0dacb8; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6731: structure has no member named `root'. */
struct ChainSlot__f0dacb8 {
  s16 objectId;
  u8 unk02[2];
  s32 swing;   // the arc's width, eased each frame
  s32 spacing; // between links
  u8 unk08[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

/* VIEW(f0dacb8): tag struct ChainWork as struct ChainWork__f0dacb8; visual_attributes.c has another definition; tried with the definition the rest of the object has, Func_080dacb8 moves: 1 of 173 instructions in tla-ja. */
struct ChainWork__f0dacb8 {
  u8 unk00[4];
  struct ChainSlot__f0dacb8 slots[8];
};

struct Actor *Actor_GetById(u32 objectId);
s32 Party_GetLeaderActor(void);

#define CHAIN_WORK ((struct ChainWork__f0dacb8 *)Ram_HeapSlots->unk84[8])

void Func_080dacb8(void) {
  struct ChainWork__f0dacb8 *work = CHAIN_WORK;
  struct ChainSlot__f0dacb8 *slot;
  struct Actor *leader;
  s32 x, y, z;
  struct ChainLink *link;
  s32 i;
  s32 target;
  s32 phase;
  s32 swing;
  s32 sway;
  s32 radius;
  s32 angle;

  i = 0;
  leader = Actor_GetById(Party_GetLeaderActor());
  target = 0x100;
  for (slot = work->slots; i <= 7; i++, slot++) {
    if (slot->root == 0 || slot->active == 0)
      continue;

    slot->phase += 0x200;
    link = slot->root->next;
    x = link->x;
    y = link->y;
    z = link->z;
    swing = slot->swing;

    // Swing less while the leader stands within 8 pixels across, a pixel
    // deep, and between 8 above and 80 below the anchor.
    if ((x - leader->x >= 0 ? x - leader->x <= 0x7ffff
                            : leader->x - x <= 0x7ffff) &&
        (u32)(z - leader->z + 0xfffff) <= 0xffffe &&
        y + (8 << 16) > leader->y && y - (80 << 16) < leader->y)
      target = 32;

    // Hang the five links below the anchor.
    // BUG: the link loop reuses the chain counter, which it leaves at 6, so
    // after the first active chain only the chain after it is updated.
    link = link->next;
    for (i = 1, sway = swing; i <= 5; i++) {
      radius = i * slot->spacing;
      angle = Iwram_MulQ16(Trig_Sin(slot->phase - i * 800), swing + sway / 2);
      link->x = x - Iwram_MulQ16(Trig_Sin(angle), radius);
      sway += swing;
      link->y = y - Iwram_MulQ16(Trig_Cos(angle), radius);
      link->z = z;
      link = link->next;
    }

    // Ease the swing towards the target.
    if (slot->swing > target)
      slot->swing = Iwram_MulQ16(slot->swing, 0xfd70);
    else
      slot->swing = Iwram_MulQ16(slot->swing, 0x11c28);
  }
}


/// Returns the first of WORK_A4's eight slots with no chain, or NULL.
struct WorkA4Slot *Func_080dae3c(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r0, #0); without it, 2 of
   * 26 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].root == NULL) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}

/// Returns the slot among WORK_A4's eight that holds id, or NULL.
struct WorkA4Slot *Func_080dae70(s32 id) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r5, #0); without it, 2 of
   * 27 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].id == id) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}

struct WorkA4Item *Func_080daea8(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Item *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}
#undef CHAIN_WORK

// ---- games/tla/src/field/worka0/f0daecc.c ----

/* VIEW(f0daecc): tag struct ChainSlot as struct ChainSlot__f0daecc; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6922: structure has no member named `root'. */
struct ChainSlot__f0daecc {
  s16 objectId;
  u8 unk02[2];
  s32 swing;
  s32 spacing;
  u8 unk0c[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

struct ChainObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x3c];
  struct {
    u8 unk00[0x28];
    s16 *entries[4];
  } *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
};

struct Actor *Actor_GetById(u32 objectId);
struct WorkA4Slot *Func_080dae70(s32 id);

#define CELL_MASK 0xfff00000 // 16-pixel cells

void Func_080daecc(s32 objectId, s32 grow) {
  struct ChainObject *object;
  struct MapWork *map;
  struct ChainSlot__f0daecc *slot;
  struct ChainLink *link;
  struct ChainLink **tail;
  struct MapCell *cells;
  struct MapCell *cell;
  s32 i;
  s32 x;
  s32 top;
  s32 z;
  s32 y;
  s32 frame;
  s32 last;
  s32 kind;
  s32 running;

  object = Actor_GetById(objectId);
  running = Ram_HeapSlots->unk84[8];
  kind = 372;
  map = Ram_HeapSlots->mapWork;
  if (running == 0) {
    if (object->sprite != 0 && object->sprite->entries[0] != 0)
      kind = *object->sprite->entries[0];
    Func_080da9a8(kind);
  }
  if (Func_080dae70(objectId) != 0)
    return;
  slot = Func_080dae3c();
  if (slot == 0)
    return;

  // Clear the collision codes of the five cells above the object.
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cells = map->layers[2].cells;
  for (i = 0; i < 5; i++) {
    z++;
    cell = cells;
    cell += x + z * 128;
    cell->collisionCode = 0;
  }

  x = (object->x & CELL_MASK) + (8 << 16);
  y = object->y & CELL_MASK;
  top = y + (8 << 16);
  z = (object->z & CELL_MASK) + (16 << 16);
  Actor_SetMode((struct Actor *)object, 0);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->z += 16 << 16;
  object->y -= 64 << 16;
  Actor_SetSingleChildFlagsFar(object, 0);

  // Hang the links.
  last = 6;
  tail = &slot->root;
  for (i = 0; i <= last && (link = Func_080daea8()) != 0; i++) {
    link->x = x;
    link->y = y + (15 << 16) - i * (14 << 16);
    link->z = z;
    if (i == 0)
      link->kind = 1;
    else if (i == 1)
      link->kind = 2;
    else if (i == last)
      link->kind = 5;
    else
      link->kind = i % 2 + 3;
    *tail = link;
    tail = &link->next;
  }

  if (grow == 0) {
    for (i = 0, link = slot->root; i <= last && link != 0; i++)
      link = link->next;
  } else {
    for (frame = 1; frame <= 28; frame++) {
      i = 0;
      link = slot->root;
      if (i <= last && link != 0) {
        s32 drop = -(frame << 15);

        do {
          link->x = x;
          link->y = top - drop / 2;
          link->z = z;
          drop += frame << 16;
          i++;
          link = link->next;
        } while (i <= last && link != 0);
      }
      wait(1);
    }
  }
  slot->active = 1;
  slot->phase = 0;
  slot->swing = 0x100;
  slot->objectId = objectId;
  slot->spacing = 14 << 16;
}


struct Unk_080db0b0Sprite {
  u8 unk00[18];
  u16 unk12;
};

// The half turn on the leader's side of the anchor: 0 on its right, 1 on its
// left.
#define SIDE_TURN(side) ((side) * (TRIG_TURN / 2))

void Func_080db0b0(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct Unk_080db0b0Sprite *heldSprite;
  s32 side = 0;
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  struct Actor *anchor = Actor_GetById(id);
  /* FAKEMATCH(wrapper): the slot read through a one-member union, which may
   * alias every store, so its reads stay after the stores before them; as
   * struct WorkA4Slot *, 12 of 378 instructions differ in tla-en. */
  union {
    struct WorkA4Slot s;
  } *slot;
  struct Unk_080db0b0Sprite *sprite;
  s32 swing;
  s32 start;
  s32 dir;
  u16 letGo;
  s32 radius;
  s32 steps;
  s32 angle;
  s32 sine;
  s32 i;

  if (leader == NULL || anchor == NULL)
    return;
  motionFlags = leader->motionFlags;
  heldSprite = leader->sprite;
  if (leader->x < anchor->x)
    side = 1;
  slot = (void *)Func_080dae70(id);
  if (slot == NULL)
    return;

  start = slot->s.swing;
  if ((slot->s.phase ^ SIDE_TURN(side)) & (TRIG_TURN / 2)) {
    for (swing = start; swing >= 0; swing -= 64) {
      slot->s.swing = swing;
      wait(1);
    }
    start = 0;
  }
  for (swing = start; swing <= 1536; swing += 64) {
    slot->s.swing = swing;
    slot->s.phase = SIDE_TURN(side) + TRIG_TURN / 4;
    wait(1);
  }
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side ^ 1) + TRIG_TURN / 4) {
    slot->s.swing = 1536;
    wait(1);
  }

  sprite = leader->sprite;
  dir = 1;
  if (side) {
    letGo = KEY_DOWN | KEY_LEFT | KEY_B;
    dir -= 2;
  } else
    letGo = KEY_DOWN | KEY_RIGHT | KEY_B;
  slot->s.active = 1;
  Actor_SetMode(leader, 32);
  while (1) {
    slot->s.phase = SIDE_TURN(side ^ 1) + TRIG_TURN / 4;
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->s.swing = 1568;
      slot->s.spacing = Q16_ONE * 29 / 2;
      sprite->unk12 = dir << 10;
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->s.swing += 16;
      slot->s.spacing += Q16_ONE / 4;
      sprite->unk12 += dir << 9;
      break;
    default:
      if (slot->s.swing > 1536)
        slot->s.swing -= 32;
      else
        slot->s.swing = 1536;
      if (slot->s.spacing > Q16_ONE * 14)
        slot->s.spacing -= Q16_ONE / 32;
      if (sprite->unk12 != 0)
        sprite->unk12 -= dir << 7;
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->unk12 = 0;
      slot->s.active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    wait(1);
  }

  sprite->unk12 = 0;
  slot->s.active = 0;
  Actor_SetMode(leader, 6);
  wait(6);
  Sound_PlayFar(152);
  Actor_SetMode(leader, 7);
  leader->speedLimit = Q16_ONE * 3;
  leader->acceleration = Q16_ONE * 2;
  leader->velocityY = Q16_ONE * 4;
  leader->motionFlags &= 0x7e;
  Actor_SetSingleChildFlagsFar(leader, 0);
  wait(5);
  Actor_SetMode(leader, 16);
  leader->motionFlags = 0;
  rootX = slot->s.root->next->x;
  rootY = slot->s.root->next->y;
  radius = Q16_ONE * 65;
  steps = 8;
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side) + TRIG_TURN * 7 / 32) {
    if (steps != 0) {
      radius += Q16_ONE * 5 / 16;
      slot->s.spacing += Q16_ONE / 16;
      steps--;
    }
    slot->s.swing = 2048;
    slot->s.active = 1;
    // A once-only block: the sine is taken before Iwram_MulQ16 is loaded.
    do {
      sine = Trig_Sin(slot->s.phase - 4000);
    } while (0);
    angle = Iwram_MulQ16(sine, slot->s.swing + slot->s.swing * 5 / 2);
    leader->x = rootX - Iwram_MulQ16(Trig_Sin(angle), radius);
    leader->y = rootY - Iwram_MulQ16(Trig_Cos(angle), radius);
    leader->targetX = leader->x;
    leader->targetY = leader->y;
    heldSprite->unk12 = angle;
    wait(1);
  }
  slot->s.active = 1;
  Actor_SetMode(leader, 7);
  leader->motionFlags = motionFlags & ~1;
  leader->velocityY = Q16_ONE * 4;
  for (i = 7; i >= 0; i--) {
    slot->s.spacing -= Q16_ONE / 16;
    wait(1);
  }
  slot->s.spacing = Q16_ONE * 14;
  heldSprite->unk12 = 0;
  Actor_SetMode(leader, 6);
  Actor_SetSingleChildFlagsFar(leader, 1);
  leader->motionFlags = motionFlags;
  wait(6);
  slot->s.active = 1;
}

/// Starts an object's chain (Func_080daecc). An object with no motion flags is
/// first moved -16 pixels in z and 64 pixels in y; one with motion
/// flags starts it only when force is set.
void Func_080db444(s32 id, s32 force) {
  struct Actor *object = Actor_GetById(id);
  s32 y;
  s32 zero;

  if (object != NULL) {
    if (object->motionFlags == 0) {
      zero = 0;
      object->z += 0xfff00000;
      y = object->y;
      object->y = y + 0x400000;
      Func_080daecc(id, zero);
    } else if (force) {
      Func_080daecc(id, 0);
    }
  }
}

// Field Psynergy: the dispatchers that run each animation

#include "field_psynergy_work.h"

/// The Psynergy animations (BattleFxScene.animation) the dispatchers name.
// The Broken Seal has 1-16; ⚓️ adds 17-30.
enum FieldPsynergyAnimation {
  FIELD_FX_PUSH = 1,  ///< pushes the target the way the pad points
  FIELD_FX_WAVE = 2,  ///< the hue wave from the caster to the target
  FIELD_FX_PAUSE = 9, ///< stops an object in its tracks
};

void BattleEffect_Run(void);
void BattleEffect_DispatchRequestKind(void);
void BattleEffect_ClearChildValueOnMismatch(void);
void FieldEvent_RunTypeHandler(void);

void FieldEffect_RunMove(void);
void Func_080ddea8(void);
void Func_080df6f4(void);
void Func_080de21c(void);
void FieldEffect_RunDouse(void);
void FieldEffect_RunWhirlwind(void);
void FieldEffect_RunFrost(void);
void FieldEffect_RunPly(void);
void FieldEffect_RunGrowth(void);
void Func_080e03cc(void);
void FieldEffect_RunRetreat(void);
void BattleEffect_RunEffect16(void);
void Func_080decb8(void);
void Func_080dd820(void);
void Func_080e09c0(void);
void Func_080e1f2c(void);
void Func_080e3074(void);
void Func_080e3d04(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e9090(void);
void BattleEffect_RunViewSway(void);
void Func_080e9af4(void);
void Func_080e4a34(void);
void BattleEffect_RunEffect30(void);
void Func_080e7844(void);
void Func_080e82cc(void);
void BattleEffect_ResumeObject(s32 id);
s32 BattleEffect_FindNearestObject(s32 self, s32 animation);
s32 BattleEffect_FilterObjectIdByFlags(s32 id);
void *Scene_FindActorTalkTrigger(s32 id);
void BattleEffect_SetupObjectPair(s32 userId, s32 targetId);
void BattleEffect_MarkChildAndRunFallbackTransition(s32 id);
void BattleEffect_PauseObject(s32 id);
void Func_080e0134(void);
void Func_080dd950(void);
void Func_080dc978(s32 sourceId, s32 targetId);
void Func_080dd054(s32 targetId);
void FieldEffect_RunLiftOnTarget(s32 targetId);
void BattleEffect_SteerLiftedTarget(s32 targetId);
void FieldEffect_RunForceOnTarget(s32 targetId);
void FieldEffect_RunDouseOnTarget(s32 targetId);
void FieldEffect_RunFrostOnTarget(s32 targetId);
void Func_080dda30(s32 targetId);
void FieldEffect_RunPlyOnTarget(s32 targetId);
/* VIEW(f0daecc): routine FieldEffect_RunWhirlwindOnTarget as void FieldEffect_RunWhirlwindOnTarget(s32 targetId); battle.h has void FieldEffect_RunWhirlwindOnTarget(void); tried as the definition's void FieldEffect_RunWhirlwindOnTarget(void), which the object already declares, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:7369: too many arguments to function `FieldEffect_RunWhirlwindOnTarget'. */
extern void FieldEffect_RunWhirlwindOnTarget__f0daecc(s32 targetId) __asm__("FieldEffect_RunWhirlwindOnTarget");
void BattleEffect_CallEffect13(s32 targetId);
void Func_080e0978(s32 targetId);
void BattleEffect_CallEffect23(s32 targetId);
void BattleEffect_CallEffect26(s32 targetId);
void BattleEffect_CallSpinEffect(s32 targetId);
void BattleEffect_CallEffect20(s32 targetId);
void Func_080e8db0(s32 targetId);
void Func_080e92b4(s32 targetId);
void BattleEffect_CallEffect24(s32 targetId);
void Func_080e7818(s32 targetId);
void BattleEffect_CallEffect22(s32 targetId);
void BattleEffect_CallEffect21(s32 targetId);
void BattleEffect_CallEffect18(s32 targetId);

/// Runs the animation's handler for kind: 0 runs it with no target chosen, 1
/// on the chosen target (secondObjectId), 2 finishes it and 3 runs the kind's
/// extra step.
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == 0)
    BattleEffect_Run();
  else if (kind == 1)
    BattleEffect_DispatchRequestKind();
  else if (kind == 2)
    BattleEffect_ClearChildValueOnMismatch();
  else if (kind == 3)
    FieldEvent_RunTypeHandler();
}

/// Runs the field Psynergy animation the scene names, with no target chosen
/// yet: each kind runs its effect to the end.
void BattleEffect_Run(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->fieldKind;
  s32 targetId = scene->secondObjectId;
  s32 objectId;

  if (animation == FIELD_FX_PUSH)
    FieldEffect_RunMove();
  else if (animation == 7)
    Func_080ddea8();
  else if (animation == 11)
    Func_080df6f4();
  else if (animation == 4)
    Func_080de21c();
  else if (animation == 5)
    FieldEffect_RunDouse();
  else if (animation == 14)
    FieldEffect_RunWhirlwind();
  else if (animation == 6)
    FieldEffect_RunFrost();
  else if (animation == 3)
    FieldEffect_RunPly();
  else if (animation == 12)
    FieldEffect_RunGrowth();
  else if (animation == 13)
    Func_080e03cc();
  else if (animation == 15)
    FieldEffect_RunRetreat();
  else if (animation == 16)
    BattleEffect_RunEffect16();
  else if (animation == 10)
    Func_080decb8();
  else if (animation == 8)
    Func_080dd820();
  else if (animation == 17)
    Func_080e09c0();
  else if (animation == 29)
    Func_080e1f2c();
  else if (animation == 25)
    Func_080e3074();
  else if (animation == 26)
    Func_080e3d04();
  else if (animation == 21)
    Func_080e572c();
  else if (animation == 18)
    Func_080e5d5c();
  else if (animation == 20)
    Func_080e68d0();
  else if (animation == 19)
    Func_080e9090();
  else if (animation == 27)
    BattleEffect_RunViewSway();
  else if (animation == 24)
    Func_080e9af4();
  else if (animation == 23)
    Func_080e4a34();
  else if (animation == 30)
    BattleEffect_RunEffect30();
  else if (animation == 22)
    Func_080e7844();
  else if (animation == 28)
    Func_080e82cc();
  else if (animation == FIELD_FX_PAUSE) {
    // Let the object stopped last time go, then stop the nearest one that
    // has an event to run; with none, play the fallback transition.
    struct MainRoutineWork *party = &gMainRoutineWork;
    s32 paused;

    paused = party->pausedObject;
    if (paused != -1) {
      BattleEffect_ResumeObject(paused);
      party->pausedObject = -1;
    }
    objectId = BattleEffect_FindNearestObject(scene->firstObjectId, animation);
    objectId = BattleEffect_FilterObjectIdByFlags(objectId);
    if (Scene_FindActorTalkTrigger(objectId) != 0) {
      BattleEffect_SetupObjectPair(scene->firstObjectId, objectId);
      BattleEffect_MarkChildAndRunFallbackTransition(objectId);
      BattleEffect_PauseObject(objectId);
      party->pausedObject = objectId;
    } else {
      Func_080e0134();
    }
  } else if (animation == FIELD_FX_WAVE) {
    // A wave still running from the last cast is reset first.
    if (*(s8 *)(event + 0xca4) != 0)
      Func_080dd950();
    Func_080dc978(scene->firstObjectId, targetId);
  }
}

/// Runs the field Psynergy animation the scene names on the target the player
/// chose (BattleFxScene.secondObjectId).
void BattleEffect_DispatchRequestKind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->fieldKind;
  s32 targetId = scene->secondObjectId;

  scene->enabled = 0;
  if (animation == FIELD_FX_PUSH)
    Func_080dd054(targetId);
  else if (animation == 7)
    FieldEffect_RunLiftOnTarget(targetId);
  else if (animation == 11)
    BattleEffect_SteerLiftedTarget(targetId);
  else if (animation == 4)
    FieldEffect_RunForceOnTarget(targetId);
  else if (animation == 5)
    FieldEffect_RunDouseOnTarget(targetId);
  else if (animation == 6)
    FieldEffect_RunFrostOnTarget(targetId);
  else if (animation == 12)
    Func_080dda30(targetId);
  else if (animation == 3)
    FieldEffect_RunPlyOnTarget(targetId);
  else if (animation == 14)
    FieldEffect_RunWhirlwindOnTarget__f0daecc(targetId);
  else if (animation == 13)
    BattleEffect_CallEffect13(targetId);
  else if (animation == 16)
    BattleEffect_RunEffect16();
  else if (animation == 17)
    Func_080e0978(targetId);
  else if (animation == 10)
    Func_080decb8();
  else if (animation == 15)
    FieldEffect_RunRetreat();
  else if (animation == 8)
    Func_080dd820();
  else if (animation == 23)
    BattleEffect_CallEffect23(targetId);
  else if (animation == 26)
    BattleEffect_CallEffect26(targetId);
  else if (animation == 25)
    BattleEffect_CallSpinEffect(targetId);
  else if (animation == 20)
    BattleEffect_CallEffect20(targetId);
  else if (animation == 19)
    Func_080e8db0(targetId);
  else if (animation == 27)
    Func_080e92b4(targetId);
  else if (animation == 24)
    BattleEffect_CallEffect24(targetId);
  else if (animation == 30)
    Func_080e7818(targetId);
  else if (animation == 22)
    BattleEffect_CallEffect22(targetId);
  else if (animation == 21)
    BattleEffect_CallEffect21(targetId);
  else if (animation == 18)
    BattleEffect_CallEffect18(targetId);
  else if (animation == 28)
    Func_080e82cc();
  else if (animation == FIELD_FX_WAVE) {
    // A wave still running from the last cast is reset first; a target that
    // is not the paused object stops walking while the wave runs.
    struct MainRoutineWork *party;

    if (*(s8 *)(event + 0xca4) != 0)
      Func_080dd950();
    party = &gMainRoutineWork;
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 1;
    Func_080dc978(scene->firstObjectId, targetId);
  } else if (animation == FIELD_FX_PAUSE) {
    // Let the object stopped last time go, then stop the target.
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->pausedObject != -1) {
      BattleEffect_ResumeObject(party->pausedObject);
      party->pausedObject = -1;
    }
    BattleEffect_PauseObject(targetId);
    party->pausedObject = targetId;
    BattleEffect_MarkChildAndRunFallbackTransition(targetId);
  }
}


void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

struct CompositeAnimation {
  u8 unk00[0x11];
  u8 displayFlags;
  u8 unk12[7];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

void Func_080debd8(void);
void FieldEvent_ShowStatusMessage(void);
void Func_080e4730(void);
void Func_080e9db4(void);
void Func_080e87c4(void);

void BattleEffect_FinishSceneAndReleaseHeapBlock(void);

/// unk26a from main_routine_work.h.
struct PartyPauseView {
  u8 unk000[0x26a];
  s16 pausedObject;
};

void BattleEffect_ClearChildValueOnMismatch(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  if (scene->fieldKind == 2) {
    struct PartyPauseView *party;

    BattleEffect_FinishSceneAndReleaseHeapBlock();
    party = (struct PartyPauseView *)&gMainRoutineWork;
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 0;
  }
}

/// Additional processing for each type.
void FieldEvent_RunTypeHandler(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  switch (scene->fieldKind) {
  case 8:
    Func_080dd950();
    break;
  case 10:
    Func_080debd8();
    break;
  case 16:
    FieldEvent_ShowStatusMessage();
    break;
  case 23:
    Func_080e4730();
    break;
  case 24:
    Func_080e9db4();
    break;
  case 28:
    Func_080e87c4();
    break;
  }
}

void Actor_ApplyRandomChildValues(struct Actor *owner) {
  s32 state;
  struct CompositeAnimation *target;
  s32 initialCount;
  s32 count;
  struct AnimationEntry **entry;
  struct AnimationEntry *current;
  u32 frame;
  s32 value;

  state = owner->animationKind;
  if (state == 1) {
    target = owner->sprite;
    if (target != 0 && (target->displayFlags & state) == 0) {
      initialCount = target->count;
      if (initialCount != 0) {
        entry = target->entries;
        frame = gFrameCount;
        count = initialCount;
        do {
          current = *entry++;
          value = frame % 6;
          count--;
          current->param = value;
        } while (count != 0);
      }
      target->dirty = 1;
    }
  }
}

void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle) {
  s32 values[3];

  if (object != 0) {
    SET_VECTOR(values, object->x, object->y, object->z);
    Vector_AddPolarOffset(magnitude, angle, values);
    Actor_SetTargetPosition(object, values[0], values[1], values[2]);
  }
}

struct DepthPart {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct DepthMapBase {
  u8 unk00[4];
  s32 base;
};

struct DepthScroll {
  s32 unk00;
  s32 offset;
};

struct DepthMap {
  struct DepthMapBase *base;
  u8 unk04[0xe0];
  struct DepthScroll scroll;
};

s32 Render_ProjectPoint(const void *point, s32 *screen);
void Func_080eb01c(void *work);

s32 Func_080db9a8(struct Actor *object, s32 kind) {
  if (kind == 0xc000)
    return ((struct DepthPart *)object->sprite)->priority;
  return 0;
}

/// Returns the 2-bit OBJ priority of the object's animation record.
s32 BattleEffect_GetObjectPriority(struct Actor *object) {
  return ((struct DepthPart *)object->sprite)->priority;
}

/// Returns the object's sort depth: from its projected z on the world map
/// (mode 3), otherwise from its y and z against the map's scroll.
s32 BattleEffect_GetObjectDepth(struct Actor *object) {
  s32 point[3];
  s32 screen[3];
  s32 depth;
  s32 *out;

  if (*((s8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    point[0] = object->x;
    out = screen;
    point[1] = object->y;
    point[2] = object->z;
    Render_ProjectPoint(point, out);
    depth = (0x200 - out[2]) / 2 + 128;
    if (depth <= 0)
      depth = 1;
  } else {
    struct DepthMap *map = (struct DepthMap *)Ram_HeapSlots->mapWork;
    struct DepthScroll *scroll = &map->scroll;
    s32 bias = scroll->offset & 0xffff0000;
    s32 base = map->base->base;

    depth = ((object->y - base + object->z - base - bias) >> 16) + 62;
  }
  return depth;
}

/// Per-frame callback: draws the effect scene's sprite (effect work +0x7c8).
void BattleEffect_DrawSceneSprite(void) {
  Func_080eb01c((u8 *)Ram_HeapSlots->effectWork + 0x7c8);
}


struct DepthSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 hidden : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 layer : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[4];
  s32 scale;
  u8 unk1c[2];
  u16 frame;
  u8 unk20[8];
};

struct DepthScene {
  u16 mode;
  u8 unk02[2];
  s32 x;
  u8 unk08[8];
  struct Actor *mainObject;
  u8 unk14[0x7b4];
  struct DepthSprite sprite;
  s16 vramSlot;
  s16 active;
};

void Func_080eaf98(struct DepthSprite *sprite, s32 a, s32 b, u32 c, s32 tiles);

void Func_080dba5c(void) {
  struct DepthScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject;
  struct DepthSprite *sprite;
  s32 position[3];
  void *buffer;
  s32 slot;
  s32 tiles;

  mainObject = scene->mainObject;
  if (scene->mode == 0xc000) {
    scene->active = 1;
    buffer = Heap_Alloc(0x200);
    // An empty once-only loop keeps the zero kept in sl after the fill's
    // arguments; without it, 12 of 103 instructions differ in tla-en.
    do {
    } while (0);
    Iwram_FillWords(buffer, 0x200, 0);
    slot = Resource_FindFreeEntry();
    tiles = VramCache_Load(slot, 0x200, buffer);
    scene->vramSlot = slot;
    Heap_Release(buffer);
    sprite = &scene->sprite;
    Func_080eaf98(sprite, 16, 31, 0x80000000, tiles);
    sprite->frame = 0;
    sprite->priority = 0;
    sprite->hidden = 0;
    sprite->layer = 0;
    sprite->scale = 0x20000;
    position[0] = scene->x;
    position[1] = mainObject->y + 0x1c0000;
    position[2] = mainObject->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Task_Add(BattleEffect_DrawSceneSprite, TASK_PASS_VBLANK);
  }
}
#undef CELL_MASK
#undef SIDE_TURN

// ---- games/tla/src/field/event_commands/depthend.c ----

void BattleEffect_DrawSceneSprite(void);

/// If the scene's sprite is active, stops its update callback
/// (Func_080dba44) and frees its VRAM cache entry.
void Func_080dbb40(void) {
  struct DepthScene *scene = Ram_HeapSlots->effectWork;

  if (scene->active != 0) {
    scene->active = 0;
    Task_Remove(BattleEffect_DrawSceneSprite);
    Resource_ResetEntry(scene->vramSlot);
  }
}

// Finds a map cell by position.


/// Returns the map cell at position x, z in the given layer, read from the
/// world map's blocks or the cell buffer by the event work's mode.
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if ((s8)event[394] == 3) {
    cells = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                               (((z / 0x200000) & 31) << 5)];
  } else {
    if (layer <= 2)
      cells = map->layers[layer].cells;
    else
      cells = (struct MapCell *)Ram_MapCellBuffer;
    cells = &cells[x / 0x100000 + (z / 0x100000) * 128];
  }
  return cells;
}

s32 GetWorldMapCollisionFar(s32 *position);
s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080201d0(s32 layer, s32 x, s32 z);

s32 Func_080dbc04(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;

  if (*(s8 *)(event + 394) == 3)
    return Func_080dbcd8(x, z, layer);
  if (Func_080dbda8(x, z, layer) != 0 || Func_080dbd5c(x, z, layer) != 0 ||
      Func_080201d0(layer, x, z) != 0)
    return 0;
  if (Func_080dbcd8(x, z, layer) != 0)
    return 1;
  if (*(s8 *)(event + 0xd36) == 0)
    return Func_080dbb78(x, z, layer)->attributeB & 1;
  return ~Func_080dbb78(x, z, layer)->attributeB & 1;
}

s32 Func_080dbca8(s32 x, s32 z) {
  return Func_080dbb78(x, z, 2)->collisionCode == 231;
}

s32 Func_080dbcc0(s32 x, s32 z) {
  return Func_080dbb78(x, z, 2)->collisionCode == 236;
}


s32 Func_080dbcd8(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;
  s32 column = x;
  if (*(s8 *)(event + 394) == 3) {
    s32 position[3];

    SET_VECTOR(position, column, 0, z);
    column = (u32)(GetWorldMapCollisionFar(position) - 5) <= 1;
  } else {
    s32 code = Func_080dbb78(column, z, 2)->collisionCode;

    code ^= 232;
    column = (u32)(-code | code) >> 31;
    column = 1 - column;
  }
  if (*(s8 *)(event + 0xd37) != 0)
    column = 1 - ((u32)(-column | column) >> 31);
  return column;
}

u8 Func_080dbd48(s32 x, s32 z, s32 layer) {
  return Func_080dbb78(x, z, layer)->attributeB & 0x40;
}

s32 Func_080dbd5c(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;

  if (*(s8 *)(event + 394) == 3) {
    s32 position[3];
    s32 collision;

    position[1] = 0;
    position[0] = x;
    position[2] = z;
    collision = GetWorldMapCollisionFar(position);
    return (u32)(collision - 2) <= 2;
  }
  return (u8)(Func_080dbb78(x, z, layer)->attributeB & 16);
}

s32 Func_080dbda8(s32 x, s32 z, s32 layer) {
  u8 code = Func_080dbb78(x, z, 2)->collisionCode;

  if (code == 233)
    return 1;
  if (code == 234)
    return 2;
  if (code == 235)
    return 3;
  return 0;
}

u8 Func_080dbdc8(s32 x, s32 z, s32 layer) {
  return Func_080dbb78(x, z, layer)->collisionCode;
}

u8 Func_080dbdd4(s32 x, s32 z, s32 layer, s32 value) {
  Func_080dbb78(x, z, layer)->collisionCode = value;
  return value;
}

s32 Func_080dbde8(s32 x, s32 z, s32 layer) {
  return Func_080dbb78(x, z, layer)->attributeB;
}

u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value) {
  Func_080dbb78(x, z, layer)->attributeB = value;
  return value;
}

u8 Func_080dbe08(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = Func_080dbde8(x, z, layer);
  u8 *cell = (u8 *)Func_080dbb78(x, z, layer);

  flags |= bits;
  cell[3] = flags;
  return flags;
}

u8 Func_080dbe44(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = Func_080dbde8(x, z, layer);
  u8 *cell = (u8 *)Func_080dbb78(x, z, layer);

  flags &= ~bits;
  cell[3] = flags;
  return flags;
}

s32 Func_080dbe80(s32 *a, s32 radius_a, s32 *b, s32 radius_b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 radius = radius_a + radius_b;

  if (dx > 0x400000 || dz > 0x400000)
    return -1;
  if (dy > radius << 16)
    return -1;
  if (dx * dx + dz * dz < radius * radius)
    return 1;
  return -1;
}

// Moves an object into place while growing it.


struct MoveScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

void Actor_SetSpritePriorityDirectFar(struct Actor *object, s32 priority);
void wait(s32 frames);

/// Moves the object from 16 pixels above the effect's main object to (x, y,
/// z), growing it from 0.25 to 1.
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z) {
  struct MoveScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject = scene->mainObject;
  s32 start[3];
  s32 i = 0;

  if (scene->mode != 0xc000)
    Actor_SetSpritePriorityDirectFar(object, 0);
  start[0] = mainObject->x;
  start[1] = mainObject->y + 0x100000;
  start[2] = mainObject->z;
  do {
    object->x = start[0] + (x - start[0]) * i / 10;
    object->y = start[1] + (y - start[1]) * i / 10;
    object->z = start[2] + (z - start[2]) * i / 10;
    object->scaleY = object->scaleX = i * (Q16_ONE * 3 / 4) / 10 + Q16_ONE / 4;
    wait(1);
    i++;
  } while (i <= 10);
}

#include "scene_tables.h"

struct MapCell;
void Actor_StartScriptFar(struct Actor *object, const s32 *script);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
u32 Random16(void);
extern const s32 gData_080f0e78[];

void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z) {
  struct Actor *mainObject =
      ((struct MoveScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 start[3];
  s32 i;

  i = 0;
  start[0] = mainObject->x;
  start[1] = mainObject->y + PIXELS(16);
  start[2] = mainObject->z;
  do {
    object->x = x + (start[0] - x) * i / 10;
    object->y = y + (start[1] - y) * i / 10;
    object->z = z + (start[2] - z) * i / 10;
    object->scaleY = object->scaleX = i * -(Q16_ONE * 3 / 4) / 10 + Q16_ONE;
    wait(1);
    i++;
  } while (i <= 10);
}

/// Spawns a particle (kind 0x2a1) at x, y, z that hops off in a random
/// direction 8 to 32 pixels away at 1 to 2 pixels a frame.
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Spawn(0x2a1, x, y, z);
  s32 magnitude;

  if (object != NULL) {
    Actor_StartScriptFar(object, gData_080f0e78);
    object->speedLimit = Random16() + Q16_ONE;
    object->acceleration = Q16_ONE;
    object->motionFlags = 2;
    object->gravity = 0xa3d;
    magnitude = Random16();
    magnitude -= Random16();
    object->velocityY = magnitude;
    Motion_SetTargetPositionFromMagnitudeAngle(
        object, Random16() * 24 + 0x80000, Random16());
  }
  return object;
}

struct ResourceSprite {
  u8 unk00[0x10];
  u8 vramBlock;
  u8 flags;
};

extern u8 gResourceBlockOwners[];

s32 ResourceTable_CountFreeBlocks(void) {
  u8 *marker = gResourceBlockOwners;
  s32 freeCount;
  s32 remaining;

  freeCount = 0;
  remaining = 0x200;
  do {
    if (*marker++ == 0xff) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

void *Sprite_ShareVramCacheEntry(void *src, void *alt) {
  void *ret;
  struct ResourceSprite *obj;

  obj = src;
  ret = NULL;
  if (obj != NULL) {
    if (alt == NULL) {
      *((u8 *)obj + 0x11) = (u8)(*((u8 *)obj + 0x11) | 1);
    } else {
      Resource_ResetEntry(obj->vramBlock);
      obj->vramBlock = ((struct ResourceSprite *)alt)->vramBlock;
      *((u8 *)obj + 0x11) = (u8)(*((u8 *)obj + 0x11) | 1);
      obj = alt;
    }
    ret = obj;
  }
  return ret;
}


#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)


struct SpawnScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
};

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct SpawnScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *object;
  u8 *part;
  s32 mask;

  object = Func_080200c0(kind, x, y, z);
  if (object != NULL) {
    if (object->animationKind == 0) {
      Actor_Destroy(object);
      return NULL;
    }
    object->terrainHeight = scene->mainObject->terrainHeight;
    object->motionFlags = 4;
    object->priorityFlags = 4;
    mask = ~0xc;
    part = object->sprite;
    part[9] &= mask;
    Actor_SetSingleChildFlagsFar(object, 0);
    Actor_SetMode(object, 1);
  }
  return object;
}

void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action) {
  s16 *activeObjectId;
  struct Actor *object;
  s32 objectId;

  objectId = 0;
  activeObjectId = (s16 *)gPlayerObjectId;
  do {
    object = ObjectTable_Get(objectId);
    if (objectId != *activeObjectId && object != NULL &&
        object != excludedObject) {
      object->movementState = groupMode;
      Actor_SetAnimationSpeedFar(object, action);
    }
    objectId++;
  } while (objectId <= 0x50);
}


void GameFlag_ClearBit(s32 flag);

void GameFlag_SetBit(s32 flag);

void Field_StartColorTransition(s32 frames, s32 color) {
  u8 *state = Ram_HeapSlots->eventWork;
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  Dma_Set(buffers->target + 0x2a0, state + 0x762, DMA_COPY_WORDS(0x150),
          REG_DMA3);
  if (*(s8 *)(state + 0xca4) == 0)
    Dma_Set(buffers->target, state + 0x222, DMA_COPY_WORDS(0x150), REG_DMA3);
  Dma_Set(buffers->target, buffers->current, DMA_COPY_WORDS(0x2a0), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x200000 | color, 1);
  GameFlag_SetBit(0x108);
  BattleEffect_StartBufferInterpolation(frames);
}

void Field_StartPaletteTransition(s32 frames) {
  u8 *state = Ram_HeapSlots->eventWork;
  s32 colors = (s32)(state + 0x222);

  BattleEffect_ApplyColorToTargetBuffer(colors, 2);
  if (*(s8 *)(state + 0xca4) != 0)
    BattleEffect_ApplyColorToTargetBuffer(0x10001, 1);
  else
    BattleEffect_ApplyColorToTargetBuffer(colors, 1);
  GameFlag_ClearBit(0x108);
  BattleEffect_StartBufferInterpolation(frames);
}


s32 GameFlag_Test(s32 flag);

/// The eight tints a field Psynergy washes the screen in (BGR555, fxdata.c).
extern const u32 gData_080f3954[8];

/// Flags 0x148-0x14f each force one tint, the last set one winning.
#define FLAG_TINT_FIRST 0x148

/// Tints the field for a Psynergy animation: a tint chosen by the frame
/// counter (so each cast looks a little different), fixed for some
/// animations, and forced by the tint flags; then fades towards it over eight
/// frames. The palettes are copied by Field_StartColorTransition.
// ☀️'s function of the same name also copies the palettes.
void BattleEffect_InitializeSharedScene(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  s32 tint;
  s32 animation;
  s32 raw;

  tint = gFrameCount & 7;
  raw = *(u16 *)&scene->fieldKind;
  /* FAKEMATCH(barrier): raw passed through an empty asm, which keeps the
   * animation's signed read (ldrsh) apart from raw's unsigned one (ldrh);
   * without it, 7 of 102 instructions differ in tla-en. */
  asm("" : "+r"(raw));
  animation = scene->fieldKind;
  if (animation == 18)
    tint = 0;
  if (animation == 26)
    tint = 2;
  if (animation == 23)
    tint = 7;
  if (animation == 25)
    tint = 0;
  if (animation == 20)
    tint = 5;
  if (animation == 29)
    tint = 0;
  if (animation == 22)
    tint = 7;
  if (animation == 19)
    tint = 6;
  if (animation == 27)
    tint = 0;
  if (animation == 21)
    tint = 7;
  if ((s16)raw == 30)
    tint = 6;

  if (GameFlag_Test(FLAG_TINT_FIRST + 0))
    tint = 0;
  if (GameFlag_Test(FLAG_TINT_FIRST + 1))
    tint = 1;
  if (GameFlag_Test(FLAG_TINT_FIRST + 2))
    tint = 2;
  if (GameFlag_Test(FLAG_TINT_FIRST + 3))
    tint = 3;
  if (GameFlag_Test(FLAG_TINT_FIRST + 4))
    tint = 4;
  if (GameFlag_Test(FLAG_TINT_FIRST + 5))
    tint = 5;
  if (GameFlag_Test(FLAG_TINT_FIRST + 6))
    tint = 6;
  if (GameFlag_Test(FLAG_TINT_FIRST + 7))
    tint = 7;
  Field_StartColorTransition(8, gData_080f3954[tint]);
}
#undef Actor_SetAnimationSpeedFar
#undef FLAG_TINT_FIRST
