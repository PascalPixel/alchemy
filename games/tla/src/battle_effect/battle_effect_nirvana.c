/// @file
/// Nirvana, Stone Justice, Soul Shatter and Serac, and the entries of Heavy
/// Divide and Strong Hit.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define NIRVANA_FRAMES 80
/// The orb is drawn as two rings of NIRVANA_SPARKS / 2 sparks, a quarter turn
/// apart, turning about it; the rings open to NIRVANA_RING_RADIUS pixels and
/// close again over the effect.
#define NIRVANA_SPARKS 24
#define NIRVANA_RING_RADIUS 64
/// What the orb's speed gains toward the user each frame: 1/1024 of the
/// distance between them.
#define NIRVANA_PULL(distance) ((distance) >> 10)

/// The Meditation Rod's unleash: an orb leaves the first target, drawn as rings
/// of sparks turning about it, and is pulled back into the user, who flashes
/// white.
///
/// Abilities: Nirvana.
void BattleEffect_RunNirvana(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle blit;
  struct BattleCamera *view;
  u8 *sheet;
  struct MotionObject *object;
  struct Particle *orb;
  s32 half;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  view = Ram_HeapSlots->cameraState;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_PINK_BURST_SHEET);
  object = BattleObject_GetFar(args->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
  orb = state->particles;
  SET_POSITION(orb, object->x, object->y + half, object->z);
  orb->velocityX = 0;
  orb->velocityY = 0;
  orb->velocityZ = 0;
  orb->variant = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit = Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != NIRVANA_FRAMES; frame++) {
    struct MotionObject *user = BattleObject_GetFar(args->user)->object;
    s32 userHalf = BattleUnit_GetEffectHeightFar(args->user) / 2;

    if (frame == 64)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    Matrix_Initialize();
    Matrix_SetLookAt(view->eye, view->focus);
    if (frame == 72)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 79)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 0)
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 42);
    if (orb->variant == 0) {
      s32 size = 6;

      for (i = 0; i != NIRVANA_SPARKS; i++) {
        s32 point[3];
        struct ScreenPoint out;

        Matrix_Push();
        Matrix_Translate(orb->x, orb->y, orb->z);
        if (i < NIRVANA_SPARKS / 2)
          Matrix_RotateX(frame * (TRIG_TURN / 128));
        else
          Matrix_RotateX(frame * (TRIG_TURN / 128) + DEGtoANG(90));
        Matrix_RotateZ(i * (TRIG_TURN / 12) + frame * (TRIG_TURN / 64));
        Matrix_Translate(
            0, Trig_Sin(frame * (TRIG_TURN / 160)) * NIRVANA_RING_RADIUS, 0);
        point[0] = 0;
        point[1] = 0;
        point[2] = 0;
        EffectPosition_ProjectPoint(point, &out);
        out.x >>= 1;
        Matrix_Pop();
        blit(canvas, sheet + gBattleEffect_MotePictures[size - 1],
             out.x - size / 2, out.y - size, size, size * 2);
      }
      Particle_AdvanceWithGravity3D(orb, 62, 0);
      if (frame > 0) {
        s32 dx = NIRVANA_PULL(user->x - orb->x);
        s32 dy = NIRVANA_PULL(user->y + userHalf - orb->y);
        s32 dz = NIRVANA_PULL(user->z - orb->z);
        s32 ox;
        s32 ux;

        orb->velocityX += dx;
        orb->velocityY += dy;
        orb->velocityZ += dz;
        ux = user->x;
        if (ux < 0)
          ux = -ux;
        ox = orb->x;
        if (ox < 0)
          ox = -ox;
        // Gone once it has passed the user.
        if (ux < ox)
          orb->variant = -1;
      }
    }
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define STONE_JUSTICE_ROCKS 21
#define STONE_JUSTICE_FRAMES 107
/// The frame the rocks are hurled at the target and the battle background
/// starts to slide after them.
#define STONE_JUSTICE_HURL_FRAME 64
/// A rock shows the next picture of its tumble each time its spin passes
/// this much.
#define STONE_JUSTICE_SPIN_STEP 96

