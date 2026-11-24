/// @file
/// A unit's elemental glow: the star field and the other glows.
// Part of battle.c.

#include "battle_internal.h"

/// One of the star field's sparks, drawn inward to the centre. Sixteen live
/// in SparkState.sparks; a spent one starts again while the field is young.
struct Spark {
  s32 motion[6]; ///< x, y, z, then their velocities, in STAR_FIELD_PIXEL units
  s32 life;      ///< frames left
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
/// The blue ring sprite sheet's colours, before its pictures; an element's
/// palette replaces them.
#define STAR_FIELD_COLORS 64
/// The sparks and rings move in 1/1024 canvas pixels: STAR_FIELD_PIXEL to a
/// pixel.
#define STAR_FIELD_PIXEL_SHIFT 10
#define STAR_FIELD_PIXEL (1 << STAR_FIELD_PIXEL_SHIFT)

/// The star field's state, for the 45 frames of a cast's glow.
struct SparkState {
  /// The blue ring sprite sheet's pictures, the sparks' and the rings'.
  u8 sheet[4544];
  struct Spark sparks[STAR_FIELD_SPARKS];
  struct Ring rings[STAR_FIELD_RINGS];
  s32 frames; ///< frames drawn; new sparks start only in the first
              ///< STAR_FIELD_SPAWN_FRAMES
  s32 ready;  ///< 1 when a drawn frame waits for ElementGlow_UploadStarField
  /// Where BG2 shows the 128 by 128 canvas: ElementGlow_Play
  /// keeps it on the unit, as a battle effect's tracked canvas would be.
  struct BattleBg2Origin canvasOrigin;
};

STATIC_ASSERT(offsetof(struct SparkState, canvasOrigin) ==
              offsetof(struct BattleEffectState, trackedCanvas.origin));

/// Draws the star field's next frame: each spark falls towards the centre and
/// shrinks as its life runs out, and the three rings move outward.
UNREAD_RESULT ElementGlow_UpdateStarField(void) {
  s32 ringIndex;
  s32 picture;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *canvas;
  s32 sparkIndex;
  struct SparkState *state;
  DrawRectangle draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *axisPosition;
  s32 axis;
  FIXED distance;
  FIXED scale;
  s32 coordinate;
  FIXED acceleration;
  u32 radius;
  s32 angle;
  u32 halfRadius;
  s32 x;
  s32 y;
  s32 life;

  canvas = gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  state = gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  state->ready = 0;
  draw = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  for (sparkIndex = 0; sparkIndex < STAR_FIELD_SPARKS; sparkIndex++) {
    spark = &state->sparks[sparkIndex];
    life = spark->life;
    // A spark ends near the centre; until then it is pulled towards it.
    if (life != 0) {
      distance = Math_SqrtFX((spark->motion[0] >> 8) * (spark->motion[0] >> 8) +
                             (spark->motion[1] >> 8) * (spark->motion[1] >> 8) +
                             (spark->motion[2] >> 8) * (spark->motion[2] >> 8));
      if (distance <= toFIXED(1) / 16 - 1) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(distance, FIXED_ONE);
        spark->life--;
        axisPosition = spark->motion;
        for (axis = 0; axis < 3; axis++) {
          coordinate = *axisPosition;
          acceleration = Iwram_MulFX(Iwram_MulFX(-coordinate >> 8, scale),
                                     FIXED_ONE * 19 / 16);
          axisPosition[3] =
              axisPosition[3] - (axisPosition[3] >> 7) + acceleration;
          *axisPosition = coordinate + axisPosition[3];
          axisPosition++;
        }
      }
      life = spark->life;
    }
    // A spent spark starts again, as ElementGlow_InitializeStarField starts
    // them, while the field is young.
    if (life == 0 && state->frames < STAR_FIELD_SPAWN_FRAMES) {
      angle = Random16();
      radius = Random16() + FIXED_ONE;
      halfRadius = radius >> 1;
      spark->motion[0] = Iwram_MulFX(Trig_Cos(angle), halfRadius);
      spark->motion[1] = Iwram_MulFX(Trig_Sin(angle), halfRadius);
      if (spark->motion[0] & 1)
        spark->motion[0] = -spark->motion[0];
      if (spark->motion[1] & 1)
        spark->motion[1] = -spark->motion[1];
      spark->motion[2] = (Random16() + toFIXED(0.5)) >> 2;
      spark->motion[3] = (-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
      spark->motion[4] = (-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
      spark->motion[5] = 0;
      life = spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
    }
    // Drawn about the canvas's centre, 1 to 4 pixels square by its
    // life left, 4 from three frames left.
    if (life != 0) {
      x = (spark->motion[0] >> STAR_FIELD_PIXEL_SHIFT) + STAR_FIELD_CENTER;
      y = (spark->motion[1] >> STAR_FIELD_PIXEL_SHIFT) + STAR_FIELD_CENTER;
      picture = life;
      if (picture < 0)
        picture = 0;
      else if (picture > 6)
        picture = 6;
      size = gElementGlow_SparkSizes[picture];
      draw(canvas, (u8 *)state + gElementGlow_SparkPictures[picture],
           x - (size >> 1), y - (size >> 1), size, size);
    }
  }
  // The rings, through the blitter that keeps the brighter colour: each shows
  // its four pictures eight frames apiece, then is no longer drawn.
  draw = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_PAIRED];
  for (ring = state->rings, ringIndex = STAR_FIELD_RINGS - 1; ringIndex >= 0;
       ringIndex--, ring++) {
    ring->x += ring->velocityX;
    ring->y += ring->velocityY;
    ringX = ring->x >> STAR_FIELD_PIXEL_SHIFT;
    ringY = ring->y >> STAR_FIELD_PIXEL_SHIFT;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      draw(canvas, (u8 *)state + gElementGlow_RingPictures[phase],
           ringX + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
           ringY + STAR_FIELD_CENTER - STAR_FIELD_RING_SIZE / 2,
           STAR_FIELD_RING_SIZE, STAR_FIELD_RING_SIZE);
    }
  }
  state->frames++;
  state->ready = 1;
}

