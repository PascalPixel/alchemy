/// @file
/// Messages shown at an object event, and the speaker's voice.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

#ifndef UBFIX
/// The work slots from the window work's onward, seen as the window work and,
/// twelve slots on, the event work in HEAP_SLOT_EVENT.
struct WorkPointers {
  u8 *window;
  u8 *slotsBetween[11]; ///< the eleven work slots after the window work's
  struct FieldWork *event;
};
#endif

/// The script command "open message": opens the script's current message in a
/// speech window next to the speaking object event, with the speaker's portrait
/// beside it when it has one, and moves on to the next message. The low 12
/// bits of objectEvent are the speaker (0 to 7, a party member, speaks from the
/// leader); its high bits place the windows: 0x1000 portrait on the left,
/// 0x2000 on the right, 0x4000 message above the speaker, 0x8000 never above.
/// Returns the message window once its text is typed out; the caller waits
/// for it to be read. Records the speaker and both windows in the field work
/// (speaker, messageWindow, sideWindow). With the debug fastForward set
/// nothing opens and it returns NULL, but the message still advances.
struct UiWindow *UiText_OpenMessageAtObject(s32 objectEvent, s32 unused) {
  struct UiRenderWork *win;
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
  struct UiWindow *handle;
  s32 portrait;
  s32 flags;
  s32 message;
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 found;
  s32 column;
#if !TBS_PATCH_1
  s32 none;
#endif

  win = (struct UiRenderWork *)gWindowWork[0];
  // UB: the fabricated WorkPointers aggregate reaches from the window slot
  // into HEAP_SLOT_EVENT. Its named load adds eight bytes in native EN
  // (0x344 to 0x34c).
#ifdef UBFIX
  work = (struct FieldWork *)gEventWork;
#else
  work = (struct FieldWork *)((struct WorkPointers *)gWindowWork)->event;
#endif
  handle = 0;
  portrait = 0;
  sprite = ObjectTable_ReadActiveValue(objectEvent);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = objectEvent & 0xf000;
  message = work->message;
  objectEvent &= 0xfff;
  x = 0;
  object = ObjectTable_Get(objectEvent);
  work->speaker = objectEvent;
  y = 0;
  found = 0;
  if (work->fastForward == 0) {
    // Find the speaker on screen, in 8-pixel tiles.
    if (object != 0) {
      if (work->mode == MODE_WORLD_MAP) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
        y -= 2;
      } else {
        if (ObjectEvent_GetScreenPosition(objectEvent, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (objectEvent <= 7) {
      sprite = objectEvent;
      object = ObjectTable_Get(gGameState.selectedObjectEvent);
      if (work->mode == MODE_WORLD_MAP) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (ObjectEvent_GetScreenPosition(gGameState.selectedObjectEvent,
                                          pos) != -1)
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
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
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
    if (win->mode != 0)
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
    found = Localization_LookupEntryIdFar(sprite);
    if (found != -1) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
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

        UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                        (u32 *)&height);
        message = -1;
        extra = lines - height + 1;
      }
    } else if (top < y) {
      s32 lines = height;

      UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
                                      (u32 *)&height);
      extra = lines - height + 1;
      message = found;
    }
#else
    none = Localization_LookupEntryIdFar(sprite);
    found = -1;
    if (none != found) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width,
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

      UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width,
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
    if (win->mode != 0) {
      WaitFrames(8);
      if (extra != 0)
        handle =
            UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
      else
        handle = UiText_OpenMessageWindowFar(message, left, top, 2);
    } else {
      s32 voice = FieldEffect_GetResourceId(sprite);

      if (extra != 0)
        handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                             (voice << 16) | 17);
      else
        handle =
            UiText_OpenMessageWindowFar(message, left, top, (voice << 16) | 1);
    }
    if (win->mode != 0)
      portrait =
          (s32)UiWindow_CreateWithSideObjectFar(sprite, 0, column, portraitTop);
    else
      portrait =
          (s32)UiWindow_CreateWithSideObjectFar(sprite, 0, column, portraitTop);
    while (!UiWork_IsCompleteFar())
      WaitFrames(1);
  }
  work->messageWindow = (s32)handle;
  work->sideWindow = portrait;
  work->message++;
  return handle;
}

