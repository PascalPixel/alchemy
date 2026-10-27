/// @file
/// The Colosso choice and the joined-party messages.
// Part of screens.c.

#include "../screens_internal.h"

// UB: the joined messages pass five of TextPrinter_Add' six
// arguments, so its flags are whatever the stack slot of the sixth holds;
// UBFIX passes 0, as the window object's own messages do.
#ifdef UBFIX
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing)                        \
  TextPrinter_Add(window, entry, x, y, (u16 *)(spacing), 0)
#else
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing)                        \
  TextPrinter_Add(window, entry, x, y, spacing)
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
/// chosen with a pointing hand. Returns the choice (0-2), or -1 when
/// cancelled with B. The Colosso maps ask it (scene/common/colosso_solo.h).
/// It takes a VRAM slot for the hand and frees it before returning.
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
          DMA_COPY32(8), REG_DMA3);
  palette[4] = 0x6318;
  while (1) {
    Window_SetTextTileRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                           HIGHLIGHT_PALETTE);
    WaitFrames(1);
    Window_SetTextTileRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                           WINDOW_PALETTE);
    if (moved) {
      moved = 0;
      MenuCursor_SetFar(&cursor, window->x * 8 - 3,
                        (window->y + row * 2) * 8 + 9, 3);
    }
    MenuCursor_AdvanceFar(&cursor);
    if (gKeysRepeat & KEY_UP) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      row--;
      moved = 1;
      if (row == -1)
        row = 2;
    }
    if (gKeysRepeat & KEY_DOWN) {
      Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
      row++;
      moved = 1;
      if (row == 3)
        row = 0;
    }
    if (gKeysRepeat & KEY_B) {
      Sound_PlaySongFar(SE_MENU_CANCEL);
      row = -1;
      break;
    }
    if (gKeysRepeat & KEY_A) {
      Sound_PlaySongFar(SE_MENU_CONFIRM);
      break;
    }
  }
  Window_Close(window, 2);
  WaitFrames(1);
  VramCache_FreeEntry(slot);
  return row;
}

/// Shows that a Djinni joined a member while the join fanfare (sound 81)
/// plays, until it ends or A, B, L or R is pressed. The message is
/// `The <element> Djinni <name> joined <member>.`, with the element's Djinni
/// portrait. `djinn` is the Djinni's number in its element. The field's
/// Djinni events reach it by far call (overworld/event_commands/djinni.c). It
/// closes the window at once (Window_Close with instant set), waits a
/// frame and frees the portrait's VRAM slot.
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn) {
  struct WindowWork *work = (struct WindowWork *)*gWindowWork;
  s32 name;
  struct Window *window;
  s32 sprite[3];
  s32 *entry;
  s32 palette, handle;
  s32 id;
  s32 text;
  u32 zero;
  s32 *p;

  window = NULL;
  zero = 0;
  name = gDjinn_ElementSprites[element & 3];
  entry = sprite;
  window = Window_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);

    work->dirty = 1;

    id = Portrait_GetForSpeaker(name);
    Icon_LoadWithPalette(id, zero, &handle, &palette, 14, zero);

    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2, its
    // tile with OBJ palette 14.
    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
    *p = palette | 0xe000;

    // No voice blips while the message types.
    work->voiceSound = zero;
    work->voiceCountdown = zero;

    // The message's arguments: the member (MESSAGE_ARGUMENT_ACTOR) and the
    // Djinni's name, its ability (ABILITY_DJINNI_FIRST,
    // MESSAGE_ARGUMENT_ABILITY).
    MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(element * DJINN_PER_ELEMENT + djinn +
                             ABILITY_DJINNI_FIRST,
                         MESSAGE_ARGUMENT_ABILITY);

    text = Message_DecodeMode1(element + MsgVenusDjinnJoined);

    JOINED_QUEUE_TEXT(window, text, 36, 2, zero);

    Sound_PlaySongFar(MUS_JOINED);

    do {
      Oam_AddSpriteAtDepth(entry, 250);
      WaitFrames(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    WaitFrames(1);
    VramCache_FreeEntry(handle);
  }
}

/// Returns the portrait character of party member `member` (0-8): from
/// gCharacter_ChildSpriteTable during the prologue, from
/// gCharacter_SpriteTable otherwise; 0 for any other member.
s32 Party_LookupCharacterValueByFlag32(u32 index) {
  if (index > 8) {
    return 0;
  }
  if (GameFlag_GetFar(FLAG_CHILDHOOD) == 0) {
    return gCharacter_SpriteTable[index];
  }
  return gCharacter_ChildSpriteTable[index];
}

/// Shows that a member joined the party while the join fanfare (sound 81)
/// plays, until it ends or A, B, L or R is pressed. The message is
/// `<member> joined your party.`, with the member's portrait. The field's
/// party events reach it by far call (overworld/event_commands/script.c).
void Party_ShowJoinedMessage(s32 member) {
  struct WindowWork *work = (struct WindowWork *)*gWindowWork;
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
        Portrait_GetForSpeaker(Party_LookupCharacterValueByFlag32(member)), 0,
        &handle, &palette, 14, 0);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = entry;
    *p++ = 0;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;
    work->voiceSound = 0;
    work->voiceCountdown = 0;
    MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
    JOINED_QUEUE_TEXT(window, Message_DecodeMode1(MsgJoinedParty), 36, 2, 0);
    Sound_PlaySongFar(MUS_JOINED);
    do {
      Oam_AddSpriteAtDepth(entry, 250);
      WaitFrames(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
    Window_Close(window, 2);
    WaitFrames(1);
    VramCache_FreeEntry(handle);
  }
}

/// Shows that two members joined the party while the join fanfare (sound 81)
/// plays, until it ends or A, B, L or R is pressed. The message is
/// `<first> and <second> joined your party.`, with both portraits.
void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  u8 *work = gWindowWork[0];
  struct Window *window;
  s32 secondOam[3];
  s32 firstOam[3];
  s32 *firstEntry;
  s32 *secondEntry;
  s32 firstHandle, palette, secondHandle;
  u32 zero;
  s32 *p;

  window = NULL;
  firstEntry = firstOam;
  window = Window_Create(1, 1, 28, 5, 0);
  zero = 0;

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
        Portrait_GetForSpeaker(Party_LookupCharacterValueByFlag32(first)), 0,
        &firstHandle, &palette, 14, zero);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = firstEntry;
    *p++ = zero;
    *p++ = 0x800c000c;
    *p = palette | 0xe000;

    secondEntry = secondOam;
    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_LookupCharacterValueByFlag32(second)), 0,
        &secondHandle, &palette, 15, zero);
    p = secondEntry;
    *p++ = zero;
    *p++ = 0x802c000c;
    *p = palette | 0xf000;

    ((struct WindowWork *)work)->voiceSound = zero;
    ((struct WindowWork *)work)->voiceCountdown = zero;

    MessageArgument_Push(first, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(second, MESSAGE_ARGUMENT_ACTOR);

    JOINED_QUEUE_TEXT(window, Message_DecodeMode1(MsgPairJoinedParty), 68, 2,
                      zero);

    Sound_PlaySongFar(MUS_JOINED);

    do {
      Oam_AddSpriteAtDepth(firstEntry, 250);
      Oam_AddSpriteAtDepth(secondEntry, 250);
      WaitFrames(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    WaitFrames(1);
    VramCache_FreeEntry(firstHandle);
    VramCache_FreeEntry(secondHandle);
  }
}

#undef JOINED_QUEUE_TEXT
