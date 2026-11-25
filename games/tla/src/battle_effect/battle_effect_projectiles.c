/// @file
/// The opening every Djinn effect shares, Cutting Edge, Heat Wave and Blizzard,
/// and the projectile volleys.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The axis each of the Djinni entrance's sparks spins on, by its number.
enum SpinAxis { SPIN_YAW, SPIN_PITCH, SPIN_ROLL, SPIN_PITCH_AND_ROLL };

/// The sparks that spin into the Djinni, the frames their centre takes from
/// the user to the anchor, and the depths that size them: one size smaller
/// every DJINNI_SPARK_DEPTH_STEP from DJINNI_SPARK_DEPTH_NEAR, held below
/// DJINNI_SPARK_DEPTH_FAR.
#define DJINNI_ENTRANCE_SPARKS 32
#define DJINNI_ENTRANCE_TRAVEL_FRAMES 40
#define DJINNI_SPARK_DEPTH_NEAR 250
#define DJINNI_SPARK_DEPTH_STEP 64
#define DJINNI_SPARK_DEPTH_FAR 634

/// Sparks swirl around a point that travels from the user to the anchor and
/// gather into a Djinni, which sways for 16 frames and bursts into motes.
/// packed is DJINNI_ENTRANCE's; the anchor's screen point goes to screen and
/// its world point to world.
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *args, u32 packed,
                                    s32 *screen, s32 *world) {
  FIXED center[3];
  FIXED goal[3];
  FIXED step[3];
  s32 point[3];
  struct ScreenPoint position;
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle draw[2];
  u8 *sheet;
  s32 frameCount;
  struct MotionObject *target;
  struct MotionObject *actor;
  u8 *palette;
  s32 resource;
  s32 frame;
  s32 i;
  s32 element;
  s32 side;
  s32 anchor;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  element = DJINNI_ENTRANCE_ELEMENT(packed);
  side = DJINNI_ENTRANCE_SIDE(packed);
  anchor = DJINNI_ENTRANCE_ANCHOR(packed);
  if (anchor == DJINNI_ANCHOR_CENTRE_BRIGHTENING) {
    frameCount = 40;
  } else if (anchor == DJINNI_ANCHOR_OVER_TARGET_SHORT) {
    state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    state->fadeStep = 0;
    frameCount = 54;
  } else {
    state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    state->fadeStep = 0;
    if (element > ELEMENT_JUPITER) {
      element -= DJINNI_ENTRANCE_LONG;
      frameCount = 84;
    } else {
      frameCount = 55;
    }
  }

  // The Djinni's palette, then its small picture.
  switch (element) {
  case ELEMENT_VENUS:
    resource = RESOURCE_BLAST_SHEET;
    break;
  case ELEMENT_MERCURY:
    resource = RESOURCE_MERCURY_DJINN_SMALL_SHEET;
    break;
  case ELEMENT_MARS:
    resource = RESOURCE_MARS_DJINN_SMALL_SHEET;
    break;
  default:
    resource = RESOURCE_JUPITER_DJINN_SMALL_SHEET;
    break;
  }
  palette = Resource_GetTableEntry(resource);
  Iwram_CopyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  switch (element) {
  case ELEMENT_VENUS:
    palette = Resource_GetTableEntry(RESOURCE_VENUS_DJINN_SMALL_SHEET);
    break;
  case ELEMENT_MERCURY:
    palette = Resource_GetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
    break;
  case ELEMENT_MARS:
    palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET);
    break;
  default:
    palette = Resource_GetTableEntry(RESOURCE_JUPITER_DJINN_SMALL_SHEET);
    break;
  }
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, state->sheet);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);

  // Additive and max blitters, clipped, and mirrored when the party casts.
  if (side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        BLITTER_MAX);
  }
  draw[0] = Ram_HeapSlots->ramCode;
  draw[1] = Ram_HeapSlots->ramCodePaired;
  actor = BattleObject_GetFar(args->user)->object;
  target = BattleObject_GetFar(args->targets[0])->object;

  // 64 sparks start on the ground around the caster, flying outward.
  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    struct Particle *spark = &state->particles[i];
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    s32 speed = (Random16() & 255) + 128;

    SET_POSITION(spark, 0, INTtoFIXED((Random16() & 31) + 20), 0);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(angle) * speed) >> 5;
    spark->variant = 0;
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // The spin's centre moves from the caster to the anchor over frames 6-45.
  center[0] = actor->x;
  center[1] = 0;
  center[2] = actor->z;
  switch (anchor) {
  case DJINNI_ANCHOR_OVER_TARGET:
    goal[0] = target->x;
    goal[1] = toFIXED(60);
    goal[2] = target->z;
    break;
  case DJINNI_ANCHOR_OVER_TARGET_SHORT:
    goal[0] = target->x;
    goal[1] = toFIXED(60);
    goal[2] = target->z;
    break;
  case DJINNI_ANCHOR_TARGET_X:
    goal[0] = target->x;
    goal[1] = toFIXED(60);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_HIGH_TARGET_X:
    goal[0] = target->x;
    goal[1] = toFIXED(120);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_OVER_USER:
    goal[0] = actor->x;
    goal[1] = toFIXED(60);
    goal[2] = actor->z;
    break;
  case DJINNI_ANCHOR_USER_X:
    goal[0] = actor->x;
    goal[1] = toFIXED(60);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_CENTRE:
  case DJINNI_ANCHOR_CENTRE_BRIGHTENING:
    goal[0] = 0;
    goal[1] = toFIXED(60);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_LOW_CENTRE:
    goal[0] = 0;
    goal[1] = toFIXED(20);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_HIGH_USER_X:
    goal[0] = actor->x;
    goal[1] = toFIXED(80);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_LOW_USER_X:
    goal[0] = actor->x;
    goal[1] = toFIXED(30);
    goal[2] = 0;
    break;
  }
  step[0] = (goal[0] - center[0]) / DJINNI_ENTRANCE_TRAVEL_FRAMES;
  step[1] = (goal[1] - center[1]) / DJINNI_ENTRANCE_TRAVEL_FRAMES;
  step[2] = (goal[2] - center[2]) / DJINNI_ENTRANCE_TRAVEL_FRAMES;

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;

    if (frame > 75)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(168 - frame * 2);
    if (anchor == DJINNI_ANCHOR_CENTRE_BRIGHTENING && frame > 23)
      Palette_BrightenBattleBg(2, 2, 2);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (frame >= 6 && frame <= 45) {
      center[0] += step[0];
      center[1] += step[1];
      center[2] += step[2];
    }
    Matrix_TranslateVector(center);
    if (frame == 0)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 24)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);

    // The sparks spin about the centre, each on its own axis, and from their
    // 24th frame fall into it; a spark that has arrived stops.
    if (frame <= 63) {
      for (i = 0; i != DJINNI_ENTRANCE_SPARKS; i++) {
        struct Particle *spark = &state->particles[i];

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
          EffectPosition_ProjectPoint(&spark->x, &position);
          position.x >>= 1;
          Matrix_Pop();
          CLAMP(position.depth, DJINNI_SPARK_DEPTH_NEAR,
                DJINNI_SPARK_DEPTH_FAR);
          size = 8 - (position.depth - DJINNI_SPARK_DEPTH_NEAR) /
                         DJINNI_SPARK_DEPTH_STEP;
          EFFECT_DRAW_MOTE(draw[1], canvas, sheet, position.x, position.y,
                           size);
          Particle_AdvanceWithGravity3D(spark, 60, 0);
          if (frame >= i / 8 + 24) {
            s32 pullX = -spark->x >> 7;
            s32 pullY = -spark->y >> 7;
            s32 pullZ = -spark->z >> 7;

            spark->velocityX += pullX;
            spark->velocityY += pullY;
            spark->velocityZ += pullZ;
            spark->velocityX = spark->velocityX * 62 / DAMPING_SCALE;
            spark->velocityY = spark->velocityY * 62 / DAMPING_SCALE;
            spark->velocityZ = spark->velocityZ * 62 / DAMPING_SCALE;
            if ((pullX > -2048 && pullX < 2048) &&
                (pullZ > -2048 && pullZ < 2048))
              spark->variant = -1;
          }
        }
      }
    }

    // The small Djinni sways at the anchor for frames 54 to 69.
    if (frame >= 54 && frame <= 69) {
      point[0] = Trig_Sin(frame * (TRIG_TURN / 64)) * 4;
      point[1] = 0;
      point[2] = 0;
      world[0] = goal[0] + Trig_Sin(frame * (TRIG_TURN / 64)) * 4;
      world[1] = goal[1];
      world[2] = goal[2];
      EffectPosition_ProjectPoint(point, &position);
      screen[0] = position.x;
      screen[1] = position.y;
      position.x >>= 1;
      draw[0](canvas, state->sheet, position.x - SMALL_DJINNI_WIDTH / 2,
              position.y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
              SMALL_DJINNI_HEIGHT);
    }

    // At frame 64 it bursts into 64 motes that fall away.
    if (frame == 64) {
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        struct Particle *mote = &state->particles[i];
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 speed = (Random16() & 255) + 128;

        mote->x = screen[0] * (toFIXED(1) / 2);
        mote->y = INTtoFIXED(screen[1]);
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    if (frame > 63) {
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        struct Particle *mote = &state->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, (s16)FIXEDtoINT(mote->x),
                           (s16)FIXEDtoINT(mote->y), size);
          Particle_AdvanceWithGravity2D(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(state);
  }
  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
}

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