/// Copies the drawn canvas to BG2's tiles.
// UB: the task runner invokes a void task.
#ifdef UBFIX
void ElementGlow_UploadStarField(void) {
#else
u32 ElementGlow_UploadStarField(void) {
#endif
  void *source = gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  struct SparkState *state = gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];

  if (source != 0) {
    if (state->ready != 0) {
      state->ready = 0;
      PaletteFade_BackupAndScaleThreeQuarters(source, BG_CHAR_BLOCK(1),
                                              BG_CHAR_SIZE);
    }
  }
}

/// The two tasks as the task runner takes them: without UBFIX they are
/// declared returning a value, and are cast to TaskFunc.
#ifdef UBFIX
#define STAR_FIELD_UPDATE_FUNC ElementGlow_UpdateStarField
#define STAR_FIELD_UPLOAD_FUNC ElementGlow_UploadStarField
#else
#define STAR_FIELD_UPDATE_FUNC ((TaskFunc)ElementGlow_UpdateStarField)
#define STAR_FIELD_UPLOAD_FUNC ((TaskFunc)ElementGlow_UploadStarField)
#endif

/// Starts a star field of the element: sixteen sparks on random rings and three
/// rings 120 degrees apart. Returns the upload task's index.
s32 ElementGlow_InitializeStarField(s32 element) {
  struct SparkState *state;
  struct Spark *spark;
  struct Ring *ring;
  s32 effectIndex;
  u8 *graphics;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED struct Spark unused;

  gBattleBackgroundState->mode = BATTLE_BACKGROUND_FIXED;
  state = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, STAR_FIELD_SIZE * STAR_FIELD_SIZE);
  /* FAKEMATCH(dead): an empty once-only loop. */
  do {
  } while (0);
  // Each spark at a random angle and height, swirling inward; it lives a frame
  // for every four pixels it starts out from the centre, and one more.
  for (effectIndex = 0; effectIndex < STAR_FIELD_SPARKS; effectIndex++) {
    s32 angle = Random16();
    u32 radius = Random16() + FIXED_ONE;
    u32 halfRadius = radius >> 1;

    spark = &state->sparks[effectIndex];
    spark->motion[0] = Iwram_MulFX(Trig_Cos(angle), halfRadius);
    spark->motion[1] = Iwram_MulFX(Trig_Sin(angle), halfRadius);
    if (spark->motion[0] & 1)
      spark->motion[0] = -spark->motion[0];
    if (spark->motion[1] & 1)
      spark->motion[1] = -spark->motion[1];
    spark->motion[2] = (Random16() + toFIXED(0.5)) >> 2;
    spark->motion[3] = (-spark->motion[0] >> 7) + (spark->motion[1] >> 8);
    spark->motion[4] = (-spark->motion[1] >> 7) + (-spark->motion[0] >> 8);
    spark->motion[5] = 0;
    spark->life = radius / (8 * STAR_FIELD_PIXEL) + 1;
  }
  for (effectIndex = 0; effectIndex < STAR_FIELD_RINGS; effectIndex++) {
    s32 radius = 4 * STAR_FIELD_PIXEL;
    s32 angle;

    ring = &state->rings[effectIndex];
    angle = effectIndex * (DEGtoANG(120));
    ring->x = Iwram_MulFX(Trig_Cos(angle), radius);
    ring->y = Iwram_MulFX(Trig_Sin(angle), radius);
    ring->velocityX = Iwram_MulFX(Trig_Cos(angle), STAR_FIELD_PIXEL / 2);
    ring->velocityY = Iwram_MulFX(Trig_Sin(angle), STAR_FIELD_PIXEL / 2);
    ring->age = 0;
  }
  state->frames = 0;
  state->ready = 0;
  state->canvasOrigin.queued = 0;
  Iwram_ClearWords(gHeapSlots[HEAP_SLOT_BATTLE_CANVAS],
                   STAR_FIELD_SIZE * STAR_FIELD_SIZE);
  // The sprite sheet's colours, then its pictures; then the element's own
  // colours over them.
  graphics = Resource_GetTableEntry(RESOURCE_BLUE_RING_SHEET);
  Iwram_CopyWords(BG_PLTT, graphics, STAR_FIELD_COLORS * 2);
  graphics += STAR_FIELD_COLORS * 2;
  Decompress_TaggedLz(graphics, state);
  switch (element) {
  case ELEMENT_VENUS:
    graphics = Resource_GetTableEntry(RESOURCE_YELLOW_PALETTE_B);
    break;
  case ELEMENT_MERCURY:
    graphics = Resource_GetTableEntry(RESOURCE_BLUE_RING_SHEET);
    break;
  case ELEMENT_MARS:
    graphics = Resource_GetTableEntry(RESOURCE_RED_PALETTE_C);
    break;
  default:
    graphics = Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_D);
    break;
  }
  Dma_Set(graphics, BG_PLTT, DMA_COPY_WORDS(STAR_FIELD_COLORS / 2), REG_DMA3);
  Io_Write32(0, &REG_BG2X);
  Io_Write32(0, &REG_BG2Y);
  Io_Write16(Q_8_8(1), &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(Q_8_8(1), &REG_BG2PD);
  BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE, 7, 7,
                               BLITTER_CLIP_X | BLITTER_CLIP_Y,
                               COMPOSITE_ADDITIVE);
  BattleEffect_BuildBlitterFar(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7,
                               BLITTER_CLIP_X | BLITTER_CLIP_Y,
                               COMPOSITE_LIGHTEN);
  Task_Add(STAR_FIELD_UPDATE_FUNC, TASK_PASS_FRAME);
  return Task_Add(STAR_FIELD_UPLOAD_FUNC, TASK_PASS_FRAME);
}

