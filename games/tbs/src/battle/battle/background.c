/// @file
/// The battle background, its palette and modes, the camera's zoom and
/// the units' highlights.
// Part of battle.c.

#include "../battle_internal.h"

// The value Select picks in the debug value editor
// (Battle_AdjustDebugValue), which the international editions
// changed.
#if TBS_PATCH_1
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

/// BattleBackground_SetPaletteLevel's and BattleBackground_Load's darkening
/// step: each level takes 1092 from the 16.16 brightness, so level 60 is
/// almost black.
#define BACKGROUND_LEVEL_STEP 1092

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

/// Copies the background's saved palette to BG palettes 6 to 13, darkened by
/// level (0 for none).
/// Called by BattleBackground_RestorePalette with 0, and by battle animations
/// that dim the background (beams.c, canvas.c). Sets BattleWork.brightness when
/// it darkens. Interrupts are off while it writes: REG_IME is given its own
/// address, whose bit 0 is clear, as irqlock.h's IME_DISABLE does, and is
/// restored after.
void BattleBackground_SetPaletteLevel(s32 unused, s32 level) {
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
    Graphics_ScaleRgb555Clamped(
        palette, BATTLE_BG_PALETTE,
        battle->brightness = Q16_ONE - level * BACKGROUND_LEVEL_STEP, 128);
  }
  {
    u32 saved = *savedInterruptEnable;

    REG_IME = saved;
  }
}

/// Switches the background's display mode, optionally sets its palette, and
/// rebuilds both its maps.
/// fade 0x80 restores the saved palette, another nonzero fade takes that
/// much from each colour channel, and 0 leaves the palette alone;
/// BATTLE_BACKGROUND_FIXED queues BATTLE_BG1CNT for BG1. Called with
/// BATTLE_BACKGROUND_FIXED by ElementGlow_Play and the battle animations'
/// canvas layers (canvas.c), and with BATTLE_BACKGROUND_ZOOMED by
/// BattleStaging_DispatchAction and Battle_ProcessRoundEnd. Sets
/// BattleBackgroundWork.mode (enum BattleBackgroundMode), adds
/// BattleBackground_ShowZoomedPage when it was BATTLE_BACKGROUND_NONE,
/// and copies the first sprite palette to BG palette 5. unusedResource is not
/// read.
void BattleBackground_SetMode(s32 mode, s32 unusedResource, s32 fade) {
  struct BattleBackgroundWork *transition = gBattleBackgroundWork;

  if (transition->mode == BATTLE_BACKGROUND_NONE) {
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK + 127);
  }
  transition->mode = mode;

  if (mode == BATTLE_BACKGROUND_FIXED) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, BATTLE_BG1CNT, IO_WRITE_16);
  }

  // BASIC_COLORS_PALETTE is the battle windows' second text palette
  // (WindowWork.textPalette): the party status window's HP and PP, the
  // Psynergy list's names and costs and the element icons are drawn in the
  // first sprite palette's colours through it. Text in a palette other than
  // the UI's is drawn over TEXT_BACKGROUND_COLOR (Text_DrawStringAsTiles), so
  // it takes WINDOW_BACKGROUND_COLOR.
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  *(u16 *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      *(u16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);

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
  BattleTransition_BuildTilemap(BG_SCREEN_BLOCK(31));
}

/// Allocates the zoomed background's scanline work
/// (HEAP_SLOT_BATTLE_SCANLINES) cleared, and sets the background's mode to
/// BATTLE_BACKGROUND_NONE, so the next load adds its scanline callback.
/// Called once by Battle_Run before BattleBackground_Load.
void BattleBackground_Init(void) {
  struct BattleAffineHdma *work;
  struct BattleBackgroundWork *state;
  work = AllocHeapSlotEwram(HEAP_SLOT_BATTLE_SCANLINES, sizeof *work);
  state = gBattleBackgroundWork;
  Dma_Fill32(0, work, sizeof *work);
  state->mode = BATTLE_BACKGROUND_NONE;
}

