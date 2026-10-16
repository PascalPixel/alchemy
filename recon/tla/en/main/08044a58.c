// Near miss, differ 14 of 124, 2026-10-15: The Broken Seal's
// Djinn_ShowJoinedMessage on this game's window tiles, its window at x 2 and
// width 26, the element's "found the Djinni" message (MsgFoundVenusDjinni,
// message 46 in the English catalog, still unnamed there) and gInput's
// pressed keys. Seven reordered pairs remain, most a load the native issues
// one row earlier: the element table's pool load, the palette's stack
// reload, the key table and the arguments of UiText_QueueRenderEntries,
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

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void UiWindow_DrawDividerLine(struct UiWindow *win, u32 x1, u32 y1, u32 x2,
                              u32 y2);
s32 Localization_LookupEntryId(s32 key);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);
void UiWork_PushValueSlot(u32 value, u32 kind);
s32 UiText_BuildRenderEntriesMode1(s32 arg0);
void UiText_QueueRenderEntries(struct UiWindow *, s32, s32, s32, s32);
void Audio_PlayCue(u32);
s32 Audio_Check(void);
void Runtime_PushSlotEntry(void *entry, s32 priority);
void UiWork_Finalize(struct UiWindow *work, s32 release);
s32 Resource_ResetEntry(u32 slot);

void Func_08044a58(s32 pc, s32 element, s32 djinn) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
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
  name = gData_0805f65e[element & 3];
  entry = sprite;
  window = UiWindow_Create(2, 1, 26, 5, 0);

  if (window != NULL) {
    UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

    work->dirty = 1;

    id = Localization_LookupEntryId(name);
    UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

    p = entry;
    *p++ = zero;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;

    work->speakerVoiceSound = zero;
    work->speakerVoiceCountdown = zero;

    UiWork_PushValueSlot(pc, 1);
    UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

    text = UiText_BuildRenderEntriesMode1(element + (s32)MsgFoundVenusDjinni);

    UiText_QueueRenderEntries(window, text, 36, 2, zero);

    Audio_PlayCue(81);

    do {
      Runtime_PushSlotEntry(entry, 250);
      WaitFrames(1);
    } while (Audio_Check() != 0 &&
             (gInput.pressed & (KEY_A | KEY_B | 0x300)) == 0);

    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}