/// Themis' Axe's unleash: rocks rise in a column beside the user and spin as
/// they fly; then they are hurled across the screen into the target
/// while the battle background slides after them, and every rock that reaches
/// the target shakes the screen and knocks it back.
///
/// Abilities: Stone Justice.
void BattleEffect_RunStoneJustice(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle blit;
  FIXED scroll;
  s32 scrollSpeed;
  s32 cueTimer;
  struct MotionObject *user;
  s32 scrollX;
  struct ScreenPoint userPosition;
  struct ScreenPoint targetPosition;
  struct Particle *rock;
  s32 frame;
  s32 hit;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_BOULDER_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  blit = Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  scroll = 0;
  scrollSpeed = 0;
  cueTimer = 0;
  user = BattleObject_GetFar(args->user)->object;
  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL * 3);

  for (frame = 0; frame != STONE_JUSTICE_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(args->user, &userPosition);
    EffectPosition_ProjectScaledBattler(args->targets[0], &targetPosition);
    if (frame == 0)
      Sound_PlayFar(SE_GATE_MOVE);
    if (frame == STONE_JUSTICE_HURL_FRAME)
      Sound_PlayFar(SE_LAUNCH);

    // Frame 0: the rocks stand in a column 2 pixels apart from canvas row 96
    // down, on the user's side of the canvas, and are thrown upward at random
    // speeds.
    if (frame == 0) {
      for (i = 0; i != STONE_JUSTICE_ROCKS; i++) {
        rock = &state->particles[i];
        if (args->side == EFFECT_SIDE_ENEMIES)
          rock->x = INTtoFIXED(Random16() % 40 + 8);
        else
          rock->x = INTtoFIXED(120 - Random16() % 40);
        Random16(); // a number drawn and not used
        rock->y = INTtoFIXED(i * 2 + 96);
        rock->velocityX = 0;
        rock->velocityY = -((Random16() & 127) + 32) << 11;
        rock->z = (Random16() & 15) + 8;
        rock->velocityZ = 0;
        rock->variant = 0;
      }
    }
    if (frame == 52)
      Actor_SelectAnimationFar(user, POSE_LEAP);

    // The background follows the rocks, ever faster.
    if (frame > STONE_JUSTICE_HURL_FRAME) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        scroll += scrollSpeed;
      else
        scroll -= scrollSpeed;
      scrollSpeed += toFIXED(1) / 2;
      scrollSpeed = scrollSpeed * 60 / DAMPING_SCALE;
    }
    scrollX = FIXEDtoINT(scroll);
    REG_BG2X = (64 - (userPosition.x - scrollX)) << 8;

    // Each rock tumbles through three pictures of one of Spire's shards (z is
    // its spin rate, velocityZ the angle) and hits the target once it passes
    // it.
    hit = 0;
    for (i = 0; i != STONE_JUSTICE_ROCKS; i++) {
      s32 cell;
      u32 width;
      s32 left;
      s32 top;

      rock = &state->particles[i];
      cell = (i % 5) * 3 + rock->velocityZ / STONE_JUSTICE_SPIN_STEP % 3;
      width = gSpire_ShardWidths[cell];
      left = FIXED_INTEGER_PART(rock->x) - width / 2 + scrollX;
      top = FIXED_INTEGER_PART(rock->y) - gSpire_ShardHeights[cell] / 2;
      blit(canvas,
           state->sheet + gSpire_ShardPictures[cell] + BOULDER_SHARD_PICTURES,
           left, top, width, gSpire_ShardHeights[cell]);
      if (rock->variant == 0) {
        if (args->side == EFFECT_SIDE_ENEMIES) {
          if (targetPosition.x >
              FIXED_INTEGER_PART(rock->x) + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        } else {
          if (targetPosition.x <
              FIXED_INTEGER_PART(rock->x) + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        }
      }
      Particle_AdvanceWithGravity2D(rock, 60, 0);
      rock->velocityZ += rock->z;
      if (frame > i / 2 + STONE_JUSTICE_HURL_FRAME) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          rock->velocityX -= toFIXED(1) / 2;
        else
          rock->velocityX += toFIXED(1) / 2;
        if (rock->z > 1)
          rock->z--;
      }
    }

    // A hit flashes and pushes the target; the impact sound plays at most
    // every eight frames.
    if (cueTimer > 0)
      cueTimer--;
    if (hit == 1) {
      if (cueTimer == 0) {
        cueTimer = 8;
        Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2), toFIXED(4),
                                    toFIXED(2), 150);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 4);
      state->shakeFrames = 2;
    }

    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(state);
  }

  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SOUL_SHATTER_FRAMES 106
/// The column, LimeColumnSheet's half of it at the start of the effect's
/// pictures, drawn twice side by side, the right half mirrored; BlastSheet's
/// spark pictures follow it.
#define SOUL_SHATTER_COLUMN_WIDTH 14
#define SOUL_SHATTER_COLUMN_HEIGHT 80
#define SOUL_SHATTER_SPARK_PICTURES PICTURE_BYTES(SOUL_SHATTER_COLUMN)
/// The sparks, each drawn for SOUL_SHATTER_SPARK_FRAMES, and the motes of the
/// target's soul, after twice as many particles.
#define SOUL_SHATTER_SPARKS 16
#define SOUL_SHATTER_SPARK_FRAMES 29
#define SOUL_SHATTER_MOTES 32

