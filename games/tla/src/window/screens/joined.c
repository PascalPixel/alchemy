/// @file
/// The joined-party and found-Djinni messages.
// Part of screens.c.

#include "../screens_internal.h"
#include "module/window/screens.h"
#include "module/window/window.h"

s32 Func_08044a54(void) { return 0; }

// The window that announces a Djinni, wider in French and Italian.
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DJINN_JOIN_X 1
#define DJINN_JOIN_WIDTH 28
#else
#define DJINN_JOIN_X 2
#define DJINN_JOIN_WIDTH 26
#endif

// UB: Message_Start is called with five of its six arguments, so the flags it
// reads are whatever the stack word after the fifth holds (the 0 left there as
// Icon_LoadWithPalette's last argument); UBFIX declares it whole and
// passes 0.
#ifdef UBFIX
struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags);
#else
s32 Message_Start();
#endif

void Func_08044a58(s32 member, s32 element, s32 djinn) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 name;
  struct Window *window;
  s32 sprite[3];
  s32 *entry;
  s32 tile, slot;
  s32 id;
  s32 text;
  // One zero for every 0 stored or passed below, as the Broken Seal's form
  // keeps it; as literal 0s, 58 of 124 instructions differ in tla-en.
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
    Icon_LoadWithPalette(id, zero, &slot, &tile, 14, zero);

    // The portrait as a bare OAM entry, pushed at depth 250 each frame: a link
    // word, attributes 0 and 1 (32x32 at y 12) and attribute 2, its tile with
    // palette 14.
    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
    *p = tile | (14 << 12);

    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

    MessageArgument_Push(member, 1);
    MessageArgument_Push(element * 20 + djinn + 300, 4);

    text = Message_DecodeMode1(element + (s32)MsgFoundVenusDjinni);

#ifdef UBFIX
    Message_Start(window, text, 36, 2, NULL, 0);
#else
    Message_Start(window, text, 36, 2, zero);
#endif

    Sound_PlayFar(81);

    do {
      // UB: Oam_AddSpriteAtDepth returns nothing but is called as an s32
      // function; UBFIX calls it through its declared type.
#ifdef UBFIX
      Oam_AddSpriteAtDepth(entry, 250);
#else
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry, 250);
#endif
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    Resource_ResetEntry(slot);
  }
}

s32 Func_08044b80(u32 index) {
  if (index > 8)
    return 0;
  return gCharacter_SpriteTable[index];
}

void Func_08044b98(s32 member) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct Window *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 tile, slot;
  s32 *p;

  window = Window_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);
    work->dirty = 1;
    Icon_LoadWithPalette(Portrait_GetForSpeaker(Func_08044b80(member)),
                                 0, &slot, &tile, 14, 0);
    // The portrait as a bare OAM entry, pushed at depth 250 each frame: a link
    // word, attributes 0 and 1 (32x32 at x 20, y 12) and attribute 2, its tile
    // with palette 14.
    p = entry;
    *p++ = 0;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = tile | (14 << 12);
    work->speakerVoiceSound = 0;
    work->speakerVoiceCountdown = 0;
    MessageArgument_Push(member, 1);
#ifdef UBFIX
    Message_Start(window, Message_DecodeMode1((s32)MsgJoinedParty), 36, 2, NULL,
                  0);
#else
    Message_Start(window, Message_DecodeMode1((s32)MsgJoinedParty), 36, 2, 0);
#endif
    Sound_PlayFar(81);
    do {
      // UB: Oam_AddSpriteAtDepth returns nothing but is called as an s32
      // function; UBFIX calls it through its declared type.
#ifdef UBFIX
      Oam_AddSpriteAtDepth(entry, 250);
#else
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry, 250);
#endif
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
    Window_Close(window, 2);
    wait(1);
    Resource_ResetEntry(slot);
  }
}

void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct Window *window;
  s32 sprite2[3];
  s32 sprite1[3];
  s32 *entry1;
  s32 *entry2;
  s32 handle1, palette, handle2;
  s32 *p;

  window = NULL;
  entry1 = sprite1;
  window = Window_Create(1, 1, 28, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 8, 0, 4, 4);

    work->dirty = 1;

    Icon_LoadWithPalette(Portrait_GetForSpeaker(Func_08044b80(first)),
                                 0, &handle1, &palette, 14, 0);
    // The portraits as bare OAM entries, pushed at depth 250 each frame: a
    // link word, attributes 0 and 1 (32x32 at y 12, x 12 and 44) and
    // attribute 2, the tile with palette 14 or 15.
    p = entry1;
    *p++ = 0;
    *p++ = 0x800c000c;
    *p = palette | 0xe000;

    entry2 = sprite2;
    Icon_LoadWithPalette(Portrait_GetForSpeaker(Func_08044b80(second)),
                                 0, &handle2, &palette, 15, 0);
    p = entry2;
    *p++ = 0;
    *p++ = 0x802c000c;
    *p = palette | 0xf000;

    work->speakerVoiceSound = 0;
    work->speakerVoiceCountdown = 0;

    MessageArgument_Push(first, 1);
    MessageArgument_Push(second, 1);

