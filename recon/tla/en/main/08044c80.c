// Under -mthumb-slow-loads (2026-10-26): differ 6 of 135 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Draft of Party_ShowPairJoinedMessage: differ 8 of 135 (tla-en, 2026-10-18).
// Its ☀️ twin is games/tbs/src/window/battle_command.c's function of the
// same name. It opens a message window with the two joining characters'
// portraits, plays the join jingle and waits for it, or for A, B or a
// shoulder button.
//
// Left: instruction order only (a constant build and two argument copies sit
// one place apart).

#include "callback_scheduler.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "render_input.h"
#include "resource.h"
#include "ram_buffer.h"
#include "window.h"
#include "input.h"

extern u8 MsgPairJoinedParty[];

struct UiWindow *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct UiWindow *, s32, s32, s32, s32);
s32 Party_LookupCharacterValueByFlag32(s32);
s32 UiText_GetSpeakerPortrait(s32);
void UiGlyph_LoadEntryWithPalette(s32, s32, s32 *, s32 *, s32, s32);
void UiWork_PushValueSlot(s32, s32);
s32 UiText_BuildRenderEntriesMode1(s32);
s32 UiText_QueueRenderEntries(struct UiWindow *, s32, s32, s32, s32);
void Sound_PlayFar(s32);
void WaitFrames(s32);
s32 Sound_GetBgmRestoreDelayFar(void);
void UiWork_Finalize(struct UiWindow *, s32);

void Party_ShowPairJoinedMessage(s32 first, s32 second) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
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
  /* FAKEMATCH: the null window is also the style argument (ROM passes its
   * register). */
  window = UiWindow_Create(1, 1, 28, 5, (s32)window);
  zero = 0;

  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

    work->dirty = 1;

    UiGlyph_LoadEntryWithPalette(
        UiText_GetSpeakerPortrait(Party_LookupCharacterValueByFlag32(first)),
        0, &handle1, &palette, 14, zero);
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

    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

    UiWork_PushValueSlot(first, 1);
    UiWork_PushValueSlot(second, 1);

    UiText_QueueRenderEntries(
        window, UiText_BuildRenderEntriesMode1((s32)MsgPairJoinedParty), 68, 2,
        zero);

    Sound_PlayFar(81);

    do {
      Oam_AddSpriteAtDepth(entry1, 250);
      Oam_AddSpriteAtDepth(entry2, 250);
      WaitFrames(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle1);
    Resource_ResetEntry(handle2);
  }
}