/// Ten musical notes fly slowly at each target and bounce off it, up or down
/// at random.
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

/// Fires the volley from the user's leap pose, animated three times as fast.
static inline void
BattleEffect_RunVolleyWithAction(struct BattleEffectParams *args, s32 mode) {
  struct Actor *actor;

  actor = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(actor, POSE_LEAP);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, mode);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL);
}

/// From the user's leap pose, one large comet in ice colours flies at each
/// target and bursts into 80 sparks.
///
/// Abilities: Cutting Edge.
void BattleEffect_RunCuttingEdge(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_CUTTING_EDGE);
}

/// From the user's leap pose, seven fireballs trailing embers fly at each
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

/// From the user's leap pose, sixty ice chips stream at each target, one a
/// frame, scattered up and down, without knocking it back.
///
/// Abilities: Blizzard.
void BattleEffect_RunBlizzard(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_BLIZZARD);
}

/// From the user's leap pose, seven fireballs trailing smoke fly level at each
/// target, two frames apart, scattered up and down, each bursting into 40
/// sparks.
///
/// Abilities: Steel, Blaze Rush.
void BattleEffect_RunBlazeRush(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_BLAZE_RUSH);
}

/// From the user's leap pose, twenty blue bubbles stream at each target, one a
/// frame, scattered up and down, each bursting into 40 drops of spray.
///
/// Abilities: Hydro Slash.
void BattleEffect_RunHydroSlash(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_HYDRO_SLASH);
}

/// From the user's leap pose, thirty turning ice crystals stream at each
/// target, two frames apart, scattered up and down.
///
/// Abilities: Diamond Dust.
void BattleEffect_RunDiamondDust(struct BattleEffectParams *args) {
  BattleEffect_RunVolleyWithAction(args, PROJECTILE_VOLLEY_DIAMOND_DUST);
}

/// Thirty white needles stream at each target, one a frame.
///
/// Abilities: Beast Needle.
void BattleEffect_RunBeastNeedle(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_BEAST_NEEDLE);
}

/// Thirty lime needles stream at each target, one a frame, and lime smoke
/// billows where they land.
///
/// Abilities: Poison Sting.
void BattleEffect_RunPoisonSting(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_POISON_STING);
}

/// Thirty blue needles stream at each target, one a frame, and blue glints
/// flash about it where they land.
///
/// Abilities: Stun Sting.
void BattleEffect_RunStunSting(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_STUN_STING);
}

/// Sixty ice chips stream at each target, one a frame, without knocking it
/// back.
///
/// Abilities: Ice Breath, Blue Dragon.
void BattleEffect_RunIceBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_ICE_BLESSING);
}

