// Part of effect.c, the battle effect object, compiled only through it.
// Swirling stars, orbiting skulls and target bursts.

// - The swirling stars spin 64 stars about a point above the field, each on
//   its own axis, then pull them back to the centre.
// - The orbiting skulls (BattleFx_RunOrbitingSkulls) circle each target in
//   turn, three to a target, their angles kept in the particles' velocityX.
// - The target bursts flare up on each target while five puffs of smoke
//   scatter around each flare; six particle slots per target.

#define SWIRLING_STARS_FRAMES 160
#define SWIRLING_STARS_COUNT 64 // stars that swirl

// Swirling stars: 64 stars scattered around a point 80 pixels up spin
// about it, each on its own axis, appearing four a frame; 30 frames after
// it appears, each star is drawn back towards the centre. Stars are drawn
// smaller the further away they are. The scene runs for 160 frames. All
// 256 stars of the pool are seeded, at rest in a cube 255 pixels across,
// though only 64 are drawn.
void BattleFx_RunSwirlingStars(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 center[3];
  struct EffectPosition screen;
  BlitterPair draw;
  struct EffectStep *star;
  s32 i;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
  BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw);
  for (i = 0; i != 256; i++) {
    star = &PARTICLES[i];
    SET_POSITION(star, ((Random16() & 0xff) - 127) << 16,
                 ((Random16() & 0xff) - 127) << 16,
                 ((Random16() & 0xff) - 127) << 16);
    SET_VELOCITY(star, 0, 0, 0);
    star->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  SET_VECTOR(center, 0, 160 << 15, 0);
  for (frame = 0; frame != SWIRLING_STARS_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_VIEW_FROM_CAMERA(camera);
    SceneTransform_ApplyPosition(center);
    star = PARTICLES;
    for (i = 0; i != SWIRLING_STARS_COUNT; i++, star++) {
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        // Turn the star about one of four axes, picked by its index, faster
        // for later stars.
        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i + 32) * 8);
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
          break;
        case 2:
          SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
          SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        }
        EffectPosition_ProjectPoint(star, &screen);
        screen.x >>= 1;
        Transform_Pop();
        CLAMP(screen.depth, 250, 0x27a);
        size = 9 - (screen.depth - 250) / 64;
        draw[0](canvas,
                work->sheet +
                    ((i & 3) * MOTE_SET_BYTES + gBattleFx_MoteCells[size - 1]),
                screen.x - size / 2, screen.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(star, 60, 0);
        if (frame > i / 4 + 30) {
          s32 dx = -star->x >> 8;
          s32 dy = -star->y >> 8;
          s32 dz = -star->z >> 8;

          star->velocityX += dx;
          star->velocityY += dy;
          star->velocityZ += dz;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

// Orbiting skulls: three skulls circle each target in turn, 16 frames
// apart, for 64 frames each, a third of a turn apart on a circle of 8
// pixels around the target. Runs with the effect already in
// gBattleFxWork.
void BattleFx_RunOrbitingSkulls(void) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 point[3];
  struct EffectPosition center;
  struct EffectPosition screen;
  s32 frame;
  /* FAKEMATCH: only draw[0] is used: the second word fills
   * four bytes of the frame */
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  u8 *skulls;
  s32 xOffset;
  s32 i;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  EFFECT_BEGIN_NARROW_CANVAS(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  skulls = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, skulls, 128);
  Resource_DecodeType01(skulls + 128, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  // The first transfer mode is overwritten at once.
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  draw[0] = (DrawRectangle)WORK_SLOT(HEAP_SLOT_BLITTER);
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  // Put the canvas on the targets' side of the screen.
  if (work->effect->side == 0)
    xOffset = 0;
  else
    xOffset = -112;
  /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the
   * seed counter is set before its pointer 2026-10-11: without it differ 2
   * (movs r4, #0 falls behind the particle offset); i = 0 ahead of the store
   * differs 170. */
  do {
    REG_BG2X = xOffset << 8;
  } while (0);
  // velocityX holds each skull's angle on its orbit.
  for (i = 0; i != 64; i++) {
    struct EffectStep *step = &work->particles[i];

    step->x = 0;
    step->y = 0;
    step->velocityX = 0;
    step->z = 4;
  }
  for (frame = 0; frame != work->effect->count * 16 + 64; frame++) {
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *object;
      u32 memberFrame;

      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      EffectPosition_ProjectScaledBattler(work->effect->actors[member],
                                          &screen);
      memberFrame = frame - member * 16;
      screen.x += xOffset;
      // Unsigned, so the frames before this target's turn are skipped too.
      if (memberFrame < 64) {
        /* FAKEMATCH: pins rec to r0 so the point is filled through r0 and
         * loop.c cannot hoist its address into fp, as native; plain differs 118
         * (hoisted into fp, frame 92), loads before stores 12, one-pass blocks
         * around the fill and the apply 2 (add r5, sp, #64 after the last
         * store). Ordinary pointer differs 118 in EN (265 native, 269 built). A
         * generic output barrier before filling differs 155 (272 built); after
         * filling it differs 118 (267 built). Keeping rec live after the call
         * differs 179 (270 built). None keeps the native loop layout. loop.c's
         * second pass hoists the invariant &point when 15 x savings x life 8
         * reaches the loop's insn count (about 100 after giv reduction). */
        register s32 *rec asm("r0");

        // The object's projection is computed, but nothing reads it.
        EFFECT_VIEW_FROM_CAMERA(camera);
        rec = point;
        SET_VECTOR(rec, object->x, object->y, object->z);
        EffectPosition_ProjectPoint(rec, &center);
        /* FAKEMATCH: sched2 barrier, so this store precedes
         * the spark counter as in the reference 2026-10-11: without it differ 2
         * (movs r4, #0 moves ahead of the store). */
        do {
          center.x += xOffset;
        } while (0);
        for (i = 0; i != 3; i++) {
          struct EffectStep *spark = &work->particles[member * 3 + i];
          s32 x;
          s32 y;

          x = screen.x +
              (((Trig_Sin(spark->velocityX + i * 0x5555) * 8)) >> 16);
          y = screen.y +
              (((Trig_Cos(spark->velocityX + i * 0x5555) * 8)) >> 16);
          spark->velocityX += 0x200;
          draw[0](canvas, work->sheet + i * 0x240, x - 12, y - 28, 24, 24);
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// The burst picture in the sheet, after the smoke.
#define TARGET_BURSTS_CELLS 0x1b00

#define TARGET_BURSTS_FRAMES 96

// Target bursts: on each target in turn, 8 frames apart, a burst flares up
// in the middle while five puffs of smoke scatter around it, for 40
// frames.
void BattleFx_RunTargetBursts(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  s32 frame;
  DrawRectangle drawSmoke;
  DrawRectangle drawBurst;
  struct EffectStep *puff;
  s32 i;
  struct EffectPosition screen;
  s32 point[3];

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage,
                             work->sheet + TARGET_BURSTS_CELLS, 0, 0);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  drawBurst = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_MAX);
  drawSmoke = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);

  // Six puffs for each target: five scatter at random, the sixth (the
  // burst) stays in the middle. The burst draws its two random numbers
  // too, unused.
  for (i = 0; i != 64; i++) {
    s32 angle;
    s32 speed;

    puff = &work->particles[i];
    angle = Random16() & 0xffff;
    speed = Random16() & 0xff;
    SET_POSITION(puff, 0, 0, 0);
    if (i % 6 == 5) {
      puff->velocityX = 0;
      puff->velocityY = 0;
    } else {
      puff->velocityX = (Trig_Sin(angle) * speed) >> 7;
      puff->velocityY = (Trig_Cos(angle) * speed) >> 9;
    }
    puff->velocityZ = 0;
    puff->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  for (frame = 0; frame != TARGET_BURSTS_FRAMES; frame++) {
    s32 member;
    struct BattleCamera *camera;

    camera = gCameraWork;
    EFFECT_FOR_EACH_TARGET(member, work->effect) {
      struct MotionObject *effect;

      effect = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, effect->x, 160 << 13, effect->z);
      SceneTransform_ApplyPosition(point);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct EffectStep *puff;

        // The target's five puffs and its burst, each through six pictures
        // of six frames, holding the last; the burst rises faster.
        for (i = 0; i != 6; i++) {
          s32 cell;

          puff = &work->particles[member * 6 + i];
          cell = puff->variant / 6;
          if (cell > 5)
            cell = 5;
          EffectPosition_ProjectPoint(puff, &screen);
          screen.x >>= 1;
          if (i == 5) {
            EffectStep_AdvanceWithGravity3D(puff, 62, 0x800);
            drawBurst(canvas, work->sheet + cell * 0x480 + TARGET_BURSTS_CELLS,
                      screen.x - 12, screen.y - 36, 24, 48);
          } else {
            EffectStep_AdvanceWithGravity3D(puff, 60, 0x200);
            drawSmoke(canvas, work->sheet + cell * 0x480, screen.x - 12,
                      screen.y - 36, 24, 48);
          }
          puff->variant += 1;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef SWIRLING_STARS_FRAMES
#undef SWIRLING_STARS_COUNT
#undef TARGET_BURSTS_CELLS
#undef TARGET_BURSTS_FRAMES
