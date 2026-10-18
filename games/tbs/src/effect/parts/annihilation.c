// Part of effect.c, the battle effect object, compiled only through it.
// Annihilation.

#define ANNIHILATION_FRAMES 64 // frames in the effect

// Abilities: Annihilation (kind 55).
//
// The user closes on the first target and fire bursts from it in a shower of
// shards. The scene runs for 64 frames:
//
//   4 on       64 shards scatter from the target and fall
//   6 to 11    a fire streak flares in front of the user
//   8          the target is struck; the canvas is filled
//   16 to 47   a fire burst grows over the target in three steps
//
// effect: the user and the target; a nonzero variant gives the burst its
// second picture and a sound.
void BattleFx_RunAnnihilation(struct BattleEffectArgument *effect) {
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct MotionObject *targetObject;
  struct EffectStep *shard;
  struct EffectPosition position;
  struct EffectPosition origin;
  struct EffectPosition out;
  DrawRectangle blitters[2];
  s32 i;
  s32 frame;
  s32 size;
  u8 *burstPixels;

  heapCache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = heapCache;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  particleSheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(heapCache, HEAP_SLOT_CAMERA);
  burstPixels = Ram_MapCellBuffer;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burstPixels, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0],
                                 4, 0);
  WaitFrames(1);
  targetObject = GetBattleObjectSlotFar(work->effect->actors[0])->object;

  i = 0;
  do {
    // 64 shards start at the target, flying across towards the middle of
    // the field; each group of four lives two frames longer than the last.
    struct EffectStep *seed = &work->particles[i];

    SET_POSITION(seed, targetObject->x, targetObject->y, targetObject->z);
    SET_VELOCITY(seed, (Random16() & 255) << 11,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
    i++;
  } while (i != 64);

  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &origin);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == 8)
      BattleEventRuntime_BeginPhaseFar(0x86);
    if (work->effect->variant != 0 && frame == 8)
      AudioCommand_PlayFar(0xd4);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &position);
    // The fire streak, one picture a frame in front of the user, drawn
    // additively and mirrored for side 1 with a blitter made for this one
    // drawing.
    if (frame >= 6 && frame <= 11) {
      if (work->effect->side == 0)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      blitters[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
      if (work->effect->side == 0)
        blitters[0](canvas, work->sheet + (frame - 6) * 0xd80,
                    position.x / 2 - 24, position.y - 24, 48, 72);
      else
        blitters[0](canvas, work->sheet + (frame - 6) * 0xd80, position.x / 2,
                    position.y - 24, 48, 72);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    }
    // The fire burst over the target grows a step every two frames up to
    // the third; a nonzero variant takes the second strip of pictures.
    if (frame >= 16 && frame <= 47) {
      s32 step = (frame - 16) / 2;
      s32 strip;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
      blitters[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
      if (step > 2)
        step = 2;
      if (work->effect->variant == 0)
        strip = 0;
      else
        strip = 0x2580;
      blitters[0](canvas, Ram_MapCellBuffer + strip + step * 0xc80,
                  origin.x / 2 - 20, origin.y - 48, 40, 80);
      BattleFx_CallNoEffect(10000);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    }
    if (frame == 8) {
      Dma_Fill32(0x3f3f3f3f, canvas, 0x4000);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Shards shrink as their lives run down; pairs of shards alternate
    // between the normal and mirrored blitters.
    if (frame > 3) {
      BattleFx_FetchRectangleBlitters(work->effect->side, blitters);
      for (i = 0; i != 64; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          EffectPosition_ApplyBaseAndYOffset(shard, &out);
          out.x >>= 1;
          out.y = out.y + origin.y - 112;
          size = (life >> 3) + 2;
          blitters[(i / 2) & 1](
              canvas, particleSheet + gBattleFx_MoteCells[size - 1],
              out.x - size / 2, out.y - size, size, size * 2);
          EffectStep_AdvanceWithGravity3D(shard, 60, -0x400);
          shard->variant--;
        }
      }
      EFFECT_RELEASE_BLITTERS();
    }
    if (frame == 8) {
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
      work->shakeFrames = 4;
    }
    // The target glows red on frames 6 and 14.
    if (frame == 6)
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_RED_TINT,
                                -1, -1, 0);
    if (frame == 14)
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_RED_TINT,
                                -1, -1, 0);
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
