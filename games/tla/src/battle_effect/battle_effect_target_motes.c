/// @file
/// The status motes of Taint and its kin, Sleep's sheep and Curse's flames, a
/// Djinni riding a swarm of sparks, and Balm, Tinder and Phoenix.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// Where Paralyze unpacks the tornado's glints in the sheet.
#define TORNADO_GLINT_SHEET 0x2580

/// The target motes' styles (BattleEffect_RunTargetMotes), each named for the
/// effect that plays it.
enum TargetMotesMode {
  TARGET_MOTES_TAINT,       ///< lime smoke closing in
  TARGET_MOTES_PINK_SMOKE,  ///< pink smoke closing in; Taint's stronger variant
  TARGET_MOTES_DELUDE,      ///< cyan smoke rising
  TARGET_MOTES_PARALYZE,    ///< smoke rising, with glints
  TARGET_MOTES_HEARTRENDER, ///< hearts closing in
  TARGET_MOTES_MYSTIC_FLAME, ///< glowing orbs closing in
  TARGET_MOTES_DROWN,        ///< vortex motes closing in
  TARGET_MOTES_DEMON_FIRE ///< orbs in Mars Djinn colours rising, then a strike
};

/// Each entry passes a particle type (color and count) to
/// BattleEffect_RunTargetMotes.
void BattleEffect_RunTaint(struct BattleEffectParams *args) {
  if (args->variant == EFFECT_VARIANT_BASIC) {
    BattleEffect_RunTargetMotes(args, TARGET_MOTES_TAINT);
    return;
  }
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/// Abilities: Poison Gel.
void BattleEffect_RunPoisonGel(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PINK_SMOKE);
}

/// Abilities: Delude, Confuse, Mist, Mucous Gel.
void BattleEffect_RunDelude(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DELUDE);
}

/// Abilities: Paralyze.
void BattleEffect_RunParalyze(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_PARALYZE);
}

/// Abilities: Heartrender.
void BattleEffect_RunHeartrender(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_HEARTRENDER);
}

/// Abilities: Mystic Flame.
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_MYSTIC_FLAME);
}

/// Abilities: Demonfire.
void BattleEffect_RunDemonFire(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DEMON_FIRE);
}

/// Abilities: Drown.
void BattleEffect_RunDrown(struct BattleEffectParams *args) {
  BattleEffect_RunTargetMotes(args, TARGET_MOTES_DROWN);
}

#define MOTE_MAX 512

// The motes live in the map cell buffer, 64 for each target.

/// Modes whose motes close in on the target and start again; the others
/// drift up and are thrown back at random. Mode 7 closes in once.
#define CONVERGES(mode)                                                        \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

#define TARGET_MOTES_SLOTS 64 ///< particles reserved for each target
#define TARGET_MOTES_CYCLE 24 ///< frames of a mote's image cycle

