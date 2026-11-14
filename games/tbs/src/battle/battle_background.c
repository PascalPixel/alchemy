/// @file
/// The battle background, its palette and modes, the camera's zoom and
/// the units' highlights.
// Part of battle.c.

#include "battle_internal.h"

// The value Select picks in the debug value editor.
#if TBS_PATCH_1
#define DEBUG_SELECT_VALUE 399
#else
#define DEBUG_SELECT_VALUE 480
#endif

/// The zoomed background's BG2CNT for the first 256 picture tiles: a 256 by
/// 256 affine map in screen block 7, 256 colours, priority 2.
#define BACKGROUND_PLANE_UPPER                                                 \
  (BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |                   \
   BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2))
/// The same map, reading the remaining picture tiles from character block 3.
#define BACKGROUND_PLANE_LOWER                                                 \
  (BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |                   \
   BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))
/// Lines outside the plane read the padded map in screen block 31.
#define BACKGROUND_PLANE_OFF                                                   \
  (BGCNT_WRAP | BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |                     \
   BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))

/// The screen lines the plane covers, 0 to 135, above the status window.
#define BACKGROUND_PLANE_LINES 136
/// The plane's top lines, left off except while commands are chosen.
#define BACKGROUND_PLANE_HIDDEN_LINES 16

/// Darkens the background by level, 0 for its own colours.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *battle = gBattleWork;
  u16 *palette = battle->palette;
  u32 ime;
  /* FAKEMATCH(temporary): a pointer to ime. */
  u32 *savedInterruptEnable = &ime;

  /* FAKEMATCH(dead): updates that cancel out end CSE's knowledge that the
   * pointer is the stack slot's address. */
  if (palette) {
    savedInterruptEnable++;
    savedInterruptEnable--;
  }
  IRQ_LOCK(*savedInterruptEnable);

  if (level == 0) {
    Dma_Set(palette, BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BG_PALETTE_COLORS), REG_DMA3);
  } else {
    Graphics_ScaleRgb555Clamped(palette, BATTLE_BG_PALETTE,
                                battle->brightness =
                                    Q16_ONE - level * BACKGROUND_LEVEL_STEP,
                                BATTLE_BG_PALETTE_COLORS);
  }
  {
    u32 saved = *savedInterruptEnable;

    REG_IME = saved;
  }
}

/// Switches the background's display mode, optionally fades its palette by
/// fade, and rebuilds both its maps.
void BattleBackground_SetMode(s32 mode, s32 unusedResource, s32 fade) {
  struct BattleBackgroundWork *transition = gBattleBackgroundWork;

  if (transition->mode == BATTLE_BACKGROUND_NONE) {
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK + 127);
  }
  transition->mode = mode;

  if (mode == BATTLE_BACKGROUND_FIXED) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_16);
  }

  // The battle windows' second text palette takes the first sprite palette's
  // colours, over the window's background colour.
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

  if (fade == BATTLE_BACKGROUND_FADE_RESTORE) {
    Dma_Set(gBattleWork->palette, BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BG_PALETTE_COLORS), REG_DMA3);
  } else if (fade != 0) {
    u16 *source = gBattleWork->palette;
    u16 *destination = BATTLE_BG_PALETTE;
    s32 colorIndex;

    for (colorIndex = 0; colorIndex != BATTLE_BG_PALETTE_COLORS; colorIndex++) {
      s32 red = source[colorIndex] & 31;
      s32 green = (source[colorIndex] >> 5) & 31;
      s32 blue = (source[colorIndex] >> 10) & 31;

      if (red > fade) {
        red -= fade;
      } else {
        red = 0;
      }
      if (green > fade) {
        green -= fade;
      } else {
        green = 0;
      }
      if (blue > fade) {
        blue -= fade;
      } else {
        blue = 0;
      }
      destination[colorIndex] = RGB(red, green, blue);
    }
  }

  Graphics_BuildSequentialTileTable(BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(BG_SCREEN_BLOCK(31));
}

/// Allocates the zoomed background's scanline work.
void BattleBackground_Init(void) {
  struct BattleAffineHdma *work;
  struct BattleBackgroundWork *state;
  work = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SCANLINES, sizeof *work);
  state = gBattleBackgroundWork;
  Dma_Fill32(0, work, sizeof *work);
  state->mode = BATTLE_BACKGROUND_NONE;
}

