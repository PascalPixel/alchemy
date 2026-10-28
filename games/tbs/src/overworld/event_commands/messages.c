/// @file
/// Messages shown at an actor, and the speaker's voice.
// Part of event_commands.c.

#include "../event_commands_internal.h"

#ifndef UBFIX
/// The heap slots from the window work's onward, seen as the window work and,
/// twelve slots on, the field work in HEAP_SLOT_FIELD.
struct HeapSlotsFromWindowView {
  u8 *window;
  u8 *slotsBetween[11]; ///< the eleven heap slots after the window work's
  struct FieldWork *event;
};
#endif

/// The event command "open message": opens the event's current message in a
/// speech window next to the speaking actor, with the speaker's portrait
/// beside it when it has one, and moves on to the next message. The low 12
/// bits of actorId are the speaker (0 to 7, a party member, speaks from the
/// leader); its high bits place the windows: 0x1000 portrait on the left,
/// 0x2000 on the right, 0x4000 message above the speaker, 0x8000 never above.
/// Returns the message window once its text is typed out; the caller waits
/// for it to be read. Records the speaker and both windows in the field work
/// (speaker, messageWindow, sideWindow). With the debug fastForward set
/// nothing opens and it returns NULL, but the message still advances.
struct Window *FieldMessage_ShowAtActor(s32 actorId, s32 unused) {
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
#if !TBS_PATCH_1
  s32 none;
#endif

