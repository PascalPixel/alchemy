/// @file
/// Projectile volleys (Slash, Shuriken, Wing Flutter, Blizzard ...).
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

/// Djinni entrance: sparks swirl around a point that travels from the user
/// to the goal and gather into the Djinni, which sways for 16 frames and
/// bursts into motes. Leaves the canvas cleared for the effect that
/// follows.
///
/// The prelude of the stat boosts, the projectile volleys, the healing motes,
/// the Djinn beams, the burst scenes, Bind, Hail, Ground, Torch and Vine,
/// called with the work block they then use. Opens the canvas layer but does
/// not close it: it removes the transfer and palette fade callbacks, frees
/// both blitters, clears the canvas and its VRAM, and sets REG_BLDALPHA back
/// to 16/16. Copies the Djinni's palette over the first 128 bytes of BG
/// palette RAM, starts a 24-frame backdrop fade (work->fadeFrames), and uses
/// PARTICLES 0 to 63 for the sparks and work->particles for the motes.
///
/// effect: the user and the targets.
/// mode: the Djinni: 0 Venus, 1 Mercury, 2 Mars, 3 Jupiter; 4 to 7 the same
///       over 84 frames instead of 64.
/// side: 1 mirrors the pictures.
/// anchor: where the Djinni appears: 0 over the first target, 1 at the
///         first target's x at depth 0, 2 over the user, 3 at the user's x
///         at depth 0, 4 in the centre of the field.
/// outX, outY: receive the Djinni's last screen position.
void BattleFx_RunDjinniEntrance(struct BattleEffectArgument *effect, s32 mode,
                                s32 side, s32 anchor, s32 *outX, s32 *outY) {
  s32 center[3];
  s32 goal[3];
  s32 step[3];
  s32 point[3];
  struct EffectPosition position;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  s32 frameCount;
  struct MotionObject *target;
  struct MotionObject *user;
  u8 *palette;
  s32 sheetId;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  if (mode > 3) {
    mode -= 4;
    frameCount = 84;
  } else {
    frameCount = 64;
  }
  switch (mode) {
  case 0:
    sheetId = (s32)&ResourceId_VenusDjinnSmallSheet;
    break;
  case 1:
    sheetId = (s32)&ResourceId_MercuryDjinnSmallSheet;
    break;
  case 2:
    sheetId = (s32)&ResourceId_MarsDjinnSmallSheet;
    break;
  default:
    sheetId = (s32)&ResourceId_JupiterDjinnSmallSheet;
    break;
  }
  // The small Djinni's resource is its palette, then its picture.
  palette = Resource_GetTableEntry(sheetId);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Resource_DecodeType01(palette + 128, work->sheet);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (side == 1) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_MAX);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // Sparks spread out flat from the centre, 20 to 51 pixels up.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 angle = Random16() & 0xffff;
    s32 speed = (Random16() & 255) + 128;

    SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
    SET_VELOCITY(spark, (Trig_Sin(angle) * speed) >> 5, 0,
                 (Trig_Cos(angle) * speed) >> 5);
    spark->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  // The centre moves from the user to the goal, 60 pixels up, over frames
  // 6 to 45.
  SET_VECTOR(center, user->x, 0, user->z);
  switch (anchor) {
  case 0:
    SET_VECTOR(goal, target->x, 0x3c0000, target->z);
    break;
  case 1:
    SET_VECTOR(goal, target->x, 0x3c0000, 0);
    break;
  case 2:
    SET_VECTOR(goal, user->x, 0x3c0000, user->z);
    break;
  case 3:
    SET_VECTOR(goal, user->x, 0x3c0000, 0);
    break;
  case 4:
    SET_VECTOR(goal, 0, 0x3c0000, 0);
    break;
  }
  SET_VECTOR(step, (goal[0] - center[0]) / 40, (goal[1] - center[1]) / 40,
             (goal[2] - center[2]) / 40);
  for (frame = 0; frame != frameCount; frame++) {
    struct BattleCamera *camera = gCameraWork;

    // The long version fades the canvas out over its last 8 frames; the sound
    // plays on frame 8.
    if (frame > 75)
      REG_BLDALPHA = BLEND_ALPHA(168 - frame * 2, 16);
    if (frame == 8)
      AudioCommand_PlayFar(SE_212);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 6 && frame <= 45) {
      center[0] += step[0];
      center[1] += step[1];
      center[2] += step[2];
    }
    SceneTransform_ApplyPosition(center);
    // The user glows white from frame 0 to frame 24.
    if (frame == 0)
      ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1, -1,
                                0);
    if (frame == 24)
      ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
    // Only the first 32 sparks are drawn. Spark i appears on frame i / 8 and
    // spins about the centre on one of four axes, drawn larger the nearer it
    // is; from 24 frames after appearing it is pulled in, and goes out once
    // it nears the centre.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame >= i / 8 && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          SceneTransform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint(&spark->x, &position);
        position.x >>= 1;
        Transform_Pop();
        CLAMP(position.depth, 250, 634);
        size = 8 - (position.depth - 250) / 64;
        EFFECT_DRAW_MOTE(draw[1], canvas, sheet, position.x, position.y, size);
        EffectStep_AdvanceWithGravity3D(spark, 60, 0);
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
      EffectPosition_ProjectPoint(point, &position);
      *outX = position.x;
      *outY = position.y;
      position.x >>= 1;
      draw[0](canvas, work->sheet, position.x - 10, position.y - 20, 20, 40);
    }
    // On frame 64 (long version only), 64 motes burst from the Djinni, each
    // lasting 9 to 24 frames; its x is a screen position, and the canvas is
    // half as wide.
    if (frame == 64) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];
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
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, Q_16_16_TO_INT(mote->x),
                           Q_16_16_TO_INT(mote->y), size);
          EffectStep_AdvanceWithGravity2D(mote, 60, 0);
          mote->variant--;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  // Leave the canvas cleared and fully shown for the effect that follows,
  // without closing the layer.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
}

