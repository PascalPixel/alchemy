/// @file
/// The battle background, its palette and modes, the camera's zoom and the
/// units' highlights, and a unit's element glow: the star field of a cast, the
/// streaks, the pillar and the burst.
// Part of battle.c.

#include "battle_internal.h"
#include "resource_ids.h"
#include "battle/battle_effect_work.h"
#include "fixed_math.h"
#include "battle/presentation.h"
#include "system/system.h"

/// The zoomed background's BG2CNT for the first 256 picture tiles: a 256 by
/// 256 affine map in screen block 7, 256 colours, priority 2.
#define BACKGROUND_PLANE_UPPER                                                 \
  (BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |                   \
   BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2))
/// The same map, reading the remaining picture tiles from character block 3.
#define BACKGROUND_PLANE_LOWER                                                 \
  (BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |                   \
   BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))
#define BACKGROUND_PLANE_OFF                                                   \
  (BGCNT_WRAP | BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK) |                   \
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

/// Darkens the battle background's palette by level steps of a sixtieth;
/// level 0 restores its saved colours.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u16 *palette = battle->palette;
  volatile u32 ime;

  IRQ_LOCK(ime);

  if (level == 0) {
    Dma_Set(palette, (void *)BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
  } else {
    ColorBuffer_Scale(palette, BATTLE_BG_PALETTE,
                      battle->brightness =
                          FIXED_ONE - level * BACKGROUND_LEVEL_STEP,
                      BATTLE_BACKGROUND_COLORS);
  }
  Irq_Unlock(&ime);
}

/// Switches the battle background's display mode and rebuilds both its maps.
/// A nonzero fade darkens the saved palette by taking fade from each colour
/// channel, and BATTLE_BACKGROUND_FADE_RESTORE puts it back undarkened; the
/// background's id is not read.
void BattleBackground_SetMode(s32 mode, s32 backgroundId, s32 fade) {
  struct BattleBackgroundWork *background = Ram_HeapSlots->battleBackground;

  if (background->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK + 127);
  background->mode = mode;

  if (mode == BATTLE_BACKGROUND_FIXED)
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_HALFWORD);

  // The battle windows' second text palette takes the first sprite palette's
  // colours, over the window's background colour.
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

  if (fade == BATTLE_BACKGROUND_FADE_RESTORE) {
    struct BattleWork *battle;
    u16 *palette;

    battle = Ram_HeapSlots->battleWork;
    palette = battle->palette;
    Dma_Set(palette, (void *)BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BACKGROUND_COLORS), REG_DMA3);
  } else if (fade != 0) {
    struct BattleWork *battle;
    u16 *source;
    u16 *destination;
    s32 i;

    battle = Ram_HeapSlots->battleWork;
    source = battle->palette;
    destination = BATTLE_BG_PALETTE;

    for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
      s32 red = RGB_RED(source[i]);
      s32 green = RGB_GREEN(source[i]);
      s32 blue = RGB_BLUE(source[i]);

      if (red > fade)
        red -= fade;
      else
        red = 0;
      if (green > fade)
        green -= fade;
      else
        green = 0;
      if (blue > fade)
        blue -= fade;
      else
        blue = 0;
      destination[i] = RGB(red, green, blue);
    }
  }
  Graphics_BuildSequentialTileTable((void *)BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(
      (void *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK));
}

/// Allocates the zoomed background's scanline work.
void BattleBackground_Init(void) {
  void *block = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SCANLINES,
                                    sizeof(struct BattleAffineHdma));
  struct BattleBackgroundWork *background = Ram_HeapSlots->battleBackground;

  IWRAM_FILL_WORDS(block, sizeof(struct BattleAffineHdma), 0);
  background->mode = BATTLE_BACKGROUND_NONE;
}

/// Frees the zoomed background's scanline work.
void BattleBackground_Free(void) {
  HeapSlot_Release(HEAP_SLOT_BATTLE_SCANLINES);
}

