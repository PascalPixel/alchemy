/// @file
/// Pictures and command icons loaded for the menus, and BG1's priority.
// Part of screens.c.

#include "window_internal.h"
#include "system/io_write.h"
#include "window/screens.h"
#include "resource_ids.h"
#include "macros.h"

/// A row of a command icon's 8x8 tiles, four across, as a sprite holds them.
#define COMMAND_ICON_TILE_ROW_BYTES (4 * TILE_SIZE_8BPP)

/// Decompresses picture `picture` of gMenuTitlePictures, a 64x16
/// heading (Save, Continue, Copy, Erase, Battle, Link, Pause, Setup) of 512
/// bytes, into VRAM slot `slot`, through a 1 KB staging block in
/// HEAP_SLOT_TILE_SCRATCH. The heap block is not freed when `slot` is not a
/// valid VRAM slot.
void MenuTitle_Load(s32 picture, s32 slot) {
  void *stagingBuffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  s32 resource = gMenuTitlePictures[picture];

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Decompress_ByteLz((void *)resource, stagingBuffer);
    VramCache_Load(slot, 512, stagingBuffer);
    HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  }
}

/// Shows heading `picture` of gMenuTitlePictures as two 32x16
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
/// by gMenuTitleBobOffsets, one step every four VBlanks.
void MenuTitle_Bob(struct WindowSprite *obj) {
  s32 value;

  value = (u8)obj->y + gMenuTitleBobOffsets[(gVBlankCount >> 2) & 7];
  obj->sprite.oam.y = value;
  obj = obj->next;
  value = (u8)obj->y + gMenuTitleBobOffsets[(gVBlankCount >> 2) & 7];
  obj->sprite.oam.y = value;
}

/// Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB)
/// into VRAM slot `slot`, remapping the colours of its top three rows of tiles
/// by table when `remap` is nonzero.
void CommandIcon_Load(s32 icon, s32 slot, s32 remap) {
  s32 size = COMMAND_ICON_BYTES;
  void *buffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, size);
  u16 *offsetTable = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Decompress_ByteLz((void *)((u32)offsetTable + offsetTable[icon]), buffer);
    if (remap != 0)
      SpriteColor_RemapPixelsFar(buffer, 3 * COMMAND_ICON_TILE_ROW_BYTES);
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
  s32 slot;
  struct WindowSprite *sprite;

  slot = VramCache_FindFreeEntry();
  sprite = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    CommandIcon_Load(icon, slot, remap);
    sprite = WindowSprite_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
    sprite->sprite.oam.fullColor = 1;
    sprite->depth = OAM_DEPTH_FRONT - 4;
  }
  return sprite;
}

/// Makes the sprite `obj` pulse: scales it about its centre by
/// gWindowSprite_PulseScales (16.16, used as 8.8), one step every two frames,
/// and puts it on draw layer 252.
void WindowSprite_Pulse(struct WindowSprite *obj) {
  s32 scale = gWindowSprite_PulseScales[(gVBlankCount >> 1) & 7] /
              (FIXED_ONE / Q_8_8(1));
  struct AffineTransform effect;

  if (obj != 0) {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    obj->sprite.oam.matrix = OamMatrix_AllocFromTransform(&effect);
    // Affine mode 3 doubles the sprite's box, so it moves up and left by 16
    // pixels (-16 in the 16-bit x and 8-bit y) to keep its centre.
    obj->sprite.oam.affineMode = 3;
    obj->sprite.oam.x = obj->x + (u16)-16;
    obj->sprite.oam.y = (u8)obj->y + (u8)-16;
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

/// A 4bpp tile row's eighth pixel, in the top four bits of its word.
#define ROW_LAST_PIXEL_SHIFT 28

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

  for (; row <= 7; row++) {
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
      if (shifted >= 1u << ROW_LAST_PIXEL_SHIFT)
        result += shifted >> ROW_LAST_PIXEL_SHIFT;
      else
        result += fallback >> ROW_LAST_PIXEL_SHIFT;
      pixel--;
      shifted <<= 4;
      fallback <<= 4;
    } while (pixel >= 0);

    *output++ = result;
  }
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
      (u32 *)(gRomShiftedTilePair + TILE_SIZE_4BPP),
      (u32 *)(offset + TILE_SIZE_4BPP), phase);
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
      (u32 *)(gRomShiftedTilePair + TILE_SIZE_4BPP),
      (u32 *)(offset + TILE_SIZE_4BPP), phase);
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
  for (; bank <= 1; bank++) {
    s32 row = 0;
    s32 destinationOffset = (destinationBank + bank) << 6;

    do {
      u16 *output = (u16 *)((u8 *)dst + destinationOffset);
      u16 *source =
          (u16 *)(BG_CHAR_BLOCK(0) + '0' * TILE_SIZE_4BPP + (row << 5));
      s32 column;

      for (column = 0; column < TILE_SIZE_4BPP / 2; column++) {
        u32 packed = *source++;
        u32 expanded = 0;
        s32 nibble = 0;

        do {
          u32 color =
              gGraphics_ExpandNibbleTable[(packed & 0xf) + paletteOffset]
              << (nibble * 4);
          nibble++;
          packed >>= 4;
          expanded |= color;
        } while (nibble <= 3);

        *output++ = expanded;
      }

      row++;
      destinationOffset += TILE_SIZE_4BPP;
    } while (row <= 9);

    paletteOffset += PALETTE_COLORS;
    destinationBank += 4;
  }
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