// Projectile volleys and rushing strikes.
// The folder README describes them under "Effect families".

#define PROJECTILE_VOLLEY_ANIMATION_STEP_FAST 0x30 ///< the user's, while firing
/// The user's animation step after the volley: the usual speed.
#define PROJECTILE_VOLLEY_ANIMATION_STEP_NORMAL 0x10

/// Six comets fly at each target, three frames apart, and burst into a flash
/// and sparks; the effect's variant picks one of three comets.
///
/// Abilities: Slash, Wind Slash, Sonic Slash (kind 19).
void BattleFx_RunSlash(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 0);
}
/// Four spinning shurikens fly at each target, five frames apart, and vanish
/// once past it.
///
/// Abilities: Shuriken (kind 54).
void BattleFx_RunShuriken(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 1);
}
/// Ten pellets fly slowly at each target and bounce off it, up or down at
/// random.
///
/// Abilities: Crazy Voice (kind 303).
void BattleFx_RunCrazyVoice(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 2);
}
/// Five yellow sparks fly level at each target, five frames apart.
///
/// Abilities: Wing Flutter (kind 307).
void BattleFx_RunWingFlutter(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 3);
}
/// Ten large yellow sparks stream level at each target, one a frame, spread up
/// and down.
///
/// Abilities: Wing Stroke (kind 381).
void BattleFx_RunWingStroke(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 11);
}
/// Ten small yellow sparks stream level at each target, one a frame.
///
/// Abilities: Wing Beat (kind 306).
void BattleFx_RunWingBeat(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 4);
}
/// Puts the user into its action pose (2) and fires the volley in this mode
/// while its animation runs fast.
///
/// Called by Cutting Edge, Heat Wave and Blizzard. Leaves the user in its
/// action pose, its animation back at the usual speed.
static inline void
BattleFx_RunVolleyWithAction(struct BattleEffectArgument *effect, s32 mode) {
  struct DispatchObject *user;

  user = (struct DispatchObject *)GetBattleObjectSlotFar(effect->actor)->object;
  ObjectDispatch_SelectAnimationFar(user, 2);
  ObjectDispatch_SetAnimationSpeedFar(user,
                                      PROJECTILE_VOLLEY_ANIMATION_STEP_FAST);
  BattleFx_RunProjectileVolley(effect, mode);
  ObjectDispatch_SetAnimationSpeedFar(user,
                                      PROJECTILE_VOLLEY_ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, one large comet in ice colours flies at each
/// target and bursts into 80 sparks.
///
/// Abilities: Cutting Edge (kind 57).
void BattleFx_RunCuttingEdge(struct BattleEffectArgument *effect) {
  BattleFx_RunVolleyWithAction(effect, 5);
}
/// From the user's action pose, seven fireballs trailing embers fly at each
/// target, two frames apart, each bursting into 40 sparks.
///
/// Abilities: Heat Wave (kind 59).
void BattleFx_RunHeatWave(struct BattleEffectArgument *effect) {
  BattleFx_RunVolleyWithAction(effect, 6);
}
/// Thirty ice chips fall from high above the user onto each target, without
/// shaking the screen or knocking the target back; the battle shows the damage
/// on frame 48.
///
/// Abilities: Sleet (kind 205).
void BattleFx_RunSleet(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 7);
}
/// Sixty ice chips stream at each target, one a frame, without knocking it
/// back.
///
/// Abilities: Ice Blessing (kind 402).
void BattleFx_RunIceBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunProjectileVolley(effect, 10);
}
/// From the user's action pose, sixty ice chips stream at each target, one a
/// frame, scattered up and down, without knocking it back.
///
/// Abilities: Blizzard (kind 63).
void BattleFx_RunBlizzard(struct BattleEffectArgument *effect) {
  BattleFx_RunVolleyWithAction(effect, 9);
}

