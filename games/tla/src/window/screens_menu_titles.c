/// @file
/// Pictures and command icons loaded for the menus, and BG1's priority.
// Part of window.c.

#include "window_internal.h"
#include "system/io_write.h"
#include "window/screens.h"
#include "resource_ids.h"
#include "macros.h"

#define Decompress_ByteLz ((void (*)(void *, void *))Decompress_ByteLz)

/// Decompresses picture `picture` of gRenderResource_PairSourceTable, a 64x16
/// heading (Save, Continue, Copy, Erase, Battle, Link, Pause, Setup) of 512
/// bytes, into VRAM slot `slot`, through a 1 KB staging block in
/// HEAP_SLOT_TILE_SCRATCH. The heap block is not freed when `slot` is not a
/// valid VRAM slot.
void MenuTitle_Load(s32 picture, s32 slot) {
  void *stagingBuffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  s32 resource = gRenderResource_PairSourceTable[picture];

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Decompress_ByteLz((void *)resource, stagingBuffer);
    VramCache_Load(slot, 512, stagingBuffer);
    HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  }
}

/// Shows heading `picture` of gRenderResource_PairSourceTable as two 32x16
/// sprites side by side at (x, y) in `window`, on draw layer 253, and returns
/// the left one (the right one follows it in the window's list).
struct WindowSprite *MenuTitle_Create(s32 picture, struct Window *window, s32 x,
                                      s32 y) {
  s32 slot;
  struct WindowSprite *first;
  struct WindowSprite *second;

  slot = VramCache_FindFreeEntry();
  if (slot >= VRAM_CACHE_ENTRY_COUNT)
    return NULL;

  // The picture is 8 tiles. Neither sprite is checked for NULL.
  MenuTitle_Load(picture, slot);
  first = WindowSprite_Create(slot, OUTPUT_SIZE_32X16, window, x, y);
  first->depth = OAM_DEPTH_FRONT - 2;
  second = WindowSprite_Create(slot, OUTPUT_SIZE_32X16, window, x + 32, y);
  second->depth = OAM_DEPTH_FRONT - 2;
  // The right half's tiles, after the left half's 8.
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
void MenuTitle_Bob(struct WindowSprite *obj) {
  s32 value;

  value = (u8)obj->y + gWindow_PairBobOffsets[(gVBlankCount >> 2) & 7];
  obj->sprite.oam.y = value;
  obj = obj->next;
  value = (u8)obj->y + gWindow_PairBobOffsets[(gVBlankCount >> 2) & 7];
  obj->sprite.oam.y = value;
}

/// Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB)
/// into VRAM slot `slot`, remapping its first 768 bytes' colours by table
/// when `remap` is nonzero.
void CommandIcon_Load(s32 icon, s32 slot, s32 remap) {
  s32 size = 1024;
  void *buffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, size);
  u16 *offsetTable = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Decompress_ByteLz((void *)((u32)offsetTable + offsetTable[icon]), buffer);
    if (remap != 0)
      SpriteColor_RemapPixelsFar(buffer, 768);
    VramCache_Load(slot, size, buffer);
    HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  }
}

/// Shows command icon `icon` (see CommandIcon_Load) as a 32x32
/// 256-colour sprite at (x, y) in `window`, on draw layer 251. Returns NULL
/// when no VRAM slot is free. The settings menu's icons come through here
/// (window/settings_menu.c).
struct WindowSprite *CommandIcon_Create(s32 icon, s32 remap,
                                        struct Window *window, s32 x, s32 y) {
  s32 index;
  struct WindowSprite *entity;

  index = VramCache_FindFreeEntry();
  entity = NULL;
  if (index != VRAM_CACHE_ENTRY_COUNT) {
    CommandIcon_Load(icon, index, remap);
    entity = WindowSprite_Create(index, OUTPUT_SIZE_32X32, window, x, y);
    entity->sprite.oam.fullColor = 1;
    entity->depth = OAM_DEPTH_FRONT - 4;
  }
  return entity;
}

/// Makes the sprite `obj` pulse: scales it about its centre by
/// gWindowSprite_PulseScales (in 1/65536, used in 1/256), one step every two
/// frames, and puts it on draw layer 252.
void WindowSprite_Pulse(struct WindowSprite *obj) {
  s32 scale = gWindowSprite_PulseScales[(gVBlankCount >> 1) & 7] / 256;
  struct AffineTransform effect;

  if (obj != 0) {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    obj->sprite.oam.matrix = OamMatrix_AllocFromTransform(&effect);
    // Affine mode 3 doubles the sprite's box, so it moves up and left by 16
    // pixels (0xfff0 and 0xf0 wrap to -16) to keep its centre.
    obj->sprite.oam.affineMode = 3;
    obj->sprite.oam.x = obj->x + 0xfff0;
    obj->sprite.oam.y = (u8)obj->y + 0xf0;
    obj->depth = OAM_DEPTH_FRONT - 3;
  }
}