/// Loads a battle background: its picture decoded into BG VRAM, its palette
/// darkened by level steps of 1/60 (none when level is negative), its tile
/// table and tilemap, and the HBlank scroll the first time.
void BattleBackground_Load(s32 mode, s32 resource, s32 level) {
  struct BattleBackgroundWork *background = Ram_HeapSlots->battleBackground;
  u8 *data = Resource_GetTableEntry(resource);
  struct BattleWork *battle = (struct BattleWork *)Ram_HeapSlots->battleWork;
  ModelDecoder decode;
  u16 *palette;

  RamCode_Load(decode, Model_DecodeCode, Model_DecodeCodeSize);
  decode(data + BACKGROUND_PALETTE_BYTES, (void *)BG_CHAR_BLOCK(2));
  Heap_Release(decode);
  palette = battle->palette;
  Dma_Set(data, palette, DMA_COPY_WORDS(BACKGROUND_PALETTE_BYTES / 4),
          REG_DMA3);
  if (level >= 0)
    ColorBuffer_Scale(palette, BATTLE_BG_PALETTE,
                      battle->brightness =
                          FIXED_ONE - level * BACKGROUND_LEVEL_STEP,
                      BATTLE_BACKGROUND_COLORS);
  if (battle->keepBackdropPalette == 0) {
    Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  }
  Graphics_BuildSequentialTileTable((void *)BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(
      (void *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK));
  // BG1's last row is left clear.
  Iwram_ClearWordsEntry((void *)(BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK) +
                                 BG_SCREEN_SIZE - BG_SCREEN_WIDTH * 2),
                        BG_SCREEN_WIDTH * 2);
  if (background->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK + 127);
  background->mode = mode;
  if (mode == BATTLE_BACKGROUND_FIXED)
    REG_BG1CNT = BATTLE_BG1CNT;
}

/// Builds the zoomed background's next page for a zoom of scale about centerX
/// and centerY, shifted by offsetX and offsetY, and shows it: below a zoom of 1
/// the plane lies flat, from 1 it wraps and turns with the camera.
void BattleCamera_SetRange(FIXED centerX, FIXED centerY, s32 offsetX,
                           s32 offsetY, FIXED scale) {
  struct BattleBackgroundWork *background = Ram_HeapSlots->battleBackground;
  struct BattleAffineHdma *scanlines = Ram_HeapSlots->battleScanlines;
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 panX;
  u16 wrap;
  u16 *line;
  struct BattleAffine *affine;
  FIXED ratio;
  s32 step;
  FIXED x;
  FIXED y;
  u32 scanline;
  u32 lowerHalfLine;

  panX = BACKGROUND_PAN_X;
  wrap = 0;
  ratio = FIXED_ONE;
  if (scale >= ratio) {
    wrap = BGCNT_WRAP;
    panX = BACKGROUND_PAN_X + YAW_PARTY_ACTION * 3 + -(s16)camera->yaw * 3;
  }
  if (scanlines == NULL)
    return;
  if ((background->mode == BATTLE_BACKGROUND_FIXED ||
       background->secondMode == BATTLE_BACKGROUND_FIXED) &&
      background->busy == 0)
    gBgScroll[1].x = panX >> 8;
  if (background->mode != BATTLE_BACKGROUND_ZOOMED)
    return;

  // The affine parameters: a plain zoom, the reference point moved so the
  // centre stays put and the picture's top row is the first line below the
  // plane's hidden lines.
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, ratio);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulFX(ratio, Iwram_MulFX(centerX, scale - FIXED_ONE));
  y = Iwram_MulFX(ratio, Iwram_MulFX(centerY, scale - FIXED_ONE));
  affine->x = ((x + toFIXED(0.5) - 1) >> 8) + offsetX + panX;
  affine->y = ((y + toFIXED(0.5) - 1) >> 8) + offsetY -
              Q_8_8(BACKGROUND_PLANE_HIDDEN_LINES);
  // The first line that reaches the picture's lower half at this zoom.
  lowerHalfLine =
      FIXEDtoINT(Iwram_RatioMulQ14(
          (s16)affine->pd, (BACKGROUND_UPPER_HALF_LINES << 8) - affine->y)) +
      1;
  scanline = 0;
  if (!GameFlag_Get(FLAG_CHOOSING_COMMANDS)) {
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
  FIXED focal = BATTLE_CAMERA_DISTANCE;
  // UB: zoom is never set, so the scale is whatever its register held; UBFIX
  // takes it as a parameter.
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 *focus = camera->focus;
  FIXED scale = INTtoFIXED(zoom) / 100;
  s32 eyeOffset[3];
  s32 originOnScreen[3];
  s32 origin[3];
  FIXED nearShare;
  FIXED halfFocal;
  FIXED zoomedFocal;
  u32 nearDepth;

  focus[0] = x;
  focus[1] = depth;
  focus[2] = y;
  origin[0] = 0;
  nearShare = FIXED_ONE * 3 / 4;
  origin[1] = 0;
  origin[2] = 0;

  nearDepth = Iwram_RatioMulQ14(focal, nearShare);
  Projection_Set(focal, nearDepth, focal * 2);
  Matrix_Initialize();
  Matrix_TranslateVector(focus);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);
  eyeOffset[0] = 0;
  eyeOffset[1] = 0;
  eyeOffset[2] = camera->distance;
  Iwram_TransformVector(eyeOffset, camera->eye);
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
  zoomedFocal = halfFocal * 2;
  nearDepth = Iwram_RatioMulQ14(zoomedFocal, nearShare);
  Projection_Set(zoomedFocal, nearDepth, halfFocal * 4);
}

