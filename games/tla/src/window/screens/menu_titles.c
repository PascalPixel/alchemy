/// @file
/// Pictures and command icons loaded for the menus, and BG1's priority.
// Part of window.c.

#include "../screens_internal.h"
#include "system/io_write.h"
#include "module/tla.h"
#include "module/window/screens.h"
#include "resource_ids.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(void *, void *))Resource_DecodeByteLzInRam)

/// Decompresses picture `picture` of gMenuTitlePictures, a 64x16
/// heading (Save, Continue, Copy, Erase, Battle, Link, Pause, Setup) of 512
/// bytes, into VRAM slot `slot`, through a 1 KB staging block in
/// HEAP_SLOT_TILE_SCRATCH. The heap block is not freed when `slot` is not a
/// valid VRAM slot.
void MenuTitle_Load(s32 groupIndex, s32 resourceIndex) {
  void *stagingBuffer = HeapSlot_AllocEwram(56, 0x400);
  s32 resourceAddress = gRenderResource_PairSourceTable[groupIndex];

  if (resourceIndex <= 0x5F) {
    Resource_DecodeByteLzInRam((void *)resourceAddress, stagingBuffer);
    VramCache_Load(resourceIndex, 0x200, stagingBuffer);
    HeapSlot_Release(56);
  }
}

struct WindowSprite *MenuTitle_Create(s32 group, struct RenderInput *input,
                                      s32 x, s32 y) {
  s32 index;
  struct WindowSprite *first;
  struct WindowSprite *second;

  index = VramCache_FindFreeEntry();
  if (index > 95)
    return NULL;

  MenuTitle_Load(group, index);
  first = WindowSprite_Create(index, OUTPUT_SIZE_32X16, input, x, y);
  first->sentinel = 0xFD;
  second = WindowSprite_Create(index, OUTPUT_SIZE_32X16, input, x + 32, y);
  second->sentinel = 0xFD;
  second->sprite.oam.index += 8;
  return first;
}

#ifndef MODERN
// A current GCC refuses the volatile redeclaration and reads name_entry.c's.
/// Counted up by every VBlank.
extern volatile u32 gVBlankCount;
#endif

/// Bobs the two sprites of a pair (see MenuTitle_Create) up and down
/// by gWindow_PairBobOffsets, one step every four VBlanks.
void MenuTitle_Bob(struct BobPairObject *obj) {
  s32 value;

  value = obj->baseY + gWindow_PairBobOffsets[(gVBlankCount >> 2) & 7];
  obj->y = value;
  obj = obj->next;
  value = obj->baseY + gWindow_PairBobOffsets[(gVBlankCount >> 2) & 7];
  obj->y = value;
}

/// Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB)
/// into VRAM slot `slot`, remapping its first 768 bytes' colours by table
/// when `remap` is nonzero.
void CommandIcon_Load(s32 index, s32 value, s32 flag) {
  s32 size = 1024;
  void *buffer = HeapSlot_AllocEwram(56, size);
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  if (value <= 95) {
    Resource_DecodeByteLzInRam((void *)((u32)base + base[index]), buffer);
    if (flag != 0)
      SpriteColor_RemapPixelsFar(buffer, 768);
    VramCache_Load(value, size, buffer);
    HeapSlot_Release(56);
  }
}

/// Shows command icon `icon` (see CommandIcon_Load) as a 32x32
/// 256-colour sprite at (x, y) in `window`, on draw layer 251. Returns NULL
/// when no VRAM slot is free. The settings menu's icons come through here
/// (window/settings_menu.c).
struct WindowSprite *CommandIcon_Create(s32 arg0, s32 arg1,
                                        struct RenderInput *arg2, s32 arg3,
                                        s32 arg4) {
  s32 index;
  struct WindowSprite *entity;

  index = VramCache_FindFreeEntry();
  entity = NULL;
  if (index != 0x60) {
    CommandIcon_Load(arg0, index, arg1);
    entity = WindowSprite_Create(index, OUTPUT_SIZE_32X32, arg2, arg3, arg4);
    entity->sprite.oam.palette256 = 1;
    entity->sentinel = 0xfb;
  }
  return entity;
}

