/// @file
/// The opening every Djinn effect shares, Cutting Edge, Heat Wave and Blizzard,
/// and the projectile volleys.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Sparks swirl around a point that travels from the user to the anchor and
/// gather into a Djinni, which sways for 16 frames and bursts into motes.
/// packed is DJINNI_ENTRANCE's; the anchor's screen point goes to screen and
/// its world point to world.
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *args, u32 packed,
                                    s32 *screen, s32 *world) {
  s32 center[3];
  s32 goal[3];
  s32 step[3];
  s32 point[3];
  struct ScreenPoint position;
  struct BattleEffectWork *work;
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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  element = DJINNI_ENTRANCE_ELEMENT(packed);
  side = DJINNI_ENTRANCE_SIDE(packed);
  anchor = DJINNI_ENTRANCE_ANCHOR(packed);
  if (anchor == DJINNI_ANCHOR_CENTRE_BRIGHTENING) {
    frameCount = 40;
  } else if (anchor == DJINNI_ANCHOR_OVER_TARGET_SHORT) {
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    frameCount = 54;
  } else {
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
    if (element > ELEMENT_JUPITER) {
      element -= DJINNI_ENTRANCE_LONG;
      frameCount = 84;
    } else {
      frameCount = 55;
    }
  }

  // The Djinn's palette, then its small picture.
  switch (element) {
  case ELEMENT_VENUS:
    resource = (s32)&ResourceId_BlastSheet;
    break;
  case ELEMENT_MERCURY:
    resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case ELEMENT_MARS:
    resource = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  palette = Resource_GetTableEntry(resource);
  Iwram_CopyWords((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  switch (element) {
  case ELEMENT_VENUS:
    resource = (s32)&ResourceId_VenusDjinnSmallSheet;
    break;
  case ELEMENT_MERCURY:
    resource = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case ELEMENT_MARS:
    resource = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    resource = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  palette = Resource_GetTableEntry(resource);
  /* FAKEMATCH(barrier): an empty asm between the picture's table entry and its
   * + 128 copies the entry before the sheet is read. */
  asm("");
  palette += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(palette, work->sheet);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  // Additive and max blitters, clipped (flipped for side 1).
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
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  actor = BattleObject_GetFar(args->user)->object;
  target = BattleObject_GetFar(args->targets[0])->object;

  // 64 sparks start on the ground around the caster, flying outward.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    struct Particle *spark = &work->particles[i];
    s32 angle = Random16() & TRIG_ANGLE_MASK;
    s32 speed = (Random16() & 255) + 128;

    SET_POSITION(spark, 0, px((Random16() & 31) + 20), 0);
    spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(angle) * speed) >> 5;
    spark->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The spin's centre moves from the caster to the anchor over frames 6-45.
  center[0] = actor->x;
  center[1] = 0;
  center[2] = actor->z;
  switch (anchor) {
  case DJINNI_ANCHOR_OVER_TARGET:
    goal[0] = target->x;
    goal[1] = px(60);
    goal[2] = target->z;
    break;
  case DJINNI_ANCHOR_OVER_TARGET_SHORT:
    goal[0] = target->x;
    goal[1] = px(60);
    goal[2] = target->z;
    break;
  case DJINNI_ANCHOR_TARGET_X:
    goal[0] = target->x;
    goal[1] = px(60);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_HIGH_TARGET_X:
    goal[0] = target->x;
    goal[1] = px(120);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_OVER_USER:
    goal[0] = actor->x;
    goal[1] = px(60);
    goal[2] = actor->z;
    break;
  case DJINNI_ANCHOR_USER_X:
    goal[0] = actor->x;
    goal[1] = px(60);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_CENTRE:
  case DJINNI_ANCHOR_CENTRE_BRIGHTENING:
    goal[0] = 0;
    goal[1] = px(60);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_LOW_CENTRE:
    goal[0] = 0;
    goal[1] = px(20);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_HIGH_USER_X:
    goal[0] = actor->x;
    goal[1] = px(80);
    goal[2] = 0;
    break;
  case DJINNI_ANCHOR_LOW_USER_X:
    goal[0] = actor->x;
    goal[1] = px(30);
    goal[2] = 0;
    break;
  }
  step[0] = (goal[0] - center[0]) / 40;
  step[1] = (goal[1] - center[1]) / 40;
  step[2] = (goal[2] - center[2]) / 40;

  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame > 75)
      REG_BLDALPHA = BLEND_ALPHA(168 - frame * 2, BLEND_WEIGHT_MAX);
    if (anchor == DJINNI_ANCHOR_CENTRE_BRIGHTENING && frame > 23)
      Palette_BrightenBattleBg(2, 2, 2);
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);
    if (frame >= 6 && frame <= 45) {
      center[0] += step[0];
      center[1] += step[1];
      center[2] += step[2];
    }
    Transform_ApplyPosition(center);
    if (frame == 0)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 24)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);

    // The 32 sparks spin about the centre, each on its own axis, and from
    // their 24th frame fall into it; a spark that has arrived stops.
    if (frame <= 63) {
      for (i = 0; i != 32; i++) {
        struct Particle *spark = &work->particles[i];

        if (frame >= i / 8 && spark->variant == 0) {
          s32 size;

          Transform_Push();
          switch (i & 3) {
          case 0:
            Transform_ApplyYaw(frame * (i * 32 + 256));
            break;
          case 1:
            Transform_ApplyPitch(-frame * (i * 32 + 256));
            break;
          case 2:
            Transform_ApplyRoll(-frame * (i * 32 + 256));
            break;
          case 3:
            Transform_ApplyPitch(-frame * (i * 32 + 256));
            Transform_ApplyRoll(-frame * (i * 32 + 256));
            break;
          }
          EffectPosition_ProjectPoint(&spark->x, &position);
          position.x >>= 1;
          Transform_Pop();
          if (position.depth < 250)
            position.depth = 250;
          if (position.depth > 634)
            position.depth = 634;
          size = 8 - (position.depth - 250) / 64;
          draw[1](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  position.x - size / 2, position.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, 0);
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

    // The small Djinn sways at the anchor for frames 54 to 69.
    if (frame >= 54 && frame <= 69) {
      point[0] = Trig_Sin(frame << 10) << 2;
      point[1] = 0;
      point[2] = 0;
      world[0] = goal[0] + (Trig_Sin(frame << 10) << 2);
      world[1] = goal[1];
      world[2] = goal[2];
      EffectPosition_ProjectPoint(point, &position);
      screen[0] = position.x;
      screen[1] = position.y;
      position.x >>= 1;
      draw[0](canvas, work->sheet, position.x - 10, position.y - 20,
              SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
    }

    // At frame 64 it bursts into 64 motes that fall away.
    if (frame == 64) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        struct Particle *mote = &work->particles[i];
        s32 angle = Random16() & TRIG_ANGLE_MASK;
        s32 speed = (Random16() & 255) + 128;

        mote->x = screen[0] << 15;
        mote->y = px(screen[1]);
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    if (frame > 63) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  (s16)(Q_16_16_TO_INT(mote->x)) - size / 2,
                  (s16)(Q_16_16_TO_INT(mote->y)) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
}

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

/// Abilities: Slash, Wind Slash, Sonic Slash.
void BattleEffect_RunSlash(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_SLASH);
}

/// Abilities: Shuriken.
void BattleEffect_RunShuriken(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_SHURIKEN);
}

/// Abilities: Crazy Voice.
void BattleEffect_RunCrazyVoice(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_CRAZY_VOICE);
}

