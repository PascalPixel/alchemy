/// @file
/// Pictures and command icons loaded for the menus, and BG1's priority.
// Part of screens.c.

#include "../screens_internal.h"

extern const u8 *gMenuTitlePictures[];

/// Decompresses picture `picture` of gMenuTitlePictures, a 64x16
/// heading (Save, Continue, Copy, Erase, Battle, Link, Pause, Setup) of 512
/// bytes, into VRAM slot `slot`, through a 1 KB staging block in
/// HEAP_SLOT_TILE_SCRATCH. The heap block is not freed when `slot` is not a
/// valid VRAM slot.
void MenuTitle_Load(s32 picture, s32 slot) {
  void *stagingBuffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  const u8 *resource = gMenuTitlePictures[picture];

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Decompress_ByteLz(resource, stagingBuffer);
    VramCache_Load(slot, 512, stagingBuffer);
    HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  }
}

/// Shows heading `picture` of gMenuTitlePictures as two 32x16
/// sprites side by side at (x, y) in `window`, on draw layer 253, and
/// returns the left one (the right one follows it in the window's list).
/// Returns NULL when no VRAM slot is free. The file list shows its
/// mode's heading this way (FileList_SelectFile), and window/settings_menu.c
/// the settings menu's Setup and the Pause menu's Pause.
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
  first->sentinel = 253;
  second = WindowSprite_Create(slot, OUTPUT_SIZE_32X16, window, x + 32, y);
  second->sentinel = 253;
  // The right half's tiles, after the left half's 8.
  second->oam.index += 8;
  return first;
}

/// Bobs the two sprites of a pair (see MenuTitle_Create) up and down
/// by gMenuTitleBobOffsets, one step every four frames.
void MenuTitle_Bob(struct WindowSprite *obj) {
  s32 value;

  // volatile: the VBlank handler counts gVBlankCount, so each sprite reads it
  // afresh; like gFrameCount's, the qualifier is no device.
  value = (u8)obj->y +
          gMenuTitleBobOffsets[(*(volatile u32 *)&gVBlankCount >> 2) & 7];
  obj->oam.y = value;
  obj = obj->next;
  value = (u8)obj->y +
          gMenuTitleBobOffsets[(*(volatile u32 *)&gVBlankCount >> 2) & 7];
  obj->oam.y = value;
}

/// Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB)
/// into VRAM slot `slot`, remapping its first 768 bytes' colours by table
/// when `remap` is nonzero.
void CommandIcon_Load(s32 icon, s32 slot, s32 remap) {
  s32 size = 1024;
  void *buffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, size);
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

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
  s32 slot;
  struct WindowSprite *sprite;

  slot = VramCache_FindFreeEntry();
  sprite = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    CommandIcon_Load(icon, slot, remap);
    sprite = WindowSprite_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
    sprite->oam.fullColor = 1;
    sprite->sentinel = 251;
  }
  return sprite;
}

/// Makes the sprite `obj` pulse: scales it about its centre by
/// gWindowSprite_PulseScales (in 1/65536, used in 1/256), one step every two
/// frames, and puts it on draw layer 252. Does nothing when `obj` is NULL.
/// The settings menu pulses its selected icon with it each frame
/// (window/settings_menu.c).
void WindowSprite_Pulse(struct WindowSprite *obj) {
  s32 scale = gWindowSprite_PulseScales[(gVBlankCount >> 1) & 7];
  struct AffineTransform effect;

  if (scale < 0)
    scale += 255;
  scale >>= 8;

  if (obj != 0) {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    obj->oam.matrix = OamMatrix_AllocFromTransform(&effect);
    // Affine mode 3 doubles the sprite's box, so it moves up and left by 16
    // pixels (0xfff0 and 0xf0 wrap to -16) to keep its centre.
    obj->oam.affineMode = 3;
    obj->oam.x = obj->x + 0xfff0;
    obj->oam.y = (u8)obj->y + 0xf0;
    obj->sentinel = 252;
  }
}

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT 6
#define RANGE_PATTERN_PAIR_WORDS 16
/// The first of the range icons' BG tiles, which Window_DrawThreeTileColumn
/// draws as tilemap entries.
#define RANGE_ICON_FIRST_TILE 0x314

