/// @file
/// The Colosso choice and the joined-party messages.
// Part of battle_command.c.

#include "../battle_command_internal.h"

// UB: the joined messages pass five of UiText_QueueRenderEntries' six
// arguments, so its flags are whatever the stack slot of the sixth holds;
// UBFIX passes 0, as the window object's own messages do.
#ifdef UBFIX
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing)                        \
  UiText_QueueRenderEntries(window, entry, x, y, (u16 *)(spacing), 0)
#else
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing)                        \
  UiText_QueueRenderEntries(window, entry, x, y, spacing)
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
/// cancelled with B. The Colosso scenes ask it (scene/common/colosso_solo.h).
/// It takes a VRAM slot for the hand and frees it before returning.
s32 PartyTalkMenu_Choose(s32 unused) {
  struct UiWindow *window;
  struct ShopCursor cursor;
  s32 slot;
  s32 row;
  s32 moved;
  u16 *palette;

  moved = 1;
  window = UiWindow_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, 2);
  UiText_DrawCharacterAtOffset(MsgTalkChoice, window, 8, 0);
  UiText_DrawCharacterAtOffset(MsgTalkChoice + 1, window, 8, 16);
  UiText_DrawCharacterAtOffset(MsgTalkChoice + 2, window, 8, 32);
  slot = Resource_FindFreeEntry();
  row = 0;
  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    VramCache_Load(slot, 128, gResource_FixedBlockBTiles);
    cursor.anchor = RenderOutput_Create(slot, OUTPUT_SIZE_16X16, window, 0, 0);
    ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 - 3,
                                       window->y * 8 + 9);
  }
  // The choice under the cursor flashes between palette 15 and palette 14,
  // a copy of it with colour 4 grey.
  palette = (u16 *)&BG_PLTT_COLOR(14, 0);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 0), palette, DMA_COPY32(8),
          REG_DMA3);
  palette[4] = 0x6318;
  while (1) {
    UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                                  14);
    WaitFrames(1);
    UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1,
                                  15);
    if (moved) {
      moved = 0;
      Shop_SetCursorFar(&cursor, window->x * 8 - 3,
                        (window->y + row * 2) * 8 + 9, 3);
    }
    ShopCursor_AdvanceFar(&cursor);
    if (gKeysRepeat & KEY_UP) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      row--;
      moved = 1;
      if (row == -1)
        row = 2;
    }
    if (gKeysRepeat & KEY_DOWN) {
      AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
      row++;
      moved = 1;
      if (row == 3)
        row = 0;
    }
    if (gKeysRepeat & KEY_B) {
      AudioCommand_PlayFar(SE_MENU_CANCEL);
      row = -1;
      break;
    }
    if (gKeysRepeat & KEY_A) {
      AudioCommand_PlayFar(SE_MENU_CONFIRM);
      break;
    }
  }
  UiWork_Finalize(window, 2);
  WaitFrames(1);
  Resource_ResetEntry(slot);
  return row;
}