/// Ends a star field and turns BG2 off.
s32 ElementGlow_End(s32 unused) {
  Iwram_ClearWords(BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
  Task_Remove(STAR_FIELD_UPLOAD_FUNC);
  return Task_Remove(STAR_FIELD_UPDATE_FUNC);
}

/// Copies count colours from source to destination, each channel scaled by
/// scale (16.16, at most 1), and returns 0.
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, FIXED scale,
                                s32 count) {
  s32 i;

  if (scale > FIXED_ONE)
    scale = FIXED_ONE;
  // Each channel scaled in place: a 16.16 scale of at most 1 keeps it inside
  // its own five bits.
  for (i = 0; i < count; i++) {
    u32 redMask = RGB(RGB_LEVEL_MAX, 0, 0);
    u32 greenMask = RGB(0, RGB_LEVEL_MAX, 0);
    u32 blueMask = RGB(0, 0, RGB_LEVEL_MAX);
    u32 color = *source;
    u32 red = color & redMask;
    u32 green = color & greenMask;
    u32 blue = blueMask & color;

    red *= scale;
    green *= scale;
    blue *= scale;
    color = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
    color |= FIXEDtoINT(blue) & blueMask;
    *destination = color;
    source++;
    destination++;
  }
  return 0;
}

/// Plays a glow of the element about a unit, and returns when it is over: the
/// star field, the streaks, the pillar or the burst.
void ElementGlow_Play(s32 unitId, s32 element, s32 mode, s32 variant) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 spare[10];
  struct ScreenPoint starFieldPosition;
  struct ScreenPoint streakPosition;
  struct BattleStagingState pillarLaunch;
  struct BattleStagingState burstLaunch;
  // The heap slots read from the camera's, as the battle effects read theirs
  // from the effect's (EFFECT_SLOT).
  void **slots = &gHeapSlots[HEAP_SLOT_CAMERA];
  volatile u16 *ime;
  struct BattleState *battle =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_CAMERA, HEAP_SLOT_BATTLE);
  struct SparkState *state;
  s32 frame;
  s32 *brightness;

  wait(1);
  BattleBackground_SetMode(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), BG_CHAR_SIZE);
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                                              DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                                              DISPCNT_WIN0_ON);
  IoWriteQueue_Store16((u32)&REG_BG2CNT, BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                                             BGCNT_SCREEN_BASE(7));
  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  wait(1);
  Io_Write16(WIN_RANGE(0, DISPLAY_WIDTH), &REG_WIN0H);
  Io_Write16(WIN_RANGE(16, 136), &REG_WIN0V);
  Io_Write16(WININ_WIN0_ALL, &REG_WININ);
  Io_Write16(WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ, &REG_WINOUT);
  if (mode == GLOW_STAR_FIELD) {
    // The star field for 45 frames, the background darkening for the first 25.
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, BLEND_ALPHA(14, BLEND_WEIGHT_MAX));
    ElementGlow_InitializeStarField(element);
    for (frame = 0; frame < STAR_FIELD_FRAMES; frame++) {
      brightness = &battle->brightness;
      ime = &REG_IME;
      state = gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
      if (frame <= 24)
        Graphics_ScaleRgb555Clamped(battle->palette, BATTLE_BG_PALETTE,
                                    *brightness = FIXED_ONE -
                                                  frame * BACKGROUND_LEVEL_STEP,
                                    BATTLE_BACKGROUND_COLORS);
      BattleMotion_ProjectScaledPosition(
          unitId, ScreenPoint_Coordinates(&starFieldPosition));
      state->canvasOrigin.x = (STAR_FIELD_CENTER - (u32)starFieldPosition.x)
                              << 8;
      state->canvasOrigin.y = (STAR_FIELD_CENTER - (u32)starFieldPosition.y)
                              << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, (u32)&state->canvasOrigin.x,
                              DMA_COPY_WORDS(2));
      state->canvasOrigin.queued = 1;
      wait(1);
    }
    ElementGlow_End(element);
  } else if (mode == GLOW_STREAKS) {
    // The streaks for 40 frames, their canvas centred on the unit likewise.
    struct BattleEffectState *effectState;

    // The effect uses its image area to stage the tracked canvas origin.
    BattleEffect_StartElementStreaksFar(element);
    for (frame = 0; frame < 40; frame++) {
      effectState = gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
      BattleMotion_ProjectScaledPosition(
          unitId, ScreenPoint_Coordinates(&streakPosition));
      effectState->trackedCanvas.origin.x =
          (STAR_FIELD_CENTER - (u32)streakPosition.x) << 8;
      effectState->trackedCanvas.origin.y =
          (STAR_FIELD_CENTER - (u32)streakPosition.y) << 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X,
                          (u32)&effectState->trackedCanvas.origin.x,
                          DMA_COPY_WORDS(2));
      effectState->trackedCanvas.origin.queued = 1;
      wait(1);
    }
    BattleEffect_EndElementStreaksFar();
  } else if (mode == GLOW_PILLAR) {
    // The pillar and the burst run a battle effect with the unit as its user
    // and only target.
    pillarLaunch.effect.prelude = 0;
    pillarLaunch.effect.kind = element;
    pillarLaunch.effect.variant = variant;
    pillarLaunch.effect.user = unitId;
    pillarLaunch.effect.targets[0] = unitId;
    pillarLaunch.effect.firstTarget = unitId;
    pillarLaunch.effect.targetCount = 1;
    pillarLaunch.effect.range = 1;
    BattleEffect_RunElementPillarFar(&pillarLaunch.effect);
  } else {
    burstLaunch.effect.kind = element;
    burstLaunch.effect.prelude = 0;
    burstLaunch.effect.variant = 0;
    burstLaunch.effect.user = unitId;
    burstLaunch.effect.targets[0] = unitId;
    burstLaunch.effect.firstTarget = unitId;
    burstLaunch.effect.targetCount = 1;
    burstLaunch.effect.range = 1;
    BattleEffect_RunElementBurstFar(&burstLaunch.effect);
  }
}

/// Copies the background's saved palette back undarkened, as the star field of
/// ElementGlow_Play leaves it dark.
void BattleBackground_RestorePalette(void) {
  BattleBackground_SetPaletteLevel(0, 0);
}

/// Removes the VCount interrupt's handler, as
/// BattleTransition_Run installs one; nothing calls it.
void Irq_RemoveVCountHandler(void) { Irq_SetHandler(IRQ_VCOUNT, 0, 0); }
