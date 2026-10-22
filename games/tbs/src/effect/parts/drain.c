/// @file
/// Break and Drain.
// Part of effect.c.

#include "../effect_internal.h"

#define DRAIN_ICE_SHARDS_PER_TARGET 128 ///< shards for each target

/// Ice shards burst up from each target in turn, 20 frames apart, tinting the
/// target as they go, and fall away below line 112.
///
/// Abilities: Break (BATTLE_EFFECT_BREAK).
///
/// Target n's 128 shards are PARTICLES n * 128 onwards, screen positions on
/// the half-width canvas in 16.16; a shard's variant counts its frames and is
/// -1 once it has gone. Loads the ice block palette into the first 64 colours
/// of the background palette.
void BattleEffect_RunBreak(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 frame;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleEffect_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);

  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_ICE_SHARDS_PER_TARGET);

  EFFECT_VIEW_FROM_CAMERA(camera);

  // Setup: scatter the shards around the middle of each body, a narrow
  // ellipse up to 2 pixels across the half-width canvas and 32 up and down;
  // each moves up to 1 pixel a frame sideways and jumps up at 2 to 6.
  EFFECT_FOR_EACH_TARGET(member, work->effect) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct EffectStep *shard;

    offset = member * DRAIN_ICE_SHARDS_PER_TARGET * sizeof(struct EffectStep);
    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
    height = BattleUnit_GetEffectHeightFar(work->effect->actors[member]) / 2;
    SET_VECTOR(point, object->x, height, object->z);
    EffectPosition_ProjectPoint(point, &screen);
    screen.x >>= 1;
    for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != DRAIN_ICE_SHARDS_PER_TARGET; i++) {
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

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Target n starts on frame n * 20 with a sound and a 20-frame tint; the
  // last target's shards get 56 frames to fall. The battle shows the damage
  // on frame 32, whatever the number of targets.
  for (frame = 0; frame != work->effect->count * 20 + 56; frame++) {
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(0);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      s32 offset;

      offset = member * DRAIN_ICE_SHARDS_PER_TARGET * sizeof(struct EffectStep);
      if (frame == member * 20) {
        AudioCommand_PlayFar(SE_143);
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 20);
      }
      // A shard falls with one of four gravities by its index (1/8 to 1/2
      // pixel a frame per frame, gDrain_Gravities) and goes out once it
      // falls back past y = 112. Its size, 1 to 3, also follows its index.
      if (frame > member * 20) {
        struct EffectStep *shard;

        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != DRAIN_ICE_SHARDS_PER_TARGET; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, Q_16_16_TO_INT(shard->x),
                             Q_16_16_TO_INT(shard->y), size);
            EffectStep_AdvanceWithGravity2D(shard, 62, gDrain_Gravities[i & 3]);
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

#define DRAIN_MOTES_PER_TARGET 128 ///< motes for each target
#define DRAIN_DRAWN 32             ///< motes drawn for each target

/// Motes of light burst from each target in turn, 20 frames apart, and are
/// pulled into the centre of the user's body.
///
/// Abilities: Drain, Psy Drain (BATTLE_EFFECT_DRAIN).
///
/// Each target flashes in its damage pose for 42 frames as its motes burst, and
/// the user glows from frame 40 until 20 frames before the end. Target n's
/// motes are PARTICLES n * 128 onwards, battle positions in 16.16; only the
/// first 32 of each 128 are moved and drawn. A negative variant marks a mote
/// that has gone out.
///
/// effect: the targets; Drain's basic variant takes the ice block palette,
///         any stronger one (Psy Drain's) the blast palette.
void BattleEffect_RunDrain(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  u8 *sheet;
  struct BattleCamera *camera;
  struct EffectPosition screen;
  s32 alternate;
  s32 member;
  s32 frame;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = ((struct BattleCamera *)WORK_SLOT(HEAP_SLOT_CAMERA));
  if (effect->variant == EFFECT_VARIANT_BASIC)
    alternate = 0;
  else
    alternate = 1;
  work->effect = effect;
  BattleEffect_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(alternate == 0
                                             ? (s32)&ResourceId_IceBlockSheet
                                             : (s32)&ResourceId_BlastSheet),
                  128);

  // The motes fly out of the centre of each target's body at random speeds
  // of up to 2 pixels a frame on each axis.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * DRAIN_MOTES_PER_TARGET);

  EFFECT_FOR_EACH_TARGET(member, work->effect) {
    struct MotionObject *object;
    s32 halfHeight;

    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
    halfHeight =
        BattleUnit_GetEffectHeightFar(work->effect->actors[member]) / 2;
    for (i = 0; i != DRAIN_MOTES_PER_TARGET; i++) {
      struct EffectStep *mote = &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

      SET_POSITION(mote, object->x, object->y + halfHeight, object->z);
      SET_VELOCITY(mote, ((Random16() & 0xff) - 128) << 10,
                   ((Random16() & 0xff) - 128) << 10,
                   ((Random16() & 0xff) - 128) << 10);
      mote->variant = 0;
    }
  }

  // Draw with the maximum blend, and lower every canvas pixel by 4 on each
  // transfer so the motes leave fading trails.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x04040404);
  AudioCommand_PlayFar(SE_142);

  // Frame 0 plays the drain sound; target n bursts on frame n * 20; frame 40
  // starts the user's glow and frame 64 shows the damage. The last target's
  // motes get 72 frames to reach the user.
  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 halfHeight;

    user = GetBattleObjectSlotFar(work->effect->actor)->object;
    halfHeight = BattleUnit_GetEffectHeightFar(work->effect->actor) / 2;
    if (frame == 64)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 40)
      ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1, -1,
                                0);
    if (frame == work->effect->count * 20 + 52)
      ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      if (frame == member * 20)
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 42);
      if (frame > member * 20) {
        for (i = 0; i != DRAIN_DRAWN; i++) {
          struct EffectStep *mote =
              &PARTICLES[member * DRAIN_MOTES_PER_TARGET + i];

          if (mote->variant >= 0) {
            s32 size;

            // The canvas is shown at double width, so halve x.
            EffectPosition_ProjectPoint(mote, &screen);
            screen.x >>= 1;
            size = 6;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, screen.x, screen.y, size);
            EffectStep_AdvanceWithGravity3D(mote, 62, 0);

            // 10 frames after it appears (one frame later for each mote), a
            // mote accelerates towards the user by 1/256 of the distance, and
            // goes out once it is within 16 pixels horizontally.
            if (frame > member * 20 + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x - mote->x) >> 8;
              dy = (user->y + halfHeight - mote->y) >> 8;
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
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef DRAIN_ICE_SHARDS_PER_TARGET
#undef DRAIN_MOTES_PER_TARGET
#undef DRAIN_DRAWN