  win = (struct WindowWork *)gWindowWork[0];
  // UB: the fabricated HeapSlotsFromWindowView aggregate reaches from the
  // window slot into HEAP_SLOT_FIELD. Its named load adds eight bytes in native
  // EN (0x344 to 0x34c).
#ifdef UBFIX
  work = gFieldWork;
#else
  work = (struct FieldWork *)((struct HeapSlotsFromWindowView *)gWindowWork)
             ->event;
#endif
  handle = 0;
  portrait = 0;
  sprite = Actor_ReadActiveValue(actorId);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = actorId & 0xf000;
  message = work->message;
  actorId &= 0xfff;
  x = 0;
  actor = Actor_Lookup(actorId);
  work->speaker = actorId;
  y = 0;
  found = 0;
  if (work->fastForward == 0) {
    // Find the speaker on screen, in 8-pixel tiles.
    if (actor != 0) {
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
        Transform_ProjectPoint(&actor->x.fixed, pos);
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
    } else if (actorId <= 7) {
      sprite = actorId;
      actor = Actor_Lookup(gAdventure.leaderActorId);
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
        Transform_ProjectPoint(&actor->x.fixed, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (Actor_GetScreenPosition(gAdventure.leaderActorId, pos) != -1)
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
      left = 15;
      top = 10;
    } else {
      left = 0;
      top = 0;
      Text_GetMessageSizeAltFar(message, &left, &top, (u32 *)&width,
                                (u32 *)&height);
      left = x - width / 2;
      if (flags & SPEAKER_WINDOW_ABOVE)
        top = y - height - 1;
      else if (!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    // Place the portrait beside the speaker, on the side with more room. The
    // screen is 30 tiles wide (columns 0 to 29) and margin is the room the
    // portrait takes, in tiles: 5 while Mind Read shows thoughts (the window
    // mode), else 4.
    if (win->mindRead != 0)
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

    // A speaker with a portrait: put the portrait's 5 rows just above or
    // below the message, within the screen's rows 0 to 19, and grow the
    // message to the portrait's height.
#if TBS_PATCH_1
    found = Portrait_GetForSpeakerFar(sprite);
    if (found != -1) {
      Text_GetMessageSizeAltFar(message, &left, &top, (u32 *)&width,
                                (u32 *)&height);
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
    none = Portrait_GetForSpeakerFar(sprite);
    found = -1;
    if (none != found) {
      Text_GetMessageSizeAltFar(message, &left, &top, (u32 *)&width,
                                (u32 *)&height);
      message = found;
      if (top > y)
        portraitTop = top - 5;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + 5 > 19)
        portraitTop = top - 5;
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
    else if (column + margin > 29)
      column = 29 - margin;

    // Open the message and the portrait, and wait until the text is typed
    // out.
    // Mind Read's thoughts (window mode set) open after 8 frames with no
    // voice; spoken messages zoom open (option bit 0) with the speaker's
    // voice in bits 16 and up.
    if (win->mindRead != 0) {
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
    if (win->mindRead != 0)
      portrait = (s32)Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    else
      portrait = (s32)Portrait_OpenWindowFar(sprite, 0, column, portraitTop);
    while (!Message_AllTypedFar())
      wait(1);
  }
  work->messageWindow = (s32)handle;
  work->sideWindow = portrait;
  work->message++;
  return handle;
}

/// The event command "show message": opens the current message at actorId
/// (FieldMessage_ShowAtActor) and waits until it has been read and its
/// window has closed; the speaker's portrait is closed once the text is typed
/// out. The message closes by itself after 600 frames, or at once while
/// Select, L, R and A are all held. About 70 scenes reach it
/// (Event_ShowMessage, MESSAGE_PUT); the trigger runner (overworld/triggers.c),
/// Field_RunPsynergyEvent (dispatch.c), Field_UseItem for an item trigger that
/// is a message (overworld/items.c) and the debug menu's message test call it
/// too.
void Event_ShowMessageAndWait(s32 actorId, s32 flags) {
  s32 member;
  struct FieldWork *runtime = gFieldWork;
  struct Window *window = FieldMessage_ShowAtActor(actorId, flags);
  s32 sprite;
  u32 frames;

  frames = 0;
  wait(1);
  // A party member the scene does not place has its portrait under its
  // character id, as FieldMessage_ShowAtActor opened it.
  sprite = Actor_ReadActiveValue(actorId);
  if (actorId <= 7) {
    member = actorId & 0x0fff;

    if (Scene_FindActorPlacement(member) == 0) {
      sprite = member;
    }
  }
  Portrait_CloseWindowFar(sprite);

  if (runtime->fastForward == 0) {
    while (Window_IsClosedFar(window) == 0) {
      wait(1);
      frames++;
      if (frames > 600 || ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                           (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
        Message_CloseFinishedWindowsFar();
      }
    }
  }

  wait(1);
}

/// The event command "show message and wait": shows the message, then waits the
/// given number of frames.
void Event_ShowMessageThenWait(s32 actorId, s32 flags, s32 frames) {
  Event_ShowMessageAndWait(actorId, flags);
  Event_Wait(frames);
}

/// The event command "ask yes or no": opens the question at actorId, lets
/// the leader answer, then shows the next message for yes or the one after it
/// for no. Returns 0 for yes, 1 for no. Either way the event's message number
/// ends past both answers, so the event goes on with the same message
/// whatever was chosen.
s32 Event_AskYesNoAndShowResponse(s32 actorId, s32 flags) {
  s32 answer;
  struct FieldWork *runtime;
  struct Adventure *save;

  FieldMessage_ShowAtActor(actorId, flags);
  save = &gAdventure;
  answer = Event_AskYesNoWithActorAnimation(save->leaderActorId, 0);
  if (answer == 0) {
    Event_ShowMessageAndWait(actorId, flags);
    runtime = gFieldWork;
    runtime->message += 1;
  } else {
    runtime = gFieldWork;
    runtime->message += 1;
    Event_ShowMessageAndWait(actorId, flags);
  }
  return answer;
}

void Event_EmptyCallback(void) {}

/// Unused: reached only through a bare veneer in
/// cartridge/far_call/OVERWORLD.S, which has no symbol, so no code can call it.
/// It would record the speaker, open the message near it and wait unless
/// fast-forwarding, then advance the message index. Like
/// Event_ShowMessageAtPosition it moves the position 32 away from the middle
/// and clamps it to 8..312 and 20..220, ranges larger than the 240 by 160
/// screen, before passing it to a window routine that takes tiles; with no
/// caller, the units it expected can't be told.
void Event_ShowSpeakerMessage(s32 speaker) {
  // BUG: the native lookup discards its result and leaves pos unset.
#ifdef UBFIX
  s32 pos[3];
  struct Actor *object;
#else
  struct {
    s32 x, y;
  } pos;
#endif
  struct FieldWork *state = gFieldWork;

  speaker &= 0xfff;
#ifdef UBFIX
  // Only the actor slots hold actor pointers;
  // Actor_Lookup's legacy range also exposes the event fields after
  // them.
  if ((u32)speaker < sizeof(state->actors) / sizeof(state->actors[0]))
    object = Actor_Lookup(speaker);
  else
    object = NULL;
#else
  Actor_Lookup(speaker);
#endif
  state->speaker = speaker;
  if (state->fastForward == 0) {
#ifdef UBFIX
    s32 positionFound = 0;

    // Supply the actual speaker position as FieldMessage_ShowAtActor does.
    // A missing actor or failed projection has no position to open
    // around; still consume the message, as the fast-forward path does.
    if (object != NULL) {
      if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
        positionFound =
            Transform_ProjectPoint(
                (const u8 *)object + offsetof(struct Actor, x), pos) != 0;
      else
        positionFound = Actor_GetScreenPosition(speaker, pos) != -1;
    }
    if (positionFound) {
      s32 px = pos[0];
      s32 py = pos[1];
      s32 win;
#else
    s32 px = pos.x;
    s32 py = pos.y;
    s32 win;
#endif

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
      win = (s32)Message_OpenWindowFar(state->message, px, py, 1);
      state->messageWindow = win;
      while (Window_IsClosedFar((struct Window *)win) == 0)
        wait(1);
#ifdef UBFIX
    }
#endif
  }
  state->message++;
}

/// Unused: reached only through a bare veneer in OVERWORLD.S; forwards its
/// arguments to Event_ShowMessageAtPosition.
void Event_ForwardShowMessageAtPosition(s32 unused0, s32 unused1, s32 x,
                                        s32 y) {
  Event_ShowMessageAtPosition(unused0, unused1, x, y);
}

/// Unused: reached only through a bare veneer in OVERWORLD.S. It would open the
/// event's message at (x, y), wait for it and advance the message index; its
/// units are as unknown as Event_ShowSpeakerMessage's.
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
  struct FieldWork *state = gFieldWork;
  s32 py = y;
  s32 px = x;
  s32 minX = 8;
  s32 minY = 20;
  s32 window;

  if (py > 119)
    py += 32;
  else
    py -= 32;

  if (x < minX)
    px = minX;
  if (px > 312)
    px = 312;
  CLAMP(py, minY, 220);

  window = (s32)Message_OpenWindowFar(state->message, px, py, 1);
  while (Window_IsClosedFar((struct Window *)window) == 0)
    wait(1);
  state->message++;
}

/// Closes the portrait of an actor's sprite, if it has one.
void Actor_RunIfActive(s32 actorId) {
  s32 sprite;

  sprite = Actor_ReadActiveValue(actorId);
  if (sprite != -1)
    Portrait_CloseWindowFar(sprite);
}

/// The event command "show two messages and wait": two actors speak at
/// once, each message at its own (x, y) with its portrait at (portraitX,
/// portraitY), all in tiles. Waits for A, B, L or R, then closes both.
void Event_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                  s32 firstPortraitX, s32 firstPortraitY,
                                  s32 second, s32 secondX, s32 secondY,
                                  s32 secondPortraitX, s32 secondPortraitY) {
  struct FieldWork *work = gFieldWork;
  s32 firstSprite = Actor_ReadActiveValue(first);
  s32 secondSprite = Actor_ReadActiveValue(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = (s32)Message_OpenWindowFar(work->message++, firstX, firstY,
                                           Speaker_GetVoice(firstSprite) << 16);
  Portrait_OpenWindowFar(firstSprite, 0, firstPortraitX, firstPortraitY);

  secondWindow = (s32)Message_OpenWindowFar(
      work->message++, secondX, secondY, Speaker_GetVoice(secondSprite) << 16);
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

/// Sets the voice of the messages that follow to that of actorId's sprite,
/// at the rate the message speed option picks (gMessageSpeed_VoiceRates). With
/// 0x80000000 the messages are silent.
void Message_SetVoiceFromActor(s32 actorId) {
  struct WindowWork *state = (struct WindowWork *)gWindowWork[0];
  u8 rate;
  s32 voice;

  // Direct u16 stores shrink the English function from 0x58 to 0x54 bytes.
  // Keep the original signed stores; the corresponding u16 alias is legal.
  if (actorId == 0x80000000) {
    *(s16 *)&state->voiceSound = (rate = 0);
    *(s16 *)&state->voiceCountdown = rate;
  } else {
    voice = Speaker_GetVoice(Actor_ReadActiveValue(actorId));
    rate = gMessageSpeed_VoiceRates[gAdventure.messageSpeed];
    *(s16 *)&state->voiceSound = voice;
    *(s16 *)&state->voiceCountdown = rate;
  }
}