/// Sixty ice chips stream at each target, one a frame, from the user's breath.
///
/// Abilities: Chill Breath.
void BattleEffect_RunChillBreath(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_CHILL_BREATH);
}

/// The particle buffer at the map cell buffer: flashes from 0, sparks from
/// 100, the smoke trails from 512, the falling-streak particles from 960.
#define PROJECTILE_VOLLEY_PARTICLE_COUNT PARTICLE_POOL_COUNT
#define PROJECTILE_VOLLEY_FIRST_SPARK 100  ///< the first spark in PARTICLES
#define PROJECTILE_VOLLEY_FIRST_TRAIL 512  ///< the first trail in PARTICLES
#define PROJECTILE_VOLLEY_FIRST_STREAK 960 ///< the first streak in PARTICLES
#define PROJECTILE_VOLLEY_STREAKS                                              \
  (PROJECTILE_VOLLEY_PARTICLE_COUNT - PROJECTILE_VOLLEY_FIRST_STREAK)
/// The stings' short streak, then what each draws where it lands: Poison
/// Sting's smoke, Stun Sting's glints.
#define SHORT_STREAK_WIDTH 17
#define SHORT_STREAK_HEIGHT 2
#define PROJECTILE_VOLLEY_STING_HIT_PICTURES PICTURE_BYTES(SHORT_STREAK)
/// Hydro Slash's bubble, BlueBeamSheet's first picture, decoded at the sprite
/// sheet's start; its three pictures copied at every second byte after it,
/// which nothing draws.
#define PROJECTILE_VOLLEY_HALVED_BUBBLE_PICTURES 4096
#define PROJECTILE_VOLLEY_HALVED_BUBBLE_BYTES                                  \
  ((2 * PICTURE_BYTES(BLUE_BUBBLE) + PICTURE_BYTES(BLUE_LARGE_SPLASH)) / 2)
/// A hit's sparks fly at up to 4 pixels a frame each way, as a random -128 to
/// 127 shifted up; Cutting Edge's at up to 8.
#define PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT 11
#define CUTTING_EDGE_SPARK_SPEED_SHIFT 12

/// The volley's comets, its CometSheet resources: Slash, Wind Slash and Sonic
/// Slash fire the one their variant picks.
enum ProjectileVolleyComet {
  PROJECTILE_VOLLEY_COMET_A, ///< CometSheetA
  PROJECTILE_VOLLEY_COMET_B, ///< CometSheetB
  PROJECTILE_VOLLEY_COMET_C  ///< CometSheetC, also Cutting Edge's
};

/// The second blitter, loaded again for each draw that needs its own.
#define DRAW_ALTERNATE (Ram_HeapSlots->ramCodePaired)

#define PROJECTILE_VOLLEY_STREAK_PICTURES 8192 ///< wind streaks
#define PROJECTILE_VOLLEY_DJINNI_PICTURE 26048 ///< the small Mercury Djinni

#define PROJECTILE_VOLLEY_COMET_PICTURES 4096 ///< comets: the top half of each
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
/// Sleet's, Blizzard's, Ice Blessing's and Chill Breath's ice pellets, and
/// Crazy Voice's musical notes, six pictures of ProjectileVolleyImage.
#define PROJECTILE_VOLLEY_PELLET_SIZE 8
#define PROJECTILE_VOLLEY_NOTE_WIDTH 8
#define PROJECTILE_VOLLEY_NOTE_HEIGHT 16
/// The comets' width, and the shuriken's six pictures.
#define PROJECTILE_VOLLEY_COMET_WIDTH 32
#define SHURIKEN_WIDTH 32
#define SHURIKEN_HEIGHT 24

