/// @file
/// Characters drawn into windows, and the per-frame sprite update.
// Part of window.c.

#include "window_internal.h"

/// Pixels added to the page arrow's and the sprite glyphs' screen y, which
/// differ by language.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PAGE_ARROW_Y_OFFSET 1
#define GLYPH_SPRITE_Y_OFFSET 0
#else
#define PAGE_ARROW_Y_OFFSET (-1)
#define GLYPH_SPRITE_Y_OFFSET (-2)
#endif

typedef s32 (*GlyphRoutine)(struct Window *, u32, u32, u32, const u8 *);

/// A glyph sprite's size in OAM attribute 1: 16x16, with the square shape.
#define GLYPH_SPRITE_SIZE (OBJ_SIZE_16X16 >> 16)

/// Draws a character of a window's text with its top-left corner at pixel (x,
/// y) of the window's interior and returns how far the text advances.
s32 Glyph_Draw(struct Window *window, s32 character, s32 x, s32 y, s32 mode) {
  struct WindowState *state = WINDOW_STATE;
  u32 spacingX = state->letterSpacing;
  s32 width;
  u32 spacingY = state->lineSpacing;

  // A text canvas window draws the glyph into its canvas.
  if (mode != GLYPH_DRAW_PAGE_ARROW &&
      (window->flags & WINDOW_FLAG_TEXT_CANVAS) != 0) {
    u8 *font;
    GlyphRoutine draw;
    s32 result;
#if LANGUAGE == LANGUAGE_JAPANESE
    s32 mark;

    font = NULL;
    if (gBattleMessageState->window == window) {
      font = Resource_GetTableEntry(RESOURCE_SMALL_FONT);
      width = 3;
      if (character == ' ')
        goto done;
    }
    mark = (s16)gFont_MarkGlyphCodes[(u32)character >> TEXT_CODE_MARK_SHIFT];
    character &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
    width = 4;
#else
    if (gBattleMessageState->window == window) {
      Resource_GetTableEntry(RESOURCE_SMALL_FONT);
      Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
    width = 4;
#endif
    if (character != ' ') {
      RamCode_Load(draw, Glyph_DrawArm, Glyph_DrawCodeSize);
      result = draw(window, character, x, y, font);
#if LANGUAGE == LANGUAGE_JAPANESE
      if ((u16)mark != 0)
        result += draw(window, (u16)mark, x + result, y, font);
#endif
      Heap_Release(draw);
      return result;
    }
  } else if (character == ' ') {
    width = WINDOW_SPRITE_SPACE_WIDTH;
  } else {
    // Other windows show each glyph as a 16x16 sprite, animated by the text
    // style (the motions of enum TextStyle) or, with GLYPH_DRAW_PAGE_ARROW, as
    // the page arrow.
    u32 tiles[32];
    struct WindowSprite *windowSprite;
    struct QueuedSprite *sprite;
    s32 index;

    windowSprite = WindowSprite_AcquireFree();
    if (windowSprite == NULL)
      return 0;
    // The sprite's own four of the glyph tiles.
    index = (windowSprite - state->sprites) * 4;
    windowSprite->motion = WINDOW_SPRITE_MOTION_STILL;
    windowSprite->kind = WINDOW_SPRITE_PLAIN;
    if (mode == GLYPH_DRAW_PAGE_ARROW) {
      width = 1;
      windowSprite->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    } else {
      switch (state->outline) {
      case TEXT_STYLE_SHAKE:
        windowSprite->motion = WINDOW_SPRITE_MOTION_SHAKE;
        break;
      case TEXT_STYLE_POP:
        windowSprite->motion = WINDOW_SPRITE_MOTION_POP;
        windowSprite->animationStep = 8;
        break;
      case TEXT_STYLE_SPIN:
        windowSprite->motion = WINDOW_SPRITE_MOTION_SPIN;
        windowSprite->animationStep = 0;
        break;
      case TEXT_STYLE_WOBBLE:
        windowSprite->motion = WINDOW_SPRITE_MOTION_WOBBLE;
        windowSprite->animationStep = 0;
        break;
      }
#if LANGUAGE == LANGUAGE_JAPANESE
      width = Text_DrawGlyphPair(character, tiles);
#else
      {
        s32 drawn = Text_DrawGlyphPair(character, tiles);

        if (drawn == 0)
          drawn = 1;
        width = drawn;
      }
#endif
    }

    if (windowSprite->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
      // The page arrow sits in the window's bottom-right corner.
      sprite = &windowSprite->sprite;
      if (state->glyphResource == NO_GLYPH_RESOURCE)
        state->glyphResource = VramCache_FindFreeEntry();
      sprite->oam.x = (window->x << 3) + ((window->width - 2) << 3) + 4;
      sprite->oam.y =
          (window->y << 3) + ((window->height - 2) << 3) + PAGE_ARROW_Y_OFFSET;
    } else {
      // Copy the glyph's tiles to its sprite tiles (sprite VRAM) and place it:
      // half the line spacing below y and half the letter spacing right of x,
      // in screen pixels.
      Dma_Set(
          tiles,
          (void *)(OBJ_VRAM0 + (state->glyphTiles + index) * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(32), REG_DMA3);
      ((u16 *)&windowSprite->sprite.oam)[0] =
          y + (spacingY >> 1) + window->y * 8 + GLYPH_SPRITE_Y_OFFSET;
      ((u16 *)&windowSprite->sprite.oam)[1] =
          (x + (spacingX >> 1) + window->x * 8 + 2) | GLYPH_SPRITE_SIZE;
      ((u16 *)&windowSprite->sprite.oam)[2] = state->glyphTiles + index;
      sprite = &windowSprite->sprite;
    }
    windowSprite->depth = OAM_DEPTH_FRONT - 1;
    windowSprite->x = sprite->oam.x;
    windowSprite->y = sprite->oam.y;
    windowSprite->index = index;
    windowSprite->next = NULL;
    WindowSprite_AppendToList(&window->sprites, windowSprite);
  }
done:
  return width;
}

#if LANGUAGE != LANGUAGE_JAPANESE

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  void *state = gWindowState[0];
  struct WindowState *base = state;
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
    s32 column = (u16)-2;
    /* FAKEMATCH(temporary): state, done with the window state, holds the
     * acquired sprite until out takes it. */
    state = WindowSprite_AcquireFree();
    if (state == NULL)
      return;
    out = state;
    idx = (out - base->sprites) * 4;
    out->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    sprite = &out->sprite;
    slot = &base->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = VramCache_FindFreeEntry();
    // 0xfffe and 254 are -2: the arrow sits on the interior's last tile, 4
    // pixels right and 1 up.
    sprite->oam.x = (win->x + (column + win->width)) * 8 + 4;
#ifdef UBFIX
    // Write row twice, in order.
    row = (u8)win->height + 254;
    row = (u8)win->y + row;
#else
    row = (u8)win->y + (row = (u8)win->height + 254);
#endif
    sprite->oam.y = row * 8 - 1;
    out->x = sprite->oam.x;
    out->y = sprite->oam.y;
    out->next = NULL;
    out->index = idx;
    if (out->motion == 0)
      out->motion = mode;
    WindowSprite_AppendToList(&win->sprites, out);
  } else if (tile <= TEXT_LAST_FONT_CODE) {
    // Past the border's top-left tile, and only inside the tilemap.
    x++;
    y++;
    pos = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
    if (pos < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
      ((struct WindowState *)state)->tilemap[pos] = WINDOW_TILE(tile);
  }
}

#else

/// The tiles a voiced mark takes in place of its own over the frame's top edge
/// and over a divider; a semi-voiced mark takes the next.
#define VOICED_MARK_OVER_BORDER_TILE 30
#define VOICED_MARK_OVER_DIVIDER_TILE 14

/// The Japanese Window_PutGlyph, which also places the kana marks.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  u8 *state = gWindowState[0];
  /* FAKEMATCH(dead): two XORs with tile that cancel. */
  u8 *base = (u8 *)((unsigned long)state ^ tile);
  struct WindowSprite *out;
  s32 idx;
  u16 *slot;
  struct QueuedSprite *sprite;
  u32 pos;
  u16 row;

  base = (u8 *)((unsigned long)base ^ tile);

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == GLYPH_DRAW_PAGE_ARROW) {
    s32 column = (u16)-2;
    /* FAKEMATCH(temporary): left, a copy of win->x for the one sum; as
     * win->x. */
    u16 left;
    out = WindowSprite_AcquireFree();
    if (out == NULL)
      return;
    idx = (out - ((struct WindowState *)base)->sprites) * 4;
    out->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    sprite = &out->sprite;
    slot = &((struct WindowState *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = VramCache_FindFreeEntry();
    left = win->x;
    sprite->oam.x = (left + (column + win->width)) * 8 + 4;
    // UB: row is written twice without a sequence point.
#ifdef UBFIX
    row = (u8)win->height + 254;
    row = (u8)win->y + row;
#else
    row = (u8)win->y + (row = (u8)win->height + 254);
#endif
    sprite->oam.y = row * 8 + 1;
    out->x = sprite->oam.x;
    out->y = sprite->oam.y;
    out->next = NULL;
    out->index = idx;
    if (out->motion == 0)
      out->motion = mode;
    WindowSprite_AppendToList(&win->sprites, out);
  } else if (tile <= TEXT_LAST_FONT_CODE) {
    if (tile - TEXT_MARK_VOICED <= 1) {
      // A kana mark goes in the tile above the character before it. Over the
      // frame's top edge or a divider it takes another tile.
      // XXX (no evidence): whether a voiced kana ever lands on a window's top
      // edge or a divider, where it takes VOICED_MARK_OVER_BORDER_TILE (parts
      // of the Japanese HP and PP labels) or VOICED_MARK_OVER_DIVIDER_TILE (the
      // dividers); no scene staged so far opens such a window.
      u32 line;

      line = (win->y + y) * BG_SCREEN_WIDTH;
      switch (
          *(((struct WindowState *)base)->tilemap + (line + (win->x + x)))) {
      case WINDOW_TILE_BORDER_TOP:
        tile -= TEXT_MARK_VOICED - VOICED_MARK_OVER_BORDER_TILE;
        break;
      case WINDOW_TILE_DIVIDER_HORIZONTAL:
        tile -= TEXT_MARK_VOICED - VOICED_MARK_OVER_DIVIDER_TILE;
        break;
      }
    } else {
      x++;
      y++;
    }
    pos = (win->y + y) * BG_SCREEN_WIDTH + (win->x + x);
    if (pos < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
      ((struct WindowState *)state)->tilemap[pos] = WINDOW_TILE(tile);
  }
}

#endif

/// The windows' tilemap entries in use: a screen block's rows down the
/// screen.
#define TILEMAP_ENTRIES (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)

/// Writes value into a window's tilemap at tile (x, y) of its interior, with
/// the palette mode names (TILEMAP_ENTRY_), or as it is for
/// TILEMAP_ENTRY_PLAIN; TILEMAP_ENTRY_NONE writes nothing. y -1 is the border's
/// top row, where the menus put their page numbers and arrows
/// (menu/menu_shared.c); a place beyond the bottom or right frame, or outside
/// the tilemap, is ignored.
void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode) {
  struct WindowState *state = WINDOW_STATE;
  s32 palette;
  s32 index;

  y += 1;
  x += 1;
  if (y > (u32)(window->height - 1))
    return;
  if (x > (u32)(window->width - 1))
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

  switch (mode) {
  case TILEMAP_ENTRY_NONE:
    return;
  case TILEMAP_ENTRY_HIGHLIGHT:
  case TILEMAP_ENTRY_WINDOW:
  case TILEMAP_ENTRY_ICON:
    index = (window->y + y) * BG_SCREEN_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    state->tilemap[index] = palette | value;
    return;
  case TILEMAP_ENTRY_PLAIN:
  default:
    index = (window->y + y) * BG_SCREEN_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    state->tilemap[index] = value;
    return;
  }
}

/// The steps of gWindowSprite_BounceScales: a swell to 1.5 times and back.
#define BOUNCE_SCALE_STEPS 32

/// Steps a scaling sprite's animation (WINDOW_SPRITE_MOTION_SWELL to
/// WINDOW_SPRITE_MOTION_SHRINK_HALF) and sets its affine scale for this frame.
void WindowSprite_UpdateScaleAnimation(struct WindowSprite *windowSprite) {
  // Scales are 8.8 fixed point: Q_8_8(1) is the sprite's own size.
  s32 scale = Q_8_8(1);
  struct QueuedSprite *sprite = &windowSprite->sprite;
  struct AffineTransform effect;

  switch (windowSprite->motion) {
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
    sprite->oam.affineMode = SPRITE_AFFINE_OFF;
    sprite->oam.x = windowSprite->x;
    sprite->oam.y = windowSprite->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->oam.matrix = OamMatrix_AllocFromTransform(&effect);
    // Larger than its size the sprite needs the double-size box; smaller, it
    // fits its own.
    if (scale > Q_8_8(1)) {
      sprite->oam.affineMode = SPRITE_AFFINE_DOUBLE_SIZE;
      sprite->oam.x = windowSprite->x - 8;
      sprite->oam.y = windowSprite->y - 8;
    } else {
      sprite->oam.affineMode = SPRITE_AFFINE_ON;
      sprite->oam.x = windowSprite->x;
      sprite->oam.y = windowSprite->y;
    }
  }
}

/// A frame of the next-page arrow: an 8 by 16 sprite of 256 colours.
#define PAGE_ARROW_FRAME_BYTES (2 * TILE_SIZE_8BPP)
/// The steps of gWindowSprite_WobbleOffsets and gWindowSprite_BounceOffsets.
#define WOBBLE_STEPS 20
#define HOP_STEPS 16

/// The per-frame sprite update: moves each sprite that an open window carries
/// by its animation mode (WINDOW_SPRITE_MOTION_) and queues it for drawing, at
/// its depth slot (WindowSprite.depth).
void WindowSprite_UpdateAll(void) {
  struct WindowState *state = WINDOW_STATE;
  struct Window *window = state->windows;
  s32 i;
  struct AffineTransform effect;

  for (i = 0; i != WINDOW_COUNT; window++, i++) {
    struct WindowSprite *windowSprite;
    u32 phase;

    if (!(window->flags & WINDOW_FLAG_OPEN))
      continue;
    windowSprite = window->sprites.head;
    // The page arrow's picture: one of 8, each shown for 4 VBlanks.
    phase = (gVBlankCount >> 2) & 7;
    while (windowSprite != NULL) {
      struct QueuedSprite *sprite = &windowSprite->sprite;

      if (window->animation == WINDOW_ANIMATION_SWELL) {
        windowSprite->animationStep = 2;
        windowSprite->motion = WINDOW_SPRITE_MOTION_PULSE;
      }
      switch (windowSprite->motion) {
      case WINDOW_SPRITE_MOTION_PAGE_ARROW:
        // VRAM_CACHE_ENTRY_COUNT means no entry was free for the arrow.
        if (state->glyphResource != VRAM_CACHE_ENTRY_COUNT) {
          const u8 *tiles =
              &gWindowSprite_PageArrowTiles[phase * PAGE_ARROW_FRAME_BYTES];

          sprite->oam.index = VramCache_Load(state->glyphResource,
                                             PAGE_ARROW_FRAME_BYTES, tiles);
          windowSprite->index = sprite->oam.index;
          sprite->oam.objectMode = OBJ_MODE_NORMAL;
          sprite->oam.mosaic = 0;
          sprite->oam.fullColor = 1;
          sprite->oam.size = 0;
          sprite->oam.shape = SPRITE_SHAPE_TALL;
          sprite->oam.y = windowSprite->y +
                          gWindowSprite_CursorBobOffsets[gVBlankCount % 80] + 2;
          sprite->oam.affineMode = SPRITE_AFFINE_OFF;
          sprite->oam.matrix = 0;
        }
        break;
      case WINDOW_SPRITE_MOTION_SHAKE:
        // Every other frame each axis moves -1 to 1 pixels from its place:
        // half the sum of two random 0 to 2, less 1.
        if (gVBlankCount & 1) {
          sprite->oam.x = windowSprite->x +
                          (s32)((Random16_Below(3) + Random16_Below(3)) >> 1) -
                          1;
          sprite->oam.y = windowSprite->y +
                          (s32)((Random16_Below(3) + Random16_Below(3)) >> 1) -
                          1;
        }
        break;
      case WINDOW_SPRITE_MOTION_POP:
        // Twice its size for animationStep frames, which Glyph_Draw starts
        // at 8.
        if (windowSprite->animationStep == 0) {
          sprite->oam.matrix = 0;
          sprite->oam.affineMode = SPRITE_AFFINE_OFF;
          sprite->oam.x = windowSprite->x;
          sprite->oam.y = windowSprite->y;
          break;
        }
        effect.scaleX = Q_8_8(2);
        effect.scaleY = Q_8_8(2);
        effect.angle = 0;
        sprite->oam.matrix = OamMatrix_AllocFromTransform(&effect);
        sprite->oam.affineMode = SPRITE_AFFINE_DOUBLE_SIZE;
        sprite->oam.x = windowSprite->x - 5;
        sprite->oam.y = windowSprite->y - 5;
        windowSprite->animationStep--;
        break;
      case WINDOW_SPRITE_MOTION_SPIN:
        // animationStep is the angle, three 256ths of a turn a frame, a turn
        // in about 85 frames, while the sprite circles its place four pixels
        // out (a sine shifted down 14 bits).
        effect.scaleX = Q_8_8(1);
        effect.scaleY = Q_8_8(1);
        windowSprite->animationStep += TRIG_TURN * 3 / 256;
        effect.angle = windowSprite->animationStep;
        sprite->oam.matrix = OamMatrix_AllocFromTransform(&effect);
        sprite->oam.affineMode = SPRITE_AFFINE_ON;
        sprite->oam.x = windowSprite->x -
                        (Trig_Sin(effect.angle + TRIG_TURN * 29 / 32) >> 14) -
                        2;
        sprite->oam.y = windowSprite->y -
                        (Trig_Cos(effect.angle + TRIG_TURN * 13 / 32) >> 14) -
                        2;
        break;
      case WINDOW_SPRITE_MOTION_WOBBLE:
        // One step of gWindowSprite_WobbleOffsets every other frame.
        if (gVBlankCount & 1)
          windowSprite->animationStep++;
        sprite->oam.x =
            windowSprite->x +
            gWindowSprite_WobbleOffsets[windowSprite->animationStep %
                                        WOBBLE_STEPS][0];
        sprite->oam.y =
            windowSprite->y +
            gWindowSprite_WobbleOffsets[windowSprite->animationStep %
                                        WOBBLE_STEPS][1] -
            2;
        break;
      case WINDOW_SPRITE_MOTION_HOP_UP:
        windowSprite->animationStep++;
        sprite->oam.y =
            windowSprite->y -
            gWindowSprite_BounceOffsets[windowSprite->animationStep &
                                        (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_HOP_DOWN:
      case WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE:
      case WINDOW_SPRITE_MOTION_HOP_DOWN_UNUSED:
        windowSprite->animationStep++;
        sprite->oam.y =
            windowSprite->y +
            gWindowSprite_BounceOffsets[windowSprite->animationStep &
                                        (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT:
        windowSprite->animationStep++;
        sprite->oam.x =
            windowSprite->x -
            gWindowSprite_BounceOffsets[windowSprite->animationStep &
                                        (HOP_STEPS - 1)];
        sprite->oam.y =
            windowSprite->y +
            gWindowSprite_BounceOffsets[windowSprite->animationStep &
                                        (HOP_STEPS - 1)];
        break;
      case WINDOW_SPRITE_MOTION_PULSE:
        // 1.25 times for animationStep frames, then its own size.
        if (windowSprite->animationStep != 0) {
          effect.scaleX = Q_8_8(1.25);
          effect.scaleY = Q_8_8(1.25);
          effect.angle = 0;
          sprite->oam.matrix = OamMatrix_AllocFromTransform(&effect);
          sprite->oam.affineMode = SPRITE_AFFINE_DOUBLE_SIZE;
          sprite->oam.x = windowSprite->x - 8;
          sprite->oam.y = windowSprite->y - 8;
          windowSprite->animationStep--;
          break;
        }
        sprite->oam.matrix = 0;
        sprite->oam.affineMode = SPRITE_AFFINE_OFF;
        sprite->oam.x = windowSprite->x;
        sprite->oam.y = windowSprite->y;
        break;
      case WINDOW_SPRITE_MOTION_SWELL:
      case WINDOW_SPRITE_MOTION_SWELL_HALF:
      case WINDOW_SPRITE_MOTION_SHRINK:
      case WINDOW_SPRITE_MOTION_SHRINK_HALF:
        WindowSprite_UpdateScaleAnimation((struct WindowSprite *)windowSprite);
        break;
      }
      // Queue it unless it is hidden, or an arrow with no picture.
      if (windowSprite->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
        if (state->glyphResource != VRAM_CACHE_ENTRY_COUNT)
          Oam_AddSpriteAtDepth(sprite, windowSprite->depth);
      } else if (windowSprite->motion != WINDOW_SPRITE_MOTION_HIDDEN) {
        Oam_AddSpriteAtDepth(sprite, windowSprite->depth);
      }
      windowSprite = windowSprite->next;
      phase = (gVBlankCount >> 2) & 7;
    }
  }
}

#undef PAGE_ARROW_Y_OFFSET
#undef GLYPH_SPRITE_Y_OFFSET
#undef GLYPH_SPRITE_SIZE
#undef TILEMAP_ENTRIES
#undef BOUNCE_SCALE_STEPS
#undef PAGE_ARROW_FRAME_BYTES
#undef WOBBLE_STEPS
#undef HOP_STEPS
#undef VOICED_MARK_OVER_BORDER_TILE
#undef VOICED_MARK_OVER_DIVIDER_TILE
