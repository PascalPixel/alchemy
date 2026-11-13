/// @file
/// The Colosso choice and the joined-party messages.
// Part of screens.c.

#include "../screens_internal.h"

// UB: the joined messages pass five of Message_Start' six
// arguments, so its flags are whatever the stack slot of the sixth holds;
// UBFIX passes 0, as the window object's own messages do.
#ifdef UBFIX
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing)                        \
  Message_Start(window, entry, x, y, (u16 *)(spacing), 0)
#else
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing)                        \
  Message_Start(window, entry, x, y, spacing)
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TALK_WINDOW_X 7
#define TALK_WINDOW_WIDTH 18
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define TALK_WINDOW_X 9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X 8
#define TALK_WINDOW_WIDTH 16
#endif

/// The Colosso choice of what to do: "Descriptions", "Cheer" or "Nothing",
/// chosen with a pointing hand.
s32 ColossoMenu_Choose(s32 unused) {
  struct Window *window;
  struct MenuCursor cursor;
  s32 slot;
  s32 row;
  s32 moved;
  u16 *palette;

  moved = 1;
  window = Window_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7,
                         WINDOW_FLAG_INSTANT);
  Text_DrawMessageAsTiles(MsgTalkChoice, window, 8, 0);
  Text_DrawMessageAsTiles(MsgTalkChoice + 1, window, 8, 16);
  Text_DrawMessageAsTiles(MsgTalkChoice + 2, window, 8, 32);
  slot = VramCache_FindFreeEntry();
  row = 0;
  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(slot, 128, gPointingHandTiles);
    cursor.anchor = WindowSprite_Create(slot, OUTPUT_SIZE_16X16, window, 0, 0);
    MenuCursor_SetPositionImmediateFar(&cursor, window->x * 8 - 3,
                                       window->y * 8 + 9);
  }
  // The choice under the cursor flashes between WINDOW_PALETTE and
  // HIGHLIGHT_PALETTE, a copy of it with WINDOW_BACKGROUND_COLOR grey.
  palette = (u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
          DMA_COPY_WORDS(8), REG_DMA3);
  palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
  while (1) {
    Window_SetTextTileRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                           HIGHLIGHT_PALETTE);
    wait(1);
    Window_SetTextTileRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                           WINDOW_PALETTE);
    if (moved) {
      moved = 0;
      MenuCursor_SetFar(&cursor, window->x * 8 - 3,
                        (window->y + row * 2) * 8 + 9, 3);
    }
    MenuCursor_AdvanceFar(&cursor);
    if (gKeysRepeat & KEY_UP) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      row--;
      moved = 1;
      if (row == -1)
        row = 2;
    }
    if (gKeysRepeat & KEY_DOWN) {
      Sound_PlayFar(SE_MENU_CURSOR_MOVE);
      row++;
      moved = 1;
      if (row == 3)
        row = 0;
    }
    if (gKeysRepeat & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      row = -1;
      break;
    }
    if (gKeysRepeat & KEY_A) {
      Sound_PlayFar(SE_MENU_CONFIRM);
      break;
    }
  }
  Window_Close(window, 2);
  wait(1);
  VramCache_FreeEntry(slot);
  return row;
}

