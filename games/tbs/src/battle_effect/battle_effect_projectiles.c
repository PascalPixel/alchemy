/// @file
/// The Djinni entrance that opens the Djinn's effects, and the projectile
/// volleys (Slash, Shuriken, Wing Flutter, Blizzard ...).
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The sparks the Djinni entrance sets out round the user, of which the first
/// ENTRANCE_SPARKS spin and gather.
#define ENTRANCE_SPARK_SLOTS 64
#define ENTRANCE_SPARKS 32
/// A spinning spark is drawn at size ENTRANCE_SPARK_SIZE_MAX at depth
/// ENTRANCE_SPARK_DEPTH_NEAR or nearer, one size smaller every
/// ENTRANCE_SPARK_DEPTH_STEP farther, its depth held below
/// ENTRANCE_SPARK_DEPTH_FAR.
#define ENTRANCE_SPARK_SIZE_MAX 8
#define ENTRANCE_SPARK_DEPTH_NEAR 250
#define ENTRANCE_SPARK_DEPTH_STEP 64
#define ENTRANCE_SPARK_DEPTH_FAR 634
/// The entrance's frames, and the long entrance's, which ends in a burst of
/// motes.
#define ENTRANCE_FRAMES 64
#define ENTRANCE_LONG_FRAMES 84

