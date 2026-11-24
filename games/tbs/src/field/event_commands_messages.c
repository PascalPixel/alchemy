/// @file
/// Messages shown at an actor, and the speaker's voice.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Message_SetVoiceFromActor's actor id for no voice: it clears the voice.
#define MESSAGE_VOICE_NONE 0x80000000

/// Portrait_OpenWindow's window is 5 tiles square.
#define PORTRAIT_WINDOW_TILES 5

/// Opens the event's current message in a speech window next to the speaking
/// actor, with the speaker's portrait beside
/// it when it has one, and moves on to the next message.
struct Window *FieldMessage_ShowAtActor(s32 actorId, s32 unused) {
  struct WindowState *windowState;
  struct FieldState *state;
  s32 sprite;
  s32 extra;
  s32 portraitTop;
  s32 margin;
  s32 height;
  s32 width;
  s32 top;
  s32 left;
  s32 pos[3];
  struct Window *window;
  s32 portrait;
  s32 flags;
  s32 message;
  struct Actor *actor;
  s32 x;
  s32 y;
  s32 found;
  s32 column;
#if !TBS_PATCH_1
  s32 portraitId;
#endif

  windowState = WINDOW_STATE;
  state = gFieldState;
  window = 0;
  portrait = 0;
  sprite = Actor_GetSpriteById(actorId);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = actorId & SPEAKER_FLAGS;
  message = state->message;
  actorId &= SPEAKER_ACTOR_MASK;
  x = 0;
  actor = Actor_Lookup(actorId);
  state->speaker = actorId;
  y = 0;
  found = 0;
  if (state->fastForward == 0) {
    // Find the speaker on screen, in 8-pixel tiles.
    if (actor != 0) {
      if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
        Matrix_ProjectPoint(&actor->x.fixed, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
        y -= 2;
      } else {
        if (Actor_GetScreenPosition(actorId, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (actorId < ACTOR_FIRST_PLACED) {
      sprite = actorId;
      actor = Actor_Lookup(gMainRoutineState.leaderActorId);
      if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
        Matrix_ProjectPoint(&actor->x.fixed, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (Actor_GetScreenPosition(gMainRoutineState.leaderActorId, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    }

    // Place the message above or below the speaker, or mid-screen when the
    // speaker is not on screen.
    if (!found) {
      left = DISPLAY_TILE_WIDTH / 2;
      top = DISPLAY_TILE_HEIGHT / 2;
    } else {
      left = 0;
      top = 0;
      Text_GetMessageSizeAllPagesFar(message, &left, &top, (u32 *)&width,
                                     (u32 *)&height);
      left = x - width / 2;
      if (flags & SPEAKER_WINDOW_ABOVE)
        top = y - height - 1;
      else if (!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    // Place the portrait beside the speaker, on the side with more room.
    if (windowState->mindRead != 0)
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
#if TBS_PATCH_1
    found = Portrait_GetForSpeakerFar(sprite);
    if (found != -1) {
      Text_GetMessageSizeAllPagesFar(message, &left, &top, (u32 *)&width,
                                     (u32 *)&height);
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

        Text_GetMessageSizeFar(message, &left, &top, (u32 *)&width,
                               (u32 *)&height);
        message = -1;
        extra = lines - height + 1;
      }
    } else if (top < y) {
      s32 lines = height;

      Text_GetMessageSizeFar(message, &left, &top, (u32 *)&width,
                             (u32 *)&height);
      extra = lines - height + 1;
      message = found;
    }
#else
    portraitId = Portrait_GetForSpeakerFar(sprite);
    found = -1;
    if (portraitId != found) {
      Text_GetMessageSizeAllPagesFar(message, &left, &top, (u32 *)&width,
                                     (u32 *)&height);
      message = found;
      if (top > y)
        portraitTop = top - PORTRAIT_WINDOW_TILES;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + PORTRAIT_WINDOW_TILES > DISPLAY_TILE_HEIGHT - 1)
        portraitTop = top - PORTRAIT_WINDOW_TILES;
    }
    if (top < y) {
      s32 lines = height;

      Text_GetMessageSizeFar(message, &left, &top, (u32 *)&width,
                             (u32 *)&height);
      message = -1;
      extra = lines - height + 1;
    }
#endif
    if (column < 0)
      column = 0;
    else if (column + margin > DISPLAY_TILE_WIDTH - 1)
      column = DISPLAY_TILE_WIDTH - 1 - margin;

    // Open the message and the portrait, and wait until the text is typed out.
    if (windowState->mindRead != 0) {
      wait(8);
      if (extra != 0)
        window = Message_OpenWindowFar(message, left, top + extra - 1,
                                       MESSAGE_WINDOW_ANCHOR_BOTTOM |
                                           MESSAGE_WINDOW_UNREAD);
      else
        window =
            Message_OpenWindowFar(message, left, top, MESSAGE_WINDOW_UNREAD);
    } else {
      s32 voice = Speaker_GetVoice(sprite);

      if (extra != 0)
        window = Message_OpenWindowFar(message, left, top + extra - 1,
                                       MESSAGE_WINDOW_VOICE(voice) |
                                           MESSAGE_WINDOW_ANCHOR_BOTTOM |
                                           MESSAGE_WINDOW_ZOOM);
      else
        window = Message_OpenWindowFar(message, left, top,
                                       MESSAGE_WINDOW_VOICE(voice) |
                                           MESSAGE_WINDOW_ZOOM);
    }
    // DEAD: both arms open the same portrait behind a test of
    // windowState->mindRead; the ROM keeps the test's load of mindRead.
    if (windowState->mindRead != 0)
      portrait = (s32)Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    else
      portrait = (s32)Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    while (!Message_AllTypedFar())
      wait(1);
  }
  state->messageWindow = (s32)window;
  state->sideWindow = portrait;
  state->message++;
  return window;
}

/// Opens the current message at actorId (FieldMessage_ShowAtActor) and waits
/// until it has been read and its window has closed; the speaker's portrait is
/// closed once the text is typed out.
void Event_ShowMessageAndWait(s32 actorId, s32 flags) {
  s32 member;
  struct FieldState *fieldState = gFieldState;
  struct Window *window = FieldMessage_ShowAtActor(actorId, flags);
  s32 sprite;
  u32 frames;

  frames = 0;
  wait(1);
  // A party member the scene does not place has its portrait under its
  // character id, as FieldMessage_ShowAtActor opened it.
  sprite = Actor_GetSpriteById(actorId);
  if (actorId < ACTOR_FIRST_PLACED) {
    member = actorId & SPEAKER_ACTOR_MASK;

    if (Scene_FindActorPlacement(member) == 0) {
      sprite = member;
    }
  }
  Portrait_CloseWindowFar(sprite);

  if (fieldState->fastForward == 0) {
    while (Window_IsClosedFar(window) == 0) {
      wait(1);
      frames++;
      // After 600 frames, or at once with Select, L, R and A held, the windows
      // whose text is typed out close.
      if (frames > 600 || ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                           (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
        Message_CloseFinishedWindowsFar();
      }
    }
  }

  wait(1);
}

/// Shows the message, then waits the given number of frames.
void Event_ShowMessageThenWait(s32 actorId, s32 flags, s32 frames) {
  Event_ShowMessageAndWait(actorId, flags);
  Event_Wait(frames);
}

/// Opens the question at actorId, lets the leader answer, then shows the next
/// message for yes or the one after it for no.
s32 Event_AskYesNoAndShowResponse(s32 actorId, s32 flags) {
  s32 answer;
  struct FieldState *fieldState;
  struct MainRoutineState *mainState;

  FieldMessage_ShowAtActor(actorId, flags);
  mainState = &gMainRoutineState;
  answer = Event_AskYesNoWithActorAnimation(mainState->leaderActorId, 0);
  if (answer == 0) {
    Event_ShowMessageAndWait(actorId, flags);
    fieldState = gFieldState;
    fieldState->message += 1;
  } else {
    fieldState = gFieldState;
    fieldState->message += 1;
    Event_ShowMessageAndWait(actorId, flags);
  }
  return answer;
}

void Event_EmptyCallback(void) {}

/// Event_ShowSpeakerMessage and Event_ShowMessageAtPosition move a position
/// MESSAGE_POSITION_SHIFT away from the middle row of a MESSAGE_POSITION_WIDTH
/// by MESSAGE_POSITION_HEIGHT area, larger than the 240 by 160 screen, and hold
/// it MESSAGE_POSITION_MARGIN_X and MESSAGE_POSITION_MARGIN_Y inside the area.
#define MESSAGE_POSITION_WIDTH 320
#define MESSAGE_POSITION_HEIGHT 240
#define MESSAGE_POSITION_MARGIN_X 8
#define MESSAGE_POSITION_MARGIN_Y 20
#define MESSAGE_POSITION_SHIFT 32

/// Unused: reached only through a bare veneer in
/// gamepak/far_call/field_sequence.s, which has no symbol, so no code can call
/// it. It would record the speaker, open the message near it and wait unless
/// fast-forwarding, then advance the message index. It places the message as
/// Event_ShowMessageAtPosition does, before passing the position to a window
/// routine that takes tiles; with no caller, the units it expected can't be
/// told.
void Event_ShowSpeakerMessage(s32 speaker) {
  // BUG: the lookup's result is discarded, and pos is left unset.
#ifdef UBFIX
  s32 pos[3];
  struct Actor *actor;
#else
  struct {
    s32 x, y;
  } pos;
#endif
  struct FieldState *fieldState = gFieldState;

  speaker &= SPEAKER_ACTOR_MASK;
#ifdef UBFIX
  // Only the actor slots hold actor pointers;
  // Actor_Lookup's legacy range also exposes the event fields after
  // them.
  if ((u32)speaker < sizeof(fieldState->actors) / sizeof(fieldState->actors[0]))
    actor = Actor_Lookup(speaker);
  else
    actor = NULL;
#else
  Actor_Lookup(speaker);
#endif
  fieldState->speaker = speaker;
  if (fieldState->fastForward == 0) {
#ifdef UBFIX
    s32 positionFound = 0;

    // Supply the actual speaker position as FieldMessage_ShowAtActor does.
    if (actor != NULL) {
      if (fieldState->mode == ACTOR_SYSTEM_WORLD_MAP)
        positionFound =
            Matrix_ProjectPoint((const u8 *)actor + offsetof(struct Actor, x),
                                pos) != 0;
      else
        positionFound = Actor_GetScreenPosition(speaker, pos) != -1;
    }
    if (positionFound) {
      s32 windowX = pos[0];
      s32 windowY = pos[1];
      s32 win;
#else
    s32 windowX = pos.x;
    s32 windowY = pos.y;
    s32 win;
#endif

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
      win = (s32)Message_OpenWindowFar(fieldState->message, windowX, windowY,
                                       MESSAGE_WINDOW_ZOOM);
      fieldState->messageWindow = win;
      while (Window_IsClosedFar((struct Window *)win) == 0)
        wait(1);
#ifdef UBFIX
    }
#endif
  }
  fieldState->message++;
}

/// Unused: reached only through a bare veneer in field_sequence.s; forwards its
/// arguments to Event_ShowMessageAtPosition.
void Event_ForwardShowMessageAtPosition(s32 unusedA, s32 unusedB, s32 x,
                                        s32 y) {
  Event_ShowMessageAtPosition(unusedA, unusedB, x, y);
}

/// Unused: reached only through a bare veneer in field_sequence.s. It would
/// open the event's message at (x, y), moved and held as
/// MESSAGE_POSITION_WIDTH says, wait for it and advance the message index; its
/// units are as unknown as Event_ShowSpeakerMessage's.
void Event_ShowMessageAtPosition(s32 unusedA, s32 unusedB, s32 x, s32 y) {
  struct FieldState *fieldState = gFieldState;
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

  window = (s32)Message_OpenWindowFar(fieldState->message, windowX, windowY,
                                      MESSAGE_WINDOW_ZOOM);
  while (Window_IsClosedFar((struct Window *)window) == 0)
    wait(1);
  fieldState->message++;
}

/// Closes the portrait of an actor's sprite, if it has one.
void Actor_ClosePortrait(s32 actorId) {
  s32 sprite;

  sprite = Actor_GetSpriteById(actorId);
  if (sprite != -1)
    Portrait_CloseWindowFar(sprite);
}

/// Two actors speak at once, each message at its own (x, y) with its portrait
/// at (portraitX, portraitY), all in tiles. Waits for A, B, L or R, then closes
/// both.
void Event_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                  s32 firstPortraitX, s32 firstPortraitY,
                                  s32 second, s32 secondX, s32 secondY,
                                  s32 secondPortraitX, s32 secondPortraitY) {
  struct FieldState *state = gFieldState;
  s32 firstSprite = Actor_GetSpriteById(first);
  s32 secondSprite = Actor_GetSpriteById(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = (s32)Message_OpenWindowFar(
      state->message++, firstX, firstY,
      MESSAGE_WINDOW_VOICE(Speaker_GetVoice(firstSprite)));
  Portrait_OpenWindowFar(firstSprite, 0, firstPortraitX, firstPortraitY);

  secondWindow = (s32)Message_OpenWindowFar(
      state->message++, secondX, secondY,
      MESSAGE_WINDOW_VOICE(Speaker_GetVoice(secondSprite)));
  Portrait_OpenWindowFar(secondSprite, 0, secondPortraitX, secondPortraitY);

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait(1);
  while ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
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

/// Sets the voice of the messages that follow to that of actorId's sprite, at
/// the rate the message speed option picks (gMessageSpeed_VoiceRates);
/// MESSAGE_VOICE_NONE clears it.
void Message_SetVoiceFromActor(s32 actorId) {
  struct WindowState *windowState = WINDOW_STATE;
  u8 rate;
  s32 voice;

  if (actorId == MESSAGE_VOICE_NONE) {
    windowState->voiceSound = 0;
    windowState->voiceCountdown = 0;
  } else {
    voice = Speaker_GetVoice(Actor_GetSpriteById(actorId));
    rate = gMessageSpeed_VoiceRates[gMainRoutineState.messageSpeed];
    windowState->voiceSound = voice;
    windowState->voiceCountdown = rate;
  }
}

#undef MESSAGE_VOICE_NONE
#undef PORTRAIT_WINDOW_TILES
#undef MESSAGE_POSITION_WIDTH
#undef MESSAGE_POSITION_HEIGHT
#undef MESSAGE_POSITION_MARGIN_X
#undef MESSAGE_POSITION_MARGIN_Y
#undef MESSAGE_POSITION_SHIFT
