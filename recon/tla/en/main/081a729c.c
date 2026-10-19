// Draft, differ 99 of 254: the title screen, as the end of The Broken Seal's Title_RunIntro
// (games/tbs/src/graphics/palette/palette.c). Fades to white, shows resource
// 0x16's picture on BG2 (its map numbered from tile 0x1a0, two blank columns
// a row), resets the scroll, and with prompt set loads the five prompt
// sprites from gTitle_PromptTiles. After the fade from white it holds 660
// frames, or 5400 with the prompt; after the first 120 the prompt's three
// sprites blink by Data_081a873e's levels, and A or Start ends it early,
// returning 1 (else 0). Resource 0x16 is Title_ScreenPicture in the
// directory, which needs an id name there before adoption. Left: the tile
// counter, as in Func_081a6638: the native rebuilds 0x10000 in the loop
// for its s16 step (combine's form, after loop.c), where here loop.c
// strength-reduces the shifted counter, so the map loops' registers shift;
// the counter as u16 or s32 is worse (122, 126); the prompt flag and the
// hold count take other registers. The hold count as an if/else (not a
// default and an if) lets loop.c move the key's exit block before the loop
// as the native has it (149 to 132), and the step written apart from the
// store keeps the counter's copy (132 to 99).
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "graphics.h"
#include "ram_buffer.h"
#include "bg_scroll.h"
#include "input.h"
#include "callback_scheduler.h"

struct TitleSprite {
  u32 next;
  u8 y;
  u8 attr0High;
  u16 x : 9;
  u16 attr1High : 7;
  s16 tile;
};

struct TitleScreenWork {
  u8 unk00[16];
  struct TitleSprite sprites[8];
};

extern const u8 Data_081a873e[];
extern char ResourceId_TitleScreenPicture;
extern const u8 gTitle_PromptTiles[];

void Func_08014b70(void);
void VramCache_Reset(void);
void Ui_LoadWindowGraphics(void);
void Fade_ToWhite(s32 frames);
void Fade_FromWhite(s32 frames);
void Fade_Wait(void);
void WaitFrames(s32 frames);
u16 Random16(void);
void Func_080140d8(void *entry, s32 slot);

s32 Func_081a729c(s32 prompt) {
  s32 result = 0;
  s32 blankTile = 0x1ff;
  struct TitleScreenWork *work = Runtime_AllocateBlock(172, 0x70);
  u8 *data;
  u16 *tilemap;
  s16 tile;
  u32 i;
  u32 j;
  u32 holdFrames;

  Scheduler_ResetTaskTable();
  Fade_ToWhite(1);
  Func_08014b70();
  VramCache_Reset();
  WaitFrames(1);
  REG_BG2CNT = 0x681;
  REG_DISPCNT = 0x1440;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry((s32)&ResourceId_TitleScreenPicture);
  Dma_Set(data, (void *)BG_PLTT, 0x84000070, REG_DMA3);
  Resource_DecodeType01(data + 0x1c0, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)0x06006800, 0x84002580, REG_DMA3);
  tilemap = (u16 *)0x06003000;
  tile = 0x1a0;
  for (i = 0; i < 20; i++) {
    for (j = 0; j < 30; j++) {
      *tilemap++ = tile;
      tile++;
    }
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;
  Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004, REG_DMA3);
  Ui_LoadWindowGraphics();
  Func_08014b70();
  if (prompt != 0) {
    u8 *buffer = Runtime_AllocateBlock(56, 0x400);
    struct TitleSprite *sprite = work->sprites;

    Resource_DecodeByteLzInRam(gTitle_PromptTiles, buffer);
    for (i = 0; i < 5; i++) {
      s32 vramBlock =
          VramCache_Load(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *spriteWords = (u32 *)sprite;

      *spriteWords++ = 0;
      sprite++;
      *spriteWords++ = 0x40004000;
      *spriteWords = vramBlock;
    }
    Runtime_ReleaseHeapBlock(56);
  }
  Fade_FromWhite(30);
  Fade_Wait();
  REG_DISPCNT = 0x1540;
  if (prompt != 0)
    holdFrames = 5400;
  else
    holdFrames = 660;
  for (i = 0; i < holdFrames; i++) {
    if (i > 120) {
      if (prompt != 0) {
        struct TitleSprite *sprite = work->sprites;
        s32 x;
        u32 level;

        for (j = 0, x = 80; j < 3; j++) {
          sprite->x = x;
          sprite->y = 124;
          Func_080140d8(sprite, 0);
          x += 32;
          sprite++;
        }
        level = Data_081a873e[i % 60];
        REG_BLDCNT = 0x2f50;
        REG_BLDALPHA = (16 - level) * 256 + level;
      }
      if (gInput.pressed & 9) {
        result = 1;
        break;
      }
    }
    Random16();
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(172);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  WaitFrames(1);
  return result;
}
