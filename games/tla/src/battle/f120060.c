#include "types.h"
#include "input.h"
#include "iowrite.h"

struct CursorSprite {
  struct CursorSprite *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk;
};

struct MenuPoint {
  s32 x;
  s32 y;
};

extern u32 gFrameCount;

s32 Ui_GetTableWordZeroFar(s32 index);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32 frames);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 slot, s32 source);
s32 Resource_ResetEntry(u32 slot);
void QueueIoWriteDelay10(u32 address, u32 value);
void QueueIoWriteDelay6(u32 address, u32 value);
void Oam_AddSpriteAtDepth(void *entry, s32 priority);

/// The Broken Seal's BattlePresentation_WaitForPromptAt: shows the cursor
/// bobbing at (x, y) and waits for A, B, L or R; returns 1.
s32 Func_08120060(s32 x, s32 y) {
  struct MenuPoint position;
  s32 pointerTiles;
  struct CursorSprite promptStorage;
  struct CursorSprite *prompt;
  s32 tileSlot;

  pointerTiles = Ui_GetTableWordZeroFar(0);
  position.x = x;
  position.y = y;
  while (!UiWork_IsCompleteFar())
    WaitFrames(1);
  prompt = &promptStorage;
  tileSlot = VramCache_AllocEntry(128);
  while (1) {
    QueueIoWriteDelay10(0x0400004a, 4);
    QueueIoWriteDelay6(0x0400004a, 16);
    Io_Write16(16, (volatile u16 *)0x04000052);
    ((s32 *)prompt)[1] = 0x40000000;
    ((s32 *)prompt)[2] = 0;
    prompt->tile = VramCache_Reload(tileSlot, pointerTiles);
    prompt->x = position.x + ((gFrameCount & 4) >> 1) - 4;
    prompt->y = position.y - ((gFrameCount & 4) >> 2) - 8;
    Oam_AddSpriteAtDepth(prompt, 240);
    if (gInput.pressed & 0x303)
      break;
    WaitFrames(1);
  }
  Resource_ResetEntry(tileSlot);
  WaitFrames(1);
  return 1;
}
