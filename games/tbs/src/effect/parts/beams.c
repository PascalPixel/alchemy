/// @file
/// Beam sequences and the ring and spark scene.
// Part of effect.c.

#include "../effect_internal.h"

/// The beam sequence's pictures, by effect kind: the kinds below 100 that
/// BattleAnim_DispatchByIdRange sends here. No ability's row selects one; the
/// unused BattlePresentation_RunPlainAttack plays an attack's kind, 0 for
/// Attack.
enum BeamKind {
  BEAM_PLAIN_ATTACK,  ///< blue arc A: Attack's kind
  BEAM_ARC_A,         ///< blue arc A
  BEAM_ARC_B,         ///< blue arc B
  BEAM_IMAGE,         ///< the beam sequence image, growing
  BEAM_WAVE,          ///< the wave, played backwards
  BEAM_ARC_A_INVERTED ///< blue arc A, upside down
};

/// Draws the selected beam and its projected sparks over the first target: a
/// beam, arc or wave shoots from the user; from frame 4 the target is hit,
/// a yellow orb flashes over it and sparks fly off, for 32 frames in all.
/// effect->kind picks the picture.
///
/// Reached through BattleAnim_DispatchByIdRange for kinds 136 to 199 and,
/// through its unsigned test, every kind below 100;
/// BattlePresentation_RunPlainAttack (battle/staging.c) sends every plain
/// attack there. Only the kinds of enum BeamKind have pictures. Shows the
/// canvas on BG1 (BattleAnim_SetupCanvasTileMap), loads palette colours 0 to
/// 63, scrolls BG1 through gBgScroll[1] and uses work->particles 0 to 63 (only
/// 0 to 31 fly) and both blitter slots; at the end it removes the transfer
/// callback, frees the blitters, sets gBgScroll[1].y back to 32 and hides BG1
/// again (BattleAnim_HideBg1AndRestoreBackdrop).
void BattleAnim_RunBeamSequence(struct BattleAnimArgs *effect) {
  struct ScreenPoint screenPosition;
  struct ScreenPoint anchor;
  struct ScreenPoint point;
  void **heapCache;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 scrollX;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 kind;
  struct Unit *unit;
  BlitterPair draw;
  DrawRectangle *slots;
  struct MotionObject *target;
  void *colors = (void *)BG_PLTT;
  s32 half;
  s32 frame;
  s32 i;

  heapCache = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  cursor = heapCache;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = ANIM_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = ANIM_SLOT(heapCache, HEAP_SLOT_CAMERA);
  kind = effect->kind;
  work->effect = effect;
  unit = Unit_GetFar(effect->user);
  WaitFrames(1);
  BattleAnim_ConfigureEffectDisplay();
  BattleAnim_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  WaitFrames(1);
  // Two blitters, additive and brightest-wins; BEAM_ARC_A_INVERTED draws its
  // pictures upside down, and side 1 mirrors them.
  if (kind == BEAM_ARC_A_INVERTED) {
    if (work->effect->side == 0) {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        COMPOSITE_ADDITIVE);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
    } else {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                        COMPOSITE_ADDITIVE);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                        BLITTER_MAX);
    }
  } else {
    if (work->effect->side == 0) {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, BLITTER_MAX);
    } else {
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    }
  }
  slots = (DrawRectangle *)gHeapSlots;
  draw[0] = slots[HEAP_SLOT_RAM_CODE];
  draw[1] = slots[HEAP_SLOT_RAM_CODE_2];
  WaitFrames(1);
  // The picture, by BeamKind. An enemy user (unit id above 7) takes the small
  // Mars Djinni's palette, a party member the crescent's. The sparks and the
  // yellow orb follow.
  if (kind == BEAM_WAVE) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work->sheet, 1, 1);
  } else if (kind == BEAM_IMAGE) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work->sheet,
                               0, 0);
  } else {
    switch (kind) {
    case BEAM_PLAIN_ATTACK:
    case BEAM_ARC_A:
    case BEAM_ARC_A_INVERTED:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work->sheet, 1,
                                 1);
      break;
    case BEAM_ARC_B:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1,
                                 1);
      break;
    }
  }
  if (work->effect->user > 7)
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
        128);
  else
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_CrescentSheet), 128);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  // Scroll BG1 so the first target falls at canvas x 96 on side 0 and 32 on
  // side 1, as far as a scroll held between -128 and 0 allows; anchor
  // becomes the target's position on the canvas.
  Projection_Sprite(work->effect->targets[0], &anchor);
  if (work->effect->side == 0)
    scrollX = 96 - anchor.x;
  else
    scrollX = 32 - anchor.x;
  if (scrollX > 0)
    scrollX = 0;
  if (scrollX < -128)
    scrollX = -128;
  anchor.x += scrollX;
  gBgScroll[1].x = scrollX;
  gBgScroll[1].y = 80;
  WaitFrames(1);
  // 64 sparks start at the first target's middle and fly away from the
  // centre of the field at random speeds, mostly rising; variant is a
  // spark's life in moves, 16 to 79, so the later sparks are larger and last
  // longer. Only the first 32 fly (see the frame loop).
  target = GetBattleObjectSlotFar(work->effect->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(work->effect->targets[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct Particle *spark = &work->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
    SET_VELOCITY(spark, (Random16() & 255) << 10,
                 ((Random16() & 255) - 32) << 10,
                 ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  // 32 frames: the picture plays from frame 0, the hit lands on frame 4.
  for (frame = 0; frame != 32; frame++) {
    // On frame 5 the battle shows the damage, with the heavy impact if the
    // user's class is flagged in the summon table and SE_IMPACT otherwise. On
    // frame 4 the target takes KNOCKBACK_NUDGE.
    if (frame == 5) {
      if (BattleSprite_HasHeavyImpactFar(unit->id))
        ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        ANIM_SHOW_DAMAGE(SE_IMPACT);
    }
    if (frame == 4)
      BattleMotion_ApplyKnockbackFar(work->effect->targets[0], KNOCKBACK_NUDGE);
    // The picture plays from the user, to its left on side 0 and its right
    // on side 1: the wave's six 48x16 cells backwards and the arc's six
    // 48x72 cells, two frames each; the beam image grows through its six
    // cells, three frames each.
    Projection_ScaledUnit(work->effect->user, &screenPosition);
    screenPosition.y += 16;
    if (kind == BEAM_WAVE) {
      if (frame <= 11) {
        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + (5 - frame / 2) * (48 * 16),
                  screenPosition.x + scrollX - 48, screenPosition.y - 8, 48,
                  16);
        else
          draw[1](canvas, work->sheet + (5 - frame / 2) * (48 * 16),
                  screenPosition.x + scrollX, screenPosition.y - 8, 48, 16);
      }
    } else if ((u32)kind <= BEAM_ARC_B || kind == BEAM_ARC_A_INVERTED) {
      if (frame <= 11) {
        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + frame / 2 * (48 * 72),
                  screenPosition.x + scrollX - 48, screenPosition.y - 40, 48,
                  72);
        else
          draw[1](canvas, work->sheet + frame / 2 * (48 * 72),
                  screenPosition.x + scrollX, screenPosition.y - 40, 48, 72);
      }
    } else {
      if (frame <= 17) {
        s32 cell = frame / 3;

        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + gBeam_Cells[cell],
                  screenPosition.x + gBeam_CellReaches[cell] + scrollX - 58,
                  screenPosition.y - gBeam_CellHeights[cell] / 2,
                  gBeam_CellWidths[cell], gBeam_CellHeights[cell]);
        else
          draw[1](canvas, work->sheet + gBeam_Cells[cell],
                  screenPosition.x - gBeam_CellReaches[cell] + scrollX -
                      gBeam_CellWidths[cell] + 58,
                  screenPosition.y - gBeam_CellHeights[cell] / 2,
                  gBeam_CellWidths[cell], gBeam_CellHeights[cell]);
      }
    }
    // Frames 4 to 15: the yellow orb flashes over the target, six 40x48
    // cells, two frames each.
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapGridCellBuffer + (frame - 4) / 2 * (40 * 48),
              anchor.x - 16, anchor.y - 24, 40, 48);
    ANIM_VIEW_FROM_CAMERA(camera);
    // From frame 4 the sparks fly. Only the first 32 move: each is drawn
    // and moved twice a frame, so it flies at double speed and its life
    // runs out twice as fast. Even ones add, odd ones take the brighter
    // pixel.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Render_ProjectPoint(&spark->x, ScreenPoint_Coordinates(&point));
          point.x += scrollX;
          size >>= 3;
          size += 2;
          ANIM_DRAW_FLARE(draw[pair & 1], canvas, sheet, point.x, point.y,
                          size);
          Particle_AdvanceWithGravity3D(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    ANIM_PRESENT_FRAME(work);
  }

  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  ANIM_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  BattleAnim_HideBg1AndRestoreBackdrop();
}

