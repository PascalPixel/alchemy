/// @file
/// Projectile volleys (Slash, Shuriken, Wing Flutter, Blizzard ...).
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// Sparks swirl around a point that travels from the user to the goal and
/// gather into a Djinni, which sways for 16 frames and bursts into motes.
/// Leaves the canvas cleared for the animation that follows.
///
/// The prelude of the stat boosts, the projectile volleys, the healing motes,
/// the Djinn beams, the burst animations, Bind, Hail, Ground, Torch and Vine,
/// called with the work block they then use. Opens the canvas layer but does
/// not close it: it removes the transfer and palette fade callbacks, frees
/// both blitters, clears the canvas and its VRAM, and sets REG_BLDALPHA back
/// to 16/16. Copies the Djinni's palette over the first 128 bytes of BG
/// palette RAM, starts a 24-frame battle background fade (work->fadeFrames),
/// and uses PARTICLES 0 to 63 for the sparks and work->particles for the motes.
///
/// effect: the user and the targets.
/// element: the Djinni's element, plus DJINNI_ENTRANCE_LONG for the 84-frame
///          entrance instead of the 64-frame one.
/// side: ANIM_SIDE_PARTY mirrors the pictures.
/// anchor: where the Djinni appears (enum DjinniEntranceAnchor).
/// outX, outY: receive the Djinni's last screen position.
void BattleAnim_RunDjinniEntrance(struct BattleAnimArgs *effect, s32 element,
                                  s32 side, s32 anchor, s32 *outX, s32 *outY) {
  s32 center[3];
  s32 goal[3];
  s32 step[3];
  s32 point[3];
  struct ScreenPoint position;
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  s32 frameCount;
  struct Actor *target;
  struct Actor *user;
  u8 *palette;
  s32 sheetId;
  s32 frame;
  s32 i;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  sheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  if (element > ELEMENT_JUPITER) {
    element -= DJINNI_ENTRANCE_LONG;
    frameCount = 84;
  } else {
    frameCount = 64;
  }
  switch (element) {
  case ELEMENT_VENUS:
    sheetId = (s32)&ResourceId_VenusDjinnSmallSheet;
    break;
  case ELEMENT_MERCURY:
    sheetId = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case ELEMENT_MARS:
    sheetId = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    sheetId = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  // The small Djinni's resource is its palette, then its picture.
  palette = Resource_GetTableEntry(sheetId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Decompress_TaggedLz(palette + 128, work->sheet);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (side == ANIM_SIDE_PARTY) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_LIGHTEN);
  } else {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  }
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  user = GetBattleObjectSlotFar(work->effect->user)->actor;
  target = GetBattleObjectSlotFar(work->effect->targets[0])->actor;
  // Sparks spread out flat from the centre, 20 to 51 pixels up, at 4 to 12
  // pixels a frame (speed / 32).
  for (i = 0; i != 64; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 angle = Random16() & 0xffff;
    s32 speed = (Random16() & 255) + 128;

    SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
    SET_VELOCITY(spark, (Trig_Sin(angle) * speed) >> 5, 0,
                 (Trig_Cos(angle) * speed) >> 5);
    spark->variant = 0;
  }
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  // The centre moves from the user to the goal, 60 pixels up, over frames
  // 6 to 45.
  SET_VECTOR(center, user->x.fixed, 0, user->z.fixed);
  // Every caller passes a literal anchor, so goal is always set before the
  // interpolation starts; none passes DJINNI_ANCHOR_USER_X.
  switch (anchor) {
  case DJINNI_ANCHOR_OVER_TARGET:
    SET_VECTOR(goal, target->x.fixed, 0x3c0000, target->z.fixed);
    break;
  case DJINNI_ANCHOR_TARGET_X:
    SET_VECTOR(goal, target->x.fixed, 0x3c0000, 0);
    break;
  case DJINNI_ANCHOR_OVER_USER:
    SET_VECTOR(goal, user->x.fixed, 0x3c0000, user->z.fixed);
    break;
  case DJINNI_ANCHOR_USER_X:
    SET_VECTOR(goal, user->x.fixed, 0x3c0000, 0);
    break;
  case DJINNI_ANCHOR_CENTRE:
    SET_VECTOR(goal, 0, 0x3c0000, 0);
    break;
  }
  SET_VECTOR(step, (goal[0] - center[0]) / 40, (goal[1] - center[1]) / 40,
             (goal[2] - center[2]) / 40);
  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    // The long version fades the canvas out over its last 8 frames; the sound
    // plays on frame 8.
    if (frame > 75)
      REG_BLDALPHA = BLEND_ALPHA(168 - frame * 2, 16);
    if (frame == 8)
      Sound_PlaySongFar(SE_POWER_UP);
    ANIM_VIEW_FROM_CAMERA(camera);
    if (frame >= 6 && frame <= 45) {
      center[0] += step[0];
      center[1] += step[1];
      center[2] += step[2];
    }
    Transform_ApplyPosition(center);
    // The user glows white from frame 0 to frame 24.
    if (frame == 0)
      BattleAnim_TintBattler(work->effect->user, SPRITE_COLOR_WHITE, -1, -1, 0);
    if (frame == 24)
      BattleAnim_TintBattler(work->effect->user, 0, -1, -1, 0);
    // Only the first 32 sparks are drawn. Spark i appears on frame i / 8 and
    // spins about the centre on one of four axes, (256 + 32i) / 65536 turn a
    // frame, drawn larger the nearer it is. From 24 frames after appearing
    // its speed gains 1/128 of its offset towards the centre each frame, then
    // keeps 62/64 of itself (on top of the step's own 60/64), and it goes out
    // once it is within 4 pixels of the centre across and in depth (height
    // is not tested).
    for (i = 0; i != 32; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (frame >= i / 8 && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Transform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case SPIN_PITCH:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case SPIN_ROLL:
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case SPIN_PITCH_AND_ROLL:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        Projection_Point(&spark->x, &position);
        position.x >>= 1;
        Transform_Pop();
        CLAMP(position.depth, 250, 634);
        size = 8 - (position.depth - 250) / 64;
        ANIM_DRAW_MOTE(draw[1], canvas, sheet, position.x, position.y, size);
        Particle_AdvanceWithGravity3D(spark, 60, 0);
        if (frame >= i / 8 + 24) {
          s32 pullX = -spark->x >> 7;
          s32 pullY = -spark->y >> 7;
          s32 pullZ = -spark->z >> 7;

          spark->velocityX += pullX;
          spark->velocityY += pullY;
          spark->velocityZ += pullZ;
          SET_VELOCITY(spark, spark->velocityX * 62 / 64,
                       spark->velocityY * 62 / 64, spark->velocityZ * 62 / 64);
          if ((pullX > -2048 && pullX < 2048) &&
              (pullZ > -2048 && pullZ < 2048))
            spark->variant = -1;
        }
      }
    }
    // The Djinni sways 4 pixels either side of the centre on frames 54 to
    // 69; the short version ends on frame 63, before its burst.
    if (frame >= 54 && frame <= 69) {
      SET_VECTOR(point, (Trig_Sin(frame << 10) * 4), 0, 0);
      Projection_Point(point, &position);
      *outX = position.x;
      *outY = position.y;
      position.x >>= 1;
      draw[0](canvas, work->sheet, position.x - 10, position.y - 20, 20, 40);
    }
    // On frame 64 (long version only), 64 motes burst from the Djinni, each
    // lasting 9 to 24 frames, thrown in a random direction at 2 to 6 pixels a
    // frame (speed / 64) with the vertical part doubled. The Djinni's x is a
    // screen position, halved for the canvas.
    if (frame == 64) {
      for (i = 0; i != 64; i++) {
        struct Particle *mote = &work->particles[i];
        s32 angle = Random16() & 0xffff;
        s32 speed = (Random16() & 255) + 128;

        mote->x = *outX << 15;
        mote->y = *outY << 16;
        mote->velocityX = (Trig_Sin(angle) * speed) >> 6;
        mote->velocityY = (Trig_Cos(angle) * speed) >> 5;
        mote->variant = (Random16() & 15) + 8;
      }
    }
    // Live motes shrink as their frames run out.
    if (frame > 63) {
      for (i = 0; i != 64; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          ANIM_DRAW_MOTE(draw[0], canvas, sheet, Q_16_16_TO_INT(mote->x),
                         Q_16_16_TO_INT(mote->y), size);
          Particle_AdvanceWithGravity2D(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    ANIM_PRESENT_FRAME(work);
  }
  // Leave the canvas cleared and fully shown for the animation that follows,
  // without closing the layer.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  ANIM_RELEASE_BLITTERS();
  Task_Remove(Palette_StepFadeTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
}

// Projectile volleys and rushing strikes.
// The folder README describes them under "Animation families".

#define PROJECTILE_VOLLEY_ANIMATION_STEP_FAST 0x30 ///< the user's, while firing
/// The user's animation step after the volley: the usual speed.
#define PROJECTILE_VOLLEY_ANIMATION_STEP_NORMAL 0x10

/// Six comets fly at each target, three frames apart, and burst into a flash
/// and sparks; the animation's variant picks one of three comets.
///
/// Abilities: Slash, Wind Slash, Sonic Slash (BATTLE_ANIM_SLASH).
void BattleAnim_RunSlash(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_SLASH);
}
/// Four spinning shurikens fly at each target, five frames apart, and vanish
/// once past it.
///
/// Abilities: Shuriken (BATTLE_ANIM_SHURIKEN).
void BattleAnim_RunShuriken(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_SHURIKEN);
}
/// Ten pellets fly slowly at each target and bounce off it, up or down at
/// random.
///
/// Abilities: Crazy Voice (BATTLE_ANIM_CRAZY_VOICE).
void BattleAnim_RunCrazyVoice(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_CRAZY_VOICE);
}
/// Five yellow sparks fly level at each target, five frames apart.
///
/// Abilities: Wing Flutter (BATTLE_ANIM_WING_FLUTTER).
void BattleAnim_RunWingFlutter(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_WING_FLUTTER);
}
/// Ten large yellow sparks stream level at each target, one a frame, spread up
/// and down.
///
/// Abilities: Wing Stroke (BATTLE_ANIM_WING_STROKE).
void BattleAnim_RunWingStroke(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_WING_STROKE);
}
/// Ten small yellow sparks stream level at each target, one a frame.
///
/// Abilities: Wing Beat (BATTLE_ANIM_WING_BEAT).
void BattleAnim_RunWingBeat(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_WING_BEAT);
}
/// Puts the user into its leap pose and fires the volley in this mode
/// while its animation runs fast.
///
/// Called by Cutting Edge, Heat Wave and Blizzard. Leaves the user in its
/// action pose, its animation back at the usual speed.
static inline void BattleAnim_RunVolleyWithAction(struct BattleAnimArgs *effect,
                                                  s32 mode) {
  struct Actor *user;

  user = GetBattleObjectSlotFar(effect->user)->actor;
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, PROJECTILE_VOLLEY_ANIMATION_STEP_FAST);
  BattleAnim_RunProjectileVolley(effect, mode);
  Actor_SetAnimationSpeedFar(user, PROJECTILE_VOLLEY_ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, one large comet in ice colours flies at each
/// target and bursts into 80 sparks.
///
/// Abilities: Cutting Edge (BATTLE_ANIM_CUTTING_EDGE).
void BattleAnim_RunCuttingEdge(struct BattleAnimArgs *effect) {
  BattleAnim_RunVolleyWithAction(effect, PROJECTILE_VOLLEY_CUTTING_EDGE);
}
/// From the user's action pose, seven fireballs trailing embers fly at each
/// target, two frames apart, each bursting into 40 sparks.
///
/// Abilities: Heat Wave (BATTLE_ANIM_HEAT_WAVE).
void BattleAnim_RunHeatWave(struct BattleAnimArgs *effect) {
  BattleAnim_RunVolleyWithAction(effect, PROJECTILE_VOLLEY_HEAT_WAVE);
}
/// Thirty ice chips fall from high above the user onto each target, without
/// shaking the screen or knocking the target back; the battle shows the damage
/// on frame 48.
///
/// Abilities: Sleet (BATTLE_ANIM_SLEET).
void BattleAnim_RunSleet(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_SLEET);
}
/// Sixty ice chips stream at each target, one a frame, without knocking it
/// back.
///
/// Abilities: Ice Blessing (BATTLE_ANIM_ICE_BLESSING).
void BattleAnim_RunIceBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunProjectileVolley(effect, PROJECTILE_VOLLEY_ICE_BLESSING);
}
/// From the user's action pose, sixty ice chips stream at each target, one a
/// frame, scattered up and down, without knocking it back.
///
/// Abilities: Blizzard (BATTLE_ANIM_BLIZZARD).
void BattleAnim_RunBlizzard(struct BattleAnimArgs *effect) {
  BattleAnim_RunVolleyWithAction(effect, PROJECTILE_VOLLEY_BLIZZARD);
}

