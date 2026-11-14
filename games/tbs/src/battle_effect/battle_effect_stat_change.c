/// @file
/// Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...).
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The stat boosts' modes (BattleEffect_RunStatBoost), each named for the
/// effect that plays it.
enum StatBoostMode {
  STAT_BOOST_IMPACT,  ///< a dagger, in Mars colours
  STAT_BOOST_GUARD,   ///< a shield, in Mars colours
  STAT_BOOST_WARD,    ///< a shield, in Jupiter colours
  STAT_BOOST_GRANITE, ///< a pane of light, in Venus colours
  STAT_BOOST_ZEPHYR,  ///< a gust of wind, keeping the palette
  STAT_BOOST_FLASH    ///< a pane of light, in Mars colours
};

/// The stat reductions' modes (BattleEffect_RunStatReduction).
enum StatReductionMode {
  STAT_REDUCTION_DULL,   ///< a dagger, in the pink burst's colours
  STAT_REDUCTION_IMPAIR, ///< a shield, in the glow orb's colours
  STAT_REDUCTION_WEAKEN, ///< a shield, in the pink burst's colours
  STAT_REDUCTION_UNUSED  ///< played as STAT_REDUCTION_WEAKEN; unused
};

/// Light swirls into each target and bursts, then a dagger appears in Mars
/// colours.
///
/// Abilities: Impact, High Impact, Demon Spear, Angel Spear; Forge.
void BattleEffect_RunImpact(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_IMPACT);
}
/// Light swirls into each target and bursts, then a shield appears in Mars
/// colours.
///
/// Abilities: Guard, Protect, Guardian, Protector; Corona.
void BattleEffect_RunGuard(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GUARD);
}
/// Light swirls into each target and bursts, then a shield appears in Jupiter
/// colours.
///
/// Abilities: Ward, Resist, Magic Shell, Magic Shield; Breeze.
void BattleEffect_RunWard(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_WARD);
}
/// Light swirls into each target and bursts, then a pane of light appears in
/// Venus colours.
///
/// Abilities: Granite.
void BattleEffect_RunGranite(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GRANITE);
}
/// Light swirls into each target and bursts, then a pane of light appears in
/// Mars colours.
///
/// Abilities: Flash.
void BattleEffect_RunFlash(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_FLASH);
}
/// Light swirls into each target and bursts, then a gust of wind sweeps across
/// it in the colours already loaded, as this is the one boost that keeps the
/// palette.
///
/// Abilities: Zephyr.
void BattleEffect_RunZephyr(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_ZEPHYR);
}
/// Where each picture sits in the sheet.
enum StatBoostSheet {
  STAT_BOOST_FLARES = 0,      ///< flares (ParticleSpritesD)
  STAT_BOOST_BURST = 1550,    ///< six 40x40 bursts of light (GlowOrb)
  STAT_BOOST_PICTURE = 11150, ///< the picture the mode picks
  STAT_BOOST_DJINNI = 26048   ///< the 40x40 Djinni
};

