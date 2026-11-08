// Matches: differ 0 (tla-en, 2026-11-08)
// Every edition (tla-en, tla-ja, tla-de, tla-es, tla-fr, tla-it) differs 0,
// the logo pieces' resting places chosen by LANGUAGE below (German, Spanish
// and Italian 24, 88, 152; French 19, 83, 147; English and Japanese 30, 83,
// 140).
// 2026-11-08: the last 2 were the frame table's const: its loads then do not
// wait on the spill stores, and sched2 lifts offsetX's load over offsetY's
// address. Declared plain, as gData_081a84e6 is, offsetX's load waits for rows'
// spill and the listing's order follows with no device; the once-only block
// around offsetX and offsetY that earlier rounds kept (2 of 360) is gone.
// A frame callback of the intro (its state in heap slot 43, 0x030000ac): clears
// OAM, then in phase 0 steps an animation through gData_081a8378 along the path
// in gData_081a84e6, drawing its 8-wide grid of 8x8 sprites, until the path
// ends (-1): then loads resources 0x1b and 0x1c and enters phase 1, which
// places the three logo pieces with Title_PlaceLogoPiece, grows their shared
// affine scale to 256, fades the blend in and loads resources 0x1d to 0x1f at
// frames 8, 16 and 24.
// Both tables are declared non-const: a const table's loads are scheduled
// across the stores before them. The sprite expressions' spellings steer
// induction variables and loop reversal.
#include "types.h"
#include "edition.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"

// Where the left, middle and right logo pieces come to rest.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define LOGO_LEFT_X 24
#define LOGO_MIDDLE_X 88
#define LOGO_RIGHT_X 152
#elif LANGUAGE == LANGUAGE_FRENCH
#define LOGO_LEFT_X 19
#define LOGO_MIDDLE_X 83
#define LOGO_RIGHT_X 147
#else
#define LOGO_LEFT_X 30
#define LOGO_MIDDLE_X 83
#define LOGO_RIGHT_X 140
#endif

struct TitleLogoState {
  s16 phase;
  s16 frame; ///< index into gData_081a8378 (5 halfwords a frame)
  s16 timer;
  s16 step; ///< index into gData_081a84e6 (3 halfwords a step); the scale in
            ///< phase 1
  s16 delay;
};

struct OamEntry {
  u32 y : 8;
  u32 affine : 2;
  u32 mode : 2;
  u32 mosaic : 1;
  u32 colors256 : 1;
  u32 shape : 2;
  u32 x : 9;
  u32 matrix : 5;
  u32 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

extern s16 gData_081a8378[];
extern s16 gData_081a84e6[];
extern u8 Data_02026800[];
extern char ResourceId_Data01B;
extern char ResourceId_Data01C;
extern char ResourceId_Data01D;
extern char ResourceId_Data01E;
extern char ResourceId_Data01F;

void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void Title_PlaceLogoPiece(s32 tile, s32 x, u32 y);

#define LOAD_LOGO_TILES(id)                                                    \
  {                                                                            \
    u8 *buffer = Data_02026800;                                                \
    Resource_DecodeByteLzInRam(Resource_GetTableEntry((s32)&(id)), buffer);    \
    Dma_Set(buffer, (void *)0x06010000,                                        \
            (DMA_ENABLE | DMA_32BIT) << 16 | 0xc00, REG_DMA3);                 \
  }

void Func_081a6b50(void) {
  struct TitleLogoState *logo;
  struct OamEntry sprite;
  volatile u32 fill;
  s32 x;
  s32 tile;
  s32 left;
  s32 rows;
  s32 top;
  s32 offsetX;
  s32 offsetY;
  s32 row;
  s32 column;
  s32 y;
  s32 pieceX;
  volatile u16 *matrix;

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
      LOAD_LOGO_TILES(ResourceId_Data01C);
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
    offsetX = gData_081a8378[logo->frame + 3];
    offsetY = gData_081a8378[logo->frame + 4];
    left = x + offsetX - 28;
    top = gData_081a84e6[logo->step + 1] + offsetY;
    for (row = 0; row < rows; row++) {
      for (column = 0; column < 8; column++) {
        Dma_Fill32Cell(fill, 0, &sprite, sizeof(sprite));
        sprite.tile = tile + column + row * 8;
        sprite.x = left + column * 8;
        sprite.y = top + row * 8;
        Dma_Set(&sprite, (u8 *)0x07000000 + (row * 8 + column) * 8,
                (DMA_ENABLE | DMA_32BIT) << 16 | 2, REG_DMA3);
      }
    }
  } else if (logo->phase == 1) {
    pieceX = (logo->step - 256) / 4 + LOGO_LEFT_X;
    y = (logo->step - 256) / 32 + 50;
    Title_PlaceLogoPiece(0, pieceX, y);
    Title_PlaceLogoPiece(128, LOGO_MIDDLE_X, y);
    pieceX = LOGO_RIGHT_X - (logo->step - 256) / 4;
    Title_PlaceLogoPiece(256, pieceX, y);
    matrix = (volatile u16 *)0x07000006;
    *matrix = logo->step;
    matrix += 4;
    *matrix = 0;
    matrix += 4;
    *matrix = 0;
    matrix[4] = logo->step;
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
        LOAD_LOGO_TILES(ResourceId_Data01D)
      else if (logo->timer == 16)
        LOAD_LOGO_TILES(ResourceId_Data01E)
      else if (logo->timer == 24)
        LOAD_LOGO_TILES(ResourceId_Data01F)
    }
  }
}