/// Sparks swirl around a point that travels from the user to the goal and
/// gather into a Djinni, which sways for 16 frames and bursts into motes.
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *args,
                                    s32 element, s32 side, s32 anchor,
                                    s32 *outX, s32 *outY) {
  s32 center[3];
  s32 goal[3];
  s32 step[3];
  s32 point[3];
  struct ScreenPoint position;
  struct BattleEffectState *state;
  void *canvas;
  BlitterPair draw;
  u8 *motes;
  s32 frameCount;
  struct Actor *target;
  struct Actor *user;
  u8 *resource;
  s32 sheetId;
  s32 frame;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  state->fadeStep = 0;
  if (element > ELEMENT_JUPITER) {
    element -= DJINNI_ENTRANCE_LONG;
    frameCount = ENTRANCE_LONG_FRAMES;
  } else {
    frameCount = ENTRANCE_FRAMES;
  }
  switch (element) {
  case ELEMENT_VENUS:
    sheetId = RESOURCE_VENUS_DJINN_SMALL_SHEET;
    break;
  case ELEMENT_MERCURY:
    sheetId = RESOURCE_MERCURY_DJINN_SMALL_SHEET;
    break;
  case ELEMENT_MARS:
    sheetId = RESOURCE_MARS_DJINN_SMALL_SHEET;
    break;
  default:
    sheetId = RESOURCE_JUPITER_DJINN_SMALL_SHEET;
    break;
  }
  resource = Resource_GetTableEntry(sheetId);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  Decompress_TaggedLz(resource + SHEET_PALETTE_BYTES, state->sheet);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
  if (side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
  }
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  user = BattleObject_GetFar(state->args->user)->actor;
  target = BattleObject_GetFar(state->args->targets[0])->actor;
  for (i = 0; i != ENTRANCE_SPARK_SLOTS; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    s32 speed = (Random16() & 255) + 128;

    SET_POSITION(spark, 0, INTtoFIXED((Random16() & 31) + 20), 0);
    SET_VELOCITY(spark, (Trig_Sin(angle) * speed) >> 5, 0,
                 (Trig_Cos(angle) * speed) >> 5);
    spark->variant = 0;
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  SET_VECTOR(center, user->x.fixed, 0, user->z.fixed);
  switch (anchor) {
  case DJINNI_ANCHOR_OVER_TARGET:
    SET_VECTOR(goal, target->x.fixed, toFIXED(60), target->z.fixed);
    break;
  case DJINNI_ANCHOR_TARGET_X:
    SET_VECTOR(goal, target->x.fixed, toFIXED(60), 0);
    break;
  case DJINNI_ANCHOR_OVER_USER:
    SET_VECTOR(goal, user->x.fixed, toFIXED(60), user->z.fixed);
    break;
  case DJINNI_ANCHOR_USER_X:
    SET_VECTOR(goal, user->x.fixed, toFIXED(60), 0);
    break;
  case DJINNI_ANCHOR_CENTRE:
    SET_VECTOR(goal, 0, toFIXED(60), 0);
    break;
  }
  SET_VECTOR(step, (goal[0] - center[0]) / 40, (goal[1] - center[1]) / 40,
             (goal[2] - center[2]) / 40);
  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    if (frame > 75)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(168 - frame * 2);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 6 && frame <= 45) {
      center[0] += step[0];
      center[1] += step[1];
      center[2] += step[2];
    }
    Matrix_TranslateVector(center);
    if (frame == 0)
      BattleEffect_TintBattler(state->args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 24)
      BattleEffect_TintBattler(state->args->user, SPRITE_COLOR_NORMAL,
                               TINT_KEEP, TINT_KEEP, 0);
    for (i = 0; i != ENTRANCE_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (frame >= i / 8 && spark->variant == 0) {
        s32 size;

        Matrix_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Matrix_RotateY(frame * ((i + 8) * (TRIG_TURN / 2048)));
          break;
        case SPIN_PITCH:
          Matrix_RotateX(-frame * ((i + 8) * (TRIG_TURN / 2048)));
          break;
        case SPIN_ROLL:
          Matrix_RotateZ(-frame * ((i + 8) * (TRIG_TURN / 2048)));
          break;
        case SPIN_PITCH_AND_ROLL:
          Matrix_RotateX(-frame * ((i + 8) * (TRIG_TURN / 2048)));
          Matrix_RotateZ(-frame * ((i + 8) * (TRIG_TURN / 2048)));
          break;
        }
        Projection_Point(&spark->x, &position);
        position.x >>= 1;
        Matrix_Pop();
        CLAMP(position.depth, ENTRANCE_SPARK_DEPTH_NEAR,
              ENTRANCE_SPARK_DEPTH_FAR);
        size = ENTRANCE_SPARK_SIZE_MAX -
               (position.depth - ENTRANCE_SPARK_DEPTH_NEAR) /
                   ENTRANCE_SPARK_DEPTH_STEP;
        EFFECT_DRAW_MOTE(draw[1], canvas, motes, position.x, position.y, size);
        Particle_AdvanceWithGravity3D(spark, 60, 0);
        if (frame >= i / 8 + 24) {
          s32 pullX = -spark->x >> 7;
          s32 pullY = -spark->y >> 7;
          s32 pullZ = -spark->z >> 7;

          spark->velocityX += pullX;
          spark->velocityY += pullY;
          spark->velocityZ += pullZ;
          SET_VELOCITY(spark, spark->velocityX * 62 / DAMPING_SCALE,
                       spark->velocityY * 62 / DAMPING_SCALE,
                       spark->velocityZ * 62 / DAMPING_SCALE);
          if ((pullX > -(toFIXED(1) / 32) && pullX < toFIXED(1) / 32) &&
              (pullZ > -(toFIXED(1) / 32) && pullZ < toFIXED(1) / 32))
            spark->variant = -1;
        }
      }
    }
    // The Djinni sways over the goal.
    if (frame >= 54 && frame <= 69) {
      SET_VECTOR(point, (Trig_Sin(frame * (TRIG_TURN / 64)) * 4), 0, 0);
      Projection_Point(point, &position);
      *outX = position.x;
      *outY = position.y;
      position.x >>= 1;
      draw[0](canvas, state->sheet, position.x - SMALL_DJINNI_WIDTH / 2,
              position.y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
              SMALL_DJINNI_HEIGHT);
    }
    // The long version's Djinni bursts into motes.
    if (frame == ENTRANCE_FRAMES) {
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        struct Particle *mote = &state->particles[i];
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 speed = (Random16() & 255) + 128;

        mote->x = *outX << 15;
        mote->y = INTtoFIXED(*outY);
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    if (frame >= ENTRANCE_FRAMES) {
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        struct Particle *mote = &state->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          EFFECT_DRAW_MOTE(draw[0], canvas, motes, FIXEDtoINT(mote->x),
                           FIXEDtoINT(mote->y), size);
          Particle_AdvanceWithGravity2D(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_DarkenBackgroundStep);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
}

#define PROJECTILE_VOLLEY_ANIMATION_STEP_FAST                                  \
  (ANIM_STEP_NORMAL * 3) ///< the user's, while firing

/// Six comets fly at each target, three frames apart, and burst into a flash
/// and sparks; the effect's variant picks one of three comets.
///
/// Abilities: Slash, Wind Slash, Sonic Slash.
void BattleEffect_RunSlash(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_SLASH);
}
/// Four spinning shurikens fly at each target, five frames apart, and vanish
/// once past it.
///
/// Abilities: Shuriken.
void BattleEffect_RunShuriken(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_SHURIKEN);
}
/// Ten pellets fly slowly at each target and bounce off it, up or down at
/// random.
///
/// Abilities: Crazy Voice.
void BattleEffect_RunCrazyVoice(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_CRAZY_VOICE);
}
/// Five yellow sparks fly level at each target, five frames apart.
///
/// Abilities: Wing Flutter.
void BattleEffect_RunWingFlutter(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_WING_FLUTTER);
}
/// Ten large yellow sparks stream level at each target, one a frame, spread up
/// and down.
///
/// Abilities: Wing Stroke.
void BattleEffect_RunWingStroke(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_WING_STROKE);
}
/// Ten small yellow sparks stream level at each target, one a frame.
///
/// Abilities: Wing Beat.
void BattleEffect_RunWingBeat(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_WING_BEAT);
}
/// Fires the volley from the user's leap pose.
static inline void
BattleEffect_RunVolleyWithAction(struct BattleEffectParams *args, s32 mode) {
  struct Actor *user;

  user = BattleObject_GetFar(args->user)->actor;
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, PROJECTILE_VOLLEY_ANIMATION_STEP_FAST);
  BattleEffect_RunProjectileVolley(args, mode);
  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL);
}

