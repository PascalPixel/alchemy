/// @file
/// Nirvana, Stone Justice, Soul Shatter and Serac.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define NIRVANA_FRAMES 80

/// A mote leaves the first target and is drawn as a ring of 24 sparks
/// turning about it while it is pulled back to the user.
void BattleEffect_RunNirvana(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  struct BattleCamera *view;
  u8 *sheet;
  struct MotionObject *object;
  struct Particle *orb;
  s32 half;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  view = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet),
                       SHEET_PALETTE_BYTES);
  object = BattleObject_GetFar(args->targets[0])->object;
  half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
  orb = work->particles;
  SET_POSITION(orb, object->x, object->y + half, object->z);
  orb->velocityX = 0;
  orb->velocityY = 0;
  orb->velocityZ = 0;
  orb->variant = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  blit = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != NIRVANA_FRAMES; frame++) {
    struct MotionObject *user = BattleObject_GetFar(args->user)->object;
    s32 userHalf = BattleUnit_GetEffectHeightFar(args->user) / 2;

    if (frame == 64)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    Transform_Reset();
    Transform_SetLookAt(view->eye, view->focus);
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

      for (i = 0; i != 24; i++) {
        s32 point[3];
        struct ScreenPoint out;

        Transform_Push();
        Transform_ApplyTranslation(orb->x, orb->y, orb->z);
        if (i <= 11)
          Transform_ApplyPitch(frame << 9);
        else
          Transform_ApplyPitch((frame << 9) + TRIG_TURN / 4);
        Transform_ApplyRoll(i * 5461 + (frame << 10));
        Transform_ApplyTranslation(0, Trig_Sin(frame * 409) << 6, 0);
        point[0] = 0;
        point[1] = 0;
        point[2] = 0;
        EffectPosition_ProjectPoint(point, &out);
        out.x >>= 1;
        Transform_Pop();
        blit(canvas, sheet + gBattleEffect_MotePictures[size - 1],
             out.x - size / 2, out.y - size, size, size * 2);
      }
      BattleEffectKernels_IntegrateVector3(orb, 62, 0);
      if (frame > 0) {
        s32 dx = (user->x - orb->x) >> 10;
        s32 dy = (user->y + userHalf - orb->y) >> 10;
        s32 dz = (user->z - orb->z) >> 10;
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
        if (ux < ox)
          orb->variant = -1;
      }
    }
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The pixel half of a 16.16 fixed-point value.
static inline s16 *Fixed_Halves(s32 *value) {
  /* FAKEMATCH(wrapper): the halves read through this inline. */
  return (s16 *)value;
}

#define ROCKS 21

#define STONE_JUSTICE_FRAMES 107