/// The script command "show message": opens the current message at objectEvent
/// (UiText_OpenMessageAtObject) and waits until it has been read and its
/// window has closed; the speaker's portrait is closed once the text is typed
/// out. The message closes by itself after 600 frames, or at once while
/// Select, L, R and A are all held. About 70 maps reach it as
/// Engine_ScriptShowMessage (Script_ShowMessage, MESSAGE_PUT); the map event
/// runner (object/events.c), Field_RunEventAction (dispatch.c), Field_UseItem
/// for an item event that is a message (object/items.c) and the debug menu's
/// message test call it too.
void FieldScript_ShowMessageAndWait(s32 objectEvent, s32 flags) {
  s32 member;
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;
  struct UiWindow *window = UiText_OpenMessageAtObject(objectEvent, flags);
  s32 sprite;
  u32 frames;

  frames = 0;
  WaitFrames(1);
  // A party member the map does not place has its portrait under its
  // character id, as UiText_OpenMessageAtObject opened it.
  sprite = ObjectTable_ReadActiveValue(objectEvent);
  if (objectEvent <= 7) {
    member = objectEvent & 0x0fff;

    if (Map_FindObjectEventTemplate(member) == 0) {
      sprite = member;
    }
  }
  UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);

  if (runtime->fastForward == 0) {
    while (UiWork_IsIdleFar(window) == 0) {
      WaitFrames(1);
      frames++;
      if (frames > 600 || ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) &&
                           (gKeysHeld & KEY_L) && (gKeysHeld & KEY_A))) {
        UiWork_FinalizePendingCoreFar();
      }
    }
  }

  WaitFrames(1);
}

/// The script command "show message and wait": shows the message, then waits
/// wait frames.
void FieldScript_ShowMessageThenWait(s32 objectEvent, s32 flags, s32 wait) {
  FieldScript_ShowMessageAndWait(objectEvent, flags);
  FieldScript_WaitFrames(wait);
}

/// The script command "ask yes or no": opens the question at objectEvent, lets
/// the leader answer, then shows the next message for yes or the one after it
/// for no. Returns 0 for yes, 1 for no. Either way the script's message number
/// ends past both answers, so the script goes on with the same message
/// whatever was chosen.
s32 FieldScript_AskYesNoAndShowResponse(s32 objectEvent, s32 flags) {
  s32 answer;
  struct FieldWork *runtime;
  struct GameState *save;

  UiText_OpenMessageAtObject(objectEvent, flags);
  save = &gGameState;
  answer = FieldScript_AskYesNoWithObjectEventAnimation(
      save->selectedObjectEvent, 0);
  if (answer == 0) {
    FieldScript_ShowMessageAndWait(objectEvent, flags);
    runtime = (struct FieldWork *)gEventWork;
    runtime->message += 1;
  } else {
    runtime = (struct FieldWork *)gEventWork;
    runtime->message += 1;
    FieldScript_ShowMessageAndWait(objectEvent, flags);
  }
  return answer;
}

void FieldScript_EmptyCallback(void) {}

/// Unused: reached only through a bare veneer in system/far_call/OBJECT.S,
/// which has no symbol, so no code can call it. It would record the speaker,
/// open the message near it and wait unless fast-forwarding, then advance the
/// message index. Like FieldScript_ShowMessageAtPosition it moves the position
/// 32 away from the middle and clamps it to 8..312 and 20..220, ranges larger
/// than the 240 by 160 screen, before passing it to a window service that
/// takes tiles; with no caller, the units it expected can't be told.
void FieldScript_ShowSpeakerMessage(s32 speaker) {
  // BUG: the native lookup discards its result and leaves pos unset.
#ifdef UBFIX
  s32 pos[3];
  struct ObjectRuntime *object;
#else
  struct {
    s32 x, y;
  } pos;
#endif
  struct FieldWork *state = (struct FieldWork *)gEventWork;

  speaker &= 0xfff;
#ifdef UBFIX
  // Only the object event slots hold object event pointers; ObjectTable_Get's
  // legacy range also exposes the event fields after them.
  if ((u32)speaker <
      sizeof(state->objectEvents) / sizeof(state->objectEvents[0]))
    object = ObjectTable_Get(speaker);
  else
    object = NULL;
#else
  ObjectTable_Get(speaker);
#endif
  state->speaker = speaker;
  if (state->fastForward == 0) {
#ifdef UBFIX
    s32 positionFound = 0;

    // Supply the actual speaker position as UiText_OpenMessageAtObject does.
    // A missing object event or failed projection has no position to open
    // around; still consume the message, as the fast-forward path does.
    if (object != NULL) {
      if (state->mode == MODE_WORLD_MAP)
        positionFound =
            Render_ProjectPoint(
                (const u8 *)object + offsetof(struct ObjectRuntime, x), pos) !=
            0;
      else
        positionFound = ObjectEvent_GetScreenPosition(speaker, pos) != -1;
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
      win = (s32)UiText_OpenMessageWindowFar(state->message, px, py, 1);
      state->messageWindow = win;
      while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
        WaitFrames(1);
#ifdef UBFIX
    }
#endif
  }
  state->message++;
}

