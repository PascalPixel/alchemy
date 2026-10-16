// Near miss: differ 8 of 95 (2026-10-12). From ☀️ Party_ShowJoinedMessage.
// Three scheduling differences remain, no instruction or register differs:
// the palette load comes after the 0xe000 mask starts (native: load first);
// the button mask 0x303 starts two calls early (native: after Audio_PlayCue,
// interleaved with the gInput address); and in the wait loop the entry
// pointer is moved to r0 before r1 takes 250 (native: r1 first).
// A barrier after Audio_PlayCue fixes the second but moves
// UiWork_PushValueSlot's arguments; four permuter minutes found nothing.
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "types.h"
#include "input.h"
#include "window.h"
#include "uiflow.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

void UiWindow_DrawDividerLine(struct UiWindow *, s32, s32, s32, s32);
void UiWork_PushValueSlot(s32, s32);
struct UiChannelSlot *UiText_QueueRenderEntries();
void Audio_PlayCue(s32);
s32 Func_08044b80(u32 index);
extern u8 MsgJoinedParty[];
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 style);

void Func_08044b98(s32 member) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
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
        Localization_LookupEntryId(Func_08044b80(member)), 0, &handle, &palette,
        14, 0);
    p = entry;
    *p++ = 0;
    *p++ = 0x8000000c | ((2 * 8 + 4) << 16);
    *p = palette | 0xe000;
    work->result[0] = 0;
    work->result[1] = 0;
    UiWork_PushValueSlot(member, 1);
    UiText_QueueRenderEntries(
        window, UiText_BuildRenderEntriesMode1((s32)MsgJoinedParty), 36, 2, 0);
    Audio_PlayCue(81);
    do {
      Runtime_PushSlotEntry(entry, 250);
      WaitFrames(1);
    } while (Audio_Check() != 0 && (gInput.pressed & 0x303) == 0);
    UiWork_Finalize(window, 2);
    WaitFrames(1);
    Resource_ResetEntry(handle);
  }
}