/// PARTICLES holds the hit flashes from 0, the sparks from 100 and the
/// fireball trails from 512.
#define PROJECTILE_VOLLEY_PARTICLE_COUNT 1024

/// The bytes of a gProjectiles_Modes row.
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
#define PROJECTILE_VOLLEY_DJINNI_CELLS 0x65c0 ///< the small Mercury djinni
#define PROJECTILE_VOLLEY_FIRST_SPARK 100     ///< the first spark in PARTICLES
#define PROJECTILE_VOLLEY_FIRST_TRAIL 512     ///< the first trail in PARTICLES

/// Projectile volley: shots fly from the user to each target and burst
/// into a flash and sparks when they pass it. The hit sound plays at most
/// once every 8 frames.
///
/// Called by the eleven wrappers above, and by casting impacts 15 (mode 9)
/// and 26 (mode 8, which no wrapper uses) once their prelude ends. With more
/// than 63 shots in all it fires at the first target only, writing
/// work->effect->count. Writes REG_BLDCNT in mode 1 and the first 128 bytes
/// of BG palette RAM in modes 2 and 5; sets work->shakeFrames on each hit
/// but in mode 7. Ends with the transfer callback removed, the blitter freed
/// and the canvas layer closed; the second blitter is built and freed for
/// each picture that needs it.
///
/// effect: the user and the targets. variant picks the comet in mode 0
///         (mode 5 always uses comet 2, mode 8 comet 0); with prelude 1 a
///         Mercury Djinni enters first.
/// mode: the shot and its flight (0 to 11): 0, 5 and 8 comets (5 in the
///       ice palette), 1 shurikens, 2 bouncing pellets, 3, 4 and 11
///       yellow sparks, 6 fireballs with trails, 7, 9 and 10 ice chips.
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct EffectPosition position;
  s32 screenX;
  s32 screenY;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 soundCooldown;
  s32 total;
  s32 kind;
  s32 shots;
  s32 steps;
  struct MotionObject *user;
  s32 k;
  // The trail particles placed, and later the sparks' speed shift: one
  // local for both, as separate locals change the register allocation.
  s32 trailsOrShift;
  s32 hit;
  s32 x;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  // The camera's slot, read back from the effect's.
  camera = *(struct BattleCamera **)((u8 *)slots - (HEAP_SLOT_BATTLE_EFFECT -
                                                    HEAP_SLOT_CAMERA) *
                                                       sizeof(void *));
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  soundCooldown = 0;
  work->effect = effect;
  kind = effect->variant;
  // The canvas's BG priority: 0 for Ice Blessing, 1 for the others. A
  // Djinni prelude then opens the layer again at priority 0.
  if (mode == 10)
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
  if (work->effect->prelude == 1)
    BattleFx_RunDjinniEntrance(effect, 1, work->effect->side, 2, &screenX,
                               &screenY);

  // The main blitter, mirrored on side 1: brightest-wins for Cutting Edge,
  // additive for the others.
  if (mode == 5) {
    if (work->effect->side == 1)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  } else {
    if (work->effect->side == 1)
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
    else
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
  }
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];

  // The shot pictures, and how long each mode's trails last: comets keep
  // three quarters of each level a frame, shurikens none, the rest half.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == 0 || mode == 5 || mode == 8) {
    if (mode == 5)
      kind = 2;
    if (mode == 8)
      kind = 0;
    if (kind == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_CELLS, 1,
                                 1);
    else if (kind == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_CELLS, 1,
                                 1);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC,
                                 work->sheet + PROJECTILE_VOLLEY_COMET_CELLS, 1,
                                 1);
    // The ice palette is copied twice, before and after the streaks load.
    if (mode == 5) {
      const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      Iwram_CopyWords((void *)BG_PLTT, pal, 128);
    }
    Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet,
                               work->sheet + PROJECTILE_VOLLEY_STREAK_CELLS, 1,
                               0);
    if (mode == 5) {
      const void *pal = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      Iwram_CopyWords((void *)BG_PLTT, pal, 128);
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  } else if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work->sheet, 1,
                               1);
    REG_BLDCNT = 0;
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == 2) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage,
                               work->sheet, 0, 0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else if (mode == 3 || mode == 4 || mode == 11) {
    Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work->sheet,
                               1, 1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else if (mode == 6) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work->sheet, 1,
                               1);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    // The ice chips, and the small Mercury Djinni the prelude draws.
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               work->sheet + PROJECTILE_VOLLEY_DJINNI_CELLS, 1,
                               0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  // Where the shots start and how fast they fly.
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  shots = (gProjectiles_Modes[mode][PROJECTILE_VOLLEY_SHOTS]);
  steps = (gProjectiles_Modes[mode][PROJECTILE_VOLLEY_STEPS]);
  {
    s32 stagger = (gProjectiles_Modes[mode][PROJECTILE_VOLLEY_STAGGER]);
    s32 delay = (gProjectiles_Modes[mode][PROJECTILE_VOLLEY_DELAY]);
    s32 height = BattleUnit_GetEffectHeightFar(work->effect->actor);

    // Keep the shots within work->particles' 64.
    if (work->effect->count * shots > 63)
      work->effect->count = 1;

    // Each shot starts at the user, at a height its mode picks, and is aimed
    // to reach its target in `steps` frames: straight across for sparks and
    // Cutting Edge, at a random height near the target for the others.

    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[i])->object;
      s32 targetHeight = BattleUnit_GetEffectHeightFar(work->effect->actors[i]);

      for (j = 0; j != shots; j++) {
        struct EffectStep *shot = &work->particles[i * shots + j];

        shot->x = user->x;
        if (mode == 7)
          shot->y = user->y + ((Random16() & 15) << 16) + 0x3a0000;
        else if (mode == 10)
          shot->y = user->y + height / 2;
        else if (mode == 6)
          shot->y = user->y + height / 2;
        else if (mode == 9)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if ((u32)(mode - 3) <= 1)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == 11)
          shot->y = user->y + height / 2 + ((32 - (Random16() & 63)) << 16);
        else if (mode == 5)
          shot->y = user->y + height / 2;
        else
          shot->y = user->y + height;
        shot->z = user->z;
        shot->velocityX = (target->x - shot->x) / steps;
        if (mode == 7)
          shot->velocityY =
              (s32)(target->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) /
              steps;
        else if (mode == 8)
          shot->velocityY =
              (s32)(target->y + ((Random16() & 7) << 16) - shot->y + 0x160000) /
              steps;
        else if (mode == 9)
          shot->velocityY =
              (s32)(target->y + ((64 - (Random16() & 63)) << 16) - shot->y) /
              steps;
        else if (mode == 10)
          shot->velocityY =
              (s32)(target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) /
              steps;
        else if ((u32)(mode - 3) <= 1 || mode == 11 || mode == 5)
          shot->velocityY = 0;
        else if (mode == 6)
          shot->velocityY = (s32)(target->y + targetHeight / 2 +
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        else
          shot->velocityY = (s32)(target->y + targetHeight -
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        shot->velocityZ = (target->z - shot->z) / steps;
        shot->variant = 0;
      }
    }
    // Free every flash, spark and trail.
    for (i = 0; i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++)
      PARTICLES[i].variant = 0;
    // Every shot's flight, then 16 frames (32 for fireballs) for the last
    // hits to fade.
    if (mode == 6)
      total = work->effect->count * delay + shots * stagger + 32;
    else
      total = work->effect->count * delay + shots * stagger + 16;

    // Each frame: the prelude's Djinni, the shots and their hits, the hit
    // flashes, the sparks and the trails, in that order.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      // The battle shows the damage on frame 16, 32 for Heat Wave and 48 for
      // Sleet; Cutting Edge shows it on its first hit instead.
      if (soundCooldown > 0)
        soundCooldown--;
      if (mode == 6) {
        if (frame == 4)
          AudioCommand_PlayFar(SE_136);
        if (frame == 32)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      } else if (mode == 7) {
        if (frame == 48)
          EFFECT_SHOW_DAMAGE(SE_133);
      } else if (mode != 5) {
        if (frame == 16)
          EFFECT_SHOW_DAMAGE(SE_133);
      }

      // Prelude: the small Djinni circles, and from frame 70 rises away; it
      // is drawn twice on the first four frames. Its blitter is built and
      // freed each frame.
      if (work->effect->prelude == 1) {
        s32 x = ((-Trig_Sin(frame << 11) * 4) >> 16) + screenX / 2 - 10;
        s32 y = ((Trig_Cos(frame << 11) * 2) >> 16) + screenY - 24;

        if (frame > 69)
          y = y - frame * 2 + 138;
        if (work->effect->side == 1)
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_ADDITIVE);
        else
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_ADDITIVE);
        draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        if (frame <= 3)
          draw[1](canvas, work->sheet + PROJECTILE_VOLLEY_DJINNI_CELLS, x, y,
                  20, 40);
        draw[1](canvas, work->sheet + PROJECTILE_VOLLEY_DJINNI_CELLS, x, y, 20,
                40);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }

      // The shots: shot j at target i launches after frame
      // j * stagger + i * delay. index counts every shot, so each has its own
      // hit flash in PARTICLES.
      EFFECT_VIEW_FROM_CAMERA(camera);
      index = 0;
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(work->effect->actors[i])->object;

        for (j = 0; j != shots; j++, index++) {
          if (j * stagger + i * delay < frame) {
            struct EffectStep *shot = &work->particles[i * shots + j];
            struct EffectPosition *screenPosition = &position;

            // The screen position (half across, on the canvas).
            EffectPosition_ProjectPoint(shot, screenPosition);
            screenPosition->x >>= 1;
            shot->x += shot->velocityX;
            shot->y += shot->velocityY;
            shot->z += shot->velocityZ;
            if (mode == 6) {
              // Leave two trail particles behind the fireball.
              trailsOrShift = 0;
              for (k = PROJECTILE_VOLLEY_FIRST_TRAIL;
                   k != PROJECTILE_VOLLEY_PARTICLE_COUNT; k++) {
                struct EffectStep *particle = &PARTICLES[k];

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
              if (target->x < 0) {
                hit = x < 0;
                /* FAKEMATCH: cancelling updates under if (target) end CSE
                 * before the hit test and emit nothing; without them, 77
                 * instructions differ in tbs-en. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              } else {
                hit = x >= 0;
                /* FAKEMATCH: the same CSE block end keeps the opposite sign
                 * result before its side check; without this second pair, 14
                 * instructions differ in tbs-en. */
                if (target) {
                  hit ^= x;
                  hit ^= x;
                }
                if (!hit)
                  goto drawShot;
              }
              if (ABS(x) >= ABS(target->x)) {
                s32 spawned = 0;
                struct EffectStep *flash = &PARTICLES[index];

                shot->variant = 1;
                if (mode == 5) {
                  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
                } else if (mode != 6 && soundCooldown == 0) {
                  soundCooldown = 8;
                  AudioCommand_PlayFar(SE_132);
                }
                // The hit: the shot's flash starts where it was drawn, the
                // screen shakes (but for Sleet), the target flashes and,
                // but for the ice chips, recoils.
                if (mode == 2) {
                  // Pellets bounce back, up or down at random.
                  s32 kick = ((Random16() & 31) + 32) << 12;

                  if (Random16() & 1)
                    shot->velocityY += kick;
                  else
                    shot->velocityY -= kick;
                  shot->velocityX = -shot->velocityX;
                }
                flash->variant = 1;
                SET_POSITION(flash, screenPosition->x, screenPosition->y, 0);
                if (mode != 7)
                  work->shakeFrames = 2;
                ObjectGroup_UpdateMembers(work->effect->actors[i],
                                          SPRITE_COLOR_WHITE, POSE_DAMAGE, i,
                                          8);
                if (mode != 7 && mode != 9 && mode != 10) {
                  if (mode == 5)
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i],
                                                       4);
                  else
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i],
                                                       5);
                }
                // Sparks: the mode's count, in the first free slots from 100,
                // living 16 to 23 frames; Cutting Edge's fly twice as fast.
                trailsOrShift = mode == 5 ? 12 : 11;
                for (k = PROJECTILE_VOLLEY_FIRST_SPARK;
                     k != PROJECTILE_VOLLEY_FIRST_TRAIL; k++) {
                  struct EffectStep *particle = &PARTICLES[k];

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
                        (gProjectiles_Modes[mode][PROJECTILE_VOLLEY_SPARKS]))
                      break;
                  }
                }
              }
            }
          drawShot:
            // Draw the shot, hit or not.
            if (mode == 0 || mode == 5 || mode == 8) {
              // The picture holds the top half; the bottom half is drawn
              // flipped.
              if (work->effect->side == 1)
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                                    COMPOSITE_MAX);
              else
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + PROJECTILE_VOLLEY_COMET_CELLS,
                  screenPosition->x - 16,
                  screenPosition->y - gProjectiles_CometHeights[kind], 32,
                  gProjectiles_CometHeights[kind]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
              if (work->effect->side == 1)
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                    COMPOSITE_MAX);
              else
                EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                    EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                                    COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + PROJECTILE_VOLLEY_COMET_CELLS,
                  screenPosition->x - 16, screenPosition->y, 32,
                  gProjectiles_CometHeights[kind]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            } else if (mode == 1) {
              // Shurikens vanish once past their target.
              if (ABS(shot->x) <= ABS(target->x))
                draw[0](canvas, work->sheet + frame % 6 * (32 * 24),
                        screenPosition->x - 16, screenPosition->y - 12, 32, 24);
            } else if (mode == 7 || mode == 9 || mode == 10) {
              s32 cell = j & 3;
              draw[0](canvas, work->sheet + gProjectiles_PelletOffsets[cell],
                      screenPosition->x - 4, screenPosition->y - 4, 8, 8);
            } else if (mode == 2) {
              draw[0](canvas, work->sheet + j % 6 * (8 * 16),
                      screenPosition->x - 4, screenPosition->y - 8, 8, 16);
            } else if (mode == 3) {
              draw[0](canvas,
                      work->sheet + PROJECTILE_VOLLEY_MEDIUM_SPARK_CELLS,
                      screenPosition->x - 9, screenPosition->y - 7, 18, 13);
            } else if (mode == 4) {
              draw[0](canvas, work->sheet, screenPosition->x - 6,
                      screenPosition->y - 4, 12, 8);
            } else if (mode == 11) {
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
      if (mode == 0 || mode == 5 || mode == 8) {
        for (i = 0; i != work->effect->count * shots; i++) {
          struct EffectStep *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + gProjectiles_FlashOffsets[step],
                  flash->x - gProjectiles_FlashWidths[step],
                  flash->y - gProjectiles_FlashRises[step],
                  gProjectiles_FlashWidths[step],
                  gProjectiles_FlashHeights[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas, work->sheet + gProjectiles_FlashOffsets[step],
                  flash->x,
                  flash->y + gProjectiles_FlashSteps[step] -
                      gProjectiles_FlashRises[step],
                  gProjectiles_FlashWidths[step],
                  gProjectiles_FlashHeights[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            } else {
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas,
                  work->sheet + gProjectiles_FlashOffsets[step] +
                      PROJECTILE_VOLLEY_FLASH_TURNED,
                  flash->x - gProjectiles_FlashRises[step],
                  flash->y - gProjectiles_FlashWidths[step],
                  gProjectiles_FlashHeights[step],
                  gProjectiles_FlashWidths[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                                  EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                                  COMPOSITE_MAX);
              ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE])(
                  canvas,
                  work->sheet + gProjectiles_FlashOffsets[step] +
                      PROJECTILE_VOLLEY_FLASH_TURNED,
                  flash->x + gProjectiles_FlashSteps[step] -
                      gProjectiles_FlashRises[step],
                  flash->y, gProjectiles_FlashHeights[step],
                  gProjectiles_FlashWidths[step]);
              Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce off the bottom (y 112) at half speed,
      // shrinking as their frames run out.
      for (i = PROJECTILE_VOLLEY_FIRST_SPARK;
           i != PROJECTILE_VOLLEY_FIRST_TRAIL; i++) {
        struct EffectStep *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 3) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, Q_16_16_TO_INT(particle->x),
                           Q_16_16_TO_INT(particle->y), size);
          EffectStep_AdvanceWithGravity2D(particle, 60, 0x1000);
          if (particle->y > 0x700000)
            particle->velocityY = -particle->velocityY / 2;
          particle->variant--;
        }
      }

      // Fireball trails rise and shrink as they fade.
      for (i = PROJECTILE_VOLLEY_FIRST_TRAIL;
           i != PROJECTILE_VOLLEY_PARTICLE_COUNT; i++) {
        struct EffectStep *particle = &PARTICLES[i];

        if (particle->variant > 0) {
          s32 size = (particle->variant >> 4) + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, Q_16_16_TO_INT(particle->x),
                           Q_16_16_TO_INT(particle->y), size);
          EffectStep_AdvanceWithGravity2D(particle, 60, -0x4000);
          particle->variant--;
        }
      }
      Camera_ApplyShake(4, 4);
      EFFECT_END_FRAME(work);
    }
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Sends a battler back to its place (its anchor) with a hop.
///
/// Nothing calls it; ObjMotRst (battle/motion.c) sets the same motion without
/// the hop. Sets the battler's motion object to move at up to 8 pixels a frame,
/// jumping at 4 pixels a frame under the usual gravity, and selects its
/// animation 1.
UNUSED void BattleFx_InitializeRisingObject(s32 id) {
  struct BattleObj *slot = GetBattleObjectSlotFar(id);
  struct MotionObject *object = slot->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x40000;
  object->verticalMotionStrength = 0xab85;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  object->snapToTarget = 1;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  Object_SetMoveTargetFar((struct ObjectRuntime *)object, slot->anchorX, 0,
                          slot->anchorZ);
  ObjectDispatch_SelectAnimationFar(object, 1);
}

