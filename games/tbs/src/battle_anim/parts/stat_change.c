/// @file
/// Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...).
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// The stat boosts' modes (BattleAnim_RunStatBoost), each named for the
/// animation that plays it.
enum StatBoostMode {
  STAT_BOOST_IMPACT,  ///< a dagger, in Mars colours
  STAT_BOOST_GUARD,   ///< a shield, in Mars colours
  STAT_BOOST_WARD,    ///< a shield, in Jupiter colours
  STAT_BOOST_GRANITE, ///< a violet streak, in Venus colours
  STAT_BOOST_ZEPHYR,  ///< a wisp, keeping the palette
  STAT_BOOST_FLASH    ///< a violet streak, in Mars colours
};

/// The stat reductions' modes (BattleAnim_RunStatReduction).
enum StatReductionMode {
  STAT_REDUCTION_DULL,   ///< a dagger, in the pink burst's colours
  STAT_REDUCTION_IMPAIR, ///< a shield, in the glow orb's colours
  STAT_REDUCTION_WEAKEN, ///< a shield, in the pink burst's colours
  STAT_REDUCTION_UNUSED  ///< played as STAT_REDUCTION_WEAKEN; unused
};

/// Light swirls into each target and bursts, then a dagger appears in Mars
/// colours.
///
/// Abilities: Impact, High Impact, Demon Spear, Angel Spear
/// (BATTLE_ANIM_IMPACT); Forge (BATTLE_ANIM_FORGE); none
/// (BATTLE_ANIM_UNUSED_IMPACT).
void BattleAnim_RunImpact(struct BattleAnimArgs *args) {
  BattleAnim_RunStatBoost(args, STAT_BOOST_IMPACT);
}
/// Light swirls into each target and bursts, then a shield appears in Mars
/// colours.
///
/// Abilities: Guard, Protect, Guardian, Protector (BATTLE_ANIM_GUARD); Corona
/// (BATTLE_ANIM_CORONA).
void BattleAnim_RunGuard(struct BattleAnimArgs *args) {
  BattleAnim_RunStatBoost(args, STAT_BOOST_GUARD);
}
/// Light swirls into each target and bursts, then a shield appears in Jupiter
/// colours.
///
/// Abilities: Ward, Resist, Magic Shell, Magic Shield (BATTLE_ANIM_WARD);
/// Breeze (BATTLE_ANIM_BREEZE).
void BattleAnim_RunWard(struct BattleAnimArgs *args) {
  BattleAnim_RunStatBoost(args, STAT_BOOST_WARD);
}
/// Light swirls into each target and bursts, then a violet streak appears in
/// Venus colours.
///
/// Abilities: Granite (BATTLE_ANIM_GRANITE).
void BattleAnim_RunGranite(struct BattleAnimArgs *args) {
  BattleAnim_RunStatBoost(args, STAT_BOOST_GRANITE);
}
/// Light swirls into each target and bursts, then a violet streak appears in
/// Mars colours.
///
/// Abilities: Flash (BATTLE_ANIM_FLASH).
void BattleAnim_RunFlash(struct BattleAnimArgs *args) {
  BattleAnim_RunStatBoost(args, STAT_BOOST_FLASH);
}
/// Light swirls into each target and bursts, then a wisp appears in the colours
/// already loaded, as this is the one boost that keeps the palette.
///
/// Abilities: Zephyr (BATTLE_ANIM_ZEPHYR).
void BattleAnim_RunZephyr(struct BattleAnimArgs *args) {
  BattleAnim_RunStatBoost(args, STAT_BOOST_ZEPHYR);
}
/// Where each picture sits in the sheet.
enum StatBoostSheet {
  STAT_BOOST_FLARES = 0x0000,  ///< flares (ParticleSpritesD)
  STAT_BOOST_BURST = 0x060e,   ///< six 40x40 bursts of light (GlowOrb)
  STAT_BOOST_PICTURE = 0x2b8e, ///< the picture the mode picks
  STAT_BOOST_DJINNI = 0x65c0   ///< the 40x40 Djinni
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
/// Reached through the six wrappers above, Impact to Zephyr. Copies the mode's
/// palette over the first 128 bytes of BG palette RAM (all but Zephyr), writes
/// REG_BG2PA and REG_BG2X, and ends with the transfer task removed and the
/// canvas layer closed; the blitters are built and freed within each frame.
///
/// args: the targets. With prelude 1, a Djinni appears first, sways and
///         rises.
/// mode: the picture and its palette (enum StatBoostMode). The Djinni is
///       Venus for Granite, Jupiter for Ward and Zephyr, Mars otherwise;
///       its sheet lends the palette in every mode but Zephyr.
void BattleAnim_RunStatBoost(struct BattleAnimArgs *args, u32 mode) {
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  BlitterPair draw;
  s32 member;
  s32 canvasShiftX;
  s32 drawFlags;
  s32 frame;
  s32 i;
  struct ScreenPoint base;
  s32 center[3];
  struct ScreenPoint screen;
  s32 point[3];
  s32 originX;
  s32 originY;

  cursor = gBattleAnimWork;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->args = args;
  BattleAnim_StartCanvasLayer(0);

  // Bring out the Djinni of the mode's element; origin
  // receives where it appears, and x is scaled to the canvas, shown 1.25
  // times as wide.
  if (work->args->prelude == 1) {
    if (mode == STAT_BOOST_GRANITE)
      BattleAnim_RunDjinniEntrance(args, ELEMENT_VENUS, work->args->side ^ 1,
                                   DJINNI_ANCHOR_TARGET_X, &originX, &originY);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      BattleAnim_RunDjinniEntrance(args, ELEMENT_JUPITER, work->args->side ^ 1,
                                   DJINNI_ANCHOR_TARGET_X, &originX, &originY);
    else
      BattleAnim_RunDjinniEntrance(args, ELEMENT_MARS, work->args->side ^ 1,
                                   DJINNI_ANCHOR_TARGET_X, &originX, &originY);
    originX = originX * 4 / 5;
  }
  REG_BG2PA = Q_8_8(0.8); // 1.25 times the width

  // Pictures and palette: the flares, the burst, the mode's picture, then the
  // Djinni, whose sheet also lends its palette in every mode but Zephyr.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + STAT_BOOST_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + STAT_BOOST_BURST, 1, 1);
  if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
    if (mode == STAT_BOOST_GRANITE)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet),
                      128);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                      128);
  } else if (mode == STAT_BOOST_ZEPHYR) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
  } else {
    void *palette;

    if (mode == STAT_BOOST_IMPACT)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    if (mode == STAT_BOOST_IMPACT)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == STAT_BOOST_GUARD)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  if (mode == STAT_BOOST_GRANITE)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);

  // Scatter the flares around the centre: x and z within 25 pixels, y
  // within 50. All 512 are placed, though each target draws only its first
  // 32 of 64.
  for (i = 0; i != STAT_BOOST_SCATTERED; i++) {
    struct Particle *flare = &PARTICLES[i];

    SET_POSITION(flare, (Random16() % 200 - 100) << 14,
                 (Random16() % 200 - 100) << 15,
                 (Random16() % 200 - 100) << 14);
    flare->variant = 0;
  }

  // With one target, shift the canvas so that target is at its centre
  // (64); with several, show the half of the screen on the targets' side.
  if (work->args->targetCount == 1) {
    Projection_ScaledUnit(work->args->targets[0], &base);
    canvasShiftX = -base.x * 4 / 5 + 64;
  } else {
    if (work->args->side == ANIM_SIDE_PARTY)
      canvasShiftX = -64;
    else
      canvasShiftX = 0;
  }
  REG_BG2X = canvasShiftX << 8;
  // Each transfer keeps half of every level, so the flares leave trails.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  if (work->args->side == ANIM_SIDE_PARTY)
    drawFlags = ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X;
  else
    drawFlags = ANIM_CLIP_BOUNDS;
  Sound_PlaySongFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 8 + 108; frame++) {
    struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);

    // The battle plays its messages from frame 80, as the first target's
    // picture appears.
    if (frame == 80)
      ANIM_SHOW_DAMAGE(0);

    // The Djinni, facing the other way from the flares, sways 20 pixels from
    // side to side over 32 frames and, after frame 32, rises 2 pixels a frame.
    // For the first 4 frames it is also added on top, a flash as it appears.
    if (work->args->prelude == 1) {
      s32 x =
          ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags ^ COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
      draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, drawFlags ^ COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
      draw[1] = gHeapSlots[HEAP_SLOT_RAM_CODE_2];
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      ANIM_RELEASE_BLITTERS();
    }

    // Each target's own timeline, starting 8 frames after the one before.
    ANIM_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor =
          GetBattleObjectSlotFar(work->args->targets[member])->actor;
      s32 height =
          BattleUnit_GetEffectHeightFar(work->args->targets[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Sound_PlaySongFar(SE_POWER_UP);
      ANIM_VIEW_FROM_CAMERA(camera);
      // Centre on two thirds of the target's height.
      SET_VECTOR(point, actor->x.fixed, height, actor->z.fixed);
      Transform_ApplyPosition(point);
      if (frame == member * 8 + 48)
        BattleAnim_TintBattler(work->args->targets[member], SPRITE_COLOR_WHITE,
                               -1, member, 16);

      // Turn the flares and move them 2 pixels a frame towards the centre
      // (distance is half the distance in pixels, the root's 8.8 >> 9); flare
      // i appears i frames after the target's turn starts. Impact turns them
      // by pitch, Guard not at all, Ward by yaw, the others by yaw and roll,
      // 1/64 turn a frame.
      if (frame > member * 8) {
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
        draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, drawFlags, COMPOSITE_ADDITIVE);
        draw[1] = gHeapSlots[HEAP_SLOT_RAM_CODE_2];
        if (mode == STAT_BOOST_IMPACT) {
          Transform_ApplyPitch(-frame << 10);
        } else if (mode == STAT_BOOST_GUARD) {
        } else if (mode == STAT_BOOST_WARD) {
          Transform_ApplyYaw(frame << 10);
        } else {
          Transform_ApplyYaw(frame << 10);
          Transform_ApplyRoll(frame << 10);
        }
        for (i = 0; i != STAT_BOOST_DRAWN; i++) {
          struct Particle *flare = &PARTICLES[member * STAT_BOOST_SLOTS + i];

          if (frame > member * 8 + i) {
            s32 xx = (flare->x >> 8) * (flare->x >> 8);
            s32 yy = (flare->y >> 8) * (flare->y >> 8);
            s32 zz = (flare->z >> 8) * (flare->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              // The size (half the width) runs 6 to 1 over depths 314 to 634.
              Projection_Point(flare, &screen);
              screen.x = screen.x * 4 / 5 + canvasShiftX;
              CLAMP(screen.depth, 314, 634);
              size = 6 - (screen.depth - 314) / 64;
              ANIM_DRAW_FLARE(draw[1], canvas, work->sheet, screen.x, screen.y,
                              size);
              flare->x -= flare->x / distance;
              flare->y -= flare->y / distance;
              flare->z -= flare->z / distance;
            }
          }
        }
        ANIM_RELEASE_BLITTERS();
      }

      // The burst and the picture are drawn at the centre's screen position.
      SET_VECTOR(center, 0, 0, 0);
      Projection_Point(center, &screen);
      screen.x = screen.x * 4 / 5 + canvasShiftX;

      // The burst: six frames of 4 frames each, from frame 52.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cell = (frame - member * 8 - 52) / 4 % 6;

        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
        draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
        draw[0](canvas, work->sheet + cell * (40 * 40) + STAT_BOOST_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE);
      }

      // The mode's picture, from frame 80.
      if (mode == STAT_BOOST_IMPACT) {
        // Seven 24x40 frames, 4 frames each.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cell = (frame - member * 8 - 80) / 4 % 7;

          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
          draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
          draw[0](canvas, work->sheet + cell * (24 * 40) + STAT_BOOST_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          FreeHeapSlot(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH) {
        // Six 32x64 frames, 4 frames each.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cell = (frame - member * 8 - 80) / 4 % 6;

          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
          draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
          draw[0](canvas, work->sheet + cell * (32 * 64) + STAT_BOOST_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          FreeHeapSlot(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_ZEPHYR) {
        // 64x32 frames, 2 frames each, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cell = (frame - member * 8 - 80) / 2 % 6;

          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
          draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, drawFlags | COMPOSITE_FLIP_Y,
                            COMPOSITE_LIGHTEN);
          draw[1] = gHeapSlots[HEAP_SLOT_RAM_CODE_2];
          draw[0](canvas, work->sheet + cell * (64 * 32) + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, work->sheet + cell * (64 * 32) + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y + 8, 64, 32);
          ANIM_RELEASE_BLITTERS();
        }
      } else {
        // Six 40x40 frames, 4 frames each, added.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cell = (frame - member * 8 - 80) / 4 % 6;

          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_ADDITIVE);
          draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
          draw[0](canvas, work->sheet + cell * (40 * 40) + STAT_BOOST_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          FreeHeapSlot(HEAP_SLOT_RAM_CODE);
        }
      }
    }
    ANIM_END_FRAME(work);
  }

  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
}

