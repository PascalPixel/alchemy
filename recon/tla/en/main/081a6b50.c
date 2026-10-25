// Draft, differ 292 of 360: a title sequence frame callback (its state in heap
// slot 43, 0x030000ac): clears OAM, then in phase 0 steps an animation
// through gData_081a8378 (a frame's first tile, rows, delay and offsets) along
// the path in gData_081a84e6, drawing its 8-wide grid of 8x8 sprites, until
// the path ends (-1): then loads resources 0x1b and 0x1c (a sprite palette
// and tiles), sets blending and enters phase 1, which places the three logo
// pieces with Title_PlaceLogoPiece, grows their shared affine scale to 256,
// fades the blend in a step every three frames and loads resources 0x1d to
// 0x1f at frames 8, 16 and 24. Same calls, tables and control flow; left:
// register allocation (the native keeps the fill word's address in r5 and
// the path table in r7) and the sprite loop's counters.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"

struct TitleLogoState {
  s16 phase;
  s16 frame; ///< index into gData_081a8378 (5 halfwords a frame)
  s16 timer;
  s16 step; ///< index into gData_081a84e6 (3 halfwords a step); the scale in
            ///< phase 1
  s16 delay;
};

struct TitleSprite {
  u32 y : 8;
  u32 attr0 : 8;
  u32 x : 9;
  u32 attr1 : 7;
  u16 tile : 10;
  u16 attr2 : 6;
  u16 unused;
};

extern const s16 gData_081a8378[];
extern const s16 gData_081a84e6[];
extern u8 Data_02026800[];
extern char ResourceId_Data01B;
extern char ResourceId_Data01C;
extern char ResourceId_Data01D;
extern char ResourceId_Data01E;
extern char ResourceId_Data01F;

void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void Title_PlaceLogoPiece(s32 tile, s32 x, u32 y);

void Func_081a6b50(void) {
  struct TitleLogoState *logo;
  struct TitleSprite sprite;
  volatile u32 fill;
  s32 x;
  s32 tile;
  s32 rows;
  s32 left;
  s32 top;
  s32 row;
  s32 column;
  s32 offset;
  u8 *oam;
  s32 y;
  s32 id;

  logo = Ram_HeapSlots->unk84[10];
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  if (logo->phase == 0) {
    if (logo->timer == 0) {
      if (gData_081a8378[logo->frame + 5] != -1)
        logo->frame += 5;
      logo->timer = gData_081a8378[logo->frame + 2];
    } else {
      logo->timer--;
    }
    if (logo->delay == 0) {
      logo->step += 3;
      logo->delay = gData_081a84e6[logo->step + 2];
    } else {
      logo->delay--;
    }
    x = gData_081a84e6[logo->step];
    if (x == -1) {
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_Data01B),
              (void *)0x05000200, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
              REG_DMA3);
      Resource_DecodeByteLzInRam(
          Resource_GetTableEntry((s32)&ResourceId_Data01C), Data_02026800);
      Dma_Set(Data_02026800, (void *)0x06010000,
              (DMA_ENABLE | DMA_32BIT) << 16 | 0xc00, REG_DMA3);
      logo->phase = 1;
      logo->frame = 1;
      logo->timer = 0;
      logo->step = 160;
      logo->delay = 0;
      REG_BLDCNT = 0x250;
      REG_BLDALPHA = 0x1001;
      return;
    }
    tile = gData_081a8378[logo->frame];
    rows = gData_081a8378[logo->frame + 1];
    left = x + gData_081a8378[logo->frame + 3] - 28;
    top = gData_081a84e6[logo->step + 1] + gData_081a8378[logo->frame + 4];
    offset = 0;
    for (row = 0; row < rows; row++) {
      x = left;
      y = top;
      oam = (u8 *)0x07000000 + row * 64;
      for (column = 0; column < 8; column++) {
        Dma_Fill32Cell(fill, 0, &sprite, sizeof(sprite));
        sprite.tile = tile + offset + column;
        sprite.x = x;
        sprite.y = y;
        Dma_Set(&sprite, oam, (DMA_ENABLE | DMA_32BIT) << 16 | 2, REG_DMA3);
        oam += 8;
        x += 8;
      }
      offset += 8;
      top += 8;
    }
  } else if (logo->phase == 1) {
    y = (logo->step - 256) / 32 + 50;
    Title_PlaceLogoPiece(0, (logo->step - 256) / 4 + 30, y);
    Title_PlaceLogoPiece(128, 83, y);
    Title_PlaceLogoPiece(256, 140 - (logo->step - 256) / 4, y);
    *(volatile u16 *)0x07000006 = logo->step;
    *(volatile u16 *)0x0700000e = 0;
    *(volatile u16 *)0x07000016 = 0;
    *(volatile u16 *)0x0700001e = logo->step;
    if (logo->step < 256) {
      logo->step += 2;
      if (logo->step > 256)
        logo->step = 256;
    }
    if (logo->frame < 16) {
      logo->timer++;
      if (logo->timer % 3 == 0) {
        logo->frame++;
        REG_BLDALPHA = logo->frame | 0x1000;
      }
      if (logo->timer == 8)
        id = (s32)&ResourceId_Data01D;
      else if (logo->timer == 16)
        id = (s32)&ResourceId_Data01E;
      else if (logo->timer == 24)
        id = (s32)&ResourceId_Data01F;
      else
        return;
      Resource_DecodeByteLzInRam(Resource_GetTableEntry(id), Data_02026800);
      Dma_Set(Data_02026800, (void *)0x06010000,
              (DMA_ENABLE | DMA_32BIT) << 16 | 0xc00, REG_DMA3);
    }
  }
}
