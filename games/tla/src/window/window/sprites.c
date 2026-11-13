/// @file
/// Characters drawn into windows, and the per-frame sprite update.
// Part of window.c.

#include "../window_internal.h"

typedef s32 (*GlyphRoutine)(struct Window *, u32, u32, u32, const u8 *);

extern const u8 Glyph_DrawCode[];
extern u8 Glyph_DrawCodeSize[];
extern char ResourceId_WindowTiles;
extern char ResourceId_SmallFont;

s32 Text_DrawGlyphPair(s32 character, u32 *tiles);

// A space's width in pixels, set by the font.
#if LANGUAGE == LANGUAGE_JAPANESE
#define SPACE_WIDTH 7
#else
#define SPACE_WIDTH 5
#endif
// The page arrow's offset from the window's last inner tile, and a sprite
// glyph's from its text position, in pixels; the later editions moved both.
#if TLA_PATCH_1
#define ARROW_X 4
#define ARROW_Y (-1)
#define GLYPH_X 2
#define GLYPH_Y (-2)
#else
#define ARROW_X 3
#define ARROW_Y 1
#define GLYPH_X 1
#define GLYPH_Y 0
#endif

/// Draws a character of a window's text with its top-left corner at pixel
/// (x, y) of the window's interior and returns how far the text advances. A
/// text canvas window draws it into the canvas; other windows show it as a
/// sprite carried by the window. GLYPH_DRAW_PAGE_ARROW shows the next-page
/// arrow instead.
/// A sprite glyph takes a window sprite and its four tiles from the glyph
/// sheet (WindowWork.glyphTiles, four for each sprite); with no sprite free
/// nothing is drawn and it returns 0. The sprite stays until its window is
/// cleared or closed.
s32 Glyph_Draw(struct Window *window, s32 character, s32 x, s32 y, s32 mode) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 width;
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;

  if (mode != 1 && (window->flags & 8) != 0) {
#if LANGUAGE == LANGUAGE_JAPANESE
    u8 *font = NULL;
    GlyphRoutine draw;
    s32 result;
    s16 mark;

    // The small font, and the kana's voicing mark drawn as a second glyph
    // after it.
    if (**(struct Window ***)&((void **)Ram_HeapSlots)[37] == window) {
      font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
      width = 3;
      if (character == ' ')
        goto done;
    }
    mark = gFont_MarkGlyphCodes[(u32)character >> TEXT_MARK_SHIFT];
    character &= (1 << TEXT_MARK_SHIFT) - 1;
    width = 4;
    if (character != ' ') {
      RamCode_Load(draw, Glyph_DrawCode, Glyph_DrawCodeSize);
      result = draw(window, character, x, y, font);
      {
        u16 markCode = mark;

        if (markCode != 0)
          result += draw(window, markCode, x + result, y, font);
      }
      Heap_Release(draw);
      return result;
    }
#else
    u8 *font;
    GlyphRoutine draw;
    s32 result;

    if (**(struct Window ***)&((void **)Ram_HeapSlots)[37] == window) {
      Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    width = 4;
    if (character != ' ') {
      RamCode_Load(draw, Glyph_DrawCode, Glyph_DrawCodeSize);
      result = draw(window, character, x, y, font);
      Heap_Release(draw);
      return result;
    }
#endif
  } else if (character == ' ') {
    width = SPACE_WIDTH;
  } else {
    u32 tiles[32];
    struct WindowSprite *output;
    struct QueuedSprite *sprite;
    s32 index;

    output = WindowSprite_AcquireFree();
    if (output == NULL)
      return 0;
    index = (output - work->outputs) * 4;
    output->active = 1;
    output->kind = 0;
    if (mode == 1) {
      width = 1;
      output->active = 2;
    } else {
      switch (work->outline) {
      case 3:
        output->active = 5;
        break;
      case 4:
        output->active = 6;
        output->animStep = 8;
        break;
      case 5:
        output->active = 7;
        output->animStep = 0;
        break;
      case 2:
        output->active = 4;
        output->animStep = 0;
        break;
      }
      {
        s32 drawn = Text_DrawGlyphPair(character, tiles);

        // The Japanese edition keeps a width of 0.
#if TLA_PATCH_1
        if (drawn == 0)
          drawn = 1;
#endif
        width = drawn;
      }
    }

    if ((u8)output->active == 2) {
      sprite = &output->sprite;
      if (work->glyphResource == 99)
        work->glyphResource = VramCache_FindFreeEntry();
      sprite->oam.x = (window->x << 3) + ((window->width - 2) << 3) + ARROW_X;
      sprite->oam.y = (window->y << 3) + ((window->height - 2) << 3) + ARROW_Y;
    } else {
      u16 *attr;

      Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
              0x84000020, (volatile u32 *)0x040000d4);
      attr = (u16 *)&output->sprite.oam;
      if (work->menuState & 4)
        *attr++ = (y + (spacingY >> 1) + window->y * 8 + GLYPH_Y) | 0x400;
      else
        *attr++ = y + (spacingY >> 1) + window->y * 8 + GLYPH_Y;
      *attr = (x + (spacingX >> 1) + window->x * 8 + GLYPH_X) | 0x4000;
      attr[1] = work->glyphTiles + index;
      sprite = &output->sprite;
    }
    output->sentinel = 253;
    output->x = sprite->oam.x;
    output->y = sprite->oam.y;
    output->index = index;
    output->next = NULL;
    WindowSprite_AppendToList(&window->output, output);
  }
