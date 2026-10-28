// Draft, differ 4 of 124 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Near miss, differ 10 of 124 (14 before Message_Start was
// declared returning s32, as msgwait.c defines it), 2026-10-25: The Broken Seal's
// Djinn_ShowJoinedMessage on this game's window tiles, its window at x 2 and
// width 26, the element's "found the Djinni" message (MsgFoundVenusDjinni,
// message 46 in the English catalog, still unnamed there) and gInput's
// pressed keys. Five reordered pairs remain, most a load the native issues
// one row earlier: the element table's pool load, the palette's stack
// reload, the key table and the arguments of Message_Start,
// which the native sets in the order r3, r2, r0. A device search found
// nothing; it needs message 46 named in all six catalogs before it can link.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "window.h"
#include "input.h"
#include "io_reg.h"
#include "render_input.h"

extern const s16 gData_0805f65e[];
extern u8 MsgFoundVenusDjinni[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2,
                              u32 y2);
s32 Portrait_GetForSpeaker(s32 key);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);
void MessageArgument_Push(u32 value, u32 kind);
s32 Message_DecodeMode1(s32 arg0);
s32 Message_Start(struct Window *, s32, s32, s32, s32);
void Sound_PlayFar(u32);
s32 Sound_GetBgmRestoreDelayFar(void);
void Oam_AddSpriteAtDepth(void *entry, s32 priority);
void Window_Close(struct Window *work, s32 release);
s32 Resource_ResetEntry(u32 slot);

void Func_08044a58(s32 pc, s32 element, s32 djinn) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
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
  name = gData_0805f65e[element & 3];
  entry = sprite;
  window = Window_Create(2, 1, 26, 5, 0);

  if (window != NULL) {
    Window_DrawDividerLine(window, 4, 0, 4, 4);

    work->dirty = 1;

    id = Portrait_GetForSpeaker(name);
    UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;

    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

    MessageArgument_Push(pc, 1);
    MessageArgument_Push(element * 20 + djinn + 300, 4);

    text = Message_DecodeMode1(element + (s32)MsgFoundVenusDjinni);

    Message_Start(window, text, 36, 2, zero);

    Sound_PlayFar(81);

    do {
      Oam_AddSpriteAtDepth(entry, 250);
      WaitFrames(1);
    } while (Sound_GetBgmRestoreDelayFar() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | 0x300)) == 0);

    Window_Close(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}
