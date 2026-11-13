/// @file
/// Break and Drain.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define DRAIN_ICE_SHARDS_PER_TARGET 128

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
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET), 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_ICE_SHARDS_PER_TARGET);

  EFFECT_VIEW_FROM_CAMERA(camera);

  // Scatter the shards around the middle of each body.
  EFFECT_FOR_EACH_TARGET(member, work->args) {
    struct Actor *actor;
    s32 offset;
    s32 height;
    struct Particle *shard;

    offset = member * DRAIN_ICE_SHARDS_PER_TARGET * sizeof(struct Particle);
    actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
    height = BattleUnit_GetEffectHeightFar(work->args->targets[member]) / 2;
    SET_VECTOR(point, actor->x.fixed, height, actor->z.fixed);
    Projection_Point(point, &screen);
    screen.x >>= 1;
    for (i = 0, shard = (struct Particle *)(Ram_MapGridCellBuffer + offset);
         i != DRAIN_ICE_SHARDS_PER_TARGET; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & TRIG_ANGLE_MASK;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + px(screen.x);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + px(screen.y);
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != work->args->targetCount * 20 + 56; frame++) {
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      s32 offset;

      offset = member * DRAIN_ICE_SHARDS_PER_TARGET * sizeof(struct Particle);
      if (frame == member * 20) {
        Sound_PlayFar(SE_CRACK);
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, TINT_KEEP, member, 20);
      }
      // A shard falls and goes out below the ground.
      if (frame > member * 20) {
        struct Particle *shard;

        for (i = 0, shard = (struct Particle *)(Ram_MapGridCellBuffer + offset);
             i != DRAIN_ICE_SHARDS_PER_TARGET; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, Q_16_16_TO_INT(shard->x),
                             Q_16_16_TO_INT(shard->y), size);
            Particle_AdvanceWithGravity2D(shard, 62, gDrain_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && Q_16_16_TO_INT(shard->y) > 112)
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
#define DRAIN_DRAWN 32 ///< motes drawn for each target

/// Motes of light burst from each target in turn, 20 frames apart, and are
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
  camera = ((struct BattleCamera *)HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA));
  if (args->variant == EFFECT_VARIANT_BASIC)
    alternate = 0;
  else
    alternate = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(alternate == 0
                                             ? RESOURCE_ICE_BLOCK_SHEET
                                             : RESOURCE_BLAST_SHEET),
                  128);

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

  for (frame = 0; frame != work->args->targetCount * 20 + 72; frame++) {
    struct Actor *user;
    s32 halfHeight;

    user = GetBattleObjectSlotFar(work->args->user)->actor;
    halfHeight = BattleUnit_GetEffectHeightFar(work->args->user) / 2;
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 40)
      BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == work->args->targetCount * 20 + 52)
      BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      if (frame == member * 20)
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 42);
      if (frame > member * 20) {
        for (i = 0; i != DRAIN_DRAWN; i++) {
          struct Particle *mote =
              &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

          if (mote->variant >= 0) {
            s32 size = 6;

            Projection_Point(mote, &screen);
            screen.x >>= 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x, screen.y, size);
            Particle_AdvanceWithGravity3D(mote, 62, 0);

            // After a moment a mote is pulled into the user.
            if (frame > member * 20 + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x.fixed - mote->x) >> 8;
              dy = (user->y.fixed + halfHeight - mote->y) >> 8;
              dz = (user->z.fixed - mote->z) >> 8;
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -4096 && dx < 4096) && (dz > -4096 && dz < 4096))
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

#undef DRAIN_ICE_SHARDS_PER_TARGET
#undef DRAIN_MOTES_PER_TARGET
#undef DRAIN_DRAWN