/// The Soul Brand's unleash: a column bursts up under the target in a spray
/// of sparks and throws it into the air; the user glows, and from frame 32
/// motes of the target's soul fly out and are drawn back into the user, who
/// flashes white. The column rises from frame 8, stands at its full height
/// and sinks from frame 80, as the target comes down.
///
/// Abilities: Soul Shatter.
void BattleEffect_RunSoulShatter(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  u8 *sheet;
  struct MotionObject *target;
  DrawRectangle draw[2];
  struct ScreenPoint position;
  struct ScreenPoint screen;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 12);
  Resource_LoadAndDecompress(RESOURCE_LIME_COLUMN_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET,
                             state->sheet + SOUL_SHATTER_SPARK_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  target = BattleObject_GetFar(args->targets[0])->object;
  EffectPosition_ProjectBattler(args->targets[0], &position);
  position.x /= 2;
  state->shakeFrames = 8;

  for (frame = 0; frame != SOUL_SHATTER_FRAMES; frame++) {
    // Frame 0: sparks leave the column's foot upward, and the motes start
    // at the target, 52 pixels up, in every direction.
    if (frame == 0) {
      target->y = 0;
      {
        struct Particle *spark;

        for (i = 0, spark = state->particles; i != SOUL_SHATTER_SPARKS;
             i++, spark++) {
          s32 speed = (Random16() & 0xff) + 256;
          s32 angle = (Random16() & 0x7fff) + DEGtoANG(90);

          spark->x = INTtoFIXED(position.x);
          spark->y = INTtoFIXED(96 - (Random16() & 31));
          spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
          spark->variant = Random16() & 7;
        }
      }
      {
        struct Particle *mote;

        for (i = 0, mote = &state->particles[SOUL_SHATTER_SPARKS * 2];
             i != SOUL_SHATTER_MOTES; i++, mote++) {
          s32 speed = (Random16() & 127) + 128;
          s32 angle = Random16() & 0x7fff;

          SET_POSITION(mote, target->x, toFIXED(52), target->z);
          mote->velocityY = ((Random16() & 0xff) - 128) << 9;
          mote->velocityX = (Trig_Sin(angle) * speed) >> 7;
          mote->velocityZ = (Trig_Cos(angle) * speed) >> 7;
          mote->variant = 0;
        }
      }
    }

    // The cues: the burst, the target thrown up 32 pixels, the user raising
    // a hand, the drain and the target's fall.
    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 10) {
      state->shakeFrames = 8;
      Sound_PlayFar(SE_CLATTER);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 80);
      target->verticalMotionStrength = 0;
      target->y = toFIXED(32);
    }
    if (frame == 80)
      target->verticalMotionStrength = BATTLE_UNIT_GRAVITY;
    if (frame == 24)
      BattleEffect_TintBattler(args->user, TINT_KEEP, POSE_CAST, TINT_KEEP, 0);
    if (frame == 40)
      Sound_PlayFar(SE_SWIRL);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 64)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 88)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);

    // The column: its two halves, the right one mirrored.
    if (frame > 7) {
      s32 centre = position.x;
      s32 height;

      if (frame <= 79)
        height = frame * 12 - 96;
      else
        height = 560 - frame * 6;
      if (height > 0) {
        u32 mode;
        s32 x;

        if (height > SOUL_SHATTER_COLUMN_HEIGHT) {
          height = SOUL_SHATTER_COLUMN_HEIGHT;
          mode = BLITTER_MAX;
        } else {
          mode = BLITTER_ADDITIVE;
        }
        for (i = 0, x = centre - SOUL_SHATTER_COLUMN_WIDTH; i != 2;
             i++, x += SOUL_SHATTER_COLUMN_WIDTH) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, mode);
          else
            EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                                EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
          draw[0] = Ram_HeapSlots->ramCode;
          draw[0](canvas, state->sheet, x, 104 - height,
                  SOUL_SHATTER_COLUMN_WIDTH, height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }

    // The sparks: from frame 8, two more each frame, through seven
    // pictures three frames apiece, the last held; one in four is drawn
    // mirrored.
    BattleEffect_BuildCanvasBlitters(args->side, draw);
    {
      struct Particle *spark;

      for (i = 0, spark = state->particles; i != SOUL_SHATTER_SPARKS;
           i++, spark++) {
        if (frame >= i / 2 + 8 && spark->variant < SOUL_SHATTER_SPARK_FRAMES) {
          s32 cell = spark->variant / 3;
          s32 x = FIXED_INTEGER_PART(spark->x);
          s32 y = FIXED_INTEGER_PART(spark->y);

          if (cell > 6)
            cell = 6;
          draw[(i & 3) ? 0 : 1](
              canvas,
              state->sheet + gSoulShatter_SparkPictures[cell] +
                  SOUL_SHATTER_SPARK_PICTURES,
              x - gSoulShatter_SparkSizes[cell] / 2,
              y - gSoulShatter_SparkSizes[cell] / 2,
              gSoulShatter_SparkSizes[cell], gSoulShatter_SparkSizes[cell]);
          spark->variant++;
          Particle_AdvanceWithGravity2D(spark, 62, -toFIXED(1) / 8);
        }
      }
    }

    // From frame 32 the motes fly; mote i turns toward the user's middle
    // from frame 32 + i and vanishes once it passes the user.
    if (frame > 31) {
      struct MotionObject *user;
      s32 userHalfHeight;
      struct BattleCamera *camera;

      user = BattleObject_GetFar(args->user)->object;
      userHalfHeight = BattleUnit_GetEffectHeightFar(args->user) / 2;
      camera = Ram_HeapSlots->cameraState;
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      for (i = 0; i != SOUL_SHATTER_MOTES; i++) {
        struct Particle *mote = &state->particles[SOUL_SHATTER_SPARKS * 2 + i];
        if (mote->variant >= 0) {
          s32 size = (i & 1) + 9;
          s32 userX;
          s32 sameSide;
          s32 moteX;

          EffectPosition_ProjectPoint(&mote->x, &screen);
          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  (screen.x >> 1) - size / 2, screen.y - size, size, size * 2);
          Particle_AdvanceWithGravity3D(mote, 62, 0);
          if (frame > i + 32) {
            s32 dx = (user->x - mote->x) >> 7;
            s32 dy = (user->y + userHalfHeight - mote->y) >> 7;
            s32 dz = (user->z - mote->z) >> 7;

            mote->velocityX += dx;
            mote->velocityY += dy;
            mote->velocityZ += dz;
            moteX = mote->x;
            if (moteX < 0)
              moteX = -moteX;
            /* FAKEMATCH(dead): the read of user->x in a once-only block; as the
             * plain read. */
            do {
              userX = user->x;
            } while (0);
            if (userX < 0)
              userX = -userX;
            // Gone once it has passed the user on the user's side of x = 0.
            /* FAKEMATCH(dead): each arm stores its test in sameSide, never
             * read; as the bare tests. */
            if (moteX >= userX && (user->x < 0 ? (sameSide = mote->x < 0)
                                               : (sameSide = mote->x >= 0)))
              mote->variant = -1;
          }
        }
      }
    }

    EFFECT_RELEASE_BLITTERS();
    if (frame <= 7)
      BattleCamera_Shake(4, 4);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

