/// @file
/// Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...).
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// A stat boost brings out a Djinni, which sways from side to side and, after
// 32 frames, rises 2 pixels a frame; then it swirls flares of light in to
// each target in turn, 8 frames apart, and a burst of light and the mode's
// picture (a dagger, a shield, a streak or a wisp) play over it.

/// Gathers stat-boost light and displays a dagger in Mars colours.
///
/// Abilities: Impact, High Impact, Demon Spear, Angel Spear (kind 26); Forge
/// (kind 210); kind 46 is used by no ability.
void BattleFx_RunImpact(struct BattleEffectArgument *effect) {
  BattleFx_RunStatBoost(effect, 0);
}
/// Gathers stat-boost light and displays a shield in Mars colours.
///
/// Abilities: Guard, Protect, Guardian, Protector (kind 28); Corona (kind 211).
void BattleFx_RunGuard(struct BattleEffectArgument *effect) {
  BattleFx_RunStatBoost(effect, 1);
}
/// Gathers stat-boost light and displays a shield in Jupiter colours.
///
/// Abilities: Ward, Resist, Magic Shell, Magic Shield (kind 30); Breeze (kind
/// 215).
void BattleFx_RunWard(struct BattleEffectArgument *effect) {
  BattleFx_RunStatBoost(effect, 2);
}
/// Gathers stat-boost light and displays a violet streak in Venus colours.
///
/// Abilities: Granite (kind 200).
void BattleFx_RunGranite(struct BattleEffectArgument *effect) {
  BattleFx_RunStatBoost(effect, 3);
}
/// Gathers stat-boost light and displays a violet streak in Mars colours.
///
/// Abilities: Flash (kind 213).
void BattleFx_RunFlash(struct BattleEffectArgument *effect) {
  BattleFx_RunStatBoost(effect, 5);
}
/// Gathers stat-boost light and displays a wisp without replacing its palette.
///
/// Abilities: Zephyr (kind 216).
void BattleFx_RunZephyr(struct BattleEffectArgument *effect) {
  BattleFx_RunStatBoost(effect, 4);
}
/// Where each picture sits in the sheet.
enum StatBoostSheet {
  STAT_BOOST_FLARES = 0x0000,  ///< flares (ParticleSpritesD)
  STAT_BOOST_BURST = 0x060e,   ///< six 40x40 bursts of light (GlowOrb)
  STAT_BOOST_PICTURE = 0x2b8e, ///< the picture the mode picks
  STAT_BOOST_DJINNI = 0x65c0   ///< the 40x40 djinni
};

#define STAT_BOOST_SCATTERED 512 ///< flares scattered at the start
#define STAT_BOOST_DRAWN 32      ///< flares drawn for each target
#define STAT_BOOST_SLOTS 64      ///< particles reserved for each target