/// Shots fly from the user to each target and burst into a flash and sparks
/// when they pass it. The hit sound plays at most once every 8 frames.
void BattleEffect_RunProjectileVolley(struct BattleEffectParams *args,
                                      s32 mode) {
  struct ScreenPoint screen;
  struct ScreenPoint world;
  struct ScreenPoint origin;
  struct ScreenPoint position;
  struct BattleEffectState *state;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleCamera *camera;
  u8 *sheet;
  s32 soundCooldown;
  s32 total;
  s32 comet;
  s32 shots;
  s32 steps;
  struct MotionObject *user;
  s32 k;
  // The smoke puffs placed, and later the sparks' speed shift.
  s32 trailsOrShift;
  s32 x;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  sheet = Ram_HeapSlots->battleSheet;
  soundCooldown = 0;
  comet = args->variant;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  // With a prelude, a Mercury Djinni appears over the user first.
  if (args->prelude == 1)
    BattleEffect_RunDjinniEntrance(
        args,
        DJINNI_ENTRANCE(ELEMENT_MERCURY, args->side, DJINNI_ANCHOR_OVER_USER),
        &screen, &world);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  // A 128x128 blitter, mirrored for the other side.
  if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
      mode == PROJECTILE_VOLLEY_BLAZE_RUSH ||
      mode == PROJECTILE_VOLLEY_HYDRO_SLASH) {
    if (args->side == EFFECT_SIDE_PARTY)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    if (args->side == EFFECT_SIDE_PARTY)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
  }
  draw[0] = Ram_HeapSlots->ramCode;

  // The shot pictures.
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  if (mode == PROJECTILE_VOLLEY_SLASH ||
      mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
      mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
      comet = PROJECTILE_VOLLEY_COMET_C;
    if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
      comet = PROJECTILE_VOLLEY_COMET_A;
    if (comet == PROJECTILE_VOLLEY_COMET_A)
      Resource_LoadAndDecompress(
          RESOURCE_COMET_SHEET_A,
          state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
    else if (comet == PROJECTILE_VOLLEY_COMET_B)
      Resource_LoadAndDecompress(
          RESOURCE_COMET_SHEET_B,
          state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
    else
      Resource_LoadAndDecompress(
          RESOURCE_COMET_SHEET_C,
          state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES, 1, 1);
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      LOAD_BG_PALETTE(RESOURCE_ICE_BLOCK_SHEET);
    }
    Resource_LoadAndDecompress(RESOURCE_WIND_STREAK_SHEET,
                               state->sheet + PROJECTILE_VOLLEY_STREAK_PICTURES,
                               1, 0);
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      LOAD_BG_PALETTE(RESOURCE_ICE_BLOCK_SHEET);
    }
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  } else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
             mode <= PROJECTILE_VOLLEY_STUN_STING) {
    // DEAD: eight bytes of level 60 that the streak's picture, decoded over
    // them next, replaces; the ROM keeps the loop.
    for (i = 0; i != 8; i++)
      state->sheet[i] = 60;
    Resource_LoadAndDecompress(RESOURCE_SHORT_STREAK_PICTURE, state->sheet, 1,
                               1);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
    if (mode == PROJECTILE_VOLLEY_BEAST_NEEDLE) {
      REG_BLDCNT = 0;
    } else if (mode == PROJECTILE_VOLLEY_POISON_STING) {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_LIME_PALETTE),
                           SHEET_PALETTE_BYTES);
      Resource_LoadAndDecompress(
          RESOURCE_SMOKE_SHEET,
          state->sheet + PROJECTILE_VOLLEY_STING_HIT_PICTURES, 1, 0);
    } else {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_WATER_SPRAY_SHEET),
                           SHEET_PALETTE_BYTES);
      Resource_LoadAndDecompress(
          RESOURCE_TORNADO_SHEET,
          state->sheet + PROJECTILE_VOLLEY_STING_HIT_PICTURES, 1, 0);
    }
  } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
    Resource_LoadAndDecompress(RESOURCE_SHURIKEN_SHEET, state->sheet, 1, 1);
    REG_BLDCNT = 0;
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_COMET_SHEET_A),
                         SHEET_PALETTE_BYTES);
    Resource_LoadAndDecompress(RESOURCE_PROJECTILE_VOLLEY_IMAGE, state->sheet,
                               0, 0);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
             mode == PROJECTILE_VOLLEY_WING_BEAT ||
             mode == PROJECTILE_VOLLEY_WING_STROKE) {
    Resource_LoadAndDecompress(RESOURCE_YELLOW_SPARK_SHEET, state->sheet, 1, 1);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE ||
             mode == PROJECTILE_VOLLEY_BLAZE_RUSH) {
    Resource_LoadAndDecompress(RESOURCE_FIREBALL_SHEET, state->sheet, 1, 1);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
    Resource_LoadAndDecompress(RESOURCE_CRYSTAL_SHEET, state->sheet, 1, 1);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == PROJECTILE_VOLLEY_HYDRO_SLASH) {
    u8 *bubble;

    Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET, bubble = state->sheet,
                               1, 1);
    // DEAD: the pictures narrowed to every second byte, which nothing draws;
    // the ROM keeps the loop.
    for (i = 0; i != PROJECTILE_VOLLEY_HALVED_BUBBLE_BYTES; i++) {
      state->sheet[PROJECTILE_VOLLEY_HALVED_BUBBLE_PICTURES + i] = *bubble;
      bubble += 2;
    }
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    Resource_LoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, state->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
                               state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE,
                               1, 0);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  }
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);

  // Where each shot starts and how fast it flies.
  user = BattleObject_GetFar(args->user)->object;
  shots = gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_SHOTS];
  steps = gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_STEPS];
  {
    s32 stagger = gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_STAGGER];
    s32 delay = gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_DELAY];
    s32 height = BattleUnit_GetEffectHeightFar(args->user);

    // Too many shots for the particles: only the first target is shot at.
    if (shots * args->targetCount >= (s32)ARRAY_COUNT(state->particles))
      args->targetCount = 1;

    for (i = 0; i != args->targetCount; i++) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[i])->object;
      s32 targetHeight = BattleUnit_GetEffectHeightFar(args->targets[i]);

      for (j = 0; j != shots; j++) {
        struct Particle *shot = &state->particles[i * shots + j];

        if (mode >= PROJECTILE_VOLLEY_BLAZE_RUSH &&
            mode <= PROJECTILE_VOLLEY_DIAMOND_DUST)
          shot->x = user->x * 2;
        else
          shot->x = user->x;
        if (mode == PROJECTILE_VOLLEY_SLEET)
          shot->y = user->y + INTtoFIXED(Random16() & 15) + toFIXED(58);
        else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
          shot->y = user->y + height / 2;
        else if (mode == PROJECTILE_VOLLEY_CHILL_BREATH)
          shot->y = user->y + toFIXED(50);
        else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
          shot->y = user->y + height / 2;
        else if (mode >= PROJECTILE_VOLLEY_BLAZE_RUSH &&
                 mode <= PROJECTILE_VOLLEY_DIAMOND_DUST)
          shot->y = user->y + height / 2 + INTtoFIXED(Random16() % 80 - 16);
        else if (mode == PROJECTILE_VOLLEY_BLIZZARD)
          shot->y = user->y + height / 2 + INTtoFIXED(16 - (Random16() & 31));
        else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
                 mode == PROJECTILE_VOLLEY_WING_BEAT)
          shot->y = user->y + height / 2 + INTtoFIXED(16 - (Random16() & 31));
        else if (mode == PROJECTILE_VOLLEY_WING_STROKE)
          shot->y = user->y + height / 2 + INTtoFIXED(32 - (Random16() & 63));
        else if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
          shot->y = user->y + height / 2;
        else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                 mode <= PROJECTILE_VOLLEY_STUN_STING)
          shot->y = user->y + INTtoFIXED(Random16() % 24);
        else
          shot->y = user->y + height;
        shot->z = user->z;
        shot->velocityX = (target->x - shot->x) / steps;
        shot->velocityZ = (target->z - shot->z) / steps;
        if (mode == PROJECTILE_VOLLEY_SLEET) {
          shot->velocityY = (s32)(target->y + INTtoFIXED(Random16() & 63) -
                                  shot->y - toFIXED(12)) /
                            steps;
        } else if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
          shot->velocityY = (s32)(target->y + INTtoFIXED(Random16() & 7) -
                                  shot->y + toFIXED(22)) /
                            steps;
        } else if (mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
          shot->velocityY =
              (s32)(target->y + INTtoFIXED(64 - (Random16() & 63)) - shot->y) /
              steps;
        } else if (mode == PROJECTILE_VOLLEY_BLIZZARD) {
          shot->velocityY =
              (s32)(target->y + INTtoFIXED(64 - (Random16() & 63)) - shot->y) /
              steps;
        } else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                   mode <= PROJECTILE_VOLLEY_STUN_STING) {
          shot->velocityY = shot->y / 32;
        } else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING) {
          // These start two steps along their flight.
          shot->velocityY = (s32)(target->y + INTtoFIXED(Random16() & 31) -
                                  shot->y + toFIXED(4)) /
                            steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == PROJECTILE_VOLLEY_CHILL_BREATH) {
          shot->velocityY = (s32)(target->y + INTtoFIXED(Random16() & 31) -
                                  shot->y + toFIXED(4)) /
                            steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
                   mode == PROJECTILE_VOLLEY_WING_BEAT ||
                   mode == PROJECTILE_VOLLEY_WING_STROKE ||
                   mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
                   mode == PROJECTILE_VOLLEY_BLAZE_RUSH) {
          shot->velocityY = 0;
        } else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
          shot->velocityY = (s32)(target->y + targetHeight / 2 +
                                  INTtoFIXED(Random16() & 15) - shot->y) /
                            steps;
        } else {
          shot->velocityY = (s32)(target->y + targetHeight -
                                  INTtoFIXED(Random16() & 15) - shot->y) /
                            steps;
        }
        shot->variant = 0;
      }
    }
    for (i = 0; i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++)
      PARTICLES[i].variant = 0;

    // The falling-streak particles start on the first target, held.
    EffectPosition_ProjectBattler(args->targets[0], &origin);
    for (i = 0; i != PROJECTILE_VOLLEY_STREAKS; i++) {
      struct Particle *p = &PARTICLES[PROJECTILE_VOLLEY_FIRST_STREAK + i];

      SET_POSITION(p, INTtoFIXED(origin.x / 2), toFIXED(80), 0);
      p->velocityX = ((Random16() & 255) - 128) << 9;
      p->velocityY = ((Random16() & 255) - 128) << 9;
      p->velocityZ = 0;
      p->variant = -1;
    }

    if (mode == PROJECTILE_VOLLEY_HEAT_WAVE || mode == PROJECTILE_VOLLEY_SLEET)
      total = (args->targetCount - 1) * delay + shots * stagger + 52;
    else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE ||
             mode == PROJECTILE_VOLLEY_BLAZE_RUSH ||
             mode == PROJECTILE_VOLLEY_HYDRO_SLASH ||
             mode == PROJECTILE_VOLLEY_DIAMOND_DUST ||
             mode == PROJECTILE_VOLLEY_POISON_STING)
      total = (args->targetCount - 1) * delay + shots * stagger + 48;
    else
      total = (args->targetCount - 1) * delay + shots * stagger + 32;

    // Each frame: the cues, the Djinni, the shots and their impacts, then
    // the flashes, sparks, smoke and streaks.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      if (soundCooldown > 0)
        soundCooldown--;
      if (mode == PROJECTILE_VOLLEY_HEAT_WAVE ||
          mode == PROJECTILE_VOLLEY_BLAZE_RUSH ||
          mode == PROJECTILE_VOLLEY_HYDRO_SLASH ||
          mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
        if (frame == 4)
          Sound_PlayFar(SE_WHOOSH);
        if (frame == 32)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
        if (frame == 32)
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
      } else if (mode == PROJECTILE_VOLLEY_SLEET) {
        if (frame == 48)
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
      } else if (mode != PROJECTILE_VOLLEY_CUTTING_EDGE) {
        if (frame == 16)
          EFFECT_SHOW_DAMAGE(SE_IMPACT);
      }

      // The prelude's small Mercury Djinni circles, then from frame 70 flies
      // off upwards.
      if (args->prelude == 1) {
        s32 djinnX = FIXEDtoINT(-Trig_Sin(frame * (TRIG_TURN / 32)) * 4) +
                     screen.x / 2 - SMALL_DJINNI_WIDTH / 2;
        s32 djinnY =
            FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 2) + screen.y - 24;

        if (frame > 69)
          djinnY = djinnY - frame * 2 + 138;
        if (args->side == EFFECT_SIDE_PARTY)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              BLITTER_ADDITIVE);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                              BLITTER_ADDITIVE);
        draw[1] = DRAW_ALTERNATE;
        if (frame <= 3)
          draw[1](canvas, state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE,
                  djinnX, djinnY, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        draw[1](canvas, state->sheet + PROJECTILE_VOLLEY_DJINNI_PICTURE, djinnX,
                djinnY, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }

      // The shots.
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      index = 0;
      for (i = 0; i != args->targetCount; i++) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        for (j = 0; j != shots; j++, index++) {
          struct Particle *shot;
          s32 targetDelay = i * delay;

          if (j * stagger + targetDelay >= frame)
            continue;
          shot = &state->particles[i * shots + j];
          if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
              mode <= PROJECTILE_VOLLEY_STUN_STING && shot->variant == 1)
            continue;

          // The screen position (the canvas is half as wide).
          EffectPosition_ProjectPoint((s32 *)shot, &position);
          position.x >>= 1;
          shot->x += shot->velocityX;
          shot->y += shot->velocityY;
          shot->z += shot->velocityZ;
          if (mode == PROJECTILE_VOLLEY_HEAT_WAVE ||
              mode == PROJECTILE_VOLLEY_BLAZE_RUSH) {
            // Two puffs of smoke behind the fireball.
            trailsOrShift = 0;
            for (k = PROJECTILE_VOLLEY_FIRST_TRAIL;
                 k != PROJECTILE_VOLLEY_FIRST_STREAK; k++) {
              struct Particle *p = &PARTICLES[k];

              if (p->variant == 0) {
                p->x = INTtoFIXED(position.x);
                p->y = INTtoFIXED(position.y);
                p->velocityX = ((Random16() & 255) - 128) << 11;
                p->velocityY = ((Random16() & 255) - 128) << 11;
                p->variant = (Random16() & 7) + 32;
                trailsOrShift++;
                if (trailsOrShift == 2)
                  break;
              }
            }
          }
          if (shot->variant == 0) {
            s32 hit;

            // The shot hits when it passes the target's x on its side.
            x = shot->x;
            if (target->x < 0) {
              hit = x < 0;
              /* FAKEMATCH(dead): the twin's cancelling updates under a test
               * that does nothing. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            } else {
              hit = x >= 0;
              /* FAKEMATCH(dead): the same for the other side. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            }
            if (ABS(x) >= ABS(target->x)) {
              s32 spawned = 0;
              struct Particle *flash = &PARTICLES[index];

              shot->variant = 1;
              if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
                EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
              } else if (mode != PROJECTILE_VOLLEY_HEAT_WAVE &&
                         mode != PROJECTILE_VOLLEY_BLAZE_RUSH &&
                         mode != PROJECTILE_VOLLEY_HYDRO_SLASH &&
                         mode != PROJECTILE_VOLLEY_DIAMOND_DUST &&
                         soundCooldown == 0) {
                soundCooldown = 8;
                Sound_PlayFar(SE_WATER);
              }
              if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
                // The pellet bounces off.
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
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, i, 8);
              if (mode != PROJECTILE_VOLLEY_SLEET &&
                  mode != PROJECTILE_VOLLEY_BLIZZARD &&
                  mode != PROJECTILE_VOLLEY_ICE_BLESSING &&
                  mode != PROJECTILE_VOLLEY_CHILL_BREATH &&
                  mode != PROJECTILE_VOLLEY_DIAMOND_DUST &&
                  mode != PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                  mode != PROJECTILE_VOLLEY_POISON_STING &&
                  mode != PROJECTILE_VOLLEY_STUN_STING) {
                if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
                  BattleMotion_ApplyKnockbackFar(args->targets[i],
                                                 KNOCKBACK_HURL);
                else
                  BattleMotion_ApplyKnockbackFar(args->targets[i],
                                                 KNOCKBACK_SKID);
              }
              if (mode == PROJECTILE_VOLLEY_POISON_STING ||
                  mode == PROJECTILE_VOLLEY_STUN_STING) {
                // The streak lets its particle loose.
                s32 n;
                /* FAKEMATCH(temporary): held stands for the 0 stored. */
                s32 held;

                // A once-only loop the ROM keeps, with its counter, its test
                // against 1 and its branch back.
                for (n = 0, held = 0; n != 1; n++) {
                  /* FAKEMATCH(dead): updates of held that cancel out. */
                  held ^= n;
                  held ^= n;
                  PARTICLES[PROJECTILE_VOLLEY_FIRST_STREAK + j + n].variant =
                      held;
                }
              } else if (mode != PROJECTILE_VOLLEY_BEAST_NEEDLE) {
                // Sparks fly from the hit.
                trailsOrShift = mode == PROJECTILE_VOLLEY_CUTTING_EDGE
                                    ? CUTTING_EDGE_SPARK_SPEED_SHIFT
                                    : PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT;
                for (k = PROJECTILE_VOLLEY_FIRST_SPARK;
                     k != PROJECTILE_VOLLEY_FIRST_TRAIL; k++) {
                  struct Particle *p = &PARTICLES[k];

                  if (p->variant == 0) {
                    p->x = INTtoFIXED(position.x);
                    p->y = INTtoFIXED(position.y);
                    p->velocityX = ((Random16() & 255) - 128) << trailsOrShift;
                    p->velocityY = ((Random16() & 255) - 128) << trailsOrShift;
                    p->variant = (Random16() & 7) + 16;
                    spawned++;
                    if (spawned ==
                        gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_SPARKS])
                      break;
                  }
                }
              }
            }
          }
        draw_shot:
          if (mode == PROJECTILE_VOLLEY_SLASH ||
              mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
              mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
            // Only the comet's upper half is drawn; the lower half is the
            // same picture flipped.
            if (args->side == EFFECT_SIDE_PARTY)
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                  BLITTER_MAX);
            else
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
            DRAW_ALTERNATE(canvas,
                           state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                           position.x - PROJECTILE_VOLLEY_COMET_WIDTH / 2,
                           position.y - gProjectileVolley_CometHeights[comet],
                           PROJECTILE_VOLLEY_COMET_WIDTH,
                           gProjectileVolley_CometHeights[comet]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            if (args->side == EFFECT_SIDE_PARTY)
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  BLITTER_MAX);
            else
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                                  BLITTER_MAX);
            DRAW_ALTERNATE(canvas,
                           state->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                           position.x - PROJECTILE_VOLLEY_COMET_WIDTH / 2,
                           position.y, PROJECTILE_VOLLEY_COMET_WIDTH,
                           gProjectileVolley_CometHeights[comet]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
          } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
            if (ABS(shot->x) <= ABS(target->x))
              draw[0](canvas,
                      state->sheet + frame % 6 * PICTURE_BYTES(SHURIKEN),
                      position.x - SHURIKEN_WIDTH / 2,
                      position.y - SHURIKEN_HEIGHT / 2, SHURIKEN_WIDTH,
                      SHURIKEN_HEIGHT);
          } else if (mode == PROJECTILE_VOLLEY_SLEET ||
                     mode == PROJECTILE_VOLLEY_BLIZZARD ||
                     mode == PROJECTILE_VOLLEY_ICE_BLESSING ||
                     mode == PROJECTILE_VOLLEY_CHILL_BREATH) {
            draw[0](
                canvas, state->sheet + gProjectileVolley_PelletPictures[j & 3],
                position.x - PROJECTILE_VOLLEY_PELLET_SIZE / 2,
                position.y - PROJECTILE_VOLLEY_PELLET_SIZE / 2,
                PROJECTILE_VOLLEY_PELLET_SIZE, PROJECTILE_VOLLEY_PELLET_SIZE);
          } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
            draw[0](
                canvas,
                state->sheet + j % 6 * PICTURE_BYTES(PROJECTILE_VOLLEY_NOTE),
                position.x - PROJECTILE_VOLLEY_NOTE_WIDTH / 2,
                position.y - PROJECTILE_VOLLEY_NOTE_HEIGHT / 2,
                PROJECTILE_VOLLEY_NOTE_WIDTH, PROJECTILE_VOLLEY_NOTE_HEIGHT);
          } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER) {
            EFFECT_DRAW_PICTURE(
                draw[0], canvas, state->sheet,
                PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE,
                position.x - PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_WIDTH / 2,
                position.y -
                    (PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_HEIGHT + 1) / 2);
          } else if (mode == PROJECTILE_VOLLEY_WING_BEAT) {
            EFFECT_DRAW_PICTURE(
                draw[0], canvas, state->sheet,
                PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE,
                position.x - PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_WIDTH / 2,
                position.y - PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_HEIGHT / 2);
          } else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                     mode <= PROJECTILE_VOLLEY_STUN_STING) {
            draw[0](canvas, state->sheet, position.x - SHORT_STREAK_WIDTH / 2,
                    position.y - SHORT_STREAK_HEIGHT / 2, SHORT_STREAK_WIDTH,
                    SHORT_STREAK_HEIGHT);
          } else if (mode == PROJECTILE_VOLLEY_HYDRO_SLASH) {
            draw[0](canvas, state->sheet, position.x - BLUE_BUBBLE_WIDTH / 2,
                    position.y - BLUE_BUBBLE_HEIGHT / 2, BLUE_BUBBLE_WIDTH,
                    BLUE_BUBBLE_HEIGHT);
          } else if (mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
            // Five crystals, each turning through three cels.
            s32 cel = (i * shots + j) % 5 * 3 + frame / 4 % 3;
            u32 width;
            u32 height;
            u8 *picture = state->sheet + gPrism_ShardPictures[cel] +
                          PICTURE_BYTES(CRYSTAL);
            s32 left;
            s32 top;

            width = gPrism_ShardWidths[cel];
            left = position.x - (width >> 1);
            height = gPrism_ShardHeights[cel];
            top = position.y - (height >> 1);
            draw[0](canvas, picture, left, top, width, height);
          } else if (mode == PROJECTILE_VOLLEY_WING_STROKE) {
            EFFECT_DRAW_PICTURE(
                draw[0], canvas, state->sheet,
                PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE,
                position.x -
                    (PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_WIDTH + 1) / 2,
                position.y -
                    (PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_HEIGHT + 1) / 2);
          } else {
            draw[0](canvas, state->sheet, position.x - FLAMEBALL_WIDTH / 2,
                    position.y - FLAMEBALL_HEIGHT / 2, FLAMEBALL_WIDTH,
                    FLAMEBALL_HEIGHT);
          }
        }
      }

      // The comets' flashes, 12 frames: half the picture, then the same
      // half flipped (odd flashes use the picture turned on its side).
      if (mode == PROJECTILE_VOLLEY_SLASH ||
          mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
          mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
        for (i = 0; i != args->targetCount * shots; i++) {
          struct Particle *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;
            s32 flags;

            if (!(i & 1)) {
              flags = EFFECT_CLIP_BOUNDS;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(
                  canvas, state->sheet + gProjectileVolley_FlashPictures[step],
                  flash->x - gProjectileVolley_FlashWidths[step],
                  flash->y - gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(
                  canvas, state->sheet + gProjectileVolley_FlashPictures[step],
                  flash->x,
                  flash->y + gProjectileVolley_FlashSteps[step] -
                      gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            } else {
              flags = EFFECT_CLIP_BOUNDS;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(canvas,
                             state->sheet +
                                 gProjectileVolley_FlashPictures[step] +
                                 PROJECTILE_VOLLEY_FLASH_TURNED,
                             flash->x - gProjectileVolley_FlashRises[step],
                             flash->y - gProjectileVolley_FlashWidths[step],
                             gProjectileVolley_FlashHeights[step],
                             gProjectileVolley_FlashWidths[step]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(canvas,
                             state->sheet +
                                 gProjectileVolley_FlashPictures[step] +
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

      // Sparks fall and bounce at y = 112; Hydro Slash's spray falls faster.
      for (i = PROJECTILE_VOLLEY_FIRST_SPARK;
           i != PROJECTILE_VOLLEY_FIRST_TRAIL; i++) {
        struct Particle *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 3) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, FIXED_INTEGER_PART(p->x),
                           FIXED_INTEGER_PART(p->y), size);
          if (mode == PROJECTILE_VOLLEY_HYDRO_SLASH)
            Particle_AdvanceWithGravity2D(p, 60, toFIXED(1) / 4);
          else
            Particle_AdvanceWithGravity2D(p, 60, toFIXED(1) / 16);
          if (p->y > toFIXED(112))
            p->velocityY = -p->velocityY / 2;
          p->variant--;
        }
      }

      // Smoke rises.
      for (i = PROJECTILE_VOLLEY_FIRST_TRAIL;
           i != PROJECTILE_VOLLEY_FIRST_STREAK; i++) {
        struct Particle *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 4) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, FIXED_INTEGER_PART(p->x),
                           FIXED_INTEGER_PART(p->y), size);
          Particle_AdvanceWithGravity2D(p, 60, -toFIXED(1) / 4);
          p->variant--;
        }
      }

      if (mode == PROJECTILE_VOLLEY_POISON_STING) {
        // Smoke billows up where each streak landed: six cels, four frames
        // each.
        for (i = 0; i != PROJECTILE_VOLLEY_STREAKS; i++) {
          struct Particle *p = &PARTICLES[PROJECTILE_VOLLEY_FIRST_STREAK + i];

          if ((u32)p->variant < SMOKE_PUFF_FRAMES) {
            draw[0](canvas,
                    state->sheet + PROJECTILE_VOLLEY_STING_HIT_PICTURES +
                        p->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
                    FIXED_INTEGER_PART(p->x) - SMOKE_PUFF_WIDTH / 2,
                    FIXED_INTEGER_PART(p->y) - SMOKE_PUFF_HEIGHT / 2,
                    SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
            Particle_AdvanceWithGravity2D(p, 62, -toFIXED(1) / 64);
            p->variant++;
          }
        }
      } else if (mode == PROJECTILE_VOLLEY_STUN_STING) {
        // Glints scatter about each landed streak, each flipped at random.
        for (i = 0; i != PROJECTILE_VOLLEY_STREAKS; i++) {
          struct Particle *p = &PARTICLES[PROJECTILE_VOLLEY_FIRST_STREAK + i];

          if ((u32)p->variant <= 7) {
            s32 glint = frame & 3;

            s32 angle = Random16() & TRIG_ANGLE_MASK;
            s32 radius = (Random16() & 31) + 4;
            s32 glintX = FIXED_INTEGER_PART(p->x) +
                         (FIXEDtoINT(Trig_Sin(angle) * radius) >> 1) -
                         (gBattleEffect_GlintPictureWidths[glint] >> 1);
            s32 glintY = FIXED_INTEGER_PART(p->y) -
                         (FIXEDtoINT(Trig_Cos(angle) * radius) >> 1) -
                         (gBattleEffect_GlintPictureHeights[glint] >> 1);
            u32 flip = Random16();

            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                gProjectileVolley_GlintFlips[flip & 3] |
                                    EFFECT_CLIP_BOUNDS,
                                BLITTER_ADDITIVE);
            DRAW_ALTERNATE(canvas,
                           state->sheet + PROJECTILE_VOLLEY_STING_HIT_PICTURES +
                               gBattleEffect_GlintPictures[glint],
                           glintX, glintY,
                           gBattleEffect_GlintPictureWidths[glint],
                           gBattleEffect_GlintPictureHeights[glint]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            p->variant++;
          }
        }
      }
      BattleCamera_Shake(4, 4);
      EFFECT_END_FRAME(state);
    }
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Unused: nothing calls it. Sends a unit back to its place (its anchor) with a
/// hop.
UNUSED void BattleEffect_HopBackToSpot(s32 slot) {
  struct BattleObject *context = BattleObject_GetFar(slot);
  struct Actor *actor = context->object;

  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(8);
  actor->velocityY = toFIXED(4);
  actor->gravity = BATTLE_UNIT_GRAVITY;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  actor->snapToTarget = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, context->anchorX, 0, context->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_STANDING);
}

