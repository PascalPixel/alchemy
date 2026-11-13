/// @file
/// Messages shown at an actor, and the speaker's voice.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "gamepak/far_call/window.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "scene_call.h"

s32 Portrait_OpenWindowFar(s32 sprite, s32 zero, s32 x, s32 y);

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
  sprite = Actor_GetSpriteById(localId);
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
        if (Actor_GetScreenPosition(localId, pos) != -1)
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
        if (Actor_GetScreenPosition(Party_GetLeaderActor(), pos) != -1)
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
      portrait = Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    else
      portrait = Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    while (!Message_AllTypedFar())
      wait(1);
  }
  work->objectStatus = handle;
  *(s32 *)((u8 *)work + 0x1e8) = portrait;
  work->messageId++;
  return handle;
}

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
  id = Actor_GetSpriteById(object);
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

  Message_SetVoiceFromActor(0x80000000);
  wait(1);
}

void Event_ShowMessageThenWait(s32 object, s32 message, s32 frames) {
  Func_080d3fb0(object, message);
  Event_Wait(frames);
}

void Event_ShowMessageAndWait(s32 object, s32 message) {
  Func_080d3fb0(object, message);
}

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
void Event_ShowSpeakerMessage(s32 speaker) {
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

void Event_ShowCounter(s32 a, s32 b, s32 c, s32 d) {
  Event_ShowMessageAtPosition(a, b, c, d);
}

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
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
void Actor_ClosePortrait(s32 id) {
  s32 no;

  no = Actor_GetSpriteById(id);
  if (no != -1)
    Window_CloseMatchingIdFar(no);
}

/// The event command "show two messages and wait": two actors speak at once,
/// each message at its own (x, y) with its portrait at (portraitX,
/// portraitY), all in tiles. When delay is nonzero, the second message waits
/// for the first to finish, checking every delay frames. Waits for A, B, L or
/// R, then closes both.
// As ☀️'s function of the same name, which has no delay.
void Event_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                  s32 firstPortraitX, s32 firstPortraitY,
                                  s32 second, s32 secondX, s32 secondY,
                                  s32 secondPortraitX, s32 secondPortraitY,
                                  s32 delay) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 firstSprite = Actor_GetSpriteById(first);
  s32 secondSprite = Actor_GetSpriteById(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = Message_OpenWindowFar((s16)event->messageId++, firstX, firstY,
                                      Speaker_GetVoice(firstSprite) << 16);
  Portrait_OpenWindowFar(firstSprite, 0, firstPortraitX, firstPortraitY);
  if (delay != 0) {
    while (!Message_AllTypedFar())
      wait(delay);
  }

  secondWindow =
      Message_OpenWindowFar((s16)event->messageId++, secondX, secondY,
                            Speaker_GetVoice(secondSprite) << 16);
  Portrait_OpenWindowFar(secondSprite, 0, secondPortraitX, secondPortraitY);

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

extern const u8 gMessageSpeed_VoiceRates[];

void Message_SetVoiceFromActor(s32 key) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u8 value;
  s32 id;

  if (key == 0x80000000) {
    *(s16 *)&work->speakerVoiceSound = (value = 0);
    *(s16 *)&work->speakerVoiceCountdown = value;
  } else {
    id = Speaker_GetVoice(Actor_GetSpriteById(key));
    value = gMessageSpeed_VoiceRates[gMainRoutineWork.eventMode];
    *(s16 *)&work->speakerVoiceSound = id;
    *(s16 *)&work->speakerVoiceCountdown = value;
  }
}

void FieldCamera_FollowActor(s32 id, s32 flag) {
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
