// Part of effect.c, the battle effect object, compiled only through it.
// Hail and Ground.

// The rock toss (Ground): a Venus Djinni throws six rocks from the user,
// shedding chips, which swirl and home in on the first target.

#define REVEAL_CHIP_COUNT 64 // ice chips

// Where the water burst's pictures sit in the sheet, after the waves.
#define REVEAL_CHIP_CELLS 0x400
#define REVEAL_DJINNI_CELLS 0x65c0

#define HAIL_FRAMES 64

// Abilities: Hail (kind 207).
//
// Water burst: after a Mercury djinni enters, ice chips burst out of the
// first target and waves rise along the bottom of the screen; the canvas
// fades out over the last 16 of its 64 frames. The 64 chips start together
// and are let go four a frame from frame 4, alternately mirrored; the
// battle shows the damage on frame 8. The chips live in PARTICLES,
// since work holds the waves' sheet.
//
// effect: the user and the targets; prelude 1 keeps the djinni circling.
void BattleFx_RunHail(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition origin;
  s32 screenY;
  s32 screenX;
  BlitterPair draw;
  struct EffectStep *chip;
  s32 i;
  s32 frame;
  s32 rise;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_RunDjinniEntrance(effect, 1, work->effect->side, 2, &screenX,
                             &screenY);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  EffectPosition_ProjectBattler(work->effect->actors[0], &origin);

  // The chips burst up from the first target.
  for (i = 0; i != REVEAL_CHIP_COUNT; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + 0x4000;
    speed = (Random16() & 0x1ff) + 0x80;
    PARTICLES[i].x = ((origin.x / 2 + (Random16() & 0xf)) - 8) << 16;
    PARTICLES[i].y = (origin.y + 8) << 16;
    PARTICLES[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    PARTICLES[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    PARTICLES[i].z = Random16() & 0x7f;
    PARTICLES[i].velocityZ = Random16() & 0x7f;
    PARTICLES[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != HAIL_FRAMES; frame++) {
    // Fade out over the last 16 frames.
    if (frame > 47)
      REG_BLDALPHA = (64 - frame) | 0x1000;
    if (frame == 1) {
      Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet,
                                 work->sheet + REVEAL_CHIP_CELLS, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 work->sheet + REVEAL_DJINNI_CELLS, 1, 0);
    }

    // Prelude: the small djinni circles.
    if (work->effect->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame << 11;
      x = (((-Trig_Sin(orbit) * 4)) >> 16) + screenX / 2 - 10;
      y = (((Trig_Cos(orbit) * 2)) >> 16) + screenY - 22;
      // Never true: the effect ends after 64 frames.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      draw[1](canvas, work->sheet + REVEAL_DJINNI_CELLS, x, y, 20, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + REVEAL_DJINNI_CELLS, x, y, 20, 40);
    }

    for (i = 0, chip = PARTICLES; i != REVEAL_CHIP_COUNT; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 cell;
        s32 width;
        s32 height;

        cell = (chip->z / 128) & 3;
        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gHail_CellWidths[cell];
        height = gHail_CellHeights[cell];
        draw[i & 1](canvas, work->sheet + REVEAL_CHIP_CELLS + gHail_Cells[cell],
                    HI(chip->x) - width / 2, HI(chip->y) - height / 2, width,
                    height);
#else
        draw[i & 1](canvas, work->sheet + REVEAL_CHIP_CELLS + gHail_Cells[cell],
                    HI(chip->x) - (width = gHail_CellWidths[cell]) / 2,
                    HI(chip->y) - (height = gHail_CellHeights[cell]) / 2, width,
                    height);
#endif
        EffectStep_AdvanceWithGravity3D(chip, 63, 0x1000);
      }
    }

    if (frame == 8) {
      work->shakeFrames = frame;
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 16);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 3);
    }

    // Waves rise along the bottom, 4 pixels a frame up to 32, drifting
    // away from the user.
    rise = frame * 4;
    if (rise > 32)
      rise = 32;
    if (work->effect->side == 0) {
      for (i = 0; i != 5; i++)
        draw[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                32, 32);
    } else {
      for (i = 0; i != 5; i++)
        draw[0](canvas, work->sheet, ((i << 5) + (frame / 4 & 31)) - 32,
                120 - rise, 32, 32);
    }

    Camera_ApplyShake(4, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// The small Venus djinni in the rock toss's sheet.
#define ROCK_TOSS_DJINNI_CELLS 0x65c0

#define GROUND_FRAMES 96
#define GROUND_LAUNCHED 8       // rocks launched
#define GROUND_CHIPS 32         // chip slots
#define GROUND_ROCKS 6          // rocks thrown
#define GROUND_CHIPS_PER_ROCK 4 // chips each rock sheds

// Abilities: Ground (kind 203).
//
// Venus djinni rock toss: after a Venus djinni enters, six rocks rise from
// the user shedding chips, swirl, and home in on the first target, each
// hitting it as it lands. A rock's variant is 1 while it flies and 0 once
// it has hit; the chips are particles 32 to 63, four for each rock.
//
// effect: the user and the targets; prelude 1 keeps the djinni circling.
void BattleFx_RunGround(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *user;
  struct EffectPosition position;
  s32 screenX;
  s32 screenY;
  s32 j;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  camera = WORK_SLOT(HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_RunDjinniEntrance(effect, 0, work->effect->side, 2, &screenX,
                             &screenY);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (work->effect->side == 1)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_MAX);
  draw[0] =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + ROCK_TOSS_DJINNI_CELLS, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  user = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // The rocks leave the user 66 pixels up, fanning out.
  for (i = 0; i != GROUND_LAUNCHED; i++) {
    struct EffectStep *rock = &work->particles[i];

    SET_POSITION(rock, user->x, 0x420000, user->z);
    SET_VELOCITY(rock, (i * 0x500000) >> 5,
                 (s32)(((Random16() & 127) - 64) << 16) >> 6,
                 (s32)(((Random16() & 255) - 127) << 16) >> 5);
    if (rock->x > 0)
      rock->velocityX = -rock->velocityX;
    rock->variant = 1;
  }
  for (frame = 0; frame != GROUND_FRAMES; frame++) {
    if (frame > 16)
      BattleFx_StepPaletteToResource((s32)&ResourceId_PortalSheet);
    // Prelude: the djinni circles above the user and from frame 17 rises 2
    // pixels a frame. For its first four frames it is drawn additively
    // first, so it flashes in.
    if (work->effect->prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (((-Trig_Sin(angle) * 4)) >> 16) + screenX / 2 - 10;
      s32 y = (((Trig_Cos(angle) * 2)) >> 16) + screenY - 22;

      if (frame > 16)
        y = y - frame * 2 + 32;
      if (work->effect->side == 1)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                            COMPOSITE_ADDITIVE);
      if (frame <= 3)
        ((DrawRectangle)((union HeapState *)gWorkSlot)
             ->slots[HEAP_SLOT_BLITTER_ALTERNATE])(
            canvas, work->sheet + ROCK_TOSS_DJINNI_CELLS, x, y, 20, 40);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      draw[0](canvas, work->sheet + ROCK_TOSS_DJINNI_CELLS, x, y, 20, 40);
    }
    // Every 2 frames each chip takes a random picture (velocityX) and
    // flip (velocityY).
    if ((frame & 1) == 0) {
      for (i = 0; i != GROUND_CHIPS; i++) {
        struct EffectStep *chip = &work->particles[32 + i];

        chip->velocityX = Random16() % 6 + 3;
        chip->velocityY = gHail_ChipFlips[Random16() & 3];
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != GROUND_ROCKS; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant == 1) {
        if (frame > i * 2) {
          s32 x;
          s32 y;

          EffectPosition_ProjectPoint(&rock->x, &position);
          position.x >>= 1;
          x = position.x - 12;
          y = position.y - 24;
          draw[0](canvas, work->sheet, x, y, 24, 48);
          // The rock's crust flickers between chips 1 and 2 every 2 frames.
          if ((frame & 3) <= 1)
            draw[0](canvas, work->sheet + gHail_ChipCells[1],
                    x + gHail_ChipX[1], y + gHail_ChipY[1], gHail_ChipWidths[1],
                    gHail_ChipHeights[1]);
          else
            draw[0](canvas, work->sheet + gHail_ChipCells[2],
                    x + gHail_ChipX[2], y + gHail_ChipY[2], gHail_ChipWidths[2],
                    gHail_ChipHeights[2]);
          // The four loose chips: each loads a blitter with its own flip,
          // and a flipped chip is placed from the rock's opposite edge so it
          // stays on the rock.
          for (j = 0; j != GROUND_CHIPS_PER_ROCK; j++) {
            struct EffectStep *chip = &work->particles[32 + i * 4 + j];
            s32 chipX;
            s32 chipY;

            EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, chip->velocityY,
                                COMPOSITE_MAX);
            draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)
                          ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
            if (chip->velocityY & 4)
              chipX = x - gHail_ChipWidths[chip->velocityX] -
                      gHail_ChipX[chip->velocityX] + 24;
            else
              chipX = x + gHail_ChipX[chip->velocityX];
            if (chip->velocityY & 8)
              chipY = y - gHail_ChipHeights[chip->velocityX] -
                      gHail_ChipY[chip->velocityX] + 48;
            else
              chipY = y + gHail_ChipY[chip->velocityX];
            draw[1](canvas, work->sheet + gHail_ChipCells[chip->velocityX],
                    chipX, chipY, gHail_ChipWidths[chip->velocityX],
                    gHail_ChipHeights[chip->velocityX]);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
          }
          rock->x += rock->velocityX;
          rock->y += rock->velocityY;
          rock->z += rock->velocityZ;
        }
        // After 16 frames the rock homes in on the target, damped until
        // frame 85, and hits it below 20 pixels.
        if (frame > i * 2 + 16) {
          rock->velocityX += (target->x - rock->x) >> 8;
          rock->velocityY += (0x140000 - rock->y) >> 8;
          rock->velocityZ += (target->z - rock->z) >> 8;
          if (frame < i * 2 + 85) {
            SET_VELOCITY(rock, rock->velocityX * 60 / 64,
                         rock->velocityY * 60 / 64, rock->velocityZ * 60 / 64);
          }
          if (rock->y < 0x140000) {
            work->shakeFrames = 8;
            rock->variant = 0;
            AudioCommand_PlayFar(SE_HEAVY_IMPACT);
            EFFECT_STRIKE_TARGET(work->effect, 0, 4, 4);
          }
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

// Shakes the battle view while gBattleFxWork's shakeFrames lasts, moving
// BG1 and the projection centre by a random offset; once the shake runs
// out, BG1 returns to its saved position and the centre to (120, 120). It
// takes two random numbers each shaking frame, x first.
//
// rangeX: the horizontal range, a power of two.
// rangeY: the vertical range, a power of two; both offsets are centred on
//         half of it.
void Camera_ApplyShake(s32 rangeX, u32 rangeY) {
  s32 displayY;
  s32 offsetX;
  s32 *remainingFrames;
  s32 restoredPosition;
  struct BattleEffectWork *work;
  s32 offsetY;
  s32 halfRange;
  s32 randomX;
  struct BgScroll *displayPosition;
  struct Projection *displayCenter;
  struct BgScroll *restoredScroll;
  struct Projection *restoredCenter;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    randomX = (rangeX - 1) & Random16();
    halfRange = (s32)(rangeY + (rangeY >> 0x1F)) >> 1;
    offsetY = ((rangeY - 1) & Random16()) - halfRange;
    displayPosition = gBgScroll;
    offsetX = randomX - halfRange;
    displayY = offsetY + 0x20;
    displayPosition[1].x = offsetX;
    displayPosition[1].y = displayY;
    displayCenter = &gProjection;
    displayCenter->centerX = 0x78 - offsetX;
    displayCenter->centerY = 0x78 - offsetY;
    *remainingFrames -= 1;
    return;
  }
  restoredPosition = work->savedBg1X;
  restoredScroll = gBgScroll;
  restoredScroll[1].x = restoredPosition;
  restoredPosition = work->savedBg1Y;
  restoredScroll[1].y = restoredPosition;
  restoredCenter = &gProjection;
  restoredCenter->centerX = 0x78;
  restoredCenter->centerY = 0x78;
}