/// Points the battle camera at (x, depth, y) with a zoom in percent, and lines
/// the zoomed background up with it: where the battlefield's origin falls on
/// the screen moves the plane as much, and the zoom scales the focal length
/// and the far depth.
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 *focus = camera->focus;
  FIXED scale = INTtoFIXED(zoom) / 100;
  FIXED eyeOffset[3];
  s32 originOnScreen[3];
  s32 origin[3];
  FIXED focal = BATTLE_CAMERA_DISTANCE;
  FIXED nearShare;
  FIXED halfFocal;
  FIXED zoomedFocal;
  u32 nearDepth;

  focus[0] = x;
  focus[1] = depth;
  focus[2] = y;
  origin[0] = 0;
  nearShare = FIXED_ONE * 3 / 4;
  origin[1] = 0;
  origin[2] = 0;

  nearDepth = Iwram_RatioMulQ14(focal, nearShare);
  Projection_Set(focal, nearDepth, focal * 2);
  Matrix_Initialize();
  Matrix_TranslateVector(focus);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);
  eyeOffset[0] = 0;
  eyeOffset[1] = 0;
  eyeOffset[2] = focal;
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
  zoomedFocal = halfFocal * 2;
  nearDepth = Iwram_RatioMulQ14(zoomedFocal, nearShare);
  Projection_Set(zoomedFocal, nearDepth, halfFocal * 4);
}

/// Points the battle camera at the ground between two units, with a zoom in
/// percent.
void BattleCamera_SetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                        s32 zoom) {
  struct MotionObject *first = BattleObject_Get(firstUnitId)->object;
  struct MotionObject *second = BattleObject_Get(secondUnitId)->object;
  FIXED firstX = first->x;
  FIXED secondX = second->x;
  FIXED firstZ = first->z;
  FIXED secondZ = second->z;
  FIXED x = (secondX + firstX) / 2;
  FIXED z = (secondZ + firstZ) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames, its blend weight falling
/// by 2 a frame from 15, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaOut(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX) - step;
    wait(1);
  }
}

/// Fades BG2 in over the backdrop in eight frames, its blend weight rising by
/// 2 a frame from 1, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    wait(1);
  }
}

/// Unused: sets BLDCNT to brighten every layer and the backdrop.
UNUSED void Battle_BrightenAllLayers(void) {
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

/// Unused: returns the 16.16 product of a and b.
UNUSED FIXED Battle_MulFX(FIXED a, FIXED b) { return Iwram_MulFX(a, b); }

/// What BattleTransition_InitializeEntries treats as an array of these;
/// Matrix_SetUnit writes over it as a matrix.
struct BattleTransitionEntry {
  u32 value;   ///< the first word, read before and after the identity is set
  u32 sum;     ///< set to the first word's old value plus its new one
  u32 rest[2]; ///< never read
};

/// Unused: sets entries to the identity transform, then stores the first word's
/// old value plus its new one in the second.
UNUSED void
BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(entries);
  entries[0].sum = previous + entries[0].value;
}

/// Unused: returns value divided by 16.
UNUSED s32 BattleBackground_DivideBy16(s32 value) { return value / 16; }

/// What Select picks: another value in the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DEBUG_VALUE_SELECT 411
#else
#define DEBUG_VALUE_SELECT 544
#endif
/// What R picks, in every release.
#define DEBUG_VALUE_R 399
/// The digits the debug value steps: left and right step its ones, up and
/// down its tens, L and R its hundreds.
#define DEBUG_VALUE_TENS 10
#define DEBUG_VALUE_HUNDREDS 100