// Stat-reduction shards. The animations the folder README groups with them
// under "Animation families" follow in their own parts: Flare (flare.c), Quake
// (quake.c), Fire (fire.c), Frost (frost.c) and, after ray.c, Smoke Bomb and
// Blast (eruption.c).

/// A dagger appears over each target and breaks into shards that fade through
/// darker copies, in the pink burst's colours.
///
/// Abilities: Dull, Blunt (BATTLE_ANIM_DULL).
void BattleAnim_RunDull(struct BattleAnimArgs *args) {
  BattleAnim_RunStatReduction(args, STAT_REDUCTION_DULL);
}
/// A shield appears over each target and breaks into shards that fade through
/// darker copies, in the glow orb's colours.
///
/// Abilities: Impair, Debilitate (BATTLE_ANIM_IMPAIR); none
/// (BATTLE_ANIM_UNUSED_IMPAIR).
///
/// Meltdown's casting impact (CASTING_IMPACT_IMPACT_MELTDOWN) also ends by
/// calling it.
void BattleAnim_RunImpair(struct BattleAnimArgs *args) {
  BattleAnim_RunStatReduction(args, STAT_REDUCTION_IMPAIR);
}
/// A shield appears over each target and breaks into fading shards, in the pink
/// burst's colours.
///
/// Abilities: Weaken, Enfeeble (BATTLE_ANIM_WEAKEN).
void BattleAnim_RunWeaken(struct BattleAnimArgs *args) {
  BattleAnim_RunStatReduction(args, STAT_REDUCTION_WEAKEN);
}
/// Breaks the shield picture into fading shards as Weaken does
/// (STAT_REDUCTION_UNUSED). No animation table entry reaches it.
UNUSED void BattleAnim_RunUnusedStatReduction(struct BattleAnimArgs *args) {
  BattleAnim_RunStatReduction(args, STAT_REDUCTION_UNUSED);
}
/// The shard picture in the sheet, followed by seven copies of it, each
/// with its colour levels capped 7 lower than the one before.
#define STAT_REDUCTION_SHARD_OFFSETS 0x2580
#define STAT_REDUCTION_SHARD_BYTES 696 ///< bytes in each copy
#define STAT_REDUCTION_SHARDS 24       ///< shards drawn for each target
#define STAT_REDUCTION_SLOTS 32        ///< particles reserved for each target

