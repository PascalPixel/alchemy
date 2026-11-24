/// @file
/// The battle background, its palette and modes, the camera's zoom and
/// the units' highlights.
// Part of battle.c.

#include "battle_internal.h"

/// The value Select picks in the debug value editor.
#if TBS_PATCH_1
#define DEBUG_VALUE_SELECT 399
#else
#define DEBUG_VALUE_SELECT 480
#endif
/// The debug value editor's step for up and down; left and right step by one.
#define DEBUG_VALUE_COARSE_STEP 100

/// The zoomed background's BG2CNT for the first 256 picture tiles: a 256 by
/// 256 affine map in screen block 7, 256 colours, priority 2.
#define BACKGROUND_PLANE_UPPER                                                 \
  (BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |                   \
   BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2))
/// The same map, reading the remaining picture tiles from character block 3.
#define BACKGROUND_PLANE_LOWER                                                 \
  (BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |                   \
   BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))
/// Lines outside the plane read the padded map in this screen block.
#define BACKGROUND_PADDED_SCREEN 31
#define BACKGROUND_PLANE_OFF                                                   \
  (BGCNT_WRAP | BGCNT_SCREEN_BASE(BACKGROUND_PADDED_SCREEN) |                  \
   BGCNT_256_COLORS | BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))

/// The screen lines the plane covers, 0 to 135, above the status window.
#define BACKGROUND_PLANE_LINES 136
/// The plane's top lines, left off except while commands are chosen.
#define BACKGROUND_PLANE_HIDDEN_LINES 16
/// The picture's lines that BACKGROUND_PLANE_UPPER reads: its first 256
/// tiles, 32 to a row.
#define BACKGROUND_UPPER_HALF_LINES 64

/// A battle background resource: its BATTLE_BACKGROUND_COLORS colours, then
/// its packed picture.
#define BACKGROUND_PALETTE_BYTES (BATTLE_BACKGROUND_COLORS * 2)

/// The fixed background's sideways pan in 8.8 pixels while the zoom is below
/// 1; from 1 on it moves three 8.8 pixels a step of the camera's yaw, so the
/// picture turns with the camera, and is the same when the camera faces the
/// party's actions (YAW_PARTY_ACTION).
#define BACKGROUND_PAN_X Q_8_8(8)

/// The steps of each half of gBattleHighlight_BlendRamp: the first half plays
/// once, then the second over and over.
#define HIGHLIGHT_RAMP_HALF 16

/// Darkens the background by level, 0 for its own colours.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleState *battle = gBattleState;
  u16 *palette = battle->palette;
  volatile u32 ime;

  Irq_Lock(&ime);

  if (level == 0) {
    Dma_Set(palette, BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
  } else {
    Graphics_ScaleRgb555Clamped(palette, BATTLE_BG_PALETTE,
                                battle->brightness =
                                    FIXED_ONE - level * BACKGROUND_LEVEL_STEP,
                                BATTLE_BACKGROUND_COLORS);
  }
  Irq_Unlock(&ime);
}

