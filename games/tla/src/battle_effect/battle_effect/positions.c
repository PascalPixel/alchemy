/// @file
/// Effect positions, projection and id dispatch helpers.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

/// Projects a unit onto the screen, raised by half its height, from the battle
/// camera.
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen) {
  u8 *camera;
  s32 *object;
  u8 *info;
  s32 scale;

  camera = Ram_HeapSlots->cameraWork;
  object = BattleObject_GetFar(unit)->object;
  info = GetMotionRecordFar(object, 0);
  Transform_Reset();
  Transform_SetLookAt(camera, camera + 12);
  scale = Iwram_MulQ16(Render_ProjectPoint(object + 2, screen),
                       *(s32 *)(info + 12));
  screen[1] -= Iwram_MulQ16(scale, BattleUnit_GetEffectHeightFar(unit) >> 17);
  return 0;
}

/// Reads the held keys when the first target is an enemy and does nothing with
/// them.
void BattleEffect_ProbeKeysForExtendedActor(void) {
  struct BattleEffectWork *work;
  struct BattleEffectParams *args;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  args = work->args;

  // DEAD: nothing uses the held keys read here; their load is in the ROM.
  if (args->targets[0] > 0x7f)
    (void)gInput.held;
}

/// Runs a battle effect from the second range of kinds: the casting impacts,
/// the beams and the ring and spark effects.
void BattleEffect_RunByIdRange(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  s32 kind;
  s32 value;

  HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_SHEET, 1550);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  work->args = args;
  work->vblanksSinceTransfer = 1;
  kind = args->kind;
  value = kind - 100;
  if ((u32)value <= 100) {
    BattleEffect_RunCastingImpact(args, value);
  } else if (kind == 210 || kind == 10) {
    BattleEffect_RunCanvasModeByCount(args);
  } else if (kind > 199) {
    BattleEffect_RunCriticalHit(args);
  } else {
    BattleEffect_RunAttackSlash(args);
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// The second blitter slot read as what it holds, a drawing routine.
#define ALTERNATE_BLITTER_SLOT                                                 \
  (((struct {                                                                  \
     u8 before[188];                                                           \
     DrawRectangle draw;                                                       \
   } *)Ram_HeapSlots)                                                          \
       ->draw)

/// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.

#define SPARK_COUNT 64

/// A plain attack: a slash or streak sweeps from the user over its first
/// target, a yellow orb flashes over the target and sparks fly off.
void BattleEffect_RunAttackSlash(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  struct ScreenPoint anchor;
  struct ScreenPoint point;
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

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  kind = args->kind;
  unit = Unit_Get(args->user);

  // Setup: the display, the tile map on BG1, the blitters for the side.
  wait(1);
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  wait(1);
  if (kind == 5 || kind == 6) {
    if (args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
    }
  } else {
    if (args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          BLITTER_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    }
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
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
  Dma_Set(Resource_GetTableEntry(args->user > 7
                                     ? (s32)&ResourceId_MarsDjinnSmallSheet
                                     : (s32)&ResourceId_StarCrescentSheet),
          (void *)BG_PLTT, 0x84000020, REG_DMA3);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer,
                             1, 0);
  // Each transfer leaves half the last frame behind.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // The layer is centred on the first target, kept within 128 pixels.
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  EffectPosition_ProjectSprite(args->targets[0], &anchor);
  if (args->side == EFFECT_SIDE_ENEMIES)
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
  target = BattleObject_GetFar(args->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
  for (i = 0; i != SPARK_COUNT; i++) {
    struct Particle *spark = &work->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
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
      if (BattleSprite_HasHeavyImpactFar(unit->enemyId))
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    // The target recoils, harder for some kinds.
    if (frame == 4) {
      if (kind == 7)
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      else if (kind == 5 || kind == 6)
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HOP);
      else if (kind == 8)
        BattleEffect_StartActorMotion(args->targets[0], 1, 0x10000, 0x40000,
                                      0x30000, 150);
      else
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
    }
    EffectPosition_ProjectScaledBattler(args->user, &pos);
    pos.y += 16;
    if (kind == 6)
      pos.y += 16;

    // The beam, from beside the caster towards the target.
    if (kind == 4) {
      // The wave: six cels, two frames each, played backwards.
      if (frame <= 11) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          draw[0](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  pos.x + shake - 48, pos.y - 8, 48, 16);
        else
          draw[0](canvas, work->sheet + (5 - frame / 2) * 0x300, pos.x + shake,
                  pos.y - 8, 48, 16);
      }
    } else if ((u32)kind <= 2 || kind == 5 || kind == 6) {
      // The arc: six cels, two frames each.
      if (frame <= 11) {
        if (args->side == EFFECT_SIDE_ENEMIES)
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

        if (args->side == EFFECT_SIDE_ENEMIES)
          draw[0](canvas, work->sheet + gBeam_Pictures[cel],
                  pos.x + gBeam_PictureReaches[cel] + shake - 58,
                  pos.y - gBeam_PictureHeights[cel] / 2,
                  gBeam_PictureWidths[cel], gBeam_PictureHeights[cel]);
        else
          draw[0](canvas, work->sheet + gBeam_Pictures[cel],
                  pos.x - gBeam_PictureReaches[cel] + shake -
                      gBeam_PictureWidths[cel] + 58,
                  pos.y - gBeam_PictureHeights[cel] / 2,
                  gBeam_PictureWidths[cel], gBeam_PictureHeights[cel]);
      }
    }

    // The orb flashes on the target, six cels, two frames each.
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780,
              anchor.x - 16, anchor.y - 24, 40, 48);
    Transform_Reset();
    Transform_SetLookAt(camera, camera + 3);
    // From frame 4 the sparks fly, two to each particle, through the two
    // blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Render_ProjectPoint(&spark->x, &point.x);
          point.x += shake;
          size >>= 3;
          size += 2;
          draw[pair & 1](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                         point.x - size, point.y - size, size * 2, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, -px(1) / 64);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  BattleEffect_HideBg1AndRestoreBackdrop();
}