/// A point projected onto the screen: x, y and depth.
struct ProjectedPoint {
  s32 x, y, depth;
};

/// The spire's corners and its mesh, in the map cell buffer.
#define SERAC_SPIRE_CORNERS ((struct ModelVertex *)Ram_MapCellBuffer)
#define SERAC_SPIRE_MESH (Ram_MapCellBuffer + 8192)

/// Where MercuryDjinnSheet is decoded in the effect's pictures.
#define SERAC_MERCURY_DJINN_PICTURES 16384

#define SERAC_FRAMES 88

/// Where BlueCloudPuffSheet is decoded in the effect's pictures.
#define SERAC_CLOUD_PUFFS 1024
/// Where FlameBandTexture, the mist inside the spire, is decoded in the map
/// cell buffer.
#define SERAC_MIST 16384

/// The spire of ice: a cone of SERAC_SPIRE_RINGS rings of SERAC_SPIRE_SIDES
/// corners, each quad of its mesh 16 texels across and 8 high. It grows from
/// frame 26 by SERAC_SPIRE_GROWTH a frame to its scale SERAC_SPIRE_SCALE.
#define SERAC_SPIRE_RINGS 8
#define SERAC_SPIRE_SIDES 8
#define SERAC_SPIRE_QUAD_TEXELS 16
#define SERAC_SPIRE_GROWTH toFIXED(0.1526)
#define SERAC_SPIRE_SCALE toFIXED(0.91553)

/// The camera circles the spire SERAC_CAMERA_TURN a frame, slowing from
/// frame 64, and turns no further than SERAC_CAMERA_YAW_MAX either way.
#define SERAC_CAMERA_TURN 320
#define SERAC_CAMERA_YAW_MAX 14000

/// The puffs of dust where the Djinni lands, each drawn for
/// SERAC_DUST_PUFF_FRAMES through BlueCloudPuffSheet's four pictures.
#define SERAC_DUST_PUFFS 32
#define SERAC_DUST_PUFF_FRAMES 24