/// Switches the background's display mode, optionally fades its palette by
/// fade, and rebuilds both its maps.
void BattleBackground_SetMode(s32 mode, s32 unusedResource, s32 fade) {
  struct BattleBackgroundState *view = gBattleBackgroundState;

  if (view->mode == BATTLE_BACKGROUND_NONE) {
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK_FIRST);
  }
  view->mode = mode;

  if (mode == BATTLE_BACKGROUND_FIXED) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_HALFWORD);
  }

  // The battle windows' second text palette takes the first sprite palette's
  // colours, over the window's background colour.
  Dma_Set(OBJ_PLTT, &BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

  if (fade == BATTLE_BACKGROUND_FADE_RESTORE) {
    Dma_Set(gBattleState->palette, BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
  } else if (fade != 0) {
    u16 *source = gBattleState->palette;
    u16 *destination = BATTLE_BG_PALETTE;
    s32 colorIndex;

    for (colorIndex = 0; colorIndex != BATTLE_BACKGROUND_COLORS; colorIndex++) {
      s32 red = RGB_RED(source[colorIndex]);
      s32 green = RGB_GREEN(source[colorIndex]);
      s32 blue = RGB_BLUE(source[colorIndex]);

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

  BattleTransition_BuildAffineTilemap(BG_SCREEN_BLOCK(7));
  BattleTransition_BuildFlatTilemap(BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN));
}

/// Allocates the zoomed background's scanline state.
void BattleBackground_Init(void) {
  struct BattleAffineHdma *scanlines;
  struct BattleBackgroundState *view;
  scanlines =
      HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SCANLINES, sizeof *scanlines);
  view = gBattleBackgroundState;
  Dma_Fill32(0, scanlines, sizeof *scanlines);
  view->mode = BATTLE_BACKGROUND_NONE;
}

/// Frees the zoomed background's scanline state.
void BattleBackground_Free(void) {
  HeapSlot_Release(HEAP_SLOT_BATTLE_SCANLINES);
}

/// The bit decoder BattleBackground_Load copies to RAM and runs. It returns
/// the source after the last halfword it read.
typedef const u8 *(*BitDecoder)(const u8 *source, void *destination);

/// Loads a battle background, its picture, its palette darkened by level (none
/// when negative) and both its maps.
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level) {
  struct BattleBackgroundState *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  u8 *backgroundData = Resource_GetTableEntry(resourceId);
  struct BattleState *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  u16 *palette;

  RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, BitDecoder_DecodeImage,
                   BitDecoder_Size);
  ((BitDecoder)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_TEMP))(
      backgroundData + BACKGROUND_PALETTE_BYTES, BG_CHAR_BLOCK(2));
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_TEMP);
  palette = battle->palette;
  Dma_Set(backgroundData, palette, DMA_COPY_WORDS(BACKGROUND_PALETTE_BYTES / 4),
          REG_DMA3);
  if (level >= 0)
    Graphics_ScaleRgb555Clamped(palette, BATTLE_BG_PALETTE,
                                battle->brightness =
                                    FIXED_ONE - level * BACKGROUND_LEVEL_STEP,
                                BATTLE_BACKGROUND_COLORS);
  Dma_Set(OBJ_PLTT, &BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  BattleTransition_BuildAffineTilemap(BG_SCREEN_BLOCK(7));
  BattleTransition_BuildFlatTilemap(BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN));
  // BG1's last row is left clear.
  Iwram_ClearWords((BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN) + BG_SCREEN_SIZE -
                    BG_SCREEN_WIDTH * 2),
                   BG_SCREEN_WIDTH * 2);
  if (view->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK_FIRST);
  view->mode = mode;
  if (mode == BATTLE_BACKGROUND_FIXED)
    REG_BG1CNT = BATTLE_BG1CNT;
}

/// Builds the zoomed background's next page for a zoom of scale about centerX
/// and centerY, shifted by offsetX and offsetY, and shows it: below a zoom of 1
/// the plane lies flat, from 1 it wraps and turns with the camera.
void BattleCamera_SetRange(FIXED centerX, FIXED centerY, s32 offsetX,
                           s32 offsetY, FIXED scale) {
  struct BattleBackgroundState *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleAffineHdma *scanlines =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SCANLINES);
  struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  u16 wrap;
  s32 panX;
  u16 *line;
  struct BattleAffine *affine;
  FIXED ratio;
  s32 step;
  FIXED x;
  s32 y;
  u32 scanline;
  u32 lowerHalfLine;

  panX = BACKGROUND_PAN_X;
  wrap = 0;
  if (scale >= FIXED_ONE) {
    wrap = BGCNT_WRAP;
    panX = BACKGROUND_PAN_X + YAW_PARTY_ACTION * 3 + -(s16)camera->yaw * 3;
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
  // centre stays put and the picture's top row is the first line below the
  // plane's hidden lines.
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_DivFX(scale, FIXED_ONE);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulFX(ratio, Iwram_MulFX(centerX, scale - FIXED_ONE));
  y = Iwram_MulFX(ratio, Iwram_MulFX(centerY, scale - FIXED_ONE));
  affine->x = ((x + toFIXED(0.5) - 1) >> 8) + offsetX + panX;
  y = ((y + toFIXED(0.5) - 1) >> 8) + offsetY -
      Q_8_8(BACKGROUND_PLANE_HIDDEN_LINES);
  affine->y = y;

  // The first line that reaches the picture's lower half at this zoom.
  lowerHalfLine = FIXEDtoINT(Iwram_DivFX(
                      (s16)step, (BACKGROUND_UPPER_HALF_LINES << 8) - y)) +
                  1;

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

/// Unused: BattleCamera_SetView with the eye at the camera's own distance
/// rather than BATTLE_CAMERA_DISTANCE, and no zoom taken; written as a call to
/// it, the eye would stay at BATTLE_CAMERA_DISTANCE.
#ifdef UBFIX
UNUSED void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) {
#else
UNUSED void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) {
#endif
  FIXED distance = BATTLE_CAMERA_DISTANCE;
  // UB: zoom is never set, so the scale is whatever it happens to hold; UBFIX
  // takes it as a parameter.
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *camera = gBattleCamera;
  s32 *focus = camera->focus;
  FIXED scale = INTtoFIXED(zoom) / 100;
  s32 eyeOffset[3];
  s32 originOnScreen[3];
  s32 origin[3];
  FIXED nearShare;
  FIXED halfFocal;
  FIXED focal;
  u32 nearDepth;

  SET_VECTOR(focus, x, depth, y);
  origin[0] = 0;
  nearShare = FIXED_ONE * 3 / 4;
  origin[1] = 0;
  origin[2] = 0;

  nearDepth = Iwram_DivFX(distance, nearShare);
  Projection_Set(distance, nearDepth, distance * 2);
  Matrix_Initialize();
  Matrix_TranslateVector(focus);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);
  eyeOffset[0] = 0;
  eyeOffset[1] = 0;
  eyeOffset[2] = camera->distance;
  Iwram_TransformVector(eyeOffset, (s32 *)camera);
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Matrix_Initialize();
  Matrix_SetLookAt(camera->eye, focus);
  Matrix_ProjectPoint(origin, originOnScreen);

  BattleCamera_SetRange(
      toFIXED(BATTLE_PROJECTION_CENTER_X), toFIXED(BATTLE_PROJECTION_CENTER_Y),
      (BATTLE_PROJECTION_CENTER_X - originOnScreen[0]) << 8,
      (BATTLE_PROJECTION_CENTER_Y - originOnScreen[1]) << 8, scale);
  halfFocal = scale * (FIXEDtoINT(BATTLE_CAMERA_DISTANCE) / 2);
  focal = halfFocal * 2;
  nearDepth = Iwram_DivFX(focal, nearShare);
  Projection_Set(focal, nearDepth, halfFocal * 4);
}