done:
  return width;
}

// Where the glyph's sprite sits against the text position, in pixels: the
// Japanese font's glyphs are drawn into their tiles differently (see
// Text_DrawGlyphPair).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_SPRITE_OFFSET_X 1
#define GLYPH_SPRITE_OFFSET_Y 0
#else
#define GLYPH_SPRITE_OFFSET_X 2
#define GLYPH_SPRITE_OFFSET_Y (-2)
#endif

// The sprite cell at WindowSprite.sprite: a link, then the OAM words.
struct GlyphSprite {
  struct GlyphSprite *next;
  u16 y : 8;
  u16 attr0 : 8;
  u16 x : 9;
  u16 attr1 : 7;
  u16 tile;
};

s32 Text_DrawGlyphPair(s32 code, u32 *tiles);

s32 Func_0803c068(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  u32 tiles[32];
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;
  struct WindowSprite *output;
  struct GlyphSprite *sprite;
  s32 index;
  s32 width;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return 0;
  index = (output - work->outputs) * 4;
  output->active = 1;
  output->kind = 0;
  width = Text_DrawGlyphPair(character, tiles);
  Dma_Set(tiles, (void *)(0x06010000 + (work->glyphTiles + index) * 32),
          0x84000020, REG_DMA3);
  ((u16 *)&output->sprite.oam)[0] =
      y + (spacingY >> 1) + window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + GLYPH_SPRITE_OFFSET_X) | 0x4000;
  ((u16 *)&output->sprite.oam)[2] = work->glyphTiles + index;
  sprite = (struct GlyphSprite *)&output->sprite;
  output->sentinel = 253;
  output->x = sprite->x;
  output->y = sprite->y;
  output->index = index;
  output->next = NULL;
  WindowSprite_AppendToList(&window->output, output);
  return width;
}

void Func_0803c170(const void *source, void *destination) {
  u8 buffer[128];

  Text_DrawGlyphPair((s32)source, (u32 *)buffer);
  Dma_Set(buffer, destination, DMA_COPY_WORDS(32), REG_DMA3);
}

// Where the glyph's sprite sits against the text position, in pixels, as in
// Func_0803c068: the Japanese font's glyphs are drawn into their tiles
// differently (see Text_DrawGlyphPair).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_SPRITE_OFFSET_X 1
#define GLYPH_SPRITE_OFFSET_Y 0
#else
#define GLYPH_SPRITE_OFFSET_X 2
#define GLYPH_SPRITE_OFFSET_Y (-2)
#endif

void *Func_0803c198(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowWork *work = Ram_HeapSlots->windowTiles;
  // Unused: gives the stack frame its 128 bytes.
  u32 tiles[32];
  u32 spacingX = work->glyphSpacing;
  u32 spacingY = work->lineSpacing;
  struct WindowSprite *output;
  struct QueuedSprite *sprite;
  s32 index;
  void *vram;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return NULL;
  index = (output - work->outputs) * 4;
  output->active = 1;
  output->kind = 0;
  vram = (void *)(0x06010000 + (work->glyphTiles + index) * 32);
  ((u16 *)&output->sprite.oam)[0] =
      y + (spacingY >> 1) + window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + window->x * 8 + GLYPH_SPRITE_OFFSET_X) | 0x4000;
  ((u16 *)&output->sprite.oam)[2] = work->glyphTiles + index;
  /* FAKEMATCH(temporary): the cell read back through a pointer to output's
   * sprite; as output->sprite.oam.x and .y, 27 of 99 instructions differ in
   * tla-en. */
  sprite = &output->sprite;
  output->sentinel = 253;
  output->x = sprite->oam.x;
  output->y = sprite->oam.y;
  output->index = index;
  output->next = NULL;
  /* FAKEMATCH(dead): active stored a second time with the 1 it already holds;
   * as the first store alone, 46 of 99 instructions differ in tla-en. */
  output->active = 1;
  WindowSprite_AppendToList(&window->output, output);
  return vram;
}