/// Unused: reached only through a bare veneer in OBJECT.S; forwards its
/// arguments to FieldScript_ShowMessageAtPosition.
void FieldScript_ForwardShowMessageAtPosition(s32 unused0, s32 unused1, s32 x,
                                              s32 y) {
  FieldScript_ShowMessageAtPosition(unused0, unused1, x, y);
}

/// Unused: reached only through a bare veneer in OBJECT.S. It would open the
/// script's message at (x, y), wait for it and advance the message index; its
/// units are as unknown as FieldScript_ShowSpeakerMessage's.
void FieldScript_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
  struct FieldWork *state = (struct FieldWork *)gEventWork;
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

  window = (s32)UiText_OpenMessageWindowFar(state->message, px, py, 1);
  while (UiWork_IsIdleFar((struct UiWindow *)window) == 0)
    WaitFrames(1);
  state->message++;
}

/// Closes the portrait of an object event's sprite, if it has one.
void ObjectTable_RunIfActive(s32 objectEvent) {
  s32 sprite;

  sprite = ObjectTable_ReadActiveValue(objectEvent);
  if (sprite != -1)
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
}

/// The script command "show two messages and wait": two object events speak at
/// once, each message at its own (x, y) with its portrait at (portraitX,
/// portraitY), all in tiles. Waits for A, B, L or R, then closes both.
void FieldScript_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                        s32 firstPortraitX, s32 firstPortraitY,
                                        s32 second, s32 secondX, s32 secondY,
                                        s32 secondPortraitX,
                                        s32 secondPortraitY) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  s32 firstSprite = ObjectTable_ReadActiveValue(first);
  s32 secondSprite = ObjectTable_ReadActiveValue(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow = (s32)UiText_OpenMessageWindowFar(
      work->message++, firstX, firstY,
      FieldEffect_GetResourceId(firstSprite) << 16);
  UiWindow_CreateWithSideObjectFar(firstSprite, 0, firstPortraitX,
                                   firstPortraitY);

  secondWindow = (s32)UiText_OpenMessageWindowFar(
      work->message++, secondX, secondY,
      FieldEffect_GetResourceId(secondSprite) << 16);
  UiWindow_CreateWithSideObjectFar(secondSprite, 0, secondPortraitX,
                                   secondPortraitY);

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames(1);
  while ((gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
    WaitFrames(1);

  WaitFrames(1);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(firstSprite);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(secondSprite);
  UiWork_FinalizePendingCoreFar();
  WaitFrames(1);

  while (UiWork_IsIdleFar((struct UiWindow *)firstWindow) == 0)
    WaitFrames(1);
  while (UiWork_IsIdleFar((struct UiWindow *)secondWindow) == 0)
    WaitFrames(1);

  WaitFrames(1);
}

/// Sets the voice of the messages that follow to that of objectEvent's sprite,
/// at the rate the message speed option picks (gUi_RenderResultValues). With
/// 0x80000000 the messages are silent.
void Ui_SetMessageVoiceFromObjectEvent(s32 objectEvent) {
  struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
  u8 rate;
  s32 voice;

  // Direct u16 stores shrink the English function from 0x58 to 0x54 bytes.
  // Keep the original signed stores; the corresponding u16 alias is legal.
  if (objectEvent == 0x80000000) {
    *(s16 *)&state->speakerVoiceSound = (rate = 0);
    *(s16 *)&state->speakerVoiceCountdown = rate;
  } else {
    voice = FieldEffect_GetResourceId(ObjectTable_ReadActiveValue(objectEvent));
    rate = gUi_RenderResultValues[gGameState.messageSpeed];
    *(s16 *)&state->speakerVoiceSound = voice;
    *(s16 *)&state->speakerVoiceCountdown = rate;
  }
}
