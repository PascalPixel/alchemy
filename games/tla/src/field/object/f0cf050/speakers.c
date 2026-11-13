/// @file
/// Speaker definitions, and a heading picture shown in a window.
// Part of f0cf050.c.

#include "../f0cf050_internal.h"
#include "event.h"
#include "module/field/f0cf050.h"
#include "resource_ids.h"

/* VIEW(definition_flags): routine BattleEffect_GetAnimationValue as s32 BattleEffect_GetAnimationValue(void); the definition has s32 BattleEffect_GetAnimationValue(u32 objectId); tried as the definition's s32 BattleEffect_GetAnimationValue(u32 objectId), which the object already declares, it does not compile in tla-ja: games/tla/src/field/object/f0cf050.c:2239: too few arguments to function `BattleEffect_GetAnimationValue'. */
extern s32 BattleEffect_GetAnimationValue__definition_flags(void) __asm__("BattleEffect_GetAnimationValue");

/// Returns the voice sound of sprite id's messages, its definition's value
/// plus 0x100, or 0 for none (0xff) or with option 3
/// (gMainRoutineWork.battleFxEnabled) off. Message_OpenWindowFar takes it in
/// the high half of its last argument.
s32 Speaker_GetVoice(s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 value;

  if (party->battleFxEnabled == 0 ||
      (value = BattleEffect_FindDefinition(id)->value) == 0xff)
    return 0;
  return value + 0x100;
}

u8 BattleEffect_GetFlags(void) {
  return BattleEffect_FindDefinition(BattleEffect_GetAnimationValue__definition_flags())->flags;
}

extern const s32 gData_080f3228[];

/// Looks key up in gData_080f3228's pairs and returns its value, or -1.
s32 Func_080d1ee8(s32 key) {
  const s32 *entry = gData_080f3228;
  s32 value;

  while (1) {
    value = entry[0];
    if (value == -1)
      return value;
    if (value == key)
      return entry[1];
    entry += 2;
  }
}

s32 Func_080d1f0c(s32 key) {
  Func_080d1ee8(key);
  return 0;
}

s32 Func_080d1f18(s32 key) { return Func_080d1ee8(key); }

/// Draws heading picture `heading` of the headings resource into a 26 by 5 cell
/// window and opens the window from the centre, 5 pixels a side each frame
/// for 24 frames.
void Func_080d1f20(s32 heading) {
  u16 tile;
  s32 *headings = Resource_GetTableEntry((s32)&ResourceId_Headings);
  u8 *tiles;
  u16 *cell;
  u32 row, column;
  s32 left, right;

  tile = 16;
  Event_WaitTransitionFrames();
  cell = (u16 *)0x06002000;
  tiles = Heap_AllocEwram(0x1040);
  headings += headings[heading] / 4;
  Resource_DecodeByteLzInRam(headings, tiles);
  Dma_Set(tiles, (void *)0x06000200, 0x84000410, (volatile u32 *)0x040000d4);
  Heap_Release(tiles);
  REG_DISPCNT &= 0xfeff;
  Dma_Fill32(0xf000, cell, 0x800);
  cell = (u16 *)0x060020c0;
  for (row = 0; row <= 4; row++) {
    cell += 2;
    for (column = 0; column <= 25; column++) {
      *cell++ = (u16)(tile++) | 0xf000;
    }
    cell += 4;
  }
  REG_DISPCNT &= 0x1fff;
  REG_DISPCNT |= 0x2000;
  REG_WININ = 0x3f;
  REG_WINOUT = 0x3e;
  REG_WIN0H = 0x7878;
  REG_WIN0V = 0x1848;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10008, 0);
  BattleEffect_StartBufferInterpolation(24);
  REG_DISPCNT ^= 0x100;
  left = 120;
  right = 120;
  // column is reused for the window's frames; a local of its own differs.
  for (column = 0; column <= 23; column++) {
    left -= 5;
    right += 5;
    REG_WIN0H = (left << 8) | right;
    wait(1);
  }
}

/// Waits up to 120 frames, stopping early when a button is pressed.
void Event_WaitTwoSecondsOrPress(void) {
  u32 i;

  for (i = 0; i <= 119; i++) {
    wait(1);
    if (gInput.pressed != 0)
      break;
  }
}

/// Clears the left (corner & 3) + 1 pixels of halfword corner >> 2 in the
/// 4bpp tile at (x, y) of a 26-tile-wide grid at VRAM 0x06000200.
void Tile_ClearLeftPixels(s32 x, s32 y, u32 corner) {
  u16 *cell = (u16 *)(0x06000200 + ((y * 26 + x) << 5));

  cell += corner >> 2;

  switch (corner & 3) {
  case 0: {
    u32 value = *cell;
    u32 mask;

    mask = 0xfff0;
    mask &= value;
    *cell = mask;
  } break;
  case 1:
    *cell &= 0xff00;
    break;
  case 2:
    *cell &= 0xf000;
    break;
  case 3:
    *cell &= 0;
    break;
  }
}

extern s16 gData_080f3230[];

#define DMA3 REG_DMA3

/// Closes Func_080d1f20's picture window: fades the palette out while wiping
/// the picture's 26 by 5 tiles from the left by the steps in gData_080f3230
/// (runs of widths, each run ended by a negative entry and the whole by -9),
/// then turns BG0 and the window off, clears the screen block, reloads the
/// window tiles and turns BG0 back on.
void Func_080d20fc(void) {
  s32 step = 0;
  u32 x;
  u32 y;
  volatile u32 fill;

  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(24);
  for (;;) {
    for (; gData_080f3230[step] >= 0; step++) {
      for (x = 0; x < 26; x++) {
        for (y = 0; y < 5; y++)
          Tile_ClearLeftPixels(x, y, gData_080f3230[step]);
      }
    }
    wait(2);
    if (gData_080f3230[step] == -9)
      break;
    step++;
  }
  REG_DISPCNT ^= DISPCNT_BG0_ON | DISPCNT_WIN0_ON;
  Dma_Fill32Cell(fill, 0xf000, (void *)0x06002000, 0x800);
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          (void *)0x06000000, 0x84000800, DMA3);
  REG_DISPCNT ^= DISPCNT_BG0_ON;
}
#undef DMA3
