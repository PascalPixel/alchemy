// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 16 of 237 in tla-en. First difference: this issues `ldrh r3, [r6, #0]`
// earlier (row 108) than the native does (row 111).
// First draft of Func_08101638: opens the Djinn menu's backdrop screen, the
// twin of The Broken Seal's Menu_OpenBackdropScreen
// (games/tbs/src/menu/menu/djinn.c): saves the tiles and palettes it covers,
// draws the backdrop, lists the owners (Func_081019a4), then makes the
// list's cursor and the two page arrows and places the arrows at the list
// window's corners.
// Differ 18 of 237 (2026-10-19), instruction order only: the native loads
// palette 15's colour 4 before it builds the first store's address, stores
// arrows[0] before it reads the arrow's attribute byte, and loads the
// window's x before the 0xffff mask. The two IWRAM wrappers keep each
// call's constants (as plain calls, differ 127 with the x and y chains
// fixed); an empty asm before the colour store costs the pool estimate four
// bytes (differ 40). Three minutes of permute found nothing nearer.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct SpriteAttr {
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

struct MenuIcon {
  u8 unk00[5];
  u8 active;
  u16 x;
  u16 y;
  u8 unk0a[0x0a];
  struct SpriteAttr oam;
};

struct MenuWindow {
  u8 unk00[0x0c];
  u16 x;
  u16 y;
};

struct DjinnMenuBackdrop {
  u8 list[0xa8];
  u8 tiles[0x2000];
  u8 palette[0x80];
};

struct DjinnMenuWork {
  u8 unk000[0x10];
  struct MenuWindow *pageWindow;
  struct MenuIcon *cursor;
  u8 unk018[0x1c];
  struct MenuWindow *listWindow;
  u8 unk038[0x140];
  struct MenuIcon *arrows[2];
  struct DjinnMenuBackdrop *backdrop;
};

extern u8 gMenu_BackdropBorderTile[];
extern char ResourceId_ShopGemTiles;

s32 Window_UpdateOrCreate(struct MenuWindow **window, s32 x, s32 y, s32 width,
                            s32 height, s32 frame);
void wait(s32 frames);
void Graphics_ExpandVramTilesByColorTableFar(void *tiles);
void *Palette_GetBasicObjColors(void);
void Graphics_AdjustPalettes(s32 bank);
void Func_081019a4(void *list);
void WindowSprite_ClearListFar(struct MenuWindow *window);
struct MenuIcon *Icon_CreateWithResourceVariant(struct MenuWindow *window,
                                                  s32 x, s32 y);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadResourceFar(s32 entry, s32 size, s32 resource);
struct MenuIcon *WindowSprite_CreateFar(s32 entry, s32 attributes,
                                        struct MenuWindow *window, s32 x,
                                        s32 y);

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as the scene layer does. */
  Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as the scene layer does. */
  Iwram_FillWords(destination, size, value);
}

void Func_08101638(void) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuRuntime;
  struct DjinnMenuBackdrop *backdrop = menu->backdrop;
  struct MenuWindow *window;
  struct MenuIcon *icon;
  s32 entry;
  s32 x;
  s32 y;

  Window_UpdateOrCreate(&menu->listWindow, 0, 5, 30, 15, 2);
  wait(1);
  CopyWords_(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
  CopyWords_(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
  FillWords_(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
  FillWords_((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
  Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
  CopyWords_(BG_CHAR_BLOCK(1) + 150 * 32, gMenu_BackdropBorderTile, 32);
  Dma_Set(Palette_GetBasicObjColors(), (void *)&BG_PLTT_COLOR(5, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  Graphics_AdjustPalettes(8);
  BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
  BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
  Func_081019a4(backdrop);

  window = menu->pageWindow;
  WindowSprite_ClearListFar(window);
  icon = Icon_CreateWithResourceVariant(window, -8, 11);
  icon->active = 13;
  menu->cursor = icon;
  entry = Resource_FindFreeEntry();
  if (entry < 96) {
    VramBlock_LoadResourceFar(entry, 128, (s32)&ResourceId_ShopGemTiles);
    icon = WindowSprite_CreateFar(entry, 0x40000000, window, 0, 0);
    icon->active = 13;
    menu->arrows[0] = icon;
    icon->oam.affineIndex |= 8;
    icon = WindowSprite_CreateFar(entry, 0x40000000, window, 0, 0);
    icon->active = 13;
    menu->arrows[1] = icon;
  }

  window = menu->listWindow;
  icon = menu->arrows[0];
  x = window->x * 8;
  icon->oam.x = icon->x = x;
  y = window->y * 8 + 4;
  icon->oam.y = icon->y = y;
  icon = menu->arrows[1];
  x += 224;
  icon->oam.x = icon->x = x;
  icon->oam.y = icon->y = y;
}