/// Moves the object rate percent of the way to the target's object in the
/// given frames.
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames,
                                    s32 rate) {
  struct BattleObject *slot = BattleObject_GetFar(id);
  struct BattleObject *targetSlot = BattleObject_GetFar(targetId);
  struct Actor *obj = (struct Actor *)slot->object;
  struct Actor *target = (struct Actor *)targetSlot->object;
  FIXED diffX = target->x.fixed - obj->x.fixed;
  FIXED startX = obj->x.fixed;
  FIXED dx = rate * diffX / 100;
  FIXED diffZ = target->z.fixed - obj->z.fixed;
  FIXED startZ = obj->z.fixed;
  FIXED dz = rate * diffZ / 100;
  FIXED x = startX + dx;
  FIXED z = startZ + dz;
  s32 sx = dx >> 8; // the distance in 1/256 pixels
  s32 sz = dz >> 8;
  s32 dist;

  dist = Iwram_Sqrt(sx * sx + sz * sz);
  dist = (dist << 8) / frames;
  obj->acceleration = dist;
  obj->speedLimit = dist;
  obj->snapToTarget = 1;
  obj->gravity = BATTLE_UNIT_GRAVITY;
  obj->velocityY = 0;
  obj->bounce = 0;
  obj->steeringFlags = 1;
  Actor_StartIdleFar(obj);
  Actor_SetMoveTargetFar(obj, x, 0, z);
  Actor_SelectAnimationFar(obj, POSE_LEAP);
}