#ifdef UBFIX
    Message_Start(window, Message_DecodeMode1((s32)MsgPairJoinedParty), 68, 2,
                  NULL, 0);
#else
    Message_Start(window, Message_DecodeMode1((s32)MsgPairJoinedParty), 68, 2,
                  0);
#endif

    Sound_PlayFar(81);

    do {
      // UB: Oam_AddSpriteAtDepth returns nothing but is called as an s32
      // function; UBFIX calls it through its declared type.
#ifdef UBFIX
      Oam_AddSpriteAtDepth(entry1, 250);
      Oam_AddSpriteAtDepth(entry2, 250);
#else
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry1, 250);
      ((s32 (*)(void *, s32))Oam_AddSpriteAtDepth)(entry2, 250);
#endif
      wait(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    Window_Close(window, 2);
    wait(1);
    Resource_ResetEntry(handle1);
    Resource_ResetEntry(handle2);
  }
}

// The window that lists the Djinn a summon needs is wider and taller in
// Spanish and French, whose text gives each Djinn count a line of its own.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define TO_SUMMON_WINDOW_X 2
#define TO_SUMMON_WINDOW_WIDTH 25
#define TO_SUMMON_WINDOW_HEIGHT 9
#else
#define TO_SUMMON_WINDOW_X 3
#define TO_SUMMON_WINDOW_WIDTH 24
#define TO_SUMMON_WINDOW_HEIGHT 7
#endif

extern u8 MsgCanNowSummon[], MsgToSummon[];

void Func_08044dc8(s32 summon) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  struct Window *window;
  s32 entries;
  u32 resource;
  // Unused: gives the stack frame its 28 bytes.
  UNUSED s32 unused[7];
  s32 counts[4];
  const u8 *djinn;
  s32 i;
  s32 n;

  window = NULL;
  window = Window_Create(4, 1, 22, 5, 0);
  if (window == NULL)
    return;
  work->dirty = 1;
  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  MessageArgument_Push(Data_0805f676[summon & 31], 4);
  entries = Message_DecodeMode1((s32)MsgCanNowSummon);
  // UB: five of Message_Start's six arguments, as above; the flags are the
  // stack word resource takes, which nothing sets. UBFIX passes 0.
#ifdef UBFIX
  Message_Start(window, entries, 0, 2, NULL, 0);
#else
  Message_Start(window, entries, 0, 2, 0);
#endif
  Sound_PlayFar(81);
  do
    wait(1);
  while (Sound_GetBgmRestoreDelayFar() != 0 && (gInput.pressed & 0x303) == 0);
  Window_Close(window, 2);
  wait(1);

  window = Window_Create(TO_SUMMON_WINDOW_X, 1, TO_SUMMON_WINDOW_WIDTH,
                         TO_SUMMON_WINDOW_HEIGHT, 0);
  if (window == NULL)
    return;
  MessageArgument_Push(Data_0805f676[summon & 31], 4);
  entries = Message_DecodeMode1((s32)MsgToSummon);
  djinn = (const u8 *)SummonDefinition_Get(summon) + 4;
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
    MessageArgument_Push(most, 5);
    Func_0803cf6c((s32)MsgSummonVenus + best);
    counts[best] = 0;
  }
  Func_0803cf6c((s32)MsgSummonVenus + 4);
#ifdef UBFIX
  Message_Start(window, entries, 4, 2, NULL, 0);
#else
  Message_Start(window, entries, 4, 2, 0);
#endif
  do
    wait(1);
  while ((gInput.pressed & 0x303) == 0);
  Window_Close(window, 2);
  wait(1);
  Resource_ResetEntry(resource);
}

#undef DJINN_JOIN_X
#undef DJINN_JOIN_WIDTH
#undef TO_SUMMON_WINDOW_X
#undef TO_SUMMON_WINDOW_WIDTH
#undef TO_SUMMON_WINDOW_HEIGHT
