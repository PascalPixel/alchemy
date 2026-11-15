/// @file
/// The battle background, its palette and modes, the camera's zoom and the
/// units' highlights, the star field behind a summon and a unit's element
/// effect.
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
/// Lines outside the plane read the padded map in this screen block.
#define BACKGROUND_PADDED_SCREEN 31
#define BACKGROUND_PLANE_OFF                                                   \
  (BGCNT_WRAP | BGCNT_SCREEN_BASE(BACKGROUND_PADDED_SCREEN) |                  \
   BGCNT_256_COLORS | BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(2))
/// The screen lines the plane covers, 0 to 135, above the status window.
#define BACKGROUND_PLANE_LINES 136
/// The plane's top lines, left off except while commands are chosen.
#define BACKGROUND_PLANE_HIDDEN_LINES 16

/// Sets the palette brightness. Level 0 transfers the original colours
/// unchanged.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  u16 *palette = screen->palette;
  volatile u32 ime;

  IRQ_LOCK(ime);

  if (level == 0) {
    Dma_Set(palette, (void *)BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BG_PALETTE_COLORS), REG_DMA3);
  } else {
    ColorBuffer_Scale(palette, BATTLE_BG_PALETTE,
                      screen->brightness =
                          FIXED_ONE - level * BACKGROUND_LEVEL_STEP,
                      BATTLE_BG_PALETTE_COLORS);
  }
  Irq_Unlock(&ime);
}

/// As The Broken Seal's, with this game's heap slots, its callback
/// BattleBackground_ShowZoomedPage at 0xcff and its IO write of 0x1f83 to
/// BG1CNT.
void BattleBackground_SetMode(s32 mode, s32 value, s32 fade) {
  struct BattleBackgroundView *transition =
      (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;

  if (transition->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_FRAME + 127);
  transition->mode = mode;

  if (mode == BATTLE_BACKGROUND_FIXED)
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_HALFWORD);

  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

  if (fade == BATTLE_BACKGROUND_FADE_RESTORE) {
    struct BattleWork *work;
    u16 *palette;

    work = Ram_HeapSlots->battleWork;
    palette = work->palette;
    Dma_Set(palette, (void *)BATTLE_BG_PALETTE,
            DMA_COPY_HALFWORDS(BATTLE_BG_PALETTE_COLORS), REG_DMA3);
  } else if (fade != 0) {
    struct BattleWork *work;
    u16 *source;
    u16 *destination;
    s32 i;

    work = Ram_HeapSlots->battleWork;
    source = work->palette;
    destination = BATTLE_BG_PALETTE;

    for (i = 0; i != BG_PLTT_COLORS / 2; i++) {
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
      destination[i] = (blue << 10) | (green << 5) | red;
    }
  }
  Graphics_BuildSequentialTileTable((void *)BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(
      (void *)BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN));
}

/// Allocates the zoomed background's scanline work.
void BattleBackground_Init(void) {
  void *block = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SCANLINES, 0x2a0);
  struct BattleBackgroundWork *work = Ram_HeapSlots->battleBackground;

  IWRAM_FILL_WORDS(block, 0x2a0, 0);
  work->mode = 0;
}

/// Frees the zoomed background's scanline work.
void BattleBackground_Free(void) {
  HeapSlot_Release(HEAP_SLOT_BATTLE_SCANLINES);
}

/// Loads a battle background: its picture decoded into BG VRAM, its palette
/// darkened by level steps of 1/60 (none when level is negative), its tile
/// table and tilemap, and the HBlank scroll the first time.
void BattleBackground_Load(s32 mode, s32 resource, s32 level) {
  struct BattleBackgroundWork *view = Ram_HeapSlots->battleBackground;
  u8 *data = Resource_GetTableEntry(resource);
  struct BattleWork *session = (struct BattleWork *)Ram_HeapSlots->battleWork;
  ModelDecoder decode;
  u16 *palette;

  RamCode_Load(decode, Model_DecodeCode, Model_DecodeCodeSize);
  decode(data + 256, (void *)BG_CHAR_BLOCK(2));
  Heap_Release(decode);
  palette = session->palette;
  Dma_Set(data, palette, DMA_COPY_WORDS(64), REG_DMA3);
  if (level >= 0)
    ColorBuffer_Scale(palette, BATTLE_BG_PALETTE,
                      session->brightness =
                          FIXED_ONE - level * BACKGROUND_LEVEL_STEP,
                      BATTLE_BG_PALETTE_COLORS);
  if (session->keepBackdropPalette == 0) {
    Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
            DMA_COPY_HALFWORDS(16), REG_DMA3);
    BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
        BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  }
  Graphics_BuildSequentialTileTable((void *)BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(
      (void *)BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN));
  // BG1's last row is left clear.
  Iwram_ClearWordsEntry(
      (void *)(BG_SCREEN_BLOCK(BACKGROUND_PADDED_SCREEN) + BG_SCREEN_SIZE - 64),
      64);
  if (view->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_FRAME + 127);
  view->mode = mode;
  if (mode == BATTLE_BACKGROUND_FIXED)
    REG_BG1CNT = BATTLE_BG1CNT;
}