/// PARTICLES holds the hit flashes from 0, the sparks from 100 and the
/// fireball trails from 512.
#define PROJECTILE_VOLLEY_PARTICLE_COUNT 1024

/// The bytes of a gProjectileVolley_Modes row.
enum VolleyField {
  PROJECTILE_VOLLEY_SHOTS,   ///< shots at each target
  PROJECTILE_VOLLEY_STEPS,   ///< frames a shot takes to reach its target
  PROJECTILE_VOLLEY_STAGGER, ///< frames between shots
  PROJECTILE_VOLLEY_DELAY,   ///< frames between targets
  PROJECTILE_VOLLEY_SPARKS   ///< sparks thrown on each hit
};

// Where the volley's pictures sit in the sheet.
#define PROJECTILE_VOLLEY_COMET_CELLS 0x1000  ///< comets: the top half of each
#define PROJECTILE_VOLLEY_STREAK_CELLS 0x2000 ///< wind streaks
/// The hit flashes turned on their side, after the upright ones.
#define PROJECTILE_VOLLEY_FLASH_TURNED 0x128a
/// The yellow spark sheet: the small spark (12x8) first, then the medium one
/// (18x13), then the large one.
#define PROJECTILE_VOLLEY_MEDIUM_SPARK_CELLS (12 * 8)
#define PROJECTILE_VOLLEY_LARGE_SPARK_CELLS (12 * 8 + 18 * 13)
#define PROJECTILE_VOLLEY_DJINNI_CELLS 0x65c0 ///< the small Mercury Djinni
#define PROJECTILE_VOLLEY_FIRST_SPARK 100     ///< the first spark in PARTICLES
#define PROJECTILE_VOLLEY_FIRST_TRAIL 512     ///< the first trail in PARTICLES

