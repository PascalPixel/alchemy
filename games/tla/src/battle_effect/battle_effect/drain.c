/// @file
/// Break and Drain.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "macros.h"

// Break's ice shards (effect kind 43).

#include "battle/battle_action.h"

#include "field/effect_step.h"

#include "system/heap_release.h"

#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

#define SHARDS_PER_MEMBER 128
#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)

/// Ice shards burst up from each target in turn, 20 frames apart.
/// Setup: 128 shards per target are scattered around the middle of its body
/// and thrown sideways and up. Frame loop: from its cue a target flashes, and
/// its shards fall under one of four gravities until they drop below y = 112.
/// The effect runs 20 frames per target plus 56.
void BattleEffect_RunBreak(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  s32 *camera;
  s32 point[3];
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                       128);
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
    SHARDS[i].variant = -1;

  Transform_Reset();
  Transform_SetLookAt(camera, camera + 3);

  // Scatter each target's shards around the middle of its body.
  for (member = 0; member != effect->count; member++) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct EffectStep *shard;
    struct EffectPosition *position;

    offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
    /* FAKEMATCH(temporary): position holds &screen. */
    position = &screen;
    object = BattleObject_GetFar(effect->actors[member])->object;
    height = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
    point[0] = object->x;
    point[1] = height;
    point[2] = object->z;
    EffectPosition_ProjectPoint(point, position);
    screen.x >>= 1;
    for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != SHARDS_PER_MEMBER; i++) {
      s32 radius;
      s32 angle;
      s32 lift;

      radius = Random16() & 0xff;
      angle = Random16() & 0xffff;
      shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
      shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
      shard->velocityX = (128 - (Random16() & 0xff)) << 9;
      lift = Random16() & 0xff;
      shard->variant = 0;
      shard->velocityY = (-lift - 128) << 10;
      shard++;
    }
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 20 + 56; frame++) {
    if (frame == 32)
      BattlePlayback_StartFar(0);
    for (member = 0; member != effect->count; member++) {
      s32 offset;

      offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
      // The target's cue: a chime and a flash.
      if (frame == member * 20) {
        Sound_PlayFar(SE_CRACK);
        BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 20);
      }
      // Its shards fly, and go out once they fall past y = 112.
      if (frame > member * 20) {
        struct EffectStep *shard;

        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != SHARDS_PER_MEMBER; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            draw(canvas, sheet + gBattleEffect_MoteCells[size - 1],
                 HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(shard, 62,
                                                 gDrain_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && HI(shard->y) > 112)
              shard->variant = -1;
          }
          shard++;
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Drain, Psy Drain (kind 42).
void BattleEffect_RunDrain(u8 *effect) {
  s32 value = *(s32 *)(effect + 24);

  BattleEffect_RunDrainMotes(effect, (u32)(-value | value) >> 31);
}

/// Abilities: True Collide (kind 451).
void BattleEffect_RunTrueCollide(u8 *effect) {
  BattleEffect_RunDrainMotes(effect, 2);
}

#define MOTES_PER_MEMBER 128
#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)

/// Motes of light burst out of each target in turn, 20 frames apart, and
/// are drawn into the user, as The Broken Seal's BattleEffect_RunDrain.
/// Setup: each target's 128 motes start at the middle of its body with
/// random velocities; the canvas darkens by 4 a transfer (in mode 2 it
/// trails instead). Frame loop: a target flashes as its motes leave; ten
/// frames after leaving (one frame later for each mote) a mote accelerates
/// toward the middle of the user by 1/256 of the distance a frame and goes
/// out within 16 pixels of it across. The user glows from frame 40 until 20
/// frames before the end.
void BattleEffect_RunDrainMotes(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  struct EffectPosition screen;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  if (mode == 2)
    BattleEffect_DrawShadedCirclesMirrored(sheet, 10, 0x10000);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 0)
    palette = (s32)&ResourceId_IceBlockSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_BlastSheet;
  else
    palette = (s32)&ResourceId_VioletPaletteC;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);

  // The motes leave the middle of each target's body.
  for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
    MOTES[i].variant = -1;

  for (member = 0; member != effect->count; member++) {
    struct MotionObject *object;
    s32 half;

    object = BattleObject_GetFar(effect->actors[member])->object;
    half = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
    for (i = 0; i != MOTES_PER_MEMBER; i++) {
      struct EffectStep *mote = &MOTES[member * MOTES_PER_MEMBER + i];
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

  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 2) {
    // A violet ramp from color 1, blended 16:6, with a trail.
    u16 *color = BG_PLTT + 1;

    for (i = 1; i != 64; i++) {
      s32 blue = 21 - (i + 1) / 3;

      *color++ = ((21 - i / 3) << 10) | blue;
    }
    REG_BLDALPHA = 0x610;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    // Each transfer lowers the canvas's pixels by 4: an afterimage.
    work->transferMode = 3;
    work->transferValue = 0x04040404;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(SE_SWIRL);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 half;

    user = BattleObject_GetFar(effect->actor)->object;
    half = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
    if (frame == 64)
      BattlePlayback_StartFar(133);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 40)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, 0);
    if (frame == effect->count * 20 + 52)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, 0);
    for (member = 0; member != effect->count; member++) {
      if (frame == member * 20)
        BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 42);
      if (frame > member * 20) {
        for (i = 0; i != 32; i++) {
          struct EffectStep *mote = &MOTES[member * MOTES_PER_MEMBER + i];

          if (mote->variant >= 0) {
            u8 size = Data_081982f4[mode];

            // The canvas shows at twice the width: halve x.
            EffectPosition_ProjectPoint((s32 *)mote, &screen);
            screen.x >>= 1;
            if (mode == 2) {
              draw(canvas, sheet, screen.x - size / 2, screen.y - size, size,
                   size * 2);
              BattleEffectKernels_IntegrateVector3(mote, 61, 0);
            } else {
              draw(canvas, sheet + gBattleEffect_MoteCells[size - 1],
                   screen.x - size / 2, screen.y - size, size, size * 2);
              BattleEffectKernels_IntegrateVector3(mote, 62, 0);
            }

            // Home in on the user once ten frames out.
            if (frame > member * 20 + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x - mote->x) >> 8;
              dy = (user->y + half - mote->y) >> 8;
              dz = (user->z - mote->z) >> 8;
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -0x1000 && dx < 0x1000) &&
                  (dz > -0x1000 && dz < 0x1000))
                mote->variant = -1;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
