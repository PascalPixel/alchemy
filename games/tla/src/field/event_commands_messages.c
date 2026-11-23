/// @file
/// Messages shown at an actor, and the speaker's voice.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "gamepak/far_call/window.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "field/scene_call.h"
#include "field/speaker.h"
#include "macros.h"

/// Message_SetVoiceFromActor's key for no voice: it clears the voice.
#define MESSAGE_VOICE_NONE 0x80000000

/// The event command "open message": opens the event's current message in a
/// speech window next to the speaking actor, with the speaker's portrait beside
/// it when it has one, and moves on to the next message.
s32 FieldMessage_ShowAtActor(s32 speaker, s32 unused) {
  struct WindowWork *win;
  struct FieldWork *work;
  s32 sprite;
  s32 extra;
  s32 portraitTop;
  s32 margin;
  s32 height;
  s32 width;
  s32 top;
  s32 left;
  s32 pos[3];
  struct Window *handle;
  s32 portrait;
  s32 flags;
  s32 message;
  struct Actor *actor;
  s32 x;
  s32 y;
  s32 found;
  s32 column;

  win = (struct WindowWork *)Ram_HeapSlots->windowWork;
  work = gFieldWork;
  handle = 0;
  portrait = 0;
  sprite = Actor_GetSpriteById(speaker);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = speaker & SPEAKER_FLAGS;
  message = work->message;
  speaker &= SPEAKER_ACTOR_MASK;
  x = 0;
  actor = Actor_Lookup(speaker);
  work->speaker = speaker;
  y = 0;
  found = 0;
  if (work->fastForward == 0) {
    if (actor != 0) {
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
        Matrix_ProjectPoint(&actor->x.fixed, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
        y -= 2;
      } else {
        if (Actor_GetScreenPosition(speaker, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (speaker <= ACTOR_PIERS) {
      sprite = speaker;
      actor = Actor_Lookup(Party_GetLeaderActor());
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
        Matrix_ProjectPoint(&actor->x.fixed, pos);
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
      left = DISPLAY_TILE_WIDTH / 2;
      top = DISPLAY_TILE_HEIGHT / 2;
    } else {
      left = 0;
      top = 0;
      Text_GetMessageSizeAllPagesFar(message, &left, &top, &width, &height);
      left = x - (s32)width / 2;
      if (flags & SPEAKER_WINDOW_ABOVE)
        top = y - height - 1;
      else if (!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    // Place the portrait beside the speaker, on the side with more room.
    if (win->mindRead != 0)
      margin = 5;
    column = x;
    if (flags & SPEAKER_PORTRAIT_LEFT) {
      column = x - margin - 2;
      if (column < 0)
        column = 0;
    } else if (flags & SPEAKER_PORTRAIT_RIGHT) {
      column += 2;
      if (column + margin > DISPLAY_TILE_WIDTH - 1)
        column = DISPLAY_TILE_WIDTH - 1 - margin;
    } else if (column <= DISPLAY_TILE_WIDTH / 2) {
      column = column - margin - 2;
      if (column < 0)
        column = x + 2;
    } else {
      column += 2;
      if (column + margin > DISPLAY_TILE_WIDTH - 1)
        column = x - margin - 2;
    }

    // A speaker with a portrait: put the portrait just above or below the
    // message, within the screen's rows, and grow the message to the
    // portrait's height.
    found = Portrait_GetForSpeakerFar(sprite);
    if (found != -1) {
      Text_GetMessageSizeAllPagesFar(message, &left, &top, &width, &height);
      message = -1;
      if (top > y)
        portraitTop = top - PORTRAIT_WINDOW_TILES;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + PORTRAIT_WINDOW_TILES > DISPLAY_TILE_HEIGHT - 1)
        portraitTop = top - PORTRAIT_WINDOW_TILES;
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
    else if (column + margin > DISPLAY_TILE_WIDTH - 1)
      column = DISPLAY_TILE_WIDTH - 1 - margin;

    // Open the message and the portrait, and wait until the text is typed out.
    if (win->mindRead != 0) {
      wait(8);
      if (extra != 0)
        handle = Message_OpenWindowFar(message, left, top + extra - 1,
                                       MESSAGE_WINDOW_ANCHOR_BOTTOM |
                                           MESSAGE_WINDOW_UNREAD);
      else
        handle =
            Message_OpenWindowFar(message, left, top, MESSAGE_WINDOW_UNREAD);
    } else {
      s32 voice = Speaker_GetVoice(sprite);

      if (extra != 0)
        handle = Message_OpenWindowFar(message, left, top + extra - 1,
                                       MESSAGE_WINDOW_VOICE(voice) |
                                           MESSAGE_WINDOW_ANCHOR_BOTTOM |
                                           MESSAGE_WINDOW_ZOOM);
      else
        handle = Message_OpenWindowFar(message, left, top,
                                       MESSAGE_WINDOW_VOICE(voice) |
                                           MESSAGE_WINDOW_ZOOM);
    }
    if (win->mindRead != 0)
      portrait = (s32)Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    else
      portrait = (s32)Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    while (!Message_AllTypedFar())
      wait(1);
  }
  work->messageWindow = (s32)handle;
  work->sideWindow = (struct Window *)portrait;
  work->message++;
  return handle;
}

/// Opens the event's current message at the speaker and waits for its window;
/// after ten seconds, or with Select, R, L and A held, the text is finished at
/// once.
void FieldMessage_ShowAtActorAndWait(s32 speaker, s32 flags) {
  s32 masked;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  void *window = FieldMessage_ShowAtActor(speaker, flags);
  s32 id;
  u32 frames;

  frames = 0;
  wait(1);
  id = Actor_GetSpriteById(speaker);
  if (speaker <= ACTOR_PIERS) {
    masked = speaker & SPEAKER_ACTOR_MASK;
    if (Scene_FindActorPlacement(masked) == 0)
      id = masked;
  }
  Portrait_CloseWindowFar(id);

  if (work->fastForward == 0) {
    while (Window_IsClosedFar((s32)window) == 0) {
      wait(1);
      frames++;
      if (frames > 600 ||
          ((gInput.held & KEY_SELECT) && (gInput.held & KEY_R) &&
           (gInput.held & KEY_L) && (gInput.held & KEY_A)))
        Message_CloseFinishedWindowsFar();
    }
  }

  Message_SetVoiceFromActor(MESSAGE_VOICE_NONE);
  wait(1);
}

/// The event command "show message and wait": shows the message, then waits the
/// given number of frames.
void Event_ShowMessageThenWait(s32 speaker, s32 flags, s32 frames) {
  FieldMessage_ShowAtActorAndWait(speaker, flags);
  Event_Wait(frames);
}

/// The event command "show message": opens the current message at the speaker
/// (FieldMessage_ShowAtActor) and waits until it has been read and its window
/// has closed; the speaker's portrait is closed once the text is typed out.
void Event_ShowMessageAndWait(s32 speaker, s32 flags) {
  FieldMessage_ShowAtActorAndWait(speaker, flags);
}

/// The event command "ask yes or no": opens the question at the speaker, lets
/// the leader answer, then shows the next message for yes or the one after it
/// for no.
s32 Event_AskYesNoAndShowResponse(s32 speaker, s32 flags) {
  s32 result;

  FieldMessage_ShowAtActor(speaker, flags);
  result = Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0);
  if (result == 0) {
    FieldMessage_ShowAtActorAndWait(speaker, flags);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    FieldMessage_ShowAtActorAndWait(speaker, flags);
  }
  return result;
}

/// An empty hook; it does nothing in the released game.
void Event_NoOpUpdate(void) {}

/// Event_ShowSpeakerMessage and Event_ShowMessageAtPosition move a position
/// MESSAGE_POSITION_SHIFT away from the middle row of a MESSAGE_POSITION_WIDTH
/// by MESSAGE_POSITION_HEIGHT area, larger than the 240 by 160 screen, and hold
/// it MESSAGE_POSITION_MARGIN_X and MESSAGE_POSITION_MARGIN_Y inside the area.
#define MESSAGE_POSITION_WIDTH 320
#define MESSAGE_POSITION_HEIGHT 240
#define MESSAGE_POSITION_MARGIN_X 8
#define MESSAGE_POSITION_MARGIN_Y 20
#define MESSAGE_POSITION_SHIFT 32

/// Unused: nothing calls it. It would record the speaker, open the message near
/// it and wait unless fast-forwarding, then advance the message index. It
/// places the message as Event_ShowMessageAtPosition does, before passing the
/// position to a window routine that takes tiles; with no caller, the units it
/// expected can't be told.
void Event_ShowSpeakerMessage(s32 speaker) {
  // BUG: the lookup's result is discarded, and pos is left unset.
  struct {
    s32 x, y;
  } pos;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  speaker &= SPEAKER_ACTOR_MASK;
  Actor_Lookup(speaker);
  work->speaker = speaker;
  if (work->fastForward == 0) {
    s32 windowX = pos.x;
    s32 windowY = pos.y;
    s32 window;

    if (windowY > MESSAGE_POSITION_HEIGHT / 2 - 1)
      windowY += MESSAGE_POSITION_SHIFT;
    else
      windowY -= MESSAGE_POSITION_SHIFT;
    windowX = windowX < MESSAGE_POSITION_MARGIN_X ? MESSAGE_POSITION_MARGIN_X
                                                  : windowX;
    if (windowX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
      windowX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
    windowY = windowY < MESSAGE_POSITION_MARGIN_Y ? MESSAGE_POSITION_MARGIN_Y
                                                  : windowY;
    if (windowY > MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y)
      windowY = MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y;
    window = Message_OpenWindowFar((s16)work->message, windowX, windowY,
                                   MESSAGE_WINDOW_ZOOM);
    work->messageWindow = window;
    while (Window_IsClosedFar(window) == 0)
      wait(1);
  }
  work->message++;
}

/// Unused: nothing calls it; forwards its arguments to
/// Event_ShowMessageAtPosition.
void Event_ShowCounter(s32 a, s32 b, s32 c, s32 d) {
  Event_ShowMessageAtPosition(a, b, c, d);
}

/// Unused: nothing calls it. It would open the event's message at (x, y), wait
/// for it and advance the message index; its units are as unknown as
/// Event_ShowSpeakerMessage's.
void Event_ShowMessageAtPosition(s32 unusedA, s32 unusedB, s32 x, s32 y) {
  struct FieldWork *state = gFieldWork;
  s32 windowY = y;
  s32 windowX = x;
  s32 minX = MESSAGE_POSITION_MARGIN_X;
  s32 minY = MESSAGE_POSITION_MARGIN_Y;
  s32 window;

  if (windowY > MESSAGE_POSITION_HEIGHT / 2 - 1)
    windowY += MESSAGE_POSITION_SHIFT;
  else
    windowY -= MESSAGE_POSITION_SHIFT;

  if (x < minX)
    windowX = minX;
  if (windowX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
    windowX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
  CLAMP(windowY, minY, MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y);

  window = (s32)Message_OpenWindowFar(state->message, windowX, windowY,
                                      MESSAGE_WINDOW_ZOOM);
  while (Window_IsClosedFar((struct Window *)window) == 0)
    wait(1);
  state->message++;
}

/// Closes the portrait of an actor's sprite, if it has one.
void Actor_ClosePortrait(s32 actorId) {
  s32 sprite;

  sprite = Actor_GetSpriteById(actorId);
  if (sprite != -1)
    Portrait_CloseWindowFar(sprite);
}

/// The event command "show two messages and wait": two actors speak at once,
/// each message at its own (x, y) with its portrait at (portraitX,
/// portraitY), all in tiles. When delay is nonzero, the second message waits
/// for the first to finish, checking every delay frames. Waits for A, B, L or
/// R, then closes both.
void Event_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                  s32 firstPortraitX, s32 firstPortraitY,
                                  s32 second, s32 secondX, s32 secondY,
                                  s32 secondPortraitX, s32 secondPortraitY,
                                  s32 delay) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 firstSprite = Actor_GetSpriteById(first);
  s32 secondSprite = Actor_GetSpriteById(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = Message_OpenWindowFar(
      (s16)event->message++, firstX, firstY,
      MESSAGE_WINDOW_VOICE(Speaker_GetVoice(firstSprite)));
  Portrait_OpenWindowFar(firstSprite, 0, firstPortraitX, firstPortraitY);
  if (delay != 0) {
    while (!Message_AllTypedFar())
      wait(delay);
  }

  secondWindow = Message_OpenWindowFar(
      (s16)event->message++, secondX, secondY,
      MESSAGE_WINDOW_VOICE(Speaker_GetVoice(secondSprite)));
  Portrait_OpenWindowFar(secondSprite, 0, secondPortraitX, secondPortraitY);

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait(1);
  while ((gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
    wait(1);

  wait(1);
  Portrait_CloseWindowFar(firstSprite);
  Portrait_CloseWindowFar(secondSprite);
  Message_CloseFinishedWindowsFar();
  wait(1);

  while (Window_IsClosedFar((struct Window *)firstWindow) == 0)
    wait(1);
  while (Window_IsClosedFar((struct Window *)secondWindow) == 0)
    wait(1);

  wait(1);
}

extern const u8 gMessageSpeed_VoiceRates[];

/// Sets the voice of the messages that follow to that of actor key's sprite, at
/// the rate the message speed option picks (gMessageSpeed_VoiceRates);
/// MESSAGE_VOICE_NONE clears it.
void Message_SetVoiceFromActor(s32 key) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  u8 value;
  s32 id;

  if (key == MESSAGE_VOICE_NONE) {
    *(s16 *)&work->speakerVoiceSound = (value = 0);
    *(s16 *)&work->speakerVoiceCountdown = value;
  } else {
    id = Speaker_GetVoice(Actor_GetSpriteById(key));
    value = gMessageSpeed_VoiceRates[gMainRoutineWork.messageSpeed];
    *(s16 *)&work->speakerVoiceSound = id;
    *(s16 *)&work->speakerVoiceCountdown = value;
  }
}

/// The event command "camera follow actor": the camera follows the actor, put
/// on it at once unless keepPosition leaves it where it is to set out from.
void FieldCamera_FollowActor(s32 actorId, s32 keepPosition) {
  struct Actor *actor;
  struct Actor *target;
  struct FieldWork *work;
  struct MapWork *map;

  actor = Actor_Lookup(actorId);
  work = Ram_HeapSlots->fieldWork;
  target = work->camera;
  map = Ram_HeapSlots->mapGrid;
  if (actor != 0) {
    map->origin = &target->x.fixed;
    Actor_StartCameraFar(target, actor);
    if (keepPosition == 0) {
      SET_ACTOR_POSITION(target, actor->x.fixed, actor->y.fixed,
                         actor->z.fixed);
      wait(1);
      if (work->mode != ACTOR_SYSTEM_WORLD_MAP)
        MapGrid_CenterOnFollowedPointFar();
    }
  }
}

/// The event command "camera set speed": the camera's top speed and
/// acceleration, 16.16 pixels per frame.
void Camera_SetSpeedParameters(FIXED speed, FIXED acceleration) {
  struct FieldWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));

  work->camera->speedLimit = speed;
  work->camera->acceleration = acceleration;
}
#undef MESSAGE_VOICE_NONE
#undef MESSAGE_POSITION_WIDTH
#undef MESSAGE_POSITION_HEIGHT
#undef MESSAGE_POSITION_MARGIN_X
#undef MESSAGE_POSITION_MARGIN_Y
#undef MESSAGE_POSITION_SHIFT
