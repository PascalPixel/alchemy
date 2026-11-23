/// @file
/// Break and Drain.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Break and Drain take the targets in turn, this many frames apart.
#define DRAIN_TARGET_FRAMES 20

#define BREAK_SHARDS_PER_TARGET 128
/// Break runs DRAIN_TARGET_FRAMES a target and then this many while the last
/// shards fall.
#define BREAK_FALL_FRAMES 56
#define BREAK_HIT_FRAME 32
/// The canvas row below which a falling shard goes out.
#define BREAK_SHARD_GROUND 112

/// Ice shards burst up from each target in turn, tinting it, and fall away.
///
/// Abilities: Break.
void BattleEffect_RunBreak(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET),
                  SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * BREAK_SHARDS_PER_TARGET);

  EFFECT_VIEW_FROM_CAMERA(camera);

  // Scatter the shards around the middle of each body.
  EFFECT_FOR_EACH_TARGET(member, work->args) {
    struct Actor *actor;
    s32 offset;
    s32 height;
    struct Particle *shard;

    offset = member * BREAK_SHARDS_PER_TARGET * sizeof(struct Particle);
    actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
    height = BattleUnit_GetEffectHeightFar(work->args->targets[member]) / 2;
    SET_VECTOR(point, actor->x.fixed, height, actor->z.fixed);
    Projection_Point(point, &screen);
    screen.x >>= 1;
    for (i = 0, shard = (struct Particle *)(Ram_MapGridCellBuffer + offset);
         i != BREAK_SHARDS_PER_TARGET; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & TRIG_ANGLE_MASK;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + INTtoFIXED(screen.x);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + INTtoFIXED(screen.y);
      // Thrown up at 2 to 6 pixels a frame, and up to a pixel a frame aside.
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != work->args->targetCount * DRAIN_TARGET_FRAMES +
                               BREAK_FALL_FRAMES;
       frame++) {
    if (frame == BREAK_HIT_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      s32 offset;

      offset = member * BREAK_SHARDS_PER_TARGET * sizeof(struct Particle);
      if (frame == member * DRAIN_TARGET_FRAMES) {
        Sound_PlayFar(SE_CRACK);
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, TINT_KEEP, member, 20);
      }
      // A shard falls and goes out below the ground.
      if (frame > member * DRAIN_TARGET_FRAMES) {
        struct Particle *shard;

        for (i = 0, shard = (struct Particle *)(Ram_MapGridCellBuffer + offset);
             i != BREAK_SHARDS_PER_TARGET; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, FIXEDtoINT(shard->x),
                             FIXEDtoINT(shard->y), size);
            Particle_AdvanceWithGravity2D(shard, 62, gDrain_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 &&
                FIXEDtoINT(shard->y) > BREAK_SHARD_GROUND)
              shard->variant = -1;
          }
          shard++;
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define DRAIN_MOTES_PER_TARGET 128
#define DRAIN_DRAWN 32    ///< motes drawn for each target
#define DRAIN_MOTE_SIZE 6 ///< a mote's size in pixels
/// Drain runs DRAIN_TARGET_FRAMES a target and then this many; the user glows
/// from DRAIN_GLOW_FRAME until DRAIN_GLOW_END_FRAMES before the end.
#define DRAIN_END_FRAMES 72
#define DRAIN_GLOW_FRAME 40
#define DRAIN_GLOW_END_FRAMES 20
#define DRAIN_HIT_FRAME 64
/// A mote homes in on the user this many frames after its target's turn,
/// each mote a frame after the one before, gaining a 256th of the distance a
/// frame.
#define DRAIN_PULL_DELAY 10
#define DRAIN_PULL(distance) ((distance) >> 8)
/// A mote goes out within this many pixels of the middle of the user, across
/// and in depth.
#define DRAIN_ARRIVAL_DISTANCE 16

/// Motes of light burst from each target in turn, and are
/// pulled into the centre of the user's body.
///
/// Abilities: Drain, Psy Drain.
void BattleEffect_RunDrain(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  u8 *sheet;
  struct BattleCamera *camera;
  struct ScreenPoint screen;
  s32 alternate;
  s32 member;
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  if (args->variant == EFFECT_VARIANT_BASIC)
    alternate = 0;
  else
    alternate = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Iwram_CopyWords(BG_PLTT,
                  Resource_GetTableEntry(alternate == 0
                                             ? RESOURCE_ICE_BLOCK_SHEET
                                             : RESOURCE_BLAST_SHEET),
                  SHEET_PALETTE_BYTES);

  // The motes fly out of each target's body.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_MOTES_PER_TARGET);

  EFFECT_FOR_EACH_TARGET(member, work->args) {
    struct Actor *actor;
    s32 halfHeight;

    actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
    halfHeight = BattleUnit_GetEffectHeightFar(work->args->targets[member]) / 2;
    for (i = 0; i != DRAIN_MOTES_PER_TARGET; i++) {
      struct Particle *mote = &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

      SET_POSITION(mote, actor->x.fixed, actor->y.fixed + halfHeight,
                   actor->z.fixed);
      SET_VELOCITY(mote, ((Random16() & 0xff) - 128) << 10,
                   ((Random16() & 0xff) - 128) << 10,
                   ((Random16() & 0xff) - 128) << 10);
      mote->variant = 0;
    }
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != work->args->targetCount * DRAIN_TARGET_FRAMES +
                               DRAIN_END_FRAMES;
       frame++) {
    struct Actor *user;
    s32 halfHeight;

    user = GetBattleObjectSlotFar(work->args->user)->actor;
    halfHeight = BattleUnit_GetEffectHeightFar(work->args->user) / 2;
    if (frame == DRAIN_HIT_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == DRAIN_GLOW_FRAME)
      BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == work->args->targetCount * DRAIN_TARGET_FRAMES +
                     DRAIN_END_FRAMES - DRAIN_GLOW_END_FRAMES)
      BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      if (frame == member * DRAIN_TARGET_FRAMES)
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 42);
      if (frame > member * DRAIN_TARGET_FRAMES) {
        for (i = 0; i != DRAIN_DRAWN; i++) {
          struct Particle *mote =
              &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

          if (mote->variant >= 0) {
            s32 size = DRAIN_MOTE_SIZE;

            Projection_Point(mote, &screen);
            screen.x >>= 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x, screen.y, size);
            Particle_AdvanceWithGravity3D(mote, 62, 0);

            // After a moment a mote is pulled into the user.
            if (frame > member * DRAIN_TARGET_FRAMES + i + DRAIN_PULL_DELAY) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = DRAIN_PULL(user->x.fixed - mote->x);
              dy = DRAIN_PULL(user->y.fixed + halfHeight - mote->y);
              dz = DRAIN_PULL(user->z.fixed - mote->z);
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE)) &&
                   dx < DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE))) &&
                  (dz > -DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE)) &&
                   dz < DRAIN_PULL(toFIXED(DRAIN_ARRIVAL_DISTANCE))))
                mote->variant = -1;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef DRAIN_TARGET_FRAMES
#undef BREAK_SHARDS_PER_TARGET
#undef BREAK_FALL_FRAMES
#undef BREAK_HIT_FRAME
#undef BREAK_SHARD_GROUND
#undef DRAIN_MOTES_PER_TARGET
#undef DRAIN_DRAWN
#undef DRAIN_MOTE_SIZE
#undef DRAIN_END_FRAMES
#undef DRAIN_GLOW_FRAME
#undef DRAIN_GLOW_END_FRAMES
#undef DRAIN_HIT_FRAME
#undef DRAIN_PULL_DELAY
#undef DRAIN_PULL
#undef DRAIN_ARRIVAL_DISTANCE