/// Flares of light swirl in to the centre of each target, one target every
/// 8 frames, burst, and the mode's picture plays. Target n's flares appear
/// from frame 8n + 1; it glows white on 8n + 48, the burst plays from
/// 8n + 52 and the picture, with a sound, from 8n + 80. The canvas is shown
/// at 1.25 times its width (canvas x is 4/5 of screen x).
/// PARTICLES holds the gathering flares, 64 for each target.
///
/// effect: the targets. With prelude 1, a djinni appears first, sways and
///         rises.
/// mode: the picture: 0 a dagger, 1 and 2 a shield, 3 and 5 a violet
///       streak, 4 a wisp. The palette comes from the Venus djinni in mode
///       3, the Jupiter djinni in mode 2, the Mars djinni in modes 0, 1 and
///       5; mode 4 keeps the palette. The djinni is Venus in mode 3,
///       Jupiter in modes 2 and 4, Mars otherwise.
void BattleFx_RunStatBoost(struct BattleEffectArgument *effect, u32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  s32 member;
  s32 canvasShiftX;
  s32 drawFlags;
  s32 frame;
  s32 i;
  struct EffectPosition base;
  s32 center[3];
  struct EffectPosition screen;
  s32 point[3];
  s32 originX;
  s32 originY;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // Bring out the djinni (Venus is 0, Jupiter 3, Mars 2); origin receives
  // where it appears.
  if (work->effect->prelude == 1) {
    if (mode == 3)
      BattleFx_RunDjinniEntrance(effect, 0, work->effect->side ^ 1, 1, &originX,
                                 &originY);
    else if (mode == 2 || mode == 4)
      BattleFx_RunDjinniEntrance(effect, 3, work->effect->side ^ 1, 1, &originX,
                                 &originY);
    else
      BattleFx_RunDjinniEntrance(effect, 2, work->effect->side ^ 1, 1, &originX,
                                 &originY);
    originX = originX * 4 / 5;
  }
  REG_BG2PA = Q_8_8(0.8); // 1.25 times the width

  // Pictures and palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + STAT_BOOST_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + STAT_BOOST_BURST, 1, 1);
  if (mode == 3 || mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
    if (mode == 3)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet),
                      128);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                      128);
  } else if (mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
  } else {
    void *palette;

    if (mode == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    if (mode == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 4)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == 1)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else if (mode == 2 || mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);

  // Scatter the flares around the centre: x and z within 25 pixels, y
  // within 50.
  for (i = 0; i != STAT_BOOST_SCATTERED; i++) {
    struct EffectStep *flare = &PARTICLES[i];

    SET_POSITION(flare, (Random16() % 200 - 100) << 14,
                 (Random16() % 200 - 100) << 15,
                 (Random16() % 200 - 100) << 14);
    flare->variant = 0;
  }

  // With one target, shift the canvas so that target is at its centre
  // (64).
  if (work->effect->count == 1) {
    EffectPosition_ProjectScaledBattler(work->effect->actors[0], &base);
    canvasShiftX = -base.x * 4 / 5 + 64;
  } else {
    if (work->effect->side == 1)
      canvasShiftX = -64;
    else
      canvasShiftX = 0;
  }
  REG_BG2X = canvasShiftX << 8;
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer halves the canvas, for trails
  work->transferValue = 50; // percent
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  if (work->effect->side == 1)
    drawFlags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
  else
    drawFlags = EFFECT_CLIP_BOUNDS;
  AudioCommand_PlayFar(SE_142);

  for (frame = 0; frame != work->effect->count * 8 + 108; frame++) {
    struct BattleCamera *camera = WORK_SLOT(HEAP_SLOT_CAMERA);

    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    // The djinni, facing the other way from the flares, sways from side to
    // side and, after 32 frames, rises 2 pixels a frame. For the first 4
    // frames it is also added on top.
    if (work->effect->prelude == 1) {
      s32 x =
          ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags ^ COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
      draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          drawFlags ^ COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
      draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }

    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(work->effect->actors[member])->object;
      s32 height =
          BattleUnit_GetEffectHeightFar(work->effect->actors[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        AudioCommand_PlayFar(SE_212);
      EFFECT_VIEW_FROM_CAMERA(camera);
      // Centre on two thirds of the target's height.
      SET_VECTOR(point, object->x, height, object->z);
      SceneTransform_ApplyPosition(point);
      if (frame == member * 8 + 48)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 16);

      // Turn the flares and move them 2 pixels a frame towards the centre;
      // flare i appears i frames after the target's turn starts. Mode 0
      // turns them by pitch, mode 1 not at all, mode 2 by yaw, the others
      // by yaw and roll.
      if (frame > member * 8) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
        draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, drawFlags,
                            COMPOSITE_ADDITIVE);
        draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        if (mode == 0) {
          SceneTransform_ApplyPitch(-frame << 10);
        } else if (mode == 1) {
        } else if (mode == 2) {
          SceneTransform_ApplyYaw(frame << 10);
        } else {
          SceneTransform_ApplyYaw(frame << 10);
          SceneTransform_ApplyRoll(frame << 10);
        }
        for (i = 0; i != STAT_BOOST_DRAWN; i++) {
          struct EffectStep *flare = &PARTICLES[member * STAT_BOOST_SLOTS + i];

          if (frame > member * 8 + i) {
            s32 xx = (flare->x >> 8) * (flare->x >> 8);
            s32 yy = (flare->y >> 8) * (flare->y >> 8);
            s32 zz = (flare->z >> 8) * (flare->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              // The size (half the width) runs 6 to 1 over depths 314 to 634.
              EffectPosition_ProjectPoint(flare, &screen);
              screen.x = screen.x * 4 / 5 + canvasShiftX;
              CLAMP(screen.depth, 314, 634);
              size = 6 - (screen.depth - 314) / 64;
              EFFECT_DRAW_FLARE(draw[1], canvas, work->sheet, screen.x,
                                screen.y, size);
              flare->x -= flare->x / distance;
              flare->y -= flare->y / distance;
              flare->z -= flare->z / distance;
            }
          }
        }
        EFFECT_RELEASE_BLITTERS();
      }

      // The centre's position on screen.
      SET_VECTOR(center, 0, 0, 0);
      EffectPosition_ProjectPoint(center, &screen);
      screen.x = screen.x * 4 / 5 + canvasShiftX;

      // The burst: six frames of 4 frames each, from frame 52.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cell = (frame - member * 8 - 52) / 4 % 6;

        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
        draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
        draw[0](canvas, work->sheet + cell * (40 * 40) + STAT_BOOST_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
      }

      // The mode's picture, from frame 80.
      if (mode == 0) {
        // Seven 24x40 frames, 4 frames each.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cell = (frame - member * 8 - 80) / 4 % 7;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          draw[0](canvas, work->sheet + cell * (24 * 40) + STAT_BOOST_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      } else if (mode == 3 || mode == 5) {
        // Six 32x64 frames, 4 frames each.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cell = (frame - member * 8 - 80) / 4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          draw[0](canvas, work->sheet + cell * (32 * 64) + STAT_BOOST_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      } else if (mode == 4) {
        // 64x32 frames, 2 frames each, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cell = (frame - member * 8 - 80) / 2 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_MAX);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              drawFlags | COMPOSITE_FLIP_Y, COMPOSITE_MAX);
          draw[1] = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
          draw[0](canvas, work->sheet + cell * (64 * 32) + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, work->sheet + cell * (64 * 32) + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y + 8, 64, 32);
          EFFECT_RELEASE_BLITTERS();
        }
      } else {
        // Six 40x40 frames, 4 frames each, added.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cell = (frame - member * 8 - 80) / 4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, drawFlags, COMPOSITE_ADDITIVE);
          draw[0] = gWorkSlot[HEAP_SLOT_BLITTER];
          draw[0](canvas, work->sheet + cell * (40 * 40) + STAT_BOOST_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Stat-reduction shards, flame arcs, rock pillars and erupting particles.
// Named table fields describe the counts and timing values used by each
// family's variant rows.
//
// A stat reduction shows the dagger or the shield over each target, 4 frames
// apart, and breaks it into 24 shards that fly off and darken through seven
// dimmed copies of the shard picture. Its shards live in PARTICLES, 64 for
// each target, and are drawn with trails: each transfer of the canvas fades
// or darkens what is already there.
//
// The scenes after it each throw a pattern of particles and play it out over
// a fixed number of frames:
//
// - Flare raises nine flames one after another along a half circle,
//   optionally stacked two or three high.
// - Quake raises three rock pillars that throw the targets into the air.
// - Fire's homing embers fly out of the user, hang, and then home in on the
//   targets, striking with a flare where they reach the ground.
// - Frost's rising motes spiral up around the target while ice columns spring
//   up, then burst: the motes spread and fall, chips fly and smoke rises over
//   the heights they span.
// - The blast (Blast, Smoke Bomb) bursts a fountain of motes and puffs of
//   smoke up beside the targets, which fall back under gravity.
//
// Each scene takes over the canvas layer, loads its sheet, seeds its
// particles and then runs its frames, each ending with a canvas present. A
// free or waiting particle is usually marked by a negative variant, which
// otherwise counts the frames of its animation. effect->variant picks each
// scene's counts from its timing table. Several steps consume random numbers,
// so their order within a frame is part of the effect.

/// Breaks the dagger picture into shards that fade through darker copies.
///
/// Abilities: Dull, Blunt (kind 27).
void BattleFx_RunDull(struct BattleEffectArgument *effect) {
  BattleFx_RunStatReduction(effect, 0);
}
/// A shield appears over each target and breaks into shards that fade through
/// darker copies, in the glow orb's colours (style 1).
///
/// Abilities: Impair, Debilitate (kind 29); kind 47 is used by no ability.
void BattleFx_RunImpair(struct BattleEffectArgument *effect) {
  BattleFx_RunStatReduction(effect, 1);
}
/// A shield appears over each target and breaks into fading shards, in the pink
/// burst's colours (style 2).
///
/// Abilities: Weaken, Enfeeble (kind 31).
void BattleFx_RunWeaken(struct BattleEffectArgument *effect) {
  BattleFx_RunStatReduction(effect, 2);
}
/// Breaks the shield picture into fading shards with style 3, which plays as
/// style 2 does. No effect table entry reaches it.
UNUSED void
BattleFx_RunStatReductionMode3(struct BattleEffectArgument *effect) {
  BattleFx_RunStatReduction(effect, 3);
}
// The shard picture in the sheet, followed by seven copies of it, each
// with its colour levels capped 7 lower than the one before.
#define STAT_REDUCTION_SHARD_CELLS 0x2580
#define STAT_REDUCTION_SHARD_BYTES 696 ///< bytes in each copy
#define STAT_REDUCTION_SHARDS 24       ///< shards drawn for each target
#define STAT_REDUCTION_SLOTS 32        ///< particles reserved for each target

/// Stat cuts: a dagger (mode 0) or a shield (other modes) appears over each
/// target, 4 frames apart, and then breaks into 24 shards that fly off and
/// fade out through the darker copies. In each target's own frames the
/// picture plays until about frame 24, then breaks with a sound; the shards
/// darken from frame 29 and are gone after frame 60.
///
/// effect: the targets.
/// mode: 0 the dagger and the pink burst palette, 1 the shield and the
///       glow orb palette, 2 and 3 the shield and the pink burst palette.
void BattleFx_RunStatReduction(struct BattleEffectArgument *effect, s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct EffectPosition screen;
  s32 point[3];
  BlitterPair blitters;
  s32 i;
  s32 paletteId;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == 0)
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    paletteId = (s32)&ResourceId_GlowOrbSheet;
  else
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                             work->sheet + STAT_REDUCTION_SHARD_CELLS, 0, 0);

  // Build the seven darker copies of the shard picture.
  for (i = 1; i != 8; i++) {
    for (j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
      s32 level = work->sheet[STAT_REDUCTION_SHARD_CELLS + j];

      EFFECT_CAP_LEVEL(level, 64 - i * 7);
      work->sheet[STAT_REDUCTION_SHARD_CELLS + i * STAT_REDUCTION_SHARD_BYTES +
                  j] = level;
    }
  }

  // On side 1 the canvas moves 112 pixels right, so everything is drawn
  // 112 pixels further left on it.
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards start in a grid of eight rows and four columns and fly
  // away from the user's side, scattering in a random direction.
  for (i = 0; i != 512; i++) {
    struct EffectStep *shard = &PARTICLES[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == 0) {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 2 - 2) << 16;
    } else {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 8 - 16) << 16;
    }
    if (work->effect->side == 1)
      shard->velocityX = 0x20000;
    else
      shard->velocityX = -0x20000;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleFx_FetchRectangleBlitters(work->effect->side, draw = blitters);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  for (frame = 0; frame != work->effect->count * 4 + 64; frame++) {
    camera = gCameraWork;
    if (frame == 72)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(j, work->effect) {
      struct MotionObject *object;
      s32 targetFrame;
      s32 x;
      s32 y;

      targetFrame = frame - j * 4;
      object = GetBattleObjectSlotFar(work->effect->actors[j])->object;
      if (targetFrame > 0) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        // Centre on the point 20 pixels above the target; the view is set
        // from the camera twice.
        SET_VECTOR(point, object->x, 160 << 13, object->z);
        EFFECT_VIEW_FROM_CAMERA(camera);
        SceneTransform_ApplyPosition(point);
        SET_VECTOR(origin, 0, 0, 0);
        EffectPosition_ProjectPoint(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == 0) {
          if (targetFrame <= 26)
            blitters[0](canvas, work->sheet + targetFrame / 4 % 7 * (24 * 40),
                        x - 12, y - 20, 24, 40);
        } else {
          if (targetFrame <= 23)
            draw[1](canvas, work->sheet + targetFrame / 4 % 6 * (40 * 40),
                    x - 20, y - 20, 40, 40);
        }
        if (targetFrame == 24)
          AudioCommand_PlayFar(SE_143);
        // From frame 24 the picture breaks; the shards darken one copy
        // every 4 frames after frame 28.
        if (targetFrame >= 24 && targetFrame <= 60) {
          s32 level;

          level = 0;
          if (targetFrame > 28) {
            level = (targetFrame - 24) / 4;
            if (level > 7)
              level = 7;
          }
          for (i = 0; i != STAT_REDUCTION_SHARDS; i++) {
            struct EffectStep *shard = &PARTICLES[j * STAT_REDUCTION_SLOTS + i];
            s32 cell;

            // Shard i takes shape i % 4, turning through its three frames
            // every 8 frames from a random start.
            cell = i % 4 * 3 + (shard->variant + targetFrame) / 8 % 3;
            EffectPosition_ProjectPoint(shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            blitters[0](canvas,
                        work->sheet +
                            (level * STAT_REDUCTION_SHARD_BYTES +
                             gStatChange_ShardCells[cell]) +
                            STAT_REDUCTION_SHARD_CELLS,
                        x, y, gStatChange_ShardWidths[cell],
                        gStatChange_ShardHeights[cell]);
            EffectStep_AdvanceWithGravity3D(shard, 60, 0);
          }
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef STAT_BOOST_SCATTERED
#undef STAT_BOOST_DRAWN
#undef STAT_BOOST_SLOTS
#undef STAT_REDUCTION_SHARD_CELLS
#undef STAT_REDUCTION_SHARD_BYTES
#undef STAT_REDUCTION_SHARDS
#undef STAT_REDUCTION_SLOTS