/// Flares of light swirl in to the centre of each target in turn and burst, and
/// the mode's picture plays over it.
void BattleEffect_RunStatBoost(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);

  if (work->args->prelude == 1) {
    if (mode == STAT_BOOST_GRANITE)
      BattleEffect_RunDjinniEntrance(args, ELEMENT_VENUS, work->args->side ^ 1,
                                     DJINNI_ANCHOR_TARGET_X, &originX,
                                     &originY);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      BattleEffect_RunDjinniEntrance(
          args, ELEMENT_JUPITER, work->args->side ^ 1, DJINNI_ANCHOR_TARGET_X,
          &originX, &originY);
    else
      BattleEffect_RunDjinniEntrance(args, ELEMENT_MARS, work->args->side ^ 1,
                                     DJINNI_ANCHOR_TARGET_X, &originX,
                                     &originY);
    originX = originX * 4 / 5;
  }
  REG_BG2PA = Q_8_8(0.8); // 1.25 times the width

  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D,
                             work->sheet + STAT_BOOST_FLARES, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET,
                             work->sheet + STAT_BOOST_BURST, 1, 1);
  if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH) {
    Resource_LoadAndDecompress(RESOURCE_VIOLET_STREAK_SHEET,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
    if (mode == STAT_BOOST_GRANITE)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SHEET), 128);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET), 128);
  } else if (mode == STAT_BOOST_ZEPHYR) {
    Resource_LoadAndDecompress(RESOURCE_WISP_SHEET,
                               work->sheet + STAT_BOOST_PICTURE, 1, 1);
  } else {
    void *palette;

    if (mode == STAT_BOOST_IMPACT)
      Resource_LoadAndDecompress(RESOURCE_DAGGER_SHEET,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress(RESOURCE_SHIELD_SHEET,
                                 work->sheet + STAT_BOOST_PICTURE, 1, 0);
    if (mode == STAT_BOOST_IMPACT)
      palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SHEET);
    else if (mode == STAT_BOOST_GUARD)
      palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
    else
      palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  if (mode == STAT_BOOST_GRANITE)
    Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SHEET,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
    Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SHEET,
                               work->sheet + STAT_BOOST_DJINNI, 1, 0);

  for (i = 0; i != 512; i++) {
    struct Particle *flare = &PARTICLES[i];

    SET_POSITION(flare, (Random16() % 200 - 100) << 14,
                 (Random16() % 200 - 100) << 15,
                 (Random16() % 200 - 100) << 14);
    flare->variant = 0;
  }

  if (work->args->targetCount == 1) {
    Projection_ScaledUnit(work->args->targets[0], &base);
    canvasShiftX = -base.x * 4 / 5 + 64;
  } else {
    if (work->args->side == EFFECT_SIDE_PARTY)
      canvasShiftX = -64;
    else
      canvasShiftX = 0;
  }
  REG_BG2X = canvasShiftX << 8;
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  if (work->args->side == EFFECT_SIDE_PARTY)
    drawFlags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
  else
    drawFlags = EFFECT_CLIP_BOUNDS;
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * 8 + 108; frame++) {
    struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);

    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    if (work->args->prelude == 1) {
      s32 x =
          ((Trig_Sin(frame << 11) * 20) >> 16) + originX + canvasShiftX - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + originY - 24;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags ^ COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, drawFlags ^ COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + STAT_BOOST_DJINNI, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }

    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor =
          GetBattleObjectSlotFar(work->args->targets[member])->actor;
      s32 height =
          BattleUnit_GetEffectHeightFar(work->args->targets[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Sound_PlayFar(SE_POWER_UP);
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, actor->x.fixed, height, actor->z.fixed);
      Transform_ApplyPosition(point);
      if (frame == member * 8 + 48)
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, TINT_KEEP, member, 16);

      // Flares turn and close in on the centre.
      if (frame > member * 8) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
        draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, drawFlags,
                            COMPOSITE_ADDITIVE);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
        if (mode == STAT_BOOST_IMPACT) {
          Transform_ApplyPitch(-frame << 10);
        } else if (mode == STAT_BOOST_GUARD) {
        } else if (mode == STAT_BOOST_WARD) {
          Transform_ApplyYaw(frame << 10);
        } else {
          Transform_ApplyYaw(frame << 10);
          Transform_ApplyRoll(frame << 10);
        }
        for (i = 0; i != 32; i++) {
          struct Particle *flare = &PARTICLES[member * 64 + i];

          if (frame > member * 8 + i) {
            s32 xx = (flare->x >> 8) * (flare->x >> 8);
            s32 yy = (flare->y >> 8) * (flare->y >> 8);
            s32 zz = (flare->z >> 8) * (flare->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 size;

              Projection_Point(flare, &screen);
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

      SET_VECTOR(center, 0, 0, 0);
      Projection_Point(center, &screen);
      screen.x = screen.x * 4 / 5 + canvasShiftX;

      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 picture = (frame - member * 8 - 52) / 4 % 6;

        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
        draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
        draw[0](canvas, work->sheet + picture * (40 * 40) + STAT_BOOST_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }

      if (mode == STAT_BOOST_IMPACT) {
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 picture = (frame - member * 8 - 80) / 4 % 7;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
          draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
          draw[0](canvas,
                  work->sheet + picture * (24 * 40) + STAT_BOOST_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH) {
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 picture = (frame - member * 8 - 80) / 4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
          draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
          draw[0](canvas,
                  work->sheet + picture * (32 * 64) + STAT_BOOST_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_ZEPHYR) {
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 picture = (frame - member * 8 - 80) / 2 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags, COMPOSITE_LIGHTEN);
          draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                              drawFlags | COMPOSITE_FLIP_Y, COMPOSITE_LIGHTEN);
          draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
          draw[0](canvas,
                  work->sheet + picture * (64 * 32) + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas,
                  work->sheet + picture * (64 * 32) + STAT_BOOST_PICTURE,
                  screen.x - 32, screen.y + 8, 64, 32);
          EFFECT_RELEASE_BLITTERS();
        }
      } else {
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 picture = (frame - member * 8 - 80) / 4 % 6;

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, drawFlags,
                              COMPOSITE_ADDITIVE);
          draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
          draw[0](canvas,
                  work->sheet + picture * (40 * 40) + STAT_BOOST_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// A dagger appears over each target and breaks into shards that fade through
/// darker copies, in the pink burst's colours.
///
/// Abilities: Dull, Blunt.
void BattleEffect_RunDull(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_DULL);
}
/// A shield appears over each target and breaks into shards that fade through
/// darker copies, in the glow orb's colours.
///
/// Abilities: Impair, Debilitate.
void BattleEffect_RunImpair(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_IMPAIR);
}
/// A shield appears over each target and breaks into fading shards, in the pink
/// burst's colours.
///
/// Abilities: Weaken, Enfeeble.
void BattleEffect_RunWeaken(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_WEAKEN);
}
/// Breaks the shield picture into fading shards as Weaken does; no effect table
/// entry reaches it.
UNUSED void
BattleEffect_RunUnusedStatReduction(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_UNUSED);
}
/// The shard picture in the sheet, followed by seven copies of it, each
/// with its colour levels capped 7 lower than the one before.
#define STAT_REDUCTION_SHARD_PICTURES 9600
#define STAT_REDUCTION_SHARD_BYTES 696 ///< bytes in each copy

