/// @file
/// A unit's elemental glow: the star field and the other glows.
// Part of battle.c.

#include "../battle_internal.h"

/// One of the star field's sparks, drawn inward to the centre. Sixteen live
/// in SparkWork.sparks; a spent one starts again while the field is young.
struct Spark {
  s32 pos[6]; ///< x, y, z, then their velocities, in 1/1024 canvas pixels
  s32 life;   ///< frames left
};

/// The star field's work: its sprite sheet, 16 sparks and 3 rings. It lives
/// in the HEAP_SLOT_BATTLE_EFFECT work slot from BattleFx_InitializeStarField
/// to Graphics_ResetVramBlockAndReleaseHeapBlocks, the 45 frames of a cast's
/// glow (BattleFx_PlayUnitElementEffect, mode 0). BattleFx_UpdateStarField
/// draws from it, BattleFx_UploadStarField reads its ready flag, and
/// BattleFx_PlayUnitElementEffect writes its canvas origin.
struct SparkWork {
  /// The blue ring sheet's pictures, decoded: the sparks' sizes at
  /// gBattleFx_StarWorkOffsets, the rings' frames at
  /// gBattleFx_RingFrameOffsets.
  u8 sheet[0x11c0];
  struct Spark sparks[16];
  struct Ring rings[3];
  s32 frames; ///< frames drawn; new sparks start only in the first 25
  s32 ready;  ///< 1 when a drawn frame waits for BattleFx_UploadStarField
  /// Where BG2 shows the 128 by 128 canvas: BattleFx_PlayUnitElementEffect
  /// keeps it on the unit, as an effect's tracked canvas would be.
  struct BattleCanvasOrigin canvasOrigin;
};

STATIC_ASSERT(offsetof(struct SparkWork, canvasOrigin) ==
              offsetof(struct BattleEffectWork, trackedCanvas.origin));

/// A blitter loaded into a work slot: draws a width by height picture at
/// (x, y) on the canvas.
typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width,
                         s32 height);