/// Unused debug aid: with Start held, the d-pad and L/R step the battle work's
/// debugValue by its ones, tens and hundreds until A takes it; Select held
/// picks DEBUG_VALUE_SELECT and R held DEBUG_VALUE_R; otherwise value comes
/// back unchanged.
UNUSED s32 Battle_AdjustDebugValue(s32 value) {
  if (gInput.held & KEY_START) {
    while (1) {
      struct BattleWork *battle = Ram_HeapSlots->battleWork;

      if (gInput.repeat & KEY_LEFT)
        battle->debugValue--;
      if (gInput.repeat & KEY_RIGHT)
        battle->debugValue++;
      if (gInput.repeat & KEY_L)
        battle->debugValue -= DEBUG_VALUE_HUNDREDS;
      if (gInput.repeat & KEY_R)
        battle->debugValue += DEBUG_VALUE_HUNDREDS;
      if (gInput.repeat & KEY_UP)
        battle->debugValue -= DEBUG_VALUE_TENS;
      if (gInput.repeat & KEY_DOWN)
        battle->debugValue += DEBUG_VALUE_TENS;
      if (gInput.pressed & KEY_A) {
        value = battle->debugValue;
        break;
      }
      wait(1);
    }
  }
  if (gInput.held & KEY_SELECT)
    value = DEBUG_VALUE_SELECT;
  if (gInput.held & KEY_R)
    value = DEBUG_VALUE_R;
  return value;
}

/// Sets the sprite mode of a unit's picture and its shadow.
void BattleHighlight_SetSpriteMode(s32 unitId, s32 mode) {
  struct BattleObject *slot = BattleObject_Get(unitId);
  struct MotionObject *object;
  struct Sprite *record;
  struct Sprite **records;
  s32 recordIndex;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->recordStorageKind & ACTOR_SPRITE_KIND_MASK) {
  case ACTOR_SPRITE_ONE: {
    record = object->records;
    record->image.objectMode = mode;
    record->shadow.objectMode = mode;
    break;
  }
  case ACTOR_SPRITE_GROUP: {
    records = object->records;
    recordIndex = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->image.objectMode = mode;
      record->shadow.objectMode = mode;
      recordIndex++;
    } while (recordIndex < ACTOR_SPRITE_GROUP_SIZE);
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

extern s8 gBattleHighlight_BlendRamp[];

/// The steps of each half of gBattleHighlight_BlendRamp: the first half plays
/// once, then the second over and over.
#define HIGHLIGHT_RAMP_HALF 16

/// Steps the highlight's blend: HIGHLIGHT_FADE_IN and HIGHLIGHT_FADE_OUT fade
/// the units listed in or out over 32 frames, HIGHLIGHT_PULSE blends them by
/// the ramp, and HIGHLIGHT_SELECTED lightens them by it: the first sixteen
/// steps once, then the second sixteen over and over.
void BattleBackground_StepHighlightBlend(void) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;

  if (battle != NULL) {
    if (battle->highlightMode == HIGHLIGHT_FADE_IN) {
      u32 level = battle->highlightStep >> 1;

      if (level > BLEND_WEIGHT_MAX)
        level = BLEND_WEIGHT_MAX;
      REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
      REG_BLDALPHA = ((BLEND_WEIGHT_MAX - level) << 8) | level;
      battle->highlightStep++;
    } else if (battle->highlightMode == HIGHLIGHT_FADE_OUT) {
      u32 level = battle->highlightStep >> 1;

      if (level > BLEND_WEIGHT_MAX)
        level = BLEND_WEIGHT_MAX;
      REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
      REG_BLDALPHA = (level << 8) | (BLEND_WEIGHT_MAX - level);
      battle->highlightStep++;
    } else if (battle->highlightMode == HIGHLIGHT_PULSE) {
      REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
      REG_BLDALPHA = (gBattleHighlight_BlendRamp[battle->highlightStep] << 8) |
                     (BLEND_WEIGHT_MAX -
                      gBattleHighlight_BlendRamp[battle->highlightStep]);
      battle->highlightStep =
          ((battle->highlightStep + 1) & (HIGHLIGHT_RAMP_HALF - 1)) |
          (battle->highlightStep >= HIGHLIGHT_RAMP_HALF - 1
               ? HIGHLIGHT_RAMP_HALF
               : 0);
    } else if (battle->highlightMode != HIGHLIGHT_OFF) {
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
      REG_BLDY = gBattleHighlight_BlendRamp[battle->highlightStep];
      battle->highlightStep =
          ((battle->highlightStep + 1) & (HIGHLIGHT_RAMP_HALF - 1)) |
          (battle->highlightStep >= HIGHLIGHT_RAMP_HALF - 1
               ? HIGHLIGHT_RAMP_HALF
               : 0);
    }
  }
}

