/// @file
/// Break and Drain.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Break and Drain take the targets in turn, this many frames apart.
#define DRAIN_TARGET_FRAMES 20

#define BREAK_SHARDS_PER_TARGET 128
/// Break runs DRAIN_TARGET_FRAMES a target and then this many while the last
/// shards fall.
#define BREAK_FALL_FRAMES 56
#define BREAK_HIT_FRAME 32
/// The canvas row below which a falling shard goes out.
#define BREAK_SHARD_GROUND 112

/// What BattleEffect_RunDrainMotes draws, each named for the ability that plays
/// it.
enum DrainMotesMode {
  DRAIN_MOTES_DRAIN,       ///< white and pale blue motes with long trails
  DRAIN_MOTES_PSY_DRAIN,   ///< golden motes with long trails
  DRAIN_MOTES_TRUE_COLLIDE ///< large violet orbs, dark at the heart
};

/// Ice shards burst up from each target in turn, tinting it, and fall away.
///
/// Abilities: Break.
void BattleEffect_RunBreak(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 point[3];
  struct ScreenPoint screen;
  s32 frame;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET),
                       SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;

  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * BREAK_SHARDS_PER_TARGET);

  EFFECT_VIEW_FROM_CAMERA(camera);

  // Scatter the shards around the middle of each body.
  for (member = 0; member != args->targetCount; member++) {
    struct MotionObject *object;
    s32 height;

    object = BattleObject_GetFar(args->targets[member])->object;
    height = BattleUnit_GetEffectHeightFar(args->targets[member]) / 2;
    point[0] = object->x;
    point[1] = height;
    point[2] = object->z;
    EffectPosition_ProjectPoint(point, &screen);
    screen.x >>= 1;
    for (i = 0; i != BREAK_SHARDS_PER_TARGET; i++) {
      struct Particle *shard = &PARTICLES[member * BREAK_SHARDS_PER_TARGET + i];
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
    }
  }

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0;
       frame != args->targetCount * DRAIN_TARGET_FRAMES + BREAK_FALL_FRAMES;
       frame++) {
    if (frame == BREAK_HIT_FRAME)
      EFFECT_SHOW_DAMAGE(0);
    for (member = 0; member != args->targetCount; member++) {
      if (frame == member * DRAIN_TARGET_FRAMES) {
        Sound_PlayFar(SE_CRACK);
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, member, 20);
      }
      // A shard falls and goes out below the ground.
      if (frame > member * DRAIN_TARGET_FRAMES) {
        for (i = 0; i != BREAK_SHARDS_PER_TARGET; i++) {
          struct Particle *shard =
              &PARTICLES[member * BREAK_SHARDS_PER_TARGET + i];

          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, FIXED_INTEGER_PART(shard->x),
                             FIXED_INTEGER_PART(shard->y), size);
            Particle_AdvanceWithGravity2D(shard, 62, gDrain_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 &&
                FIXED_INTEGER_PART(shard->y) > BREAK_SHARD_GROUND)
              shard->variant = -1;
          }
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Motes of light burst from each target in turn, and are pulled into the
/// centre of the user's body: white for the basic variant, golden for the
/// stronger.
///
/// Abilities: Drain, Psy Drain.
void BattleEffect_RunDrain(struct BattleEffectParams *args) {
  s32 variant = args->variant;

  // Psy Drain's for any variant past the basic one.
  BattleEffect_RunDrainMotes(args, NONZERO(variant));
}

/// Large violet orbs burst from each target in turn, and are pulled into the
/// centre of the user's body.
///
/// Abilities: True Collide.
void BattleEffect_RunTrueCollide(struct BattleEffectParams *args) {
  BattleEffect_RunDrainMotes(args, DRAIN_MOTES_TRUE_COLLIDE);
}

#define DRAIN_MOTES_PER_TARGET 128
#define DRAIN_DRAWN 32 ///< motes drawn for each target
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
/// The red and blue of True Collide's first colour, a dim violet.
#define TRUE_COLLIDE_VIOLET 21

/// Motes burst out of each target in turn and are pulled into the centre of
/// the user's body, which glows white as they arrive (enum DrainMotesMode).
void BattleEffect_RunDrainMotes(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  struct ScreenPoint screen;
  s32 palette;
  s32 i;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (mode == DRAIN_MOTES_TRUE_COLLIDE)
    BattleEffect_DrawShadedCirclesMirrored(sheet, 10, FIXED_ONE);
  else
    Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  if (mode == DRAIN_MOTES_DRAIN)
    palette = RESOURCE_ICE_BLOCK_SHEET;
  else if (mode == DRAIN_MOTES_PSY_DRAIN)
    palette = RESOURCE_BLAST_SHEET;
  else
    palette = RESOURCE_VIOLET_PALETTE_C;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette),
                       SHEET_PALETTE_BYTES);

  // The motes fly out of each target's body.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_MOTES_PER_TARGET);

  for (member = 0; member != args->targetCount; member++) {
    struct MotionObject *object;
    s32 halfHeight;

    object = BattleObject_GetFar(args->targets[member])->object;
    halfHeight = BattleUnit_GetEffectHeightFar(args->targets[member]) / 2;
    for (i = 0; i != DRAIN_MOTES_PER_TARGET; i++) {
      struct Particle *mote = &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];
      s32 speed;

      SET_POSITION(mote, object->x, object->y + halfHeight, object->z);
      mote->velocityX = ((Random16() & 0xff) - 128) << 10;
      mote->velocityY = ((Random16() & 0xff) - 128) << 10;
      mote->velocityZ = (speed = (Random16() & 0xff) - 128) << 10;
      // True Collide's fly out twice as fast.
      if (mode == DRAIN_MOTES_TRUE_COLLIDE) {
        mote->velocityX <<= 1;
        mote->velocityY <<= 1;
        mote->velocityZ = speed * (toFIXED(1) / 32);
      }
      mote->variant = 0;
    }
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  if (mode == DRAIN_MOTES_TRUE_COLLIDE) {
    // Colours 1 to 63 run from violet down to black, so an orb is dark where
    // its level is highest; the canvas is blended 16:6 and trails.
    u16 *color = &BG_PLTT_COLOR(0, 1);

    for (i = 1; i != SHEET_PALETTE_COLORS; i++) {
      s32 red = TRUE_COLLIDE_VIOLET - (i + 1) / 3;

      *color++ = RGB(red, 0, TRUE_COLLIDE_VIOLET - i / 3);
    }
    REG_BLDALPHA = BLEND_ALPHA(16, 6);
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    // Each transfer lowers every level by 4, leaving a trail.
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  }
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0;
       frame != args->targetCount * DRAIN_TARGET_FRAMES + DRAIN_END_FRAMES;
       frame++) {
    struct MotionObject *user;
    s32 halfHeight;

    user = BattleObject_GetFar(args->user)->object;
    halfHeight = BattleUnit_GetEffectHeightFar(args->user) / 2;
    if (frame == DRAIN_HIT_FRAME)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == DRAIN_GLOW_FRAME)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == args->targetCount * DRAIN_TARGET_FRAMES + DRAIN_END_FRAMES -
                     DRAIN_GLOW_END_FRAMES)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);
    for (member = 0; member != args->targetCount; member++) {
      if (frame == member * DRAIN_TARGET_FRAMES)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, member, 42);
      if (frame > member * DRAIN_TARGET_FRAMES) {
        for (i = 0; i != DRAIN_DRAWN; i++) {
          struct Particle *mote =
              &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

          if (mote->variant >= 0) {
            u8 size = gDrainMotes_Sizes[mode];

            EffectPosition_ProjectPoint((s32 *)mote, &screen);
            screen.x >>= 1;
            if (mode == DRAIN_MOTES_TRUE_COLLIDE) {
              draw(canvas, sheet, screen.x - size / 2, screen.y - size, size,
                   size * 2);
              Particle_AdvanceWithGravity3D(mote, 61, 0);
            } else {
              EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x, screen.y, size);
              Particle_AdvanceWithGravity3D(mote, 62, 0);
            }

            // After a moment a mote is pulled into the user.
            if (frame > member * DRAIN_TARGET_FRAMES + i + DRAIN_PULL_DELAY) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = DRAIN_PULL(user->x - mote->x);
              dy = DRAIN_PULL(user->y + halfHeight - mote->y);
              dz = DRAIN_PULL(user->z - mote->z);
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
    EFFECT_END_FRAME(state);
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
#undef DRAIN_END_FRAMES
#undef DRAIN_GLOW_FRAME
#undef DRAIN_GLOW_END_FRAMES
#undef DRAIN_HIT_FRAME
#undef DRAIN_PULL_DELAY
#undef DRAIN_PULL
#undef DRAIN_ARRIVAL_DISTANCE
#undef TRUE_COLLIDE_VIOLET
