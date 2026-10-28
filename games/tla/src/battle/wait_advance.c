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
  s16 x; ///< tiles
  s16 y; ///< tiles
};

struct AdvanceOffset {
  u8 unk00[4];
  u16 x; ///< 1/256 pixels
  u16 y; ///< 1/256 pixels
};

/// The battle message window and its scroll offset, in heap slot 0x94.
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
void Oam_AddSpriteAtDepth(void *entry, s32 priority);
void Sound_PlayFar(s32 cue);

/// Waits for the player to advance a battle message: once the window has
/// finished printing, shows the bobbing arrow at its corner until B is held,
/// or A, B, L or R is pressed (or held, after 16 frames), then plays the
/// cursor sound. The Broken Seal's BattleMessage_WaitForAdvance, with the
/// message window read from its heap slot.
void BattlePresentation_WaitForAdvance(void) {
  struct AdvanceSprite *prompt;
  struct AdvanceSprite promptStorage;
  s32 frame;
  s32 tileSlot;
  s32 pictureTiles;
  s32 sheet;
  struct AdvanceWindow *window;
  struct AdvanceOffset *offset;

  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  prompt = &promptStorage;
  /* FAKEMATCH(temporary): the arrow sheet's address held in a local, so the
   * native's stack frame offset is added after the window loads; as
   * (s32)Data_0812996c at its use, 36 of 146 instructions differ in tla-en. */
  sheet = (s32)Data_0812996c;
  tileSlot = VramCache_AllocEntry(128);
  for (frame = 0;; frame++) {
    // Cycle the eight pictures, one every four frames.
    pictureTiles = ((gFrameCount >> 2) & 7) * 128 + sheet;
    window = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->window;
    offset = ((struct AdvanceDisplay *)Ram_HeapSlots->unk84[4])->offset;
    QueueIoWriteDelay10(0x0400004a, 4);
    QueueIoWriteDelay6(0x0400004a, 16);
    Io_Write16(16, (volatile u16 *)0x04000052);
    ((u32 *)prompt)[1] = 0xa400;
    ((u32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pictureTiles);
    prompt->x = window->x * 8 + (offset->x >> 8) + 4;
    // Bob up and down by up to 2 pixels, once every 16 frames.
    prompt->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 +
                (offset->y >> 8) + 6;
    Oam_AddSpriteAtDepth(prompt, 240);
    // A held advance key counts only after 16 frames, so one still held from
    // the previous message does not skip this one.
    /* FAKEMATCH(volatile): held read afresh for the third test, as the
     * native reloads it; as plain gInput.held, 14 of 146 instructions differ
     * in tla-en. */
    if ((gInput.held & 2) || (gInput.pressed & 0x303) ||
        (frame > 15 && (*(volatile u32 *)&gInput.held & 0x303)))
      break;
    WaitFrames(1);
  }
  Sound_PlayFar(111);
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
}