/// Highlights the units in actors (up to UNIT_MAX, ended by
/// BATTLE_UNIT_LIST_END), the others taking the other blend, or with
/// HIGHLIGHT_OFF stops the highlight.
void BattleHighlight_SetModes(u16 *actors, s32 mode) {
  s16 activeActors[UNIT_MAX];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u32 count;
  u32 i;

  if (mode == HIGHLIGHT_OFF) {
    Task_Remove(BattleBackground_StepHighlightBlend);
    Io_Write16(mode, &REG_BLDY);
    BattleHighlight_ClearAllSpriteModes();
    wait(1);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
  }
  if (battle != NULL && mode != HIGHLIGHT_OFF) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);

    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, (u16 *)activeActors);
    for (i = 0; i < count; i++)
      BattleHighlight_SetSpriteMode(activeActors[i], mode & 1);

    if (actors != NULL) {
      u32 actor = *actors;

      i = 0;
      actors++;
      if (actor != BATTLE_UNIT_LIST_END) {
        do {
          BattleHighlight_SetSpriteMode(actor, (mode & 1) ^ 1);
          i++;
          if (i >= UNIT_MAX)
            break;
          actor = *actors++;
        } while (actor != BATTLE_UNIT_LIST_END);
      }
    }
    wait(1);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
    Task_Add(BattleBackground_StepHighlightBlend, TASK_PASS_VBLANK);
  }
}

/// One of the star field's sparks, drawn inward to the centre. Sixteen live
/// in SparkWork.sparks; a spent one starts again while the field is young.
struct Spark {
  s32 motion[6]; ///< x, y, z, then their velocities, in STAR_FIELD_PIXEL units
  s32 life;      ///< frames left
};

/// A ring of the star field leaving its centre: its place, its speed and its
/// age.
struct Ring {
  s32 x;
  s32 y;
  s32 velocityX;
  s32 velocityY;
  s32 age;
};

/// The star field's sparks and rings, and the frames in which a spent spark
/// starts again.
#define STAR_FIELD_SPARKS 16
#define STAR_FIELD_RINGS 3
#define STAR_FIELD_SPAWN_FRAMES 25
#define STAR_FIELD_SIZE 128 ///< the canvas's width and height in pixels
#define STAR_FIELD_CENTER (STAR_FIELD_SIZE / 2)
#define STAR_FIELD_RING_SIZE 32
#define STAR_FIELD_FRAMES 45 ///< the frames a cast's glow lasts
/// The blue ring sheet's colours, before its pictures; an element's palette
/// replaces them.
#define STAR_FIELD_COLORS 64
/// The sparks and rings move in 1/1024 canvas pixels: STAR_FIELD_PIXEL to a
/// pixel.
#define STAR_FIELD_PIXEL_SHIFT 10
#define STAR_FIELD_PIXEL (1 << STAR_FIELD_PIXEL_SHIFT)

