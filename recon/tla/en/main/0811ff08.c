// Draft, differ 36 of 146 (2026-10-26; 50 before): ☀️'s
// BattleMessage_WaitForAdvance (games/tbs/src/battle/battle/events.c), ported:
// shows the bobbing arrow at the message window and waits for B held, or A,
// B, L or R pressed (or held after 16 frames); plays the cursor sound. The
// arrow is ☀️'s bitfield sprite (tile, x and y stored through their fields).
// The native reads gInput.held again for the frame > 15 test where alchemy-gcc
// reuses the first read; read through a volatile view there (tagged) the key
// test matches. Left: the frame tiles' address (the arrow sheet plus 128 a
// frame) is added after the message window's loads in the native, so the
// window and offset pointers and the sheet take other registers, and the
// window and offset reads in the x and y sums trade r9 and r10.
#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "iowrite.h"

struct AdvanceSprite {
  struct AdvanceSprite *next;
  u16 y : 8;
  u16 flags : 8;
  u16 x : 9;
  u16 size : 7;
  u16 tile : 10;
  u16 palette : 6;
  u16 affineParam;
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

  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    frameTiles = ((gFrameCount >> 2) & 7) * 128 + (s32)Data_0812996c;
    window = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->window;
    offset = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->offset;
    QueueIoWriteDelay10(0x0400004a, 4);
    QueueIoWriteDelay6(0x0400004a, 16);
    Io_Write16(16, (volatile u16 *)0x04000052);
    ((u32 *)prompt)[1] = 0xa400;
    ((u32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, frameTiles);
    prompt->x = window->x * 8 + (offset->x >> 8) + 4;
    prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                (offset->y >> 8) + 6;
    Runtime_PushSlotEntry(prompt, 240);
    /* FAKEMATCH(volatile): held read afresh for the third test, as the
     * native reloads it; as plain gInput.held, the draft differs in 50 of
     * 146 instructions in tla-en rather than 36. */
    if ((gInput.held & 2) || (gInput.pressed & 0x303) ||
        (frame > 15 && (*(volatile u32 *)&gInput.held & 0x303)))
      break;
    WaitFrames(1);
  }
  Audio_PlayCue(111);
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
}