/// A dagger (Dull) or a shield (the others) appears over each target and breaks
/// into shards that fly off and fade.
void BattleEffect_RunStatReduction(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  s32 origin[3];
  struct ScreenPoint screen;
  s32 point[3];
  BlitterPair blitters;
  s32 i;
  s32 paletteId;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  if (mode == STAT_REDUCTION_DULL)
    Resource_LoadAndDecompress(RESOURCE_DAGGER_SHEET, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress(RESOURCE_SHIELD_SHEET, work->sheet, 1, 1);
  if (mode == STAT_REDUCTION_DULL)
    paletteId = RESOURCE_PINK_BURST_SHEET;
  else if (mode == STAT_REDUCTION_IMPAIR)
    paletteId = RESOURCE_GLOW_ORB_SHEET;
  else
    paletteId = RESOURCE_PINK_BURST_SHEET;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
  Resource_LoadAndDecompress(RESOURCE_PALETTE_RAMP_IMAGE,
                             work->sheet + STAT_REDUCTION_SHARD_PICTURES, 0, 0);

  for (i = 1; i != 8; i++) {
    for (j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
      s32 level = work->sheet[STAT_REDUCTION_SHARD_PICTURES + j];

      EFFECT_CAP_LEVEL(level, 64 - i * 7);
      work->sheet[STAT_REDUCTION_SHARD_PICTURES +
                  i * STAT_REDUCTION_SHARD_BYTES + j] = level;
    }
  }

  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(112 << 8);
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards fly away from the user's side, scattering.
  for (i = 0; i != 512; i++) {
    struct Particle *shard = &PARTICLES[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & TRIG_ANGLE_MASK;
    shard->x = 0;
    if (mode == STAT_REDUCTION_DULL) {
      shard->y = px((i & 31) / 4 * 6 - 10);
      shard->z = px(i % 4 * 2 - 2);
    } else {
      shard->y = px((i & 31) / 4 * 6 - 10);
      shard->z = px(i % 4 * 8 - 16);
    }
    if (work->args->side == EFFECT_SIDE_PARTY)
      shard->velocityX = px(2);
    else
      shard->velocityX = -px(2);
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + px(1);
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleEffect_FetchRectangleBlitters(work->args->side, blitters);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != work->args->targetCount * 4 + 64; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    // The loop runs count * 4 + 64 frames, so frame 72 comes only with three
    // or more targets. With one or two, BattlePlayback_WaitForReady starts
    // playback once the effect returns, so the messages follow the shards
    // instead of overlapping them.
    // XXX (no evidence): whether that difference was meant; both orders show
    // the same messages, so only the author's intent would say.
    if (frame == 72)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(j, work->args) {
      struct Actor *actor;
      s32 targetFrame;
      s32 x;
      s32 y;

      targetFrame = frame - j * 4;
      actor = GetBattleObjectSlotFar(work->args->targets[j])->actor;
      if (targetFrame > 0) {
        // DEAD: the view set from the camera a first time, which the second
        // call repeats; the ROM keeps both calls.
        EFFECT_VIEW_FROM_CAMERA(camera);
        SET_VECTOR(point, actor->x.fixed, 160 << 13, actor->z.fixed);
        EFFECT_VIEW_FROM_CAMERA(camera);
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
            blitters[1](canvas, work->sheet + targetFrame / 4 % 6 * (40 * 40),
                        x - 20, y - 20, 40, 40);
        }
        if (targetFrame == 24)
          Sound_PlayFar(SE_CRACK);
        if (targetFrame >= 24 && targetFrame <= 60) {
          s32 level;

          level = 0;
          if (targetFrame > 28) {
            level = (targetFrame - 24) / 4;
            if (level > 7)
              level = 7;
          }
          for (i = 0; i != 24; i++) {
            struct Particle *shard = &PARTICLES[j * 32 + i];
            s32 picture;

            picture = i % 4 * 3 + (shard->variant + targetFrame) / 8 % 3;
            Projection_Point(shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            blitters[0](canvas,
                        work->sheet +
                            (level * STAT_REDUCTION_SHARD_BYTES +
                             gStatChange_ShardPictures[picture]) +
                            STAT_REDUCTION_SHARD_PICTURES,
                        x, y, gStatChange_ShardWidths[picture],
                        gStatChange_ShardHeights[picture]);
            Particle_AdvanceWithGravity3D(shard, 60, 0);
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef STAT_REDUCTION_SHARD_PICTURES
#undef STAT_REDUCTION_SHARD_BYTES
