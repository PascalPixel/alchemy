/// @file
/// Pictures and command icons loaded for the menus, and BG1's priority.
/// Part of battle_command.c, the menu object, compiled only through it.

#include "../battle_command_internal.h"

extern const u8 *gRenderResource_PairSourceTable[];

/// Decompresses picture `picture` of gRenderResource_PairSourceTable (two
/// 32x32 sprites side by side, 512 bytes) into VRAM slot `slot`. The heap
/// block is not freed when `slot` is not a valid VRAM slot.
void RenderResource_LoadPair(s32 picture, s32 slot) {
  void *stagingBuffer = Runtime_AllocateBlock(14, 0x400);
  const u8 *resource = gRenderResource_PairSourceTable[picture];

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Resource_DecodeByteLz(resource, stagingBuffer);
    VramCache_Load(slot, 0x200, stagingBuffer);
    Runtime_ReleaseHeapBlock(14);
  }
}

/// Shows picture `picture` of gRenderResource_PairSourceTable as two 32x32
/// sprites side by side at (x, y) in `window`, on draw layer 253, and
/// returns the left one (the right one follows it in the window's list).
/// Returns NULL when no VRAM slot is free.
struct RenderOutput *
RenderResource_CreatePair(s32 picture, struct UiWindow *window, s32 x, s32 y) {
  s32 slot;
  struct RenderOutput *first;
  struct RenderOutput *second;

  slot = Resource_FindFreeEntry();
  if (slot >= VRAM_CACHE_ENTRY_COUNT)
    return NULL;

  // 0x4000 is the wide shape bit of attribute 0, with size 2: 64x32.
  RenderResource_LoadPair(picture, slot);
  first = RenderOutput_Create(slot, 0x80004000, window, x, y);
  first->sentinel = 0xFD;
  second = RenderOutput_Create(slot, 0x80004000, window, x + 32, y);
  second->sentinel = 0xFD;
  // The right half's tiles.
  second->oam.index += 8;
  return first;
}

/// Bobs the two sprites of a pair (see RenderResource_CreatePair) up and down
/// by gUi_PairBobOffsets, one step every four frames.
void RenderOutput_BobPair(struct RenderOutput *obj) {
  s32 value;

  value = (u8)obj->y + gUi_PairBobOffsets[(gFrameTick >> 2) & 7];
  obj->oam.y = value;
  obj = obj->next;
  /* FAKEMATCH(volatile): gFrameTick read again for the second sprite, as the
   * bytes do; read plainly, CSE reuses the first index and 20 of 22
   * instructions differ in tbs-en. */
  value =
      (u8)obj->y + gUi_PairBobOffsets[(*(volatile u32 *)&gFrameTick >> 2) & 7];
  obj->oam.y = value;
}

/// Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB)
/// into VRAM slot `slot`, remapping its first 768 bytes' colours by table
/// when `remap` is nonzero.
void RenderResource_LoadFrame(s32 icon, s32 slot, s32 remap) {
  s32 size = 1024;
  void *buffer = Runtime_AllocateBlock(14, size);
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    Resource_DecodeByteLz((void *)((u32)offsetTable + offsetTable[icon]),
                          buffer);
    if (remap != 0)
      Runtime_RemapBytesByTableFar(buffer, 768);
    VramCache_Load(slot, size, buffer);
    Runtime_ReleaseHeapBlock(14);
  }
}

/// Shows command icon `icon` (see RenderResource_LoadFrame) as a 32x32
/// 256-colour sprite at (x, y) in `window`, on draw layer 251. Returns NULL
/// when no VRAM slot is free.
struct RenderOutput *RenderResource_CreateFrame(s32 icon, s32 remap,
                                                struct UiWindow *window, s32 x,
                                                s32 y) {
  s32 slot;
  struct RenderOutput *output;

  slot = Resource_FindFreeEntry();
  output = NULL;
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    RenderResource_LoadFrame(icon, slot, remap);
    output = RenderOutput_Create(slot, OUTPUT_SIZE_32X32, window, x, y);
    output->oam.palette256 = 1;
    output->sentinel = 0xfb;
  }
  return output;
}

/// Makes the sprite `obj` pulse: scales it about its centre by
/// gUi_ObjectPulseScales (in 1/65536, used in 1/256), one step every two
/// frames, and puts it on draw layer 252. Does nothing when `obj` is NULL.
void Ui_ApplyTableScaleToObject(struct RenderOutput *obj) {
  s32 scale = gUi_ObjectPulseScales[(gFrameTick >> 1) & 7];
  struct AffineTransform effect;

  if (scale < 0)
    scale += 255;
  scale >>= 8;

  if (obj != 0) {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    obj->oam.matrix = AffineMatrix_BuildForEffect(&effect);
    obj->oam.affineMode = 3;
    obj->oam.x = obj->x + 0xfff0;
    obj->oam.y = (u8)obj->y + 0xf0;
    obj->sentinel = 0xfc;
  }
}

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

