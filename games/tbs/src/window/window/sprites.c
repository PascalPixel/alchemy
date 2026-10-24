/// @file
/// Characters drawn into windows, and the per-frame sprite update.
// Part of window.c.

#include "../window_internal.h"

typedef s32 (*GlyphRoutine)(struct Window *, u32, u32, u32, const u8 *);

/// A window sprite's sprite cell: the link and OAM attributes in its 12 bytes
/// from offset 0x10.
#define SPRITE_CELL(output) ((struct CursorSprite *)(output)->link)

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
  struct WindowWork *work = WINDOW_WORK;
  u32 spacingX = work->letterSpacing;
  s32 width;
  u32 spacingY = work->lineSpacing;

  // A text canvas window draws the glyph into its canvas. In the battle
  // message window a space only advances 3 pixels; Japanese draws there with
  // the small font, and every canvas outside Japanese with the window tiles'
  // 8x8 font; the two lookups there whose results go unused change nothing.
  if (mode != 1 && (window->flags & WINDOW_FLAG_TEXT_CANVAS) != 0) {
    u8 *font;
    GlyphRoutine draw;
    s32 result;
#if (LANGUAGE == LANGUAGE_JAPANESE)
    s32 mark;

    font = NULL;
    if (((struct BattleMessageWork *)
             gWindowWork[HEAP_SLOT_BATTLE_MESSAGE - HEAP_SLOT_WINDOW])
            ->window == window) {
      font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
      width = 3;
      if (character == ' ')
        goto done;
    }
    mark = (s16)gFont_MarkGlyphCodes[(u32)character >> TEXT_CODE_MARK_SHIFT];
    character &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
    width = 4;
#else
    if (((struct BattleMessageWork *)
             gWindowWork[HEAP_SLOT_BATTLE_MESSAGE - HEAP_SLOT_WINDOW])
            ->window == window) {
      Resource_GetTableEntry((s32)&ResourceId_SmallFont);
      Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
      width = 3;
      if (character == ' ')
        goto done;
    }
    font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    width = 4;
#endif
    if (character != ' ') {
      RamCode_Load(draw, Glyph_DrawArm, Glyph_DrawCodeSize);
      result = draw(window, character, x, y, font);
#if (LANGUAGE == LANGUAGE_JAPANESE)
      if ((u16)mark != 0)
        result += draw(window, (u16)mark, x + result, y, font);
#endif
      Free(draw);
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
    struct CursorSprite *sprite;
    s32 index;

    windowSprite = WindowSprite_AcquireFree();
    if (windowSprite == NULL)
      return 0;
    // The sprite's own four tiles of the glyph sheet.
    index = (windowSprite - work->sprites) * 4;
    windowSprite->motion = WINDOW_SPRITE_MOTION_STILL;
    windowSprite->kind = WINDOW_SPRITE_GLYPH;
    if (mode == GLYPH_DRAW_PAGE_ARROW) {
      width = 1;
      windowSprite->motion = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    } else {
      switch (work->outline) {
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
#if (LANGUAGE == LANGUAGE_JAPANESE)
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

    if ((u8)windowSprite->motion == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
      // The page arrow sits in the window's bottom-right corner. Its picture
      // is loaded each frame (WindowSprite_UpdateAll) into a VRAM cache
      // entry of its own, taken here when none is held.
      sprite = SPRITE_CELL(windowSprite);
      if (work->glyphResource == NO_GLYPH_RESOURCE)
        work->glyphResource = VramCache_FindFreeEntry();
      sprite->x = (window->x << 3) + ((window->width - 2) << 3) + 4;
#if (LANGUAGE == LANGUAGE_JAPANESE)
      sprite->y = (window->y << 3) + ((window->height - 2) << 3) + 1;
#else
      sprite->y = (window->y << 3) + ((window->height - 2) << 3) - 1;
#endif
    } else {
      // Copy the glyph's tiles to its sprite tiles (OBJ VRAM) and place it:
      // half the line spacing below y and half the letter spacing right of x,
      // in screen pixels. 0x4000 in attribute 1 is size 1, with the square
      // shape a 16x16 sprite.
      Dma_Set(tiles, (void *)(OBJ_VRAM0 + (work->glyphTiles + index) * 32),
              DMA_COPY32(32), REG_DMA3);
#if (LANGUAGE == LANGUAGE_JAPANESE)
      ((u16 *)&windowSprite->oam)[0] = y + (spacingY >> 1) + window->y * 8;
#else
      ((u16 *)&windowSprite->oam)[0] = y + (spacingY >> 1) + window->y * 8 - 2;
#endif
      ((u16 *)&windowSprite->oam)[1] =
          (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
      ((u16 *)&windowSprite->oam)[2] = work->glyphTiles + index;
      sprite = SPRITE_CELL(windowSprite);
    }
    // Glyphs are drawn at depth slot 254.
    windowSprite->sentinel = 254;
    windowSprite->x = sprite->x;
    windowSprite->y = sprite->y;
    windowSprite->index = index;
    windowSprite->next = NULL;
    WindowSprite_AppendToList(&window->sprites, windowSprite);
  }
done:
  return width;
}

// The view is a struct of its own for matching: read through a cast of
// WindowSprite.link instead, WindowSprite_UpdateAll does not match (578
// instructions become 605), and through WindowSprite itself Window_PutGlyph's
// position stores move.

/// A view of struct WindowSprite with its sprite cell (from offset 0x10) as the
/// member attr, for the code that moves and queues the sprite. Its fields are
/// WindowSprite's.
struct WindowSpriteCellView {
  struct WindowSpriteCellView *next;
  u8 kind;
  u8 active;         ///< its WINDOW_SPRITE_MOTION_ motion
  u16 x;             ///< screen pixels, where the motion is centred
  u16 y;             ///< screen pixels, where the motion is centred
  u16 unused;        ///< WindowSprite's; no code names it
  u16 animationStep; ///< step of its motion's animation
  u8 index;
  u8 sentinel;              ///< depth slot it is drawn at
  struct CursorSprite attr; ///< the sprite cell queued each frame
};

STATIC_ASSERT(offsetof(struct WindowSpriteCellView, x) ==
              offsetof(struct WindowSprite, x));
STATIC_ASSERT(offsetof(struct WindowSpriteCellView, y) ==
              offsetof(struct WindowSprite, y));
STATIC_ASSERT(offsetof(struct WindowSpriteCellView, active) ==
              offsetof(struct WindowSprite, motion));
STATIC_ASSERT(offsetof(struct WindowSpriteCellView, animationStep) ==
              offsetof(struct WindowSprite, animationStep));
STATIC_ASSERT(offsetof(struct WindowSpriteCellView, index) ==
              offsetof(struct WindowSprite, index));
STATIC_ASSERT(offsetof(struct WindowSpriteCellView, attr) ==
              offsetof(struct WindowSprite, link));

#if (LANGUAGE != LANGUAGE_JAPANESE)

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior. GLYPH_DRAW_PAGE_ARROW shows the next-page arrow sprite in the
/// window's corner instead. The code is the tile's number in WINDOW_PALETTE, so
/// only codes up to 0xff are drawn; a place beyond the bottom or right frame is
/// ignored.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  void *work = gWindowWork[0];
  u8 *base = work;
  struct WindowSpriteCellView *out;
  s32 idx;
  u16 *slot;
  struct CursorSprite *attr;
  u32 pos;
  u16 row;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == GLYPH_DRAW_PAGE_ARROW) {
    s32 column;
    work = WindowSprite_AcquireFree();
    if (work == NULL)
      return;
    out = work;
    idx =
        ((struct WindowSprite *)out - ((struct WindowWork *)base)->sprites) * 4;
    out->active = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    attr = &out->attr;
    slot = &((struct WindowWork *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = VramCache_FindFreeEntry();
    // 0xfffe and 254 are -2: the arrow sits on the interior's last tile, 4
    // pixels right and 1 up.
    column = 0xfffe;
    attr->x = (win->x + (column + *(u16 *)&win->width)) * 8 + 4;
#ifdef UBFIX
    // Sequence the two writes to row. Combining them in one expression has
    // no intervening sequence point; plain stores reorder native EN code.
    row = (u8)win->height + 254;
    row = (u8)win->y + row;
#else
    row = (u8)win->y + (row = (u8)win->height + 254);
#endif
    attr->y = row * 8 - 1;
    out->x = attr->x;
    out->y = attr->y;
    out->next = NULL;
    out->index = idx;
    if (out->active == 0)
      out->active = mode;
    WindowSprite_AppendToList(&win->sprites, (struct WindowSprite *)out);
  } else if (tile <= 0xff) {
    // Past the frame's top-left tile, and only inside the tilemap.
    x++;
    y++;
    pos = (win->y + y) * 32 + (win->x + x);
    if (pos < 32 * 20)
      ((struct WindowWork *)work)->tilemap[pos] = WINDOW_TILE(tile);
  }
}

#else

/// The Japanese Window_PutGlyph, which also places the kana marks.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode) {
  u8 *work = gWindowWork[0];
  /* FAKEMATCH(dead): two XORs with tile that cancel keep work and base apart,
   * with the native ip to r8 copy; as base = work, 2 of 141 instructions
   * differ in tbs-ja. */
  u8 *base = (u8 *)((unsigned long)work ^ tile);
  struct WindowSpriteCellView *out;
  s32 idx;
  u16 *slot;
  struct CursorSprite *attr;
  u32 pos;
  u16 row;

  base = (u8 *)((unsigned long)base ^ tile);

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == GLYPH_DRAW_PAGE_ARROW) {
    s32 column;
    u16 left;
    out = (struct WindowSpriteCellView *)WindowSprite_AcquireFree();
    if (out == NULL)
      return;
    idx =
        ((struct WindowSprite *)out - ((struct WindowWork *)base)->sprites) * 4;
    out->active = WINDOW_SPRITE_MOTION_PAGE_ARROW;
    attr = &out->attr;
    slot = &((struct WindowWork *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = VramCache_FindFreeEntry();
    column = 0xfffe;
    left = win->x;
    attr->x = (left + (column + *(u16 *)&win->width)) * 8 + 4;
#ifdef UBFIX
    // As above, the original writes row twice without a sequence point.
    row = (u8)win->height + 254;
    row = (u8)win->y + row;
#else
    row = (u8)win->y + (row = (u8)win->height + 254);
#endif
    attr->y = row * 8 + 1;
    out->x = attr->x;
    out->y = attr->y;
    out->next = NULL;
    out->index = idx;
    if (out->active == 0)
      out->active = mode;
    WindowSprite_AppendToList(&win->sprites, (struct WindowSprite *)out);
  } else if (tile <= 0xff) {
    if (tile - TEXT_VOICED_MARK <= 1) {
      // A kana mark goes in the tile above the character before it. Over the
      // frame's top edge or a divider line, it takes the mark tile drawn over
      // that background.
      u32 line;

      line = (win->y + y) * 32;
      switch (*(((struct WindowWork *)base)->tilemap + (line + (win->x + x)))) {
      case WINDOW_TILE_FRAME_TOP:
        tile -= 0xc0;
        break;
      case WINDOW_TILE_DIVIDER_HORIZONTAL:
        tile -= 0xd0;
        break;
      }
    } else {
      x++;
      y++;
    }
    pos = (win->y + y) * 32 + (win->x + x);
    if (pos < 32 * 20)
      ((struct WindowWork *)work)->tilemap[pos] = WINDOW_TILE(tile);
  }
}

#endif

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

/// Writes value into a window's tilemap at tile (x, y) of its interior, with
/// the palette mode names (TILEMAP_ENTRY_), or as it is for
/// TILEMAP_ENTRY_PLAIN; TILEMAP_ENTRY_NONE writes nothing. y -1 is the frame's
/// top row, where the menus put their page numbers and arrows
/// (menu/menu/common.c); a place beyond the bottom or right frame, or outside
/// the tilemap, is ignored. The entry is not uploaded until the tilemap is
/// next marked dirty.
void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode) {
  struct WindowWork *work = WINDOW_WORK;
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
    palette = WINDOW_PALETTE << 12;
    break;
  case TILEMAP_ENTRY_HIGHLIGHT:
    palette = HIGHLIGHT_PALETTE << 12;
    break;
  case TILEMAP_ENTRY_ICON:
    palette = 0x1000;
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
    index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    work->tilemap[index] = palette | value;
    return;
  case TILEMAP_ENTRY_PLAIN:
  default:
    index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    work->tilemap[index] = value;
    return;
  }
}

/// Steps a scaling sprite's animation (WINDOW_SPRITE_MOTION_SWELL to
/// WINDOW_SPRITE_MOTION_SHRINK_HALF) and sets its affine scale for this frame.
/// The swells loop over gWindowSprite_BounceScales' 32 steps; the shrinks take
/// its second half, every other entry, over 8 frames and then stay at full
/// size.
void WindowSprite_UpdateScaleAnimation(struct WindowSprite *windowSprite) {
  // Scales are 8.8 fixed point: 256 is the sprite's own size.
  s32 scale = 256;
  struct CursorSprite *sprite = SPRITE_CELL(windowSprite);
  struct AffineTransform effect;

  switch ((u8)windowSprite->motion) {
  case WINDOW_SPRITE_MOTION_SWELL:
    scale = gWindowSprite_BounceScales[windowSprite->animationStep++ & 31];
    break;
  case WINDOW_SPRITE_MOTION_SWELL_HALF:
    scale = gWindowSprite_BounceScales[windowSprite->animationStep++ & 31] >> 1;
    break;
  case WINDOW_SPRITE_MOTION_SHRINK:
    if (windowSprite->animationStep <= 7)
      scale =
          gWindowSprite_BounceScales[windowSprite->animationStep++ * 2 + 16];
    break;
  case WINDOW_SPRITE_MOTION_SHRINK_HALF:
    if (windowSprite->animationStep <= 7)
      scale =
          gWindowSprite_BounceScales[windowSprite->animationStep++ * 2 + 16] >>
          1;
    break;
  }
  if (scale == 256) {
    sprite->affineIndex = 0;
    sprite->affine = 0;
    sprite->x = windowSprite->x;
    sprite->y = windowSprite->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->affineIndex = OamMatrix_AllocFromTransform(&effect);
    // Larger than its size the sprite needs the double-size affine mode (3),
    // whose 32x32 area starts 8 pixels up and left; smaller, plain affine (1).
    if (scale > 256) {
      sprite->affine = 3;
      // x read through a u16 pointer, which loads the -8 first; as sprite->x
      // the two loads swap.
      sprite->x = *(u16 *)&windowSprite->x - 8;
      sprite->y = (u16)windowSprite->y - 8;
    } else {
      sprite->affine = 1;
      sprite->x = windowSprite->x;
      sprite->y = windowSprite->y;
    }
  }
}

/// The per-frame sprite update: moves each sprite that an open window carries
/// by its animation mode (WINDOW_SPRITE_MOTION_) and queues it for drawing, at
/// its depth slot (WindowSprite.sentinel). The last step of
/// WindowSystem_Update. While a window swells (WINDOW_EFFECT_SWELL) all its
/// sprites pulse.
void WindowSprite_UpdateAll(void) {
  struct WindowWork *work = WINDOW_WORK;
  struct Window *window = work->windows;
  s32 no;
  struct AffineTransform effect;

  for (no = 0; no != WINDOW_COUNT; window++, no++) {
    struct WindowSpriteCellView *windowSprite;
    u32 phase;

    if (!(window->flags & WINDOW_FLAG_OPEN))
      continue;
    windowSprite = (struct WindowSpriteCellView *)window->sprites.head;
    // The page arrow's frame: one of 8, each shown for 4 frames.
    phase = (gVBlankCount >> 2) & 7;
    while (windowSprite != NULL) {
      struct CursorSprite *sprite = &windowSprite->attr;

      if (window->effect == WINDOW_EFFECT_SWELL) {
        windowSprite->animationStep = 2;
        windowSprite->active = WINDOW_SPRITE_MOTION_PULSE;
      }
      switch (windowSprite->active) {
      case WINDOW_SPRITE_MOTION_PAGE_ARROW:
        // 0x60 is VRAM_CACHE_ENTRY_COUNT: no entry was free for the arrow.
        // Its 128-byte frame is an 8x16 sprite of 256 colours (colors 1,
        // shape 2, size 0), bobbing over 80 frames.
        if (work->glyphResource != 0x60) {
          const u8 *tiles = &gWindowSprite_CursorFrameTiles[phase * 128];

          sprite->tile = VramCache_Load(work->glyphResource, 128, tiles);
          windowSprite->index = sprite->tile;
          sprite->mode = 0;
          sprite->mosaic = 0;
          sprite->colors = 1;
          sprite->size = 0;
          sprite->shape = 2;
          sprite->y = windowSprite->y +
                      gWindowSprite_CursorBobOffsets[gVBlankCount % 80] + 2;
          sprite->affine = 0;
          sprite->affineIndex = 0;
        }
        break;
      case WINDOW_SPRITE_MOTION_SHAKE:
        // Every other frame each axis moves -1 to 1 pixels from its place:
        // half the sum of two random 0 to 2, less 1.
        if (gVBlankCount & 1) {
          sprite->x =
              windowSprite->x +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
          sprite->y =
              windowSprite->y +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
        }
        break;
      case WINDOW_SPRITE_MOTION_POP:
        // Double size (512 in 8.8) for animationStep frames, which
        // Glyph_Draw starts at 8.
        if (windowSprite->animationStep == 0) {
          sprite->affineIndex = 0;
          sprite->affine = 0;
          sprite->x = windowSprite->x;
          sprite->y = windowSprite->y;
          break;
        }
        effect.scaleX = 512;
        effect.scaleY = 512;
        effect.angle = 0;
        sprite->affineIndex = OamMatrix_AllocFromTransform(&effect);
        sprite->affine = 3;
        sprite->x = windowSprite->x - 5;
        sprite->y = windowSprite->y - 5;
        windowSprite->animationStep--;
        break;
      case WINDOW_SPRITE_MOTION_SPIN:
        // animationStep is the angle, 65536 a turn: 768 a frame, a turn in
        // about 85 frames, while the sprite circles its place.
        effect.scaleX = 256;
        effect.scaleY = 256;
        windowSprite->animationStep += 768;
        effect.angle = windowSprite->animationStep;
        sprite->affineIndex = OamMatrix_AllocFromTransform(&effect);
        sprite->affine = 1;
        sprite->x =
            windowSprite->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
        sprite->y =
            windowSprite->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
        break;
      case WINDOW_SPRITE_MOTION_WOBBLE:
        // One step of the 20 of gWindowSprite_WobbleOffsets every other frame.
        if (gVBlankCount & 1)
          windowSprite->animationStep++;
        sprite->x =
            windowSprite->x +
            gWindowSprite_WobbleOffsets[windowSprite->animationStep % 20][0];
        sprite->y =
            windowSprite->y +
            gWindowSprite_WobbleOffsets[windowSprite->animationStep % 20][1] -
            2;
        break;
      case WINDOW_SPRITE_MOTION_HOP_UP:
        windowSprite->animationStep++;
        sprite->y =
            windowSprite->y -
            gWindowSprite_BounceOffsets[windowSprite->animationStep & 15];
        break;
      case WINDOW_SPRITE_MOTION_HOP_DOWN:
      case WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE:
      case WINDOW_SPRITE_MOTION_HOP_DOWN_3:
        windowSprite->animationStep++;
        sprite->y =
            windowSprite->y +
            gWindowSprite_BounceOffsets[windowSprite->animationStep & 15];
        break;
      case WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT:
        windowSprite->animationStep++;
        sprite->x =
            windowSprite->x -
            gWindowSprite_BounceOffsets[windowSprite->animationStep & 15];
        sprite->y =
            windowSprite->y +
            gWindowSprite_BounceOffsets[windowSprite->animationStep & 15];
        break;
      case WINDOW_SPRITE_MOTION_PULSE:
        // 1.25 times (320 in 8.8) for animationStep frames, then its own size.
        if (windowSprite->animationStep != 0) {
          effect.scaleX = 320;
          effect.scaleY = 320;
          effect.angle = 0;
          sprite->affineIndex = OamMatrix_AllocFromTransform(&effect);
          sprite->affine = 3;
          sprite->x = windowSprite->x - 8;
          sprite->y = windowSprite->y - 8;
          windowSprite->animationStep--;
          break;
        }
        sprite->affineIndex = 0;
        sprite->affine = 0;
        sprite->x = windowSprite->x;
        sprite->y = windowSprite->y;
        break;
      case WINDOW_SPRITE_MOTION_SWELL:
      case WINDOW_SPRITE_MOTION_SWELL_HALF:
      case WINDOW_SPRITE_MOTION_SHRINK:
      case WINDOW_SPRITE_MOTION_SHRINK_HALF:
        WindowSprite_UpdateScaleAnimation((struct WindowSprite *)windowSprite);
        break;
      }
      // Queue it unless it is hidden, or an arrow with no picture.
      if (windowSprite->active == WINDOW_SPRITE_MOTION_PAGE_ARROW) {
        if (work->glyphResource != 0x60)
          Oam_AddSpriteAtDepth(sprite, windowSprite->sentinel);
      } else if (windowSprite->active != WINDOW_SPRITE_MOTION_HIDDEN) {
        Oam_AddSpriteAtDepth(sprite, windowSprite->sentinel);
      }
      windowSprite = windowSprite->next;
      phase = (gVBlankCount >> 2) & 7;
    }
  }
}