/// Builds the icon of ability `resource` in a transfer block taken from
/// HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM cache entry `index` with it and
/// returns the entry's first tile.
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

/// The 4bpp decoder Icon_Decompress copies into RAM.
extern u8 Tile_Decompress4bppCode[];
/// The decoder's byte count, as this symbol's address (ld_script.ld).
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

/// Opens a 6x4 window at the upper left, draws its label and returns its
/// handle.
s32 Link_CreateCountdownLabelWindow(void) {
  s32 win = (s32)Window_Create(0, 0, 6, 4, 6);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  Text_DrawStringAtOffset(gLink_TimeLabelString, (struct Window *)win, 0, 0);
#else
  Text_DrawStringInWindow(gLink_TimeLabelString, win, 0, 0);
#endif
  return win;
}

/// Decodes command icon `icon` (RESOURCE_COMMAND_ICONS, 1 KB of 256-colour
/// pixels) in a block taken from HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM cache
/// entry `slotIndex` with it and returns the entry's first tile.
s32 Resource_LoadIndexedIntoBuffer(s32 slotIndex, s32 icon) {
  GlyphTransfer *buffer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  u16 *offsetTable = Resource_GetTableEntry(RESOURCE_COMMAND_ICONS);
  u8 **encoded = &buffer->encoded;
  void *target = (void *)((u32)offsetTable + offsetTable[icon]);
  s32 tile;

  *encoded = target;
  Decompress_ByteLz(target, buffer);
  tile = VramCache_Reload(slotIndex, (s32)buffer);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return tile;
}

/// The command icons on BG1 share at most 64 BG colours, given out in the order
/// their pixels first use them; a colour met after that takes colour 64.
#define COMMAND_ICON_COLORS 64
/// A command icon as BG1 tiles: 32 by 32 pixels, 16 tiles of 256 colours.
#define COMMAND_ICON_BG_TILES (COMMAND_ICON_BYTES / TILE_SIZE_8BPP)

/// Loads command icon `glyph` as 8bpp BG tiles into character block 1 from
/// 64-byte tile `tile` on (COMMAND_ICON_BG_TILES, 32x32), for the command row's
/// BG1.
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
  remapped = Heap_Alloc(COMMAND_ICON_BYTES);
  source = decoded->input;
  destination = remapped;
  for (i = 0; i < COMMAND_ICON_BYTES; i++) {
    index = *source++;
    if (palette->slot[index] == GLYPH_PALETTE_SLOT_FREE) {
      palette->slot[index] = palette->count;
      if (palette->count < COMMAND_ICON_COLORS) {
        BG_PLTT[palette->count] = OBJ_PLTT[index];
        palette->count++;
      }
    }
    *destination++ = palette->slot[index];
  }
  Dma_Set(remapped, BG_CHAR_BLOCK(1) + tile * TILE_SIZE_8BPP,
          DMA_COPY_WORDS(COMMAND_ICON_BYTES / 4), REG_DMA3);
  Heap_Release(remapped);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Loads the command row's icon `index` as command icon `icon`: as BG1 tiles
/// from tile index * COMMAND_ICON_BG_TILES (BattleCommandRow_LoadIconTiles),
/// and into the row's shared VRAM cache entry for its 32x32 sprite, which
/// BattleCommandRow_Run shows only for the selected icon.
void BattleCommandRow_LoadIcon(struct BattleCommandRowWork *work, s32 index,
                               u32 icon) {
  struct BattleCommandRowIcon *entry = &work->entries[index];

  BattleCommandRow_LoadIconTiles(&work->palette, icon,
                                 index * COMMAND_ICON_BG_TILES);
  work->entries[index].kind = icon;
  MenuSprite_Initialize(entry->sprite, OBJ_SIZE_32X32 | OBJ_256_COLORS);
  entry->sprite.oam.f.tile =
      Resource_LoadIndexedIntoBuffer(work->entries[index].resource, icon);
}

/// Puts BG1 behind the other layers (priority 3). The command row runs it as
/// a task each VBlank, in the VBlank pass, with CommandRow_PutBg1InFront at
/// line COMMAND_ROW_TOP * 8, so that BG1 is behind above the strip and in front
/// from it down.
void CommandRow_PutBg1Behind(void) {
  u32 value = REG_BG1CNT;

  value &= ~BGCNT_PRIORITY(3);
  value |= BGCNT_PRIORITY(3);
  REG_BG1CNT = value;
}

/// Brings BG1 to the front (priority 0): the command row's VCount handler,
/// run at line COMMAND_ROW_TOP * 8, the top of its icon strip.
void CommandRow_PutBg1InFront(void) {
  u32 value = REG_BG1CNT;

  value &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT = value;
}

/// Clears BG0's vertical scroll (REG_BG0VOFS) and installs
/// CommandRow_PutBg1InFront, which clears BG1's priority bits, as the VCount
/// handler at line COMMAND_ROW_TOP * 8. Nothing calls it.
UNUSED void Graphics_InstallBg1VCountHandler(void) {
  Io_Write16(0, &REG_BG0VOFS);
  Irq_SetHandler(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, CommandRow_PutBg1InFront);
}

#undef COMMAND_ICON_TILE_ROW_BYTES
#undef ROW_LAST_PIXEL_SHIFT
#undef COMMAND_ICON_COLORS
#undef COMMAND_ICON_BG_TILES
