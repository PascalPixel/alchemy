/// @file
/// Effect positions, projection and id dispatch helpers.
// Part of f15585c.c.

#include "../f15585c_internal.h"

void *GetMotionRecordFar(void *object, s32 index);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Render_ResetTransformState(void);

s32 Render_ProjectPoint(const void *point, s32 *screen);

s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  u8 *camera;
  s32 *object;
  u8 *info;
  s32 scale;

  camera = Ram_HeapSlots->cameraWork;
  object = GetBattleObjectSlotFar(unit)->object;
  info = GetMotionRecordFar(object, 0);
  Render_ResetTransformState();
  Transform_SetLookAt(camera, camera + 12);
  scale = Iwram_MulQ16(Render_ProjectPoint(object + 2, screen),
                       *(s32 *)(info + 12));
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeightFar(unit) >> 17);
  return 0;
}

void BattleEffect_ProbeKeysForExtendedActor(void) {
  struct BattleFxWorkTail *work;
  struct BattleEffectArgument *effect;

  work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  effect = work->effect;

  // DEAD: nothing uses the held keys read here; their load is in the ROM.
  if (effect->actors[0] > 0x7f)
    (void)((volatile struct InputState *)&gInput)->held;
}

void Func_0815f16c(struct BattleEffectArgument *effect, s32 index);
void BattleEffect_RunCanvasModeByCount(struct BattleEffectArgument *effect);
void Func_0815e9f4(struct BattleEffectArgument *effect);
void Func_0815e3ac(struct BattleEffectArgument *effect);

void BattleEffect_DispatchByIdRange(struct BattleEffectArgument *effect) {
  struct BattleFxWorkTail *work;
  s32 kind;
  s32 value;

  HeapSlot_AllocEwram(100, 0x60e);
  HeapSlot_Alloc(92, 0x7b7c);
  HeapSlot_Alloc(96, 0x4000);
  work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  work->effect = effect;
  work->active = 1;
  kind = effect->kind;
  value = kind - 100;
  if ((u32)value <= 100) {
    Func_0815f16c(effect, value);
  } else if (kind == 210 || kind == 10) {
    BattleEffect_RunCanvasModeByCount(effect);
  } else if (kind > 199) {
    Func_0815e9f4(effect);
  } else {
    Func_0815e3ac(effect);
  }
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  HeapSlot_Release(100);
}