/// A critical hit: motes of light close in on the attacker as it hangs in its
/// leap, then a crescent slash sweeps across the first target and sparks fly
/// from it.
void BattleEffect_RunCriticalHit(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  struct ScreenPoint anchor;
  struct ScreenPoint point;
  struct ScreenPoint spot;
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

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  big = 1;
  if (args->kind <= 199)
    big = 0;

  // Setup, one step of the view's swing between each load.
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
  wait(1);
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  slots = (DrawRectangle *)Ram_HeapSlots;
  draw[0] = slots[HEAP_SLOT_RAM_CODE / 4];
  draw[1] = slots[HEAP_SLOT_RAM_CODE_2 / 4];
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet,
                             Ram_MapCellBuffer, 1, 1);
  // An enemy caster's sparks take the Mars Djinn's colours.
  if (args->user > 7)
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
            (void *)BG_PLTT, 0x84000020, REG_DMA3);
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
  wait(1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The gathering ring, for the larger kinds: the caster holds still while
  // 64 sparks spin in towards it on random axes.
  if (big == 1) {
    object = BattleObject_GetFar(args->user)->object;
    for (i = 0; i != SPARK_COUNT; i++) {
      struct Particle *mote = &work->particles[i];

      SET_POSITION(mote, (Random16() & 63) + 16, 0, 0);
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
    EffectPosition_ProjectScaledBattler(args->user, &pos);
    shake = 64 - pos.x;
    gBgScroll[1].x = shake;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
    Sound_PlayFar(SE_POWER_UP);
    for (frame = 0; frame != 32; frame++) {
      BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
      // Spark i starts at frame i / 4, 16 to 79 pixels out, and moves in
      // 4 pixels a frame until it reaches the caster.
      for (i = 0; i != SPARK_COUNT; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Transform_Reset();
          Transform_ApplyRoll(mote->velocityZ);
          Transform_ApplyPitch(mote->velocityX);
          Transform_ApplyYaw(mote->velocityY);
          EffectPosition_ProjectPoint(&mote->x, &point);
          point.x += 64;
          point.y = point.y + pos.y + 24;
          if (point.depth < -60)
            point.depth = -60;
          if (point.depth > 60)
            point.depth = 60;
          point.depth += 60;
          draw[0](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                  point.x - size, point.y - size, size * 2, size * 2);
          mote->x -= 4;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
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
  Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), words);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  EffectPosition_ProjectScaledBattler(args->targets[0], &anchor);
  if (args->side == EFFECT_SIDE_ENEMIES)
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
  target = BattleObject_GetFar(args->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
  for (i = 0; i != SPARK_COUNT; i++) {
    struct Particle *spark = &work->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
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
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 4)
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
    EffectPosition_ProjectScaledBattler(args->user, &pos);
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
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);
    // From frame 4 the sparks fly, two to each particle, through the two
    // blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          EffectPosition_ProjectPoint(&spark->x, &spot);
          spot.x += shake;
          size >>= 3;
          size += 2;
          spot.y += 16;
          draw[pair & 1](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                         spot.x - size, spot.y - size, size * 2, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, -px(1) / 64);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Teardown: the background palette steps back over seven frames.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  for (frame = 0, background = &Ram_HeapSlots->battleWork->background;
       frame != 7; frame++) {
    BattleBackground_SetPaletteLevelFar(*background, 6 - frame);
    wait(1);
  }
  BattleEffect_HideBg1AndRestoreBackdrop();
}

/// Starts a unit moving away from the centre with its own force, acceleration,
/// top speed, hop and reach in percent, as BattleMotion_ApplyKnockback does
/// from its tables; four enemies (ids 357, 369, 373 and 376) are not moved,
/// only posed.
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent) {
  struct BattleObj *slot;
  struct MotionObject *object;
  u32 enemy;

  slot = BattleObject_GetFar(id);
  object = slot->object;
  enemy = ((struct BattleUnit *)Unit_Get(id))->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    object->acceleration = acceleration;
    object->speedLimit = speedLimit;
    if (force != 0 || object->velocityY != 0)
      object->velocityY = velocityY;
    Actor_StartIdleFar(object);
    Actor_SetMoveTargetFar(object, slot->anchorX * percent / 100, 0,
                           slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SelectAnimationFar(object, 5);
}
