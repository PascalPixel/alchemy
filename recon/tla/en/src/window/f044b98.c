#include "types.h"
#include "io_reg.h"
#include "input.h"
#include "ram_buffer.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "task.h"
#include "window.h"
#include "text/msg_ids.h"

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
s32 Func_08044b80(u32 index);
s32 Portrait_GetForSpeaker(u32 value);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);
void MessageArgument_Push(u32 value, u32 kind);
s32 Message_DecodeMode1(s32 message);
// UB: Message_Start is called with five of its six arguments, so the mode it
// reads is whatever the stack word after the fifth holds (the 0 left there as
// UiGlyph_LoadEntryWithPalette's last argument); UBFIX declares it whole and
// passes 0.
#ifdef UBFIX
s32 Message_Start(struct Window *window, s32 entry, s32 x, s32 y, u16 *bounds,
                  s32 mode);
#else
s32 Message_Start();
#endif
void Sound_PlayFar(s32 cue);

void Func_08044b98(s32 member) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  /* FAKEMATCH(dead): window initialized to NULL, which Window_Create's result
   * replaces at once, so the zero the later stores share is built after that
   * call; as an uninitialized declaration, 39 of 95 instructions differ in
   * tla-en. */
  struct Window *window = NULL;
  s32 sprite[3];
  s32 *entry = sprite;
  s32 tile, slot;
  s32 *p;

  window = Window_Create(2, 1, 26, 5, 0);
  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);
    work->dirty = 1;
    UiGlyph_LoadEntryWithPalette(Portrait_GetForSpeaker(Func_08044b80(member)),
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
      // UB: Oam_AddSpriteAtDepth returns nothing; UBFIX calls it through its
      // declared type.
#ifdef UBFIX
      Oam_AddSpriteAtDepth(entry, 250);
#else
      /* FAKEMATCH(wrapper): called through a cast to the s32 function the
       * bytes show, which sets r1 to 250 before moving the entry to r0; as a
       * call by its void prototype, 2 of 95 instructions differ in tla-en. */
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