/// Builds the zoomed background's next page for a zoom of scale about centerX
/// and centerY, shifted by offsetX and offsetY, and shows it: below a zoom of 1
/// the plane lies flat, from 1 it wraps and turns with the camera.
void BattleCamera_SetRange(FIXED centerX, FIXED centerY, s32 offsetX,
                           s32 offsetY, FIXED scale) {
  struct BattleBackgroundView *view =
      (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;
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

  // The sideways pan, 8.8 pixels: 8 when zoomed out, else 104 less three
  // times the yaw, so the picture turns with the camera.
  panX = 8 << 8;
  wrap = 0;
  ratio = FIXED_ONE;
  if (scale >= ratio) {
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
  affine->x = ((x + 0x7fff) >> 8) + offsetX + panX;
  affine->y = ((y + 0x7fff) >> 8) + offsetY - (16 << 8);
  lowerHalfLine =
      (Iwram_RatioMulQ14((s16)affine->pd, (64 << 8) - affine->y) >> 16) + 1;
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

#ifdef UBFIX
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) {
#else
void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) {
#endif
  FIXED span = BATTLE_CAMERA_DISTANCE;
  // UB: zoom is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
#ifndef UBFIX
  s32 zoom;
#endif
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->focus;
  FIXED scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  FIXED alpha;
  FIXED half;
  FIXED width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = FIXED_ONE * 3 / 4;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Matrix_Initialize();
  Matrix_TranslateVector(pos);
  Matrix_RotateY((s16)scene->yaw);
  Matrix_RotateX((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = scene->distance;
  Iwram_TransformVector(renderBounds, scene->eye);
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Matrix_Initialize();
  Matrix_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(
      toFIXED(BATTLE_PROJECTION_CENTER_X), toFIXED(BATTLE_PROJECTION_CENTER_Y),
      (BATTLE_PROJECTION_CENTER_X - measuredBounds[0]) << 8,
      (BATTLE_PROJECTION_CENTER_Y - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

/// Places the battle camera at a position and zoom for a transition: sets the
/// projection, turns the scene to the camera's yaw and pitch, projects the
/// origin and gives the camera its range from where the origin lands on the
/// screen.
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *scene = Ram_HeapSlots->cameraWork;
  s32 *pos = scene->focus;
  FIXED scale = (zoom << 16) / 100;
  FIXED renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  FIXED span = BATTLE_CAMERA_DISTANCE;
  FIXED alpha;
  FIXED half;
  FIXED width;
  u32 result;

  pos[0] = x;
  pos[1] = depth;
  pos[2] = y;
  sourceBounds[0] = 0;
  alpha = FIXED_ONE * 3 / 4;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Matrix_Initialize();
  Matrix_TranslateVector(pos);
  Matrix_RotateY((s16)scene->yaw);
  Matrix_RotateX((s16)scene->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = span;
  Iwram_TransformVector(renderBounds, (s32 *)scene);
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Matrix_Initialize();
  Matrix_SetLookAt(scene->eye, pos);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(
      toFIXED(BATTLE_PROJECTION_CENTER_X), toFIXED(BATTLE_PROJECTION_CENTER_Y),
      (BATTLE_PROJECTION_CENTER_X - measuredBounds[0]) << 8,
      (BATTLE_PROJECTION_CENTER_Y - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

// Battle presentation: center the camera between two objects.

/// Centres the camera on two objects' midpoint in x and z.
/// a, b: object IDs.
/// zoom: scale in percent.
void BattleCamera_SetViewAtPairMidpoint(s32 a, s32 b, s32 zoom) {
  struct MotionObject *obj_a = BattleObject_Get(a)->object;
  struct MotionObject *obj_b = BattleObject_Get(b)->object;
  s32 ax = obj_a->x;
  s32 bx = obj_b->x;
  s32 az = obj_a->z;
  s32 bz = obj_b->z;
  s32 x = (bx + ax) / 2;
  s32 z = (bz + az) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames, its blend weight falling
/// by 2 a frame from 15, while the backdrop's stays 16. Leaves REG_BLDCNT
/// blending BG2 over the backdrop.
void BattleBackground_FadeAlphaOut(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
    // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
    // code.
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
  // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
  // code.
  for (step = 1; step <= BLEND_WEIGHT_MAX; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    wait(1);
  }
}

/// Unused: sets BLDCNT to brighten every layer and the backdrop.
UNUSED void Battle_BrightenAllLayers(void) {
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

/// Returns the 16.16 product of a and b.
FIXED Battle_MulFX(FIXED a, FIXED b) { return Iwram_MulFX(a, b); }

/// A transform as BattleTransition_InitializeEntries reads its first two words.
struct BattleTransitionEntry {
  u32 value;
  u32 sum;
  u32 field8;
  u32 fieldc;
};

/// Unused: sets entries to the identity transform, then stores the first word's
/// old value plus its new one in the second.
void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register unit matrix store of matrix.h")
  Matrix_SetUnit(entries);
  entries[0].sum = previous + entries[0].value;
}

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_RunDrainMotes(void *effect, s32 mode);
s32 MapGrid_GetAttributeBAt(s32 x, s32 z, s32 layer);
u8 MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value);
void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

/// Unused: returns value divided by 16.
s32 BattleBackground_DivideBy16(s32 value) { return value / 16; }

// What Select picks: another value in the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DEBUG_VALUE_SELECT 411
#else
#define DEBUG_VALUE_SELECT 544
#endif
/// What R picks, in every release.
#define DEBUG_VALUE_R 399

/// A debug choice of a battle value: with Start held, the d-pad and L/R step
/// the battle work's debugValue (by 1, 10 and 100) until A takes it; Select
/// held picks DEBUG_VALUE_SELECT and R held DEBUG_VALUE_R; otherwise value
/// comes back unchanged.
s32 Battle_AdjustDebugValue(s32 value) {
  if (gInput.held & KEY_START) {
    while (1) {
      struct BattleWork *work = Ram_HeapSlots->battleWork;

      if (gInput.repeat & KEY_LEFT)
        work->debugValue--;
      if (gInput.repeat & KEY_RIGHT)
        work->debugValue++;
      if (gInput.repeat & KEY_L)
        work->debugValue -= 100;
      if (gInput.repeat & KEY_R)
        work->debugValue += 100;
      if (gInput.repeat & KEY_UP)
        work->debugValue -= 10;
      if (gInput.repeat & KEY_DOWN)
        work->debugValue += 10;
      if (gInput.pressed & KEY_A) {
        value = work->debugValue;
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

// Battle presentation: toggle translucency on unit graphics.

/// A part of a unit's sprite (its image, or its shadow at 0x1c) as the
/// highlight reads its OBJ mode.
struct ActorSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u8 unk06[0x16];
};

/// A unit's sprite as the highlight reads it: its image and its shadow.
struct ActorSpriteRecord {
  struct ActorSpritePart part[2];
};

/// Sets the OBJ mode of both sprite parts of each of the actor's records.
void BattleHighlight_SetSpriteMode(s32 actorId, s32 mode) {
  struct BattleObject *slot = BattleObject_Get(actorId);
  struct MotionObject *object;
  struct ActorSpriteRecord *record;
  struct ActorSpriteRecord **records;
  s32 i;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->recordStorageKind & ACTOR_SPRITE_KIND_MASK) {
  case ACTOR_SPRITE_ONE: {
    record = object->records;
    record->part[0].objectMode = mode;
    record->part[1].objectMode = mode;
    break;
  }
  case ACTOR_SPRITE_GROUP: {
    records = object->records;
    i = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->part[0].objectMode = mode;
      record->part[1].objectMode = mode;
      i++;
    } while (i <= 3);
    break;
  }
  }
}

/// Makes every unit's graphics but except's translucent (OBJ mode 1).
/// except: unit ID to leave unchanged.
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

/// Restores normal rendering (OBJ mode 0) for all units.
void BattleHighlight_ClearAllSpriteModes(void) {
  s16 unitIds[UNIT_MAX];
  s32 unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
    BattleHighlight_SetSpriteMode(unitIds[unitIndex], SPRITE_OPAQUE);
}

// Steps the battle highlight's blend.

extern s8 gBattleHighlight_BlendRamp[];

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
          ((battle->highlightStep + 1) & 0xf) |
          (battle->highlightStep > 14 ? BLEND_WEIGHT_MAX : 0);
    } else if (battle->highlightMode != HIGHLIGHT_OFF) {
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
      REG_BLDY = gBattleHighlight_BlendRamp[battle->highlightStep];
      battle->highlightStep =
          ((battle->highlightStep + 1) & 0xf) |
          (battle->highlightStep > 14 ? BLEND_WEIGHT_MAX : 0);
    }
  }
}

/// Highlights the units in actors (up to 14, ended by BATTLE_UNIT_LIST_END),
/// the others taking the other blend, or with HIGHLIGHT_OFF stops the
/// highlight.
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
          if (i > 13)
            break;
          actor = *actors++;
        } while (actor != BATTLE_UNIT_LIST_END);
      }
    }
    wait(1);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
    Task_Add(BattleBackground_StepHighlightBlend, TASK_PASS_FRAME);
  }
}

/// A spark of the star field behind a summon: its position and velocity, 16.16,
/// and the frames it has left.
struct Spark {
  FIXED pos[6]; // x, y, z and their velocities, 16.16
  s32 life;
};

/// A ring of the star field leaving its centre: its place, its speed and its
/// age.
struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
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

/// The star field's work: the canvas sheet, sixteen sparks, three rings, the
/// frames counted, whether the canvas is ready, and its centre.
struct SparkWork {
  u8 sheet[4544];
  struct Spark sparks[STAR_FIELD_SPARKS];
  struct Ring rings[STAR_FIELD_RINGS];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// The star tables: each picture's size and sheet offset, and the ring's four
// pictures.
extern const u8 gElementGlow_StarSizes[];
extern const s32 gElementGlow_StarPictures[];
extern const s32 gElementGlow_RingPictures[];

/// Steps the star field behind a summon one frame: its sparks spiral in
/// towards the centre along a damped spiral and shrink as they near it; while
/// the field is young (24 frames) a spark that dies is born again on a random
/// ring. Then three rings drift outward through their four frames. The canvas
/// is marked ready for the transfer at the end.
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
      dist = Math_SqrtFX((spark->pos[0] >> 8) * (spark->pos[0] >> 8) +
                         (spark->pos[1] >> 8) * (spark->pos[1] >> 8) +
                         (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
      if (dist <= 0xfff) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(dist, FIXED_ONE);
        spark->life--;
        mul = Iwram_MulFX;
        pos = spark->pos;
        for (k = 2; k >= 0; k--) {
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
      spark->pos[0] = Iwram_MulFX(Trig_Cos(angle), half);
      spark->pos[1] = Iwram_MulFX(Trig_Sin(angle), half);
      if (spark->pos[0] & 1)
        spark->pos[0] = -spark->pos[0];
      if (spark->pos[1] & 1)
        spark->pos[1] = -spark->pos[1];
      spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
      spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
      spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
      spark->pos[5] = 0;
      life = spark->life = (radius >> 13) + 1;
    }
    if (life != 0) {
      x = (spark->pos[0] >> 10) + STAR_FIELD_CENTER;
      y = (spark->pos[1] >> 10) + STAR_FIELD_CENTER;
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
    ring->x += ring->velX;
    ring->y += ring->velY;
    ringX = ring->x >> 10;
    ringY = ring->y >> 10;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      /* FAKEMATCH(dead): an empty once-only block after the age's store; as
       * the store alone, 2 of 267 instructions differ in tla-en. */
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

/// Copies the star field's canvas into BG character block 1 at three quarters
/// of its brightness once the field marks it ready.
s32 ElementGlow_UploadStarField(void) {
  void *source = Ram_HeapSlots->battleCanvas;
  u8 *context = Ram_HeapSlots->battleEffect;

  if (source != 0) {
    u32 *active = (u32 *)(context + 0x13c0);

    if (*active != 0) {
      *active = 0;
      return ColorBuffer_ScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(1),
                                            FIXED_ONE / 4);
    }
  }
  return (s32)source;
}

s32 BattleEffect_LoadBlitterFar(s32 slot, s32 modeAndFlags);

/// Returns the x of the point at angle and radius from the origin.
static __inline__ s32 PolarX(s32 angle, FIXED radius) {
  return Iwram_MulFX(Trig_Cos(angle), radius);
}

/// Returns the y of the point at angle and radius from the origin.
static __inline__ s32 PolarY(s32 angle, FIXED radius) {
  return Iwram_MulFX(Trig_Sin(angle), radius);
}

/// Sets up the star field behind a summon: sixteen sparks on random rings
/// around the centre, three rings leaving it at thirds of a turn, the blue ring
/// sheet and its palette with the mode's colour (0 yellow, 1 blue, 2 red, else
/// violet) over the first 32 colours, BG2 unscaled at the origin, the additive
/// and max blitters (BattleEffect_LoadBlitterFar, the veneer of
/// BattleEffect_LoadBlitter), and the two frame callbacks.
s32 ElementGlow_InitializeStarField(s32 mode) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 i;
  u8 *data;

  ((s32 *)Ram_HeapSlots->battleBackground)[2] = BATTLE_BACKGROUND_FIXED;
  work = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  for (i = 0; i < STAR_FIELD_SPARKS; i++) {
    s32 angle = Random16();
    u32 radius = Random16() + FIXED_ONE;
    u32 half = radius >> 1;

    spark = &work->sparks[i];
    spark->pos[0] = Iwram_MulFX(Trig_Cos(angle), half);
    spark->pos[1] = Iwram_MulFX(Trig_Sin(angle), half);
    if (spark->pos[0] & 1)
      spark->pos[0] = -spark->pos[0];
    if (spark->pos[1] & 1)
      spark->pos[1] = -spark->pos[1];
    spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
    spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
    spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
    spark->pos[5] = 0;
    spark->life = (radius >> 13) + 1;
  }
  for (i = 0; i < STAR_FIELD_RINGS; i++) {
    FIXED radius = 0x1000;
    s32 angle;

    ring = &work->rings[i];
    angle = i * (DEGtoANG(120));
    ring->x = PolarX(angle, radius);
    ring->y = PolarY(angle, radius);
    ring->velX = PolarX(angle, 0x200);
    ring->velY = PolarY(angle, 0x200);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->originPending = 0;
  Iwram_ClearWordsEntry(Ram_HeapSlots->battleCanvas,
                        STAR_FIELD_SIZE * STAR_FIELD_SIZE);
  data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
  Iwram_CopyWordsEntry((void *)BG_PLTT, data, 128);
  data += 128;
  Decompress_TaggedLz(data, work);
  switch (mode) {
  case ELEMENT_VENUS:
    data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
    break;
  case ELEMENT_MERCURY:
    data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    break;
  case ELEMENT_MARS:
    data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
    break;
  default:
    data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
    break;
  }
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(32), REG_DMA3);
  Io_Write32(0, &REG_BG2X);
  Io_Write32(0, &REG_BG2Y);
  Io_Write16(Q_8_8(1), &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(Q_8_8(1), &REG_BG2PD);
  /* LEFTOVER(frame): the twin's five-argument BattleEffect_BuildBlitterFar,
   * never taken here: of the 8 bytes the ROM's frame reserves (sub sp, #8),
   * no instruction addresses sp+0 to sp+3, the call's fifth argument; as the
   * call left out, 10 of 255 instructions differ in tla-en. */
  if (0)
    BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 7, 7, 3, 3);
  BattleEffect_LoadBlitterFar(HEAP_SLOT_RAM_CODE, 0x03);
  BattleEffect_LoadBlitterFar(HEAP_SLOT_RAM_CODE_PAIRED, 0x13);
  Task_Add(ElementGlow_UpdateStarField, TASK_PASS_VBLANK);
  return Task_Add(ElementGlow_UploadStarField, TASK_PASS_VBLANK);
}

/// Ends a star field: clears the tiles in character block 1, releases its
/// four heap slots, turns BG2 off and removes both its tasks. Called by
/// ElementGlow_Play once mode 0's field has played. Returns Task_Remove's
/// result, which the caller does not read.
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

/// Does nothing; nothing calls it.
void ElementGlow_ReservedNoOp(void) {}

s32 BattleMotion_ProjectScaledPosition(s32 slot, s32 *screen);
void BattleEffect_RunElementBurstFar(struct BattlePresentationWork *launch);
void BattleEffect_StartElementStreaksFar(s32 mode);
void BattleEffect_EndElementStreaksFar(void);

/// Plays a unit's element glow in one of four ways, and returns when it is
/// over: 0, the star field around the unit while the backdrop darkens over 25
/// frames (45 frames); 1, the prepared canvas scene of the kind, centred on the
/// unit (40 frames); 2, the ice fleck burst; 3, the gathering burst.
void ElementGlow_Play(s32 unit, s32 kind, s32 mode, s32 variant) {
  struct ScreenPoint starPosition;
  struct ScreenPoint streakPosition;
  struct BattlePresentationWork pillarLaunch;
  struct BattlePresentationWork burstLaunch;
  s32 origin[2];
  struct BattleWork *session = Ram_HeapSlots->battleWork;
  volatile u16 *ime;
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct ScreenPoint *pos;

  wait(1);
  BattleBackground_SetMode(BATTLE_BACKGROUND_FIXED, session->background, 0);
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
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, BLEND_ALPHA(14, BLEND_WEIGHT_MAX));
    ElementGlow_InitializeStarField(kind);
    for (frame = 0, pos = &starPosition; frame <= 44; frame++) {
      brightness = &session->brightness;
      ime = &REG_IME;
      work = (struct SparkWork *)Ram_HeapSlots->battleEffect;
      if (frame <= 24)
        ColorBuffer_Scale(session->palette, BATTLE_BG_PALETTE,
                          *brightness =
                              FIXED_ONE - frame * BACKGROUND_LEVEL_STEP,
                          BATTLE_BG_PALETTE_COLORS);
      BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
      work->originX = (STAR_FIELD_CENTER - pos->x) << 8;
      work->originY = (STAR_FIELD_CENTER - pos->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, (u32)&work->originX,
                              DMA_COPY_WORDS(2));
      work->originPending = 1;
      wait(1);
    }
    ElementGlow_End(kind);
  } else if (mode == GLOW_STREAKS) {
    BattleEffect_StartElementStreaksFar(kind);
    for (frame = 0; frame < 40; frame++) {
      BattleMotion_ProjectScaledPosition(unit, (s32 *)&streakPosition);
      origin[0] = (STAR_FIELD_CENTER - streakPosition.x) << 8;
      origin[1] = (STAR_FIELD_CENTER - streakPosition.y) << 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, (u32)origin, DMA_COPY_WORDS(2));
      wait(1);
    }
    BattleEffect_EndElementStreaksFar();
  } else if (mode == GLOW_PILLAR) {
    pillarLaunch.flags = 0;
    pillarLaunch.kind = kind;
    pillarLaunch.variant = variant;
    pillarLaunch.actor = unit;
    pillarLaunch.actors[0] = unit;
    pillarLaunch.firstTarget = unit;
    pillarLaunch.count = 1;
    pillarLaunch.initialValue = 1;
    BattleEffect_RunElementPillarFar(&pillarLaunch);
  } else {
    burstLaunch.flags = 0;
    burstLaunch.kind = kind;
    burstLaunch.variant = 0;
    burstLaunch.actor = unit;
    burstLaunch.firstTarget = unit;
    burstLaunch.actors[0] = unit;
    burstLaunch.count = 1;
    burstLaunch.initialValue = 1;
    BattleEffect_RunElementBurstFar(&burstLaunch);
  }
}

/// Puts the background's palette back undarkened, at level 0.
/// Called by BattleStaging_HideField and BattleStaging_RunPsynergyCast.
void BattleBackground_RestorePalette(void) {
  BattleBackground_SetPaletteLevel(0, 0);
}

/// Removes the VCount interrupt's handler; nothing calls it.
void Irq_RemoveVCountHandler(void) { Irq_SetHandler(IRQ_VCOUNT, 0, 0); }

/// The Broken Seal's routine without its level check:
/// the average level of a formation's enemy kinds, counting only kinds the
/// player has met (flag 0x608 + id, or all with flag 0x174); -1 for an empty
/// formation, -2 when one present kind is unmet, -3 when none counts.
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