/// Abilities: Wing Flutter.
void BattleEffect_RunWingFlutter(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_WING_FLUTTER);
}

/// Abilities: Wing Stroke.
void BattleEffect_RunWingStroke(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_WING_STROKE);
}

/// Abilities: Wing Beat.
void BattleEffect_RunWingBeat(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_WING_BEAT);
}

/// From the user's action pose, one large comet in ice colours flies at each
/// target and bursts into 80 sparks.
///
/// Abilities: Cutting Edge.
void BattleEffect_RunCuttingEdge(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_CUTTING_EDGE);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, seven fireballs trailing embers fly at each
/// target, two frames apart, each bursting into 40 sparks.
///
/// Abilities: Heat Wave.
void BattleEffect_RunHeatWave(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_HEAT_WAVE);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// Abilities: Sleet.
void BattleEffect_RunSleet(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_SLEET);
}

/// From the user's action pose, sixty ice chips stream at each target, one a
/// frame, scattered up and down, without knocking it back.
///
/// Abilities: Blizzard.
void BattleEffect_RunBlizzard(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_BLIZZARD);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, the projectile volley's shots fly at each
/// target.
///
/// Abilities: Steel, Blaze Rush.
void BattleEffect_RunBlazeRush(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_BLAZE_RUSH);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, the projectile volley's shots fly at each
/// target.
///
/// Abilities: Hydro Slash.
void BattleEffect_RunHydroSlash(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_HYDRO_SLASH);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, the projectile volley's shots fly at each
/// target.
///
/// Abilities: Diamond Dust.
void BattleEffect_RunDiamondDust(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_DIAMOND_DUST);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// Abilities: Beast Needle.
void BattleEffect_RunBeastNeedle(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_BEAST_NEEDLE);
}