// Builds the twelve range icons Window_DrawThreeTileColumn draws, as
// the 24 BG tiles from RANGE_ICON_FIRST_TILE: icon n is a centre tile and an
// end tile, filled with WINDOW_BACKGROUND_COLOR, then marked row by row by XOR
// with gMenu_BarPatternRows (its colour 5 turns the fill to colour 1). Rows 1
// to 7 are marked; the first set grows its pattern with the row, the second
// skips row 1. Battle_CollectPartyCommands builds them as the command menus
// open. Icon 10's tiles, the two from DJINN_MENU_ARROW_TILE, are also where
// BattleDjinnMenu_Run puts its blinking arrow (gDjinnMenu_BlinkTile). It is
// declared to return s32 but returns no value, and no caller reads one; UBFIX
// makes it void, which changes the register its epilogue returns through.
#ifdef UBFIX
void Menu_BuildLocalizedPatternTiles(void) {
#else
s32 Menu_BuildLocalizedPatternTiles(void) {
#endif
  u32 *vram =
      (u32 *)(BG_CHAR_BLOCK(0) + RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP);
  s32 patternSet;
  s32 patternIndex;

  for (patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
    for (patternIndex = 0; patternIndex < RANGE_PATTERN_COUNT; patternIndex++) {
      u32 *tile =
          vram + patternSet * (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_WORDS) +
          patternIndex * RANGE_PATTERN_PAIR_WORDS;
      s32 row;

      Iwram_FillWords(tile, 64, TEXT_CANVAS_BACKGROUND_WORD);
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

// Draws range icon `pattern` across tiles (x, y) to (x + 2, y) of `window`'s
// interior: its end tile mirrored, its centre tile and its end tile, in the
// UI palette. Callers pass an ability's range less one (the aimed unit and
// range - 1 either side), or 11 for ABILITY_RANGE_ALL: the Psynergy list and
// the Djinn preview.
// It is declared to return int but returns no value, and no caller reads
// one; UBFIX makes it void, which changes its epilogue and the argument
// setup of one call in the Djinn preview.
#ifdef UBFIX
void Window_DrawThreeTileColumn(struct Window *window, s32 x, s32 y,
                                s32 pattern, s32 unused) {
#else
int Window_DrawThreeTileColumn(struct Window *window, s32 x, s32 y, s32 pattern,
                               s32 unused) {
#endif
  s32 patternOffset = pattern * 2;
  s32 rightTile = patternOffset + WINDOW_TILE(RANGE_ICON_FIRST_TILE + 1);

  Window_SetTilemapEntry(window, TILE_HFLIP | rightTile, x, y, 0);
  Window_SetTilemapEntry(
      window, patternOffset + WINDOW_TILE(RANGE_ICON_FIRST_TILE), x + 1, y, 0);
  Window_SetTilemapEntry(window, rightTile, x + 2, y, 0);
}

/// Merges two 8x8 4bpp tiles row by row into `output`: `second` moved
/// `shift` pixels (right when positive, left when negative), drawn over
/// `first`, which shows wherever the moved pixel is colour 0. Pixels moved
/// past the tile's edge are lost.
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

  // Phases 0 to 3 of a 16-frame cycle become shifts 2, 2, 3, 3.
  if (phase > 2) {
    phase = 2;
  }
  if (phase <= 0) {
    phase = 1;
  }
  phase++;
  Graphics_MergeShiftedTileRows(
      (u32 *)(BG_CHAR_BLOCK(0) +
              (WINDOW_TILE_BORDER_TOP & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
      (u32 *)gPageArrowTiles, (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows(
      (u32 *)(BG_CHAR_BLOCK(0) +
              (WINDOW_TILE_BORDER_TOP_RIGHT & TILE_NUMBER_MASK) *
                  TILE_SIZE_4BPP),
      (u32 *)(gPageArrowTiles + 32), (u32 *)(offset + 32), phase);
}

/// Copies the UI's ten digit tiles ('0' to '9', tiles 0x30 to 0x39) twice into
/// `dst`, each pixel mapped through gGraphics_ExpandNibbleTable: first with
/// colour 1 drawn as colour 10, then unchanged. The battle menus build their
/// page numbers this way at LIST_PAGE_DIGITS_TILE (LIST_TILE_PAGE_NUMBER), as
/// do the password pages of the title's Send (scene/file_screen.c) and the
/// field Djinn menu (menu/menu/djinn.c).
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

/// Builds icon `resource` (0 to 31) of gIcon_MiscIconPointers, the table
/// the status icons come from (Icon_DrawMisc), in a
/// transfer block taken from HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM cache entry
/// `index` with it and returns the entry's first tile. The heap block is
/// released before it returns. The Status screen loads its status icons
/// this way, by status kind (1 to 18).
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
/// cache entry `index` with it and returns the entry's first tile. The heap
/// block is released before it returns. The battle item list and the chosen
/// item's window use it.
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
/// first tile. The heap block is released before it returns. The chosen
/// Psynergy's and summon's windows and the Djinn preview use it.
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

/// Loads the portrait of character `member` (ISAAC to PIERS, 0 to 7; any
/// higher value shows Isaac's) into VRAM cache entry `cacheSlot`, in
/// PORTRAIT_PALETTE, and returns its first tile. In the prologue, Isaac and
/// Garet use their childhood portraits. The Status screen and the battle
/// command menus draw the party member's portrait with it.
s32 Portrait_Load(u32 member, s32 cacheSlot) {
  s32 tile;
  s32 slot;
  u32 portrait;

  portrait = member;
  slot = cacheSlot;
  if (portrait > (u32)PIERS) {
    portrait = 0;
  }
  if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0) {
    switch (portrait) {
    case ISAAC:
      portrait = FACE_ISAAC_CHILD;
      break;
    case GARET:
      portrait = FACE_GARET_CHILD;
      break;
    }
  }
  Icon_LoadWithPalette(portrait, 0, &slot, &tile, PORTRAIT_PALETTE, 1);
  return tile;
}

/// Returns gWindowSprite_PointerTiles[0], whatever `no` is. The battle's prompt
/// reads it by far call (BattleMessage_WaitForAdvanceAt).
s32 WindowSprite_GetPointerTiles(s32 no) {
  if (no != 0)
    no = 0;
  return gWindowSprite_PointerTiles[no];
}

/// Copies the 4bpp decoder into RAM, calls it with the transfer's encoded
/// source and byte buffer, then releases the copied code. Its ARM entry reads
/// the stream through r0 and writes bytes through r1, as Icon_Decode does.
UNUSED void Icon_Decompress(GlyphTransfer *work) {
  RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_Decompress4bpp,
                   Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))((union HeapState *)gHeapSlots)
       ->slots[HEAP_SLOT_RAM_CODE_TEMP])(work->encoded, work->input);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_TEMP);
}

// How the link timer's heading is drawn: Spanish and French at a pixel
// offset (Text_DrawStringAtOffset), the others one tile a character
// (Text_DrawStringInWindow).
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
#define LINK_COUNTDOWN_DRAW_HEADING Text_DrawStringAtOffset
#else
#define LINK_COUNTDOWN_DRAW_HEADING Text_DrawStringInWindow
#endif

/// Opens the link battle timer's window, 6 by 4 tiles at the top left, with
/// its heading (gLink_TimeLabelString), and returns it; the seconds go under
/// the heading (UpdateLinkSessionCountdown), which keeps the window in
/// BattleMenuWork.timerWindow until the commands are chosen.
struct Window *Link_CreateCountdownLabelWindow(void) {
  struct Window *win = Window_Create(0, 0, 6, 4, 6);

  LINK_COUNTDOWN_DRAW_HEADING(gLink_TimeLabelString, win, 0, 0);
  return win;
}

/// Decodes command icon `icon` (ResourceId_CommandIcons, 1 KB of 256-colour
/// pixels) in a block taken from HEAP_SLOT_GLYPH_TRANSFER, reloads VRAM cache
/// entry `slotIndex` with it and returns the entry's first tile. The heap block
/// is released before it returns. The command row and the chosen command's icon
/// use it.
s32 Resource_LoadIndexedIntoBuffer(s32 slotIndex, s32 icon) {
  GlyphTransfer *buffer =
      HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  u8 **encoded = &buffer->encoded;
  void *target = (void *)((u32)offsetTable + offsetTable[icon]);
  s32 result;

  *encoded = target;
  Decompress_ByteLz(target, buffer);
  result = VramCache_Reload(slotIndex, (s32)buffer);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return result;
}

/// Loads command icon `glyph` as 8bpp BG tiles into character block 1 from
/// 64-byte tile `tile` on (16 tiles, 32x32), for the command row's BG1. The
/// row's icons share `palette`: each colour index first met takes the next
/// BG palette entry, which gets that index's sprite palette colour. Past 64
/// entries every new index takes entry 64, whose colour is not set here.
/// HEAP_SLOT_GLYPH_TRANSFER holds the decoded icon and a 1 KB Heap_Alloc block
/// the remapped pixels; both are freed before it returns.
void Text_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *source;
  u8 *destination;
  s32 i;
  u32 index;

  decoded = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*decoded));
  font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
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
  Dma_Set(remapped, BG_CHAR_BLOCK(1) + tile * 64, DMA_COPY32(256), REG_DMA3);
  Heap_Release(remapped);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
}

/// Loads the command row's icon `index` as command icon `icon`: as BG1 tiles
/// from tile index * 16 (Text_LoadRemappedGlyph), and into the row's
/// shared VRAM cache entry for its 32x32 sprite, which BattleCommandRow_Run
/// shows only for the selected icon. The icon number is also its command
/// (BattleCommandRowIcon.kind), which names its label.
void Text_DrawCharacter(struct BattleCommandRowWork *work, s32 index,
                        u32 icon) {
  struct BattleCommandRowIcon *entry = &work->entries[index];

  Text_LoadRemappedGlyph(&work->palette, icon, index * 16);
  work->entries[index].kind = icon;
  MenuSprite_Initialize(entry->sprite, 0x80002000);
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
/// CommandRow_PutBg1InFront as the VCount handler at line 136, as the command
/// row does. Nothing calls it.
UNUSED void Graphics_InstallBg1VCountHandler(void) {
  Io_Write16(0, &REG_BG0VOFS);
  Irq_SetHandler(IRQ_VCOUNT, 136, CommandRow_PutBg1InFront);
}