/// The four rings of mist spread from the spire's foot from frame
/// SERAC_RINGS_FRAME; a ring of gBattleEffect_CylinderMesh starts at
/// SERAC_RING_START_SCALE and grows by its growth times SERAC_RING_GROWTH a
/// frame.
#define SERAC_RINGS_FRAME 22
#define SERAC_RING_START_SCALE toFIXED(0.25586)
#define SERAC_RING_GROWTH toFIXED(0.0229)

/// A shard's y speed is three times a random fraction of a pixel less
/// SERAC_SHARD_SPEED_BIAS: from about -0.58 to 2.42 pixels a frame, 0.92 on
/// average.
#define SERAC_SHARD_SPEED_BIAS 12768

/// "Strike a chilling finishing blow." The Mercury Djinni drops onto the
/// target, dust bursting where it lands; a spire of ice (its mesh built on the
/// first frame) grows up around the target in rings of mist while the camera
/// circles, the caster closes in at frame 56, and at 64 the spire shatters
/// into 64 tumbling shards.
///
/// Abilities: Serac.
void BattleEffect_RunSerac(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleEffectState *state;
  s32 frame;
  DrawRectangle blitters[2];
  s32 screenB[3];
  s32 screenA[3];
  u8 *mistSheet;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  BattleEffect_ConfigureEffectDisplay();
  if (args->side == EFFECT_SIDE_ENEMIES) {
    BattleEffect_StartTiledCanvasFilled(BGCNT_WRAP);
  } else {
    BattleEffect_StartTiledCanvas(BGCNT_WRAP);
  }
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_MERCURY, args->side,
                      DJINNI_ANCHOR_OVER_TARGET_SHORT),
      screenA, screenB);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
  Iwram_ClearWordsEntry(canvas, CANVAS_VIEW_BYTES);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2PA = Q_8_8(1);
  BattleEffect_BuildCanvasBlitters(0, blitters);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  mistSheet = Ram_MapCellBuffer + SERAC_MIST;
  Resource_LoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, mistSheet, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_DUST_BLOB_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_BLUE_CLOUD_PUFF_SHEET,
                             state->sheet + SERAC_CLOUD_PUFFS, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
                             state->sheet + SERAC_MERCURY_DJINN_PICTURES, 1, 0);
  LOAD_BG_PALETTE(RESOURCE_BLUE_FLAME_SHEET);
  {
    struct BattleCamera *camera;
    s16 savedYaw;
    struct MotionObject *target;
    struct ProjectedPoint screen;
    s32 world[3];
    struct ProjectedPoint point;
    struct ProjectedPoint dropScreen;
    struct ModelTexture texture;
    struct RenderModel *model;
    void *vertices;
    u8 color[1];
    s32 savedCenterX;
    s32 savedCenterY;
    s32 row;
    s32 column;
    s32 i;

    camera = (struct BattleCamera *)Ram_HeapSlots->cameraState;
    savedYaw = camera->yaw;
    target = BattleObject_GetFar(args->targets[0])->object;
    REG_BG2PA = Q_8_8(1);
    REG_BG2X = 0;

    for (frame = 0; frame != SERAC_FRAMES; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        for (row = 0; row != SERAC_SPIRE_RINGS; row++) {
          for (column = 0; column != SERAC_SPIRE_SIDES; column++) {
            s32 height;
            s32 radius;
            s32 angle;

            if (row == 0) {
              height = gSerac_RowHeights[row];
              radius = gSerac_RowRadii[row];
            } else {
              height = gSerac_RowHeights[row] + (Random16() & 3);
              radius = gSerac_RowRadii[row] + (Random16() & 3);
            }
            angle = column * (TRIG_TURN / SERAC_SPIRE_SIDES) + TRIG_TURN / 16;
            SERAC_SPIRE_CORNERS[row * SERAC_SPIRE_SIDES + column].x =
                FIXEDtoINT(Trig_Sin(angle) * radius);
            SERAC_SPIRE_CORNERS[row * SERAC_SPIRE_SIDES + column].y = -height;
            SERAC_SPIRE_CORNERS[row * SERAC_SPIRE_SIDES + column].z =
                FIXEDtoINT(Trig_Cos(angle) * radius);
            // Each quad between this ring and the next is two triangles of the
            // mesh.
            if (row < SERAC_SPIRE_RINGS - 1) {
              struct MeshTriangle *firstTriangle =
                  (struct MeshTriangle *)SERAC_SPIRE_MESH +
                  (row * SERAC_SPIRE_SIDES + column) * 2;
              struct MeshTriangle *secondTriangle;
              s32 bottom = row * 8 + 7;
              s32 left = column * SERAC_SPIRE_QUAD_TEXELS;
              s32 right = left + SERAC_SPIRE_QUAD_TEXELS - 1;
              s32 next;
              s32 here;

              firstTriangle->uv[0][0] = right;
              firstTriangle->uv[0][1] = row * 8;
              firstTriangle->uv[1][0] = right;
              firstTriangle->uv[2][1] = bottom;
              firstTriangle->uv[1][1] = bottom;
              firstTriangle->uv[2][0] = left;
              secondTriangle = (struct MeshTriangle *)SERAC_SPIRE_MESH +
                               (row * SERAC_SPIRE_SIDES + column) * 2 + 1;
              secondTriangle->uv[0][0] = left;
              secondTriangle->uv[2][0] = left;
              secondTriangle->uv[0][1] = row * 8;
              secondTriangle->uv[2][1] = bottom;
              secondTriangle->uv[1][0] = right;
              secondTriangle->uv[1][1] = row * 8;
              next = (column + 1) % SERAC_SPIRE_SIDES + row * SERAC_SPIRE_SIDES;
              firstTriangle->vertex[0] = next;
              here = column % SERAC_SPIRE_SIDES + row * SERAC_SPIRE_SIDES;
              firstTriangle->vertex[1] = here + SERAC_SPIRE_SIDES;
              firstTriangle->vertex[2] = next + SERAC_SPIRE_SIDES;
              secondTriangle->vertex[0] = here;
              secondTriangle->vertex[1] = here + SERAC_SPIRE_SIDES;
              secondTriangle->vertex[2] = next;
            }
          }
        }
        gSerac_MeshEnd[0] = 0;
        gSerac_MeshEnd[1] = 0;
        gSerac_MeshEnd[2] = 0;
      }

      // The camera circles the spire.
      if ((u32)(frame - 4) <= 79) {
        s32 turn;

        if (frame <= 63) {
          turn = SERAC_CAMERA_TURN;
        } else {
          turn = 1600 - frame * 20;
        }
        if (args->side == EFFECT_SIDE_ENEMIES) {
          camera->yaw -= turn;
        } else {
          camera->yaw += turn;
        }
        if (args->side == EFFECT_SIDE_ENEMIES) {
          if (camera->yaw < -SERAC_CAMERA_YAW_MAX) {
            camera->yaw = -SERAC_CAMERA_YAW_MAX;
          }
        } else if (camera->yaw > SERAC_CAMERA_YAW_MAX) {
          camera->yaw = SERAC_CAMERA_YAW_MAX;
        }
      }

      vertices = Heap_AllocEwram(1024);
      model = Render_AllocateModelList(1);
      savedCenterX = gProjection.centerX;
      savedCenterY = gProjection.centerY;
      gProjection.centerY = savedCenterY - 16;
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      world[0] = target->x;
      world[1] = 0;
      world[2] = target->z;
      Matrix_ProjectPoint(world, &screen);
      if (screen.x <= 63) {
        screen.x = 64;
      }
      if (screen.x > 176) {
        screen.x = 176;
      }
      gProjection.centerX = 184 - screen.x;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X, (64 - screen.x) << 8, IO_WRITE_WORD);

      // Dust where the Djinni lands.
      if ((u32)frame <= 63) {
        if (frame == 0) {
          for (i = 0; i != SERAC_DUST_PUFFS; i++) {
            struct Particle *p = &state->particles[i];
            s32 speed = (Random16() & 1023) + 1024;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            SET_POSITION(p, target->x, 0, target->z);
            p->velocityX = (Trig_Sin(angle) * speed) >> 9;
            p->velocityY = 0;
            p->velocityZ = (Trig_Cos(angle) * speed) >> 9;
            p->variant = -i;
          }
        }
        for (i = 0; i != SERAC_DUST_PUFFS; i++) {
          struct Particle *p = &state->particles[i];
          s32 picture;
          s32 x;
          s32 y;

          if ((u32)++p->variant >= SERAC_DUST_PUFF_FRAMES) {
            continue;
          }
          picture = p->variant / 6;
          Matrix_ProjectPoint(p, &point);
          Particle_AdvanceWithGravity3D(p, 60, toFIXED(1) / 8);
          x = point.x - CLOUD_PUFF_WIDTH / 2;
          y = point.y - CLOUD_PUFF_HEIGHT / 2;
          blitters[0](canvas,
                      state->sheet + SERAC_CLOUD_PUFFS +
                          picture * PICTURE_BYTES(CLOUD_PUFF),
                      x, y, CLOUD_PUFF_WIDTH, CLOUD_PUFF_HEIGHT);
        }
      }
      // The Djinni falls.
      if (frame <= 29) {
        point.x = target->x;
        point.y = toFIXED(60) - frame * toFIXED(2);
        point.depth = target->z + (Trig_Sin(frame * (TRIG_TURN / 128)) << 5);
        Matrix_ProjectPoint(&point, &dropScreen);
        blitters[1](canvas, state->sheet + SERAC_MERCURY_DJINN_PICTURES,
                    dropScreen.x - 20, dropScreen.y - 30, DJINNI_WIDTH,
                    DJINNI_HEIGHT);
      }
      if (frame == 4) {
        Sound_PlayFar(SE_SWITCH_MECHANISM);
      }
      if (frame == 30) {
        Sound_PlayFar(SE_DJINNI_SET);
      }
      if (frame == 60) {
        Sound_PlayFar(SE_POWER_UP);
      }
      if (frame == 64) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
      }
      if (frame == 30) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
      }
      if (frame == 26) {
        state->shakeFrames = 8;
      }
      if (frame == 64) {
        state->shakeFrames = 4;
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      if (frame == 70) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      if (frame == 56) {
        // The caster closes four fifths of the way.
        struct Actor *caster =
            (struct Actor *)BattleObject_GetFar(args->user)->object;
        struct Actor *to =
            (struct Actor *)BattleObject_GetFar(args->targets[0])->object;
        s32 x = caster->x.fixed;
        FIXED z;
        s32 dx = (to->x.fixed - x) * 80 / 100;
        s32 dz;
        s32 speed;

        z = caster->z.fixed;
        dz = (to->z.fixed - z) * 80 / 100;
        x += dx;
        z += dz;
        dx >>= 8;
        dz >>= 8;
        speed = (Iwram_Sqrt(dx * dx + dz * dz) << 8) / 10;
        caster->acceleration = speed;
        caster->speedLimit = speed;
        caster->snapToTarget = 1;
        caster->velocityY = toFIXED(6);
        caster->gravity = toFIXED(6) / 5;
        caster->bounce = 0;
        caster->steeringFlags = 1;
        Actor_StartIdleFar(caster);
        Actor_SetMoveTargetFar(caster, x, 0, z);
        Actor_SelectAnimationFar(caster, POSE_LEAP);
      }

      if (frame > 63) {
        if (frame == 64) {
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                               CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
            struct Particle *p = &state->particles[i];
            s32 angle = (i & 7) * (TRIG_TURN / 8);
            s32 ring;

            SET_POSITION(p,
                         target->x + ((Trig_Sin(angle) * (ring = i / 8)) << 2),
                         ((Random16() & TRIG_ANGLE_MASK) << 2) -
                             ring * toFIXED(10) + toFIXED(64),
                         target->z + ((Trig_Cos(angle) * ring) << 2));
            p->velocityX = ((Random16() & TRIG_ANGLE_MASK) << 1) + toFIXED(1);
            p->velocityY =
                ((Random16() & TRIG_ANGLE_MASK) - SERAC_SHARD_SPEED_BIAS) * 3;
            p->velocityZ = ((Random16() & TRIG_ANGLE_MASK) - toFIXED(0.5)) << 1;
            if (p->x < 0) {
              p->velocityX = -p->velocityX;
            }
            p->variant = (Random16() & 31) + 32;
          }
        } else {
          model->kind = MODEL_SOLID_MAX;
          model->flags = 0;
          model->mesh = gBattleEffect_TriangleMesh;
          model->vertices = vertices;
          for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
            struct Particle *p = &state->particles[i];
            s32 angle = (frame + i) * (TRIG_TURN / 64);

            if (p->variant < 0) {
              continue;
            }
            color[0] = p->variant;
            p->variant -= 2;
            model->shade = color;
            Particle_AdvanceWithGravity3D(p, 62, toFIXED(1) / 64);
            Matrix_Push();
            Matrix_Translate(p->x, p->y, p->z);
            Matrix_RotateX(angle);
            Matrix_RotateZ(angle);
            Matrix_ScaleUniform(FIXED_ONE);
            Render_ProjectVertices(gBattleEffect_TriangleCorners, vertices, 3);
            Render_DrawModelList(model);
            Matrix_Pop();
          }
        }
      }
      if (frame < 64) {
        if (frame > 25) {
          s32 scale;
          s32 start;
          // The spire is drawn at its own shade, the mist inside it 32 levels
          // dimmer.
          s32 spireShade;
          const void *mesh = SERAC_SPIRE_MESH;

          // The spire, then the mist inside it.
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = (const u8 *)state->sheet;
          model->kind = MODEL_SHADED_MAX;
          model->flags = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
          model->vertices = vertices;
          model->scrollU = 0;
          model->texture = &texture;
          model->mesh = mesh;
          model->scrollV = -((frame / 2) & 31);
          Matrix_Push();
          spireShade = 0;
          if (frame <= 31) {
            scale = frame * SERAC_SPIRE_GROWTH - 26 * SERAC_SPIRE_GROWTH;
          } else {
            scale = SERAC_SPIRE_SCALE;
          }
          model->shade = (const void *)spireShade;
          Matrix_Translate(target->x, -toFIXED(4), target->z);
          Matrix_Scale(scale, scale, scale);
          Render_ProjectVertices((const u8 *)gMapGridCellBuffer, vertices,
                                 SERAC_SPIRE_RINGS * SERAC_SPIRE_SIDES);
          Render_DrawModelList(model);
          spireShade = -32;
          model->shade = (const void *)spireShade;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->scrollU = 0;
          model->scrollV = (frame / 2) & 31;
          model->flags = MODEL_ONE_SIDED;
          Render_DrawModelList(model);
          Matrix_Pop();

          // Four rings of mist spread from its foot.
          texture.logWidth = 7;
          texture.logHeight = 7;
          texture.pixels = mistSheet;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->texture = &texture;
          model->mesh = gBattleEffect_CylinderMesh;
          model->flags = 0;
          model->vertices = vertices;
          model->scrollU = 0;
          model->scrollV = 0;
          for (i = 0; i != 4; i++) {
            s32 size;
            s32 shade;

            start = gSerac_RingStarts[i] + SERAC_RINGS_FRAME;
            if (frame <= start) {
              continue;
            }
            size = (frame - start) * gSerac_RingGrowth[i] * SERAC_RING_GROWTH +
                   SERAC_RING_START_SCALE;
            shade = (start - frame) * 8 + 56;
            if (shade > 0) {
              shade = 0;
            }
            if (shade <= MODEL_SHADE_INVISIBLE) {
              continue;
            }
            Matrix_Push();
            model->shade = (const void *)shade;
            Matrix_Translate(target->x, INTtoFIXED(gSerac_RingHeights[i]),
                             target->z);
            Matrix_Scale(size, size, size);
            model->scrollU = ((frame + i * 5) & 7) << 4;
            Render_ProjectVertices(gBattleEffect_CylinderCorners, vertices, 32);
            Render_DrawModelList(model);
            Matrix_Pop();
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
      gProjection.centerX = savedCenterX;
      gProjection.centerY = savedCenterY;
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(state);
    }
  }
  EFFECT_FINISH_CANVAS();
}