/// Stone Justice: 21 boulders rise in a column beside the user and tumble
/// across the screen into the target, while the battle background slides
/// with them.
///
/// Setup: canvas layer 1 at full width, the boulder sheet, one transparent
/// blitter, and the user's sprite group set apart (48) for the length of
/// the effect.
/// Frame loop (107 frames): the boulders are thrown at frame 0 and spin as
/// they fly; from frame 64 the background scrolls after them and each
/// boulder turns toward the target; every boulder that reaches the target
/// shakes the screen and knocks the target back.
///
/// Abilities: Bane, Stone Justice.
void BattleEffect_RunStoneJustice(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 scroll;
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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  blit = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  scroll = 0;
  scrollSpeed = 0;
  cueTimer = 0;
  user = BattleObject_GetFar(args->user)->object;
  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL * 3);

  for (frame = 0; frame != STONE_JUSTICE_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(args->user, &userPosition);
    EffectPosition_ProjectScaledBattler(args->targets[0], &targetPosition);
    if (frame == 0)
      Sound_PlayFar(SE_GATE_MOVE);
    if (frame == 64)
      Sound_PlayFar(SE_104);

    // Frame 0: the boulders stand in a column 2 pixels apart, on the
    // user's side of the canvas, and are thrown upward at random speeds.
    if (frame == 0) {
      for (i = 0; i != ROCKS; i++) {
        rock = &work->particles[i];
        if (args->side == EFFECT_SIDE_ENEMIES)
          rock->x = px(Random16() % 40 + 8);
        else
          rock->x = px(120 - Random16() % 40);
        Random16();
        rock->y = px(i * 2 + 96);
        rock->velocityX = 0;
        rock->velocityY = -((Random16() & 127) + 32) << 11;
        rock->z = (Random16() & 15) + 8;
        rock->velocityZ = 0;
        rock->variant = 0;
      }
    }
    if (frame == 52)
      Actor_SelectAnimationFar(user, 2);

    // From frame 64 the background follows the boulders, ever faster.
    if (frame > 64) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        scroll += scrollSpeed;
      else
        scroll -= scrollSpeed;
      scrollSpeed += 0x8000;
      scrollSpeed = scrollSpeed * 60 / DAMPING_SCALE;
    }
    scrollX = Q_16_16_TO_INT(scroll);
    REG_BG2X = (64 - (userPosition.x - scrollX)) << 8;

    // Each boulder spins through three pictures of its shard (z is its
    // spin rate, velocityZ the angle) and hits the target once it passes
    // it.
    hit = 0;
    for (i = 0; i != ROCKS; i++) {
      s32 cell;
      u32 width;

      rock = &work->particles[i];
      cell = (i % 5) * 3 + rock->velocityZ / 96 % 3;
      width = gSpire_ShardWidths[cell];

      blit(canvas, work->sheet + gSpire_ShardPictures[cell] + 0x83c,
           Fixed_Halves(&rock->x)[1] - width / 2 + scrollX,
           Fixed_Halves(&rock->y)[1] - gSpire_ShardHeights[cell] / 2, width,
           gSpire_ShardHeights[cell]);
      if (rock->variant == 0) {
        if (args->side == EFFECT_SIDE_ENEMIES) {
          if (targetPosition.x >
              Fixed_Halves(&rock->x)[1] + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        } else {
          if (targetPosition.x <
              Fixed_Halves(&rock->x)[1] + userPosition.x - 64) {
            rock->variant = 1;
            hit = 1;
          }
        }
      }
      BattleEffectKernels_IntegrateVector2(rock, 60, 0);
      rock->velocityZ += rock->z;
      if (frame > i / 2 + 64) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          rock->velocityX -= px(1) / 2;
        else
          rock->velocityX += px(1) / 2;
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
      BattleEffect_StartActorMotion(args->targets[0], 1, px(2), px(4), px(2),
                                    150);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 4);
      work->shakeFrames = 2;
    }

    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPARK_OFFSET 0x460
#define SPARKS 16
#define MOTES 32

// Soul Shatter: a column bursts up under the target in a spray of sparks
// and throws it into the air; the user glows, and from frame 32 motes of
// the target's soul fly out and are drawn back into the user.
//
// Setup: canvas layer 1 at a light blend, centred on the target; the
// column, the spark pictures after it, the mote sheet and the orange
// palette.
// Frame loop (106 frames): the column rises from frame 8, 12 pixels a
// frame, stands at 80 pixels (drawn with max blitters) and sinks from
// frame 80; the target is thrown up at frame 10 and comes down at frame
// 80.
#define SOUL_SHATTER_FRAMES 106