/// The star field's work, for the 45 frames of a cast's glow.
struct SparkWork {
  /// The blue ring sheet's pictures, the sparks' and the rings'.
  u8 sheet[4544];
  struct Spark sparks[STAR_FIELD_SPARKS];
  struct Ring rings[STAR_FIELD_RINGS];
  s32 frames; ///< frames drawn; new sparks start only in the first
              ///< STAR_FIELD_SPAWN_FRAMES
  s32 ready;  ///< 1 when a drawn frame waits for ElementGlow_UploadStarField
  /// Where BG2 shows the 128 by 128 canvas, in 8.8 pixels: ElementGlow_Play
  /// keeps it on the unit.
  s32 originX;
  s32 originY;
  s32 originPending; ///< 1 once the origin is queued for REG_BG2X
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// The star tables: each picture's size and sheet offset, and the ring's four
// pictures.
extern const u8 gElementGlow_StarSizes[];
extern const s32 gElementGlow_StarPictures[];
extern const s32 gElementGlow_RingPictures[];

/// Draws the star field's next frame: each spark falls towards the centre and
/// shrinks as its life runs out, a spent one starting again while the field is
/// young, and the three rings move outward.
void ElementGlow_UpdateStarField(void) {
  s32 j;
  s32 frame;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *dest;
  s32 i;
  struct SparkWork *work;
  DrawFunc draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *pos;
  s32 k;
  FIXED dist;
  FIXED scale;
  s32 step;
  u32 radius;
  s32 angle;
  u32 half;
  s32 x;
  s32 y;
  s32 life;
  s32 (*mul)(s32 a, s32 b);

  dest = Ram_HeapSlots->battleCanvas;
  work = (struct SparkWork *)Ram_HeapSlots->battleEffect;
  work->ready = 0;
  draw = (DrawFunc)Ram_HeapSlots->ramCode;
  for (i = 0; i < STAR_FIELD_SPARKS; i++) {
    spark = &work->sparks[i];
    life = spark->life;
    if (life != 0) {
      dist = Math_SqrtFX((spark->motion[0] >> 8) * (spark->motion[0] >> 8) +
                         (spark->motion[1] >> 8) * (spark->motion[1] >> 8) +
                         (spark->motion[2] >> 8) * (spark->motion[2] >> 8));
      if (dist <= toFIXED(1) / 16 - 1) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(dist, FIXED_ONE);
        spark->life--;
        mul = Iwram_MulFX;
        pos = spark->motion;
        for (k = 0; k < 3; k++) {
          step = mul(mul(-*pos >> 8, scale), FIXED_ONE * 19 / 16);
          pos[3] = pos[3] - (pos[3] >> 7) + step;
          *pos += pos[3];
          pos++;
        }
      }
      life = spark->life;
    }
    if (life == 0 && work->frames < STAR_FIELD_SPAWN_FRAMES) {
      angle = Random16();
      radius = Random16() + FIXED_ONE;
      half = radius >> 1;
      spark->motion[0] = Iwram_MulFX(Trig_Cos(angle), half);
      spark->motion[1] = Iwram_MulFX(Trig_Sin(angle), half);
      if (spark->motion[0] & 1)
        spark->motion[0] = -spark->motion[0];
      if (spark->motion[1] & 1)
        spark->motion[1] = -spark->motion[1];
      spark->motion[2] = (u32)(Random16() + toFIXED(0.5)) >> 2;
      spark->motion[3] = (-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
      spark->motion[4] = (-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
      spark->motion[5] = 0;
      life = spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
    }
    if (life != 0) {
      x = (spark->motion[0] >> STAR_FIELD_PIXEL_SHIFT) + STAR_FIELD_CENTER;
      y = (spark->motion[1] >> STAR_FIELD_PIXEL_SHIFT) + STAR_FIELD_CENTER;
      frame = life;
      if (frame < 0)
        frame = 0;
      else if (frame > 6)
        frame = 6;
      size = gElementGlow_StarSizes[frame];
      draw(dest, (u8 *)work + gElementGlow_StarPictures[frame], x - (size >> 1),
           y - (size >> 1), size, size);
    }
  }
  draw = (DrawFunc)Ram_HeapSlots->ramCodePaired;
  for (ring = work->rings, j = STAR_FIELD_RINGS - 1; j >= 0; j--, ring++) {
    ring->x += ring->velocityX;
    ring->y += ring->velocityY;
    ringX = ring->x >> STAR_FIELD_PIXEL_SHIFT;
    ringY = ring->y >> STAR_FIELD_PIXEL_SHIFT;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      /* FAKEMATCH(dead): an empty once-only block after the age's store; as the
       * store alone. */
      do {
      } while (0);
      draw(dest, (u8 *)work + gElementGlow_RingPictures[phase],
           ringX + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
           ringY + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
           STAR_FIELD_RING_SIZE, STAR_FIELD_RING_SIZE);
    }
  }
  work->frames++;
  work->ready = 1;
}

s32 ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);

/// Copies the drawn canvas to BG2's tiles, at three quarters of its
/// brightness.
s32 ElementGlow_UploadStarField(void) {
  void *source = Ram_HeapSlots->battleCanvas;
  struct SparkWork *work = Ram_HeapSlots->battleEffect;

  if (source != 0) {
    if (work->ready != 0) {
      work->ready = 0;
      return ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(1),
                                            BG_CHAR_SIZE);
    }
  }
  return (s32)source;
}

/// Returns the x of the point at angle and radius from the origin.
static __inline__ s32 PolarX(s32 angle, s32 radius) {
  return Iwram_MulFX(Trig_Cos(angle), radius);
}

/// Returns the y of the point at angle and radius from the origin.
static __inline__ s32 PolarY(s32 angle, s32 radius) {
  return Iwram_MulFX(Trig_Sin(angle), radius);
}

