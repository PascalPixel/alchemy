/// @file
/// The plain attack's slash, and the critical hit's gathering light and
/// crescent.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The plain attack's pictures, by effect kind: the attacker's attack effect,
/// from its class's gBattleSprites row for an enemy, 1 for Isaac, Garet and
/// Jenna and 4 for Ivan and Mia (BattleActionRun_Build).
enum BeamKind {
  BEAM_PLAIN_ATTACK,  ///< drawn as BEAM_ARC_A; no attack has kind 0
  BEAM_ARC_A,         ///< one blue slash: Isaac's, Garet's and Jenna's
  BEAM_ARC_B,         ///< three blue slashes side by side
  BEAM_IMAGE,         ///< a streak of light, growing
  BEAM_WAVE,          ///< three short blue arcs, backwards: Ivan's and Mia's
  BEAM_ARC_A_INVERTED ///< one blue slash, upside down
};

/// A plain attack's picture: the slash or streak args->kind picks plays from
/// the user over its first target; from frame 4 the target is hit, a yellow orb
/// flashes over it and sparks fly off, for 32 frames in all.
void BattleEffect_RunAttackSlash(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct ScreenPoint anchor;
  struct ScreenPoint screen;
  struct BattleEffectWork *work;
  void *canvas;
  s32 scrollX;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 kind;
  struct Unit *unit;
  BlitterPair draw;
  struct Actor *target;
  s32 half;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  kind = args->kind;
  work->args = args;
  unit = Unit_GetFar(args->user);
  wait(1);
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  wait(1);
  // Two blitters, additive and brightest-wins; BEAM_ARC_A_INVERTED draws its
  // pictures upside down, and EFFECT_SIDE_PARTY mirrors them.
  if (kind == BEAM_ARC_A_INVERTED) {
    if (work->args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
    }
  } else {
    if (work->args->side == EFFECT_SIDE_ENEMIES) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
    }
  }
  {
    DrawRectangle *blitters = EFFECT_BLITTERS;

    draw[0] = blitters[HEAP_SLOT_RAM_CODE];
    draw[1] = blitters[HEAP_SLOT_RAM_CODE_2];
  }
  wait(1);
  // The picture, by BeamKind.
  if (kind == BEAM_WAVE) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work->sheet, 1, 1);
  } else if (kind == BEAM_IMAGE) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamImage, work->sheet, 0, 0);
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
  if (work->args->user > PIERS)
    Iwram_CopyWords(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CrescentSheet),
                    128);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  // Scroll BG1 so the first target falls at canvas x 96 on EFFECT_SIDE_ENEMIES
  // and 32 on EFFECT_SIDE_PARTY, as far as a scroll held between -128 and 0
  // allows; anchor becomes the target's position on the canvas.
  Projection_Sprite(work->args->targets[0], &anchor);
  if (work->args->side == EFFECT_SIDE_ENEMIES)
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
  wait(1);
  // 64 sparks start at the first target's middle and fly away from the centre
  // of the field at random speeds, mostly rising.
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  half = BattleUnit_GetEffectHeightFar(work->args->targets[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct Particle *spark = &work->particles[i];

    SET_POSITION(spark, target->x.fixed, target->y.fixed + half,
                 target->z.fixed);
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
    // user's class is flagged in the summon table and SE_IMPACT otherwise.
    if (frame == 5) {
      if (BattleSprite_HasHeavyImpactFar(unit->id))
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    if (frame == 4)
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_NUDGE);
    // The picture plays from the user, to its left on EFFECT_SIDE_ENEMIES and
    // its right on EFFECT_SIDE_PARTY: the wave's six 48x16 pictures backwards
    // and the arc's six 48x72 pictures, two frames each; the beam image grows
    // through its six pictures, three frames each.
    Projection_ScaledUnit(work->args->user, &screenPosition);
    screenPosition.y += 16;
    if (kind == BEAM_WAVE) {
      if (frame <= 11) {
        if (work->args->side == EFFECT_SIDE_ENEMIES)
          draw[1](canvas, work->sheet + (5 - frame / 2) * (48 * 16),
                  screenPosition.x + scrollX - 48, screenPosition.y - 8, 48,
                  16);
        else
          draw[1](canvas, work->sheet + (5 - frame / 2) * (48 * 16),
                  screenPosition.x + scrollX, screenPosition.y - 8, 48, 16);
      }
    } else if ((u32)kind <= BEAM_ARC_B || kind == BEAM_ARC_A_INVERTED) {
      if (frame <= 11) {
        if (work->args->side == EFFECT_SIDE_ENEMIES)
          draw[1](canvas, work->sheet + frame / 2 * (48 * 72),
                  screenPosition.x + scrollX - 48, screenPosition.y - 40, 48,
                  72);
        else
          draw[1](canvas, work->sheet + frame / 2 * (48 * 72),
                  screenPosition.x + scrollX, screenPosition.y - 40, 48, 72);
      }
    } else {
      if (frame <= 17) {
        s32 picture = frame / 3;

        if (work->args->side == EFFECT_SIDE_ENEMIES)
          draw[1](canvas, work->sheet + gBeam_Pictures[picture],
                  screenPosition.x + gBeam_PictureReaches[picture] + scrollX -
                      58,
                  screenPosition.y - gBeam_PictureHeights[picture] / 2,
                  gBeam_PictureWidths[picture], gBeam_PictureHeights[picture]);
        else
          draw[1](canvas, work->sheet + gBeam_Pictures[picture],
                  screenPosition.x - gBeam_PictureReaches[picture] + scrollX -
                      gBeam_PictureWidths[picture] + 58,
                  screenPosition.y - gBeam_PictureHeights[picture] / 2,
                  gBeam_PictureWidths[picture], gBeam_PictureHeights[picture]);
      }
    }
    // Frames 4 to 15: the yellow orb flashes over the target, six 40x48
    // pictures, two frames each.
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapGridCellBuffer + (frame - 4) / 2 * (40 * 48),
              anchor.x - 16, anchor.y - 24, 40, 48);
    EFFECT_VIEW_FROM_CAMERA(camera);
    // From frame 4 the sparks fly.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct Particle *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Transform_ProjectPoint(&spark->x, ScreenPoint_Coordinates(&screen));
          screen.x += scrollX;
          size >>= 3;
          size += 2;
          EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, screen.x, screen.y,
                            size);
          Particle_AdvanceWithGravity3D(spark, 60, -px(1) / 64);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  BattleEffect_HideBg1AndRestoreBackdrop();
}

