/// @file
/// Part of window.c, the window object, compiled only through it.
/// Characters drawn into windows, and the per-frame sprite update.

#include "../window_internal.h"

typedef s32 (*GlyphRoutine)(struct UiWindow *, u32, u32, u32, const u8 *);

/// A render output's sprite cell: the link and OAM attributes in its 12 bytes
/// from offset 0x10.
#define SPRITE_CELL(output) ((struct CursorSprite *)(output)->unk10)

/// Draws a character of a window's text with its top-left corner at pixel
/// (x, y) of the window's interior and returns how far the text advances. A
/// text canvas window draws it into the canvas; other windows show it as a
/// sprite carried by the window. mode 1 shows the next-page arrow instead.
s32 UiText_DrawGlyph(struct UiWindow *window, s32 character, s32 x, s32 y,
                     s32 mode) {
  struct UiRenderWork *work = UI_WORK;
  u32 spacingX = work->letterSpacing;
  s32 width;
  u32 spacingY = work->lineSpacing;

  // A text canvas window draws the glyph into its canvas.
  if (mode != 1 && (window->flags & UI_WINDOW_TEXT_CANVAS) != 0) {
    u8 *font;
    GlyphRoutine draw;
    s32 result;
#if (LANGUAGE == LANGUAGE_JAPANESE)
    s32 mark;

    font = NULL;
    if (((struct BattleDisplayWork *)
             gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW])
            ->window == window) {
      font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
      width = 3;
      if (character == ' ')
        goto done;
    }
    mark = (s16)gUiText_MarkGlyphCodes[(u32)character >> TEXT_CODE_MARK_SHIFT];
    character &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
    width = 4;
#else
    if (((struct BattleDisplayWork *)
             gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW])
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
      RamCode_Load(draw, UiText_DrawGlyphArm, UiText_DrawGlyphCodeSize);
      result = draw(window, character, x, y, font);
#if (LANGUAGE == LANGUAGE_JAPANESE)
      if ((u16)mark != 0)
        result += draw(window, (u16)mark, x + result, y, font);
#endif
      MemFree(draw);
      return result;
    }
  } else if (character == ' ') {
    width = UI_SPRITE_SPACE_WIDTH;
  } else {
    // Other windows show each glyph as a 16x16 sprite, animated by the text
    // style (UiRenderWork.outline 2 to 5) or, in mode 1, as the page arrow.
    u32 tiles[32];
    struct RenderOutput *output;
    struct CursorSprite *sprite;
    s32 index;

    output = RenderOutput_AcquireFree();
    if (output == NULL)
      return 0;
    index = (output - work->outputs) * 4;
    output->active = UI_SPRITE_STILL;
    output->kind = 0;
    if (mode == 1) {
      width = 1;
      output->active = UI_SPRITE_PAGE_ARROW;
    } else {
      switch (work->outline) {
      case 3:
        output->active = UI_SPRITE_SHAKE;
        break;
      case 4:
        output->active = UI_SPRITE_POP;
        output->animStep = 8;
        break;
      case 5:
        output->active = UI_SPRITE_SPIN;
        output->animStep = 0;
        break;
      case 2:
        output->active = UI_SPRITE_WOBBLE;
        output->animStep = 0;
        break;
      }
#if (LANGUAGE == LANGUAGE_JAPANESE)
      width = UiText_RenderGlyphPair(character, tiles);
#else
      {
        s32 drawn = UiText_RenderGlyphPair(character, tiles);

        if (drawn == 0)
          drawn = 1;
        width = drawn;
      }
#endif
    }

    if ((u8)output->active == UI_SPRITE_PAGE_ARROW) {
      // The page arrow sits in the window's bottom-right corner.
      sprite = SPRITE_CELL(output);
      if (work->glyphResource == NO_GLYPH_RESOURCE)
        work->glyphResource = Resource_FindFreeEntry();
      sprite->x = (window->x << 3) + ((window->width - 2) << 3) + 4;
#if (LANGUAGE == LANGUAGE_JAPANESE)
      sprite->y = (window->y << 3) + ((window->height - 2) << 3) + 1;
#else
      sprite->y = (window->y << 3) + ((window->height - 2) << 3) - 1;
#endif
    } else {
      // Copy the glyph's tiles to its sprite tiles (OBJ VRAM) and place it.
      Dma_Set(tiles, (void *)(OBJ_VRAM0 + (work->glyphTiles + index) * 32),
              DMA_COPY32(32), REG_DMA3);
#if (LANGUAGE == LANGUAGE_JAPANESE)
      ((u16 *)&output->oam)[0] = y + (spacingY >> 1) + window->y * 8;
#else
      ((u16 *)&output->oam)[0] = y + (spacingY >> 1) + window->y * 8 - 2;
#endif
      ((u16 *)&output->oam)[1] =
          (x + (spacingX >> 1) + window->x * 8 + 2) | 0x4000;
      ((u16 *)&output->oam)[2] = work->glyphTiles + index;
      sprite = SPRITE_CELL(output);
    }
    output->sentinel = 254;
    output->x = sprite->x;
    output->y = sprite->y;
    output->index = index;
    output->next = NULL;
    RenderOutput_AppendToList(&window->output, output);
  }