/// Frees the zoomed background's scanline work.
void BattleBackground_Free(void) {
  HeapSlot_Release(HEAP_SLOT_BATTLE_SCANLINES);
}

/// The bit decoder BattleBackground_Load copies to RAM and runs.
typedef void (*BitDecoder)(const u8 *source, void *destination);

/// Loads a battle background, its picture, its palette darkened by level (none
/// when negative) and both its maps.
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level) {
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  u8 *backgroundData = Resource_GetTableEntry(resourceId);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  u16 *palette;

  RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, BitDecoder_DecodeImage,
                   BitDecoder_Size);
  {
    u32 offset = 256;
    void *vram;

    /* FAKEMATCH(dead): two updates that cancel, under a test that emits
     * nothing, end a CSE block. */
    if (battle) {
      offset++;
      offset--;
    }
    vram = BG_CHAR_BLOCK(2);
    ((BitDecoder)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_TEMP))(
        backgroundData + offset, vram);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_TEMP);
  palette = battle->palette;
  Dma_Set(backgroundData, palette, DMA_COPY_WORDS(64), REG_DMA3);
  if (level >= 0)
    Graphics_ScaleRgb555Clamped(palette, BATTLE_BG_PALETTE,
                                battle->brightness =
                                    Q16_ONE - level * BACKGROUND_LEVEL_STEP,
                                BATTLE_BG_PALETTE_COLORS);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  Graphics_BuildSequentialTileTable(BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(BG_SCREEN_BLOCK(31));
  // BG1's last row is left clear.
  Iwram_ClearWords((BG_SCREEN_BLOCK(31) + BG_SCREEN_SIZE - 64), 64);
  if (view->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK + 127);
  view->mode = mode;
  if (mode == BATTLE_BACKGROUND_FIXED)
    REG_BG1CNT = BATTLE_BG1CNT;
}

/// Builds the zoomed background's next page for a zoom of scale about centerX
/// and centerY, shifted by offsetX and offsetY, and shows it: below a zoom of 1
/// the plane lies flat, from 1 it wraps and turns with the camera.
void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale) {
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleAffineHdma *scanlines =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SCANLINES);
  struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  u16 wrap;
  s32 panX;
  u16 *line;
  struct BattleAffine *affine;
  s32 ratio;
  s32 step;
  s32 x;
  s32 y;
  u32 scanline;
  u32 lowerHalfLine;

  // The sideways pan, 8.8 pixels: 8 when zoomed out, else 104 less three
  // times the yaw, so the picture turns with the camera.
  panX = 8 << 8;
  wrap = 0;
  if (scale >= Q16_ONE) {
    wrap = BGCNT_WRAP;
    panX = (104 << 8) + -(s16)camera->yaw * 3;
  }
  if (scanlines == NULL)
    return;
  if ((view->mode == BATTLE_BACKGROUND_FIXED ||
       view->secondMode == BATTLE_BACKGROUND_FIXED) &&
      view->busy == 0)
    gBgScroll[1].x = panX >> 8;
  if (view->mode != BATTLE_BACKGROUND_ZOOMED)
    return;

  // The affine parameters: a plain zoom, the reference point moved so the
  // centre stays put, 16 pixels up.
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, Q16_ONE);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - Q16_ONE));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - Q16_ONE));
  affine->x = ((x + 0x7fff) >> 8) + offsetX + panX;
  y = ((y + 0x7fff) >> 8) + offsetY - (16 << 8);
  affine->y = y;

  // The first line that reaches the picture's lower half at this zoom.
  lowerHalfLine = (Iwram_RatioMulQ14((s16)step, (64 << 8) - y) >> 16) + 1;

  scanline = 0;
  if (!GameFlag_GetFar(FLAG_CHOOSING_COMMANDS)) {
    for (; scanline < BACKGROUND_PLANE_HIDDEN_LINES; scanline++)
      *line++ = BACKGROUND_PLANE_OFF;
  }
  if (lowerHalfLine > BACKGROUND_PLANE_LINES)
    lowerHalfLine = BACKGROUND_PLANE_LINES;
  for (; scanline < lowerHalfLine; scanline++)
    *line++ = wrap | BACKGROUND_PLANE_UPPER;
  for (; scanline < BACKGROUND_PLANE_LINES; scanline++)
    *line++ = wrap | BACKGROUND_PLANE_LOWER;
  for (; scanline < DISPLAY_HEIGHT; scanline++)
    *line++ = BACKGROUND_PLANE_OFF;
  scanlines->page ^= 1;
}