/// Around each target in turn, motes (smoke, hearts, orbs) close in on it or
/// rise, and the target flashes.
void BattleEffect_RunTargetMotes(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct ScreenPoint screen;
  FIXED point[3];
  struct ScreenPoint base;
  DrawRectangle draw[2];
  void *palette;
  s32 *row;
  s32 i;

  // Set up: the canvas layer and the mode's sheet and palette.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  count = 16;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, work->sheet, 1, 1);
  if (mode == TARGET_MOTES_TAINT) {
    palette = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
  } else if (mode == TARGET_MOTES_PINK_SMOKE) {
    palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  } else if (mode == TARGET_MOTES_DELUDE) {
    palette = Resource_GetTableEntry(RESOURCE_CYAN_PALETTE);
  } else if (mode == TARGET_MOTES_PARALYZE) {
    palette = Resource_GetTableEntry(RESOURCE_BLAST_SHEET);
  } else if (mode == TARGET_MOTES_HEARTRENDER) {
    palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  } else if (mode == TARGET_MOTES_MYSTIC_FLAME) {
    Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET, work->sheet, 1, 0);
    palette = Resource_GetTableEntry(RESOURCE_GLOW_ORB_SHEET);
  } else if (mode == TARGET_MOTES_DEMON_FIRE) {
    count = 24;
    Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_SHEET, work->sheet, 1, 0);
    palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  } else {
    count = 32;
    Resource_LoadAndDecompress(RESOURCE_VORTEX_SHEET, work->sheet, 1, 0);
    palette = Resource_GetTableEntry(RESOURCE_VORTEX_SHEET);
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  if (mode == TARGET_MOTES_HEARTRENDER)
    Resource_LoadAndDecompress(RESOURCE_HEART_SHEET, work->sheet, 1, 1);
  if (mode == TARGET_MOTES_PARALYZE)
    Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                               work->sheet + TORNADO_GLINT_SHEET, 1, 0);

  // Scatter the motes around the origin.
  for (i = 0; i != MOTE_MAX; i++) {
    struct Particle *mote = &PARTICLES[i];

    if (CONVERGES(mode)) {
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

  if (CONVERGES(mode))
    total = args->targetCount * 8 + 64;
  else
    total = args->targetCount * 8 + 32;
  if (!(mode <= TARGET_MOTES_PINK_SMOKE || mode == TARGET_MOTES_PARALYZE))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Mode 7 sends the caster at the first target 46 frames before the
    // end and strikes it 14 frames later; the others let the battle show the
    // damage.
    if (mode == TARGET_MOTES_DEMON_FIRE) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, 0);
      if (frame == total - 32) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }

    // Ripple the canvas: one horizontal offset for each line.
    row = work->bg2xLines;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
      *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(args->targets[member])->object;

      // Mode 3: two tornado glints a frame around the target, each drawn
      // with a blitter of its own, mirrored at random.
      if (mode == TARGET_MOTES_PARALYZE && frame > member * 8 &&
          frame < member * 8 + 32) {
        EffectPosition_ProjectScaledBattler(args->targets[member], &base);

        for (i = 0; i != 2; i++) {
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          s32 cel = frame & 3;
          s32 radius = (Random16() & 31) + 4;
          s32 x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
                  (gBattleEffect_GlintPictureWidths[cel] >> 1);
          s32 y = base.y - FIXEDtoINT(Trig_Cos(angle) * radius) -
                  (gBattleEffect_GlintPictureHeights[cel] >> 1);
          u32 flip = Random16();

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              gTargetMotes_GlintFlips[flip & 3] |
                                  COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                              BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
              canvas,
              work->sheet + gBattleEffect_GlintPictures[cel] +
                  TORNADO_GLINT_SHEET,
              x, y + 16, gBattleEffect_GlintPictureWidths[cel],
              gBattleEffect_GlintPictureHeights[cel]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }

      // An additive and a max blitter for this target's motes.
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_MAX);
      draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      point[0] = object->x;
      point[1] = toFIXED(20);
      point[2] = object->z;
      Matrix_TranslateVector(point);

      // From its turn on, the target's motes turn around it.
      if (frame > member * 8) {
        Matrix_RotateY(frame << 9);
        if (mode <= TARGET_MOTES_PINK_SMOKE || mode == TARGET_MOTES_HEARTRENDER)
          Matrix_RotateX(frame << 9);
        for (i = 0; i != count; i++) {
          struct Particle *mote = &PARTICLES[member * TARGET_MOTES_SLOTS + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 picture = mote->variant / 4;

              EffectPosition_ProjectPoint((s32 *)mote, &screen);
              screen.x >>= 1;
              if (mode == TARGET_MOTES_MYSTIC_FLAME ||
                  mode == TARGET_MOTES_DEMON_FIRE)
                draw[1](canvas, work->sheet + picture * PICTURE_BYTES(GLOW_ORB),
                        screen.x - GLOW_ORB_WIDTH / 2,
                        screen.y - GLOW_ORB_HEIGHT / 2, GLOW_ORB_WIDTH,
                        GLOW_ORB_HEIGHT);
              else if (mode == TARGET_MOTES_DROWN)
                draw[1](canvas, work->sheet + 3072, screen.x - 6, screen.y - 12,
                        12, 24);
              else if (mode == TARGET_MOTES_HEARTRENDER)
                draw[1](canvas, work->sheet, screen.x - HEART_WIDTH / 2,
                        screen.y - HEART_HEIGHT / 2, HEART_WIDTH, HEART_HEIGHT);
              else
                draw[(i & 3) != 0](
                    canvas, work->sheet + picture * PICTURE_BYTES(VORTEX),
                    screen.x - VORTEX_WIDTH / 2, screen.y - VORTEX_HEIGHT / 2,
                    VORTEX_WIDTH, VORTEX_HEIGHT);

              // Close in by a step of the distance, or drift up.
              if (DECAYS(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += toFIXED(1);
              }
              mote->variant++;
              if (mote->variant == TARGET_MOTES_CYCLE) {
                if (DECAYS(mode)) {
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

      // The target flashes 16 frames into its turn, until the end.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, member, delay);
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (!(mode <= TARGET_MOTES_PINK_SMOKE || mode == TARGET_MOTES_PARALYZE))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHEEP_COUNT 12
#define SHEEP_FRAMES 98
#define GROUND_Y toFIXED(92)

#define SLEEP_SEEDED 32 ///< particles cleared at the start

#define SHEEP_WIDTH 32
#define SHEEP_HEIGHT 32

/// Twelve sheep drop onto the targets' side, tumbling, and each target falls
/// asleep in turn.
///
/// Abilities: Sleep.
void BattleEffect_RunSleep(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  struct ScreenPoint middle;
  FIXED record[3];
  struct Particle *one;
  struct Particle *sheep;
  s32 i;
  s32 frame;
  DrawRectangle drawB;
  DrawRectangle drawA;
  s32 scroll;
  s32 member;

  // Set up: the canvas layer, the sheep sheet and a transparent blitter
  // pair, the second mirrored.
  sheep = (struct Particle *)Ram_MapCellBuffer;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  scroll = 0;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = scroll;
  Resource_LoadAndDecompress(RESOURCE_SHEEP_CARD_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  drawA = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_TRANSPARENT);
  drawB = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  // The sheep wait above the screen with a random spin and starting cel.
  for (i = 0; i != SLEEP_SEEDED; i++) {
    one = &sheep[i];
    one->x = INTtoFIXED((Random16() & 63) + 32);
    one->y = -toFIXED(32);
    one->velocityY = Random16() & 0;
    one->z = Random16() & 3;
    one->variant = Random16() & 255;
  }

  // Centre the canvas on the targets; what BG2 cannot reach moves the sheep.
  BattleEffect_GetTargetsMidpoint(&middle);
  middle.x -= 64;
  if (middle.x < 0) {
    scroll = middle.x;
    middle.x = 0;
  } else if (middle.x > 112) {
    scroll = middle.x - 112;
    middle.x = 112;
  }
  REG_BG2X = -middle.x << 8;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != SHEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    // Place each target, and from frame 64 restore its palette, one
    // target every 16 frames.
    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object;

      object = BattleObject_GetFar(args->targets[member])->object;
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      record[0] = object->x;
      record[1] = toFIXED(40);
      record[2] = object->z;
      Matrix_TranslateVector(record);
      if (frame == member * 16 + 64)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
                                 POSE_DAMAGE, TINT_KEEP, 0);
    }

    // The sheep fall and bounce, each spinning through its eight cels:
    // the first four drawn as they are, the last four mirrored.
    one = sheep;
    for (i = 0; i != SHEEP_COUNT; i++, one++) {
      if (frame > i * 4 && one->y <= 0x7fffff) {
        s32 cel;

        cel = (one->variant / 16) & 7;
        if (cel < 4)
          drawA(canvas, work->sheet + (cel << 10) + 0x800,
                scroll + (FIXEDtoINT(one->x)) - 16, (FIXEDtoINT(one->y)) - 16,
                SHEEP_WIDTH, SHEEP_HEIGHT);
        else
          drawB(canvas, work->sheet + (cel << 10) - 0x800,
                scroll + (FIXEDtoINT(one->x)) - 16, (FIXEDtoINT(one->y)) - 16,
                SHEEP_WIDTH, SHEEP_HEIGHT);
        one->y += one->velocityY;
        one->velocityY += toFIXED(1) / 8;
        one->variant += one->z;
        if (one->y > GROUND_Y && one->velocityY == 0) {
          s32 speed = one->velocityY + 1;
          one->y = GROUND_Y;
          one->z += 4;
          one->velocityY = -speed / 2;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Tear down.
  EFFECT_FINISH_CANVAS();
}

#define MARKER_FRAMES 72 ///< how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

/// FlameColumnSheet: the flame's nine pictures, then the marker at the
/// target's feet.
#define CURSE_FLAME_WIDTH 16
#define CURSE_FLAME_HEIGHT 12
#define CURSE_MARKER_PICTURE_WIDTH 16
#define CURSE_MARKER_PICTURE_HEIGHT 20
#define CURSE_MARKER_PICTURE (9 * PICTURE_BYTES(CURSE_FLAME))

/// A small flame burns above each target in turn, over a marker at its feet.
///
/// Abilities: Curse; Lucid Prophecy.
void BattleEffect_RunCurse(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  struct Particle *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct ScreenPoint pos;

  // Set up: the canvas layer, unscaled; for side 1 the canvas shows the
  // right half of the field.
  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_FLAME_COLUMN_SHEET, work->sheet, 1, 1);
  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR;
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each flame starts at a random point in its turn.
  for (i = 0; i != args->targetCount; i++) {
    work->particles[i].variant = Random16() & 63;
  }

  frame = 0;
  while (frame != (args->targetCount << 5) + 32) {
    if (frame == 32) {
      EFFECT_SHOW_DAMAGE(0);
    }
    i = 0;
    if (args->targetCount != 0) {
      do {
        start = i * MARKER_STAGGER;
        if (frame == start) {
          Sound_PlayFar(SE_CRACK);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The marker at the target's feet, then the flame, six frames a cel.
          marker = &work->particles[i];
          EffectPosition_ProjectScaledBattler(args->targets[i], &pos);
          if (args->side == EFFECT_SIDE_PARTY) {
            pos.x -= NARROW_CANVAS_PARTY_X;
          }
          y = pos.y;
          pos.y = y - 16;
          draw[0](canvas, work->sheet + CURSE_MARKER_PICTURE, pos.x - 8,
                  y - CURSE_MARKER_PICTURE_HEIGHT, CURSE_MARKER_PICTURE_WIDTH,
                  CURSE_MARKER_PICTURE_HEIGHT);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16,
                    CURSE_FLAME_WIDTH, CURSE_FLAME_HEIGHT);
          }
        }
        i++;
      } while (i != args->targetCount);
    }
    EFFECT_PRESENT_CANVAS(work);
    frame += 1;
  }

  // Tear down.
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define DJINN_HEAL_FRAMES 98

#define DJINN_HEAL_SPARKS 512 ///< sparks in PARTICLES, 64 for each target
#define DJINN_HEAL_SPARKS_PER_TARGET 64
#define DJINN_HEAL_SWIRL_MOTES 32 ///< motes swirled in

/// A Djinni rides a swarm of sparks from the user to the first target, then
/// hovers over the targets while sparkles gather on each in turn.
void BattleEffect_RunDjinnHeal(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  FIXED position[3];
  FIXED target[3];
  FIXED delta[3];
  s32 rider[3];
  struct ScreenPoint screen;
  FIXED point[3];
  struct ScreenPoint view;
  DrawRectangle callbacks[2];

  // Set up: the canvas layer, the Djinn and the particle sprites, and the
  // blitter pair for the side the Djinn faces.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(args->side ^ 1, draw = callbacks);
  actor = BattleObject_GetFar(args->user)->object;
  goal = BattleObject_GetFar(args->targets[0])->object;

  // The swarm: 64 sparks thrown out level from the Djinn.
  for (i = 0; i != DJINN_HEAL_SPARKS_PER_TARGET; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 heading = Random16() & TRIG_ANGLE_MASK;
    s32 speed = (Random16() & 0xff) + 128;

    SET_POSITION(spark, 0, INTtoFIXED((Random16() & 31) + 20), 0);
    spark->velocityX = (Trig_Sin(heading) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(heading) * speed) >> 5;
    spark->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The Djinn travels from the caster to above the first target in 40
  // frames.
  position[0] = actor->x;
  position[1] = 0;
  position[2] = actor->z;
  target[0] = goal->x;
  target[1] = toFIXED(90);
  target[2] = 0;
  delta[0] = (target[0] - position[0]) / 40;
  delta[1] = (target[1] - position[1]) / 40;
  delta[2] = (target[2] - position[2]) / 40;

  for (frame = 0; frame != DJINN_HEAL_FRAMES; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 80)
      Sound_PlayFar(SE_SWIRL);
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    Matrix_TranslateVector(position);

    // The caster flashes for the first 24 frames.
    if (frame == 0)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 24)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);

    // Each spark spins in one of four planes, smaller further away, and
    // after 30 frames is pulled back toward the Djinn.
    for (i = 0; i != DJINN_HEAL_SWIRL_MOTES; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (frame > i && spark->variant == 0) {
        s32 size;

        Matrix_Push();
        switch (i & 3) {
        case 0:
          Matrix_RotateY(frame * (i * 32 + 256));
          break;
        case 1:
          Matrix_RotateX(-frame * (i * 32 + 256));
          break;
        case 2:
          Matrix_RotateZ(-frame * (i * 32 + 256));
          break;
        case 3:
          Matrix_RotateX(-frame * (i * 32 + 256));
          Matrix_RotateZ(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint((s32 *)spark, &screen);
        screen.x >>= 1;
        Matrix_Pop();
        if (screen.depth < 250)
          screen.depth = 250;
        if (screen.depth > 634)
          screen.depth = 634;
        size = 9 - (screen.depth - 250) / 64;
        callbacks[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                     screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }

    // The Djinn bobs over the target for the last 15 frames.
    if (frame > 82) {
      rider[0] = 0;
      rider[1] = Trig_Sin(frame << 10) << 2;
      rider[2] = 0;
      EffectPosition_ProjectPoint(rider, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17,
              SMALL_DJINNI_WIDTH, MERCURY_SMALL_DJINNI_HEIGHT);
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // The second part: 64 puffs of sparkle for each target.
  for (i = 0; i != DJINN_HEAL_SPARKS; i++) {
    struct Particle *spark = &PARTICLES[i];

    SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                 ((Random16() & 0x7f) + 64) << 15,
                 ((Random16() & 0xff) - 127) << 15);
    spark->variant = 0;
  }
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, sheet, 0, 0);

  for (frame = 0; frame != args->targetCount * 8 + 72; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The Djinn sways over the targets, then rises away for the last 32
    // frames.
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (frame >= args->targetCount * 8 + 40)
      target[1] += toFIXED(4);
    point[0] = target[0];
    point[1] = target[1];
    point[2] = target[2] + Trig_Sin(frame << 11) * 40;
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17,
                 SMALL_DJINNI_WIDTH, 34);

    // Each target in turn, eight frames apart: its puffs close in around
    // it, and it flashes from 40 to 64 frames into its turn.
    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(args->targets[member])->object;
      s32 start = member * 8;

      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      position[0] = object->x;
      position[1] = toFIXED(40);
      position[2] = object->z;
      Matrix_TranslateVector(position);
      if (frame == start + 30)
        Sound_PlayFar(SE_RECOVERY);
      if (frame == start + 40)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, TINT_KEEP, 0);
      if (frame == start + 64)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
                                 TINT_KEEP, TINT_KEEP, 0);
      if (frame > start) {
        Matrix_RotateY((frame - start) << 9);
        for (i = 0; i != DJINN_HEAL_SPARKS_PER_TARGET; i++) {
          struct Particle *spark =
              &PARTICLES[member * DJINN_HEAL_SPARKS_PER_TARGET + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 cel;

              EffectPosition_ProjectPoint((s32 *)spark, &view);
              view.x >>= 1;
              /* LEFTOVER(template): the clamp and size by depth, idle here,
               * shared with BattleEffect_RunHealingMotes, which uses the size,
               * as the twin's Djinn Heal keeps it; as the clamp alone, 448 of
               * 744 instructions differ in tla-en. */
              CLAMP(view.depth, 314, 634);
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + gBattleEffect_SparklePictures[cel];
                u32 edge = gBattleEffect_SparkleSizes[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Tear down.
  EFFECT_FINISH_CANVAS();
}

/// Abilities: Balm.
void BattleEffect_RunBalm(struct BattleEffectParams *args) {
  BattleEffect_RunTumblingQuads(args, TUMBLING_QUADS_BALM);
}

/// Abilities: Tinder.
void BattleEffect_RunTinder(struct BattleEffectParams *args) {
  BattleEffect_RunTumblingQuads(args, TUMBLING_QUADS_TINDER);
}

/// Abilities: Phoenix, Regen Dance.
void BattleEffect_RunPhoenix(struct BattleEffectParams *args) {
  BattleEffect_RunTumblingQuads(args, TUMBLING_QUADS_PHOENIX);
}
#undef BLITTER
#undef GROUND_Y
#undef MOTE_MAX
#undef CONVERGES
#undef DECAYS
#undef SHEEP_COUNT
#undef SHEEP_FRAMES
#undef MARKER_FRAMES
#undef MARKER_STAGGER
#undef MARKER_CELS
