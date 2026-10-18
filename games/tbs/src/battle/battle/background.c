// Part of battle.c, the battle object, compiled only through it.
// The battle background, its palette and perspective, the camera's zoom and
// the units' highlights.

#include "../battle_internal.h"

// The value Select picks in the debug value editor
// (Runtime_AdjustDebugValueWithButtons), which the international editions
// changed.
#if TBS_PATCH_1
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

// Uploads the saved battle palette, scaling its brightness unless the level is
// zero. Preserves the interrupt-enable state around the update.
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level) {
  struct BattleWork *battle = gBattleWork;
  u16 *palette = battle->palette;
  u32 ime;
  u32 *savedInterruptEnable = &ime;

  /* FAKEMATCH(one-pass): the IME save in a one-pass block and a loop on a zero
   * local, which keep it in its pointer's register; as a plain block, 4 of 43
   * instructions differ in tbs-en. */
  do {
    u16 *interruptEnable = (u16 *)0x04000208;
    s32 once = 0;

    do {
      *savedInterruptEnable = *interruptEnable;
      *interruptEnable = (u32)interruptEnable;
    } while (once);
  } while (0);

  if (level == 0) {
    Dma_Set(palette, BATTLE_BG_PALETTE, DMA_COPY_HALFWORDS(128), REG_DMA3);
  } else {
    Graphics_ScaleRgb555Clamped(palette, BATTLE_BG_PALETTE,
                                battle->brightness = 0x10000 - level * 1092,
                                128);
  }
  {
    u32 saved = *savedInterruptEnable;

    REG_IME = saved;
  }
}

// Selects the background mode and restores or darkens its palette, then
// rebuilds the background tile lookup and map. unusedResource is not read.
void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 unusedResource,
                                             s32 fade) {
  struct BattleBackgroundView *transition =
      (struct BattleBackgroundView *)gTransitionWork;

  if (transition->mode == 0) {
    Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
  }
  transition->mode = mode;

  if (mode == 1) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, BATTLE_BG1CNT, 0x20000);
  }

  Dma_Set((void *)OBJ_PLTT, &BG_PLTT_COLOR(5, 0), DMA_COPY_HALFWORDS(16),
          REG_DMA3);
  *(u16 *)&BG_PLTT_COLOR(5, 14) = *(u16 *)&BG_PLTT_COLOR(15, 4);

  if (fade == 0x80) {
    Dma_Set(gBattleWork->palette, BATTLE_BG_PALETTE, DMA_COPY_HALFWORDS(128),
            REG_DMA3);
  } else if (fade != 0) {
    u16 *source = gBattleWork->palette;
    u16 *destination = BATTLE_BG_PALETTE;
    s32 colorIndex;

    for (colorIndex = 0; colorIndex != 128; colorIndex++) {
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
      destination[colorIndex] = (blue << 10) | (green << 5) | red;
    }
  }

  Graphics_BuildSequentialTileTable(BG_SCREEN_BLOCK(7));
  BattlePresentation_BuildTilemap(BG_SCREEN_BLOCK(31));
}

// Allocates and clears the affine scanline workspace and resets background
// mode.
void BattlePresentation_InitializeWorkAndResetState(void) {
  struct BattleAffineHdma *work;
  struct BattleBackgroundView *state;
  work = Runtime_AllocateBlock(10, sizeof *work);
  state = (struct BattleBackgroundView *)gTransitionWork;
  Dma_Fill32(0, work, sizeof *work);
  state->mode = 0;
}

// Releases the battle background's affine scanline workspace.
void Runtime_ReleaseHeapBlock10(void) { Runtime_ReleaseHeapBlock(10); }

typedef void (*BitDecoder)(const u8 *source, void *destination);

// Decodes the background image, saves and uploads its palette, rebuilds its
// tilemap and enables scanline updates for the requested display mode.
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level) {
  struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  u8 *backgroundData = Resource_GetTableEntry(resourceId);
  struct BattleWork *battle = WORK_SLOT(HEAP_SLOT_BATTLE);
  u16 *palette;

  RamCode_LoadSlot(49, BitDecoder_DecodeImage, BitDecoder_Size);
  {
    u32 offset = 0x100;
    void *vram;

    /* FAKEMATCH(dead): two updates that cancel, under a test that emits
     * nothing, end a CSE block so the VRAM address loads before the data's
     * add; without them, 2 of 103 instructions differ in tbs-en. */
    if (battle) {
      offset++;
      offset--;
    }
    vram = BG_CHAR_BLOCK(2);
    ((BitDecoder)WORK_SLOT(49))(backgroundData + offset, vram);
  }
  Runtime_ReleaseHeapBlock(49);
  palette = battle->palette;
  Dma_Set(backgroundData, palette, DMA_COPY_WORDS(64), REG_DMA3);
  if (level >= 0)
    Graphics_ScaleRgb555Clamped(palette, BATTLE_BG_PALETTE,
                                battle->brightness = 0x10000 - level * 1092,
                                128);
  Dma_Set((void *)OBJ_PLTT, &BG_PLTT_COLOR(5, 0), DMA_COPY_HALFWORDS(16),
          REG_DMA3);
  *(u16 *)&BG_PLTT_COLOR(5, 14) = *(u16 *)&BG_PLTT_COLOR(15, 4);
  Graphics_BuildSequentialTileTable(BG_SCREEN_BLOCK(7));
  BattlePresentation_BuildTilemap(BG_SCREEN_BLOCK(31));
  Iwram_ClearWords((BG_SCREEN_BLOCK(31) + 0x7c0), 64);
  if (view->mode == 0)
    Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
  view->mode = mode;
  if (mode == 1)
    REG_BG1CNT = BATTLE_BG1CNT;
}