/// Unused: sets the camera's eye at its stored distance without taking a zoom.
#ifdef UBFIX
UNUSED void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) {
#else
UNUSED void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) {
#endif
  s32 span = BATTLE_CAMERA_DISTANCE;
  // UB: zoom is never set, so the scale is whatever its register held; UBFIX
  // takes it as a parameter.
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *camera = gBattleCamera;
  s32 *focus = camera->focus;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  SET_VECTOR(focus, x, depth, y);
  sourceBounds[0] = 0;
  alpha = Q16_ONE * 3 / 4;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Transform_Reset();
  Transform_ApplyPosition(focus);
  Transform_ApplyYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = camera->distance;
  Iwram_TransformVector(renderBounds, (s32 *)camera);
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Transform_Reset();
  Transform_SetLookAt(camera->eye, focus);
  Transform_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(px(120), px(120), (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 255;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

/// Points the battle camera at (x, depth, y) with a zoom in percent, and lines
/// the zoomed background up with it.
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *camera = gBattleCamera;
  s32 *focus = camera->focus;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 span = BATTLE_CAMERA_DISTANCE;
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  SET_VECTOR(focus, x, depth, y);
  sourceBounds[0] = 0;
  alpha = Q16_ONE * 3 / 4;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  // The eye 510 pixels back from the focus.
  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Transform_Reset();
  Transform_ApplyPosition(focus);
  Transform_ApplyYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = span;
  Iwram_TransformVector(renderBounds, (s32 *)camera);
  // Where the battlefield's origin falls on the screen moves the plane as much.
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Transform_Reset();
  Transform_SetLookAt(camera->eye, focus);
  Transform_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(px(120), px(120), (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  // The zoom scales the focal length and the far depth.
  half = scale * 255;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

/// Points the battle camera at the ground between two units, with a zoom in
/// percent.
void BattleCamera_SetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                        s32 zoom) {
  struct Actor *firstActor = BattleObject_Get(firstUnitId)->actor;
  struct Actor *secondActor = BattleObject_Get(secondUnitId)->actor;
  s32 firstX = firstActor->x.fixed;
  s32 secondX = secondActor->x.fixed;
  s32 firstZ = firstActor->z.fixed;
  s32 secondZ = secondActor->z.fixed;
  s32 x = (secondX + firstX) / 2;
  s32 z = (secondZ + firstZ) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames.
void BattleBackground_FadeAlphaOut(void) {
  s32 alphaStep;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (alphaStep = 1; alphaStep <= 16; alphaStep += 2) {
    REG_BLDALPHA = BLEND_ALPHA(16, 16) - alphaStep;
    wait(1);
  }
}

/// Fades BG2 in over the backdrop in eight frames.
void BattleBackground_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, 16);
    wait(1);
  }
}

/// What BattleTransition_InitializeEntries treats as an array of
/// these; Transform_SetIdentity writes over it as a matrix.
struct BattleTransitionEntry {
  u32 value;   ///< the first word, read before and after the identity is set
  u32 sum;     ///< set to the first word's old value plus its new one
  u32 rest[2]; ///< never read
};

/// Unused: lightens every layer by REG_BLDY.
void BattleBackground_SetBlendControl(void) {
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

/// Unused: sets entries to the identity transform, then stores the first word's
/// old value plus its new one in the second.
void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  // XXX (no evidence): what the sum is for; nothing calls this in either game.
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}

/// Unused.
s32 BattleBackground_DivideBy16(s32 value) { return value / 16; }

/// Unused debug aid: while Start is held, edits a value with the d-pad until A
/// takes it; with Select held, returns DEBUG_SELECT_VALUE.
s32 Battle_AdjustDebugValue(s32 selectedValue) {
  struct BattleWork *battle;

  if (gKeysHeld & KEY_START) {
    for (;;) {
      battle = gBattleWork;
      if (gKeysRepeat & KEY_LEFT)
        battle->debugValue -= 1;
      if (gKeysRepeat & KEY_RIGHT)
        battle->debugValue += 1;
      if (gKeysRepeat & KEY_UP)
        battle->debugValue -= 100;
      if (gKeysRepeat & KEY_DOWN)
        battle->debugValue += 100;
      if (gKeysPressed & KEY_A)
        selectedValue = battle->debugValue;
      else {
        wait(1);
        continue;
      }
      break;
    }
  }
  if (gKeysHeld & KEY_SELECT)
    selectedValue = DEBUG_SELECT_VALUE;
  return selectedValue;
}

/// Sets the sprite mode of a unit's picture and its shadow.
void BattleHighlight_SetSpriteMode(s32 unitId, s32 mode) {
  struct BattleObject *slot = BattleObject_Get(unitId);
  struct Actor *actor;
  struct Sprite *record;
  struct Sprite **records;
  s32 recordIndex;

  if (slot == 0)
    return;
  actor = slot->actor;
  if (actor == 0)
    return;
  switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
  case ACTOR_SPRITE_ONE: {
    record = actor->sprite;
    record->image.objectMode = mode;
    record->shadow.objectMode = mode;
    break;
  }
  case ACTOR_SPRITE_GROUP: {
    records = (struct Sprite **)actor->sprite;
    recordIndex = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->image.objectMode = mode;
      record->shadow.objectMode = mode;
    } while (++recordIndex < ACTOR_SPRITE_GROUP_SIZE);
    break;
  }
  }
}