/// Moves a battler 80% of the way to another one over a number of frames,
/// in its action pose. The speed is that distance over the frames; the
/// distance is taken in 1/256 pixels so its square fits in 32 bits.
///
/// Called by BattleFx_RunRushingStrike. Sets the battler's acceleration,
/// speed limit and gravity, turns it to face its motion, starts it moving on
/// the ground towards that point and selects its animation 2.
void BattleFx_SetApproachMotion(s32 id, s32 targetId, s32 frames) {
  struct BattleObj *slot = GetBattleObjectSlotFar(id);
  struct BattleObj *targetSlot = GetBattleObjectSlotFar(targetId);
  struct MotionObject *object = slot->object;
  struct MotionObject *target = targetSlot->object;
  s32 rate = 80;
  s32 diffX = target->x - object->x;
  s32 startX = object->x;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z - object->z;
  s32 startZ = object->z;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 stepX = dx >> 8;
  s32 stepZ = dz >> 8;
  s32 speed;

  speed = Iwram_Sqrt(stepX * stepX + stepZ * stepZ);
  speed = (speed << 8) / frames;
  object->acceleration = speed;
  object->speedLimit = speed;
  object->snapToTarget = 1;
  object->verticalMotionStrength = 0xab85;
  object->velocityY = 0;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 1;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)object);
  Object_SetMoveTargetFar((struct ObjectRuntime *)object, x, 0, z);
  ObjectDispatch_SelectAnimationFar(object, 2);
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
