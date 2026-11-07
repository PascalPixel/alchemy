#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "window.h"
#include "uiwindow.h"
#include "battle_summon.h"

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void MessageArgument_Push(s32 value, s32 kind);
s32 Message_DecodeMode1(s32 message);
s32 Message_Start(struct Window *window, s32 entries, s32 x, s32 y, s32 flags);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
s32 Sound_GetBgmRestoreDelayFar(void);
void Func_0803cf6c(s32 message);
s32 Resource_ResetEntry(u32 slot);

extern const u16 Data_0805f676[];
extern const u8 MsgCanNowSummon[], MsgToSummon[];
/// The first of four Djinn counts (Venus, Mercury, Mars, Jupiter); the message
/// four past it ends their line.
extern const u8 MsgSummonVenus[];

void Func_08044dc8(s32 summon) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  struct Window *window;
  s32 entries;
  u32 resource;
  /* FAKEMATCH(dead): counts holds 11 words where four are used, which gives
   * the native's 52-byte stack frame; as counts[4], 4 of 162 instructions
   * differ in tla-en. */
  s32 counts[11];
  const u8 *djinn;
  s32 i;
  s32 n;

  /* FAKEMATCH(dead): window set to NULL, which Window_Create's result
   * replaces at once; without that store, 33 of 162 instructions differ in
   * tla-en. */
  window = NULL;
  window = Window_Create(4, 1, 22, 5, 0);
  if (window == NULL)
    return;
  work->dirty = 1;
  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  MessageArgument_Push(Data_0805f676[summon & 31], 4);
  entries = Message_DecodeMode1((s32)MsgCanNowSummon);
  Message_Start(window, entries, 0, 2, 0);
  Sound_PlayFar(81);
  do
    wait(1);
  while (Sound_GetBgmRestoreDelayFar() != 0 && (gInput.pressed & 0x303) == 0);
  Window_Close(window, 2);
  wait(1);

  window = Window_Create(3, 1, 24, 7, 0);
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
    MessageArgument_Push(most, 5);
    Func_0803cf6c((s32)MsgSummonVenus + best);
    counts[best] = 0;
  }
  Func_0803cf6c((s32)MsgSummonVenus + 4);
  Message_Start(window, entries, 4, 2, 0);
  do
    wait(1);
  while ((gInput.pressed & 0x303) == 0);
  Window_Close(window, 2);
  wait(1);
  Resource_ResetEntry(resource);
}