// Builds the next affine background page from the view centre, offsets and
// zoom, dividing scanlines into sky, ground and hidden rows before swapping
// pages.
void BattleCamera_SetRange(s32 centerX, s32 centerY, s32 offsetX, s32 offsetY,
                           s32 scale) {
  struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleAffineHdma *scanlines = WORK_SLOT(10);
  struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  u16 wrap;
  s32 horizon;
  u16 *line;
  struct BattleAffine *affine;
  s32 ratio;
  s32 step;
  s32 x;
  s32 y;
  u32 scanline;
  u32 groundEndLine;

  horizon = 0x800;
  wrap = 0;
  if (scale >= 0x10000) {
    wrap = 0x2000;
    horizon = 0x6800 + -(s16)camera->yaw * 3;
  }
  if (scanlines == NULL)
    return;
  if ((view->mode == 1 || view->secondMode == 1) && view->busy == 0)
    gBgScroll[1].x = horizon >> 8;
  if (view->mode != 2)
    return;
  line = scanlines->lines[scanlines->page ^ 1];
  ratio = Iwram_RatioMulQ14(scale, 0x10000);
  step = ratio >> 8;
  affine = &scanlines->affine;
  affine->pa = step;
  affine->pb = 0;
  affine->pc = 0;
  affine->pd = step;
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - 0x10000));
  affine->x = ((x + 0x7fff) >> 8) + offsetX + horizon;
  y = ((y + 0x7fff) >> 8) + offsetY - 0x1000;
  affine->y = y;
  groundEndLine = (Iwram_RatioMulQ14((s16)step, 0x4000 - y) >> 16) + 1;
  scanline = 0;
  if (!GameFlag_TestFar(0x16b)) {
    for (; scanline < 16; scanline++)
      *line++ = 0x3f8e;
  }
  if (groundEndLine > 136)
    groundEndLine = 136;
  for (; scanline < groundEndLine; scanline++)
    *line++ = wrap | 0x478a;
  for (; scanline < 136; scanline++)
    *line++ = wrap | 0x478e;
  for (; scanline < 160; scanline++)
    *line++ = 0x3f8e;
  scanlines->page ^= 1;
}

// Unused depth-based camera setup. Its original zoom local is uninitialized;
// no current caller establishes the missing zoom contract.
UNUSED void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y) {
  s32 span = 0x01fe0000;
  // BUG: zoom is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
  s32 uninitializedZoom;
  struct BattleCamera *camera = gCameraWork;
  s32 *focus = camera->pos;
  s32 scale = (uninitializedZoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  SET_VECTOR(focus, x, depth, y);
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(focus);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = camera->distance;
  Iwram_TransformVector(renderBounds, (s32 *)camera);
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, focus);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

// Sets the camera focus and zoom, projects the origin to align the background,
// then installs the projection bounds scaled by the zoom percentage.
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *camera = gCameraWork;
  s32 *focus = camera->pos;
  s32 scale = (zoom << 16) / 100;
  s32 renderBounds[3];
  s32 measuredBounds[3];
  s32 sourceBounds[3];
  s32 span = 0x01fe0000;
  s32 alpha;
  s32 half;
  s32 width;
  u32 result;

  SET_VECTOR(focus, x, depth, y);
  sourceBounds[0] = 0;
  alpha = 0xc000;
  sourceBounds[1] = 0;
  sourceBounds[2] = 0;

  result = Iwram_RatioMulQ14(span, alpha);
  Projection_Set(span, result, span * 2);
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(focus);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);
  renderBounds[0] = 0;
  renderBounds[1] = 0;
  renderBounds[2] = span;
  Iwram_TransformVector(renderBounds, (s32 *)camera);
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Render_ResetTransformState();
  Transform_SetLookAt(camera->eye, focus);
  Render_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

// Centers the camera at the two objects' midpoint in x and z.
// zoom: scale percentage.
void BattlePres_SetupTransitionAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                              s32 zoom) {
  struct MotionObject *firstObject = BattleObjGet(firstUnitId)->object;
  struct MotionObject *secondObject = BattleObjGet(secondUnitId)->object;
  s32 firstX = firstObject->x;
  s32 secondX = secondObject->x;
  s32 firstZ = firstObject->z;
  s32 secondZ = secondObject->z;
  s32 x = (secondX + firstX) / 2;
  s32 z = (secondZ + firstZ) / 2;

  BattlePres_SetupTransitionScene(x, 0, z, zoom);
}