/// Shows that a Djinni joined a party member while the join jingle (MUS_JOINED)
/// plays, until it ends or A, B, L or R is pressed.
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn) {
  struct WindowWork *work = WINDOW_WORK;
  s32 speaker;
  struct Window *window;
  s32 portrait[3];
  s32 *entry;
  s32 palette, handle;
  s32 *word;
  u32 cleared;

  window = NULL;
  cleared = 0;
  speaker = gDjinn_ElementSprites[element & 3];
  entry = portrait;
  window = Window_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);
  if (window == NULL)
    return;
  Window_DrawDividerLine(window, 4, 0, 4, 4);
  work->dirty = 1;
  Icon_LoadWithPalette(Portrait_GetForSpeaker(speaker), cleared, &handle,
                       &palette, PORTRAIT_PALETTE, cleared);
  // The portrait as a bare OAM entry, pushed onto draw layer 250 each
  // frame: a link word, attributes 0 and 1 and attribute 2.
  word = entry;
  *word++ = cleared;
  *word++ = OBJ_SIZE_32X32 | ((DJINN_JOIN_X * 8 + 4) << 16) | 12;
  *word = palette | OBJ_PALETTE(PORTRAIT_PALETTE);
  // No voice blips while the message types.
  work->voiceSound = cleared;
  work->voiceCountdown = cleared;
  MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_Push(element * DJINN_PER_ELEMENT + djinn +
                           ABILITY_DJINNI_FIRST,
                       MESSAGE_ARGUMENT_ABILITY);
  JOINED_QUEUE_TEXT(
      window, Message_DecodeAndClearArguments(element + MsgVenusDjinnJoined),
      36, 2, cleared);
  Sound_PlayFar(MUS_JOINED);
  do {
    Oam_AddSpriteAtDepth(entry, 250);
    wait(1);
  } while (Sound_GetBgmRestoreDelayFar() != 0 &&
           (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
  Window_Close(window, 2);
  wait(1);
  VramCache_FreeEntry(handle);
}

/// Returns the portrait character of party member `member` (0-8): from
/// gCharacter_ChildSpriteTable during the prologue, from
/// gCharacter_SpriteTable otherwise; 0 for any other party member.
s32 Party_GetMemberPortraitSprite(u32 index) {
  if (index > 8) {
    return 0;
  }
  if (GameFlag_GetFar(FLAG_CHILDHOOD) == 0) {
    return gCharacter_SpriteTable[index];
  }
  return gCharacter_ChildSpriteTable[index];
}

/// Shows that a party member joined the party while the join jingle
/// (MUS_JOINED) plays, until it ends or A, B, L or R is pressed.
void Party_ShowJoinedMessage(s32 member) {
  struct WindowWork *work = WINDOW_WORK;
  struct Window *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 palette, handle;
  s32 *p;

  window = Window_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);
    work->dirty = 1;
    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_GetMemberPortraitSprite(member)), 0,
        &handle, &palette, PORTRAIT_PALETTE, 0);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 and attribute 2.
    p = entry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | ((2 * 8 + 4) << 16) | 12;
    *p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);
    work->voiceSound = 0;
    work->voiceCountdown = 0;
    MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
    JOINED_QUEUE_TEXT(window, Message_DecodeAndClearArguments(MsgJoinedParty),
                      36, 2, 0);
    Sound_PlayFar(MUS_JOINED);
    do {
      Oam_AddSpriteAtDepth(entry, 250);
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
    Window_Close(window, 2);
    wait(1);
    VramCache_FreeEntry(handle);
  }
}

/// Shows that two party members joined the party while the join jingle (sound
/// 81) plays, until it ends or A, B, L or R is pressed.
void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  u8 *work = gWindowWork[0];
  struct Window *window;
  s32 secondOam[3];
  s32 firstOam[3];
  s32 *firstEntry;
  s32 *secondEntry;
  s32 firstHandle, palette, secondHandle;
  s32 *p;

  window = NULL;
  firstEntry = firstOam;
  window = Window_Create(1, 1, 28, 5, 0);

  if (window != NULL) {
    // BUG: from (8, 0) to (4, 4) is neither across nor down, so
    // Window_DrawDividerLine draws nothing and the window has no divider. The
    // single joins draw (4, 0, 4, 4), half a tile before their text at x 36;
    // column 8 is the same place before this one's text at x 68.
#ifdef BUGFIX
    Window_DrawDividerLine(window, 8, 0, 8, 4);
#else
    Window_DrawDividerLine(window, 8, 0, 4, 4);
#endif

    ((struct WindowWork *)work)->dirty = 1;

    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_GetMemberPortraitSprite(first)), 0,
        &firstHandle, &palette, PORTRAIT_PALETTE, 0);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 and attribute 2.
    p = firstEntry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | (12 << 16) | 12;
    *p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);

    secondEntry = secondOam;
    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_GetMemberPortraitSprite(second)), 0,
        &secondHandle, &palette, PORTRAIT_PALETTE + 1, 0);
    p = secondEntry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | (44 << 16) | 12;
    *p = palette | OBJ_PALETTE(PORTRAIT_PALETTE + 1);

    ((struct WindowWork *)work)->voiceSound = 0;
    ((struct WindowWork *)work)->voiceCountdown = 0;

    MessageArgument_Push(first, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(second, MESSAGE_ARGUMENT_ACTOR);

    JOINED_QUEUE_TEXT(
        window, Message_DecodeAndClearArguments(MsgPairJoinedParty), 68, 2, 0);

    Sound_PlayFar(MUS_JOINED);

    do {
      Oam_AddSpriteAtDepth(firstEntry, 250);
      Oam_AddSpriteAtDepth(secondEntry, 250);
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    VramCache_FreeEntry(firstHandle);
    VramCache_FreeEntry(secondHandle);
  }
}

#undef JOINED_QUEUE_TEXT