/// Shots fly from the user to each target and burst into a flash and sparks
/// when they pass it. The hit sound plays at most once every 8 frames.
///
/// Called by the eleven wrappers above, and by two unused casting impacts
/// once their prelude ends: CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS fires
/// Blizzard's volley, CASTING_IMPACT_IMPACT_UNUSED_COMETS
/// PROJECTILE_VOLLEY_IMPACT_COMETS, which no wrapper uses. With more than 63
/// shots in all it fires at the first target only, writing
/// work->effect->count. Writes REG_BLDCNT for Shuriken and the first 128
/// bytes of BG palette RAM for Crazy Voice and Cutting Edge; sets
/// work->shakeFrames on each hit but for Sleet. The second blitter is built and
/// freed for each picture that needs it.
///
/// effect: the user and the targets. variant picks Slash's comet (Cutting
///         Edge always fires PROJECTILE_VOLLEY_COMET_C, the impact's comets
///         PROJECTILE_VOLLEY_COMET_A); with prelude 1 a Mercury Djinni
///         enters first.
/// mode: the shot and its flight (enum ProjectileVolleyMode).
void BattleAnim_RunProjectileVolley(struct BattleAnimArgs *effect, s32 mode) {
  struct ScreenPoint position;
  s32 screenX;
  s32 screenY;
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 soundCooldown;
  s32 total;
  s32 comet;
  s32 shots;
  s32 steps;
  struct Actor *user;
  s32 k;
  // The trail particles placed, and later the sparks' speed shift: one
  // local for both, as separate locals change the register allocation.
  s32 trailsOrShift;
  s32 hit;
  s32 x;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  camera = ANIM_SLOT(slots, HEAP_SLOT_CAMERA);
  sheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  soundCooldown = 0;
  work->effect = effect;
  comet = effect->variant;
  // The canvas's BG priority: 0 for Ice Blessing, 1 for the others. A
  // Djinni prelude then opens the layer again at priority 0.
  if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
    BattleAnim_StartCanvasLayer(0);
  else
    BattleAnim_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (work->effect->prelude == 1)
    BattleAnim_RunDjinniEntrance(effect, ELEMENT_MERCURY, work->effect->side,
                                 DJINNI_ANCHOR_OVER_USER, &screenX, &screenY);

  // The main blitter, mirrored on ANIM_SIDE_PARTY: brightest-wins for Cutting
  // Edge, additive for the others.
  if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
    if (work->effect->side == ANIM_SIDE_PARTY)
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    else
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
  } else {
    if (work->effect->side == ANIM_SIDE_PARTY)
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    else
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
  }
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];

  // The shot pictures, and how long each mode's trails last: comets keep
  // three quarters of each level a frame, shurikens none, the rest half.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == PROJECTILE_VOLLEY_SLASH ||
      mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
      mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
      comet = PROJECTILE_VOLLEY_COMET_C;
    if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
      comet = PROJECTILE_VOLLEY_COMET_A;
    if (comet == PROJECTILE_VOLLEY_COMET_A)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_CELLS, 1,
                                 1);
    else if (comet == PROJECTILE_VOLLEY_COMET_B)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_CELLS, 1,
                                 1);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_CELLS, 1,
                                 1);
    // The ice palette is copied twice, before and after the streaks load.
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      Iwram_CopyWords((void *)BG_PLTT, pal, 128);
    }
    Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet,
                               work->sheet + PROJECTILE_VOLLEY_STREAK_CELLS, 1,
                               0);
    if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE) {
      const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      Iwram_CopyWords((void *)BG_PLTT, pal, 128);
    }
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
    Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work->sheet, 1,
                               1);
    REG_BLDCNT = 0;
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage,
                               work->sheet, 0, 0);
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER ||
             mode == PROJECTILE_VOLLEY_WING_BEAT ||
             mode == PROJECTILE_VOLLEY_WING_STROKE) {
    Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work->sheet,
                               1, 1);
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work->sheet, 1,
                               1);
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    // The ice chips, and the small Mercury Djinni the prelude draws.
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               work->sheet + PROJECTILE_VOLLEY_DJINNI_CELLS, 1,
                               0);
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  }
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // Where the shots start and how fast they fly.
  user = GetBattleObjectSlotFar(work->effect->user)->actor;
  shots = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_SHOTS]);
  steps = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_STEPS]);
  {
    s32 stagger = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_STAGGER]);
    s32 delay = (gProjectileVolley_Modes[mode][PROJECTILE_VOLLEY_DELAY]);
    s32 height = BattleUnit_GetEffectHeightFar(work->effect->user);

    // Keep the shots within work->particles' 64.
    if (work->effect->targetCount * shots > 63)
      work->effect->targetCount = 1;

    // Each shot starts at the user, at a height its mode picks, and is aimed
    // to reach its target in `steps` frames: straight across for sparks and
    // Cutting Edge, at a random height near the target for the others.
    //
    // Heights above the feet, in pixels, h being the battler's table height
    // (BattleUnit_GetEffectHeightFar): a shot starts at 58 to 73 for Sleet,
    // h / 2 for Cutting Edge, Heat Wave and Ice Blessing, h / 2 - 15 to
    // h / 2 + 16 for Wing Flutter, Wing Beat and Blizzard, h / 2 - 31 to
    // h / 2 + 32 for Wing Stroke, and h for the rest. It aims at -12 to 51
    // above the target for Sleet, 22 to 29 for the impact's comets, 1 to 64
    // for Blizzard, 4 to 35 for Ice Blessing, h / 2 to h / 2 + 15 for Heat
    // Wave and h - 15 to h for Slash, Shuriken and Crazy Voice.

    ANIM_FOR_EACH_TARGET(i, work->effect) {
      struct Actor *target =
          GetBattleObjectSlotFar(work->effect->targets[i])->actor;
      s32 targetHeight =
          BattleUnit_GetEffectHeightFar(work->effect->targets[i]);

      for (j = 0; j != shots; j++) {
        struct Particle *shot = &work->particles[i * shots + j];

        shot->x = user->x.fixed;
        if (mode == PROJECTILE_VOLLEY_SLEET)
          shot->y = user->y.fixed + ((Random16() & 15) << 16) + 0x3a0000;
        else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
          shot->y = user->y.fixed + height / 2;
        else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
          shot->y = user->y.fixed + height / 2;
        else if (mode == PROJECTILE_VOLLEY_BLIZZARD)
          shot->y =
              user->y.fixed + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if ((u32)(mode - PROJECTILE_VOLLEY_WING_FLUTTER) <= 1)
          shot->y =
              user->y.fixed + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == PROJECTILE_VOLLEY_WING_STROKE)
          shot->y =
              user->y.fixed + height / 2 + ((32 - (Random16() & 63)) << 16);
        else if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
          shot->y = user->y.fixed + height / 2;
        else
          shot->y = user->y.fixed + height;
        shot->z = user->z.fixed;
        shot->velocityX = (target->x.fixed - shot->x) / steps;
        if (mode == PROJECTILE_VOLLEY_SLEET)
          shot->velocityY = (s32)(target->y.fixed + ((Random16() & 63) << 16) -
                                  shot->y - 0xc0000) /
                            steps;
        else if (mode == PROJECTILE_VOLLEY_IMPACT_COMETS)
          shot->velocityY = (s32)(target->y.fixed + ((Random16() & 7) << 16) -
                                  shot->y + 0x160000) /
                            steps;
        else if (mode == PROJECTILE_VOLLEY_BLIZZARD)
          shot->velocityY = (s32)(target->y.fixed +
                                  ((64 - (Random16() & 63)) << 16) - shot->y) /
                            steps;
        else if (mode == PROJECTILE_VOLLEY_ICE_BLESSING)
          shot->velocityY = (s32)(target->y.fixed + ((Random16() & 31) << 16) -
                                  shot->y + 0x40000) /
                            steps;
        else if ((u32)(mode - PROJECTILE_VOLLEY_WING_FLUTTER) <= 1 ||
                 mode == PROJECTILE_VOLLEY_WING_STROKE ||
                 mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
          shot->velocityY = 0;
        else if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
          shot->velocityY = (s32)(target->y.fixed + targetHeight / 2 +
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        else
          shot->velocityY = (s32)(target->y.fixed + targetHeight -
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        shot->velocityZ = (target->z.fixed - shot->z) / steps;
        shot->variant = 0;
      }
    }
    // Free every flash, spark and trail.
    for (i = 0; i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++)
      PARTICLES[i].variant = 0;
    // Every shot's flight, then 16 frames (32 for fireballs) for the last
    // hits to fade.
    if (mode == PROJECTILE_VOLLEY_HEAT_WAVE)
      total = work->effect->targetCount * delay + shots * stagger + 32;
    else
      total = work->effect->targetCount * delay + shots * stagger + 16;

    // Each frame: the prelude's Djinni, the shots and their hits, the hit
    // flashes, the sparks and the trails, in that order.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      // The battle shows the damage on frame 16, 32 for Heat Wave and 48 for
      // Sleet; Cutting Edge shows it on its first hit instead.
      if (soundCooldown > 0)
        soundCooldown--;
      if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
        if (frame == 4)
          Sound_PlaySongFar(SE_WHOOSH);
        if (frame == 32)
          ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      } else if (mode == PROJECTILE_VOLLEY_SLEET) {
        if (frame == 48)
          ANIM_SHOW_DAMAGE(SE_IMPACT);
      } else if (mode != PROJECTILE_VOLLEY_CUTTING_EDGE) {
        if (frame == 16)
          ANIM_SHOW_DAMAGE(SE_IMPACT);
      }

      // Prelude: the small Djinni circles, 4 pixels across and 2 up and down
      // every 32 frames, and from frame 70 rises away 2 pixels a frame; it is
      // drawn twice on the first four frames. Its blitter is built and freed
      // each frame.
      if (work->effect->prelude == 1) {
        s32 x = ((-Trig_Sin(frame << 11) * 4) >> 16) + screenX / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 2) >> 16) + screenY - 24;

        if (frame > 69)
          y = y - frame * 2 + 138;
        if (work->effect->side == ANIM_SIDE_PARTY)
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
        else
          ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
        draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
        if (frame <= 3)
          draw[1](canvas, work->sheet + PROJECTILE_VOLLEY_DJINNI_CELLS, x, y,
                  20, 40);
        draw[1](canvas, work->sheet + PROJECTILE_VOLLEY_DJINNI_CELLS, x, y, 20,
                40);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
      }

      // The shots: shot j at target i launches after frame
      // j * stagger + i * delay. index counts every shot, so each has its own
      // hit flash in PARTICLES.
      ANIM_VIEW_FROM_CAMERA(camera);
      index = 0;
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        struct Actor *target =
            GetBattleObjectSlotFar(work->effect->targets[i])->actor;

        for (j = 0; j != shots; j++, index++) {
          if (j * stagger + i * delay < frame) {
            struct Particle *shot = &work->particles[i * shots + j];
            struct ScreenPoint *screenPosition = &position;

            // The screen position (half across, on the canvas).
            Projection_Point(shot, screenPosition);
            screenPosition->x >>= 1;
            shot->x += shot->velocityX;
            shot->y += shot->velocityY;
            shot->z += shot->velocityZ;
            if (mode == PROJECTILE_VOLLEY_HEAT_WAVE) {
              // Leave two trail particles behind the fireball, drifting up to
              // 4 pixels a frame each way and living 32 to 39 frames.
              trailsOrShift = 0;
              for (k = PROJECTILE_VOLLEY_FIRST_TRAIL;
                   k != PROJECTILE_VOLLEY_PARTICLE_COUNT; k++) {
                struct Particle *particle = &PARTICLES[k];

                if (particle->variant == 0) {
                  particle->x = position.x << 16;
                  particle->y = position.y << 16;
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
              // A shot hits once it is on the target's side of the field and
              // past the target's x.
              x = shot->x;
              if (target->x.fixed < 0) {
                hit = x < 0;
                /* FAKEMATCH(dead): two updates that cancel, under a test that
                 * emits nothing, make hit a 0 or 1 value before it is tested;
                 * without them, 6 of 1605 instructions differ in tbs-en. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              } else {
                hit = x >= 0;
                /* FAKEMATCH(dead): the same pair for the other side; without
                 * it, 6 of 1605 instructions differ in tbs-en. */
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
                  ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                } else if (mode != PROJECTILE_VOLLEY_HEAT_WAVE &&
                           soundCooldown == 0) {
                  soundCooldown = 8;
                  Sound_PlaySongFar(SE_WATER);
                }
                // The hit: the shot's flash starts where it was drawn, the
                // screen shakes (but for Sleet), the target flashes and,
                // but for the ice chips, recoils.
                if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
                  // Pellets bounce back, up or down at random, gaining 2 to 4
                  // pixels a frame of vertical speed.
                  s32 kick = ((Random16() & 31) + 32) << 12;

                  if (Random16() & 1)
                    shot->velocityY += kick;
                  else
                    shot->velocityY -= kick;
                  shot->velocityX = -shot->velocityX;
                }
                flash->variant = 1;
                SET_POSITION(flash, screenPosition->x, screenPosition->y, 0);
                if (mode != PROJECTILE_VOLLEY_SLEET)
                  work->shakeFrames = 2;
                BattleAnim_TintBattler(work->effect->targets[i],
                                       SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
                if (mode != PROJECTILE_VOLLEY_SLEET &&
                    mode != PROJECTILE_VOLLEY_BLIZZARD &&
                    mode != PROJECTILE_VOLLEY_ICE_BLESSING) {
                  if (mode == PROJECTILE_VOLLEY_CUTTING_EDGE)
                    BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                                   KNOCKBACK_HURL);
                  else
                    BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                                   KNOCKBACK_SKID);
                }
                // Sparks: the mode's count, in the first free slots from 100,
                // living 16 to 23 frames, at up to 4 pixels a frame each way;
                // Cutting Edge's fly twice as fast.
                trailsOrShift =
                    mode == PROJECTILE_VOLLEY_CUTTING_EDGE ? 12 : 11;
                for (k = PROJECTILE_VOLLEY_FIRST_SPARK;
                     k != PROJECTILE_VOLLEY_FIRST_TRAIL; k++) {
                  struct Particle *particle = &PARTICLES[k];

                  if (particle->variant == 0) {
                    particle->x = position.x << 16;
                    particle->y = position.y << 16;
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
            // Draw the shot, hit or not.
            if (mode == PROJECTILE_VOLLEY_SLASH ||
                mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
                mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
              // The picture holds the top half; the bottom half is drawn
              // flipped.
              if (work->effect->side == ANIM_SIDE_PARTY)
                ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                                  ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                  COMPOSITE_LIGHTEN);
              else
                ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                                  COMPOSITE_LIGHTEN);
              ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
                  canvas, work->sheet + PROJECTILE_VOLLEY_COMET_CELLS,
                  screenPosition->x - 16,
                  screenPosition->y - gProjectileVolley_CometHeights[comet], 32,
                  gProjectileVolley_CometHeights[comet]);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
              if (work->effect->side == ANIM_SIDE_PARTY)
                ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                                  ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_LIGHTEN);
              else
                ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                                  ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                                  COMPOSITE_LIGHTEN);
              ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
                  canvas, work->sheet + PROJECTILE_VOLLEY_COMET_CELLS,
                  screenPosition->x - 16, screenPosition->y, 32,
                  gProjectileVolley_CometHeights[comet]);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
            } else if (mode == PROJECTILE_VOLLEY_SHURIKEN) {
              // Shurikens vanish once past their target.
              if (ABS(shot->x) <= ABS(target->x.fixed))
                draw[0](canvas, work->sheet + frame % 6 * (32 * 24),
                        screenPosition->x - 16, screenPosition->y - 12, 32, 24);
            } else if (mode == PROJECTILE_VOLLEY_SLEET ||
                       mode == PROJECTILE_VOLLEY_BLIZZARD ||
                       mode == PROJECTILE_VOLLEY_ICE_BLESSING) {
              s32 cell = j & 3;
              draw[0](canvas,
                      work->sheet + gProjectileVolley_PelletOffsets[cell],
                      screenPosition->x - 4, screenPosition->y - 4, 8, 8);
            } else if (mode == PROJECTILE_VOLLEY_CRAZY_VOICE) {
              draw[0](canvas, work->sheet + j % 6 * (8 * 16),
                      screenPosition->x - 4, screenPosition->y - 8, 8, 16);
            } else if (mode == PROJECTILE_VOLLEY_WING_FLUTTER) {
              draw[0](canvas,
                      work->sheet + PROJECTILE_VOLLEY_MEDIUM_SPARK_CELLS,
                      screenPosition->x - 9, screenPosition->y - 7, 18, 13);
            } else if (mode == PROJECTILE_VOLLEY_WING_BEAT) {
              draw[0](canvas, work->sheet, screenPosition->x - 6,
                      screenPosition->y - 4, 12, 8);
            } else if (mode == PROJECTILE_VOLLEY_WING_STROKE) {
              draw[0](canvas, work->sheet + PROJECTILE_VOLLEY_LARGE_SPARK_CELLS,
                      screenPosition->x - 15, screenPosition->y - 12, 29, 23);
            } else {
              draw[0](canvas, work->sheet, screenPosition->x - 20,
                      screenPosition->y - 32, 40, 64);
            }
          }
        }
      }

      // Hit flashes, 12 frames: half a picture, drawn again flipped for the
      // other half (odd flashes use the pictures turned on their side). A
      // flash's z counts its frames, stepping the picture every 2.
      if (mode == PROJECTILE_VOLLEY_SLASH ||
          mode == PROJECTILE_VOLLEY_CUTTING_EDGE ||
          mode == PROJECTILE_VOLLEY_IMPACT_COMETS) {
        for (i = 0; i != work->effect->targetCount * shots; i++) {
          struct Particle *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                                COMPOSITE_LIGHTEN);
              ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
                  canvas, work->sheet + gProjectileVolley_FlashOffsets[step],
                  flash->x - gProjectileVolley_FlashWidths[step],
                  flash->y - gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
              ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                                ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                                COMPOSITE_LIGHTEN);
              ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
                  canvas, work->sheet + gProjectileVolley_FlashOffsets[step],
                  flash->x,
                  flash->y + gProjectileVolley_FlashSteps[step] -
                      gProjectileVolley_FlashRises[step],
                  gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step]);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
            } else {
              ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS,
                                COMPOSITE_LIGHTEN);
              ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
                  canvas,
                  work->sheet + gProjectileVolley_FlashOffsets[step] +
                      PROJECTILE_VOLLEY_FLASH_TURNED,
                  flash->x - gProjectileVolley_FlashRises[step],
                  flash->y - gProjectileVolley_FlashWidths[step],
                  gProjectileVolley_FlashHeights[step],
                  gProjectileVolley_FlashWidths[step]);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
              ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                                ANIM_CLIP_BOUNDS | COMPOSITE_FLIP,
                                COMPOSITE_LIGHTEN);
              ((DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2])(
                  canvas,
                  work->sheet + gProjectileVolley_FlashOffsets[step] +
                      PROJECTILE_VOLLEY_FLASH_TURNED,
                  flash->x + gProjectileVolley_FlashSteps[step] -
                      gProjectileVolley_FlashRises[step],
                  flash->y, gProjectileVolley_FlashHeights[step],
                  gProjectileVolley_FlashWidths[step]);
              FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce off the bottom (y 112) at half speed,
      // shrinking as their frames run out: a gravity of 1/16 pixel a frame
      // each frame, keeping 60/64 of their speed.
      for (i = PROJECTILE_VOLLEY_FIRST_SPARK;
           i != PROJECTILE_VOLLEY_FIRST_TRAIL; i++) {
        struct Particle *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 3) + 1;

          ANIM_DRAW_MOTE(draw[0], canvas, sheet, Q_16_16_TO_INT(particle->x),
                         Q_16_16_TO_INT(particle->y), size);
          Particle_AdvanceWithGravity2D(particle, 60, 0x1000);
          if (particle->y > 0x700000)
            particle->velocityY = -particle->velocityY / 2;
          particle->variant--;
        }
      }

      // Fireball trails rise, gaining 1/4 pixel a frame of upward speed each
      // frame, and shrink as they fade.
      for (i = PROJECTILE_VOLLEY_FIRST_TRAIL;
           i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++) {
        struct Particle *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 4) + 1;

          ANIM_DRAW_MOTE(draw[0], canvas, sheet, Q_16_16_TO_INT(particle->x),
                         Q_16_16_TO_INT(particle->y), size);
          Particle_AdvanceWithGravity2D(particle, 60, -0x4000);
          particle->variant--;
        }
      }
      BattleCamera_Shake(4, 4);
      ANIM_END_FRAME(work);
    }
  }

  ANIM_FINISH_CANVAS_ONE_BLITTER();
}