/// Starts a star field of the element: sixteen sparks on random rings and three
/// rings 120 degrees apart, in the element's colours. Returns the upload task's
/// index.
s32 ElementGlow_InitializeStarField(s32 element) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 i;
  u8 *data;

  Ram_HeapSlots->battleBackground->mode = BATTLE_BACKGROUND_FIXED;
  work = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  for (i = 0; i < STAR_FIELD_SPARKS; i++) {
    s32 angle = Random16();
    u32 radius = Random16() + FIXED_ONE;
    u32 half = radius >> 1;

    spark = &work->sparks[i];
    spark->motion[0] = Iwram_MulFX(Trig_Cos(angle), half);
    spark->motion[1] = Iwram_MulFX(Trig_Sin(angle), half);
    if (spark->motion[0] & 1)
      spark->motion[0] = -spark->motion[0];
    if (spark->motion[1] & 1)
      spark->motion[1] = -spark->motion[1];
    spark->motion[2] = (u32)(Random16() + toFIXED(0.5)) >> 2;
    spark->motion[3] = (-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
    spark->motion[4] = (-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
    spark->motion[5] = 0;
    spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
  }
  for (i = 0; i < STAR_FIELD_RINGS; i++) {
    s32 radius = 4 * STAR_FIELD_PIXEL;
    s32 angle;

    ring = &work->rings[i];
    angle = i * (DEGtoANG(120));
    ring->x = PolarX(angle, radius);
    ring->y = PolarY(angle, radius);
    ring->velocityX = PolarX(angle, STAR_FIELD_PIXEL / 2);
    ring->velocityY = PolarY(angle, STAR_FIELD_PIXEL / 2);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->originPending = 0;
  Iwram_ClearWordsEntry(Ram_HeapSlots->battleCanvas,
                        STAR_FIELD_SIZE * STAR_FIELD_SIZE);
  data = Resource_GetTableEntry(RESOURCE_BLUE_RING_SHEET);
  Iwram_CopyWordsEntry((void *)BG_PLTT, data, STAR_FIELD_COLORS * 2);
  data += STAR_FIELD_COLORS * 2;
  Decompress_TaggedLz(data, work);
  switch (element) {
  case ELEMENT_VENUS:
    data = Resource_GetTableEntry(RESOURCE_YELLOW_PALETTE_B);
    break;
  case ELEMENT_MERCURY:
    data = Resource_GetTableEntry(RESOURCE_BLUE_RING_SHEET);
    break;
  case ELEMENT_MARS:
    data = Resource_GetTableEntry(RESOURCE_RED_PALETTE_C);
    break;
  default:
    data = Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_D);
    break;
  }
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(STAR_FIELD_COLORS / 2),
          REG_DMA3);
  Io_Write32(0, &REG_BG2X);
  Io_Write32(0, &REG_BG2Y);
  Io_Write16(Q_8_8(1), &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(Q_8_8(1), &REG_BG2PD);
  /* LEFTOVER(frame): the twin's five-argument BattleEffect_BuildBlitterFar,
   * never taken here; the ROM's frame still reserves its fifth argument and
   * never touches it; as the call left out. */
  if (0)
    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 7, 7, 3, 3);
  // Clipped to the canvas, the additive blitter and then the one that keeps
  // the brighter colour, packed as BattleEffect_LoadBlitter takes them.
  BattleEffect_LoadBlitterFar(HEAP_SLOT_RAM_CODE, COMPOSITE_CLIP_X |
                                                      COMPOSITE_CLIP_Y |
                                                      BLITTER_ADDITIVE << 4);
  BattleEffect_LoadBlitterFar(HEAP_SLOT_RAM_CODE_PAIRED, COMPOSITE_CLIP_X |
                                                             COMPOSITE_CLIP_Y |
                                                             BLITTER_MAX << 4);
  Task_Add(ElementGlow_UpdateStarField, TASK_PASS_FRAME);
  return Task_Add(ElementGlow_UploadStarField, TASK_PASS_FRAME);
}

/// Ends a star field and turns BG2 off.
s32 ElementGlow_End(s32 unused) {
  Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  Task_Remove(ElementGlow_UploadStarField);
  return Task_Remove(ElementGlow_UpdateStarField);
}

/// Unused: does nothing.
UNUSED void ElementGlow_ReservedNoOp(void) {}