// The blitter slots read as what they hold, a drawing routine.
#define ALTERNATE_BLITTER_SLOT                                                 \
  (((struct {                                                                  \
     u8 before[188];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)
#define BLITTER_SLOT                                                           \
  (((struct {                                                                  \
     u8 before[104];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define SPARK_COUNT 64


extern char ResourceId_WaveSheet;
extern char ResourceId_BeamImage;
extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowOrbSheet;
// The strike's six cels: ☀️'s gBeam_Cells and
// gTargetStrike_Reach, _StrikeWidths and _StrikeHeights.
extern u16 Data_0819886c[];
extern u8 Data_08198878[];
extern u8 Data_08198860[];
extern u8 Data_08198866[];
// Byte offsets of the spark cels in the spark sheet, by size - 1 (☀️'s
// gLuckyWheels_SparkCellOffsets).
extern u16 Data_08197424[];

void wait(s32 frames);
u32 Random16(void);
void *Resource_GetTableEntry(s32 id);

void *Unit_Get(s32 owner);
s32 Summon_IsEntrySecondaryFlaggedFar(s32 enemyId);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void EffectPosition_ProjectSprite(s32 actor, struct EffectPosition *position);

void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
// Configures the battle display for an effect.
void BattleEffect_ConfigureEffectDisplay(void);
// Sets up the canvas tile map on BG1.
void BattleEffect_SetupCanvasTileMap(void);
// Marks the transition done and shows the battle view again.
void BattleEffect_HideBg1AndRestoreBackdrop(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
// BattlePlayback_Start through its far call.
void Func_081180e8(s32 hitCue);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);

void Func_0815e3ac(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition anchor;
  struct EffectPosition point;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  s32 *camera;
  s32 kind;
  struct BattleUnit *unit;
  BlitterPair draw;
  struct MotionObject *target;
  s32 half;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  kind = effect->kind;
  unit = Unit_Get(effect->actor);

  // Setup: the display, the tile map on BG1, the blitters for the side.
  wait(1);
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = 0x1f80;
  wait(1);
  if (kind == 5 || kind == 6) {
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(HEAP_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_Y));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP_Y));
    } else {
      BattleEffect_LoadBlitter(HEAP_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP));
    }
  } else {
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(
          HEAP_BLITTER,
          BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      BattleEffect_LoadBlitter(
          HEAP_BLITTER_ALTERNATE,
          BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    } else {
      BattleEffect_LoadBlitter(HEAP_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_X));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP_X));
    }
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = ALTERNATE_BLITTER_SLOT;
  wait(1);

  // The beam's sheet for the kind.
  if (kind == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work->sheet, 1, 1);
  } else if (kind == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamImage, work->sheet, 0, 0);
  } else {
    switch (kind) {
    case 0:
    case 1:
    case 5:
    case 6:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                                 1);
      break;
    case 2:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                                 1);
      break;
    }
  }
  // An enemy's sparks take the Mars Djinn's colours, a party member's the
  // star crescent's.
  Dma_Set(Resource_GetTableEntry(effect->actor > 7
                                     ? (s32)&ResourceId_MarsDjinnSmallSheet
                                     : (s32)&ResourceId_StarCrescentSheet),
          (void *)BG_PLTT, 0x84000020, REG_DMA3);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer,
                             1, 0);
  // Each transfer leaves half the last frame behind.
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The layer is centred on the first target, kept within 128 pixels.
  REG_BG1CNT = 0x1f81;
  EffectPosition_ProjectSprite(effect->actors[0], &anchor);
  if (effect->side == 0)
    shake = 96 - anchor.x;
  else
    shake = 32 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].x = shake;
  gBgScroll[1].y = 80;
  wait(1);

  // The sparks leave the middle of the target, away from the caster, each
  // living 16 frames more than the last.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &work->particles[i];

    spark->x = target->x;
    spark->y = target->y + half;
    spark->z = target->z;
    spark->velocityX = (Random16() & 255) << 10;
    spark->velocityY = ((Random16() & 255) - 32) << 10;
    spark->velocityZ = ((Random16() & 255) - 127) << 10;
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    // The battle resumes: hit cue 134 for a flagged summon entry, else 133.
    if (frame == 5) {
      if (Summon_IsEntrySecondaryFlaggedFar(unit->enemyId))
        Func_081180e8(134);
      else
        Func_081180e8(133);
    }
    // The target recoils, harder for some kinds.
    if (frame == 4) {
      if (kind == 7)
        Func_08118088(effect->actors[0], 4);
      else if (kind == 5 || kind == 6)
        Func_08118088(effect->actors[0], 1);
      else if (kind == 8)
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0x10000, 0x40000,
                                      0x30000, 150);
      else
        Func_08118088(effect->actors[0], 0);
    }
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    pos.y += 16;
    if (kind == 6)
      pos.y += 16;

    // The beam, from beside the caster towards the target.
    if (kind == 4) {
      // The wave: six cels, two frames each, played backwards.
      if (frame <= 11) {
        if (effect->side == 0)
          draw[0](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  pos.x + shake - 48, pos.y - 8, 48, 16);
        else
          draw[0](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake,
                  pos.y - 8, 48, 16);
      }
    } else if ((u32)kind <= 2 || kind == 5 || kind == 6) {
      // The arc: six cels, two frames each.
      if (frame <= 11) {
        if (effect->side == 0)
          draw[0](canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake - 48,
                  pos.y - 40, 48, 72);
        else
          draw[0](canvas, work->sheet + frame / 2 * 0xd80, pos.x + shake,
                  pos.y - 40, 48, 72);
      }
    } else if (kind == 3) {
      // The strike: six cels, three frames each, each reaching further.
      if (frame <= 17) {
        s32 cel = frame / 3;

        if (effect->side == 0)
          draw[0](canvas, work->sheet + Data_0819886c[cel],
                  pos.x + Data_08198878[cel] + shake - 58,
                  pos.y - Data_08198866[cel] / 2, Data_08198860[cel],
                  Data_08198866[cel]);
        else
          draw[0](canvas, work->sheet + Data_0819886c[cel],
                  pos.x - Data_08198878[cel] + shake - Data_08198860[cel] + 58,
                  pos.y - Data_08198866[cel] / 2, Data_08198860[cel],
                  Data_08198866[cel]);
      }
    }

    // The orb flashes on the target, six cels, two frames each.
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780,
              anchor.x - 16, anchor.y - 24, 40, 48);
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    // From frame 4 the sparks fly, two to each particle, through the two
    // blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Render_ProjectPoint(&spark->x, &point.x);
          point.x += shake;
          size >>= 3;
          size += 2;
          draw[pair & 1](canvas, sheet + Data_08197424[size - 1],
                         point.x - size, point.y - size, size * 2, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  gBgScroll[1].y = 32;
  BattleEffect_HideBg1AndRestoreBackdrop();
}