// Where the next-page arrow sits on the interior's last tile, in pixels right
// and down from its corner. It moves with the text: the Japanese glyph sprites
// also sit 1 pixel further left and 2 lower (Func_0803c068).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAGE_ARROW_OFFSET_X 3
#define PAGE_ARROW_OFFSET_Y 1
#else
#define PAGE_ARROW_OFFSET_X 4
#define PAGE_ARROW_OFFSET_Y (-1)
#endif

/// The window tiles a kana mark is drawn over differently, as ☀️'s;
/// Window_DrawDividerLine here joins its lines to the same two.
enum {
  WINDOW_TILE_DIVIDER_HORIZONTAL = WINDOW_TILE(14), ///< a horizontal divider
  WINDOW_TILE_BORDER_TOP = WINDOW_TILE(17)          ///< the frame's top edge
};

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior, as ☀️'s Window_PutGlyph; GLYPH_DRAW_PAGE_ARROW instead shows the
/// next-page arrow sprite on the interior's last tile. In Japanese, a kana mark
/// goes in the tile above the character before it.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  struct WindowWork *base = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *tilemap = base->tilemap[0];
  struct WindowSprite *out;
  s32 idx;
  u16 *slot;
  struct QueuedSprite *sprite;
  u32 pos;
  u16 row;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == GLYPH_DRAW_PAGE_ARROW) {
    s32 column;
    u16 left;
    out = WindowSprite_AcquireFree();
    if (out == NULL)
      return;
    idx = (out - base->outputs) * 4;
    out->active = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    sprite = &out->sprite;
    slot = &base->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = VramCache_FindFreeEntry();
    // column holds its one constant, -2 as a halfword: added inline, it folds
    // into the width check above (52 of 116 instructions differ in tla-en).
    column = (u16)-2;
    left = win->x;
    sprite->oam.x = (left + (column + win->width)) * 8 + PAGE_ARROW_OFFSET_X;
#ifdef UBFIX
    row = (u8)win->height + (u8)-2;
    row = (u8)win->y + row;
#else
    // UB: row is written twice with no sequence point between; as the two
    // statements above, 6 of 116 instructions differ in tla-en.
    row = (u8)win->y + (row = (u8)win->height + (u8)-2);
#endif
    sprite->oam.y = row * 8 + PAGE_ARROW_OFFSET_Y;
    out->x = sprite->oam.x;
    out->y = sprite->oam.y;
    out->index = idx;
    out->next = NULL;
    // Read as a byte: active is never negative, and the code loads it
    // unsigned (read signed, 5 of 116 instructions differ in tla-en).
    if ((u8)out->active == 0)
      out->active = mode;
    WindowSprite_AppendToList(&win->output, out);
  } else if (tile <= TEXT_LAST_FONT_CODE) {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (tile == TEXT_MARK_VOICED || tile == TEXT_MARK_SEMIVOICED) {
      // One up and one left of where a character at (x, y) goes. Over the
      // frame's top edge or a divider, the mark takes another tile.
      u32 line;

      line = ((s16)win->y + y) * 32;
      // XXX: what tiles 14, 15, 30 and 31 show in the Japanese editions; a
      // tla-ja render of a kana mark over a window's top edge and over a
      // divider would settle it.
      switch (tilemap[line + ((s16)win->x + x)]) {
      case WINDOW_TILE_BORDER_TOP:
        tile -= 192;
        break;
      case WINDOW_TILE_DIVIDER_HORIZONTAL:
        tile -= 208;
        break;
      }
    } else {
      // Past the border's top-left tile, and only inside the tilemap.
      x++;
      y++;
    }
#else
    // Past the border's top-left tile, and only inside the tilemap.
    x++;
    y++;
#endif
    pos = ((s16)win->y + y) * 32 + ((s16)win->x + x);
    // The entry's address as an integer: tilemap[pos] takes its registers
    // the other way round (2 of 116 instructions differ in tla-en).
    if (pos < 32 * 20)
      *(u16 *)((u32)tilemap + (pos << 1)) = WINDOW_TILE(tile);
  }
}