/// Returns the Djinn tutorial's message on giving a Djinni: the second set's
/// while FLAG_ALREADY_KNOWS_DJINN is set.
s32 Djinn_GetGiveTutorialMessage(void) {
  if (GameFlag_Get(FLAG_ALREADY_KNOWS_DJINN))
    return (s32)MsgDjinnGiveTutorialB;
  return (s32)MsgDjinnGiveTutorialA;
}

/// One word of a tile marked by XOR with `mask`.
static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT 6
#define RANGE_PATTERN_PAIR_BYTES (2 * TILE_SIZE_4BPP)
/// The first of the range icons' BG tiles, which Window_DrawThreeTileColumn
/// draws as tilemap entries.
#define RANGE_ICON_FIRST_TILE 788

/// Builds the twelve range icons Window_DrawThreeTileColumn draws, as the 24 BG
/// tiles from RANGE_ICON_FIRST_TILE: icon n is a centre tile and an end tile,
/// filled with colour 4, then rows 1 to 7 marked by XOR with
/// gMenu_BarPatternRows. The first set grows its pattern with the row, the
/// second skips row 1.
UNREAD_RESULT Menu_BuildLocalizedPatternTiles(void) {
  s32 patternSet;
  s32 patternIndex;

  for (patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
    for (patternIndex = 0; patternIndex < RANGE_PATTERN_COUNT; patternIndex++) {
      u32 *tile = (u32 *)(patternSet *
                              (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_BYTES) +
                          patternIndex * RANGE_PATTERN_PAIR_BYTES +
                          (u32)BG_CHAR_BLOCK(0) +
                          RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP);
      s32 row;

      Iwram_FillWordsEntry(tile, RANGE_PATTERN_PAIR_BYTES,
                           TEXT_CANVAS_BACKGROUND_WORD);
      for (row = 1; row <= 7; row++) {
        s32 sourcePattern = patternIndex;

        if (patternSet == 1 && row <= 1) {
          continue;
        }
        if (patternSet == 0 && patternIndex > row - 2) {
          sourcePattern = row - 2;
          if (sourcePattern < 0) {
            sourcePattern = 0;
          }
        }
        tile[row] =
            XorWord(tile[row], gMenu_BarPatternRows[sourcePattern].firstTile);
        tile[row + 8] = XorWord(tile[row + 8],
                                gMenu_BarPatternRows[sourcePattern].secondTile);
      }
    }
  }
}

/// Draws range icon `pattern` across tiles (x, y) to (x + 2, y) of `window`'s
/// interior: its end tile mirrored, its centre tile and its end tile, in the UI
/// palette.
UNREAD_RESULT Window_DrawThreeTileColumn(struct Window *window, s32 x, s32 y,
                                         s32 pattern, s32 unused) {
  s32 patternOffset = pattern * 2;
  s32 rightTile = patternOffset + WINDOW_TILE(RANGE_ICON_FIRST_TILE + 1);

  Window_SetTilemapEntry(window, TILE_HFLIP | rightTile, x, y, 0);
  Window_SetTilemapEntry(
      window, patternOffset + WINDOW_TILE(RANGE_ICON_FIRST_TILE), x + 1, y, 0);
  Window_SetTilemapEntry(window, rightTile, x + 2, y, 0);
}