/// Plays the selected ring-and-spark strike, restoring the user motion after
/// it. Kinds above 199 first gather 64 motes of light into the user for 32
/// frames while it stands still; then star burst and crescent rings flash
/// over the canvas and sparks fly from the first target for 32 frames, and
/// the background palette fades back in.
///
/// Reached through BattleAnim_DispatchByIdRange for kinds 200 and up, which
/// BattleStaging_RunAttack (battle/staging.c) makes by adding 200 to the
/// kind of a critical hit; so the gathering always plays. Shows the canvas on
/// BG1, keeps the battle's transition centred between the user and the first
/// target while it loads, scrolls BG1 through gBgScroll[1] and uses
/// work->particles 0 to 63, work->imageStorage, Ram_MapGridCellBuffer and both
/// blitter slots. While the motes gather it adds Palette_StepFadeTransfer and
/// stops the user's motion, and restores both after. At the end it removes
/// the transfer callback, frees the blitters, sets gBgScroll[1].y back to 32,
/// steps the background palette level from 6 back to 0 and hides BG1 again.
void BattleAnim_RunRingAndSparkSequence(struct BattleAnimArgs *effect) {
  struct ScreenPoint screenPosition;
  struct ScreenPoint anchor;
  struct ScreenPoint point;
  struct ScreenPoint spot;
  void **heapCache;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 scrollX;
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
  void *colors = (void *)BG_PLTT;
  s32 canvasBytes = CANVAS_BYTES;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  heapCache = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  cursor = heapCache;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = ANIM_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = ANIM_SLOT(heapCache, HEAP_SLOT_CAMERA);
  // DEAD: the test is never true, as only kinds 200 and up reach this
  // runner (BattleAnim_DispatchByIdRange), so big stays 1; the ROM keeps it.
  big = 1;
  if (effect->kind <= 199)
    big = 0;
  work->effect = effect;
  // Load the blitters and pictures a step a frame, keeping the transition
  // centred between the user and the first target meanwhile. The star
  // burst goes to the image storage and the crescent to the map grid cell
  // buffer; an enemy user (unit id above 7) also takes the small Mars
  // Djinni's palette.
  BattleCamera_SetViewAtPairMidpointFar(effect->user, effect->firstTarget, 130);
  WaitFrames(1);
  BattleAnim_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  if (work->effect->side == 0) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_MAX);
  }
  slots = (DrawRectangle *)gHeapSlots;
  draw[0] = slots[HEAP_SLOT_RAM_CODE];
  draw[1] = slots[HEAP_SLOT_RAM_CODE_2];
  BattleCamera_SetViewAtPairMidpointFar(work->effect->user,
                                        work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet,
                             work->imageStorage, 1, 0);
  BattleCamera_SetViewAtPairMidpointFar(work->effect->user,
                                        work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet,
                             Ram_MapGridCellBuffer, 1, 1);
  if (work->effect->user > 7)
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
        128);
  BattleCamera_SetViewAtPairMidpointFar(work->effect->user,
                                        work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattleCamera_SetViewAtPairMidpointFar(work->effect->user,
                                        work->effect->firstTarget, 130);
  WaitFrames(1);
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The gathering: 64 motes, each in its own random rotation around the
  // user, close in 4 a frame, mote i from frame i / 4, while the palette
  // fades over 24 frames. The user is held still meanwhile: its
  // velocities, acceleration and vertical strength are saved, zeroed and
  // restored after.
  if (big == 1) {
    object = GetBattleObjectSlotFar(work->effect->user)->object;
    for (i = 0; i != 64; i++) {
      struct Particle *mote = &work->particles[i];

      SET_POSITION(mote, (Random16() & 63) + 16, 0, 0);
      SET_VELOCITY(mote, Random16() & 0xffff, Random16() & 0xffff,
                   Random16() & 0xffff);
    }
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 0);
    velocityX = object->velocityX;
    velocityY = object->velocityY;
    velocityZ = object->velocityZ;
    strength = object->verticalMotionStrength;
    acceleration = object->acceleration;
    SET_VELOCITY(object, 0, 0, 0);
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    Projection_ScaledUnit(work->effect->user, &screenPosition);
    // BG1 scrolls so the user stands at canvas x 64, where the motes are
    // centred.
    scrollX = 64 - screenPosition.x;
    gBgScroll[1].x = scrollX;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Task_Add(Palette_StepFadeTransfer, ANIM_PLAYBACK_ORDER);
    Sound_PlaySongFar(SE_212);
    for (frame = 0; frame != 32; frame++) {
      BattleCamera_SetViewAtPairMidpointFar(work->effect->user,
                                            work->effect->firstTarget, 130);
      for (i = 0; i != 64; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Render_ResetTransformState();
          Transform_ApplyRoll(mote->velocityZ);
          Transform_ApplyPitch(mote->velocityX);
          Transform_ApplyYaw(mote->velocityY);
          Projection_Point(&mote->x, &point);
          point.x += 64;
          point.y = point.y + screenPosition.y + 24;
          // DEAD: the depth is clamped to -60..60 and moved to 0..120 but
          // never read; the ROM keeps the clamp.
          CLAMP(point.depth, -60, 60);
          point.depth += 60;
          ANIM_DRAW_FLARE(draw[0], canvas, sheet, point.x, point.y, size);
          mote->x -= 4;
        }
      }
      ANIM_PRESENT_FRAME(work);
    }
    Task_Remove(Palette_StepFadeTransfer);
    ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object, 16);
    SET_VELOCITY(object, velocityX, velocityY, velocityZ);
    object->acceleration = acceleration;
    object->verticalMotionStrength = strength;
  }

  // The strike: clear the canvas, scroll BG1 so the first target is on
  // screen, and start 64 sparks at its middle, rising and flying towards the
  // centre of the field (unlike the beam sequence's, whose x speed is
  // turned around once more).
  Iwram_ClearWords(canvas, canvasBytes);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), canvasBytes);
  ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  Projection_ScaledUnit(work->effect->targets[0], &anchor);
  if (work->effect->side == 0)
    scrollX = 32 - anchor.x;
  else
    scrollX = 96 - anchor.x;
  if (scrollX > 0)
    scrollX = 0;
  if (scrollX < -128)
    scrollX = -128;
  anchor.x += scrollX;
  gBgScroll[1].y = 80;
  gBgScroll[1].x = scrollX;
  target = GetBattleObjectSlotFar(work->effect->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(work->effect->targets[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct Particle *spark = &work->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
    SET_VELOCITY(spark, (Random16() & 255) << 10, (Random16() & 255) << 10,
                 ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    if (frame == 5)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 4)
      BattleMotion_ApplyKnockbackFar(work->effect->targets[0], KNOCKBACK_NUDGE);
    Projection_ScaledUnit(work->effect->user, &screenPosition);
    screenPosition.y += 16;
    // The rings: two frames each of the two star burst cells, then of the
    // two crescent cells.
    if (frame <= 1)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 3)
      draw[0](canvas, work->imageStorage + (120 * 120), 0, 0, 120, 120);
    else if (frame <= 5)
      draw[0](canvas, Ram_MapGridCellBuffer, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, Ram_MapGridCellBuffer + (120 * 120), 0, 0, 120, 120);
    ANIM_VIEW_FROM_CAMERA(camera);
    // From frame 4 the sparks fly, as in the beam sequence: only the first
    // 32 move, each drawn and moved twice a frame.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Projection_Point(&spark->x, &spot);
          spot.x += scrollX;
          size >>= 3;
          size += 2;
          spot.y += 16;
          ANIM_DRAW_FLARE(draw[pair & 1], canvas, sheet, spot.x, spot.y, size);
          Particle_AdvanceWithGravity3D(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    ANIM_PRESENT_FRAME(work);
  }

  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  ANIM_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  // Fade the background palette back in over 7 frames.
  for (frame = 0, background = &gBattleWork->background; frame != 7; frame++) {
    BattleBackground_SetPaletteLevelFar(*background, 6 - frame);
    WaitFrames(1);
  }
  BattleAnim_HideBg1AndRestoreBackdrop();
}