/// A dagger (Dull) or a shield (the others) appears over each target, 4
/// frames apart, and then breaks into 24 shards that fly off and fade out
/// through the darker copies. In each target's own frames the picture plays
/// until about frame 24, then breaks with a sound; the shards darken from frame
/// 29 and are gone after frame 60.
///
/// Reached through Dull, Impair, Weaken and the unused wrapper above.
/// Copies the mode's palette over the first 128 bytes of BG palette RAM,
/// writes REG_BG2PA and REG_BG2X.
///
/// args: the targets.
/// mode: the picture and its palette (enum StatReductionMode).
void BattleAnim_RunStatReduction(struct BattleAnimArgs *args, s32 mode) {
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct ScreenPoint screen;
  s32 point[3];
  BlitterPair blitters;
  s32 i;
  s32 paletteId;

  cursor = gBattleAnimWork;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->args = args;
  ANIM_START_NARROW_CANVAS(0);
  if (mode == STAT_REDUCTION_DULL)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == STAT_REDUCTION_DULL)
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == STAT_REDUCTION_IMPAIR)
    paletteId = (s32)&ResourceId_GlowOrbSheet;
  else
    paletteId = (s32)&ResourceId_PinkBurstSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                             work->sheet + STAT_REDUCTION_SHARD_OFFSETS, 0, 0);

  // Build the seven darker copies of the shard picture.
  for (i = 1; i != 8; i++) {
    for (j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
      s32 level = work->sheet[STAT_REDUCTION_SHARD_OFFSETS + j];

      ANIM_CAP_LEVEL(level, 64 - i * 7);
      work->sheet[STAT_REDUCTION_SHARD_OFFSETS +
                  i * STAT_REDUCTION_SHARD_BYTES + j] = level;
    }
  }

  // On ANIM_SIDE_PARTY the canvas moves 112 pixels right, so everything is
  // drawn 112 pixels further left on it.
  if (work->args->side == ANIM_SIDE_PARTY) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards start in a grid of eight rows and four columns and fly
  // away from the user's side, scattering in a random direction; variant
  // is a random start in their turning. All 512 are placed, enough for 16
  // targets of 32 slots. Each flies 2 pixels a frame across, up to 3 either
  // way in depth and -2 to 4 up, keeping 60/64 of its speed each frame.
  for (i = 0; i != 512; i++) {
    struct Particle *shard = &PARTICLES[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == STAT_REDUCTION_DULL) {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 2 - 2) << 16;
    } else {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 8 - 16) << 16;
    }
    if (work->args->side == ANIM_SIDE_PARTY)
      shard->velocityX = 0x20000;
    else
      shard->velocityX = -0x20000;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleAnim_FetchRectangleBlitters(work->args->side, draw = blitters);
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != work->args->targetCount * 4 + 64; frame++) {
    camera = gBattleCamera;
    // The loop runs count * 4 + 64 frames, so frame 72 comes only with three
    // or more targets. With one or two, BattlePlayback_WaitForReady starts
    // playback once the animation returns, so the messages follow the shards
    // instead of overlapping them.
    // XXX (no evidence): whether that difference was meant; both orders show
    // the same messages, so only the author's intent would say.
    if (frame == 72)
      ANIM_SHOW_DAMAGE(0);
    ANIM_FOR_EACH_TARGET(j, work->args) {
      struct Actor *actor;
      s32 targetFrame;
      s32 x;
      s32 y;

      targetFrame = frame - j * 4;
      actor = GetBattleObjectSlotFar(work->args->targets[j])->actor;
      // Each target's timeline starts 4 frames after the one before.
      if (targetFrame > 0) {
        ANIM_VIEW_FROM_CAMERA(camera);
        // Centre on the point 20 pixels above the target; the view is set
        // from the camera twice.
        SET_VECTOR(point, actor->x.fixed, 160 << 13, actor->z.fixed);
        ANIM_VIEW_FROM_CAMERA(camera);
        Transform_ApplyPosition(point);
        SET_VECTOR(origin, 0, 0, 0);
        Projection_Point(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == STAT_REDUCTION_DULL) {
          if (targetFrame <= 26)
            blitters[0](canvas, work->sheet + targetFrame / 4 % 7 * (24 * 40),
                        x - 12, y - 20, 24, 40);
        } else {
          if (targetFrame <= 23)
            draw[1](canvas, work->sheet + targetFrame / 4 % 6 * (40 * 40),
                    x - 20, y - 20, 40, 40);
        }
        if (targetFrame == 24)
          Sound_PlaySongFar(SE_CRACK);
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
            struct Particle *shard = &PARTICLES[j * STAT_REDUCTION_SLOTS + i];
            s32 cell;

            // Shard i takes shape i % 4, turning through its three frames
            // every 8 frames from a random start.
            cell = i % 4 * 3 + (shard->variant + targetFrame) / 8 % 3;
            Projection_Point(shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            blitters[0](canvas,
                        work->sheet +
                            (level * STAT_REDUCTION_SHARD_BYTES +
                             gStatChange_ShardCells[cell]) +
                            STAT_REDUCTION_SHARD_OFFSETS,
                        x, y, gStatChange_ShardWidths[cell],
                        gStatChange_ShardHeights[cell]);
            Particle_AdvanceWithGravity3D(shard, 60, 0);
          }
        }
      }
    }
    ANIM_PRESENT_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

#undef STAT_BOOST_SCATTERED
#undef STAT_BOOST_DRAWN
#undef STAT_BOOST_SLOTS
#undef STAT_REDUCTION_SHARD_OFFSETS
#undef STAT_REDUCTION_SHARD_BYTES
#undef STAT_REDUCTION_SHARDS
#undef STAT_REDUCTION_SLOTS