/// Plays a glow of the element about a unit, and returns when it is over: the
/// star field, the streaks, the pillar or the burst.
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 variant) {
  struct ScreenPoint starPosition;
  struct ScreenPoint streakPosition;
  struct BattlePresentationWork pillarLaunch;
  struct BattlePresentationWork burstLaunch;
  s32 origin[2];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  volatile u16 *ime;
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct ScreenPoint *pos;

  wait(1);
  BattleBackground_SetMode(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                                              DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                                              DISPCNT_WIN0_ON);
  IoWriteQueue_Store16((u32)&REG_BG2CNT, BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                                             BGCNT_SCREEN_BASE(7));
  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  wait(1);
  Io_Write16(DISPLAY_WIDTH, &REG_WIN0H);
  Io_Write16(WIN_RANGE(16, 136), &REG_WIN0V);
  Io_Write16(WININ_WIN0_ALL, &REG_WININ);
  Io_Write16(WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ, &REG_WINOUT);
  if (mode == GLOW_STAR_FIELD) {
    // The star field for 45 frames, the background darkening for the first 25.
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, BLEND_ALPHA(14, BLEND_WEIGHT_MAX));
    ElementGlow_InitializeStarField(element);
    for (frame = 0, pos = &starPosition; frame < STAR_FIELD_FRAMES; frame++) {
      brightness = &battle->brightness;
      ime = &REG_IME;
      work = (struct SparkWork *)Ram_HeapSlots->battleEffect;
      if (frame <= 24)
        ColorBuffer_Scale(battle->palette, BATTLE_BG_PALETTE,
                          *brightness =
                              FIXED_ONE - frame * BACKGROUND_LEVEL_STEP,
                          BATTLE_BACKGROUND_COLORS);
      BattleMotion_ProjectScaledPosition(unitId, (s32 *)pos);
      work->originX = (STAR_FIELD_CENTER - pos->x) << 8;
      work->originY = (STAR_FIELD_CENTER - pos->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, (u32)&work->originX,
                              DMA_COPY_WORDS(2));
      work->originPending = 1;
      wait(1);
    }
    ElementGlow_End(element);
  } else if (mode == GLOW_STREAKS) {
    // The streaks for 40 frames, their canvas centred on the unit likewise.
    BattleEffect_StartElementStreaksFar(element);
    for (frame = 0; frame < 40; frame++) {
      BattleMotion_ProjectScaledPosition(unitId, (s32 *)&streakPosition);
      origin[0] = (STAR_FIELD_CENTER - streakPosition.x) << 8;
      origin[1] = (STAR_FIELD_CENTER - streakPosition.y) << 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, (u32)origin, DMA_COPY_WORDS(2));
      wait(1);
    }
    BattleEffect_EndElementStreaksFar();
  } else if (mode == GLOW_PILLAR) {
    // The pillar and the burst run a battle effect with the unit as its user
    // and only target.
    pillarLaunch.flags = 0;
    pillarLaunch.kind = element;
    pillarLaunch.variant = variant;
    pillarLaunch.actor = unitId;
    pillarLaunch.actors[0] = unitId;
    pillarLaunch.firstTarget = unitId;
    pillarLaunch.count = 1;
    pillarLaunch.initialValue = 1;
    BattleEffect_RunElementPillarFar(&pillarLaunch);
  } else {
    burstLaunch.flags = 0;
    burstLaunch.kind = element;
    burstLaunch.variant = 0;
    burstLaunch.actor = unitId;
    burstLaunch.firstTarget = unitId;
    burstLaunch.actors[0] = unitId;
    burstLaunch.count = 1;
    burstLaunch.initialValue = 1;
    BattleEffect_RunElementBurstFar(&burstLaunch);
  }
}

/// Copies the background's saved palette back undarkened, as the star field of
/// ElementGlow_Play leaves it dark.
void BattleBackground_RestorePalette(void) {
  BattleBackground_SetPaletteLevel(0, 0);
}

/// Unused: removes the VCount interrupt's handler, as BattleTransition_Run
/// installs one.
UNUSED void Irq_RemoveVCountHandler(void) { Irq_SetHandler(IRQ_VCOUNT, 0, 0); }

/// The average level of a formation's enemy kinds, counting only kinds the
/// player has defeated (ENEMY_DEFEATED_FLAG, or all with
/// FLAG_MONSTER_BATTLE_ANY_ENEMY); -1 for an empty formation, -2 when one
/// present kind is undefeated, -3 when none counts.
s32 BattleFormation_AverageLevel(s32 recordId) {
  struct Formation *record;
  const struct EnemyDefinition *enemy;
  u32 memberIndex;
  s32 enemyId;
  s32 levelTotal;
  s32 enemyCount;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 levels[7];

  enemyCount = 0;
  levelTotal = 0;
  record = &gFormations[recordId];
  for (memberIndex = 0; memberIndex < FORMATION_MEMBERS; memberIndex++) {
    if (record->minimumCounts[memberIndex] != 0)
      break;
  }
  if (memberIndex == FORMATION_MEMBERS)
    return -1;
  for (memberIndex = 0; memberIndex < FORMATION_MEMBERS; memberIndex++) {
    if (record->maximumCounts[memberIndex] == 0)
      continue;
    enemyId = record->memberIds[memberIndex];
    enemy = Enemy_GetDefinitionFar(enemyId + ENEMY_ID_FIRST);
    if (enemy == 0)
      continue;
    if (!GameFlag_Get(FLAG_MONSTER_BATTLE_ANY_ENEMY) &&
        !GameFlag_Get(ENEMY_DEFEATED_FLAG(enemyId + ENEMY_ID_FIRST)))
      return -2;
    levelTotal += enemy->level;
    enemyCount++;
  }
  if (enemyCount == 0)
    return -3;
  return levelTotal / enemyCount;
}