/// Abilities: Poison Sting.
void BattleEffect_RunPoisonSting(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_POISON_STING);
}

/// Abilities: Stun Sting.
void BattleEffect_RunStunSting(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_STUN_STING);
}

/// Kind 402: Ice Breath and Blue Dragon.
void BattleEffect_RunIceBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_ICE_BLESSING);
}

/// Abilities: Chill Breath.
void BattleEffect_RunChillBreath(struct BattleEffectParams *args) {
  BattleEffect_RunProjectileVolley(args, PROJECTILE_VOLLEY_CHILL_BREATH);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// The particle buffer at the map cell buffer: flashes from 0, sparks from
/// 100, smoke from 512, the falling-streak particles from 960.
#define PARTICLES ((struct Particle *)Ram_MapCellBuffer)
#define PTCL_SPARKS 100
#define PTCL_SMOKE 512
#define PTCL_STREAKS 960
#define PTCL_MAX 1024

// The five bytes of a mode in gProjectileVolley_Modes.
#define VOLLEY_SHOTS 0   ///< shots at each target
#define VOLLEY_STEPS 1   ///< frames a shot takes to reach its target
#define VOLLEY_STAGGER 2 ///< frames between shots
#define VOLLEY_DELAY 3   ///< frames between targets
#define VOLLEY_SPARKS 4  ///< sparks on impact

/// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a
/// heap slot.
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// The second blitter, loaded again for each draw that needs its own.
#define DRAW_ALTERNATE ((DrawRectangle)Ram_HeapSlots->ramCodePaired)

/// Frees the second blitter after a draw.
#define RELEASE_DRAW_BLITTER()                                                 \
  do {                                                                         \
    HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);                               \
  } while (0)

#define CUE_FIRE 136
#define CUE_IMPACT 132

/// Where WindStreakSheet is decoded in the effect's pictures.
#define PROJECTILE_VOLLEY_WIND_STREAK_PICTURES 0x2000
/// Where MercuryDjinnSmallSheet is decoded in the effect's pictures.
#define PROJECTILE_VOLLEY_MERCURY_DJINN_SMALL_PICTURES 0x65c0

