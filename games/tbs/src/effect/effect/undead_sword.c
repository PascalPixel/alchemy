// Part of effect.c, the battle effect object, compiled only through it.
// Undead Sword.

// Abilities: Undead Sword (kind 320).
//
// Two mirrored columns of light rise at the first target's feet and sink
// again, scattering 16 sparks that drift upward; the target is struck on
// frame 10. 54 frames.
void BattleFx_RunUndeadSword(struct BattleEffectArgument *effect) {
  void **heap;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *sprites;
  DrawRectangle draw[2];
  struct EffectPosition screenPosition;
  struct EffectStep *spark;
  struct EffectStep *particle;
  s32 frame;
  s32 i;
  s32 height;
  s32 mode;
  s32 x;
  u8 *src;
  s32 speed;
  s32 angle;
  s32 cell;
  s32 size;

  EFFECT_GET_SLOTS(heap, cursor, work, canvas);
  work->effect = effect;
  sprites = heap[2];
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = Q_8_8(1); // Normal horizontal scale too
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x460,
                             1, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  work->transferMode = CANVAS_COPY_AND_FADE; // 3/4 afterimage
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  // Shift so that canvas x = 64 lines up with the target.
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Scatter sparks in all directions from (64, 112), then drift upward.
  for (i = 0, spark = work->particles; i != 16; i++) {
    speed = (Random16() & 0x1ff) + 128;
    angle = Random16() & 0xffff;
    spark->x = 0x400000;
    spark->y = 0x700000;
    spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
    spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
    spark->variant = Random16() & 7;
    spark++;
  }
  work->shakeFrames = 8;
  for (frame = 0; frame != 54; frame++) {
    if (frame == 10) {
      work->shakeFrames = 8;
      BattleEventRuntime_BeginPhaseFar(212);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
    }

    // Columns grow 12 pixels per frame on frames 8-31, then shrink 6 per frame.
    // Use comparison compositing while held at 80 pixels, otherwise additive.
    if (frame > 7) {
      if (frame <= 31)
        height = frame * 12 - 96;
      else
        height = 272 - frame * 6;
      if (height > 0) {
        if (height > 80) {
          height = 80;
          mode = COMPOSITE_MAX;
        } else {
          mode = COMPOSITE_ADDITIVE;
        }
        for (i = 0, x = 50; i != 2; i++, x += 14) {
          if (i == 0)
            EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            EFFECT_LOAD_BLITTER(
                HEAP_SLOT_BLITTER,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          draw[0] = ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER]);
          ((DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER])(
              canvas, work, x, 112 - height, 14, height);
          Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
      }
    }

    // Two more sparks start each frame from frame 8; each advances to the
    // next of seven pictures every three frames and drifts upward.
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    for (i = 0, particle = work->particles; i != 16; i++) {
      if (frame >= i / 2 + 8 && particle->variant <= 28) {
        s32 px;
        s32 py;

        cell = particle->variant / 3;
        px = HI(particle->x);
        py = HI(particle->y);
        if (cell > 6)
          cell = 6;
        src = work->sheet + gUndeadSword_SparkCells[cell] + 0x460;
        size = gUndeadSword_SparkSizes[cell];
        draw[0](canvas, src, px - size / 2, py - size / 2, size, size);
        particle->variant++;
        EffectStep_AdvanceWithGravity2D(particle, 62, -0x2000);
      }
      particle++;
    }
    EFFECT_RELEASE_BLITTERS();
    if (frame <= 7)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