done:
  return width;
}

/// A view of struct RenderOutput with its sprite cell (from offset 0x10) as the
/// member attr. Read through a cast of RenderOutput.unk10 instead,
/// UiWork_AnimateSpriteSlots does not match (578 instructions become 605).
struct SpriteOutput {
  struct SpriteOutput *next;
  u8 kind;
  u8 active;
  u16 x;
  u16 y;
  u16 unk0a;
  u16 animStep;
  u8 index;
  u8 sentinel;
  struct CursorSprite attr;
};

STATIC_ASSERT(offsetof(struct SpriteOutput, x) ==
              offsetof(struct RenderOutput, x));
STATIC_ASSERT(offsetof(struct SpriteOutput, y) ==
              offsetof(struct RenderOutput, y));
STATIC_ASSERT(offsetof(struct SpriteOutput, active) ==
              offsetof(struct RenderOutput, active));
STATIC_ASSERT(offsetof(struct SpriteOutput, animStep) ==
              offsetof(struct RenderOutput, animStep));
STATIC_ASSERT(offsetof(struct SpriteOutput, index) ==
              offsetof(struct RenderOutput, index));
STATIC_ASSERT(offsetof(struct SpriteOutput, attr) ==
              offsetof(struct RenderOutput, unk10));

#if (LANGUAGE != LANGUAGE_JAPANESE)

