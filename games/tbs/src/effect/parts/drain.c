// Part of effect.c, the battle effect object, compiled only through it.
// Break and Drain.

// Break: ice shards burst from each target.
#define ICE_SHARDS_PER_TARGET 128 // shards for each target

// Abilities: Break (kind 43).
//
// Ice shards burst up from each target in turn, 20 frames apart, tinting
// the target as they go, and fall away below line 112.
// effect: the targets.
void BattleFx_RunBreak(struct BattleEffectArgument *effect) {
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
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);

  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * ICE_SHARDS_PER_TARGET);

  EFFECT_VIEW_FROM_CAMERA(camera);

  // Scatter the shards around the middle of each body; they spread sideways
  // and jump up.
  EFFECT_FOR_EACH_TARGET(member, work->effect) {
    struct MotionObject *object;
    s32 offset;
    s32 height;
    struct EffectStep *shard;

    offset = member * ICE_SHARDS_PER_TARGET * sizeof(struct EffectStep);
    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
    height = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
    SET_VECTOR(point, object->x, height, object->z);
    EffectPosition_ApplyBaseAndYOffset(point, &screen);
    screen.x >>= 1;
    for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != ICE_SHARDS_PER_TARGET; i++) {
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

  for (frame = 0; frame != work->effect->count * 20 + 56; frame++) {
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(0);
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      s32 offset;

      offset = member * ICE_SHARDS_PER_TARGET * sizeof(struct EffectStep);
      if (frame == member * 20) {
        AudioCommand_PlayFar(SE_143);
        ObjectGroup_UpdateMembers(work->effect->actors[member],
                                  SPRITE_COLOR_WHITE, -1, member, 20);
      }
      // A shard that falls back past y = 112 goes out.
      if (frame > member * 20) {
        struct EffectStep *shard;

        for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             i != ICE_SHARDS_PER_TARGET; i++) {
          if (shard->variant >= 0) {
            s32 size;

            size = i % 3 + 1;
            EFFECT_DRAW_MOTE(draw, canvas, sheet, HI(shard->x), HI(shard->y),
                             size);
            EffectStep_AdvanceWithGravity2D(shard, 62, gBreak_Gravities[i & 3]);
            shard->variant++;
            if (shard->velocityY > 0 && HI(shard->y) > 112)
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

// Gathering motes: motes of light leave the targets and are drawn into the
// user.

#define GATHERING_MOTES_PER_TARGET 128 // motes for each target
#define DRAIN_DRAWN 32                 // motes drawn for each target

// Abilities: Drain, Psy Drain (kind 42).
//
// Drain: motes of light burst from each target in turn, 20 frames apart,
// and are pulled into the centre of the user's body. Each target flashes
// in its damage pose for 42 frames as its motes burst, and the user glows
// from frame 40 until 20 frames before the end. A negative variant marks
// a mote that has gone out.
//
// effect: the targets; variant 0 takes the ice block palette, any other
//         the blast palette.
void BattleFx_RunDrain(struct BattleEffectArgument *effect) {
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
  camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
  if (effect->variant == 0)
    alternate = 0;
  else
    alternate = 1;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(alternate == 0
                                             ? (s32)&ResourceId_IceBlockSheet
                                             : (s32)&ResourceId_BlastSheet),
                  128);

  // The motes fly out of the centre of each target's body at random speeds
  // of up to 2 pixels a frame on each axis.
  EFFECT_FREE_PARTICLES(i, PARTICLES, 8 * GATHERING_MOTES_PER_TARGET);

  EFFECT_FOR_EACH_TARGET(member, work->effect) {
    struct MotionObject *object;
    s32 halfHeight;

    object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
    halfHeight =
        Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
    for (i = 0; i != GATHERING_MOTES_PER_TARGET; i++) {
      struct EffectStep *mote =
          &PARTICLES[member * GATHERING_MOTES_PER_TARGET + i];

      SET_POSITION(mote, object->x, object->y + halfHeight, object->z);
      SET_VELOCITY(mote, ((Random16() & 0xff) - 128) << 10,
                   ((Random16() & 0xff) - 128) << 10,
                   ((Random16() & 0xff) - 128) << 10);
      mote->variant = 0;
    }
  }

  // Draw with the maximum blend, and lower every canvas pixel by 4 on each
  // transfer so the motes leave fading trails.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x04040404);
  AudioCommand_PlayFar(SE_142);

  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 halfHeight;

    user = GetBattleObjectSlotFar(work->effect->actor)->object;
    halfHeight = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
    if (frame == 64)
      BattleEventRuntime_BeginPhaseFar(SE_133);
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
              &PARTICLES[member * GATHERING_MOTES_PER_TARGET + i];

          if (mote->variant >= 0) {
            s32 size;

            // The canvas is shown at double width, so halve x.
            EffectPosition_ApplyBaseAndYOffset(mote, &screen);
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