enum {
  TILEMAP_ENTRY_PLAIN,
  TILEMAP_ENTRY_NONE,
  TILEMAP_ENTRY_PALETTE_14,
  TILEMAP_ENTRY_PALETTE_15,
  TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode) {
  u16 *map = ((struct WindowWork *)Ram_HeapSlots->windowTiles)->tilemap[0];
  s32 palette;
  s32 index;

  y += 1;
  x += 1;
  if ((u32)y > (u32)(window->height - 1))
    return;
  if ((u32)x > (u32)(window->width - 1))
    return;

  switch (mode) {
  case TILEMAP_ENTRY_PALETTE_15:
    palette = 0xf000;
    break;
  case TILEMAP_ENTRY_PALETTE_14:
    palette = 0xe000;
    break;
  case TILEMAP_ENTRY_PALETTE_1:
    palette = 0x1000;
    break;
  default:
    palette = 0;
    break;
  }

  // Both entries are stored at a byte offset from map, which keeps each store's
  // base and offset in the registers they take; as map[index], 4 of 73
  // instructions differ in tla-en.
  switch (mode) {
  case TILEMAP_ENTRY_NONE:
    return;
  case TILEMAP_ENTRY_PALETTE_14:
  case TILEMAP_ENTRY_PALETTE_15:
  case TILEMAP_ENTRY_PALETTE_1:
    index = ((s16)window->y + y) * TILEMAP_WIDTH + ((s16)window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    *(u16 *)((u8 *)map + (index << 1)) = palette | value;
    return;
  case TILEMAP_ENTRY_PLAIN:
  default:
    index = ((s16)window->y + y) * TILEMAP_WIDTH + ((s16)window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    *(u16 *)((u8 *)map + (index << 1)) = value;
    return;
  }
}

s32 OamMatrix_AllocFromTransform(struct AffineTransform *transform);

struct OutputSprite {
  u32 link;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 other : 4;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 otherX : 2;
  u16 tile;
};
extern u16 gWindowSprite_BounceScales[];

void WindowSprite_UpdateScaleAnimation(struct WindowSprite *windowSprite) {
  s32 scale = 256;
  struct OutputSprite *sprite = (struct OutputSprite *)&windowSprite->sprite;
  struct AffineTransform effect;

  switch ((u8)windowSprite->active) {
  case 9:
    scale = gWindowSprite_BounceScales[windowSprite->animStep++ & 31];
    break;
  case 10:
    scale = gWindowSprite_BounceScales[windowSprite->animStep++ & 31] >> 1;
    break;
  case 11:
    if (windowSprite->animStep <= 7)
      scale = gWindowSprite_BounceScales[windowSprite->animStep++ * 2 + 16];
    break;
  case 12:
    if (windowSprite->animStep <= 7)
      scale =
          gWindowSprite_BounceScales[windowSprite->animStep++ * 2 + 16] >> 1;
    break;
  }
  if (scale == 256) {
    sprite->affineIndex = 0;
    sprite->affine = 0;
    sprite->x = (u16)windowSprite->x;
    sprite->y = (u16)windowSprite->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->affineIndex = OamMatrix_AllocFromTransform(&effect);
    if (scale > 256) {
      sprite->affine = 3;
      // x read through a u16 pointer, which loads it after the -8 is built;
      // as (u16)windowSprite->x the two loads swap.
      sprite->x = *(u16 *)&windowSprite->x - 8;
      sprite->y = (u16)windowSprite->y - 8;
    } else {
      sprite->affine = 1;
      sprite->x = (u16)windowSprite->x;
      sprite->y = (u16)windowSprite->y;
    }
  }
}

struct AnimSprite {
  struct AnimSprite *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

struct AnimOutput {
  struct AnimOutput *next;
  u8 kind;
  u8 active;
  u16 x;
  u16 y;
  u16 unk0a;
  u16 animStep;
  u8 index;
  u8 sentinel;
  struct AnimSprite attr;
};

struct PopTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 unk06 : 16;
};

extern const u8 gWindowSprite_PageArrowTiles[];
extern const u8 gWindowSprite_CursorBobOffsets[];
extern const s8 gWindowSprite_WobbleOffsets[][2];
extern const s8 gUiSprite_BounceOffsets[];

void WindowSprite_UpdateAll(void) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct Window *window = work->windows;
  s32 no;
  union {
    struct AffineTransform plain;
    struct PopTransform packed;
  } effect;

  for (no = 0; no != UI_WINDOW_COUNT; window++, no++) {
    struct AnimOutput *output;
    u32 phase;

    if (!(window->flags & 1))
      continue;
    output = (struct AnimOutput *)window->output.head;
    while (phase = (gVBlankCount >> 2) & 7, output != NULL) {
      struct AnimSprite *sprite = &output->attr;

      switch (output->active) {
      case 2:
        if (work->glyphResource != 0x60) {
          const u8 *tiles = &gWindowSprite_PageArrowTiles[phase * 128];

          sprite->tile = VramCache_Load(work->glyphResource, 128, tiles);
          output->index = sprite->tile;
          sprite->mode = 0;
          sprite->mosaic = 0;
          sprite->colors = 1;
          sprite->size = 0;
          sprite->shape = 2;
          sprite->y =
              output->y + gWindowSprite_CursorBobOffsets[gVBlankCount % 80] + 2;
          sprite->affine = 0;
          sprite->affineIndex = 0;
        }
        break;
      case 5:
        if (gVBlankCount & 1) {
          sprite->x =
              output->x +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
          sprite->y =
              output->y +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
        }
        break;
      case 6:
        if (output->animStep != 0) {
          effect.packed.scaleX = 512;
          effect.packed.scaleY = 512;
          effect.packed.angle = 0;
          sprite->affineIndex = OamMatrix_AllocFromTransform(&effect.plain);
          sprite->affine = 3;
          sprite->x = output->x - 5;
          sprite->y = output->y - 5;
          output->animStep--;
        } else {
          sprite->affineIndex = 0;
          sprite->affine = 0;
          sprite->x = output->x;
          sprite->y = output->y;
        }
        break;
      case 7:
        effect.plain.scaleX = 256;
        effect.plain.scaleY = 256;
        output->animStep += 768;
        effect.plain.angle = output->animStep;
        sprite->affineIndex = OamMatrix_AllocFromTransform(&effect.plain);
        sprite->affine = 1;
        sprite->x =
            output->x - (Trig_Sin(effect.plain.angle + 0xe800) >> 14) - 2;
        sprite->y =
            output->y - (Trig_Cos(effect.plain.angle + 0x6800) >> 14) - 2;
        break;
      case 4:
        if (gVBlankCount & 1)
          output->animStep++;
        sprite->x =
            output->x + gWindowSprite_WobbleOffsets[output->animStep % 20][0];
        sprite->y = output->y +
                    gWindowSprite_WobbleOffsets[output->animStep % 20][1] - 2;
        break;
      case 17:
        output->animStep++;
        sprite->y = output->y - gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 15:
        output->animStep++;
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 16:
        output->animStep++;
        sprite->x = output->x - gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 14:
        output->animStep++;
        sprite->x = output->x + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 18:
        output->animStep++;
        sprite->x = output->x - gUiSprite_BounceOffsets[output->animStep & 15];
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case 9:
      case 10:
      case 11:
      case 12:
        WindowSprite_UpdateScaleAnimation((struct WindowSprite *)output);
        break;
      }
      if (output->active == 2) {
        if (work->glyphResource != 0x60)
          Oam_AddSpriteAtDepth(sprite, 255);
      } else if (output->active == 19) {
        s32 x = (s16)output->x;
        s32 y = (s16)output->y;

        if ((u32)(x + 64) < 304 && (u32)(y + 64) < 224)
          Oam_AddSpriteAtDepth(sprite, output->sentinel);
      } else if (output->active != 13) {
        Oam_AddSpriteAtDepth(sprite, output->sentinel);
      }
      output = output->next;
    }
  }
}

#undef SPACE_WIDTH
#undef ARROW_X
#undef ARROW_Y
#undef GLYPH_X
#undef GLYPH_Y
#undef GLYPH_SPRITE_OFFSET_X
#undef GLYPH_SPRITE_OFFSET_Y
#undef PAGE_ARROW_OFFSET_X
#undef PAGE_ARROW_OFFSET_Y
#undef TILEMAP_WIDTH
#undef TILEMAP_ENTRIES