/// Points the battle camera at (x, depth, y) with a zoom in percent, and lines
/// the zoomed background up with it.
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *camera = gBattleCamera;
  s32 *focus = camera->focus;
  FIXED scale = INTtoFIXED(zoom) / 100;
  FIXED eyeOffset[3];
  s32 originOnScreen[3];
  s32 origin[3];
  FIXED distance = BATTLE_CAMERA_DISTANCE;
  FIXED nearShare;
  FIXED halfFocal;
  FIXED focal;
  u32 nearDepth;

  SET_VECTOR(focus, x, depth, y);
  origin[0] = 0;
  nearShare = FIXED_ONE * 3 / 4;
  origin[1] = 0;
  origin[2] = 0;

  // The eye BATTLE_CAMERA_DISTANCE back from the focus.
  nearDepth = Iwram_DivFX(distance, nearShare);
  Projection_Set(distance, nearDepth, distance * 2);
  Matrix_Initialize();
  Matrix_TranslateVector(focus);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);
  eyeOffset[0] = 0;
  eyeOffset[1] = 0;
  eyeOffset[2] = distance;
  Iwram_TransformVector(eyeOffset, (s32 *)camera);
  // Where the battlefield's origin falls on the screen moves the plane as much.
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Matrix_Initialize();
  Matrix_SetLookAt(camera->eye, focus);
  Matrix_ProjectPoint(origin, originOnScreen);

  BattleCamera_SetRange(
      toFIXED(BATTLE_PROJECTION_CENTER_X), toFIXED(BATTLE_PROJECTION_CENTER_Y),
      (BATTLE_PROJECTION_CENTER_X - originOnScreen[0]) << 8,
      (BATTLE_PROJECTION_CENTER_Y - originOnScreen[1]) << 8, scale);
  // The zoom scales the focal length and both depths.
  halfFocal = scale * (FIXEDtoINT(BATTLE_CAMERA_DISTANCE) / 2);
  focal = halfFocal * 2;
  nearDepth = Iwram_DivFX(focal, nearShare);
  Projection_Set(focal, nearDepth, halfFocal * 4);
}

/// Points the battle camera at the ground between two units, with a zoom in
/// percent.
void BattleCamera_SetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                        s32 zoom) {
  struct Actor *firstActor = BattleObject_Get(firstUnitId)->actor;
  struct Actor *secondActor = BattleObject_Get(secondUnitId)->actor;
  FIXED firstX = firstActor->x.fixed;
  FIXED secondX = secondActor->x.fixed;
  FIXED firstZ = firstActor->z.fixed;
  FIXED secondZ = secondActor->z.fixed;
  FIXED x = (secondX + firstX) / 2;
  FIXED z = (secondZ + firstZ) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames.
void BattleBackground_FadeAlphaOut(void) {
  s32 alphaStep;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (alphaStep = 1; alphaStep <= BLEND_WEIGHT_MAX; alphaStep += 2) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX) - alphaStep;
    wait(1);
  }
}

/// Fades BG2 in over the backdrop in eight frames.
void BattleBackground_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    wait(1);
  }
}

