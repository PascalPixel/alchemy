/// @file
/// Characters drawn into windows, as glyph sprites, into a text canvas or as
/// tilemap tiles, and the per-frame update that moves and shows the sprites
/// windows carry.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"

#include "system/system.h"

typedef s32 (*GlyphRoutine)(struct Window *, u32, u32, u32, const u8 *);

/// A glyph sprite's size in its OAM attribute 1: 16 by 16, four tiles.
#define GLYPH_SPRITE_SIZE (OBJ_SIZE_16X16 >> 16)

/// The glyph routine RamCode_Load copies to RAM to run, and its size, as this
/// symbol's address.
extern const u8 Glyph_DrawCode[];
extern u8 Glyph_DrawCodeSize[];

s32 Text_DrawGlyphPair(s32 character, u32 *tiles);

/// The page arrow's offset from the window's last inner tile, and a sprite
/// glyph's from its text position, in pixels; the later editions moved both.
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
/// sheet (WindowState.glyphTiles, four for each sprite); with no sprite free
/// nothing is drawn and it returns 0. The sprite stays until its window is
/// cleared or closed.
s32 Glyph_Draw(struct Window *window, s32 character, s32 x, s32 y, s32 mode) {
  struct WindowState *state = WINDOW_STATE;
  s32 width;
  u32 spacingX = state->letterSpacing;
  u32 spacingY = state->lineSpacing;

  if (mode != GLYPH_DRAW_PAGE_ARROW &&
      (window->flags & WINDOW_FLAG_TEXT_CANVAS) != 0) {
#if LANGUAGE == LANGUAGE_JAPANESE
    u8 *font = NULL;
    GlyphRoutine draw;
    s32 result;
    s16 mark;

    // The small font, and the kana's voicing mark drawn as a second glyph
    // after it.
    if (Ram_HeapSlots->battleMessage->window == window) {
      font = Resource_GetTableEntry(RESOURCE_SMALL_FONT);
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

    if (Ram_HeapSlots->battleMessage->window == window) {
      Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
    width = 4;
    if (character != ' ') {
      RamCode_Load(draw, Glyph_DrawCode, Glyph_DrawCodeSize);
      result = draw(window, character, x, y, font);
      Heap_Release(draw);
      return result;
    }
#endif
  } else if (character == ' ') {
    width = WINDOW_SPRITE_SPACE_WIDTH;
  } else {
    u32 tiles[32];
    struct WindowSprite *output;
    struct QueuedSprite *sprite;
    s32 index;

    output = WindowSprite_AcquireFree();
    if (output == NULL)
      return 0;
    index = (output - state->sprites) * 4;
    output->motion = WINDOW_SPRITE_MOTION_STILL;
    output->kind = WINDOW_SPRITE_PLAIN;
    if (mode == GLYPH_DRAW_PAGE_ARROW) {
      width = 1;
      output->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    } else {
      switch (state->outline) {
      case TEXT_STYLE_SHAKE:
        output->motion = WINDOW_SPRITE_MOTION_SHAKE;
        break;
      case TEXT_STYLE_POP:
        output->motion = WINDOW_SPRITE_MOTION_POP;
        output->animationStep = 8;
        break;
      case TEXT_STYLE_SPIN:
        output->motion = WINDOW_SPRITE_MOTION_SPIN;
        output->animationStep = 0;
        break;
      case TEXT_STYLE_WOBBLE:
        output->motion = WINDOW_SPRITE_MOTION_WOBBLE;
        output->animationStep = 0;
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

    if ((u8)output->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
      sprite = WindowSprite_GetSprite(output);
      if (state->glyphResource == NO_GLYPH_RESOURCE)
        state->glyphResource = VramCache_FindFreeEntry();
      sprite->oam.x =
          ((u16)window->x << 3) + ((window->width - 2) << 3) + ARROW_X;
      sprite->oam.y =
          ((u16)window->y << 3) + ((window->height - 2) << 3) + ARROW_Y;
    } else {
      u16 *attr;

      Dma_Set(
          tiles,
          (void *)(OBJ_VRAM0 + (state->glyphTiles + index) * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(32), REG_DMA3);
      attr = (u16 *)&output->sprite.oam;
      if (state->battleMode & WINDOW_GLYPHS_BLENDED)
        *attr++ = (y + (spacingY >> 1) + (u16)window->y * 8 + GLYPH_Y) |
                  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      else
        *attr++ = y + (spacingY >> 1) + (u16)window->y * 8 + GLYPH_Y;
      *attr = (x + (spacingX >> 1) + (u16)window->x * 8 + GLYPH_X) |
              GLYPH_SPRITE_SIZE;
      attr[1] = state->glyphTiles + index;
      sprite = WindowSprite_GetSprite(output);
    }
    output->depth = OAM_DEPTH_FRONT - 2;
    output->x = sprite->oam.x;
    output->y = sprite->oam.y;
    output->index = index;
    output->next = NULL;
    WindowSprite_AppendToList(&window->sprites, output);
  }
done:
  return width;
}

/// Where the glyph's sprite sits against the text position, in pixels: the
/// Japanese font's glyphs are drawn into their tiles differently (see
/// Text_DrawGlyphPair).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_SPRITE_OFFSET_X 1
#define GLYPH_SPRITE_OFFSET_Y 0
#else
#define GLYPH_SPRITE_OFFSET_X 2
#define GLYPH_SPRITE_OFFSET_Y (-2)
#endif

s32 Text_DrawGlyphPair(s32 code, u32 *tiles);

/// Draws character as a glyph sprite on the window at x, y and returns its
/// width, or 0 when no window sprite is free.
s32 Glyph_DrawAsSprite(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowState *state = Ram_HeapSlots->windowState;
  u32 tiles[32];
  u32 spacingX = state->letterSpacing;
  u32 spacingY = state->lineSpacing;
  struct WindowSprite *output;
  s32 index;
  s32 width;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return 0;
  index = (output - state->sprites) * 4;
  output->motion = WINDOW_SPRITE_MOTION_STILL;
  output->kind = WINDOW_SPRITE_PLAIN;
  width = Text_DrawGlyphPair(character, tiles);
  Dma_Set(tiles,
          (void *)(OBJ_VRAM0 + (state->glyphTiles + index) * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(32), REG_DMA3);
  ((u16 *)&output->sprite.oam)[0] =
      y + (spacingY >> 1) + (u16)window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + (u16)window->x * 8 + GLYPH_SPRITE_OFFSET_X) |
      GLYPH_SPRITE_SIZE;
  ((u16 *)&output->sprite.oam)[2] = state->glyphTiles + index;
  output->depth = OAM_DEPTH_FRONT - 2;
  output->x = WindowSprite_GetSprite(output)->oam.x;
  output->y = WindowSprite_GetSprite(output)->oam.y;
  output->index = index;
  output->next = NULL;
  WindowSprite_AppendToList(&window->sprites, output);
  return width;
}

/// Draws the character source names into a 16x16 tile pair and copies it to
/// destination.
void Glyph_DrawToTiles(const void *source, void *destination) {
  u8 buffer[128];

  Text_DrawGlyphPair((s32)source, (u32 *)buffer);
  Dma_Set(buffer, destination, DMA_COPY_WORDS(32), REG_DMA3);
}

/// Places a glyph sprite on the window at x, y as Glyph_DrawAsSprite does, but
/// draws nothing, and returns the VRAM address of its tiles for the caller to
/// fill, or NULL when no window sprite is free.
void *Glyph_AddBlankSprite(struct Window *window, s32 character, s32 x, s32 y) {
  struct WindowState *state = Ram_HeapSlots->windowState;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  u32 tiles[32];
  u32 spacingX = state->letterSpacing;
  u32 spacingY = state->lineSpacing;
  struct WindowSprite *output;
  s32 index;
  void *vram;

  output = WindowSprite_AcquireFree();
  if (output == NULL)
    return NULL;
  index = (output - state->sprites) * 4;
  output->motion = WINDOW_SPRITE_MOTION_STILL;
  output->kind = WINDOW_SPRITE_PLAIN;
  vram = (void *)(OBJ_VRAM0 + (state->glyphTiles + index) * TILE_SIZE_4BPP);
  ((u16 *)&output->sprite.oam)[0] =
      y + (spacingY >> 1) + (u16)window->y * 8 + GLYPH_SPRITE_OFFSET_Y;
  ((u16 *)&output->sprite.oam)[1] =
      (x + (spacingX >> 1) + (u16)window->x * 8 + GLYPH_SPRITE_OFFSET_X) |
      GLYPH_SPRITE_SIZE;
  ((u16 *)&output->sprite.oam)[2] = state->glyphTiles + index;
  output->depth = OAM_DEPTH_FRONT - 2;
  output->x = WindowSprite_GetSprite(output)->oam.x;
  output->y = WindowSprite_GetSprite(output)->oam.y;
  output->index = index;
  output->next = NULL;
  /* FAKEMATCH(dead): motion stored a second time with the 1 it already holds.
   */
  output->motion = WINDOW_SPRITE_MOTION_STILL;
  WindowSprite_AppendToList(&window->sprites, output);
  return vram;
}

/// Where the next-page arrow sits on the interior's last tile, in pixels right
/// and down from its corner. It moves with the text: the Japanese glyph sprites
/// also sit 1 pixel further left and 2 lower (Glyph_DrawAsSprite).
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAGE_ARROW_OFFSET_X 3
#define PAGE_ARROW_OFFSET_Y 1
#else
#define PAGE_ARROW_OFFSET_X 4
#define PAGE_ARROW_OFFSET_Y (-1)
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The tiles a voiced mark takes in place of its own over the frame's top edge
/// and over a divider; a semi-voiced mark takes the next.
#define MARK_OVER_BORDER_TILE 30
#define MARK_OVER_DIVIDER_TILE 14
#endif

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior; GLYPH_DRAW_PAGE_ARROW instead shows the next-page arrow sprite on
/// the interior's last tile. In Japanese, a kana mark goes in the tile above
/// the character before it.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  struct WindowState *base = WINDOW_STATE;
  u16 *tilemap = base->tilemap[0];
  struct WindowSprite *out;
  s32 idx;
  struct QueuedSprite *sprite;
  u32 pos;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == GLYPH_DRAW_PAGE_ARROW) {
    out = WindowSprite_AcquireFree();
    if (out == NULL)
      return;
    idx = (out - base->sprites) * 4;
    out->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    sprite = WindowSprite_GetSprite(out);
    if (base->glyphResource == NO_GLYPH_RESOURCE)
      base->glyphResource = VramCache_FindFreeEntry();
    sprite->oam.x =
        (win->x << 3) + ((win->width - 2) << 3) + PAGE_ARROW_OFFSET_X;
    sprite->oam.y =
        (win->y << 3) + ((win->height - 2) << 3) + PAGE_ARROW_OFFSET_Y;
    out->x = sprite->oam.x;
    out->y = sprite->oam.y;
    out->index = idx;
    out->next = NULL;
    if ((u8)out->motion == 0)
      out->motion = mode;
    WindowSprite_AppendToList(&win->sprites, out);
  } else if (tile <= TEXT_LAST_FONT_CODE) {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (tile == TEXT_MARK_VOICED || tile == TEXT_MARK_SEMIVOICED) {
      // One up and one left of where a character at (x, y) goes. Over the
      // frame's top edge or a divider, the mark takes another tile.
      u32 line;

      line = (win->y + y) * BG_SCREEN_WIDTH;
      // XXX (no evidence): whether a mark ever lands on a top edge or a
      // divider, where this puts tiles 30 and 31 (in window_tiles_tla_ja.png
      // the HP label's second tile and the PP label) or 14 and 15 (the
      // dividers themselves); the render probe stages scenes only, and cannot
      // open a tla-ja window drawn by tiles with a voiced kana on its top line.
      switch (tilemap[line + (win->x + x)]) {
      case WINDOW_TILE_BORDER_TOP:
        tile -= TEXT_MARK_VOICED - MARK_OVER_BORDER_TILE;
        break;
      case WINDOW_TILE_DIVIDER_HORIZONTAL:
        tile -= TEXT_MARK_VOICED - MARK_OVER_DIVIDER_TILE;
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
    pos = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
    if (pos < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
      *(u16 *)((u32)tilemap + (pos << 1)) = WINDOW_TILE(tile);
  }
}

/// A window tilemap's width in tiles, and its entries over the screen's rows.
#define TILEMAP_WIDTH BG_SCREEN_WIDTH
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * DISPLAY_TILE_HEIGHT)

/// Writes value into a window's tilemap at tile (x, y) of its interior, with
/// the palette mode names (TILEMAP_ENTRY_), or as it is for
/// TILEMAP_ENTRY_PLAIN; TILEMAP_ENTRY_NONE writes nothing. y -1 is the border's
/// top row, where the menus put their page numbers and arrows
/// (menu/menu_shared.c); a place beyond the bottom or right frame, or outside
/// the tilemap, is ignored.
void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode) {
  u16 *map = WINDOW_STATE->tilemap[0];
  s32 palette;
  s32 index;

  y += 1;
  x += 1;
  if ((u32)y > (u32)(window->height - 1))
    return;
  if ((u32)x > (u32)(window->width - 1))
    return;

  switch (mode) {
  case TILEMAP_ENTRY_WINDOW:
    palette = BG_TILE_PALETTE(WINDOW_PALETTE);
    break;
  case TILEMAP_ENTRY_HIGHLIGHT:
    palette = BG_TILE_PALETTE(HIGHLIGHT_PALETTE);
    break;
  case TILEMAP_ENTRY_ICON:
    palette = BG_TILE_PALETTE(1);
    break;
  default:
    palette = 0;
    break;
  }

  // Both entries are stored at the entry's byte offset from map.
  switch (mode) {
  case TILEMAP_ENTRY_NONE:
    return;
  case TILEMAP_ENTRY_HIGHLIGHT:
  case TILEMAP_ENTRY_WINDOW:
  case TILEMAP_ENTRY_ICON:
    index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    *(u16 *)((u8 *)map + (index << 1)) = palette | value;
    return;
  case TILEMAP_ENTRY_PLAIN:
  default:
    index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    *(u16 *)((u8 *)map + (index << 1)) = value;
    return;
  }
}

extern const u16 gWindowSprite_BounceScales[];

/// The steps of gWindowSprite_BounceScales: a swell to 1.5 times and back.
#define BOUNCE_SCALE_STEPS 32

/// Steps a scaling sprite's animation (WINDOW_SPRITE_MOTION_SWELL to
/// WINDOW_SPRITE_MOTION_SHRINK_HALF) and sets its affine scale for this frame.
void WindowSprite_UpdateScaleAnimation(struct WindowSprite *windowSprite) {
  s32 scale = Q_8_8(1);
  struct QueuedSprite *sprite = WindowSprite_GetSprite(windowSprite);
  struct AffineTransform effect;

  switch ((u8)windowSprite->motion) {
  case WINDOW_SPRITE_MOTION_SWELL:
    scale = gWindowSprite_BounceScales[windowSprite->animationStep++ &
                                       (BOUNCE_SCALE_STEPS - 1)];
    break;
  case WINDOW_SPRITE_MOTION_SWELL_HALF:
    scale = gWindowSprite_BounceScales[windowSprite->animationStep++ &
                                       (BOUNCE_SCALE_STEPS - 1)] >>
            1;
    break;
  // The shrinks play the bounce's second half at twice its pace.
  case WINDOW_SPRITE_MOTION_SHRINK:
    if (windowSprite->animationStep < BOUNCE_SCALE_STEPS / 4)
      scale = gWindowSprite_BounceScales[windowSprite->animationStep++ * 2 +
                                         BOUNCE_SCALE_STEPS / 2];
    break;
  case WINDOW_SPRITE_MOTION_SHRINK_HALF:
    if (windowSprite->animationStep < BOUNCE_SCALE_STEPS / 4)
      scale = gWindowSprite_BounceScales[windowSprite->animationStep++ * 2 +
                                         BOUNCE_SCALE_STEPS / 2] >>
              1;
    break;
  }
  if (scale == Q_8_8(1)) {
    sprite->oam.matrix = 0;
    sprite->oam.affineMode = OAM_AFFINE_OFF;
    sprite->oam.x = windowSprite->x;
    sprite->oam.y = windowSprite->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->oam.matrix = OamMatrix_AllocFromTransform(&effect);
    if (scale > Q_8_8(1)) {
      sprite->oam.affineMode = OAM_AFFINE_DOUBLE_SIZE;
      // Drawn at double size, so set 8 pixels up and left.
      sprite->oam.x = windowSprite->x - 8;
      sprite->oam.y = windowSprite->y - 8;
    } else {
      sprite->oam.affineMode = OAM_AFFINE_ON;
      sprite->oam.x = windowSprite->x;
      sprite->oam.y = windowSprite->y;
    }
  }
}

/// A window sprite's queued OAM attributes, as WindowSprite_UpdateAll writes
/// them.
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

/// A window sprite as WindowSprite_UpdateAll reads it, its kind and motion
/// unsigned.
struct AnimOutput {
  struct AnimOutput *next;
  u8 kind;
  u8 motion;
  u16 x;
  u16 y;
  u16 unused;
  u16 animationStep;
  u8 index;
  u8 depth;
  struct AnimSprite attr;
};

/// A pop's affine transform as WindowSprite_UpdateAll packs it: x and y scale
/// and the angle.
struct PopTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 padding : 16;
};

/// The page arrow's pictures, a frame each.
extern const u8 gWindowSprite_PageArrowTiles[];

extern const u8 gWindowSprite_CursorBobOffsets[];
extern const s8 gWindowSprite_WobbleOffsets[][2];
extern const s8 gWindowSprite_BounceOffsets[];

/// A frame of the next-page arrow: an 8 by 16 sprite of 256 colours.
#define PAGE_ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/// The steps of gWindowSprite_WobbleOffsets and gWindowSprite_BounceOffsets.
#define WOBBLE_STEPS 20
#define HOP_STEPS 16

/// The per-frame sprite update: moves each sprite that an open window carries
/// by its animation mode (WINDOW_SPRITE_MOTION_) and queues it for drawing, at
/// its depth (WindowSprite.depth).
void WindowSprite_UpdateAll(void) {
  struct WindowState *state = WINDOW_STATE;
  struct Window *window = state->windows;
  s32 no;
  union {
    struct AffineTransform plain;
    struct PopTransform packed;
  } effect;

  for (no = 0; no != WINDOW_COUNT; window++, no++) {
    struct AnimOutput *output;
    u32 phase;

    if (!(window->flags & WINDOW_FLAG_OPEN))
      continue;
    output = (struct AnimOutput *)window->sprites.head;
    while (phase = (gVBlankCount >> 2) & 7, output != NULL) {
      struct AnimSprite *sprite = &output->attr;

      switch (output->motion) {
      case WINDOW_SPRITE_MOTION_PAGE_ARROW:
        if (state->glyphResource != VRAM_CACHE_ENTRY_COUNT) {
          const u8 *tiles =
              &gWindowSprite_PageArrowTiles[phase * PAGE_ARROW_FRAME_BYTES];

          sprite->tile = VramCache_Load(state->glyphResource,
                                        PAGE_ARROW_FRAME_BYTES, tiles);
          output->index = sprite->tile;
          sprite->mode = OBJ_MODE_NORMAL;
          sprite->mosaic = 0;
          sprite->colors = OBJ_COLORS_256;
          sprite->size = 0;
          sprite->shape = OBJ_SHAPE_VERTICAL;
          sprite->y =
              output->y + gWindowSprite_CursorBobOffsets[gVBlankCount % 80] + 2;
          sprite->affine = OAM_AFFINE_OFF;
          sprite->affineIndex = 0;
        }
        break;
      case WINDOW_SPRITE_MOTION_SHAKE:
        if (gVBlankCount & 1) {
          sprite->x = output->x +
                      (s32)(((Random_Below(3)) + (Random_Below(3))) >> 1) - 1;
          sprite->y = output->y +
                      (s32)(((Random_Below(3)) + (Random_Below(3))) >> 1) - 1;
        }
        break;
      case WINDOW_SPRITE_MOTION_POP:
        if (output->animationStep != 0) {
          effect.packed.scaleX = Q_8_8(2);
          effect.packed.scaleY = Q_8_8(2);
          effect.packed.angle = 0;
          sprite->affineIndex = OamMatrix_AllocFromTransform(&effect.plain);
          sprite->affine = OAM_AFFINE_DOUBLE_SIZE;
          sprite->x = output->x - 5;
          sprite->y = output->y - 5;
          output->animationStep--;
        } else {
          sprite->affineIndex = 0;
          sprite->affine = OAM_AFFINE_OFF;
          sprite->x = output->x;
          sprite->y = output->y;
        }
        break;
      case WINDOW_SPRITE_MOTION_SPIN:
        // animationStep is the angle, three 256ths of a turn a frame, a turn
        // in about 85 frames, while the sprite circles its place four pixels
        // out.
        effect.plain.scaleX = Q_8_8(1);
        effect.plain.scaleY = Q_8_8(1);
        output->animationStep += TRIG_TURN * 3 / 256;
        effect.plain.angle = output->animationStep;
        sprite->affineIndex = OamMatrix_AllocFromTransform(&effect.plain);
        sprite->affine = OAM_AFFINE_ON;
        sprite->x = output->x -
                    FIXEDtoINT_FOUR_TIMES(
                        Trig_Sin(effect.plain.angle + TRIG_TURN * 29 / 32)) -
                    2;
        sprite->y = output->y -
                    FIXEDtoINT_FOUR_TIMES(
                        Trig_Cos(effect.plain.angle + TRIG_TURN * 13 / 32)) -
                    2;
        break;
      case WINDOW_SPRITE_MOTION_WOBBLE:
        if (gVBlankCount & 1)
          output->animationStep++;
        sprite->x =
            output->x + gWindowSprite_WobbleOffsets[output->animationStep %
                                                    WOBBLE_STEPS][0];
        sprite->y = output->y +
                    gWindowSprite_WobbleOffsets[output->animationStep %
                                                WOBBLE_STEPS][1] -
                    2;
        break;
      case WINDOW_SPRITE_MOTION_HOP_UP:
        output->animationStep++;
        sprite->y =
            output->y - gWindowSprite_BounceOffsets[output->animationStep &
                                                    (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE:
        output->animationStep++;
        sprite->y =
            output->y + gWindowSprite_BounceOffsets[output->animationStep &
                                                    (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_HOP_LEFT:
        output->animationStep++;
        sprite->x =
            output->x - gWindowSprite_BounceOffsets[output->animationStep &
                                                    (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_HOP_RIGHT:
        output->animationStep++;
        sprite->x =
            output->x + gWindowSprite_BounceOffsets[output->animationStep &
                                                    (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT:
        output->animationStep++;
        sprite->x =
            output->x - gWindowSprite_BounceOffsets[output->animationStep &
                                                    (HOP_STEPS - 1)];
        sprite->y =
            output->y + gWindowSprite_BounceOffsets[output->animationStep &
                                                    (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_SWELL:
      case WINDOW_SPRITE_MOTION_SWELL_HALF:
      case WINDOW_SPRITE_MOTION_SHRINK:
      case WINDOW_SPRITE_MOTION_SHRINK_HALF:
        WindowSprite_UpdateScaleAnimation((struct WindowSprite *)output);
        break;
      }
      if (output->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
        if (state->glyphResource != VRAM_CACHE_ENTRY_COUNT)
          Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_FRONT);
      } else if (output->motion == WINDOW_SPRITE_MOTION_ON_SCREEN) {
        s32 x = (s16)output->x;
        s32 y = (s16)output->y;

        if ((u32)(x + 64) < DISPLAY_WIDTH + 64 &&
            (u32)(y + 64) < DISPLAY_HEIGHT + 64)
          Oam_AddSpriteAtDepth(sprite, output->depth);
      } else if (output->motion != WINDOW_SPRITE_MOTION_HIDDEN) {
        Oam_AddSpriteAtDepth(sprite, output->depth);
      }
      output = output->next;
    }
  }
}

#undef ARROW_X
#undef ARROW_Y
#undef GLYPH_X
#undef GLYPH_Y
#undef GLYPH_SPRITE_OFFSET_X
#undef GLYPH_SPRITE_OFFSET_Y
#undef PAGE_ARROW_OFFSET_X
#undef PAGE_ARROW_OFFSET_Y
#if (LANGUAGE == LANGUAGE_JAPANESE)
#undef MARK_OVER_BORDER_TILE
#undef MARK_OVER_DIVIDER_TILE
#endif
#undef TILEMAP_WIDTH
#undef TILEMAP_ENTRIES