/// Puts a character into a window's tilemap as a tile at tile (x, y) of its
/// interior. mode 1 shows the next-page arrow sprite in the window's corner
/// instead.
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode) {
  void *work = gWindowWork[0];
  u8 *base = work;
  struct SpriteOutput *out;
  s32 idx;
  u16 *slot;
  struct CursorSprite *attr;
  u32 pos;
  u16 row;

  if (y > (win->height - 2))
    return;
  if (x > (win->width - 2))
    return;
  if (mode == 1) {
    s32 column;
    work = RenderOutput_AcquireFree();
    if (work == NULL)
      return;
    out = work;
    idx =
        ((struct RenderOutput *)out - ((struct UiRenderWork *)base)->outputs) *
        4;
    out->active = UI_SPRITE_PAGE_ARROW;
    attr = &out->attr;
    slot = &((struct UiRenderWork *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = Resource_FindFreeEntry();
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
    /* FAKEMATCH: needed to match: the embedded OAM view preserves
     * packed/logical position store order changed by byte-span and scalar
     * views. */
    out->x = attr->x;
    out->y = attr->y;
    out->next = NULL;
    out->index = idx;
    if (out->active == 0)
      out->active = mode;
    RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
  } else if (tile <= 0xff) {
    x++;
    y++;
    pos = (win->y + y) * 32 + (win->x + x);
    if (pos < 32 * 20)
      ((struct UiRenderWork *)work)->tilemap[pos] = UI_TILE(tile);
  }
}

#else

/// The Japanese UiWindow_PutGlyph, which also places the kana marks.
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode) {
  u8 *work = gWindowWork[0];
  /* FAKEMATCH: needed to match: this XOR pair changes no pointer bits and folds
   * without instructions, preserving work and base as independent lifetimes. A
   * direct copy merges both into r8, omits the native r8/r12 copy, and differs
   * 17 aligned instructions (0x144 versus 0x148 bytes). */
  u8 *base = (u8 *)((unsigned long)work ^ tile);
  struct SpriteOutput *out;
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
  if (mode == 1) {
    s32 column;
    u16 left;
    out = (struct SpriteOutput *)RenderOutput_AcquireFree();
    if (out == NULL)
      return;
    idx =
        ((struct RenderOutput *)out - ((struct UiRenderWork *)base)->outputs) *
        4;
    out->active = UI_SPRITE_PAGE_ARROW;
    attr = &out->attr;
    slot = &((struct UiRenderWork *)base)->glyphResource;
    if (*slot == NO_GLYPH_RESOURCE)
      *slot = Resource_FindFreeEntry();
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
    /* FAKEMATCH: needed to match: the embedded OAM view preserves
     * packed/logical position store order changed by byte-span and scalar
     * views. */
    out->x = attr->x;
    out->y = attr->y;
    out->next = NULL;
    out->index = idx;
    if (out->active == 0)
      out->active = mode;
    RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
  } else if (tile <= 0xff) {
    if (tile - TEXT_VOICED_MARK <= 1) {
      // A kana mark goes in the tile above the character before it. Over the
      // frame's top edge or a divider line, it takes the mark tile drawn over
      // that background.
      u32 line;

      line = (win->y + y) * 32;
      switch (
          *(((struct UiRenderWork *)base)->tilemap + (line + (win->x + x)))) {
      case UI_TILE_FRAME_TOP:
        tile -= 0xc0;
        break;
      case UI_TILE_DIVIDER_HORIZONTAL:
        tile -= 0xd0;
        break;
      }
    } else {
      x++;
      y++;
    }
    pos = (win->y + y) * 32 + (win->x + x);
    if (pos < 32 * 20)
      ((struct UiRenderWork *)work)->tilemap[pos] = UI_TILE(tile);
  }
}

#endif

enum {
  TILEMAP_ENTRY_PLAIN,
  TILEMAP_ENTRY_NONE,
  TILEMAP_ENTRY_PALETTE_14,
  TILEMAP_ENTRY_PALETTE_15,
  TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

/// Writes value into a window's tilemap at tile (x, y) of its interior, with
/// the palette mode names (TILEMAP_ENTRY_), or as it is for
/// TILEMAP_ENTRY_PLAIN; TILEMAP_ENTRY_NONE writes nothing.
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y,
                              u32 mode) {
  struct UiRenderWork *work = UI_WORK;
  s32 palette;
  s32 index;

  y += 1;
  x += 1;
  if (y > (u32)(window->height - 1))
    return;
  if (x > (u32)(window->width - 1))
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

  switch (mode) {
  case TILEMAP_ENTRY_NONE:
    return;
  case TILEMAP_ENTRY_PALETTE_14:
  case TILEMAP_ENTRY_PALETTE_15:
  case TILEMAP_ENTRY_PALETTE_1:
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

/// Steps a scaling sprite's animation (UI_SPRITE_SWELL to
/// UI_SPRITE_SHRINK_HALF) and sets its affine scale for this frame.
void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output) {
  s32 scale = 256;
  struct CursorSprite *sprite = SPRITE_CELL(output);
  struct AffineTransform effect;

  switch ((u8)output->active) {
  case UI_SPRITE_SWELL:
    scale = gUi_BounceScales[output->animStep++ & 31];
    break;
  case UI_SPRITE_SWELL_HALF:
    scale = gUi_BounceScales[output->animStep++ & 31] >> 1;
    break;
  case UI_SPRITE_SHRINK:
    if (output->animStep <= 7)
      scale = gUi_BounceScales[output->animStep++ * 2 + 16];
    break;
  case UI_SPRITE_SHRINK_HALF:
    if (output->animStep <= 7)
      scale = gUi_BounceScales[output->animStep++ * 2 + 16] >> 1;
    break;
  }
  if (scale == 256) {
    sprite->affineIndex = 0;
    sprite->affine = 0;
    sprite->x = output->x;
    sprite->y = output->y;
  } else {
    effect.scaleX = scale;
    effect.scaleY = scale;
    effect.angle = 0;
    sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
    if (scale > 256) {
      sprite->affine = 3;
      /* FAKEMATCH: needed to match: the unsigned halfword read of the x cell; a
       * cast of the signed member swaps the x load and the -8 pool load. */
      sprite->x = *(u16 *)&output->x - 8;
      sprite->y = (u16)output->y - 8;
    } else {
      sprite->affine = 1;
      sprite->x = output->x;
      sprite->y = output->y;
    }
  }
}

/// The per-frame sprite update: moves each sprite that an open window carries
/// by its animation mode (UI_SPRITE_) and queues it for drawing.
void UiWork_AnimateSpriteSlots(void) {
  struct UiRenderWork *work = UI_WORK;
  struct UiWindow *window = work->windows;
  s32 no;
  struct AffineTransform effect;

  for (no = 0; no != WINDOW_COUNT; window++, no++) {
    struct SpriteOutput *output;
    u32 phase;

    if (!(window->flags & UI_WINDOW_OPEN))
      continue;
    output = (struct SpriteOutput *)window->output.head;
    phase = (gFrameTick >> 2) & 7;
    while (output != NULL) {
      struct CursorSprite *sprite = &output->attr;

      if (window->effect == UI_WINDOW_EFFECT_SWELL) {
        output->animStep = 2;
        output->active = UI_SPRITE_PULSE;
      }
      switch (output->active) {
      case UI_SPRITE_PAGE_ARROW:
        if (work->glyphResource != 0x60) {
          const u8 *tiles = &gUiSprite_CursorFrameTiles[phase * 128];

          sprite->tile = VramCache_Load(work->glyphResource, 128, tiles);
          output->index = sprite->tile;
          sprite->mode = 0;
          sprite->mosaic = 0;
          sprite->colors = 1;
          sprite->size = 0;
          sprite->shape = 2;
          sprite->y =
              output->y + gUiSprite_CursorBobOffsets[gFrameTick % 80] + 2;
          sprite->affine = 0;
          sprite->affineIndex = 0;
        }
        break;
      case UI_SPRITE_SHAKE:
        if (gFrameTick & 1) {
          sprite->x =
              output->x +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
          sprite->y =
              output->y +
              (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
        }
        break;
      case UI_SPRITE_POP:
        if (output->animStep == 0) {
          sprite->affineIndex = 0;
          sprite->affine = 0;
          sprite->x = output->x;
          sprite->y = output->y;
          break;
        }
        effect.scaleX = 512;
        effect.scaleY = 512;
        effect.angle = 0;
        sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
        sprite->affine = 3;
        sprite->x = output->x - 5;
        sprite->y = output->y - 5;
        output->animStep--;
        break;
      case UI_SPRITE_SPIN:
        effect.scaleX = 256;
        effect.scaleY = 256;
        output->animStep += 768;
        effect.angle = output->animStep;
        sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
        sprite->affine = 1;
        sprite->x = output->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
        sprite->y = output->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
        break;
      case UI_SPRITE_WOBBLE:
        if (gFrameTick & 1)
          output->animStep++;
        sprite->x =
            output->x + gUiSprite_WobbleOffsets[output->animStep % 20][0];
        sprite->y =
            output->y + gUiSprite_WobbleOffsets[output->animStep % 20][1] - 2;
        break;
      case UI_SPRITE_HOP_UP:
        output->animStep++;
        sprite->y = output->y - gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case UI_SPRITE_HOP_DOWN:
      case UI_SPRITE_HOP_DOWN + 1:
      case UI_SPRITE_HOP_DOWN + 2:
        output->animStep++;
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case UI_SPRITE_HOP_DOWN_LEFT:
        output->animStep++;
        sprite->x = output->x - gUiSprite_BounceOffsets[output->animStep & 15];
        sprite->y = output->y + gUiSprite_BounceOffsets[output->animStep & 15];
        break;
      case UI_SPRITE_PULSE:
        if (output->animStep != 0) {
          effect.scaleX = 320;
          effect.scaleY = 320;
          effect.angle = 0;
          sprite->affineIndex = AffineMatrix_BuildForEffect(&effect);
          sprite->affine = 3;
          sprite->x = output->x - 8;
          sprite->y = output->y - 8;
          output->animStep--;
          break;
        }
        sprite->affineIndex = 0;
        sprite->affine = 0;
        sprite->x = output->x;
        sprite->y = output->y;
        break;
      case UI_SPRITE_SWELL:
      case UI_SPRITE_SWELL_HALF:
      case UI_SPRITE_SHRINK:
      case UI_SPRITE_SHRINK_HALF:
        RenderOutput_UpdateScaleAnimation((struct RenderOutput *)output);
        break;
      }
      if (output->active == UI_SPRITE_PAGE_ARROW) {
        if (work->glyphResource != 0x60)
          Runtime_PushSlotEntry(sprite, output->sentinel);
      } else if (output->active != UI_SPRITE_HIDDEN) {
        Runtime_PushSlotEntry(sprite, output->sentinel);
      }
      output = output->next;
      phase = (gFrameTick >> 2) & 7;
    }
  }
}