// Fades the selected blend target out in eight two-step alpha updates.
void Graphics_FadeAlphaOut(void) {
  s32 alphaStep;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (alphaStep = 1; alphaStep <= 16; alphaStep += 2) {
    REG_BLDALPHA = BLEND_ALPHA(16, 16) - alphaStep;
    WaitFrames(1);
  }
}

// Fades the selected blend target in in eight two-step alpha updates.
void Graphics_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, 16);
    WaitFrames(1);
  }
}

struct BattleTransitionEntry {
  u32 value;
  u32 sum;
  u32 field8;
  u32 fieldc;
};

// Selects the battle overlay's blend targets.
void Graphics_SetBlendControl(void) {
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

// Resets the transform and adds the previous first value to the new first
// value.
void BattlePresentation_InitializeTransitionEntries(
    struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}

// Divides a signed value by sixteen, rounding toward zero.
s32 BattlePres_DivideBy16(s32 value) { return value / 16; }

// While Start is held, edits the debug value with the d-pad and accepts it with
// A. Select overrides the result with the edition's fixed debug selection.
s32 Runtime_AdjustDebugValueWithButtons(s32 selectedValue) {
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
        WaitFrames(1);
        continue;
      }
      break;
    }
  }
  if (gKeysHeld & KEY_SELECT)
    selectedValue = DEBUG_SELECT_VALUE;
  return selectedValue;
}

// Sets both sprite parts of each available animation record to the requested
// OBJ mode.
void BattlePres_SetActorRecordMode(s32 actorId, s32 mode) {
  struct BattleObj *slot = BattleObjGet(actorId);
  struct MotionObject *object;
  struct AnimationObject *record;
  struct AnimationObject **records;
  s32 recordIndex;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->recordStorageKind & 15) {
  case 1: {
    record = object->records;
    record->image.objectMode = mode;
    record->shadow.objectMode = mode;
    break;
  }
  case 2: {
    records = object->records;
    recordIndex = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->image.objectMode = mode;
      record->shadow.objectMode = mode;
      recordIndex++;
    } while (recordIndex <= 3);
    break;
  }
  }
}

// Makes every unit sprite except the selected one translucent (OBJ mode one).
// selectedUnitId: the unit left as it is.
void BattlePres_SetOtherActorRecordModes(s32 selectedUnitId) {
  s16 unitIds[14];
  s32 unitCount = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (unitIds[unitIndex] != selectedUnitId)
      BattlePres_SetActorRecordMode(unitIds[unitIndex], 1);
  }
}

// Restores every unit sprite to normal rendering (OBJ mode zero).
void BattlePres_ClearAllActorRecordModes(void) {
  s16 unitIds[14];
  s32 unitCount = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
    BattlePres_SetActorRecordMode(unitIds[unitIndex], 0);
}

// Advances the sixteen-step highlight brightness ramp while highlighting is
// enabled.
void Graphics_AdvancePaletteCycle(void) {
  struct BattleWork *battle = gBattleWork;
  s8 *blendRamp;
  u16 rampIndex;
  s32 nextIndex;

  if (battle != NULL && battle->highlightMode != 0) {
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = BLEND_ALPHA(16, 0);
    blendRamp = gBattleFx_HighlightBlendRamp;
    REG_BLDY = blendRamp[battle->highlightStep];
    rampIndex = battle->highlightStep;
    nextIndex = (rampIndex + 1) & 0xf;
    if (rampIndex > 14)
      nextIndex |= 0x10;
    battle->highlightStep = nextIndex;
  }
}

// Starts or stops the palette highlight callback, applies the selected OBJ mode
// to every unit and the opposite mode to the explicitly selected units.
void BattlePres_SetActorModes(u16 *selectedUnits, s32 mode) {
  s16 unitIds[14];
  struct BattleWork *battle = gBattleWork;
  u32 unitCount;
  u32 unitIndex;

  if (mode == 0) {
    Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
    REG_BLDY = mode;
    BattlePres_ClearAllActorRecordModes();
    WaitFrames(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
  }
  if (battle != 0 && mode != 0) {
    battle->highlightMode = mode;
    battle->highlightStep = 0;
    REG_BLDY = 0;
    Io_Write16(BLEND_ALPHA(16, 0), &REG_BLDALPHA);

    unitCount = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
    for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
      BattlePres_SetActorRecordMode(unitIds[unitIndex], mode & 1);

    if (selectedUnits != 0) {
      u32 unitId = *selectedUnits;
      unitIndex = 0;
      selectedUnits++;
      if (unitId != BATTLE_UNIT_LIST_END) {
        do {
          BattlePres_SetActorRecordMode(unitId, (mode & 1) ^ 1);
          unitIndex++;
          if (unitIndex > 13)
            break;
          unitId = *selectedUnits++;
        } while (unitId != BATTLE_UNIT_LIST_END);
      }
    }
    WaitFrames(1);
    QueueIoWriteDelay2((u32)&REG_BLDCNT, 0);
    Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0x480);
  }
}