#undef DJINNI_ENTRANCE_SPARKS
#undef DJINNI_ENTRANCE_TRAVEL_FRAMES
#undef DJINNI_SPARK_DEPTH_NEAR
#undef DJINNI_SPARK_DEPTH_STEP
#undef DJINNI_SPARK_DEPTH_FAR
#undef PROJECTILE_VOLLEY_PARTICLE_COUNT
#undef PROJECTILE_VOLLEY_FIRST_SPARK
#undef PROJECTILE_VOLLEY_FIRST_TRAIL
#undef PROJECTILE_VOLLEY_FIRST_STREAK
#undef PROJECTILE_VOLLEY_STREAKS
#undef SHORT_STREAK_WIDTH
#undef SHORT_STREAK_HEIGHT
#undef PROJECTILE_VOLLEY_STING_HIT_PICTURES
#undef PROJECTILE_VOLLEY_HALVED_BUBBLE_PICTURES
#undef PROJECTILE_VOLLEY_HALVED_BUBBLE_BYTES
#undef PROJECTILE_VOLLEY_SPARK_SPEED_SHIFT
#undef CUTTING_EDGE_SPARK_SPEED_SHIFT
#undef DRAW_ALTERNATE
#undef PROJECTILE_VOLLEY_STREAK_PICTURES
#undef PROJECTILE_VOLLEY_DJINNI_PICTURE
#undef PROJECTILE_VOLLEY_COMET_PICTURES
#undef PROJECTILE_VOLLEY_FLASH_TURNED
#undef PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_WIDTH
#undef PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE_HEIGHT
#undef PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_WIDTH
#undef PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE_HEIGHT
#undef PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_WIDTH
#undef PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE_HEIGHT
#undef PROJECTILE_VOLLEY_SMALL_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_MEDIUM_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_LARGE_SPARK_PICTURE
#undef PROJECTILE_VOLLEY_PELLET_SIZE
#undef PROJECTILE_VOLLEY_NOTE_WIDTH
#undef PROJECTILE_VOLLEY_NOTE_HEIGHT
#undef PROJECTILE_VOLLEY_COMET_WIDTH
#undef SHURIKEN_WIDTH
#undef SHURIKEN_HEIGHT