/// From the user's action pose, one large comet in ice colours flies at each
/// target and bursts into 80 sparks.
///
/// Abilities: Cutting Edge.
void BattleEffect_RunCuttingEdge(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_CUTTING_EDGE);
}
/// From the user's action pose, seven fireballs trailing embers fly at each
/// target, two frames apart, each bursting into 40 sparks.
///
/// Abilities: Heat Wave.
void BattleEffect_RunHeatWave(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_HEAT_WAVE);
}
/// Thirty ice chips fall from high above the user onto each target.
///
/// Abilities: Sleet.
void BattleEffect_RunSleet(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_SLEET);
}
/// Sixty ice chips stream at each target, one a frame, without knocking it
/// back.
///
/// Abilities: Ice Blessing.
void BattleEffect_RunIceBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_ICE_BLESSING);
}
/// From the user's action pose, sixty ice chips stream at each target, one a
/// frame, scattered up and down, without knocking it back.
///
/// Abilities: Blizzard.
void BattleEffect_RunBlizzard(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_BLIZZARD);
}

#define PROJECTILE_VOLLEY_PARTICLE_COUNT PARTICLE_POOL_COUNT

/// The bytes of a gProjectileVolley_Modes row.
enum VolleyField {
  PROJECTILE_VOLLEY_SHOTS,   ///< shots at each target
  PROJECTILE_VOLLEY_STEPS,   ///< frames a shot takes to reach its target
  PROJECTILE_VOLLEY_STAGGER, ///< frames between shots
  PROJECTILE_VOLLEY_DELAY,   ///< frames between targets
  PROJECTILE_VOLLEY_SPARKS   ///< sparks thrown on each hit
};

#define PROJECTILE_VOLLEY_COMET_PICTURES 4096  ///< comets: the top half of each
#define PROJECTILE_VOLLEY_STREAK_PICTURES 8192 ///< wind streaks
/// The hit flashes turned on their side, after the upright ones.
#define PROJECTILE_VOLLEY_FLASH_TURNED                                         \
  (12 * 10 + 18 * 15 + 24 * 18 + 30 * 22 + 36 * 27 + 39 * 28 + 40 * 30)
/// The yellow spark sprite sheet: the small spark first, then the medium one,
/// then the large one.
#define PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_WIDTH 12
#define PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_HEIGHT 8
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_WIDTH 18
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_HEIGHT 13
#define PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_WIDTH 29
#define PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_HEIGHT 23
#define PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE 0
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE                                 \
  PICTURE_BYTES(PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE)
#define PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE                                  \
  (PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE +                                    \
   PICTURE_BYTES(PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE))