void WindowSprite_Pulse(struct Object *obj) {
  s32 v;
  struct Effect effect;

  v = gWindowSprite_PulseScales[(gVBlankCount >> 1) & 7] / 256;

  if (obj != 0) {
    effect.x = v;
    effect.y = v;
    effect.z = 0;
    obj->affine22 =
        OamMatrix_AllocFromTransform((struct AffineTransform *)&effect);
    obj->mode21 = 3;
    obj->pos22 = obj->src6 + 0xfff0;
    obj->out20 = obj->src8 + 0xf0;
    obj->out15 = 0xfc;
  }
}

s32 Djinn_GetGiveTutorialMessage(void) {
  if (GameFlag_Get(0x175))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

// Builds twelve two-tile patterns as the 24 BG tiles from tile 788, the ones
// Window_DrawThreeTileColumn draws: each tile filled with colour 4, then rows
// 1 to 7 marked by XOR with gMenu_BarPatternRows. The first set grows its
// pattern with the row, the second skips row 1. It is declared to return s32
// but returns no value, and its caller reads none.
s32 Menu_BuildLocalizedPatternTiles(void) {
  s32 set;
  s32 n;

  for (set = 0; set < 2; set++) {
    for (n = 0; n < 6; n++) {
      u32 *tile = (u32 *)(set * 0x180 + n * 0x40 + 0x06006280);
      s32 x;

      Iwram_FillWordsEntry(tile, 64, 0x44444444);
      for (x = 1; x <= 7; x++) {
        s32 mi = n;

        if (set == 1 && x <= 1) {
          continue;
        }
        if (set == 0 && n > x - 2) {
          mi = x - 2;
          if (mi < 0) {
            mi = 0;
          }
        }
        tile[x] = XorWord(tile[x], gMenu_BarPatternRows[mi].word0);
        tile[x + 8] = XorWord(tile[x + 8], gMenu_BarPatternRows[mi].word1);
      }
    }
  }
}

// UB: it is declared to return int but returns no value, and no caller reads
// one; UBFIX makes it void.
#ifdef UBFIX
void Window_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                                s32 unused) {
#else
int Window_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                               s32 unused) {
#endif
  s32 tile_offset = arg3 * 2;
  s32 tile = tile_offset + 0xF315;

  Window_SetTilemapEntry((struct Window *)arg0, 0x400 | tile, arg1, arg2, 0);
  Window_SetTilemapEntry((struct Window *)arg0, tile_offset + 0xF314, arg1 + 1,
                         arg2, 0);
  Window_SetTilemapEntry((struct Window *)arg0, tile, arg1 + 2, arg2, 0);
}

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift) {
  s32 neg;
  s32 rshift;
  s32 lshift;
  s32 i;

  neg = -shift;
  i = 0;
  rshift = neg * 4;
  lshift = shift * 4;

  do {
    u32 result;
    u32 shifted;
    u32 fallback;
    s32 j;

    shifted = *second++;
    result = 0;
    fallback = *first++;
    if (shift < 0)
      shifted >>= rshift;
    else
      shifted <<= lshift;

    j = 7;
    do {
      result <<= 4;
      if (shifted > 0x0FFFFFFF)
        result += shifted >> 28;
      else
        result += fallback >> 28;
      j--;
      shifted <<= 4;
      fallback <<= 4;
    } while (j >= 0);

    *output++ = result;
    i++;
  } while (i <= 7);
}