/// A great sword swings down onto the target, trailing blue after-images.
///
/// Abilities: Heavy Divide.
void BattleEffect_RunHeavyDivide(struct BattleEffectParams *args) {
  BattleEffect_RunHeavyBlow(args, HEAVY_BLOW_HEAVY_DIVIDE);
}

/// An axe spins across from the user to the target.
///
/// Abilities: Strong Hit.
void BattleEffect_RunStrongHit(struct BattleEffectParams *args) {
  BattleEffect_RunHeavyBlow(args, HEAVY_BLOW_STRONG_HIT);
}
#undef NIRVANA_FRAMES
#undef NIRVANA_SPARKS
#undef NIRVANA_RING_RADIUS
#undef NIRVANA_PULL
#undef STONE_JUSTICE_ROCKS
#undef STONE_JUSTICE_FRAMES
#undef STONE_JUSTICE_HURL_FRAME
#undef STONE_JUSTICE_SPIN_STEP
#undef SOUL_SHATTER_FRAMES
#undef SOUL_SHATTER_COLUMN_WIDTH
#undef SOUL_SHATTER_COLUMN_HEIGHT
#undef SOUL_SHATTER_SPARK_PICTURES
#undef SOUL_SHATTER_SPARKS
#undef SOUL_SHATTER_SPARK_FRAMES
#undef SOUL_SHATTER_MOTES
#undef SERAC_SPIRE_CORNERS
#undef SERAC_SPIRE_MESH
#undef SERAC_MERCURY_DJINN_PICTURES
#undef SERAC_FRAMES
#undef SERAC_CLOUD_PUFFS
#undef SERAC_MIST
#undef SERAC_SPIRE_RINGS
#undef SERAC_SPIRE_SIDES
#undef SERAC_SPIRE_QUAD_TEXELS
#undef SERAC_SPIRE_GROWTH
#undef SERAC_SPIRE_SCALE
#undef SERAC_CAMERA_TURN
#undef SERAC_CAMERA_YAW_MAX
#undef SERAC_DUST_PUFFS
#undef SERAC_DUST_PUFF_FRAMES
#undef SERAC_RINGS_FRAME
#undef SERAC_RING_START_SCALE
#undef SERAC_RING_GROWTH
#undef SERAC_SHARD_SPEED_BIAS