/// The comets' width, and the shuriken's six pictures.
#define PROJECTILE_VOLLEY_COMET_WIDTH 32
#define SHURIKEN_WIDTH 32
#define SHURIKEN_HEIGHT 24
#define PROJECTILE_VOLLEY_DJINNI_PICTURE 26048 ///< the small Mercury Djinni
#define PROJECTILE_VOLLEY_FIRST_SPARK 100      ///< the first spark in PARTICLES
#define PROJECTILE_VOLLEY_FIRST_TRAIL 512      ///< the first trail in PARTICLES
/// The most shots a volley fires at all its targets together, one fewer than
/// the effect's particles; past it, the volley fires at the first target alone.
#define PROJECTILE_VOLLEY_SHOTS_MAX 63
/// A hit's sparks fly at up to 4 pixels a frame each way, as a random -128 to
/// 127 shifted up; Cutting Edge's at up to 8.
#define PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT 11
#define CUTTING_EDGE_SPARK_SPEED_SHIFT 12

/// Shots fly from the user to each target and burst into a flash and sparks
/// when they pass it. The hit sound plays at most once every 8 frames.
void BattleEffect_RunProjectileVolley(struct BattleEffectParams *args,
                                      s32 mode) {
  struct ScreenPoint position;
  s32 screenX;
  s32 screenY;
  struct BattleEffectState *state;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *motes;
  s32 soundCooldown;
  s32 total;
  s32 comet;
  s32 shots;
  s32 steps;
  struct Actor *user;
  s32 k;
  s32 trailsOrShift;
  s32 hit;
  s32 x;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  soundCooldown = 0;
  state->args = args;
  comet = args->variant;
  if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (state->args->prelude == 1)
    BattleEffect_RunDjinniEntrance(args, ELEMENT_MERCURY, state->args->side,
                                   DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);

  if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
    if (state->args->side == EFFECT_SIDE_PARTY)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
  } else {
    if (state->args->side == EFFECT_SIDE_PARTY)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
  }
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
  if (mode == PROJECTILE_VOLLEY_SLASH ||
      mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
      mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
      comet = PROJECTILE_VOLLEY_COMET_C;
    if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
      comet = PROJECTILE_VOLLEY_COMET_A;
    if (comet == PROJECTILE_VOLLEY_COMET_A)
      Resource_LoadAndDecompress(
          RESOURCE_SLASH_COMET_SHEET,
          state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
    else if (comet == PROJECTILE_VOLLEY_COMET_B)
      Resource_LoadAndDecompress(
          RESOURCE_WIND_SLASH_COMET_SHEET,
          state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
    else
      Resource_LoadAndDecompress(
          RESOURCE_SONIC_SLASH_COMET_SHEET,
          state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      const void *pal = Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
      Iwram_CopyWords(BG_PLTT, pal, SHEET_PALETTE_BYTES);
    }
    Resource_LoadAndDecompress(RESOURCE_WIND_STREAK_SHEET,
                               state->sheet + PROJECTILE_VOLLEY_STREAK_PICTURES,
                               1, 0);
    // The ice colours again, though the streaks left them as they were.
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      const void *pal = Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
      Iwram_CopyWords(BG_PLTT, pal, SHEET_PALETTE_BYTES);
    }
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
    Resource_LoadAndDecompress(RESOURCE_SHURIKEN_SHEET, state->sheet, 1, 1);
    REG_BLDCNT = 0;
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
    Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_SLASH_COMET_SHEET),
                    SHEET_PALETTE_BYTES);
    Resource_LoadAndDecompress(RESOURCE_PROJECTILE_VOLLEY_IMAGE, state->sheet,
                               0, 0);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
             mode == PROJECTILE_VOLLEY_WING_BEAT ||
             mode == PROJECTILE_VOLLEY_WING_STROKE) {
    Resource_LoadAndDecompress(RESOURCE_YELLOW_SPARK_SHEET, state->sheet, 1, 1);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
    Resource_LoadAndDecompress(RESOURCE_FIREBALL_SHEET, state->sheet, 1, 1);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, state->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
                               state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE,
                               1, 0);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  }
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);

  user = BattleObject_GetFar(state->args->user)->actor;
  shots = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_SHOTS]);
  steps = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_STEPS]);
  {
    s32 stagger = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_STAGGER]);
    s32 delay = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_DELAY]);
    s32 height = BattleUnit_GetEffectHeightFar(state->args->user);

    if (state->args->targetCount * shots > PROJECTILE_VOLLEY_SHOTS_MAX)
      state->args->targetCount = 1;

    // Each shot starts at the user and is aimed at its target.

    EFFECT_FOR_EACH_TARGET(i, state->args) {
      struct Actor *target =
          BattleObject_GetFar(state->args->targets[i])->actor;
      s32 targetHeight = BattleUnit_GetEffectHeightFar(state->args->targets[i]);

      for (j = 0; j != shots; j++) {
        struct Particle *shot = &state->particles[i * shots + j];

        shot->x = user->x.fixed;
        if (mode == PROJECTILE_VOLLEY_SLEET)
          shot->y = user->y.fixed + INTtoFIXED(Random16() & 15) + toFIXED(58);
        else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
          shot->y = user->y.fixed + height / 2;
        else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
          shot->y = user->y.fixed + height / 2;
        else if (mode == PROJECTILE_VOLLEY_BLIZZARD)
          shot->y =
              user->y.fixed + height / 2 + INTtoFIXED(16 - (Random16() & 31));
        else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
                 mode == PROJECTILE_VOLLEY_WING_BEAT)
          shot->y =
              user->y.fixed + height / 2 + INTtoFIXED(16 - (Random16() & 31));
        else if (mode == PROJECTILE_VOLLEY_WING_STROKE)
          shot->y =
              user->y.fixed + height / 2 + INTtoFIXED(32 - (Random16() & 63));
        else if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
          shot->y = user->y.fixed + height / 2;
        else
          shot->y = user->y.fixed + height;
        shot->z = user->z.fixed;
        shot->velocityX = (target->x.fixed - shot->x) / steps;
        if (mode == PROJECTILE_VOLLEY_SLEET)
          shot->velocityY =
              (s32)(target->y.fixed + INTtoFIXED(Random16() & 63) - shot->y -
                    toFIXED(12)) /
              steps;
        else if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
          shot->velocityY = (s32)(target->y.fixed + INTtoFIXED(Random16() & 7) -
                                  shot->y + toFIXED(22)) /
                            steps;
        else if (mode == PROJECTILE_VOLLEY_BLIZZARD)
          shot->velocityY =
              (s32)(target->y.fixed + INTtoFIXED(64 - (Random16() & 63)) -
                    shot->y) /
              steps;
        else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
          shot->velocityY =
              (s32)(target->y.fixed + INTtoFIXED(Random16() & 31) - shot->y +
                    toFIXED(4)) /
              steps;
        else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
                 mode == PROJECTILE_VOLLEY_WING_BEAT ||
                 mode == PROJECTILE_VOLLEY_WING_STROKE ||
                 mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
          shot->velocityY = 0;
        else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
          shot->velocityY = (s32)(target->y.fixed + targetHeight / 2 +
                                  INTtoFIXED(Random16() & 15) - shot->y) /
                            steps;
        else
          shot->velocityY = (s32)(target->y.fixed + targetHeight -
                                  INTtoFIXED(Random16() & 15) - shot->y) /
                            steps;
        shot->velocityZ = (target->z.fixed - shot->z) / steps;
        shot->variant = 0;
      }
    }
    for (i = 0; i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++)
      PARTICLES[i].variant = 0;
    if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
      total = state->args->targetCount * delay + shots * stagger + 32;
    else
      total = state->args->targetCount * delay + shots * stagger + 16;

    for (frame = 0; frame != total; frame++) {
      s32 index;

      if (soundCooldown > 0)
        soundCooldown--;
      if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
        if (frame == 4)
          Sound_PlayFar(SE_WHOOSH);
        if (frame == 32)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      } else if (mode == PROJECTILE_VOLLEY_SLEET) {
        if (frame == 48)
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
      } else if (mode != PROJECTILE_VOLLEY_CUTTING_EDGE) {
        if (frame == 16)
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
      }

      // The prelude's small Djinni circles, then rises away.
      if (state->args->prelude == 1) {
        s32 x = FIXEDtoINT(-Trig_Sin(frame * (TRIG_TURN / 32)) * 4) +
                screenX / 2 - 10;
        s32 y =
            FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 2) + screenY - 24;

        if (frame > 69)
          y = y - frame * 2 + 138;
        if (state->args->side == EFFECT_SIDE_PARTY)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_ADDITIVE);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_ADDITIVE);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
        // Drawn twice, so brighter, over its first four frames.
        if (frame <= 3)
          draw[1](canvas, state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE, x, y,
                  SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        draw[1](canvas, state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }

      EFFECT_VIEW_FROM_CAMERA(camera);
      index = 0;
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        struct Actor *target =
            BattleObject_GetFar(state->args->targets[i])->actor;

        for (j = 0; j != shots; j++, index++) {
          if (j * stagger + i * delay < frame) {
            struct Particle *shot = &state->particles[i * shots + j];

            Projection_Point(shot, &position);
            position.x >>= 1;
            shot->x += shot->velocityX;
            shot->y += shot->velocityY;
            shot->z += shot->velocityZ;
            if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
              // A fireball leaves a trail of embers.
              trailsOrShift = 0;
              for (k = PROJECTILE_VOLLEY_FIRST_TRAIL;
                   k != PROJECTILE_VOLLEY_PARTICLE_COUNT; k++) {
                struct Particle *particle = &PARTICLES[k];

                if (particle->variant == 0) {
                  particle->x = INTtoFIXED(position.x);
                  particle->y = INTtoFIXED(position.y);
                  particle->velocityX = ((Random16() & 255) - 128) << 11;
                  particle->velocityY = ((Random16() & 255) - 128) << 11;
                  particle->variant = (Random16() & 7) + 32;
                  trailsOrShift++;
                  if (trailsOrShift == 2)
                    break;
                }
              }
            }
            if (shot->variant == 0) {
              // A shot hits once it passes its target.
              x = shot->x;
              if (target->x.fixed < 0) {
                hit = x < 0;
                /* FAKEMATCH(dead): two updates that cancel, under a test that
                 * does nothing. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              } else {
                hit = x >= 0;
                /* FAKEMATCH(dead): the same pair for the other side. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              }
              if (ABS(x) >= ABS(target->x.fixed)) {
                s32 spawned = 0;
                struct Particle *flash = &PARTICLES[index];

                shot->variant = 1;
                if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
                  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                } else if (mode != PROJECTILE_VOLLEY_HEAT_WAVE &&
                           soundCooldown == 0) {
                  soundCooldown = 8;
                  Sound_PlayFar(SE_WATER);
                }
                // The hit: a flash, a shake, and the target flinches.
                if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
                  // Pellets bounce off, up or down.
                  s32 kick = ((Random16() & 31) + 32) << 12;

                  if (Random16() & 1)
                    shot->velocityY += kick;
                  else
                    shot->velocityY -= kick;
                  shot->velocityX = -shot->velocityX;
                }
                flash->variant = 1;
                SET_POSITION(flash, position.x, position.y, 0);
                if (mode != PROJECTILE_VOLLEY_SLEET)
                  state->shakeFrames = 2;
                BattleEffect_TintBattler(state->args->targets[i],
                                         SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
                if (mode != PROJECTILE_VOLLEY_SLEET &&
                    mode != PROJECTILE_VOLLEY_BLIZZARD &&
                    mode != PROJECTILE_VOLLEY_ICE_BLESSING) {
                  if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
                    BattleMotion_ApplyKnockbackFar(state->args->targets[i],
                                                   KNOCKBACK_HURL);
                  else
                    BattleMotion_ApplyKnockbackFar(state->args->targets[i],
                                                   KNOCKBACK_SKID);
                }
                // Sparks fly from the hit.
                trailsOrShift = mode == PROJECTILE_VOLLEY_CUTTING_EDGE
                                    ? CUTTING_EDGE_SPARK_SPEED_SHIFT
                                    : PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT;
                for (k = PROJECTILE_VOLLEY_FIRST_SPARK;
                     k != PROJECTILE_VOLLEY_FIRST_TRAIL; k++) {
                  struct Particle *particle = &PARTICLES[k];

                  if (particle->variant == 0) {
                    particle->x = INTtoFIXED(position.x);
                    particle->y = INTtoFIXED(position.y);
                    particle->velocityX = ((Random16() & 255) - 128)
                                          << trailsOrShift;
                    particle->velocityY = ((Random16() & 255) - 128)
                                          << trailsOrShift;
                    particle->variant = (Random16() & 7) + 16;
                    spawned++;
                    if (spawned ==
                        (gProjectileVolley_Modes[mode]
                                                [PROJECTILE_VOLLEY_SPARKS]))
                      break;
                  }
                }
              }
            }
          drawShot:
            if (mode == PROJECTILE_VOLLEY_SLASH ||
                mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
                mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
              if (state->args->side == EFFECT_SIDE_PARTY)
                EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                    COMPOSITE_LIGHTEN);
              else
                EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                    EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
              EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
                  canvas, state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                  position.x - PROJECTILE_VOLLEY_COMET_WIDTH / 2,
                  position.y - gProjectileVolley_CometHeights[comet],
                  PROJECTILE_VOLLEY_COMET_WIDTH,
                  gProjectileVolley_CometHeights[comet]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              if (state->args->side == EFFECT_SIDE_PARTY)
                EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                    COMPOSITE_LIGHTEN);
              else
                EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                                    COMPOSITE_LIGHTEN);
              EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
                  canvas, state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                  position.x - PROJECTILE_VOLLEY_COMET_WIDTH / 2, position.y,
                  PROJECTILE_VOLLEY_COMET_WIDTH,
                  gProjectileVolley_CometHeights[comet]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
              if (ABS(shot->x) <= ABS(target->x.fixed))
                draw[0](canvas,
                        state->sheet + frame % 6 * PICTURE_BYTES(SHURIKEN),
                        position.x - SHURIKEN_WIDTH / 2,
                        position.y - SHURIKEN_HEIGHT / 2, SHURIKEN_WIDTH,
                        SHURIKEN_HEIGHT);
            } else if (mode == PROJECTILE_VOLLEY_SLEET ||
                       mode == PROJECTILE_VOLLEY_BLIZZARD ||
                       mode == PROJECTILE_VOLLEY_ICE_BLESSING) {
              s32 picture = j & 3;
              draw[0](canvas,
                      state->sheet + gProjectileVolley_PelletPictures[picture],
                      position.x - 4, position.y - 4, 8, 8);
            } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
              draw[0](canvas, state->sheet + j % 6 * (8 * 16), position.x - 4,
                      position.y - 8, 8, 16);
            } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER) {
              EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                  PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE,
                                  position.x - 9, position.y - 7);
            } else if (mode == PROJECTILE_VOLLEY_WING_BEAT) {
              EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                  PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE,
                                  position.x - 6, position.y - 4);
            } else if (mode == PROJECTILE_VOLLEY_WING_STROKE) {
              EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
                                  PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE,
                                  position.x - 15, position.y - 12);
            } else {
              draw[0](canvas, state->sheet, position.x - FLAMEBALL_WIDTH / 2,
                      position.y - FLAMEBALL_HEIGHT / 2, FLAMEBALL_WIDTH,
                      FLAMEBALL_HEIGHT);
            }
          }
        }
      }

      if (mode == PROJECTILE_VOLLEY_SLASH ||
          mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
          mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
        for (i = 0; i != state->args->targetCount * shots; i++) {
          struct Particle *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  COMPOSITE_LIGHTEN);
              EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
                  canvas, state->sheet + gProjectileVolley_FlashPictures[step],
                  flash->x - gProjectileVolley_FlashWidths[step],
                  flash->y - gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_LIGHTEN);
              EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
                  canvas, state->sheet + gProjectileVolley_FlashPictures[step],
                  flash->x,
                  flash->y + gProjectileVolley_FlashSteps[step] -
                      gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            } else {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  COMPOSITE_LIGHTEN);
              EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
                  canvas,
                  state->sheet + gProjectileVolley_FlashPictures[step] +
                      PROJECTILE_VOLLEY_FLASH_TURNED,
                  flash->x - gProjectileVolley_FlashRises[step],
                  flash->y - gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step],
                  gProjectileVolley_FlashWidths[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_LIGHTEN);
              EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
                  canvas,
                  state->sheet + gProjectileVolley_FlashPictures[step] +
                      PROJECTILE_VOLLEY_FLASH_TURNED,
                  flash->x + gProjectileVolley_FlashSteps[step] -
                      gProjectileVolley_FlashRises[step],
                  flash->y, gProjectileVolley_FlashHeights[step],
                  gProjectileVolley_FlashWidths[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce, shrinking as they die.
      for (i = PROJECTILE_VOLLEY_FIRST_SPARK;
           i != PROJECTILE_VOLLEY_FIRST_TRAIL; i++) {
        struct Particle *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 3) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, motes, FIXEDtoINT(particle->x),
                           FIXEDtoINT(particle->y), size);
          Particle_AdvanceWithGravity2D(particle, 60, toFIXED(1) / 16);
          if (particle->y > toFIXED(112))
            particle->velocityY = -particle->velocityY / 2;
          particle->variant--;
        }
      }

      // Fireball trails rise and fade.
      for (i = PROJECTILE_VOLLEY_FIRST_TRAIL;
           i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++) {
        struct Particle *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 4) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, motes, FIXEDtoINT(particle->x),
                           FIXEDtoINT(particle->y), size);
          Particle_AdvanceWithGravity2D(particle, 60, -toFIXED(1) / 4);
          particle->variant--;
        }
      }
      BattleCamera_Shake(4, 4);
      EFFECT_END_FRAME(state);
    }
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Sends a unit back to its place (its anchor) with a hop.
UNUSED void BattleEffect_HopBackToSpot(s32 id) {
  struct BattleObject *slot = BattleObject_GetFar(id);
  struct Actor *actor = slot->actor;

  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(8);
  actor->velocityY = toFIXED(4);
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  actor->snapToTarget = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, slot->anchorX, 0, slot->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_STANDING);
}

/// How far BattleEffect_SetApproachMotion takes a unit toward the other.
#define APPROACH_PERCENT 80

/// Moves a unit APPROACH_PERCENT of the way to another one over frames, in
/// its leap pose.
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames) {
  struct BattleObject *slot = BattleObject_GetFar(id);
  struct BattleObject *targetSlot = BattleObject_GetFar(targetId);
  struct Actor *actor = slot->actor;
  struct Actor *target = targetSlot->actor;
  s32 rate = APPROACH_PERCENT;
  FIXED diffX = target->x.fixed - actor->x.fixed;
  FIXED startX = actor->x.fixed;
  FIXED dx = rate * diffX / 100;
  FIXED diffZ = target->z.fixed - actor->z.fixed;
  FIXED startZ = actor->z.fixed;
  FIXED dz = rate * diffZ / 100;
  FIXED x = startX + dx;
  FIXED z = startZ + dz;
  s32 stepX = dx >> 8;
  s32 stepZ = dz >> 8;
  s32 speed;

  speed = Iwram_Sqrt(stepX * stepX + stepZ * stepZ);
  speed = (speed << 8) / frames;
  actor->acceleration = speed;
  actor->speedLimit = speed;
  actor->snapToTarget = 1;
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->velocityY = 0;
  actor->bounce = 0;
  actor->steeringFlags = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, x, 0, z);
  Actor_SelectAnimationFar(actor, POSE_LEAP);
}

#undef PROJECTILE_VOLLEY_ANIMATION_STEP_FAST
#undef PROJECTILE_VOLLEY_PARTICLE_COUNT
#undef PROJECTILE_VOLLEY_COMET_PICTURES
#undef PROJECTILE_VOLLEY_STREAK_PICTURES
#undef PROJECTILE_VOLLEY_FLASH_TURNED
#undef PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_DJINNI_PICTURE
#undef PROJECTILE_VOLLEY_FIRST_SPARK
#undef PROJECTILE_VOLLEY_FIRST_TRAIL
#undef PROJECTILE_VOLLEY_SHOTS_MAX
#undef PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT
#undef CUTTING_EDGE_SPARK_SPEED_SHIFT
#undef APPROACH_PERCENT
#undef ENTRANCE_SPARK_SLOTS
#undef ENTRANCE_SPARKS
#undef ENTRANCE_SPARK_SIZE_MAX
#undef ENTRANCE_SPARK_DEPTH_NEAR
#undef ENTRANCE_SPARK_DEPTH_STEP
#undef ENTRANCE_SPARK_DEPTH_FAR
#undef ENTRANCE_FRAMES
#undef ENTRANCE_LONG_FRAMES