/// Frees the zoomed background's scanline work; called by
/// Battle_Run as the battle ends.
void BattleBackground_Free(void) { FreeHeapSlot(HEAP_SLOT_BATTLE_SCANLINES); }

/// The bit decoder BattleBackground_Load copies to RAM and runs.
typedef void (*BitDecoder)(const u8 *source, void *destination);

/// Loads a battle background: its picture into character blocks 2 and 3, its
/// palette into BattleWork.palette and, darkened by level, BG palettes 6 to
/// 13, and both its maps.
/// A negative level leaves the BG palettes alone. The resource is 256 bytes
/// of palette and then the packed picture, which the RAM copy of
/// BitDecoder_DecodeImage (HEAP_SLOT_RAM_CODE_TEMP) unpacks. Called with
/// BATTLE_BACKGROUND_FIXED by
/// Battle_Run as the battle opens, and by the Atalanta and Boreas
/// animations for their violet sky. Sets BattleBackgroundWork.mode, and adds
/// BattleBackground_ShowZoomedPage when it was BATTLE_BACKGROUND_NONE.
void BattleBackground_Load(s32 mode, s32 resourceId, s32 level) {
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  u8 *backgroundData = Resource_GetTableEntry(resourceId);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  u16 *palette;

  RamCode_LoadSlot(HEAP_SLOT_RAM_CODE_TEMP, BitDecoder_DecodeImage,
                   BitDecoder_Size);
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
    ((BitDecoder)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_TEMP))(
        backgroundData + offset, vram);
  }
  FreeHeapSlot(HEAP_SLOT_RAM_CODE_TEMP);
  palette = battle->palette;
  Dma_Set(backgroundData, palette, DMA_COPY_WORDS(64), REG_DMA3);
  if (level >= 0)
    Graphics_ScaleRgb555Clamped(
        palette, BATTLE_BG_PALETTE,
        battle->brightness = Q16_ONE - level * BACKGROUND_LEVEL_STEP, 128);
  Dma_Set((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  *(u16 *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      *(u16 *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  Graphics_BuildSequentialTileTable(BG_SCREEN_BLOCK(7));
  BattleTransition_BuildTilemap(BG_SCREEN_BLOCK(31));
  // Clears BG1's last row, 0x7c0 bytes in, which BuildTilemap leaves.
  Iwram_ClearWords((BG_SCREEN_BLOCK(31) + 0x7c0), 64);
  if (view->mode == BATTLE_BACKGROUND_NONE)
    Task_Add(BattleBackground_ShowZoomedPage, TASK_PASS_VBLANK + 127);
  view->mode = mode;
  if (mode == BATTLE_BACKGROUND_FIXED)
    REG_BG1CNT = BATTLE_BG1CNT;
}

/// Builds the zoomed background's next page for a zoom of scale about centerX
/// and centerY (all 16.16), shifted by offsetX and offsetY (8.8 pixels), and
/// shows it.
/// The page holds the affine parameters and each line's BG2CNT: which half of
/// the picture a line shows, or none. At a zoom of 1 or more the plane wraps
/// and turns sideways with the camera's yaw. While the background is
/// BATTLE_BACKGROUND_FIXED (or secondMode is) and not busy, it also sets BG1's
/// horizontal scroll. Called by BattleCamera_SetView, by
/// BattleCamera_Update and the shoulder-key camera
/// (DEFINE_ADJUST_CAMERA) while BattleBackgroundWork.flag is 0, by
/// Battle_Run and by BattleCamera_SetCommandView. Does nothing before
/// HEAP_SLOT_BATTLE_SCANLINES is allocated, and builds no page unless the
/// mode is BATTLE_BACKGROUND_ZOOMED.
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
  panX = 0x800;
  wrap = 0;
  if (scale >= Q16_ONE) {
    wrap = BGCNT_WRAP;
    panX = 0x6800 + -(s16)camera->yaw * 3;
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
  x = Iwram_MulQ16(ratio, Iwram_MulQ16(centerX, scale - 0x10000));
  y = Iwram_MulQ16(ratio, Iwram_MulQ16(centerY, scale - 0x10000));
  affine->x = ((x + 0x7fff) >> 8) + offsetX + panX;
  y = ((y + 0x7fff) >> 8) + offsetY - 0x1000;
  affine->y = y;

  // The picture's lower half starts at map row 8, 64 pixels (0x4000 in 8.8)
  // down the plane: the first screen line that reaches it at this zoom.
  lowerHalfLine = (Iwram_RatioMulQ14((s16)step, 0x4000 - y) >> 16) + 1;

  // Each line's BG2CNT. The plane covers lines 0 to 135, above the status
  // window; its top 16 lines are left off except while commands are chosen.
  scanline = 0;
  if (!GameFlag_GetFar(FLAG_CHOOSING_COMMANDS)) {
    for (; scanline < 16; scanline++)
      *line++ = BACKGROUND_PLANE_OFF;
  }
  if (lowerHalfLine > 136)
    lowerHalfLine = 136;
  for (; scanline < lowerHalfLine; scanline++)
    *line++ = wrap | BACKGROUND_PLANE_UPPER;
  for (; scanline < 136; scanline++)
    *line++ = wrap | BACKGROUND_PLANE_LOWER;
  for (; scanline < DISPLAY_HEIGHT; scanline++)
    *line++ = BACKGROUND_PLANE_OFF;
  scanlines->page ^= 1;
}

/// Sets the camera's eye at its stored distance without taking a zoom;
/// nothing in the source calls it.
/// The original zoom is an uninitialized local; UBFIX makes it an explicit
/// percentage argument, as in BattleCamera_SetView.
#ifdef UBFIX
UNUSED void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y, s32 zoom) {
#else
UNUSED void BattleCamera_SetViewAtDepth(s32 x, s32 depth, s32 y) {
#endif
  s32 span = 0x01fe0000;
  // BUG: zoom is uninitialized; reads entry r4 directly (lsls r0, r4, #16).
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
  alpha = 0xc000;
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

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

/// Points the battle camera at (x, depth, y) with a zoom in percent, and lines
/// the zoomed background up with it.
/// The camera's eye is set 510 pixels from the focus along its yaw and pitch;
/// where the battlefield's origin then projects decides the plane's offset
/// (BattleCamera_SetRange), and the projection's focal length and depth range
/// are then scaled by the zoom. Called by the battle's staging
/// (BattleTransition_Run, Battle_Run, staging.c,
/// auto_battle.c), BattleCamera_SetViewAtPairMidpoint and several
/// effects. Writes the camera's pos and eye, gProjection, and the plane's
/// next page.
void BattleCamera_SetView(s32 x, s32 depth, s32 y, s32 zoom) {
  struct BattleCamera *camera = gBattleCamera;
  s32 *focus = camera->focus;
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

  // The unzoomed projection, focal length 510 pixels, and the eye 510
  // pixels back from the focus along the camera's yaw and pitch.
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
  // Where the battlefield's origin falls about the screen point (120, 120)
  // shifts the plane by as much, in 8.8 pixels.
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  Transform_Reset();
  Transform_SetLookAt(camera->eye, focus);
  Transform_ProjectPoint(sourceBounds, measuredBounds);

  BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
                        (120 - measuredBounds[1]) << 8, scale);
  // The zoom then scales the focal length (510 pixels at 100 percent) and
  // the far depth, twice it.
  half = scale * 0xff;
  width = half * 2;
  result = Iwram_RatioMulQ14(width, alpha);
  Projection_Set(width, result, half * 4);
}

/// Points the battle camera, through BattleCamera_SetView,
/// at the ground between two units' battle objects, with a zoom in percent.
/// Called by the staging of a paired action (staging.c) and by the beam
/// animations (beams.c).
void BattleCamera_SetViewAtPairMidpoint(s32 firstUnitId, s32 secondUnitId,
                                        s32 zoom) {
  struct Object *firstObject = BattleObject_Get(firstUnitId)->object;
  struct Object *secondObject = BattleObject_Get(secondUnitId)->object;
  s32 firstX = firstObject->x;
  s32 secondX = secondObject->x;
  s32 firstZ = firstObject->z;
  s32 secondZ = secondObject->z;
  s32 x = (secondX + firstX) / 2;
  s32 z = (secondZ + firstZ) / 2;

  BattleCamera_SetView(x, 0, z, zoom);
}

/// Fades BG2 out over the backdrop in eight frames, its blend weight falling
/// by 2 a frame from 15, while the backdrop's stays 16.
/// Reached only through its entry in FarCall_BattleTable, which no code in
/// TBS names. Leaves REG_BLDCNT blending BG2 over the backdrop.
void BattleBackground_FadeAlphaOut(void) {
  s32 alphaStep;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  for (alphaStep = 1; alphaStep <= 16; alphaStep += 2) {
    // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
    // code.
    REG_BLDALPHA = BLEND_ALPHA(16, 16) - alphaStep;
    WaitFrames(1);
  }
}

/// Fades BG2 in over the backdrop in eight frames, its blend weight rising by
/// 2 a frame from 1, while the backdrop's stays 16.
/// Reached only through its entry in FarCall_BattleTable, which no code in
/// TBS names. Leaves REG_BLDCNT blending BG2 over the backdrop.
void BattleBackground_FadeAlphaIn(void) {
  s32 step;

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BD;
  // Arithmetic on BLEND_ALPHA, not the macro's | form, which changes the
  // code.
  for (step = 1; step <= 16; step += 2) {
    REG_BLDALPHA = step + BLEND_ALPHA(0, 16);
    WaitFrames(1);
  }
}

/// What BattleTransition_InitializeEntries treats as an array of
/// these; Transform_SetIdentity writes over it as a matrix.
struct BattleTransitionEntry {
  u32 value;   ///< the first word, read before and after the identity is set
  u32 sum;     ///< set to the first word's old value plus its new one
  u32 rest[2]; ///< never read
};

/// Sets REG_BLDCNT to lighten every layer by REG_BLDY; nothing calls it.
void BattleBackground_SetBlendControl(void) {
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
}

// XXX (no evidence): what the sum is for; The Lost Age keeps the same
// function, and nothing in either game calls it or holds its address.
/// Sets entries to the identity transform, then stores the first word's old
/// value plus its new one in the second; nothing calls it.
void BattleTransition_InitializeEntries(struct BattleTransitionEntry *entries) {
  u32 previous = entries[0].value;

  CAMELOT_ASM("the fixed-register identity store of transform.h")
  Transform_SetIdentity(entries);
  entries[0].sum = previous + entries[0].value;
}

/// Nothing calls it.
s32 BattleBackground_DivideBy16(s32 value) { return value / 16; }

/// A debug aid: while Start is held, edits BattleWork.debugValue with the
/// d-pad (1 left and right, 100 up and down) until A takes it as the result;
/// with Select held, returns DEBUG_SELECT_VALUE instead.
/// Nothing in TBS calls it; it returns selectedValue unchanged otherwise.
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

/// Sets the OBJ mode (enum BattleSpriteMode) of every sprite of a unit's
/// battle object, the picture and its shadow.
/// Does nothing for a unit with no battle object or motion object. Called by
/// BattleHighlight_SetModes and its helpers here, by the opening's
/// storm (BattleTransition_Run), by the staging (staging.c)
/// and by auto battle (auto_battle.c).
void BattleHighlight_SetSpriteMode(s32 actorId, s32 mode) {
  struct BattleObject *slot = BattleObject_Get(actorId);
  struct Object *object;
  struct Sprite *record;
  struct Sprite **records;
  s32 recordIndex;

  if (slot == 0)
    return;
  object = slot->object;
  if (object == 0)
    return;
  switch (object->spriteStorageKind & OBJECT_SPRITE_KIND_MASK) {
  case OBJECT_SPRITE_ONE: {
    record = object->sprite;
    record->image.objectMode = mode;
    record->shadow.objectMode = mode;
    break;
  }
  case OBJECT_SPRITE_GROUP: {
    records = (struct Sprite **)object->sprite;
    recordIndex = 0;
    do {
      record = *records++;
      if (record == 0)
        break;
      record->image.objectMode = mode;
      record->shadow.objectMode = mode;
      recordIndex++;
    } while (recordIndex <= OBJECT_SPRITE_GROUP_SIZE - 1);
    break;
  }
  }
}

/// Makes every unit's sprites semi-transparent but selectedUnitId's, which are
/// left as they are; nothing calls it.
void BattleHighlight_SetOtherSpriteModes(s32 selectedUnitId) {
  s16 unitIds[14];
  s32 unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++) {
    if (unitIds[unitIndex] != selectedUnitId)
      BattleHighlight_SetSpriteMode(unitIds[unitIndex],
                                    SPRITE_SEMI_TRANSPARENT);
  }
}

/// Returns every unit's sprites to SPRITE_OPAQUE; called by
/// BattleHighlight_SetModes when the highlight stops.
void BattleHighlight_ClearAllSpriteModes(void) {
  s16 unitIds[14];
  s32 unitCount = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  s32 unitIndex;

  for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
    BattleHighlight_SetSpriteMode(unitIds[unitIndex], SPRITE_OPAQUE);
}

/// Lightens the opaque sprites, the highlighted units, by the next step of
/// gBattleHighlight_BlendRamp.
/// The ramp's first sixteen steps play once, then its second sixteen over
/// and over. Runs each frame as a task in the VBlank pass
/// (priority TASK_PASS_VBLANK), added by BattleHighlight_SetModes while
/// BattleWork.highlightMode is set; writes REG_BLDCNT, REG_BLDALPHA,
/// REG_BLDY and BattleWork.highlightStep.
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

/// Highlights the units in selectedUnits, a list ended by
/// BATTLE_UNIT_LIST_END, or with HIGHLIGHT_OFF stops the highlight.
/// With HIGHLIGHT_SELECTED (any odd mode) every other unit's sprites turn
/// semi-transparent and the selected units' stay opaque, so only they take the
/// pulsing lightening of BattleBackground_StepHighlightBlend. Called with the
/// acting unit by BattleStaging_DispatchAction, with the targets being chosen
/// by the command menu (party_commands.c, targets.c, selection.c), and with 0
/// when either is done. Sets BattleWork.highlightMode and highlightStep, and
/// adds or removes the callback.
void BattleHighlight_SetModes(u16 *selectedUnits, s32 mode) {
  s16 unitIds[14];
  struct BattleWork *battle = gBattleWork;
  u32 unitCount;
  u32 unitIndex;

  if (mode == HIGHLIGHT_OFF) {
    Task_Remove(BattleBackground_StepHighlightBlend);
    REG_BLDY = mode;
    BattleHighlight_ClearAllSpriteModes();
    WaitFrames(1);
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
    WaitFrames(1);
    IoWriteQueue_Store16((u32)&REG_BLDCNT, 0);
    Task_Add(BattleBackground_StepHighlightBlend, TASK_PASS_VBLANK);
  }
}

#undef BACKGROUND_LEVEL_STEP
#undef BACKGROUND_PLANE_UPPER
#undef BACKGROUND_PLANE_LOWER
#undef BACKGROUND_PLANE_OFF