// Draws the star field's next frame on the canvas (HEAP_SLOT_BATTLE_CANVAS):
// each spark falls towards the centre and is drawn smaller as its life runs
// out, a spent one starts again on a ring of random radius during the first
// 25 frames, and the three rings move outward through four frames. Sets
// SparkWork.ready for BattleFx_UploadStarField. Runs each frame as a
// scheduler callback added by BattleFx_InitializeStarField.
// UB: the scheduler invokes a void callback. Plain void changes the native
// return register from r1 to r0 in tbs-en; matching builds keep the old type.
#ifdef UBFIX
void BattleFx_UpdateStarField(void) {
#else
s32 BattleFx_UpdateStarField(void) {
#endif
  s32 ringIndex;
  s32 frame;
  u32 size;
  s32 ringX;
  s32 ringY;
  s32 phase;
  void *canvas;
  s32 sparkIndex;
  struct SparkWork *work;
  DrawFunc draw;
  struct Spark *spark;
  struct Ring *ring;
  s32 *axisPosition;
  s32 axis;
  s32 distance;
  s32 scale;
  s32 coordinate;
  s32 acceleration;
  u32 radius;
  s32 angle;
  u32 halfRadius;
  s32 x;
  s32 y;
  s32 life;

  canvas = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  work->ready = 0;
  draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER];
  for (sparkIndex = 0; sparkIndex < 16; sparkIndex++) {
    spark = &work->sparks[sparkIndex];
    life = spark->life;
    // A live spark ends within 0xfff (4 canvas pixels) of the centre; else
    // each axis's velocity loses 1/128 and gains a pull of 1.1875 (0x13000)
    // towards the centre, the axis's share of its distance.
    if (life != 0) {
      distance = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8) +
                           (spark->pos[1] >> 8) * (spark->pos[1] >> 8) +
                           (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
      if (distance <= 0xfff) {
        spark->life = 0;
      } else {
        scale = Iwram_RatioMulQ14(distance, 0x10000);
        spark->life--;
        axisPosition = spark->pos;
        for (axis = 2; axis >= 0; axis--) {
          coordinate = *axisPosition;
          acceleration =
              Iwram_MulQ16(Iwram_MulQ16(-coordinate >> 8, scale), 0x13000);
          axisPosition[3] =
              axisPosition[3] - (axisPosition[3] >> 7) + acceleration;
          *axisPosition = coordinate + axisPosition[3];
          axisPosition++;
        }
      }
      life = spark->life;
    }
    // A spent spark starts again, as BattleFx_InitializeStarField starts
    // them, while the field is young.
    if (life == 0 && work->frames <= 24) {
      angle = Random16();
      radius = Random16() + 0x10000;
      halfRadius = radius >> 1;
      spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), halfRadius);
      spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), halfRadius);
      if (spark->pos[0] & 1)
        spark->pos[0] = -spark->pos[0];
      if (spark->pos[1] & 1)
        spark->pos[1] = -spark->pos[1];
      spark->pos[2] = (Random16() + 0x8000) >> 2;
      spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
      spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
      spark->pos[5] = 0;
      life = spark->life = (radius >> 13) + 1;
    }
    // Drawn about the canvas's centre (64, 64), 1 to 4 pixels square by its
    // life left, 4 from three frames left.
    if (life != 0) {
      x = (spark->pos[0] >> 10) + 64;
      y = (spark->pos[1] >> 10) + 64;
      frame = life;
      if (frame < 0)
        frame = 0;
      else if (frame > 6)
        frame = 6;
      size = gBattleFx_StarSizes[frame];
      draw(canvas, (u8 *)work + gBattleFx_StarWorkOffsets[frame],
           x - (size >> 1), y - (size >> 1), size, size);
    }
  }
  // The rings, 32 by 32, with the other blitter: each shows its four frames
  // eight frames apiece, then is no longer drawn.
  draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  for (ring = work->rings, ringIndex = 2; ringIndex >= 0; ringIndex--, ring++) {
    ring->x += ring->velX;
    ring->y += ring->velY;
    ringX = ring->x >> 10;
    ringY = ring->y >> 10;
    phase = 3 - ring->age / 8;
    if (phase >= 0) {
      ring->age++;
      draw(canvas, (u8 *)work + gBattleFx_RingFrameOffsets[phase], ringX + 48,
           ringY + 48, 32, 32);
    }
  }
  work->frames++;
  work->ready = 1;
}

