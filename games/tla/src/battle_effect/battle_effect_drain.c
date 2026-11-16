/// @file
/// Break and Drain.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

// Break's ice shards.

#define SHARDS_PER_MEMBER 128
#define SHARDS ((struct Particle *)Ram_MapCellBuffer)

/// Break and Drain take the targets in turn, this many frames apart.
#define DRAIN_TARGET_FRAMES 20

/// Ice shards burst up from each target in turn, 20 frames apart.
/// Setup: 128 shards per target are scattered around the middle of its body
/// and thrown sideways and up. Frame loop: from its cue a target flashes, and
/// its shards fall under one of four gravities until they drop below y = 112.
/// The effect runs 20 frames per target plus 56.
void BattleEffect_RunBreak(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 point[3];
  struct ScreenPoint screen;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET),
                       SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;

  for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
    SHARDS[i].variant = -1;

  Matrix_Initialize();
  Matrix_SetLookAt(camera->eye, camera->focus);

  // Scatter each target's shards around the middle of its body.
  for (member = 0; member != args->targetCount; member++) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct Particle *shard;
    struct ScreenPoint *position;

    offset = member * SHARDS_PER_MEMBER * sizeof(struct Particle);
    /* FAKEMATCH(temporary): position holds &screen. */
    position = &screen;
    object = BattleObject_GetFar(args->targets[member])->object;
    height = BattleUnit_GetEffectHeightFar(args->targets[member]) / 2;
    point[0] = object->x;
    point[1] = height;
    point[2] = object->z;
    EffectPosition_ProjectPoint(point, position);
    screen.x >>= 1;
    for (i = 0, shard = (struct Particle *)(Ram_MapCellBuffer + offset);
         i != SHARDS_PER_MEMBER; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & TRIG_ANGLE_MASK;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + INTtoFIXED(screen.x);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + INTtoFIXED(screen.y);
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != args->targetCount * DRAIN_TARGET_FRAMES + 56;
       frame++) {
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(0);
    for (member = 0; member != args->targetCount; member++) {
      s32 offset;

      offset = member * SHARDS_PER_MEMBER * sizeof(struct Particle);
      // The target's cue: a chime and a flash.
      if (frame == member * DRAIN_TARGET_FRAMES) {
        Sound_PlayFar(SE_CRACK);
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, member, 20);
      }
      // Its shards fly, and go out once they fall past y = 112.
      if (frame > member * DRAIN_TARGET_FRAMES) {
        struct Particle *shard;

        for (i = 0, shard = (struct Particle *)(Ram_MapCellBuffer + offset);
             i != SHARDS_PER_MEMBER; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            draw(canvas, sheet + gBattleEffect_MotePictures[size - 1],
                 FIXED_INTEGER_PART(shard->x) - size / 2,
                 FIXED_INTEGER_PART(shard->y) - size, size, size * 2);
            Particle_AdvanceWithGravity2D(shard, 62, gDrain_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && FIXED_INTEGER_PART(shard->y) > 112)
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

/// Motes of light burst from each target in turn and are pulled into the user,
/// more of them for the stronger variant.
///
/// Abilities: Drain, Psy Drain.
void BattleEffect_RunDrain(struct BattleEffectParams *args) {
  s32 variant = args->variant;

  // 1 for any variant past the basic one, as the sign of -variant | variant.
  BattleEffect_RunDrainMotes(args, NONZERO(variant));
}

/// Motes of light burst from each target in turn and are pulled into the user,
/// in True Collide's own style.
///
/// Abilities: True Collide.
void BattleEffect_RunTrueCollide(struct BattleEffectParams *args) {
  BattleEffect_RunDrainMotes(args, 2);
}

#define MOTES_PER_MEMBER 128
#define MOTES ((struct Particle *)Ram_MapCellBuffer)

#define DRAIN_DRAWN 32 ///< motes drawn for each target

/// Motes of light burst out of each target in turn, 20 frames apart, and
/// are drawn into the user.
/// Setup: each target's 128 motes start at the middle of its body with
/// random velocities; the canvas darkens by 4 a transfer (in mode 2 it
/// trails instead). Frame loop: a target flashes as its motes leave; ten
/// frames after leaving (one frame later for each mote) a mote accelerates
/// toward the middle of the user by 1/256 of the distance a frame and goes
/// out within 16 pixels of it across. The user glows from frame 40 until 20
/// frames before the end.
void BattleEffect_RunDrainMotes(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  struct ScreenPoint screen;
  s32 palette;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (mode == 2)
    BattleEffect_DrawShadedCirclesMirrored(sheet, 10, FIXED_ONE);
  else
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  if (mode == 0)
    palette = RESOURCE_ICE_BLOCK_SHEET;
  else if (mode == 1)
    palette = RESOURCE_BLAST_SHEET;
  else
    palette = RESOURCE_VIOLET_PALETTE_C;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette),
                       SHEET_PALETTE_BYTES);

  // The motes leave the middle of each target's body.
  for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
    MOTES[i].variant = -1;

  for (member = 0; member != args->targetCount; member++) {
    struct MotionObject *object;
    s32 half;

    object = BattleObject_GetFar(args->targets[member])->object;
    half = BattleUnit_GetEffectHeightFar(args->targets[member]) / 2;
    for (i = 0; i != MOTES_PER_MEMBER; i++) {
      struct Particle *mote = &MOTES[member * MOTES_PER_MEMBER + i];
      s32 speed;

      SET_POSITION(mote, object->x, object->y + half, object->z);
      mote->velocityX = ((Random16() & 0xff) - 128) << 10;
      mote->velocityY = ((Random16() & 0xff) - 128) << 10;
      mote->velocityZ = (speed = (Random16() & 0xff) - 128) << 10;
      if (mode == 2) {
        mote->velocityX <<= 1;
        mote->velocityY <<= 1;
        mote->velocityZ = speed << 11;
      }
      mote->variant = 0;
    }
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  if (mode == 2) {
    // A violet ramp from color 1, blended 16:6, with a trail.
    u16 *color = &BG_PLTT_COLOR(0, 1);

    for (i = 1; i != 64; i++) {
      s32 blue = 21 - (i + 1) / 3;

      *color++ = ((21 - i / 3) << 10) | blue;
    }
    REG_BLDALPHA = BLEND_ALPHA(16, 6);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    // Each transfer lowers the canvas's pixels by 4: an afterimage.
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != args->targetCount * DRAIN_TARGET_FRAMES + 72;
       frame++) {
    struct MotionObject *user;
    s32 half;

    user = BattleObject_GetFar(args->user)->object;
    half = BattleUnit_GetEffectHeightFar(args->user) / 2;
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    if (frame == 40)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == args->targetCount * DRAIN_TARGET_FRAMES + 52)
      BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);
    for (member = 0; member != args->targetCount; member++) {
      if (frame == member * DRAIN_TARGET_FRAMES)
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, member, 42);
      if (frame > member * DRAIN_TARGET_FRAMES) {
        for (i = 0; i != DRAIN_DRAWN; i++) {
          struct Particle *mote = &MOTES[member * MOTES_PER_MEMBER + i];

          if (mote->variant >= 0) {
            u8 size = gDrainMotes_Sizes[mode];

            // The canvas shows at twice the width: halve x.
            EffectPosition_ProjectPoint((s32 *)mote, &screen);
            screen.x >>= 1;
            if (mode == 2) {
              draw(canvas, sheet, screen.x - size / 2, screen.y - size, size,
                   size * 2);
              Particle_AdvanceWithGravity3D(mote, 61, 0);
            } else {
              draw(canvas, sheet + gBattleEffect_MotePictures[size - 1],
                   screen.x - size / 2, screen.y - size, size, size * 2);
              Particle_AdvanceWithGravity3D(mote, 62, 0);
            }

            // Home in on the user once ten frames out.
            if (frame > member * DRAIN_TARGET_FRAMES + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x - mote->x) >> 8;
              dy = (user->y + half - mote->y) >> 8;
              dz = (user->z - mote->z) >> 8;
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -toFIXED(1) / 16 && dx < toFIXED(1) / 16) &&
                  (dz > -toFIXED(1) / 16 && dz < toFIXED(1) / 16))
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
