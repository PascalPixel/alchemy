// Draft, differ 33 of 162 (47 before, 2026-10-26; counts declared with 11
// words, which gives the native's 52-byte stack frame: a frame-gap device to tag
// if it matches, since only four are used): announces a summon (message 0x32
// with its name from Data_0805f676, a jingle, held until it ends or a button),
// then shows message 0x33 with the two elements it needs most (0x34 + element,
// then 0x38) from the SummonDefinition's djinn counts, held until a button. The
// final Resource_ResetEntry takes a slot the function never sets, a stack
// word in the native. Left: register choice
// (window and entries swap r9 and sl, the outer counter in r8 against r7);
// the zero for the stack argument built in r2 rather than taken from r6; and
// the order of the final queue call's stack store.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "window.h"
#include "uiwindow.h"
#include "battle_summon.h"

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 flags);
void UiWork_PushValueSlot(s32 value, s32 kind);
s32 UiText_BuildRenderEntriesMode1(s32 message);
s32 UiText_QueueRenderEntries(struct UiWindow *window, s32 entries, s32 x,
                              s32 y, s32 flags);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
s32 Audio_Check(void);
void Func_0803cf6c(s32 message);
s32 Resource_ResetEntry(u32 slot);

extern const u16 Data_0805f676[];
extern u8 Msg_0032[];
extern u8 Msg_0033[];
extern u8 Msg_0034[];

void Func_08044dc8(s32 summon) {
  struct UiRenderWork *work = Ram_HeapSlots->windowTiles;
  struct UiWindow *window;
  s32 entries;
  u32 resource;
  s32 counts[11];
  const u8 *djinn;
  s32 i;
  s32 n;

  window = UiWindow_Create(4, 1, 22, 5, 0);
  if (window == NULL)
    return;
  work->dirty = 1;
  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  UiWork_PushValueSlot(Data_0805f676[summon & 31], 4);
  entries = UiText_BuildRenderEntriesMode1((s32)Msg_0032);
  UiText_QueueRenderEntries(window, entries, 0, 2, 0);
  Audio_PlayCue(81);
  do
    WaitFrames(1);
  while (Audio_Check() != 0 && (gInput.pressed & 0x303) == 0);
  UiWork_Finalize(window, 2);
  WaitFrames(1);

  window = UiWindow_Create(3, 1, 24, 7, 0);
  if (window == NULL)
    return;
  UiWork_PushValueSlot(Data_0805f676[summon & 31], 4);
  entries = UiText_BuildRenderEntriesMode1((s32)Msg_0033);
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
    UiWork_PushValueSlot(most, 5);
    Func_0803cf6c((s32)Msg_0034 + best);
    counts[best] = 0;
  }
  Func_0803cf6c((s32)Msg_0034 + 4);
  UiText_QueueRenderEntries(window, entries, 4, 2, 0);
  do
    WaitFrames(1);
  while ((gInput.pressed & 0x303) == 0);
  UiWork_Finalize(window, 2);
  WaitFrames(1);
  Resource_ResetEntry(resource);
}