/// Unused: makes every unit's sprites semi-transparent but selectedUnitId's.
void BattleHighlight_SetOtherSpriteModes(s32 selectedUnitId) {
  s16 unitIds[UNIT_MAX];
  s32 unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (unitIds[unitIndex] != selectedUnitId)
      BattleHighlight_SetSpriteMode(unitIds[unitIndex],
                                    SPRITE_SEMI_TRANSPARENT);
  }
}

/// Returns every unit's sprites to SPRITE_OPAQUE.
void BattleHighlight_ClearAllSpriteModes(void) {
  s16 unitIds[UNIT_MAX];
  s32 unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
    BattleHighlight_SetSpriteMode(unitIds[unitIndex], SPRITE_OPAQUE);
}

/// Lightens the highlighted units by the next step of the blend ramp: the first
/// sixteen steps once, then the second sixteen over and over.
void BattleBackground_StepHighlightBlend(void) {
  struct BattleWork *battle = gBattleWork;
  s8 *blendRamp;
  u16 rampIndex;
  s32 nextIndex;

  if (battle != NULL && battle->highlightMode != HIGHLIGHT_OFF) {
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = BLEND_ALPHA(16, 0);
    blendRamp = gBattleHighlight_BlendRamp;
    REG_BLDY = blendRamp[battle->highlightStep];
    // From step 15 on, the next step stays in 16 to 31.
    rampIndex = battle->highlightStep;
    nextIndex = (rampIndex + 1) & 0xf;
    if (rampIndex > 14)
      nextIndex |= 0x10;
    battle->highlightStep = nextIndex;
  }
}

/// Highlights the units in selectedUnits, the others turning semi-transparent,
/// or with HIGHLIGHT_OFF stops the highlight.
void BattleHighlight_SetModes(u16 *selectedUnits, s32 mode) {
  s16 unitIds[UNIT_MAX];
  struct BattleWork *battle = gBattleWork;
  u32 unitCount;
  u32 unitIndex;

  if (mode == HIGHLIGHT_OFF) {
    Task_Remove(BattleBackground_StepHighlightBlend);
    REG_BLDY = mode;
    BattleHighlight_ClearAllSpriteModes();
    wait(1);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  }
  if (battle != 0 && mode != HIGHLIGHT_OFF) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);

    unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
    for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
      BattleHighlight_SetSpriteMode(unitIds[unitIndex], mode & 1);

    if (selectedUnits != 0) {
      u32 unitId = *selectedUnits;
      unitIndex = 0;
      selectedUnits++;
      if (unitId != BATTLE_UNIT_LIST_END) {
        do {
          BattleHighlight_SetSpriteMode(unitId, (mode & 1) ^ 1);
          unitIndex++;
          // A unit list holds at most 14.
          if (unitIndex > 13)
            break;
          unitId = *selectedUnits++;
        } while (unitId != BATTLE_UNIT_LIST_END);
      }
    }
    wait(1);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
    Task_Add(BattleBackground_StepHighlightBlend, TASK_PASS_VBLANK);
  }
}

#undef BACKGROUND_PLANE_LINES
#undef BACKGROUND_PLANE_HIDDEN_LINES
#undef BACKGROUND_PLANE_UPPER
#undef BACKGROUND_PLANE_LOWER
#undef BACKGROUND_PLANE_OFF
