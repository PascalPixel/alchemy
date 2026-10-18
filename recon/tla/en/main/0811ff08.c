// Draft, differ 52 of 146: ☀️'s BattlePresentation_WaitForAdvance
// (games/tbs/src/battle/battle.c), ported: shows the bobbing arrow at the
// message window and waits for B held, or A, B, L or R pressed (or held after
// 16 frames); plays the cursor sound. Left: instruction order (the frame
// tiles' shift and add, the window loads and the REG_WINOUT constant's
// halves sit several slots from the native's), the window and offset
// swapping r9 and r10, and in the key test the native keeps gInput's address
// in r1 and the mask in r2 and reloads held for the frame > 15 test, where
// here held stays in r1 and is reused. Tried: a for loop or while (1) with
// the frame++ at the end (same), the BLDALPHA store as a volatile constant
// (104: the halfword 16 pooled; Io_Write16 gives the native's movs).
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "iowrite.h"

struct AdvanceSprite {
  struct AdvanceSprite *next;
  u16 attr0;
  u16 attr1;
  u16 attr2;
  u16 attr3;
};

struct AdvanceWindow {
  u8 unk00[0xc];
  s16 x;
  s16 y;
};

struct AdvanceOffset {
  u8 unk00[4];
  u16 x;
  u16 y;
};

struct AdvanceDisplay {
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;
};

extern u32 gFrameCount;
extern u8 Data_0812996c[];

s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32 frames);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 slot, s32 source);
s32 Resource_ResetEntry(u32 slot);
void QueueIoWriteDelay10(u32 address, u32 value);
void QueueIoWriteDelay6(u32 address, u32 value);
s32 Trig_Sin(s32 angle);
void Runtime_PushSlotEntry(void *entry, s32 priority);
void Audio_PlayCue(s32 cue);

void BattlePresentation_WaitForAdvance(void) {
  struct AdvanceSprite *prompt;
  struct AdvanceSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 frameTiles;
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;
  s32 attr2;
  s32 attr1;
  s32 x;
  s16 tile;

  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  frame = 0;
  while (1) {
    struct AdvanceDisplay *display;

    frameTiles = ((gFrameCount >> 2) & 7) * 128 + (s32)Data_0812996c;
    display = Ram_HeapSlots->unk84[4];
    window = display->window;
    offset = display->offset;
    QueueIoWriteDelay10(0x0400004a, 4);
    QueueIoWriteDelay6(0x0400004a, 16);
    Io_Write16(16, (volatile u16 *)0x04000052);
    *(u32 *)&prompt->attr0 = 0xa400;
    *(u32 *)&prompt->attr2 = 0;
    tile = VramCache_Reload(tileSlot, frameTiles) & 0x3ff;
    attr2 = prompt->attr2;
    prompt->attr2 = (attr2 & ~0x3ff) | tile;
    x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
    attr1 = prompt->attr1;
    prompt->attr1 = (attr1 & ~0x1ff) | x;
    *(u8 *)&prompt->attr0 = Trig_Sin(gFrameCount << 12) / 32768 +
                            window->y * 8 + (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(prompt, 240);
    if ((gInput.held & 2) || (gInput.pressed & 0x303) ||
        (frame > 15 && (gInput.held & 0x303)))
      break;
    WaitFrames(1);
    frame++;
  }
  Audio_PlayCue(111);
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
}