/// Draws the two page arrows (gPageArrowTiles) into the two tiles at
/// VRAM address `offset`, sliding outward: the left arrow 2 or 3 pixels to
/// the left, the right one as far to the right, eight frames each. They are
/// drawn over the border's top edge tile (WINDOW_TILE_BORDER_TOP) and top-right
/// corner tile (WINDOW_TILE_BORDER_TOP_RIGHT). The battle menus call it each
/// frame for LIST_PAGE_ARROW_VRAM; the field menus (menu/menu, by far call) and
/// the password pages of the title's Send (scene/file_screen.c) call it for
/// PAGE_ARROW_VRAM.
void Link_DrawPageArrows(s32 offset) {
  s32 phase = (gFrameCount >> 2) & 3;

  if (phase > 2) {
    phase = 2;
  }
  if (phase <= 0) {
    phase = 1;
  }
  phase = phase + 1;
  Graphics_MergeShiftedTileRows((u32 *)0x06000220, (u32 *)gRomShiftedTilePair,
                                (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows((u32 *)0x06000240,
                                (u32 *)(gRomShiftedTilePair + 32),
                                (u32 *)(offset + 32), phase);
}

/// Link_DrawPageArrows with both arrows merged into the one tile at VRAM
/// 0x06000400; the battle's status screen calls it each frame.
void BattleStatus_DrawPageArrows(s32 offset) {
  s32 phase = (gFrameCount >> 2) & 3;

  if (phase > 2) {
    phase = 2;
  }
  if (phase <= 0) {
    phase = 1;
  }
  phase = phase + 1;
  Graphics_MergeShiftedTileRows((u32 *)0x06000400, (u32 *)gRomShiftedTilePair,
                                (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows((u32 *)0x06000400,
                                (u32 *)(gRomShiftedTilePair + 32),
                                (u32 *)(offset + 32), phase);
}

void Graphics_ExpandVramTilesByColorTable(u16 *dst) {
  s32 bank = 0;
  s32 palOffset = 0;
  s32 dstBank = 0;

  do {
    s32 row = 0;
    s32 dstOffset = (dstBank + bank) << 6;

    do {
      u16 *out = (u16 *)((u8 *)dst + dstOffset);
      u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
      s32 col = 0;

      do {
        u32 packed = *src++;
        u32 dec = 0;
        s32 nibble = 0;

        do {
          u32 color = gGraphics_ExpandNibbleTable[(packed & 15) + palOffset]
                      << (nibble * 4);
          nibble++;
          packed >>= 4;
          dec |= color;
        } while (nibble <= 3);

        col++;
        *out++ = dec;
      } while (col <= 15);

      row++;
      dstOffset += 32;
    } while (row <= 9);

    palOffset += 16;
    dstBank += 4;
    bank++;
  } while (bank <= 1);
}

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index) {
  s32 result;
  u8 *work;

  work = HeapSlot_AllocEwram(0x44, 0x608);
  Icon_DrawMisc(resource);
  result = VramCache_Reload(index, (s32)(work + 0x400));
  HeapSlot_Release(0x44);
  return result;
}

s32 ItemIcon_LoadIntoSlot(s32 resource, s32 index) {
  s32 result;
  u8 *work;

  work = HeapSlot_AllocEwram(0x44, 0x608);
  ItemIcon_Compose(resource, 0x1a);
  result = VramCache_Reload(index, (s32)(work + 0x400));
  HeapSlot_Release(0x44);
  return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index) {
  s32 out;
  s32 cur;
  s32 ret;
  u8 *work;

  work = HeapSlot_AllocEwram(0x44, 0x608);
  cur = index;
  Ability_LoadGlyph(resource, 0, &cur, &out, 1);
  ret = VramCache_Reload(index, (s32)(work + 0x400));
  HeapSlot_Release(0x44);
  return ret;
}

s32 Portrait_Load(u32 entry, s32 slot) {
  s32 tile;

  if (entry > 7)
    entry = 0;
  Icon_LoadWithPalette(Portrait_GetForSpeaker(entry), 0, &slot, &tile, 14, 1);
  return tile;
}

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_Decompress4bppCodeSize[];

/// Returns gWindow_FixedTileBlocks[0], whatever no is.
s32 WindowSprite_GetPointerTiles(s32 no) {
  if (no != 0)
    no = 0;
  return gWindow_FixedTileBlocks[no];
}

/// Copies the 4bpp decompression routine to RAM and runs it on the stream
/// whose address the work holds at 0x604, decoding into the work.
void Icon_Decompress(u8 *work) {
  void (*routine)(void *, void *);

  RamCode_Load(routine, Tile_Decompress4bppCode, Tile_Decompress4bppCodeSize);
  routine(*(void **)(work + 0x604), work);
  Heap_Release(routine);
}

#define Text_DrawStringAtOffset                                                \
  ((void (*)(u8 *, s32, s32, s32))Text_DrawStringAtOffset)

/// Opens a 6x4 window at the upper left, draws its label and returns its
/// handle.
s32 Link_CreateCountdownLabelWindow(void) {
  s32 win = (s32)Window_Create(0, 0, 6, 4, 6);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  Text_DrawStringAtOffset(gLink_TimeLabelString, win, 0, 0);
#else
  Text_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
  return win;
}

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1) {
  GlyphTransfer *buffer = HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  u16 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  u8 **slot = &buffer->encoded;
  void *target = (void *)((u32)base + base[arg1]);
  s32 ret;

  *slot = target;
  Resource_DecodeByteLzInRam(target, buffer);
  ret = VramCache_Reload(arg0, (s32)buffer);
  HeapSlot_Release(68);
  return ret;
}

void BattleCommandRow_LoadIconTiles(struct GlyphPalette *palette, s32 glyph,
                                    s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *src;
  u8 *dst;
  s32 i;
  u32 index;

  decoded = (GlyphTransfer *)HeapSlot_Alloc(68, sizeof(*decoded));
  font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
  decoded->encoded = font + ((u16 *)font)[glyph];
  Resource_DecodeByteLzInRam(decoded->encoded, decoded);
  remapped = Heap_Alloc(0x400);
  src = decoded->input;
  dst = remapped;
  for (i = 0; i < 0x400; i++) {
    index = *src++;
    if (palette->slot[index] == 255) {
      palette->slot[index] = palette->count;
      if (palette->count < 64) {
        ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
        palette->count++;
      }
    }
    *dst++ = palette->slot[index];
  }
  Dma_Set(remapped, BG_CHAR_BLOCK(1) + tile * 64, DMA_COPY_WORDS(256),
          REG_DMA3);
  Heap_Release(remapped);
  HeapSlot_Release(68);
}

typedef struct {
  u16 code : 10;
  u16 style : 6;
} CharacterCell;

void BattleCommandRow_LoadIcon(u8 *base, s32 index, u32 value) {
  u8 *entry;
  s32 offset;
  s32 store_offset;
  s32 load_offset;

  offset = index * 28;
  entry = base + offset + 0x104;
  BattleCommandRow_LoadIconTiles((struct GlyphPalette *)base, value,
                                 index * 16);
  store_offset = offset + 0x11C;
  *(u32 *)(base + store_offset) = value;
  // The sprite's shape and its cleared attributes 2 and 3 in a once-only
  // block; as plain stores, 4 of 47 instructions differ in tla-en.
  do {
    *(u32 *)(entry + 4) = 0x80002000;
    *(u32 *)(entry + 8) = 0;
  } while (0);
  load_offset = offset + 0x110;
  ((CharacterCell *)(entry + 8))->code =
      Resource_LoadIndexedIntoBuffer(*(u16 *)(base + load_offset), value);
}

/// Puts BG1 behind the other layers (priority 3). The command row runs it as
/// a task each VBlank, in the VBlank pass, with CommandRow_PutBg1InFront at
/// line 136 (tile row 17, the top of the row's icons), so that BG1 is behind
/// above the strip and in front from it down.
void CommandRow_PutBg1Behind(void) {
  u32 mask = 4;
  u32 value = REG_BG1CNT;

  mask = -mask;
  value &= mask;
  value |= 3;
  REG_BG1CNT = value;
}

/// Brings BG1 to the front (priority 0): the command row's VCount handler,
/// run at line 136, the top of its icon strip (tile row 17).
void CommandRow_PutBg1InFront(void) {
  u32 mask = 4;
  u32 value = REG_BG1CNT;

  mask = -mask;
  value &= mask;
  REG_BG1CNT = value;
}

/// Clears BG0's vertical scroll (REG_BG0VOFS) and installs
/// CommandRow_PutBg1InFront, which clears BG1's priority bits, as the VCount
/// handler at line 136. Nothing calls it.
UNUSED void Graphics_InstallBg1VCountHandler(void) {
  Io_Write16(0, &REG_BG0VOFS);
  Irq_SetHandler(2, 136, CommandRow_PutBg1InFront);
}

#undef Resource_DecodeByteLzInRam
#undef Text_DrawStringAtOffset
