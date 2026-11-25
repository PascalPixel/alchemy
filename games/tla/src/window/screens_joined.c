/// @file
/// The joined-party and found-Djinni messages.
// Part of screens.c.

#include "window_internal.h"
#include "sounds.h"
#include "window/screens.h"
#include "window/window.h"

/// Returns 0; only the far-call table refers to it.
s32 Screens_ReturnZero(void) { return 0; }

/// The window that announces a Djinni, wider in French and Italian.
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DJINN_JOIN_X 1
#define DJINN_JOIN_WIDTH 28
#else
#define DJINN_JOIN_X 2
#define DJINN_JOIN_WIDTH 26
#endif

// UB: Message_Start is called with five of its six arguments, so the flags it
// reads are whatever the stack word after the fifth holds (the 0 left there as
// Icon_LoadWithPalette's last argument), as a call with no declaration in
// scope compiles; UBFIX declares it whole and passes 0.
#ifdef UBFIX
struct Message *Message_Start(struct Window *window, s32 entry, s32 x, s32 y,
                              u16 *spacing, s32 flags);
#endif

/// Shows that a Djinni joined a party member while the join jingle
/// (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed.
void Djinn_ShowJoinedMessage(s32 member, s32 element, s32 djinn) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  s32 djinniSprite;
  struct Window *window;
  s32 sprite[3];
  s32 *entry;
  s32 tile, slot;
  s32 id;
  s32 text;
  s32 *p;
  u16 *spacing = NULL;

  window = NULL;
  djinniSprite = gDjinn_ElementSprites[element & 3];
  entry = sprite;
  window = Window_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);

    state->dirty = 1;

    id = Portrait_GetForSpeaker(djinniSprite);
    Icon_LoadWithPalette(id, 0, &slot, &tile, PORTRAIT_PALETTE, 0);

    // The portrait as a bare OAM entry, pushed each frame: a link word,
    // attributes 0 and 1 (32x32 at y 12) and attribute 2, its tile in
    // PORTRAIT_PALETTE.
    p = entry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | OBJ_X(DJINN_JOIN_X * 8 + 4) | 12;
    *p = tile | OBJ_PALETTE(PORTRAIT_PALETTE);

    state->speakerVoiceSound = 0;
    state->speakerVoiceCountdown = 0;

    MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(element * DJINN_PER_ELEMENT + djinn +
                             ABILITY_DJINNI_FIRST,
                         MESSAGE_ARGUMENT_ABILITY);

    text = Message_DecodeAndClearArguments(element + (s32)MsgFoundVenusDjinni);

#ifdef UBFIX
    Message_Start(window, text, 36, 2, spacing, 0);
#else
    Message_Start(window, text, 36, 2, spacing);
#endif

    Sound_PlayFar(MUS_A_NEW_ALLY);

    do {
      Oam_AddSpriteAtDepth(entry, OAM_DEPTH_FRONT - 5);
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    VramCache_FreeEntry(slot);
  }
}

/// Returns the sprite of party member `index` (ISAAC to PIERS), from
/// gCharacter_SpriteTable, by which Portrait_GetForSpeaker finds its portrait;
/// 0 past 8. Index 8 reads the halfword after the table's eight.
s32 Party_GetMemberPortraitSprite(u32 index) {
  if (index > 8)
    return 0;
  return gCharacter_SpriteTable[index];
}

/// Shows that a party member joined the party while the join jingle
/// (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed.
void Party_ShowJoinedMessage(s32 member) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  struct Window *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 tile, slot;
  s32 *p;

  window = Window_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);
    state->dirty = 1;
    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_GetMemberPortraitSprite(member)), 0, &slot,
        &tile, PORTRAIT_PALETTE, 0);
    // The portrait as a bare OAM entry, pushed each frame: a link word,
    // attributes 0 and 1 (32x32 at x 20, y 12) and attribute 2, its tile in
    // PORTRAIT_PALETTE.
    p = entry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | OBJ_X(2 * 8 + 4) | 12;
    *p = tile | OBJ_PALETTE(PORTRAIT_PALETTE);
    state->speakerVoiceSound = 0;
    state->speakerVoiceCountdown = 0;
    MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
#ifdef UBFIX
    Message_Start(window, Message_DecodeAndClearArguments((s32)MsgJoinedParty),
                  36, 2, NULL, 0);
#else
    Message_Start(window, Message_DecodeAndClearArguments((s32)MsgJoinedParty),
                  36, 2, 0);
#endif
    Sound_PlayFar(MUS_A_NEW_ALLY);
    do {
      Oam_AddSpriteAtDepth(entry, OAM_DEPTH_FRONT - 5);
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
    Window_Close(window, 2);
    wait(1);
    VramCache_FreeEntry(slot);
  }
}

/// Shows that two party members joined the party while the join jingle
/// (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed.
void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;
  struct Window *window;
  s32 secondSprite[3];
  s32 firstSprite[3];
  s32 *firstEntry;
  s32 *secondEntry;
  s32 firstSlot, palette, secondSlot;
  s32 *p;

  window = NULL;
  firstEntry = firstSprite;
  window = Window_Create(1, 1, 28, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 8, 0, 4, 4);

    state->dirty = 1;

    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_GetMemberPortraitSprite(first)), 0,
        &firstSlot, &palette, PORTRAIT_PALETTE, 0);
    // The portraits as bare OAM entries, pushed each frame: a link word,
    // attributes 0 and 1 (32x32 at y 12, x 12 and 44) and attribute 2, the
    // tile in PORTRAIT_PALETTE or the palette after it.
    p = firstEntry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | OBJ_X(12) | 12;
    *p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);

    secondEntry = secondSprite;
    Icon_LoadWithPalette(
        Portrait_GetForSpeaker(Party_GetMemberPortraitSprite(second)), 0,
        &secondSlot, &palette, PORTRAIT_PALETTE + 1, 0);
    p = secondEntry;
    *p++ = 0;
    *p++ = OBJ_SIZE_32X32 | OBJ_X(44) | 12;
    *p = palette | OBJ_PALETTE(PORTRAIT_PALETTE + 1);

    state->speakerVoiceSound = 0;
    state->speakerVoiceCountdown = 0;

    MessageArgument_Push(first, MESSAGE_ARGUMENT_ACTOR);
    MessageArgument_Push(second, MESSAGE_ARGUMENT_ACTOR);