/// Merges two 8x8 4bpp tiles row by row into `output`: `second` moved `shift`
/// pixels (right when positive, left when negative), drawn over `first`, which
/// shows wherever the moved pixel is colour 0.
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift) {
  s32 oppositeShift;
  s32 rightShift;
  s32 leftShift;
  s32 row;

  oppositeShift = -shift;
  row = 0;
  rightShift = oppositeShift * 4;
  leftShift = shift * 4;

  do {
    u32 result;
    u32 shifted;
    u32 fallback;
    s32 pixel;

    shifted = *second++;
    result = 0;
    fallback = *first++;
    if (shift < 0)
      shifted >>= rightShift;
    else
      shifted <<= leftShift;

    pixel = 7;
    do {
      result <<= 4;
      if (shifted > 0x0FFFFFFF)
        result += shifted >> 28;
      else
        result += fallback >> 28;
      pixel--;
      shifted <<= 4;
      fallback <<= 4;
    } while (pixel >= 0);

    *output++ = result;
    row++;
  } while (row <= 7);
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
  Graphics_MergeShiftedTileRows(
      (u32 *)(BG_CHAR_BLOCK(0) +
              (WINDOW_TILE_BORDER_TOP & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
      (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows(
      (u32 *)(BG_CHAR_BLOCK(0) +
              (WINDOW_TILE_BORDER_TOP_RIGHT & TILE_NUMBER_MASK) *
                  TILE_SIZE_4BPP),
      (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}

/// Link_DrawPageArrows for the Status screen's own arrows: both slide over the
/// window's fill tile (WINDOW_TILE_WINDOW_FILL) rather than the border's. The
/// battle's status screen calls it each frame.
void BattleStatus_DrawPageArrows(s32 offset) {
  s32 phase = (gFrameCount >> 2) & 3;

  if (phase > 2) {
    phase = 2;
  }
  if (phase <= 0) {
    phase = 1;
  }
  phase = phase + 1;
  Graphics_MergeShiftedTileRows(
      (u32 *)(BG_CHAR_BLOCK(0) +
              (WINDOW_TILE_WINDOW_FILL & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
      (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows(
      (u32 *)(BG_CHAR_BLOCK(0) +
              (WINDOW_TILE_WINDOW_FILL & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
      (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}

/// Copies the UI's ten digit tiles ('0' to '9', tiles 48 to 57) twice into
/// `dst`, each pixel mapped through gGraphics_ExpandNibbleTable: first with
/// colour 1 drawn as colour 10, then unchanged.
void Graphics_ExpandVramTilesByColorTable(u16 *dst) {
  s32 bank = 0;
  s32 paletteOffset = 0;
  s32 destinationBank = 0;

  // bank 0 writes from byte 0, bank 1 from (4 + 1) << 6 = 320, just after
  // the first ten tiles of 32 bytes. Each "row" is one tile.
  do {
    s32 row = 0;
    s32 destinationOffset = (destinationBank + bank) << 6;

    do {
      u16 *output = (u16 *)((u8 *)dst + destinationOffset);
      u16 *source =
          (u16 *)(BG_CHAR_BLOCK(0) + '0' * TILE_SIZE_4BPP + (row << 5));
      s32 column = 0;

      do {
        u32 packed = *source++;
        u32 expanded = 0;
        s32 nibble = 0;

        do {
          u32 color = gGraphics_ExpandNibbleTable[(packed & 15) + paletteOffset]
                      << (nibble * 4);
          nibble++;
          packed >>= 4;
          expanded |= color;
        } while (nibble <= 3);

        column++;
        *output++ = expanded;
      } while (column <= 15);

      row++;
      destinationOffset += 32;
    } while (row <= 9);

    paletteOffset += 16;
    destinationBank += 4;
    bank++;
  } while (bank <= 1);
}

/// Builds icon `resource` (0 to 31) of gIcon_MiscIconPointers, the table the
/// status icons come from (Icon_DrawMisc), in a transfer block taken from
/// HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM cache entry `index` with it and
/// returns the entry's first tile.
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*work));
  Icon_DrawMisc(resource);
  result = VramCache_Reload(index, (s32)work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return result;
}

/// Builds the icon of inventory item `resource` (the count above one and both
/// marks) in a transfer block taken from HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM
/// cache entry `index` with it and returns the entry's first tile.
s32 ItemIcon_LoadIntoSlot(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*work));
  ItemIcon_Compose(resource,
                   ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN);
  result = VramCache_Reload(index, (s32)work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return result;
}

/// Builds the icon of ability `resource` in a transfer block taken from heap
/// slot 17, reloads VRAM cache entry `index` with it and returns the entry's
/// first tile.
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index) {
  s32 tile;
  s32 slot;
  s32 result;
  GlyphTransfer *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*work));
  slot = index;
  Ability_LoadGlyph(resource, 0, &slot, &tile, 1);
  result = VramCache_Reload(index, (s32)work->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return result;
}

/// Loads the portrait of character `member` (ISAAC to PIERS, 0 to 7; any higher
/// value shows Isaac's) into VRAM cache entry `cacheSlot`, in PORTRAIT_PALETTE,
/// and returns its first tile.
s32 Portrait_Load(u32 member, s32 cacheSlot) {
  s32 tile;

  if (member > PIERS)
    member = ISAAC;
  Icon_LoadWithPalette(Portrait_GetForSpeaker(member), 0, &cacheSlot, &tile,
                       PORTRAIT_PALETTE, 1);
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

/// Copies the 4bpp decoder into RAM, calls it with the transfer's encoded
/// source and byte buffer, then releases the copied code.
UNUSED void Icon_Decompress(GlyphTransfer *work) {
  void (*routine)(void *, void *);

  RamCode_Load(routine, Tile_Decompress4bppCode, Tile_Decompress4bppCodeSize);
  routine(work->encoded, work->input);
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

/// Decodes command icon `icon` (ResourceId_CommandIcons, 1 KB of 256-colour
/// pixels) in a block taken from HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM cache
/// entry `slotIndex` with it and returns the entry's first tile.
s32 Resource_LoadIndexedIntoBuffer(s32 slotIndex, s32 icon) {
  GlyphTransfer *buffer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  u16 *base = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);
  u8 **slot = &buffer->encoded;
  void *target = (void *)((u32)base + base[icon]);
  s32 ret;

  *slot = target;
  Decompress_ByteLz(target, buffer);
  ret = VramCache_Reload(slotIndex, (s32)buffer);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return ret;
}

/// Loads command icon `glyph` as 8bpp BG tiles into character block 1 from
/// 64-byte tile `tile` on (16 tiles, 32x32), for the command row's BG1.
void BattleCommandRow_LoadIconTiles(struct GlyphPalette *palette, s32 glyph,
                                    s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *source;
  u8 *destination;
  s32 i;
  u32 index;

  decoded = (GlyphTransfer *)HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER,
                                            sizeof(*decoded));
  font = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);
  decoded->encoded = font + ((u16 *)font)[glyph];
  Decompress_ByteLz(decoded->encoded, decoded);
  remapped = Heap_Alloc(32 * 32);
  source = decoded->input;
  destination = remapped;
  for (i = 0; i < 32 * 32; i++) {
    index = *source++;
    if (palette->slot[index] == 255) {
      palette->slot[index] = palette->count;
      if (palette->count < 64) {
        ((u16 *)BG_PLTT)[palette->count] = ((u16 *)OBJ_PLTT)[index];
        palette->count++;
      }
    }
    *destination++ = palette->slot[index];
  }
  Dma_Set(remapped, BG_CHAR_BLOCK(1) + tile * 64, DMA_COPY_WORDS(256),
          REG_DMA3);
  Heap_Release(remapped);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

typedef struct {
  u16 code : 10;
  u16 style : 6;
} CharacterCell;

/// Loads the command row's icon `index` as command icon `icon`: as BG1 tiles
/// from tile index * 16 (BattleCommandRow_LoadIconTiles), and into the row's
/// shared VRAM cache entry for its 32x32 sprite, which BattleCommandRow_Run
/// shows only for the selected icon.
void BattleCommandRow_LoadIcon(struct BattleCommandRowWork *work, s32 index,
                               u32 icon) {
  struct BattleCommandRowIcon *entry = &work->entries[index];

  BattleCommandRow_LoadIconTiles((struct GlyphPalette *)work, icon, index * 16);
  work->entries[index].kind = icon;
  MenuSprite_Initialize(entry->sprite, OBJ_SIZE_32X32 | OBJ_256_COLORS);
  entry->sprite.oam.f.tile =
      Resource_LoadIndexedIntoBuffer(work->entries[index].resource, icon);
}

/// Puts BG1 behind the other layers (priority 3). The command row runs it as
/// a task each VBlank, in the VBlank pass, with CommandRow_PutBg1InFront at
/// line 136 (tile row 17, the top of the row's icons), so that BG1 is behind
/// above the strip and in front from it down.
void CommandRow_PutBg1Behind(void) {
  u32 value = REG_BG1CNT;

  value &= ~BGCNT_PRIORITY(3);
  value |= BGCNT_PRIORITY(3);
  REG_BG1CNT = value;
}

/// Brings BG1 to the front (priority 0): the command row's VCount handler,
/// run at line 136, the top of its icon strip (tile row 17).
void CommandRow_PutBg1InFront(void) {
  u32 value = REG_BG1CNT;

  value &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT = value;
}

/// Clears BG0's vertical scroll (REG_BG0VOFS) and installs
/// CommandRow_PutBg1InFront, which clears BG1's priority bits, as the VCount
/// handler at line 136. Nothing calls it.
UNUSED void Graphics_InstallBg1VCountHandler(void) {
  Io_Write16(0, &REG_BG0VOFS);
  Irq_SetHandler(IRQ_VCOUNT, 136, CommandRow_PutBg1InFront);
}

#undef Decompress_ByteLz
#undef Text_DrawStringAtOffset