/// Soul Shatter: two mirrored halves of a column and sixteen sparks, in an
/// effect in which the target is thrown up and 32 motes of its
/// soul drift back into the user.
void BattleEffect_RunSoulShatter(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  struct MotionObject *target;
  DrawRectangle draw[2];
  struct ScreenPoint position;
  struct ScreenPoint screen;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 12);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + SPARK_OFFSET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  target = BattleObject_GetFar(args->targets[0])->object;
  EffectPosition_ProjectBattler(args->targets[0], &position);
  position.x /= 2;
  work->shakeFrames = 8;

  for (frame = 0; frame != SOUL_SHATTER_FRAMES; frame++) {
    // Frame 0: sparks leave the column's foot upward, and the motes start
    // at the target, 52 pixels up, in every direction.
    if (frame == 0) {
      target->y = 0;
      {
        struct Particle *spark;

        for (i = 0, spark = work->particles; i != SPARKS; i++, spark++) {
          s32 speed = (Random16() & 0xff) + 256;
          s32 angle = (Random16() & 0x7fff) + TRIG_TURN / 4;

          spark->x = px(position.x);
          spark->y = px(96 - (Random16() & 31));
          spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
          spark->variant = Random16() & 7;
        }
      }
      {
        struct Particle *mote;

        for (i = 0, mote = &work->particles[SPARKS * 2]; i != MOTES;
             i++, mote++) {
          s32 speed = (Random16() & 127) + 128;
          s32 angle = Random16() & 0x7fff;

          SET_POSITION(mote, target->x, px(52), target->z);
          mote->velocityY = ((Random16() & 0xff) - 128) << 9;
          mote->velocityX = (Trig_Sin(angle) * speed) >> 7;
          mote->velocityZ = (Trig_Cos(angle) * speed) >> 7;
          mote->variant = 0;
        }
      }
    }

    // The cues: the burst, the target thrown up 32 pixels, the user's
    // glow, the drain and the target's fall.
    if (frame == 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 10) {
      work->shakeFrames = 8;
      Sound_PlayFar(SE_CLATTER);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 80);
      target->verticalMotionStrength = 0;
      target->y = px(32);
    }
    if (frame == 80)
      target->verticalMotionStrength = 0xab85;
    if (frame == 24)
      BattleEffect_TintBattler(args->user, TINT_KEEP, 3, TINT_KEEP, 0);
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

    // The column: two 14-pixel halves, the right one mirrored.
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

        if (height > 80) {
          height = 80;
          mode = BLITTER_MAX;
        } else {
          mode = BLITTER_ADDITIVE;
        }
        for (i = 0, x = centre - 14; i != 2; i++, x += 14) {
          if (i == 0)
            BattleEffect_BuildBlitter(
                104, 7, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            BattleEffect_BuildBlitter(
                104, 7, 7,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          draw[0](canvas, work->sheet, x, 104 - height, 14, height);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }

    // The sparks: from frame 8, two more each frame, through seven
    // pictures three frames apiece; one in four is drawn mirrored.
    BattleEffect_FetchRectangleBlitters(args->side, draw);
    {
      struct Particle *spark;

      for (i = 0, spark = work->particles; i != SPARKS; i++, spark++) {
        if (frame >= i / 2 + 8 && spark->variant <= 28) {
          s32 cell = spark->variant / 3;
          s32 x = Q_16_16_UPPER_HALF(spark->x);
          s32 y = Q_16_16_UPPER_HALF(spark->y);

          if (cell > 6)
            cell = 6;
          draw[(i & 3) ? 0 : 1](
              canvas,
              work->sheet + gSoulShatter_SparkPictures[cell] + SPARK_OFFSET,
              x - gSoulShatter_SparkSizes[cell] / 2,
              y - gSoulShatter_SparkSizes[cell] / 2,
              gSoulShatter_SparkSizes[cell], gSoulShatter_SparkSizes[cell]);
          spark->variant++;
          BattleEffectKernels_IntegrateVector2(spark, 62, -px(1) / 8);
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
      camera = Ram_HeapSlots->cameraWork;
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->focus);
      for (i = 0; i != MOTES; i++) {
        struct Particle *mote = &work->particles[SPARKS * 2 + i];
        if (mote->variant >= 0) {
          s32 size = (i & 1) + 9;
          s32 userX;
          s32 sameSide;
          s32 moteX;

          EffectPosition_ProjectPoint(&mote->x, &screen);
          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  (screen.x >> 1) - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(mote, 62, 0);
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
            // A once-only block around the read of user->x.
            do {
              userX = user->x;
            } while (0);
            if (userX < 0)
              userX = -userX;
            // Gone once it has passed the user on the user's side of x = 0.
            /* FAKEMATCH(dead): each arm stores its test in sameSide, never
             * read. */
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
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

struct StripQuad;

/// A point projected onto the screen: x, y and depth.
struct ProjectedPoint {
  s32 x, y, depth;
};

#define VERTICES ((struct ModelVertex *)Ram_MapCellBuffer)
#define MESH_BUFFER (Ram_MapCellBuffer + 0x2000)

/// Where MercuryDjinnSheet is decoded in the effect's pictures.
#define SERAC_MERCURY_DJINN_PICTURES 0x4000

#define SERAC_FRAMES 88

/// Where CloudPuffSheet is decoded in the effect's pictures.
#define SERAC_CLOUD_PUFFS 0x400

/// The Mercury Djinni drops onto the target, dust bursting where it lands;
/// a spire of ice (a cone of 8 rings of 8 vertices, its mesh built on the
/// first frame) grows up around the target in rings of mist while the
/// camera circles, the caster closes in at frame 56, and at 64 the spire
/// shatters into 64 tumbling shards.
///
/// Abilities: Serac.
void BattleEffect_RunSerac(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blitters[2];
  s32 screenB[3];
  s32 screenA[3];
  u8 *mistSheet;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
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
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  mistSheet = Ram_MapCellBuffer + 0x4000;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, mistSheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CF,
                             work->sheet + SERAC_CLOUD_PUFFS, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                             work->sheet + SERAC_MERCURY_DJINN_PICTURES, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       SHEET_PALETTE_BYTES);
  // A block of its own: camera, savedYaw and target take their stack slots
  // after the blitter pair's address.
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
    s32 savedScrollX;
    s32 savedScrollY;
    s32 row;
    s32 column;
    s32 i;

    camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
    savedYaw = camera->yaw;
    target = BattleObject_GetFar(args->targets[0])->object;
    REG_BG2PA = Q_8_8(1);
    REG_BG2X = 0;

    for (frame = 0; frame != SERAC_FRAMES; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        for (row = 0; row != 8; row++) {
          for (column = 0; column != 8; column++) {
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
            angle = (column << 13) + TRIG_TURN / 16;
            VERTICES[row * 8 + column].x =
                Q_16_16_TO_INT(Trig_Sin(angle) * radius);
            VERTICES[row * 8 + column].y = -height;
            VERTICES[row * 8 + column].z =
                Q_16_16_TO_INT(Trig_Cos(angle) * radius);
            if (row <= 6) {
              u8 *quad = MESH_BUFFER + (row * 8 + column) * 24;
              u8 *gridQuad;
              s32 bottom = row * 8 + 7;
              s32 left = column * 16;
              s32 right = left + 15;
              s32 next;
              s32 here;

              quad[4] = right;
              quad[5] = row * 8;
              quad[6] = right;
              quad[9] = bottom;
              quad[7] = bottom;
              quad[8] = left;
              gridQuad = MESH_BUFFER + 12 + (row * 8 + column) * 24;
              gridQuad[4] = left;
              gridQuad[8] = left;
              gridQuad[5] = row * 8;
              gridQuad[9] = bottom;
              gridQuad[6] = right;
              gridQuad[7] = row * 8;
              next = (column + 1) % 8 + row * 8;
              quad[0] = next;
              here = column % 8 + row * 8;
              quad[1] = here + 8;
              quad[2] = next + 8;
              gridQuad[0] = here;
              gridQuad[1] = here + 8;
              gridQuad[2] = next;
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
          turn = 320;
        } else {
          turn = 1600 - frame * 20;
        }
        if (args->side == EFFECT_SIDE_ENEMIES) {
          camera->yaw -= turn;
        } else {
          camera->yaw += turn;
        }
        if (args->side == EFFECT_SIDE_ENEMIES) {
          if ((s16)camera->yaw < -14000) {
            camera->yaw = -14000;
          }
        } else if ((s16)camera->yaw > 14000) {
          camera->yaw = 14000;
        }
      }

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      savedScrollX = gProjection.centerX;
      savedScrollY = gProjection.centerY;
      gProjection.centerY = savedScrollY - 16;
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->focus);
      world[0] = target->x;
      world[1] = 0;
      world[2] = target->z;
      Render_ProjectPoint(world, &screen);
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
          for (i = 0; i != 32; i++) {
            struct Particle *p = &work->particles[i];
            s32 speed = (Random16() & 1023) + 1024;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            SET_POSITION(p, target->x, 0, target->z);
            p->velocityX = (Trig_Sin(angle) * speed) >> 9;
            p->velocityY = 0;
            p->velocityZ = (Trig_Cos(angle) * speed) >> 9;
            p->variant = -i;
          }
        }
        for (i = 0; i != 32; i++) {
          struct Particle *p = &work->particles[i];
          s32 picture;
          s32 x;
          s32 y;

          if ((u32)++p->variant > 23) {
            continue;
          }
          picture = p->variant / 6;
          Render_ProjectPoint(p, &point);
          BattleEffectKernels_IntegrateVector3(p, 60, px(1) / 8);
          x = point.x - CLOUD_PUFF_WIDTH / 2;
          y = point.y - CLOUD_PUFF_HEIGHT / 2;
          blitters[0](canvas,
                      work->sheet + SERAC_CLOUD_PUFFS +
                          picture * PICTURE_BYTES(CLOUD_PUFF),
                      x, y, CLOUD_PUFF_WIDTH, CLOUD_PUFF_HEIGHT);
        }
      }
      // The Djinni falls.
      if (frame <= 29) {
        point.x = target->x;
        point.y = px(60) - (frame << 17);
        point.depth = target->z + (Trig_Sin(frame << 9) << 5);
        Render_ProjectPoint(&point, &dropScreen);
        blitters[1](canvas, work->sheet + SERAC_MERCURY_DJINN_PICTURES,
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
        EFFECT_SHOW_DAMAGE(-1);
      }
      if (frame == 30) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
      }
      if (frame == 26) {
        work->shakeFrames = 8;
      }
      if (frame == 64) {
        work->shakeFrames = 4;
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
        s32 z;
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
        caster->velocityY = px(6);
        caster->gravity = 0x13333;
        caster->bounce = 0;
        caster->steeringFlags = 1;
        Actor_StartIdleFar(caster);
        Actor_SetMoveTargetFar(caster, x, 0, z);
        Actor_SelectAnimationFar(caster, 2);
      }

      if (frame > 63) {
        if (frame == 64) {
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                               CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
            struct Particle *p = &work->particles[i];
            s32 angle = (i & 7) << 13;
            s32 ring;

            SET_POSITION(p,
                         target->x + ((Trig_Sin(angle) * (ring = i / 8)) << 2),
                         ((Random16() & TRIG_ANGLE_MASK) << 2) -
                             ((ring * 5) << 17) + 0x400000,
                         target->z + ((Trig_Cos(angle) * ring) << 2));
            p->velocityX = ((Random16() & TRIG_ANGLE_MASK) << 1) + 0x10000;
            p->velocityY = ((Random16() & TRIG_ANGLE_MASK) - 12768) * 3;
            p->velocityZ = ((Random16() & TRIG_ANGLE_MASK) - 0x8000) << 1;
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
          for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
            struct Particle *p = &work->particles[i];
            s32 angle = (frame + i) << 10;

            if (p->variant < 0) {
              continue;
            }
            color[0] = p->variant;
            p->variant -= 2;
            model->shade = color;
            BattleEffectKernels_IntegrateVector3(p, 62, px(1) / 64);
            Transform_Push();
            Transform_ApplyTranslation(p->x, p->y, p->z);
            Transform_ApplyPitch(angle);
            Transform_ApplyRoll(angle);
            Transform_ApplyUniformScale(Q16_ONE);
            Render_ProjectVertices(gBattleEffect_TriangleCorners, vertices, 3);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
      }
      if (frame < 64) {
        if (frame > 25) {
          s32 scale;
          s32 start;
          // Set before the branch below and stored after it; the second value
          // stays apart from the 7 beside it.
          s32 spireShade;
          const void *mesh = Ram_MapCellBuffer + 0x2000;

          // The spire, then the mist inside it.
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = (const u8 *)work->sheet;
          model->kind = MODEL_SHADED_MAX;
          model->flags = 3;
          model->vertices = vertices;
          model->scrollU = 0;
          model->texture = &texture;
          model->mesh = mesh;
          model->scrollV = -((frame / 2) & 31);
          Transform_Push();
          spireShade = 0;
          if (frame <= 31) {
            scale = frame * 10000 - 260000;
          } else {
            scale = 60000;
          }
          model->shade = (const void *)spireShade;
          Transform_ApplyTranslation(target->x, -px(4), target->z);
          Transform_ApplyAxisScale(scale, scale, scale);
          Render_ProjectVertices((const u8 *)gMapGridCellBuffer, vertices, 64);
          Render_DrawModelList(model);
          spireShade = -32;
          model->shade = (const void *)spireShade;
          model->kind = MODEL_SHADED_ADDITIVE;
          model->scrollU = 0;
          model->scrollV = (frame / 2) & 31;
          model->flags = 2;
          Render_DrawModelList(model);
          Transform_Pop();

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

            start = gSerac_RingStarts[i] + 22;
            if (frame <= start) {
              continue;
            }
            size = (frame - start) * gSerac_RingGrowth[i] * 1500 + 0x4180;
            shade = (start - frame) * 8 + 56;
            if (shade > 0) {
              shade = 0;
            }
            if (shade <= MODEL_SHADE_INVISIBLE) {
              continue;
            }
            Transform_Push();
            model->shade = (const void *)shade;
            Transform_ApplyTranslation(target->x, px(gSerac_RingHeights[i]),
                                       target->z);
            Transform_ApplyAxisScale(size, size, size);
            model->scrollU = ((frame + i * 5) & 7) << 4;
            Render_ProjectVertices(gBattleEffect_CylinderCorners, vertices, 32);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
      gProjection.centerX = savedScrollX;
      gProjection.centerY = savedScrollY;
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }
  }
  EFFECT_FINISH_CANVAS();
}

/// Abilities: Heavy Divide.
void BattleEffect_RunHeavyDivide(struct BattleEffectParams *args) {
  BattleEffect_RunHeavyBlow(args, HEAVY_BLOW_HEAVY_DIVIDE);
}

/// Abilities: Strong Hit.
void BattleEffect_RunStrongHit(struct BattleEffectParams *args) {
  BattleEffect_RunHeavyBlow(args, HEAVY_BLOW_STRONG_HIT);
}
#undef BLITTER
#undef ROCKS
#undef SPARK_OFFSET
#undef SPARKS
#undef MOTES
#undef VERTICES
#undef MESH_BUFFER