#define RANGE_PATTERN_SET_COUNT 2
#define RANGE_PATTERN_COUNT 6
#define RANGE_PATTERN_PAIR_WORDS 16

// Builds two sets of six paired bar-pattern tiles. Each pair starts in
// color 4, then XORs row masks from gMenu_BarPatternRows.
// The matching return type keeps pop {r1}; bx r1; void uses r0 instead.
#ifdef UBFIX
void Menu_BuildLocalizedPatternTiles(void) {
#else
s32 Menu_BuildLocalizedPatternTiles(void) {
#endif
  u32 *vram = (u32 *)0x06006280;
  s32 patternSet;
  s32 patternIndex;

  for (patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
    for (patternIndex = 0; patternIndex < RANGE_PATTERN_COUNT; patternIndex++) {
      u32 *tile =
          vram + patternSet * (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_WORDS) +
          patternIndex * RANGE_PATTERN_PAIR_WORDS;
      s32 row;

      Iwram_FillWords(tile, 64, 0x44444444);
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
            XorWord(tile[row], gMenu_BarPatternRows[sourcePattern].word0);
        tile[row + 8] =
            XorWord(tile[row + 8], gMenu_BarPatternRows[sourcePattern].word1);
      }
    }
  }
}

// Draws a range indicator across three adjacent tiles at (x, y), using the
// selected pattern and a mirrored left end.
// Void changes pop {r1}; bx r1 to r0 and reorders a preview's argument setup.
#ifdef UBFIX
void UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 x, s32 y,
                                  s32 pattern, s32 unused) {
#else
int UiWindow_DrawThreeTileColumn(struct UiWindow *window, s32 x, s32 y,
                                 s32 pattern, s32 unused) {
#endif
  s32 patternOffset = pattern * 2;
  s32 rightTile = patternOffset + 0xF315;

  UiWindow_SetTilemapEntry(window, 0x400 | rightTile, x, y, 0);
  UiWindow_SetTilemapEntry(window, patternOffset + 0xF314, x + 1, y, 0);
  UiWindow_SetTilemapEntry(window, rightTile, x + 2, y, 0);
}

/// Merge two 8x8 4bpp tiles row by row. Shift the second tile horizontally by
/// shift pixels, using the first tile wherever the shifted pixel is
/// transparent.
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

/// Animate the two link indicator tiles with opposing two/three-pixel shifts.
void Link_DrawShiftedTilePair(s32 offset) {
  s32 phase = (gFrameCount >> 2) & 3;

  if (phase > 2) {
    phase = 2;
  }
  if (phase <= 0) {
    phase = 1;
  }
  phase++;
  Graphics_MergeShiftedTileRows((u32 *)0x06000220, (u32 *)gRomShiftedTilePair,
                                (u32 *)offset, -phase);
  Graphics_MergeShiftedTileRows((u32 *)0x06000240,
                                (u32 *)(gRomShiftedTilePair + 32),
                                (u32 *)(offset + 32), phase);
}

/// Expand two palette variants of the ten source tile rows at 0x06000600,
/// mapping each packed nibble through gGraphics_ExpandNibbleTable.
void Graphics_ExpandVramTilesByColorTable(u16 *dst) {
  s32 bank = 0;
  s32 paletteOffset = 0;
  s32 destinationBank = 0;

  do {
    s32 row = 0;
    s32 destinationOffset = (destinationBank + bank) << 6;

    do {
      u16 *output = (u16 *)((u8 *)dst + destinationOffset);
      u16 *source = (u16 *)((u8 *)0x06000600 + (row << 5));
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

/// Compose a table-entry glyph in temporary transfer work, reload its cache
/// slot, then release the temporary work and return the tile index.
s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  Ui_PrepareTransferFromTableEntry(resource);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

/// Compose the item icon with style 0x1a in temporary transfer work and reload
/// the requested cache slot. Return its tile index.
s32 ItemIcon_LoadIntoSlot(s32 resource, s32 index) {
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  ItemIcon_Compose(resource, 0x1a);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

/// Build the ability glyph in temporary transfer work, reload the requested
/// cache slot and return its tile index.
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index) {
  s32 tile;
  s32 slot;
  s32 result;
  GlyphTransfer *work;

  work = Runtime_AllocateBlock(17, sizeof(*work));
  slot = index;
  Ability_LoadGlyph(resource, 0, &slot, &tile, 1);
  result = VramCache_Reload(index, (s32)work->tiles);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

/// Load a party portrait into the supplied cache slot and return its tile.
/// In the prologue, Isaac and Garet use their childhood portrait entries.
s32 UiGlyph_LoadPortrait(u32 member, s32 cacheSlot) {
  s32 tile;
  s32 slot;
  u32 portrait;

  portrait = member;
  slot = cacheSlot;
  if (portrait > 7U) {
    portrait = 0;
  }
  if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0) {
    switch (portrait) {
    case 0:
      portrait = 0x38;
      break;
    case 1:
      portrait = 0x39;
      break;
    }
  }
  UiGlyph_LoadEntryWithPalette(portrait, 0, &slot, &tile, 0xE, 1);
  return tile;
}

/// Return gUi_FixedTileBlocks[0]; treat any nonzero no as 0 as well.
s32 Ui_GetTableWordZero(s32 no) {
  if (no != 0)
    no = 0;
  return gUi_FixedTileBlocks[no];
}

/// Copies the 4bpp decoder into RAM, calls it with the transfer's encoded
/// source and byte buffer, then releases the copied code. Its ARM entry reads
/// the stream through r0 and writes bytes through r1, as UiIcon_Decode does.
/// Nothing calls it.
UNUSED void UiIcon_Decompress(GlyphTransfer *work) {
  RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
  ((void (*)(const void *, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
      work->encoded, work->input);
  Runtime_ReleaseHeapBlock(49);
}

// Open a 6x4 window at the upper left, write its heading and return it.
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
#define LINK_COUNTDOWN_DRAW_HEADING UiText_DrawStringAtOffset
#else
#define LINK_COUNTDOWN_DRAW_HEADING UiText_DrawStringInWindow
#endif

/// Create the link timer window and draw its fixed time heading.
struct UiWindow *Link_CreateCountdownLabelWindow(void) {
  struct UiWindow *win = UiWindow_Create(0, 0, 6, 4, 6);

  LINK_COUNTDOWN_DRAW_HEADING(gLink_TimeLabelString, win, 0, 0);
  return win;
}

/// Decode a command-icon table entry in temporary transfer work and reload
/// the requested cache slot. Return its tile index.
s32 Resource_LoadIndexedIntoBuffer(s32 slotIndex, s32 icon) {
  GlyphTransfer *buffer =
      Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
  u16 *offsetTable = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
  u8 **encoded = &buffer->encoded;
  void *target = (void *)((u32)offsetTable + offsetTable[icon]);
  s32 result;

  *encoded = target;
  Resource_DecodeByteLz(target, buffer);
  result = VramCache_Reload(slotIndex, (s32)buffer);
  Runtime_ReleaseHeapBlock(0x11);
  return result;
}

/// Load a command icon into 8bpp OBJ VRAM, sharing a compact palette across
/// the selection's icons. Copy each newly encountered color from OBJ to BG
/// palette storage before uploading the remapped pixels.
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph,
                              s32 tile) {
  GlyphTransfer *decoded;
  u8 *font;
  u8 *remapped;
  u8 *source;
  u8 *destination;
  s32 i;
  u32 index;

  decoded = Runtime_AllocateHeapBlock(17, sizeof(*decoded));
  font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
  decoded->encoded = font + ((u16 *)font)[glyph];
  Resource_DecodeByteLz(decoded->encoded, decoded);
  remapped = MemAlloc(0x400);
  source = decoded->input;
  destination = remapped;
  for (i = 0; i < 0x400; i++) {
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
  MemFree(remapped);
  Runtime_ReleaseHeapBlock(17);
}

/// Load a selection icon, initialize its sprite and bind the decoded tiles.
/// The icon's kind also chooses its command-name label.
void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 icon) {
  struct SelectionEntry *entry = &work->entries[index];

  UiText_LoadRemappedGlyph(&work->palette, icon, index * 16);
  work->entries[index].kind = icon;
  MenuSprite_Init(entry->sprite, 0x80002000);
  entry->sprite.oam.f.tile =
      Resource_LoadIndexedIntoBuffer(work->entries[index].resource, icon);
}

/// Put BG1 behind the other priority levels while the command strip is active.
void Graphics_SetBg1Priority3(void) {
  u32 value = REG_BG1CNT;

  value &= ~BGCNT_PRIORITY(3);
  value |= BGCNT_PRIORITY(3);
  REG_BG1CNT = value;
}

/// Restore BG1 priority 0; the VCount handler applies this below the strip.
void Graphics_SetBg1Priority0(void) {
  u32 value = REG_BG1CNT;

  value &= ~BGCNT_PRIORITY(3);
  REG_BG1CNT = value;
}

/// Clear BG0 vertical scroll and install the BG1 priority change at line 136.
/// Nothing calls it.
UNUSED void Graphics_InstallBg1VCountHandler(void) {
  u32 address = 0x04000012;
  s32 zero = 0;

  *(volatile u16 *)address = zero;
  /* FAKEMATCH(one-pass): the handler's address set in a block of its own keeps
   * its load ahead of the handler index; as a plain assignment, 2 of 10
   * instructions differ in tbs-en. */
  do {
    address = (u32)Graphics_SetBg1Priority0;
  } while (0);
  Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}