// Copies the canvas, once a frame is drawn, to BG2's tiles in character block
// 1 through ColorBuffer_BackupAndScaleThreeQuarters, and clears
// SparkWork.ready. Runs each frame as a scheduler callback added by
// BattleFx_InitializeStarField. UB: the scheduler invokes a void callback.
// Plain void changes the native return register from r1 to r0 in tbs-en;
// matching builds keep the old type.
#ifdef UBFIX
void BattleFx_UploadStarField(void) {
#else
u32 BattleFx_UploadStarField(void) {
#endif
  void *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  struct SparkWork *work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

  if (source != 0) {
    if (work->ready != 0) {
      work->ready = 0;
      ColorBuffer_BackupAndScaleThreeQuarters(source, BG_CHAR_BLOCK(1), 0x4000);
    }
  }
}

// The two callbacks as the scheduler takes them: cast to SchedulerCallback
// where the native types return a value.
#ifdef UBFIX
#define STAR_FIELD_UPDATE_CALLBACK BattleFx_UpdateStarField
#define STAR_FIELD_UPLOAD_CALLBACK BattleFx_UploadStarField
#else
#define STAR_FIELD_UPDATE_CALLBACK ((SchedulerCallback)BattleFx_UpdateStarField)
#define STAR_FIELD_UPLOAD_CALLBACK ((SchedulerCallback)BattleFx_UploadStarField)
#endif

/// Starts a star field of the element mode (0 to 3, enum Element order):
/// sixteen sparks on random rings and three rings 120 degrees apart.
/// Allocates the field's work (HEAP_SLOT_BATTLE_EFFECT) and a cleared
/// 128 by 128 canvas (HEAP_SLOT_BATTLE_CANVAS), decodes the blue ring sheet
/// into the work, loads the element's 64 colours over the first of BG palette
/// RAM, sets BG2's affine parameters to no zoom, loads both blitters and adds
/// BattleFx_UpdateStarField and BattleFx_UploadStarField. Also sets
/// BattleBackgroundView.mode to 1. Called by BattleFx_PlayUnitElementEffect;
/// Graphics_ResetVramBlockAndReleaseHeapBlocks undoes it. Returns the upload
/// callback's task index (-1 when the task table is full), which the caller
/// does not read.
s32 BattleFx_InitializeStarField(s32 mode) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 effectIndex;
  u8 *graphics;
  s32 zero;
  /* FAKEMATCH(dead): an unused Spark for the frame's 28 bytes; without it, 2
   * of 246 instructions differ in tbs-en. */
  UNUSED struct Spark unused;

  // Word 2 of the background's work is BattleBackgroundView.mode.
  ((s32 *)gTransitionWork)[2] = 1;
  work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
  /* FAKEMATCH(one-pass): an empty one-pass loop, a scheduling barrier that
   * keeps the multiply routine's address ahead of the spark cursor; without
   * it, 1 of 246 instructions differ in tbs-en. */
  do {
  } while (0);
  // Each spark at a random angle, half of 1 to 2 (16.16) out, a random height
  // above, swirling: its velocity points inward and a little sideways. An odd
  // coordinate is mirrored, and the further out, the longer it lives.
  for (effectIndex = 0; effectIndex < 16; effectIndex++) {
    s32 angle = Random16();
    u32 radius = Random16() + 0x10000;
    u32 halfRadius = radius >> 1;

    spark = &work->sparks[effectIndex];
    spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), halfRadius);
    spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), halfRadius);
    if (spark->pos[0] & 1)
      spark->pos[0] = -spark->pos[0];
    if (spark->pos[1] & 1)
      spark->pos[1] = -spark->pos[1];
    spark->pos[2] = (Random16() + 0x8000) >> 2;
    spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
    spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
    spark->pos[5] = 0;
    spark->life = (radius >> 13) + 1;
  }
  for (effectIndex = 0; effectIndex < 3; effectIndex++) {
    s32 radius = 0x1000;
    s32 angle;

    ring = &work->rings[effectIndex];
    angle = effectIndex * 0x5555;
    ring->x = Iwram_MulQ16(Trig_Cos(angle), radius);
    ring->y = Iwram_MulQ16(Trig_Sin(angle), radius);
    ring->velX = Iwram_MulQ16(Trig_Cos(angle), 0x200);
    ring->velY = Iwram_MulQ16(Trig_Sin(angle), 0x200);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->canvasOrigin.unk_08 = 0;
  Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
  // The sheet's 128 bytes of colours, then its pictures; then the
  // element's own colours over them.
  graphics = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
  Iwram_CopyWords(BG_PLTT, graphics, 128);
  graphics += 128;
  Resource_DecodeType01(graphics, work);
  switch (mode) {
  case 0:
    graphics = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
    break;
  case 1:
    graphics = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    break;
  case 2:
    graphics = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
    break;
  default:
    graphics = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
    break;
  }
  Dma_Set(graphics, BG_PLTT, DMA_COPY_WORDS(32), REG_DMA3);
  /* FAKEMATCH(one-pass): the zero and its stores in a one-pass block, which
   * keeps the zero ahead of BG2X's address; as plain stores of 0, 1 of 246
   * instructions differ in tbs-en. */
  do {
    zero = 0;
    REG_BG2X = zero;
    REG_BG2Y = zero;
  } while (0);
  Io_Write16(Q_8_8(1), &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(Q_8_8(1), &REG_BG2PD);
  BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
  BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
  Scheduler_AddOrUpdateCallback(STAR_FIELD_UPDATE_CALLBACK, 0xc80);
  return Scheduler_AddOrUpdateCallback(STAR_FIELD_UPLOAD_CALLBACK, 0xc80);
}

/// Ends a star field: clears BG2's tiles in character block 1, frees its
/// blitters, canvas and work, turns BG2 off and removes both callbacks.
/// Called by BattleFx_PlayUnitElementEffect once the field has played.
/// Returns Scheduler_RemoveCallback's result, which the caller does not
/// read.
s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused) {
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
  Scheduler_RemoveCallback(STAR_FIELD_UPLOAD_CALLBACK);
  return Scheduler_RemoveCallback(STAR_FIELD_UPDATE_CALLBACK);
}