/// A critical hit ("A critical hit!", MsgCritical): 64 motes of light close in
/// on the attacker for 32 frames while it hangs still in its leap; then a
/// crescent slash sweeps across the first target, sparks fly from it for 32
/// frames, and the background palette fades back in.
void BattleEffect_RunCriticalHit(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  struct ScreenPoint anchor;
  struct ScreenPoint screen;
  struct ScreenPoint spot;
  struct BattleEffectWork *work;
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
  struct Actor *actor;
  struct Actor *target;
  struct BattleWork *battle;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  // DEAD: never true, as only kinds from EFFECT_RANGE_RING_AND_SPARK reach
  // this runner (BattleEffect_RunByIdRange), so big stays 1; the ROM keeps the
  // compare and the store.
  big = 1;
  if (args->kind < EFFECT_RANGE_RING_AND_SPARK)
    big = 0;
  work->args = args;
  // Load the blitters and pictures a step a frame, keeping the transition
  // centred between the user and the first target meanwhile.
  BattleCamera_SetViewAtPairMidpointFar(args->user, args->firstTarget, 130);
  wait(1);
  BattleEffect_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  if (work->args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
  }
  {
    DrawRectangle *blitters = EFFECT_BLITTERS;

    draw[0] = blitters[HEAP_SLOT_RAM_CODE];
    draw[1] = blitters[HEAP_SLOT_RAM_CODE_2];
  }
  BattleCamera_SetViewAtPairMidpointFar(work->args->user,
                                        work->args->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet,
                             work->imageStorage, 1, 0);
  BattleCamera_SetViewAtPairMidpointFar(work->args->user,
                                        work->args->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet,
                             Ram_MapGridCellBuffer, 1, 1);
  if (work->args->user > PIERS)
    Iwram_CopyWords(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
  BattleCamera_SetViewAtPairMidpointFar(work->args->user,
                                        work->args->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattleCamera_SetViewAtPairMidpointFar(work->args->user,
                                        work->args->firstTarget, 130);
  wait(1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The gathering.
  if (big == 1) {
    actor = GetBattleObjectSlotFar(work->args->user)->actor;
    for (i = 0; i != 64; i++) {
      struct Particle *mote = &work->particles[i];

      SET_POSITION(mote, (Random16() & 63) + 16, 0, 0);
      SET_VELOCITY(mote, Random16() & TRIG_ANGLE_MASK,
                   Random16() & TRIG_ANGLE_MASK, Random16() & TRIG_ANGLE_MASK);
    }
    Actor_SetAnimationSpeedFar(actor, 0);
    velocityX = actor->velocityX;
    velocityY = actor->velocityY;
    velocityZ = actor->velocityZ;
    strength = actor->gravity;
    acceleration = actor->acceleration;
    SET_VELOCITY(actor, 0, 0, 0);
    actor->acceleration = 0;
    actor->gravity = 0;
    Projection_ScaledUnit(work->args->user, &screenPosition);
    // BG1 scrolls so the user stands at canvas x 64, where the motes are
    // centred.
    scrollX = 64 - screenPosition.x;
    gBgScroll[1].x = scrollX;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Task_Add(Palette_StepFadeTransfer, TASK_PASS_FRAME);
    Sound_PlayFar(SE_POWER_UP);
    for (frame = 0; frame != 32; frame++) {
      BattleCamera_SetViewAtPairMidpointFar(work->args->user,
                                            work->args->firstTarget, 130);
      for (i = 0; i != 64; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Transform_Reset();
          Transform_ApplyRoll(mote->velocityZ);
          Transform_ApplyPitch(mote->velocityX);
          Transform_ApplyYaw(mote->velocityY);
          Projection_Point(&mote->x, &screen);
          screen.x += 64;
          screen.y = screen.y + screenPosition.y + 24;
          // DEAD: the depth is clamped to -60..60 and moved to 0..120 but
          // never read; the ROM keeps the clamp and the move.
          CLAMP(screen.depth, -60, 60);
          screen.depth += 60;
          EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screen.x, screen.y, size);
          mote->x -= 4;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    }
    Task_Remove(Palette_StepFadeTransfer);
    Actor_SetAnimationSpeedFar(actor, ANIMATION_STEP_NORMAL);
    SET_VELOCITY(actor, velocityX, velocityY, velocityZ);
    actor->acceleration = acceleration;
    actor->gravity = strength;
  }

  // The strike.
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  Projection_ScaledUnit(work->args->targets[0], &anchor);
  if (work->args->side == EFFECT_SIDE_ENEMIES)
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
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  half = BattleUnit_GetEffectHeightFar(work->args->targets[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct Particle *spark = &work->particles[i];

    SET_POSITION(spark, target->x.fixed, target->y.fixed + half,
                 target->z.fixed);
    SET_VELOCITY(spark, (Random16() & 255) << 10, (Random16() & 255) << 10,
                 ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    if (frame == 5)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 4)
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_NUDGE);
    Projection_ScaledUnit(work->args->user, &screenPosition);
    screenPosition.y += 16;
    // The slash: two frames each of the star burst sheet's star and arc, then
    // of the crescent sheet's two crescents.
    if (frame <= 1)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 3)
      draw[0](canvas, work->imageStorage + (120 * 120), 0, 0, 120, 120);
    else if (frame <= 5)
      draw[0](canvas, Ram_MapGridCellBuffer, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, Ram_MapGridCellBuffer + (120 * 120), 0, 0, 120, 120);
    EFFECT_VIEW_FROM_CAMERA(camera);
    // From frame 4 the sparks fly, as in the beam: only the first
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
          EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, spot.x, spot.y,
                            size);
          Particle_AdvanceWithGravity3D(spark, 60, -px(1) / 64);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  // Fade the background palette back in over 7 frames.
  battle = gBattleWork;
  for (frame = 0; frame != 7; frame++) {
    BattleBackground_SetPaletteLevelFar(battle->background, 6 - frame);
    wait(1);
  }
  BattleEffect_HideBg1AndRestoreBackdrop();
}