/// Shows that a Djinni joined a member while the join fanfare (sound 81)
/// plays, until it ends or A, B, L or R is pressed. The message is
/// `The <element> Djinni <name> joined <member>.`, with the element's Djinni
/// portrait. `djinn` is the Djinni's number in its element. The field's
/// Djinni events reach it by far call (object/psynergy_effects/djinni.c). It
/// closes the window at once (UiWork_Finalize with instant set), waits a
/// frame and frees the portrait's VRAM slot.
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn) {
  struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
  s32 name;
  struct UiWindow *window;
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
  window = UiWindow_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

    work->dirty = 1;

    id = UiText_GetSpeakerPortrait(name);
    UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2, its
    // tile with OBJ palette 14.
    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
    *p = palette | 0xe000;

    // No voice blips while the message types.
    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

    // The message's arguments: the member (MESSAGE_ARGUMENT_ACTOR) and the
    // Djinni's name, its ability (ABILITY_DJINNI_FIRST,
    // MESSAGE_ARGUMENT_ABILITY).
    UiWork_PushValueSlot(member, MESSAGE_ARGUMENT_ACTOR);
    UiWork_PushValueSlot(element * DJINN_PER_ELEMENT + djinn +
                             ABILITY_DJINNI_FIRST,
                         MESSAGE_ARGUMENT_ABILITY);

    text = UiText_BuildRenderEntriesMode1(element + MsgVenusDjinnJoined);

    JOINED_QUEUE_TEXT(window, text, 36, 2, zero);

    AudioCommand_PlayFar(MUS_JOINED);

    do {
      Runtime_PushSlotEntry(entry, 250);
      WaitFrames(1);
    } while (AudioCommand_GetStateByteFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}

/// Returns the portrait character of party member `member` (0-8): from
/// gCharacter_ChildSpriteTable during the prologue, from
/// gCharacter_SpriteTable otherwise; 0 for any other member.
s32 Party_LookupCharacterValueByFlag32(u32 index) {
  if (index > 8) {
    return 0;
  }
  if (GameFlag_TestFar(FLAG_CHILDHOOD) == 0) {
    return gCharacter_SpriteTable[index];
  }
  return gCharacter_ChildSpriteTable[index];
}

/// Shows that a member joined the party while the join fanfare (sound 81)
/// plays, until it ends or A, B, L or R is pressed. The message is
/// `<member> joined your party.`, with the member's portrait. The field's
/// party events reach it by far call (object/psynergy_effects/events.c).
void Party_ShowJoinedMessage(s32 member) {
  struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
  struct UiWindow *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 palette, handle;
  s32 *p;

  window = UiWindow_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
    work->dirty = 1;
    UiGlyph_LoadEntryWithPalette(
        UiText_GetSpeakerPortrait(Party_LookupCharacterValueByFlag32(member)),
        0, &handle, &palette, 14, 0);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = entry;
    *p++ = 0;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;
    work->speakerVoiceSound = 0;
    work->speakerVoiceCountdown = 0;
    UiWork_PushValueSlot(member, MESSAGE_ARGUMENT_ACTOR);
    JOINED_QUEUE_TEXT(window, UiText_BuildRenderEntriesMode1(MsgJoinedParty),
                      36, 2, 0);
    AudioCommand_PlayFar(MUS_JOINED);
    do {
      Runtime_PushSlotEntry(entry, 250);
      WaitFrames(1);
    } while (AudioCommand_GetStateByteFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}

/// Shows that two members joined the party while the join fanfare (sound 81)
/// plays, until it ends or A, B, L or R is pressed. The message is
/// `<first> and <second> joined your party.`, with both portraits.
void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  u8 *work = gWindowWork[0];
  struct UiWindow *window;
  s32 sprite2[3];
  s32 sprite1[3];
  s32 *entry1;
  s32 *entry2;
  s32 handle1, palette, handle2;
  u32 zero;
  s32 *p;

  window = NULL;
  entry1 = sprite1;
  window = UiWindow_Create(1, 1, 28, 5, 0);
  zero = 0;

  if (window != NULL) {
    // BUG: from (8, 0) to (4, 4) is neither across nor down, so
    // UiWindow_DrawDividerLine draws nothing and the window has no divider.
    // XXX: likely meant (8, 0, 8, 4), because the single joins draw
    // (4, 0, 4, 4) half a tile before their text at x 36, as column 8 is
    // before this one's at x 68.
    UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

    ((struct UiRenderWork *)work)->dirty = 1;

    UiGlyph_LoadEntryWithPalette(
        UiText_GetSpeakerPortrait(Party_LookupCharacterValueByFlag32(first)), 0,
        &handle1, &palette, 14, zero);
    // The portrait as a bare OAM entry, pushed onto draw layer 250 each frame:
    // a link word, attributes 0 and 1 (32x32 at y 12) and attribute 2.
    p = entry1;
    *p++ = zero;
    *p++ = 0x800c000c;
    *p = palette | 0xe000;

    entry2 = sprite2;
    UiGlyph_LoadEntryWithPalette(
        UiText_GetSpeakerPortrait(Party_LookupCharacterValueByFlag32(second)),
        0, &handle2, &palette, 15, zero);
    p = entry2;
    *p++ = zero;
    *p++ = 0x802c000c;
    *p = palette | 0xf000;

    ((struct UiRenderWork *)work)->speakerVoiceSound = zero;
    ((struct UiRenderWork *)work)->speakerVoiceCountdown = zero;

    UiWork_PushValueSlot(first, MESSAGE_ARGUMENT_ACTOR);
    UiWork_PushValueSlot(second, MESSAGE_ARGUMENT_ACTOR);

    JOINED_QUEUE_TEXT(window,
                      UiText_BuildRenderEntriesMode1(MsgPairJoinedParty), 68, 2,
                      zero);

    AudioCommand_PlayFar(MUS_JOINED);

    do {
      Runtime_PushSlotEntry(entry1, 250);
      Runtime_PushSlotEntry(entry2, 250);
      WaitFrames(1);
    } while (AudioCommand_GetStateByteFar() != 0 &&
             (gKeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle1);
    Resource_ResetEntry(handle2);
  }
}

#undef JOINED_QUEUE_TEXT