#ifdef UBFIX
    Message_Start(window,
                  Message_DecodeAndClearArguments((s32)MsgPairJoinedParty), 68,
                  2, NULL, 0);
#else
    Message_Start(window,
                  Message_DecodeAndClearArguments((s32)MsgPairJoinedParty), 68,
                  2, 0);
#endif

    Sound_PlayFar(MUS_A_NEW_ALLY);

    do {
      Oam_AddSpriteAtDepth(firstEntry, OAM_DEPTH_FRONT - 5);
      Oam_AddSpriteAtDepth(secondEntry, OAM_DEPTH_FRONT - 5);
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    VramCache_FreeEntry(firstSlot);
    VramCache_FreeEntry(secondSlot);
  }
}

/// The window that lists the Djinn a summon needs is wider and taller in
/// Spanish and French, whose text gives each Djinn count a line of its own.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define TO_SUMMON_WINDOW_X 2
#define TO_SUMMON_WINDOW_WIDTH 25
#define TO_SUMMON_WINDOW_HEIGHT 9
#else
#define TO_SUMMON_WINDOW_X 3
#define TO_SUMMON_WINDOW_WIDTH 24
#define TO_SUMMON_WINDOW_HEIGHT 7
#endif

/// Announces a summon the party can now use: "... can now summon ..."
/// (MsgCanNowSummon) until the jingle ends or a button is pressed, then
/// "To summon" (MsgToSummon) with its two largest Djinn counts, until a button
/// is pressed.
void Summon_ShowLearnedMessage(s32 summon) {
  struct WindowState *state = Ram_HeapSlots->windowState;
  struct Window *window;
  s32 entries;
  u32 resource;
  /* LEFTOVER(frame): an unused local the frame reserves and no instruction
   * touches. */
  UNUSED s32 unused[7];
  s32 counts[4];
  const u8 *djinn;
  s32 i;
  s32 n;

  window = NULL;
  window = Window_Create(4, 1, 22, 5, 0);
  if (window == NULL)
    return;
  state->dirty = 1;
  state->speakerVoiceSound = 0;
  state->speakerVoiceCountdown = 0;
  MessageArgument_Push(gSummon_Abilities[summon & (SUMMON_END - 1)],
                       MESSAGE_ARGUMENT_ABILITY);
  entries = Message_DecodeAndClearArguments((s32)MsgCanNowSummon);
  // UB: five of Message_Start's six arguments, as above; the flags are the
  // stack word resource takes, which nothing sets. UBFIX passes 0.
#ifdef UBFIX
  Message_Start(window, entries, 0, 2, NULL, 0);
#else
  Message_Start(window, entries, 0, 2, 0);
#endif
  Sound_PlayFar(MUS_A_NEW_ALLY);
  do
    wait(1);
  while (Sound_GetBgmRestoreDelayFar() != 0 &&
         (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
  Window_Close(window, 2);
  wait(1);

  window = Window_Create(TO_SUMMON_WINDOW_X, 1, TO_SUMMON_WINDOW_WIDTH,
                         TO_SUMMON_WINDOW_HEIGHT, 0);
  if (window == NULL)
    return;
  MessageArgument_Push(gSummon_Abilities[summon & (SUMMON_END - 1)],
                       MESSAGE_ARGUMENT_ABILITY);
  entries = Message_DecodeAndClearArguments((s32)MsgToSummon);
  djinn = SummonDefinition_Get(summon)->djinnRequired;
  for (i = 0; i < 4; i++)
    counts[i] = djinn[i];
  for (n = 0; n < 2; n++) {
    s32 best = 0;
    s32 most = 0;

    for (i = 0; i < 4; i++) {
      if (most < counts[i]) {
        best = i;
        most = counts[i];
      }
    }
    // MsgSummonVenus is the first of four Djinn counts (Venus, Mercury, Mars,
    // Jupiter); the message four past it ends their line.
    MessageArgument_Push(most, MESSAGE_ARGUMENT_NUMBER);
    Message_DecodeAppend((s32)MsgSummonVenus + best);
    counts[best] = 0;
  }
  Message_DecodeAppend((s32)MsgSummonVenus + 4);
#ifdef UBFIX
  Message_Start(window, entries, 4, 2, NULL, 0);
#else
  Message_Start(window, entries, 4, 2, 0);
#endif
  do
    wait(1);
  while ((gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
  Window_Close(window, 2);
  wait(1);
  // UB: resource is never set, so this frees whichever VRAM cache entry its
  // stack word names; UBFIX frees nothing, as nothing was loaded.
#ifndef UBFIX
  VramCache_FreeEntry(resource);
#endif
}

#undef DJINN_JOIN_X
#undef DJINN_JOIN_WIDTH
#undef TO_SUMMON_WINDOW_X
#undef TO_SUMMON_WINDOW_WIDTH
#undef TO_SUMMON_WINDOW_HEIGHT