/// What BattleTransition_InitializeEntries treats as an array of
/// these; Matrix_SetUnit writes over it as a matrix.
struct BattleTransitionEntry {
  u32 value;   ///< the first word, read before and after the identity is set
  u32 sum;     ///< set to the first word's old value plus its new one
  u32 rest[2]; ///< never read
};

/// Unused: lightens every layer by REG_BLDY.
UNUSED void BattleBackground_SetBlendControl(void) {
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

/// Unused: sets entries to the identity transform, then stores the first word's
/// old value plus its new one in the second.
UNUSED void
BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  // XXX (no evidence): what the sum is for; nothing calls this in either game.
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(entries);
  entries[0].sum = previous + entries[0].value;
}

/// Unused: returns value divided by 16.
UNUSED s32 BattleBackground_DivideBy16(s32 value) { return value / 16; }

/// Unused debug aid: while Start is held, edits a value with the d-pad until A
/// takes it; with Select held, returns DEBUG_VALUE_SELECT.
UNUSED s32 Battle_AdjustDebugValue(s32 selectedValue) {
  struct BattleState *battle;

  if (gKeysHeld & KEY_START) {
    for (;;) {
      battle = gBattleState;
      if (gKeysRepeat & KEY_LEFT)
        battle->debugValue--;
      if (gKeysRepeat & KEY_RIGHT)
        battle->debugValue++;
      if (gKeysRepeat & KEY_UP)
        battle->debugValue -= DEBUG_VALUE_COARSE_STEP;
      if (gKeysRepeat & KEY_DOWN)
        battle->debugValue += DEBUG_VALUE_COARSE_STEP;
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
    selectedValue = DEBUG_VALUE_SELECT;
  return selectedValue;
}

/// Sets the sprite mode of a unit's picture and its shadow.
void BattleHighlight_SetSpriteMode(s32 unitId, s32 mode) {
  struct BattleObject *slot = BattleObject_Get(unitId);
  struct Actor *actor;
  struct Sprite *sprite;
  struct Sprite **sprites;
  s32 spriteIndex;

  if (slot == 0)
    return;
  actor = slot->actor;
  if (actor == 0)
    return;
  switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
  case ACTOR_SPRITE_ONE: {
    sprite = actor->sprite;
    sprite->image.objectMode = mode;
    sprite->shadow.objectMode = mode;
    break;
  }
  case ACTOR_SPRITE_GROUP: {
    sprites = (struct Sprite **)actor->sprite;
    spriteIndex = 0;
    do {
      sprite = *sprites++;
      if (sprite == 0)
        break;
      sprite->image.objectMode = mode;
      sprite->shadow.objectMode = mode;
    } while (++spriteIndex < ACTOR_SPRITE_GROUP_SIZE);
    break;
  }
  }
}

/// Unused: makes every unit's sprites semi-transparent but selectedUnitId's.
UNUSED void BattleHighlight_SetOtherSpriteModes(s32 selectedUnitId) {
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
  struct BattleState *battle = gBattleState;
  s8 *blendRamp;
  u16 rampIndex;
  s32 nextIndex;

  if (battle != NULL && battle->highlightMode != HIGHLIGHT_OFF) {
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
    blendRamp = gBattleHighlight_BlendRamp;
    REG_BLDY = blendRamp[battle->highlightStep];
    // From the first half's last step on, the next step stays in the second.
    rampIndex = battle->highlightStep;
    nextIndex = (rampIndex + 1) & (HIGHLIGHT_RAMP_HALF - 1);
    if (rampIndex >= HIGHLIGHT_RAMP_HALF - 1)
      nextIndex |= HIGHLIGHT_RAMP_HALF;
    battle->highlightStep = nextIndex;
  }
}

/// Highlights the units in selectedUnits, the others turning semi-transparent,
/// or with HIGHLIGHT_OFF stops the highlight.
void BattleHighlight_SetModes(u16 *selectedUnits, s32 mode) {
  s16 unitIds[UNIT_MAX];
  struct BattleState *battle = gBattleState;
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
    Io_Write16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);

    unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
    for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
      BattleHighlight_SetSpriteMode(unitIds[unitIndex], mode & 1);

    if (selectedUnits != 0) {
      u32 unitId;

      for (unitIndex = 0; unitIndex < UNIT_MAX; unitIndex++) {
        unitId = *selectedUnits++;
        if (unitId == BATTLE_UNIT_LIST_END)
          break;
        BattleHighlight_SetSpriteMode(unitId, (mode & 1) ^ 1);
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
#undef BACKGROUND_UPPER_HALF_LINES
#undef BACKGROUND_PALETTE_BYTES
#undef BACKGROUND_PAN_X
#undef HIGHLIGHT_RAMP_HALF
#undef DEBUG_VALUE_SELECT
#undef DEBUG_VALUE_COARSE_STEP