/// Copies count colours from source to destination, each channel scaled by
/// scale (16.16, at most 1), and returns 0.
/// Called to darken the background's palette by
/// BattlePresentation_SetPaletteLevel, BattleBackground_Load,
/// BattleFx_PlayUnitElementEffect, the item staging (staging.c) and the
/// effects' canvas (canvas.c).
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale,
                                s32 count) {
  s32 remaining;
  u32 redMask;
  u32 greenMask;
  u32 blueMask;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (scale > 0x10000)
    scale = 0x10000;
  if (count > 0) {
    redMask = 0x1f;
    greenMask = 0x3e0;
    blueMask = 0x7c00;
    remaining = count;
    do {
      pixel = *source;
      red = pixel & redMask;
      green = pixel & greenMask;
      blue = blueMask & pixel;
      red *= scale;
      green *= scale;
      blue *= scale;
      pixel = ((red >> 16) & redMask) | ((green >> 16) & greenMask);
      pixel |= (blue >> 16) & blueMask;
      *destination = pixel;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// Plays a glow of the element about a unit, and returns when it is over.
/// By mode: 0 the star field gathering on the unit while the background
/// darkens (a cast), 1 the element streaks, 2 the element pillar at strength
/// variant (a summon's power boost) and 3 the element burst (a Djinni set).
/// Called with 0 by BattlePresentation_RunUnitTransition and
/// BattlePres_RunPsynergyCast, with 1 by the latter, with 2 by
/// BattleUnit_ProcessTurnEnd and with 3 by Battle_ProcessRoundEnd and
/// BattleCommand_BuildPlan. Every mode first puts the background in mode 1
/// and opens window 0 over lines 16 to 135, with BG2 blended over the other
/// layers; mode 0 leaves the background's palette and BattleWork.brightness
/// darkened.
void BattleFx_PlayUnitElementEffect(s32 unitId, s32 element, s32 mode,
                                    s32 variant) {
  /* FAKEMATCH(dead): an unused array for the frame's 40 bytes; without it, 2
   * of 249 instructions differ in tbs-en. */
  UNUSED s32 spare[10];
  struct EffectPosition position;
  struct EffectPosition position2;
  struct BattlePresentationWork launch;
  struct BattlePresentationWork launch2;
  /* FAKEMATCH(temporary): one variable holds slot 12's address, the battle
   * work's base and later IME's; as battle = gWorkSlot[9] with ime set only
   * in the loop, 61 of 249 instructions differ in tbs-en. */
  volatile u16 *ime = (volatile u16 *)&gWorkSlot[12];
  struct BattleWork *battle = *(struct BattleWork **)((u8 *)ime - 12);
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct EffectPosition *projectedPosition;

  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFade(1, battle->background, 0);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                                              DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                                              DISPCNT_WIN0_ON);
  IoWriteQueue_Store16((u32)&REG_BG2CNT, BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                                             BGCNT_SCREEN_BASE(7));
  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  WaitFrames(1);
  Io_Write16(WIN_RANGE(0, 240), &REG_WIN0H);
  Io_Write16(WIN_RANGE(16, 136), &REG_WIN0V);
  Io_Write16(WININ_WIN0_ALL, &REG_WININ);
  Io_Write16(WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ, &REG_WINOUT);
  if (mode == 0) {
    // The star field for 45 frames, darkening the background a level a
    // frame for its first 25 and keeping BG2's canvas centred on the unit
    // (its origin queued to BG2X and BG2Y with interrupts off).
    IoWriteQueue_Store16((u32)&REG_BLDALPHA, BLEND_ALPHA(14, 16));
    BattleFx_InitializeStarField(element);
    for (frame = 0, projectedPosition = &position; frame <= 44; frame++) {
      brightness = &battle->brightness;
      ime = &REG_IME;
      work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
      if (frame <= 24)
        Graphics_ScaleRgb555Clamped(battle->palette, BATTLE_BG_PALETTE,
                                    *brightness = 0x10000 - frame * 1092, 128);
      BattleMotion_ProjectScaledPosition(
          unitId, EffectPosition_Coordinates(projectedPosition));
      work->canvasOrigin.x = (64U - (u32)projectedPosition->x) << 8;
      work->canvasOrigin.y = (64U - (u32)projectedPosition->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, (u32)&REG_BG2X, (u32)&work->canvasOrigin.x,
                              DMA_COPY_WORDS(2));
      work->canvasOrigin.unk_08 = 1;
      WaitFrames(1);
    }
    Graphics_ResetVramBlockAndReleaseHeapBlocks(element);
  } else if (mode == 1) {
    // The streaks for 40 frames, their canvas centred on the unit likewise.
    struct BattleEffectWork *effectWork;

    // The scene uses its image area to stage the tracked canvas origin.
    BattleFx_StartElementStreaksFar(element);
    for (frame = 0; frame < 40; frame++) {
      effectWork = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
      BattleMotion_ProjectScaledPosition(
          unitId, EffectPosition_Coordinates(&position2));
      effectWork->trackedCanvas.origin.x = (64U - (u32)position2.x) << 8;
      effectWork->trackedCanvas.origin.y = (64U - (u32)position2.y) << 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X,
                          (u32)&effectWork->trackedCanvas.origin.x,
                          DMA_COPY_WORDS(2));
      effectWork->trackedCanvas.origin.unk_08 = 1;
      WaitFrames(1);
    }
    BattleFx_EndElementStreaksFar();
  } else if (mode == 2) {
    // Modes 2 and 3 run an effect with the unit as its actor and only
    // target.
    launch.effect.prelude = 0;
    launch.effect.kind = element;
    launch.effect.variant = variant;
    launch.effect.actor = unitId;
    launch.effect.actors[0] = unitId;
    launch.effect.firstTarget = unitId;
    launch.effect.count = 1;
    launch.effect.range = 1;
    BattleFx_RunElementPillarFar(&launch.effect);
  } else {
    launch2.effect.kind = element;
    launch2.effect.prelude = 0;
    launch2.effect.variant = 0;
    launch2.effect.actor = unitId;
    launch2.effect.actors[0] = unitId;
    launch2.effect.firstTarget = unitId;
    launch2.effect.count = 1;
    launch2.effect.range = 1;
    BattleFx_RunElementBurstFar(&launch2.effect);
  }
}

/// Copies the background's saved palette back undarkened, as the star field
/// of BattleFx_PlayUnitElementEffect leaves it dark.
/// Called by BattlePresentation_RunUnitTransition and
/// BattlePres_RunPsynergyCast once the effect has played.
void BattlePres_RestorePalette(void) {
  BattlePresentation_SetPaletteLevel(0, 0);
}

/// Removes the VCount interrupt's handler, as BattlePres_RunEntryTransition
/// installs one; nothing calls it.
void Runtime_RemoveIrqHandlerSlot2(void) {
  Runtime_SetIrqHandler(IRQ_VCOUNT, 0, 0);
}