/// Sends a battler back to its place (its anchor) with a hop.
///
/// Nothing calls it; BattleMotion_ReturnToSpot (battle/motion.c) sets the same
/// motion without the hop. Sets the battler's motion object to move at up to 8
/// pixels a frame, jumping at 4 pixels a frame under the usual gravity, and
/// selects POSE_STANDING.
UNUSED void BattleAnim_InitializeRisingObject(s32 id) {
  struct BattleObject *slot = GetBattleObjectSlotFar(id);
  struct Actor *actor = slot->actor;

  actor->acceleration = 0x20000;
  actor->speedLimit = 0x80000;
  actor->velocityY = 0x40000;
  actor->gravity = 0xab85;
  actor->bounce = 0;
  actor->steeringFlags = 0;
  actor->snapToTarget = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, slot->anchorX, 0, slot->anchorZ);
  Actor_SelectAnimationFar(actor, POSE_STANDING);
}

/// Moves a battler 80% of the way to another one over a number of frames,
/// in its leap pose. The speed is that distance over the frames; the
/// distance is taken in 1/256 pixels so its square fits in 32 bits.
///
/// Called by BattleAnim_RunRushingStrike. Sets the battler's acceleration,
/// speed limit and gravity, turns it to face its motion, starts it moving on
/// the ground towards that point and selects POSE_LEAP.
void BattleAnim_SetApproachMotion(s32 id, s32 targetId, s32 frames) {
  struct BattleObject *slot = GetBattleObjectSlotFar(id);
  struct BattleObject *targetSlot = GetBattleObjectSlotFar(targetId);
  struct Actor *actor = slot->actor;
  struct Actor *target = targetSlot->actor;
  s32 rate = 80;
  s32 diffX = target->x.fixed - actor->x.fixed;
  s32 startX = actor->x.fixed;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z.fixed - actor->z.fixed;
  s32 startZ = actor->z.fixed;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 stepX = dx >> 8;
  s32 stepZ = dz >> 8;
  s32 speed;

  speed = Iwram_Sqrt(stepX * stepX + stepZ * stepZ);
  speed = (speed << 8) / frames;
  actor->acceleration = speed;
  actor->speedLimit = speed;
  actor->snapToTarget = 1;
  actor->gravity = 0xab85;
  actor->velocityY = 0;
  actor->bounce = 0;
  actor->steeringFlags = 1;
  Actor_StartIdleFar(actor);
  Actor_SetMoveTargetFar(actor, x, 0, z);
  Actor_SelectAnimationFar(actor, POSE_LEAP);
}

#undef PROJECTILE_VOLLEY_ANIMATION_STEP_FAST
#undef PROJECTILE_VOLLEY_ANIMATION_STEP_NORMAL
#undef PROJECTILE_VOLLEY_PARTICLE_COUNT
#undef PROJECTILE_VOLLEY_COMET_CELLS
#undef PROJECTILE_VOLLEY_STREAK_CELLS
#undef PROJECTILE_VOLLEY_FLASH_TURNED
#undef PROJECTILE_VOLLEY_MEDIUM_SPARK_CELLS
#undef PROJECTILE_VOLLEY_LARGE_SPARK_CELLS
#undef PROJECTILE_VOLLEY_DJINNI_CELLS
#undef PROJECTILE_VOLLEY_FIRST_SPARK
#undef PROJECTILE_VOLLEY_FIRST_TRAIL