#define PROJECTILE_VOLLEY_COMET_PICTURES 4096 ///< comets: the top half of each
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
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleCamera *camera;
  u8 *sheet;
  s32 seWait;
  s32 total;
  s32 kind;
  s32 shots;
  s32 steps;
  struct MotionObject *user;
  s32 k;
  // The smoke puffs placed, and later the sparks' speed shift.
  s32 shift;
  s32 x;

  // Set up: the work, canvas, camera and sprite sheet, the canvas layer and
  // the Djinn's place when it circles first.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->battleSheet;
  seWait = 0;
  kind = args->variant;
  BattleEffect_StartCanvasLayer(1);
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
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  // The shot pictures.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == PROJECTILE_VOLLEY_SLASH ||
      mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
      mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
      kind = 2;
    if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
      kind = 0;
    if (kind == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                                 1, 1);
    else if (kind == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                                 1, 1);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                                 1, 1);
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      void *palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

      copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
    }
    Resource_LoadAndDecompress(
        (s32)&ResourceId_WindStreakSheet,
        work->sheet + PROJECTILE_VOLLEY_WIND_STREAK_PICTURES, 1, 0);
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      void *palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

      copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  } else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
             mode <= PROJECTILE_VOLLEY_STUN_STING) {
    for (i = 0; i != 8; i++)
      work->sheet[i] = 60;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0FF, work->sheet, 1, 1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
    if (mode == PROJECTILE_VOLLEY_BEAST_NEEDLE) {
      REG_BLDCNT = 0;
    } else if (mode == PROJECTILE_VOLLEY_POISON_STING) {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                           SHEET_PALETTE_BYTES);
      Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                                 work->sheet + 0x22, 1, 0);
    } else {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet), 128);
      Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                 work->sheet + 0x22, 1, 0);
    }
  } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
    Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work->sheet, 1,
                               1);
    REG_BLDCNT = 0;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry((s32)&ResourceId_CometSheetA),
                         SHEET_PALETTE_BYTES);
    Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage,
                               work->sheet, 0, 0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
             mode == PROJECTILE_VOLLEY_WING_BEAT ||
             mode == PROJECTILE_VOLLEY_WING_STROKE) {
    Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work->sheet,
                               1, 1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE ||
             mode == PROJECTILE_VOLLEY_BLAZE_RUSH) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work->sheet, 1,
                               1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1,
                               1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == PROJECTILE_VOLLEY_HYDRO_SLASH) {
    // The beam is unpacked once and then halved, every second byte, into
    // place at 0x1000.
    u8 *beam;

    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                               beam = work->sheet, 1, 1);
    for (i = 0; i != 0x5b2; i++) {
      work->sheet[0x1000 + i] = *beam;
      beam += 2;
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress(
        (s32)&ResourceId_MercuryDjinnSmallSheet,
        work->sheet + PROJECTILE_VOLLEY_MERCURY_DJINN_SMALL_PICTURES, 1, 0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Where each shot starts and how fast it flies.
  user = BattleObject_GetFar(args->user)->object;
  shots = gProjectileVolley_Modes[mode][VOLLEY_SHOTS];
  steps = gProjectileVolley_Modes[mode][VOLLEY_STEPS];
  {
    s32 stagger = gProjectileVolley_Modes[mode][VOLLEY_STAGGER];
    s32 delay = gProjectileVolley_Modes[mode][VOLLEY_DELAY];
    s32 height = BattleUnit_GetEffectHeightFar(args->user);

    if (shots * args->targetCount > 63) // the shots must fit particles[64]
      args->targetCount = 1;

    for (i = 0; i != args->targetCount; i++) {
      struct MotionObject *target =
          BattleObject_GetFar(args->targets[i])->object;
      s32 targetHeight = BattleUnit_GetEffectHeightFar(args->targets[i]);

      for (j = 0; j != shots; j++) {
        struct Particle *shot = &work->particles[i * shots + j];

        if (mode >= PROJECTILE_VOLLEY_BLAZE_RUSH &&
            mode <= PROJECTILE_VOLLEY_DIAMOND_DUST)
          shot->x = user->x * 2;
        else
          shot->x = user->x;
        if (mode == PROJECTILE_VOLLEY_SLEET)
          shot->y = user->y + px(Random16() & 15) + px(58);
        else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
          shot->y = user->y + height / 2;
        else if (mode == PROJECTILE_VOLLEY_CHILL_BREATH)
          shot->y = user->y + px(50);
        else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
          shot->y = user->y + height / 2;
        else if (mode >= PROJECTILE_VOLLEY_BLAZE_RUSH &&
                 mode <= PROJECTILE_VOLLEY_DIAMOND_DUST)
          shot->y = user->y + height / 2 + px(Random16() % 80 - 16);
        else if (mode == PROJECTILE_VOLLEY_BLIZZARD)
          shot->y = user->y + height / 2 + px(16 - (Random16() & 31));
        else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
                 mode == PROJECTILE_VOLLEY_WING_BEAT)
          shot->y = user->y + height / 2 + px(16 - (Random16() & 31));
        else if (mode == PROJECTILE_VOLLEY_WING_STROKE)
          shot->y = user->y + height / 2 + px(32 - (Random16() & 63));
        else if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
          shot->y = user->y + height / 2;
        else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                 mode <= PROJECTILE_VOLLEY_STUN_STING)
          shot->y = user->y + px(Random16() % 24);
        else
          shot->y = user->y + height;
        shot->z = user->z;
        shot->velocityX = (target->x - shot->x) / steps;
        shot->velocityZ = (target->z - shot->z) / steps;
        if (mode == PROJECTILE_VOLLEY_SLEET) {
          shot->velocityY =
              (s32)(target->y + px(Random16() & 63) - shot->y - px(12)) / steps;
        } else if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
          shot->velocityY =
              (s32)(target->y + px(Random16() & 7) - shot->y + px(22)) / steps;
        } else if (mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
          shot->velocityY =
              (s32)(target->y + px(64 - (Random16() & 63)) - shot->y) / steps;
        } else if (mode == PROJECTILE_VOLLEY_BLIZZARD) {
          shot->velocityY =
              (s32)(target->y + px(64 - (Random16() & 63)) - shot->y) / steps;
        } else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                   mode <= PROJECTILE_VOLLEY_STUN_STING) {
          shot->velocityY = shot->y / 32;
        } else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING) {
          // These start two steps along their flight.
          shot->velocityY =
              (s32)(target->y + px(Random16() & 31) - shot->y + px(4)) / steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == PROJECTILE_VOLLEY_CHILL_BREATH) {
          shot->velocityY =
              (s32)(target->y + px(Random16() & 31) - shot->y + px(4)) / steps;
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
                                  px(Random16() & 15) - shot->y) /
                            steps;
        } else {
          shot->velocityY =
              (s32)(target->y + targetHeight - px(Random16() & 15) - shot->y) /
              steps;
        }
        shot->variant = 0;
      }
    }
    for (i = 0; i != PTCL_MAX; i++)
      PARTICLES[i].variant = 0;

    // The falling-streak particles start on the first target, held.
    EffectPosition_ProjectBattler(args->targets[0], &origin);
    for (i = 0; i != 64; i++) {
      struct Particle *p = &PARTICLES[PTCL_STREAKS + i];

      SET_POSITION(p, px(origin.x / 2), px(80), 0);
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

    // Each frame: the cues, the Djinn, the shots and their impacts, then
    // the flashes, sparks, smoke and streaks.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      if (seWait > 0)
        seWait--;
      if (mode == PROJECTILE_VOLLEY_HEAT_WAVE ||
          mode == PROJECTILE_VOLLEY_BLAZE_RUSH ||
          mode == PROJECTILE_VOLLEY_HYDRO_SLASH ||
          mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
        if (frame == 4)
          Sound_PlayFar(CUE_FIRE);
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

      // The prelude: a small Mercury Djinn circles, then from frame 70
      // flies off upwards.
      if (args->prelude == 1) {
        s32 djinnX =
            (Q_16_16_TO_INT(-Trig_Sin(frame << 11) << 2)) + screen.x / 2 - 10;
        s32 djinnY =
            (Q_16_16_TO_INT(Trig_Cos(frame << 11) << 1)) + screen.y - 24;

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
          draw[1](canvas,
                  work->sheet + PROJECTILE_VOLLEY_MERCURY_DJINN_SMALL_PICTURES,
                  djinnX, djinnY, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        draw[1](canvas,
                work->sheet + PROJECTILE_VOLLEY_MERCURY_DJINN_SMALL_PICTURES,
                djinnX, djinnY, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        RELEASE_DRAW_BLITTER();
      }

      // The shots.
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->focus);
      index = 0;
      for (i = 0; i != args->targetCount; i++) {
        struct MotionObject *target =
            BattleObject_GetFar(args->targets[i])->object;

        for (j = 0; j != shots; j++, index++) {
          struct Particle *shot;
          struct ScreenPoint *pos;
          s32 targetDelay = i * delay;

          if (j * stagger + targetDelay >= frame)
            continue;
          shot = &work->particles[i * shots + j];
          if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
              mode <= PROJECTILE_VOLLEY_STUN_STING && shot->variant == 1)
            continue;

          // The screen position (the canvas is half as wide).
          pos = &position;
          EffectPosition_ProjectPoint((s32 *)shot, pos);
          pos->x >>= 1;
          shot->x += shot->velocityX;
          shot->y += shot->velocityY;
          shot->z += shot->velocityZ;
          if (mode == PROJECTILE_VOLLEY_HEAT_WAVE ||
              mode == PROJECTILE_VOLLEY_BLAZE_RUSH) {
            // Two puffs of smoke behind the fireball.
            shift = 0;
            for (k = PTCL_SMOKE; k != PTCL_STREAKS; k++) {
              struct Particle *p = &PARTICLES[k];

              if (p->variant == 0) {
                p->x = px(position.x);
                p->y = px(position.y);
                p->velocityX = ((Random16() & 255) - 128) << 11;
                p->velocityY = ((Random16() & 255) - 128) << 11;
                p->variant = (Random16() & 7) + 32;
                shift++;
                if (shift == 2)
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
               * that emits nothing. */
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
                         seWait == 0) {
                seWait = 8;
                Sound_PlayFar(CUE_IMPACT);
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
              SET_POSITION(flash, pos->x, pos->y, 0);
              if (mode != PROJECTILE_VOLLEY_SLEET)
                work->shakeFrames = 2;
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
                s32 held;

                for (n = 0, held = 0; n != 1; n++) {
                  /* FAKEMATCH(barrier): the asm operand. */
                  __asm__("" : "+r"(held));
                  PARTICLES[PTCL_STREAKS + j + n].variant = held;
                }
              } else if (mode != PROJECTILE_VOLLEY_BEAST_NEEDLE) {
                // Sparks.
                shift = mode == PROJECTILE_VOLLEY_CUTTING_EDGE ? 12 : 11;
                for (k = PTCL_SPARKS; k != PTCL_SMOKE; k++) {
                  struct Particle *p = &PARTICLES[k];

                  if (p->variant == 0) {
                    p->x = px(position.x);
                    p->y = px(position.y);
                    p->velocityX = ((Random16() & 255) - 128) << shift;
                    p->velocityY = ((Random16() & 255) - 128) << shift;
                    p->variant = (Random16() & 7) + 16;
                    spawned++;
                    if (spawned == gProjectileVolley_Modes[mode][VOLLEY_SPARKS])
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
            DRAW_ALTERNATE(
                canvas, work->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                pos->x - 16, pos->y - gProjectileVolley_CometHeights[kind], 32,
                gProjectileVolley_CometHeights[kind]);
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
                           work->sheet + PROJECTILE_VOLLEY_COMET_PICTURES,
                           pos->x - 16, pos->y, PROJECTILE_VOLLEY_COMET_WIDTH,
                           gProjectileVolley_CometHeights[kind]);
            RELEASE_DRAW_BLITTER();
          } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
            if (ABS(shot->x) <= ABS(target->x))
              draw[0](canvas, work->sheet + frame % 6 * 768, pos->x - 16,
                      pos->y - 12, SHURIKEN_WIDTH, SHURIKEN_HEIGHT);
          } else if (mode == PROJECTILE_VOLLEY_SLEET ||
                     mode == PROJECTILE_VOLLEY_BLIZZARD ||
                     mode == PROJECTILE_VOLLEY_ICE_BLESSING ||
                     mode == PROJECTILE_VOLLEY_CHILL_BREATH) {
            draw[0](canvas,
                    work->sheet + gProjectileVolley_PelletPictures[j & 3],
                    pos->x - 4, pos->y - 4, 8, 8);
          } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
            draw[0](canvas, work->sheet + j % 6 * 128, pos->x - 4, pos->y - 8,
                    8, 16);
          } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER) {
            draw[0](canvas, work->sheet + 96, pos->x - 9, pos->y - 7, 18, 13);
          } else if (mode == PROJECTILE_VOLLEY_WING_BEAT) {
            draw[0](canvas, work->sheet, pos->x - 6, pos->y - 4, 12, 8);
          } else if (mode >= PROJECTILE_VOLLEY_BEAST_NEEDLE &&
                     mode <= PROJECTILE_VOLLEY_STUN_STING) {
            draw[0](canvas, work->sheet, pos->x - 8, pos->y - 1, 17, 2);
          } else if (mode == PROJECTILE_VOLLEY_HYDRO_SLASH) {
            draw[0](canvas, work->sheet, pos->x - 12, pos->y - 12, 24, 24);
          } else if (mode == PROJECTILE_VOLLEY_DIAMOND_DUST) {
            // Five crystals, each turning through three cels.
            s32 cel = (i * shots + j) % 5 * 3 + frame / 4 % 3;
            u32 width;
            u32 height;
            u8 *picture = work->sheet + gPrism_ShardPictures[cel] + 0x800;
            s32 left;
            s32 top;

            width = gPrism_ShardWidths[cel];
            left = pos->x - (width >> 1);
            height = gPrism_ShardHeights[cel];
            top = pos->y - (height >> 1);
            draw[0](canvas, picture, left, top, width, height);
          } else if (mode == PROJECTILE_VOLLEY_WING_STROKE) {
            draw[0](canvas, work->sheet + 330, pos->x - 15, pos->y - 12, 29,
                    23);
          } else {
            draw[0](canvas, work->sheet, pos->x - 20, pos->y - 32,
                    FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
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

            if (!(i & 1)) {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(
                  canvas, work->sheet + gProjectileVolley_FlashPictures[step],
                  flash->x - gProjectileVolley_FlashWidths[step],
                  flash->y - gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              /* FAKEMATCH(dead): an empty once-only block before the release.
               */
              do {
              } while (0);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(
                  canvas, work->sheet + gProjectileVolley_FlashPictures[step],
                  flash->x,
                  flash->y + gProjectileVolley_FlashSteps[step] -
                      gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              RELEASE_DRAW_BLITTER();
            } else {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
              {
                s32 offset = gProjectileVolley_FlashPictures[step];
                u8 *picture;
                s32 rise;
                s32 left;
                s32 width;

                // The picture, x and rise are read in a once-only block, after
                // the offset and before the width (otherwise 2 to 4 of 2278
                // differ, 2026-11-10).
                do {
                  picture = work->sheet + offset + 0x128a;
                  left = flash->x;
                  rise = gProjectileVolley_FlashRises[step];
                } while (0);
                width = gProjectileVolley_FlashWidths[step];
                DRAW_ALTERNATE(canvas, picture, left - rise, flash->y - width,
                               gProjectileVolley_FlashHeights[step], width);
              }
              do {
              } while (0);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  BLITTER_MAX);
              DRAW_ALTERNATE(canvas,
                             work->sheet +
                                 gProjectileVolley_FlashPictures[step] + 0x128a,
                             flash->x + gProjectileVolley_FlashSteps[step] -
                                 gProjectileVolley_FlashRises[step],
                             flash->y, gProjectileVolley_FlashHeights[step],
                             gProjectileVolley_FlashWidths[step]);
              RELEASE_DRAW_BLITTER();
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce at y = 112 (the beam's float up).
      for (i = PTCL_SPARKS; i != PTCL_SMOKE; i++) {
        struct Particle *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 3) + 1;

          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_UPPER_HALF(p->x) - size / 2,
                  Q_16_16_UPPER_HALF(p->y) - size, size, size * 2);
          if (mode == PROJECTILE_VOLLEY_HYDRO_SLASH)
            BattleEffectKernels_IntegrateVector2(p, 60, px(1) / 4);
          else
            BattleEffectKernels_IntegrateVector2(p, 60, px(1) / 16);
          if (p->y > px(112))
            p->velocityY = -p->velocityY / 2;
          p->variant--;
        }
      }

      // Smoke rises.
      for (i = PTCL_SMOKE; i != PTCL_STREAKS; i++) {
        struct Particle *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 4) + 1;

          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  Q_16_16_UPPER_HALF(p->x) - size / 2,
                  Q_16_16_UPPER_HALF(p->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(p, 60, -px(1) / 4);
          p->variant--;
        }
      }

      if (mode == PROJECTILE_VOLLEY_POISON_STING) {
        // Smoke billows up where each streak landed: six cels, four frames
        // each.
        for (i = 0; i != 64; i++) {
          struct Particle *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 23) {
            draw[0](
                canvas,
                work->sheet + 0x22 + p->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
                Q_16_16_UPPER_HALF(p->x) - 12, Q_16_16_UPPER_HALF(p->y) - 24,
                SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
            BattleEffectKernels_IntegrateVector2(p, 62, -px(1) / 64);
            p->variant++;
          }
        }
      } else if (mode == PROJECTILE_VOLLEY_STUN_STING) {
        // Glints of water spray scatter about each landed streak, each with
        // a random palette.
        for (i = 0; i != 64; i++) {
          struct Particle *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 7) {
            s32 glint = frame & 3;

            s32 angle = Random16() & TRIG_ANGLE_MASK;
            s32 radius = (Random16() & 31) + 4;
            s32 glintX = Q_16_16_UPPER_HALF(p->x) +
                         ((Trig_Sin(angle) * radius) >> 17) -
                         (gBattleEffect_GlintPictureWidths[glint] >> 1);
            s32 glintY = Q_16_16_UPPER_HALF(p->y) -
                         ((Trig_Cos(angle) * radius) >> 17) -
                         (gBattleEffect_GlintPictureHeights[glint] >> 1);

            BattleEffect_BuildBlitter(
                188, 7, 7, gProjectileVolley_GlintFlips[Random16() & 3] | CLIP,
                0);
            do {
              /* FAKEMATCH(dead): an empty once-only block before the glints. */
            } while (0);
            DRAW_ALTERNATE(
                canvas, work->sheet + 0x22 + gBattleEffect_GlintPictures[glint],
                glintX, glintY, gBattleEffect_GlintPictureWidths[glint],
                gBattleEffect_GlintPictureHeights[glint]);
            HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            p->variant++;
          }
        }
      }
      BattleCamera_Shake(4, 4);
      EFFECT_END_FRAME(work);
    }
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Sends a unit back to its place (its anchor) with a hop.
void BattleEffect_InitializeRisingObject(s32 slot) {
  struct BattleObject *context = BattleObject_GetFar(slot);
  struct Actor *object = context->object;

  object->acceleration = px(2);
  object->speedLimit = px(8);
  object->velocityY = px(4);
  object->gravity = 0xAB85;
  object->bounce = 0;
  object->steeringFlags = 0;
  object->snapToTarget = 1;
  Actor_StartIdleFar(object);
  Actor_SetMoveTargetFar(object, context->anchorX, 0, context->anchorZ);
  Actor_SelectAnimationFar(object, 1);
}

/// Moves the object rate percent of the way to the target's object in the
/// given frames.
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames,
                                    s32 rate) {
  struct BattleObject *slot = BattleObject_GetFar(id);
  struct BattleObject *targetSlot = BattleObject_GetFar(targetId);
  struct Actor *obj = (struct Actor *)slot->object;
  struct Actor *target = (struct Actor *)targetSlot->object;
  s32 diffX = target->x.fixed - obj->x.fixed;
  s32 startX = obj->x.fixed;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z.fixed - obj->z.fixed;
  s32 startZ = obj->z.fixed;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 sx = dx >> 8; // the distance in 1/256 pixels
  s32 sz = dz >> 8;
  s32 dist;

  dist = Iwram_Sqrt(sx * sx + sz * sz);
  dist = (dist << 8) / frames;
  obj->acceleration = dist;
  obj->speedLimit = dist;
  obj->snapToTarget = 1;
  obj->gravity = 0xab85;
  obj->velocityY = 0;
  obj->bounce = 0;
  obj->steeringFlags = 1;
  Actor_StartIdleFar(obj);
  Actor_SetMoveTargetFar(obj, x, 0, z);
  Actor_SelectAnimationFar(obj, 2);
}
