/// @file
/// Messages shown at an actor, and the speaker's voice.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

#ifndef UBFIX
/// The work slots from the window work's onward, seen as the window work and,
/// twelve slots on, the event work in slot 27.
struct WorkPointers {
  u8 *window;
  u8 unk04[0x2c];
  struct FieldWork *event;
};
#endif

/// The event command "open message": opens the event's current message in a
/// speech window next to the speaking actor, with the speaker's portrait
/// beside it when it has one, and moves on to the next message. The low 12
/// bits of actor are the speaker (0 to 7, a party member, speaks from the
/// leader); its high bits place the windows: 0x1000 portrait on the left,
/// 0x2000 on the right, 0x4000 message above the speaker, 0x8000 never above.
/// Returns the message window.
struct UiWindow *UiText_OpenMessageAtObject(s32 actor, s32 unused) {
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
  // into event slot 27. Its named load adds eight bytes in native EN
  // (0x344 to 0x34c).
#ifdef UBFIX
  work = (struct FieldWork *)gEventWork;
#else
  work = (struct FieldWork *)((struct WorkPointers *)gWindowWork)->event;
#endif
  handle = 0;
  portrait = 0;
  sprite = ObjectTable_ReadActiveValue(actor);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = actor & 0xf000;
  message = work->message;
  actor &= 0xfff;
  x = 0;
  object = ObjectTable_Get(actor);
  work->speaker = actor;
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
        if (Object_GetScreenPosition(actor, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (actor <= 7) {
      sprite = actor;
      object = ObjectTable_Get(gGameState.selectedActor);
      if (work->mode == MODE_WORLD_MAP) {
        Render_ProjectPoint(&object->x, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (Object_GetScreenPosition(gGameState.selectedActor, pos) != -1)
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
      if (flags & 0x4000)
        top = y - height - 1;
      else if (!(flags & 0x8000) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    // Place the portrait beside the speaker, on the side with more room.
    if (win->mode != 0)
      margin = 5;
    column = x;
    if (flags & 0x1000) {
      column = x - margin - 2;
      if (column < 0)
        column = 0;
    } else if (flags & 0x2000) {
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
    // below the message, and grow the message to the portrait's height.
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

    // Open the message (with the speaker's voice outside mode 1) and the
    // portrait, and wait for both to be drawn.
    if (win->mode != 0) {
      WaitFrames(8);
      if (extra != 0)
        handle =
            UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
      else
        handle = UiText_OpenMessageWindowFar(message, left, top, 2);
    } else {
      s32 voice = BattleFx_GetResourceId(sprite);

      if (extra != 0)
        handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                             (voice << 16) | 17);
      else
        handle =
            UiText_OpenMessageWindowFar(message, left, top, (voice << 16) | 1);
    }
    if (win->mode != 0)
      portrait =
          UiWindow_CreateWithSideObjectFar(sprite, 0, column, portraitTop);
    else
      portrait =
          UiWindow_CreateWithSideObjectFar(sprite, 0, column, portraitTop);
    while (!UiWork_IsCompleteFar())
      WaitFrames(1);
  }
  work->messageWindow = (s32)handle;
  work->sideWindow = portrait;
  work->message++;
  return handle;
}

/// The event command "show message": opens the current message at actor
/// (UiText_OpenMessageAtObject) and waits until it has been read. The message
/// closes by itself after 600 frames, or at once while Select, L, R and A are
/// all held.
void FieldEvent_ShowMessageAndWait(s32 actor, s32 flags) {
  s32 member;
  struct FieldWork *runtime = (struct FieldWork *)gEventWork;
  struct UiWindow *window = UiText_OpenMessageAtObject(actor, flags);
  s32 sprite;
  u32 frames;

  frames = 0;
  WaitFrames(1);
  sprite = ObjectTable_ReadActiveValue(actor);
  if (actor <= 7) {
    member = actor & 0x0fff;

    if (Scene_FindActorPlacement(member) == 0) {
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

/// The event command "show message and wait": shows the message, then waits
/// wait frames.
void BattlePres_RunActionThenWaitIfModeZero(s32 actor, s32 flags, s32 wait) {
  FieldEvent_ShowMessageAndWait(actor, flags);
  FieldEvent_WaitFrames(wait);
}

/// The event command "ask yes or no": opens the question at actor, lets the
/// leader answer, then shows the next message for yes or the one after it for
/// no. Returns 0 for yes, 1 for no.
s32 FieldEvent_AskYesNoAndShowResponse(s32 actor, s32 flags) {
  s32 answer;
  struct FieldWork *runtime;
  struct GameState *save;

  UiText_OpenMessageAtObject(actor, flags);
  save = &gGameState;
  answer = Event_AskYesNoWithActorAnimation(save->selectedActor, 0);
  if (answer == 0) {
    FieldEvent_ShowMessageAndWait(actor, flags);
    runtime = (struct FieldWork *)gEventWork;
    runtime->message += 1;
  } else {
    runtime = (struct FieldWork *)gEventWork;
    runtime->message += 1;
    FieldEvent_ShowMessageAndWait(actor, flags);
  }
  return answer;
}

/// Does nothing.
void BattleEventRuntime_EmptyCallback(void) {}

/// Opens the event's current message for speaker, kept 32 pixels above or
/// below a point and on screen, waits until it has been read and moves on to
/// the next message.
void Event_ShowSpeakerMessage(s32 speaker) {
  // BUG: pos is never set, so the window goes wherever the stack's old
  // contents put it.
  struct {
    s32 x, y;
  } pos;
  struct FieldWork *state = (struct FieldWork *)gEventWork;

  speaker &= 0xfff;
  ObjectTable_Get(speaker);
  state->speaker = speaker;
  if (state->fastForward == 0) {
    s32 px = pos.x;
    s32 py = pos.y;
    s32 win;

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
    win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
    state->messageWindow = win;
    while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
      WaitFrames(1);
  }
  state->message++;
}

/// Shows the current message near the screen point (x, y)
/// (Event_ShowMessageAtPosition).
void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y) {
  Event_ShowMessageAtPosition(unused0, unused1, x, y);
}

/// Opens the event's current message 32 pixels above or below the screen
/// point (x, y), kept on screen, waits until it has been read and moves on to
/// the next message.
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y) {
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

  window = UiText_OpenMessageWindowFar(state->message, px, py, 1);
  while (UiWork_IsIdleFar((struct UiWindow *)window) == 0)
    WaitFrames(1);
  state->message++;
}

/// Closes the portrait of an actor's sprite, if it has one.
void ObjectTable_RunIfActive(s32 actor) {
  s32 sprite;

  sprite = ObjectTable_ReadActiveValue(actor);
  if (sprite != -1)
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
}

/// The event command "show two messages and wait": two actors speak at once,
/// each message at its own (x, y) with its portrait at (portraitX, portraitY),
/// all in tiles. Waits for A, B, L or R, then closes both.
void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 firstX, s32 firstY,
                                      s32 firstPortraitX, s32 firstPortraitY,
                                      s32 second, s32 secondX, s32 secondY,
                                      s32 secondPortraitX,
                                      s32 secondPortraitY) {
  struct FieldWork *field = (struct FieldWork *)gEventWork;
  s32 firstSprite = ObjectTable_ReadActiveValue(first);
  s32 secondSprite = ObjectTable_ReadActiveValue(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow =
      UiText_OpenMessageWindowFar(field->message++, firstX, firstY,
                                  BattleFx_GetResourceId(firstSprite) << 16);
  UiWindow_CreateWithSideObjectFar(firstSprite, 0, firstPortraitX,
                                   firstPortraitY);

  secondWindow =
      UiText_OpenMessageWindowFar(field->message++, secondX, secondY,
                                  BattleFx_GetResourceId(secondSprite) << 16);
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

/// Sets the voice of the messages that follow to that of actor's sprite, at
/// the rate the message speed option picks (gUi_RenderResultValues). With
/// 0x80000000 the messages are silent.
void Ui_SetMessageVoiceFromActor(s32 actor) {
  struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
  u8 rate;
  s32 voice;

  // Direct u16 stores shrink the English function from 0x58 to 0x54 bytes.
  // Keep the original signed stores; the corresponding u16 alias is legal.
  if (actor == 0x80000000) {
    *(s16 *)&state->speakerVoiceSound = (rate = 0);
    *(s16 *)&state->speakerVoiceCountdown = rate;
  } else {
    voice = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(actor));
    rate = gUi_RenderResultValues[gGameState.messageSpeed];
    *(s16 *)&state->speakerVoiceSound = voice;
    *(s16 *)&state->speakerVoiceCountdown = rate;
  }
}