extern char ResourceId_GreyCrescentSheet;

void Sound_PlayFar(s32 cue);

void BattleCamera_SetViewAtPairMidpointFar(s32 actor, s32 target, s32 frames);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);

void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
// The roll step of the scene transform.
void Func_080150e4(s32 angle);

void Func_0815e9f4(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition anchor;
  struct EffectPosition point;
  struct EffectPosition spot;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 acceleration;
  s32 strength;
  BlitterPair draw;
  DrawRectangle *slots;
  struct MotionObject *object;
  struct MotionObject *target;
  u16 *background;
  s32 words = 0x3c00;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  big = 1;
  if (effect->kind <= 199)
    big = 0;

  // Setup, one step of the view's swing between each load.
  BattleCamera_SetViewAtPairMidpointFar(effect->actor, effect->firstTarget,
                                        130);
  wait(1);
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = 0x1f80;
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(
        HEAP_BLITTER,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    BattleEffect_LoadBlitter(
        HEAP_BLITTER_ALTERNATE,
        BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                        COMPOSITE_CLIP_Y |
                                                        COMPOSITE_FLIP_X));
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                   COMPOSITE_CLIP_Y |
                                                   COMPOSITE_FLIP_X));
  }
  slots = (DrawRectangle *)Ram_HeapSlots;
  draw[0] = slots[HEAP_BLITTER / 4];
  draw[1] = slots[HEAP_BLITTER_ALTERNATE / 4];
  BattleCamera_SetViewAtPairMidpointFar(effect->actor, effect->firstTarget,
                                        130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  BattleCamera_SetViewAtPairMidpointFar(effect->actor, effect->firstTarget,
                                        130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet,
                             Ram_MapCellBuffer, 1, 1);
  // An enemy caster's sparks take the Mars Djinn's colours.
  if (effect->actor > 7)
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
            (void *)BG_PLTT, 0x84000020, REG_DMA3);
  BattleCamera_SetViewAtPairMidpointFar(effect->actor, effect->firstTarget,
                                        130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattleCamera_SetViewAtPairMidpointFar(effect->actor, effect->firstTarget,
                                        130);
  wait(1);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The gathering ring, for the larger kinds: the caster holds still while
  // 64 sparks spin in towards it on random axes.
  if (big == 1) {
    object = GetBattleObjectSlotFar(effect->actor)->object;
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *mote = &work->particles[i];

      mote->x = (Random16() & 63) + 16;
      mote->y = 0;
      mote->z = 0;
      mote->velocityX = Random16() & 0xffff;
      mote->velocityY = Random16() & 0xffff;
      mote->velocityZ = Random16() & 0xffff;
    }
    Actor_SetAnimationSpeedFar(object, 0);
    velocityX = object->velocityX;
    velocityY = object->velocityY;
    velocityZ = object->velocityZ;
    strength = object->verticalMotionStrength;
    acceleration = object->acceleration;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    shake = 64 - pos.x;
    gBgScroll[1].x = shake;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Task_Add(Palette_StepFadeTransfer, 0x480);
    Sound_PlayFar(212);
    for (frame = 0; frame != 32; frame++) {
      BattleCamera_SetViewAtPairMidpointFar(effect->actor, effect->firstTarget,
                                            130);
      // Spark i starts at frame i / 4, 16 to 79 pixels out, and moves in
      // 4 pixels a frame until it reaches the caster.
      for (i = 0; i != SPARK_COUNT; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Render_ResetTransformState();
          Func_080150e4(mote->velocityZ);
          SceneTransform_ApplyPitch(mote->velocityX);
          SceneTransform_ApplyYaw(mote->velocityY);
          EffectPosition_ProjectPoint(&mote->x, &point);
          point.x += 64;
          point.y = point.y + pos.y + 24;
          if (point.depth < -60)
            point.depth = -60;
          if (point.depth > 60)
            point.depth = 60;
          point.depth += 60;
          draw[0](canvas, sheet + Data_08197424[size - 1], point.x - size,
                  point.y - size, size * 2, size * 2);
          mote->x -= 4;
        }
      }
      work->transferPending = 1;
      wait(1);
    }
    Task_Remove(Palette_StepFadeTransfer);
    Actor_SetAnimationSpeedFar(object, 16);
    object->velocityX = velocityX;
    object->velocityY = velocityY;
    object->velocityZ = velocityZ;
    object->acceleration = acceleration;
    object->verticalMotionStrength = strength;
  }

  // The burst: clear both canvases, blend, and centre the layer on the
  // first target, kept within 128 pixels.
  Iwram_ClearWordsEntry(canvas, words);
  Iwram_ClearWordsEntry((void *)0x06004000, words);
  work->transferMode = 2;
  work->transferValue = 75;
  REG_BG1CNT = 0x1f81;
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  if (effect->side == 0)
    shake = 32 - anchor.x;
  else
    shake = 96 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].x = shake;
  gBgScroll[1].y = 80;

  // The sparks leave the middle of the target, away from the caster, each
  // living 16 frames more than the last.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &work->particles[i];

    spark->x = target->x;
    spark->y = target->y + half;
    spark->z = target->z;
    spark->velocityX = (Random16() & 255) << 10;
    spark->velocityY = (Random16() & 255) << 10;
    spark->velocityZ = ((Random16() & 255) - 127) << 10;
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    // The battle resumes, hit cue 134; the target recoils a frame before.
    if (frame == 5)
      Func_081180e8(134);
    if (frame == 4)
      Func_08118088(effect->actors[0], 0);
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    pos.y += 16;
    // The flash: four cels, two frames each.
    if (frame <= 1)
      draw[0](canvas, work->sheet, 0, 0, 120, 120);
    else if (frame <= 3)
      draw[0](canvas, Ram_MapCellBuffer, -4, 0, 128, 128);
    else if (frame <= 5)
      draw[0](canvas, Ram_MapCellBuffer + 0x4000, -4, 0, 128, 128);
    else if (frame <= 7)
      draw[0](canvas, work->sheet + 0x3840, 0, 0, 120, 120);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    // From frame 4 the sparks fly, two to each particle, through the two
    // blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          EffectPosition_ProjectPoint(&spark->x, &spot);
          spot.x += shake;
          size >>= 3;
          size += 2;
          spot.y += 16;
          draw[pair & 1](canvas, sheet + Data_08197424[size - 1], spot.x - size,
                         spot.y - size, size * 2, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Teardown: the background palette steps back over seven frames.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  gBgScroll[1].y = 32;
  for (frame = 0, background = (u16 *)Ram_HeapSlots->battleWork->unk670;
       frame != 7; frame++) {
    BattleBackground_SetPaletteLevelFar(*background, 6 - frame);
    wait(1);
  }
  BattleEffect_HideBg1AndRestoreBackdrop();
}


void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent) {
  struct BattleObj *slot;
  struct MotionObject *object;
  u32 enemy;

  slot = GetBattleObjectSlotFar(id);
  object = slot->object;
  enemy = ((struct BattleUnit *)Unit_Get(id))->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
    if (force != 0 || object->velocityY != 0)
      object->velocityY = velocityY;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, slot->anchorX * percent / 100, 0,
                            slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMode(object, 5);
}
