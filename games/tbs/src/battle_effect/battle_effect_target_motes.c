/// @file
/// The status motes of Taint, Delude, Paralyze, Heartrender, Mystic Flame,
/// Demon Fire and Drown, then Sleep's sheep and Curse's flames.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The target motes' styles (BattleEffect_RunTargetMotes), each named for the
/// effect that plays it.
enum TargetMotesMode {
  TARGET_MOTES_TAINT,       ///< lime smoke closing in
  TARGET_MOTES_PINK_SMOKE,  ///< pink smoke closing in; Taint's stronger variant
  TARGET_MOTES_DELUDE,      ///< cyan smoke rising
  TARGET_MOTES_PARALYZE,    ///< smoke rising, with glints
  TARGET_MOTES_HEARTRENDER, ///< hearts closing in
  TARGET_MOTES_MYSTIC_FLAME, ///< glowing orbs closing in
  TARGET_MOTES_DROWN,        ///< water drops closing in
  TARGET_MOTES_DEMON_FIRE ///< orbs in Mars Djinn colours rising, then a strike
};

// Swirling motes around each target, in eight styles of one runner: each mote
// plays its six pictures and starts again, a closing mote where it is and a
// rising one at a new point below.

/// Lime smoke swirls in on each target, or pink smoke for a stronger variant.
///
/// Abilities: Taint, Poison.
void BattleEffect_RunTaint(struct BattleEffectParams *args) {
  if (args->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunTargetMotes(args, TARGET_MOTES_TAINT);
    return;
  }
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/// Pink smoke swirls in on each target, as Taint shows it for a stronger
/// variant. No effect table entry in The Broken Seal points here.
UNUSED void BattleEffect_RunUnusedPinkSmoke(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/// Cyan smoke rises around each target.
///
/// Abilities: Delude, Confuse, Mist.
void BattleEffect_RunDelude(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DELUDE);
}
/// Smoke rises around each target, and glints sparkle near it.
///
/// Abilities: Paralyze.
void BattleEffect_RunParalyze(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PARALYZE);
}
/// Hearts close in on each target, turning as they come.
///
/// Abilities: Heartrender.
void BattleEffect_RunHeartrender(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_HEARTRENDER);
}
/// Glowing orbs spiral in on each target.
///
/// Abilities: Mystic Flame.
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_MYSTIC_FLAME);
}
/// Glowing orbs in Mars Djinn colours rise around each target, and then the
/// user steps up to the first target and strikes it.
///
/// Abilities: Demon Fire.
void BattleEffect_RunDemonFire(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DEMON_FIRE);
}
/// Thirty-two water drops spiral in on each target.
///
/// Abilities: Drown.
void BattleEffect_RunDrown(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DROWN);
}
/// Styles whose motes start in a wide ball, and run on
/// TARGET_MOTES_WIDE_RUN_ON frames after the last target's motes start.
#define TARGET_MOTES_START_WIDE(mode)                                          \
  ((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_HEARTRENDER ||  \
   (mode) == TARGET_MOTES_MYSTIC_FLAME || (mode) == TARGET_MOTES_DROWN ||      \
   (mode) == TARGET_MOTES_DEMON_FIRE)
/// Styles whose motes close in on the target; the others rise.
#define TARGET_MOTES_CLOSE_IN(mode)                                            \
  ((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_HEARTRENDER ||  \
   (mode) == TARGET_MOTES_MYSTIC_FLAME || (mode) == TARGET_MOTES_DROWN)

/// Every style but Taint's smokes and Paralyze ripples the canvas.
#define TARGET_MOTES_RIPPLE(mode)                                              \
  (!((mode) <= TARGET_MOTES_PINK_SMOKE || (mode) == TARGET_MOTES_PARALYZE))

/// Where Paralyze keeps its glint images in state->sheet.
enum { TARGET_MOTES_GLINT_PICTURES = 9600 };

#define TARGET_MOTES 512      ///< motes scattered at the start
#define TARGET_MOTES_SLOTS 64 ///< particles reserved for each target
#define TARGET_MOTES_CYCLE 24 ///< frames of a mote's image cycle
/// The motes about each target: TARGET_MOTES_PER_TARGET, or Demon Fire's orbs
/// or Drown's drops.
#define TARGET_MOTES_PER_TARGET 16
#define DEMON_FIRE_ORBS 24
#define DROWN_DROPS 32
/// The frames between one target's motes starting and the next's.
#define TARGET_MOTES_STAGGER 8
/// The frames the effect runs on after the last target's motes start.
#define TARGET_MOTES_RUN_ON 32
#define TARGET_MOTES_WIDE_RUN_ON 64
/// The frames the effect runs on after the damage shows. Demon Fire's user
/// sets off DEMON_FIRE_LEAP_FRAMES before then and reaches the first target
/// over DEMON_FIRE_APPROACH_FRAMES, striking it as the damage shows.
#define TARGET_MOTES_DAMAGE_END_FRAMES 32
#define DEMON_FIRE_LEAP_FRAMES 14
#define DEMON_FIRE_APPROACH_FRAMES 16
/// VortexSheet's water drop, after its three 16x64 swirls.
#define DROWN_DROP_WIDTH 12
#define DROWN_DROP_HEIGHT 24
#define DROWN_DROP_PICTURE (3 * 16 * 64)

/// Around each target in turn, motes (smoke, hearts, orbs) close in on it or
/// rise, and the target flashes.
void BattleEffect_RunTargetMotes(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct ScreenPoint screen;
  s32 point[3];
  struct ScreenPoint base;
  BlitterPair draw;
  void *palette;
  s32 *row;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  count = TARGET_MOTES_PER_TARGET;
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
  if (mode == TARGET_MOTES_TAINT) {
    palette = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
  } else if (mode == TARGET_MOTES_PINK_SMOKE) {
    palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  } else if (mode == TARGET_MOTES_DELUDE) {
    palette = Resource_GetTableEntry(RESOURCE_CYAN_PALETTE);
  } else if (mode == TARGET_MOTES_PARALYZE) {
    palette = Resource_GetTableEntry(RESOURCE_BLAST_SHEET);
  } else if (mode == TARGET_MOTES_HEARTRENDER) {
    palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  } else if (mode == TARGET_MOTES_MYSTIC_FLAME) {
    Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET, state->sheet, 1, 0);
    palette = Resource_GetTableEntry(RESOURCE_GLOW_ORB_SHEET);
  } else if (mode == TARGET_MOTES_DEMON_FIRE) {
    count = DEMON_FIRE_ORBS;
    Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET, state->sheet, 1, 0);
    palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  } else {
    count = DROWN_DROPS;
    Resource_LoadAndDecompress(RESOURCE_VORTEX_SHEET, state->sheet, 1, 0);
    palette = Resource_GetTableEntry(RESOURCE_VORTEX_SHEET);
  }
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  if (mode == TARGET_MOTES_HEARTRENDER)
    Resource_LoadAndDecompress(RESOURCE_HEART_SHEET, state->sheet, 1, 1);
  if (mode == TARGET_MOTES_PARALYZE)
    Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                               state->sheet + TARGET_MOTES_GLINT_PICTURES, 1,
                               0);

  for (i = 0; i != TARGET_MOTES; i++) {
    struct Particle *mote = &PARTICLES[i];

    if (TARGET_MOTES_START_WIDE(mode)) {
      SET_POSITION(mote, ((Random16() & 0xff) - 127) << 15,
                   ((Random16() & 0xff) - 127) << 14,
                   ((Random16() & 0xff) - 127) << 15);
    } else {
      SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                   ((Random16() & 0xff) - 255) << 13,
                   ((Random16() & 0xff) - 127) << 13);
    }
    mote->variant = 0;
  }

  if (TARGET_MOTES_START_WIDE(mode))
    total = state->args->targetCount * TARGET_MOTES_STAGGER +
            TARGET_MOTES_WIDE_RUN_ON;
  else
    total =
        state->args->targetCount * TARGET_MOTES_STAGGER + TARGET_MOTES_RUN_ON;
  if (TARGET_MOTES_RIPPLE(mode))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    // Demon Fire: the user steps up to the first target and strikes it.
    if (mode == TARGET_MOTES_DEMON_FIRE) {
      if (frame ==
          total - TARGET_MOTES_DAMAGE_END_FRAMES - DEMON_FIRE_LEAP_FRAMES)
        BattleMotion_ApproachTargetFar(state->args->user,
                                       state->args->targets[0],
                                       DEMON_FIRE_APPROACH_FRAMES, 0);
      if (frame == total - TARGET_MOTES_DAMAGE_END_FRAMES) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        BattleMotion_ApplyKnockbackFar(state->args->targets[0], KNOCKBACK_HURL);
        state->shakeFrames = 8;
      }
    } else if (frame == total - TARGET_MOTES_DAMAGE_END_FRAMES) {
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    row = state->bg2xLines;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
      *row++ = CANVAS_WAVE_BG2X(
          toFIXED(4) -
          Trig_Sin(frame * (TRIG_TURN / 16) + i * (TRIG_TURN / 32)) * 4);

    EFFECT_FOR_EACH_TARGET(member, state->args) {
      struct Actor *actor =
          GetBattleObjectSlotFar(state->args->targets[member])->actor;

      // Paralyze's glints sparkle near the target.
      if (mode == TARGET_MOTES_PARALYZE &&
          frame > member * TARGET_MOTES_STAGGER &&
          frame < member * TARGET_MOTES_STAGGER + 32) {
        Projection_ScaledUnit(state->args->targets[member], &base);
        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;

          angle = Random16() & TRIG_ANGLE_MASK;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + (FIXEDtoINT(Trig_Sin(angle) * radius) >> 1) -
              (gBattleEffect_GlintPictureWidths[frame & 3] >> 1);
          y = base.y - FIXEDtoINT(Trig_Cos(angle) * radius) -
              (gBattleEffect_GlintPictureHeights[frame & 3] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              EFFECT_CLIP_BOUNDS |
                                  gTargetMotes_GlintFlips[Random16() & 3],
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
              canvas,
              state->sheet + gBattleEffect_GlintPictures[frame & 3] +
                  TARGET_MOTES_GLINT_PICTURES,
              x, y + 16, gBattleEffect_GlintPictureWidths[frame & 3],
              gBattleEffect_GlintPictureHeights[frame & 3]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, actor->x.fixed, toFIXED(20), actor->z.fixed);
      Matrix_TranslateVector(point);
      if (frame > member * TARGET_MOTES_STAGGER) {
        Matrix_RotateY(frame * (TRIG_TURN / 128));
        if (mode <= TARGET_MOTES_PINK_SMOKE || mode == TARGET_MOTES_HEARTRENDER)
          Matrix_RotateX(frame * (TRIG_TURN / 128));
        for (i = 0; i != count; i++) {
          struct Particle *mote = &PARTICLES[member * TARGET_MOTES_SLOTS + i];
          s32 distance;

          if (frame > member * TARGET_MOTES_STAGGER + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant < TARGET_MOTES_CYCLE) {
              s32 picture = mote->variant / 4;

              Projection_Point(mote, &screen);
              screen.x >>= 1;
              if (mode == TARGET_MOTES_MYSTIC_FLAME ||
                  mode == TARGET_MOTES_DEMON_FIRE)
                draw[1](canvas,
                        state->sheet + picture * PICTURE_BYTES(GLOW_ORB),
                        screen.x - GLOW_ORB_WIDTH / 2,
                        screen.y - GLOW_ORB_HEIGHT / 2, GLOW_ORB_WIDTH,
                        GLOW_ORB_HEIGHT);
              else if (mode == TARGET_MOTES_DROWN)
                draw[1](canvas, state->sheet + DROWN_DROP_PICTURE,
                        screen.x - DROWN_DROP_WIDTH / 2,
                        screen.y - DROWN_DROP_HEIGHT / 2, DROWN_DROP_WIDTH,
                        DROWN_DROP_HEIGHT);
              else if (mode == TARGET_MOTES_HEARTRENDER)
                draw[1](canvas, state->sheet, screen.x - HEART_WIDTH / 2,
                        screen.y - HEART_HEIGHT / 2, HEART_WIDTH, HEART_HEIGHT);
              else
                draw[(i & 3) != 0](
                    canvas, state->sheet + picture * PICTURE_BYTES(SMOKE_PUFF),
                    screen.x - SMOKE_PUFF_WIDTH / 2,
                    screen.y - SMOKE_PUFF_HEIGHT / 2, SMOKE_PUFF_WIDTH,
                    SMOKE_PUFF_HEIGHT);
              if (TARGET_MOTES_CLOSE_IN(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += toFIXED(1);
              }
              mote->variant++;
              if (mote->variant == TARGET_MOTES_CYCLE) {
                if (TARGET_MOTES_CLOSE_IN(mode)) {
                  mote->variant = 0;
                } else {
                  SET_POSITION(mote, ((Random16() & 0xff) - 127) << 13,
                               ((Random16() & 0xff) - 255) << 12,
                               ((Random16() & 0xff) - 127) << 13);
                }
              }
            }
          }
        }
      }
      EFFECT_RELEASE_BLITTERS();
      if (frame == member * TARGET_MOTES_STAGGER + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        BattleEffect_TintBattler(state->args->targets[member],
                                 SPRITE_COLOR_WHITE, POSE_DAMAGE, member,
                                 delay);
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  if (TARGET_MOTES_RIPPLE(mode))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#define SLEEP_FRAMES 148
#define SLEEP_SEEDED 32 ///< particles cleared at the start
#define SLEEP_SHEEP 12  ///< sheep dropped
/// The sheep drop SLEEP_SHEEP_STAGGER frames apart; the targets fall asleep
/// SLEEP_TARGET_STAGGER frames apart from SLEEP_ASLEEP_FRAME, and the damage
/// shows at SLEEP_DAMAGE_FRAME.
#define SLEEP_SHEEP_STAGGER 4
#define SLEEP_ASLEEP_FRAME 64
#define SLEEP_TARGET_STAGGER 16
#define SLEEP_DAMAGE_FRAME 80
/// The ground the sheep were meant to bounce off.
#define SLEEP_GROUND_Y toFIXED(92)
/// SheepSheet: four pictures, then the four drawn flipped.
#define SHEEP_WIDTH 32
#define SHEEP_HEIGHT 32
#define SHEEP_PICTURES 4
/// A sheep's variant counts its tumble, z a frame: it turns to its next
/// picture every SHEEP_TUMBLE_STEP.
#define SHEEP_TUMBLE_STEP 16

/// Twelve sheep drop onto the targets' side, tumbling, and each target falls
/// asleep in turn.
///
/// Abilities: Sleep.
void BattleEffect_RunSleep(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 anchor[3];
  struct Particle *current;
  struct Particle *sheep;
  s32 particleIndex;
  s32 frame;
  DrawRectangle drawFlipped;
  DrawRectangle drawUpright;
  s32 targetIndex;

  sheep = PARTICLES;
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_SHEEP_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  drawUpright = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      COMPOSITE_TRANSPARENT);
  drawFlipped = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

  for (particleIndex = 0; particleIndex != SLEEP_SEEDED; particleIndex++) {
    current = &sheep[particleIndex];
    current->x = INTtoFIXED((Random16() & 63) + 32);
    current->y = -toFIXED(32);
    current->velocityY = Random16() & 0; // a random number drawn for nothing
    current->z = Random16() & 3;
    current->variant = Random16() & 255;
  }
  if (state->args->side == EFFECT_SIDE_PARTY)
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_SWIRL);
  for (frame = 0; frame != SLEEP_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    if (frame == SLEEP_DAMAGE_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(targetIndex, state->args) {
      struct Actor *target;

      target = GetBattleObjectSlotFar(state->args->targets[targetIndex])->actor;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(anchor, target->x.fixed, toFIXED(40), target->z.fixed);
      Matrix_TranslateVector(anchor);
      if (frame == targetIndex * SLEEP_TARGET_STAGGER + SLEEP_ASLEEP_FRAME)
        BattleEffect_TintBattler(state->args->targets[targetIndex],
                                 SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP,
                                 0);
    }
    for (particleIndex = 0; particleIndex != SLEEP_SHEEP; particleIndex++) {
      current = &sheep[particleIndex];
      if (frame > particleIndex * SLEEP_SHEEP_STAGGER &&
          current->y <= toFIXED(CANVAS_SIZE) - 1) {
        s32 picture;

        picture =
            (current->variant / SHEEP_TUMBLE_STEP) & (SHEEP_PICTURES * 2 - 1);
        if (picture < SHEEP_PICTURES)
          drawUpright(canvas, state->sheet + picture * PICTURE_BYTES(SHEEP),
                      FIXEDtoINT(current->x) - SHEEP_WIDTH / 2,
                      FIXEDtoINT(current->y) - SHEEP_HEIGHT / 2, SHEEP_WIDTH,
                      SHEEP_HEIGHT);
        else
          drawFlipped(canvas,
                      state->sheet + picture * PICTURE_BYTES(SHEEP) -
                          SHEEP_PICTURES * PICTURE_BYTES(SHEEP),
                      FIXEDtoINT(current->x) - SHEEP_WIDTH / 2,
                      FIXEDtoINT(current->y) - SHEEP_HEIGHT / 2, SHEEP_WIDTH,
                      SHEEP_HEIGHT);
        current->y += current->velocityY;
        current->velocityY += toFIXED(1) / 8;
        current->variant += current->z;
        // BUG: meant to bounce a sheep off the ground, tumbling faster, but it
        // tests for a speed of exactly 0 just after gravity was added, so the
        // sheep almost always fall on through. BUGFIX bounces any sheep that
        // is falling.
#ifdef BUGFIX
        if (current->y > SLEEP_GROUND_Y && current->velocityY > 0) {
#else
        if (current->y > SLEEP_GROUND_Y && current->velocityY == 0) {
#endif
          s32 speed = current->velocityY + 1;
          current->y = SLEEP_GROUND_Y;
          current->z += 4;
          current->velocityY = -speed / 2;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  EFFECT_FINISH_CANVAS();
}

#define CURSE_CANDLE_FRAMES 72 ///< how long each target's candle burns
/// The frames between one target's candle and the next's.
#define CURSE_STAGGER 16
/// The flame flickers through its pictures, CURSE_FLICKER_FRAMES a picture.
#define CURSE_FLICKER_FRAMES 6

/// FlameColumnSheet: the flame's nine pictures, then the candle.
#define CURSE_FLAME_WIDTH 16
#define CURSE_FLAME_HEIGHT 12
#define CURSE_FLAME_PICTURES 9
#define CURSE_CANDLE_WIDTH 16
#define CURSE_CANDLE_HEIGHT 20
#define CURSE_CANDLE_PICTURE (CURSE_FLAME_PICTURES * PICTURE_BYTES(CURSE_FLAME))

/// A candle stands on each target's head in turn, a small flame burning on it.
///
/// Abilities: Curse; Lucid Prophecy.
void BattleEffect_RunCurse(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  struct Particle *candle;
  s32 i;
  s32 frame;
  s32 start;
  s32 picture;
  s32 y;
  struct ScreenPoint screenPosition;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  state->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_FLAME_COLUMN_SHEET, state->sheet, 1, 1);
  if (state->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  state->transferMode = CANVAS_COPY_AND_CLEAR;
  state->transferValue = 0;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  // Each flame starts at a random point of its flicker.
  EFFECT_FOR_EACH_TARGET(i, state->args) {
    state->particles[i].variant = Random16() & 63;
  }
  for (frame = 0; frame != (state->args->targetCount << 5) + 32; frame++) {
    if (frame == 32) {
      EFFECT_SHOW_DAMAGE(0);
    }
    i = 0;
    if (state->args->targetCount != 0) {
      candle = state->particles;
      do {
        start = i * CURSE_STAGGER;
        if (frame == start) {
          Sound_PlayFar(SE_CRACK);
        }
        if (frame >= start && frame < start + CURSE_CANDLE_FRAMES) {
          Projection_ScaledUnit(state->args->targets[i], &screenPosition);
          if (state->args->side == EFFECT_SIDE_PARTY) {
            screenPosition.x -= NARROW_CANVAS_PARTY_X;
          }
          y = screenPosition.y;
          screenPosition.y = y - 16;
          draw[0](canvas, state->sheet + CURSE_CANDLE_PICTURE,
                  screenPosition.x - CURSE_CANDLE_WIDTH / 2,
                  y - CURSE_CANDLE_HEIGHT, CURSE_CANDLE_WIDTH,
                  CURSE_CANDLE_HEIGHT);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            picture = ((frame - start) + candle->variant) /
                      CURSE_FLICKER_FRAMES % CURSE_FLAME_PICTURES;
            draw[0](canvas, state->sheet + picture * PICTURE_BYTES(CURSE_FLAME),
                    screenPosition.x - CURSE_FLAME_WIDTH / 2,
                    screenPosition.y - 16, CURSE_FLAME_WIDTH,
                    CURSE_FLAME_HEIGHT);
          }
        }
        candle++;
        i++;
      } while (i != state->args->targetCount);
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef TARGET_MOTES_START_WIDE
#undef TARGET_MOTES_CLOSE_IN
#undef TARGET_MOTES_RIPPLE
#undef TARGET_MOTES
#undef TARGET_MOTES_SLOTS
#undef TARGET_MOTES_CYCLE
#undef TARGET_MOTES_PER_TARGET
#undef DEMON_FIRE_ORBS
#undef DROWN_DROPS
#undef TARGET_MOTES_STAGGER
#undef TARGET_MOTES_RUN_ON
#undef TARGET_MOTES_WIDE_RUN_ON
#undef DROWN_DROP_WIDTH
#undef DROWN_DROP_HEIGHT
#undef DROWN_DROP_PICTURE
#undef SHEEP_PICTURES
#undef SHEEP_TUMBLE_STEP
#undef SLEEP_FRAMES
#undef SLEEP_SEEDED
#undef SLEEP_SHEEP
#undef TARGET_MOTES_DAMAGE_END_FRAMES
#undef DEMON_FIRE_LEAP_FRAMES
#undef DEMON_FIRE_APPROACH_FRAMES
#undef SLEEP_SHEEP_STAGGER
#undef SLEEP_ASLEEP_FRAME
#undef SLEEP_TARGET_STAGGER
#undef SLEEP_DAMAGE_FRAME
#undef SLEEP_GROUND_Y
#undef CURSE_CANDLE_FRAMES
#undef CURSE_STAGGER
#undef CURSE_FLICKER_FRAMES
#undef CURSE_FLAME_PICTURES
#undef CURSE_CANDLE_PICTURE
