// Part of effect.c, the battle effect object, compiled only through it.
// Beam sequences and the ring and spark scene.

// The beam sequence draws a growing beam picture and its projected sparks
// over the first target. The ring-and-spark scene plays a strike of rings and
// sparks, restoring the user's motion afterwards.

// Draw the selected beam and its projected sparks over the first target: a
// beam, arc or wave shoots from the user; from frame 4 the target is hit,
// a yellow orb flashes over it and sparks fly off, for 32 frames in all.
// effect->kind picks the picture.
void BattleFx_RunBeamSequence(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct EffectPosition anchor;
  struct EffectPosition point;
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 kind;
  struct BattleUnit *unit;
  BlitterPair draw;
  DrawRectangle *slots;
  struct MotionObject *target;
  void *colors = (void *)BG_PLTT;
  s32 half;
  s32 frame;
  s32 i;

  heapCache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = heapCache;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(heapCache, HEAP_SLOT_CAMERA);
  kind = effect->kind;
  work->effect = effect;
  unit = UnitGetFar(effect->actor);
  WaitFrames(1);
  BattleFx_ConfigureEffectDisplay();
  BattleFx_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  WaitFrames(1);
  // Two blitters, additive and brightest-wins; kind 5 draws its pictures
  // upside down, and side 1 mirrors them.
  if (kind == 5) {
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y,
                          COMPOSITE_MAX);
    }
  } else {
    if (work->effect->side == 0) {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                              COMPOSITE_FLIP_X,
                          COMPOSITE_MAX);
    }
  }
  slots = (DrawRectangle *)gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  WaitFrames(1);
  // The picture: kind 4 the wave, 3 the beam image, 0, 1 and 5 blue arc A,
  // 2 blue arc B. Users numbered above 7 take the small Mars Djinni's
  // palette, the others the crescent's. The sparks and the yellow orb
  // follow.
  if (kind == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, work, 1, 1);
  } else if (kind == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage, work, 0, 0);
  } else {
    switch (kind) {
    case 0:
    case 1:
    case 5:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work, 1, 1);
      break;
    case 2:
      Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work, 1, 1);
      break;
    }
  }
  if (work->effect->actor > 7)
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
        128);
  else
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_CrescentSheet), 128);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, Ram_MapCellBuffer,
                             1, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  // Scroll BG1 so the first target is on screen; shake is that scroll,
  // between -128 and 0, and anchor the target's scrolled position.
  EffectPosition_ApplyAnimationAndYOffset(work->effect->actors[0], &anchor);
  if (work->effect->side == 0)
    shake = 96 - anchor.x;
  else
    shake = 32 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].x = shake;
  gBgScroll[1].y = 80;
  WaitFrames(1);
  // 64 sparks start at the first target's middle and fly away from the
  // centre of the field at random speeds; variant is a spark's life.
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &work->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
    SET_VELOCITY(spark, (Random16() & 255) << 10,
                 ((Random16() & 255) - 32) << 10,
                 ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    // Frame 5 starts battle phase 134 if the user's class is flagged in the
    // summon table, 133 otherwise; on frame 4 the target plays motion 0.
    if (frame == 5) {
      if (Summon_IsEntrySecondaryFlaggedFar(unit->classId))
        BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
      else
        BattleEventRuntime_BeginPhaseFar(SE_133);
    }
    if (frame == 4)
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
    // The picture plays from the user, to its left on side 0 and its right
    // on side 1: the wave's six 48x16 cells backwards and the arc's six
    // 48x72 cells, two frames each; the beam image grows through its six
    // cells, three frames each.
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    screenPosition.y += 16;
    if (kind == 4) {
      if (frame <= 11) {
        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  screenPosition.x + shake - 48, screenPosition.y - 8, 48, 16);
        else
          draw[1](canvas, work->sheet + (5 - frame / 2) * 0x300,
                  screenPosition.x + shake, screenPosition.y - 8, 48, 16);
      }
    } else if ((u32)kind <= 2 || kind == 5) {
      if (frame <= 11) {
        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + frame / 2 * 0xd80,
                  screenPosition.x + shake - 48, screenPosition.y - 40, 48, 72);
        else
          draw[1](canvas, work->sheet + frame / 2 * 0xd80,
                  screenPosition.x + shake, screenPosition.y - 40, 48, 72);
      }
    } else {
      if (frame <= 17) {
        s32 cell = frame / 3;

        if (work->effect->side == 0)
          draw[1](canvas, work->sheet + gBeams_Cells[cell],
                  screenPosition.x + gBeams_CellReaches[cell] + shake - 58,
                  screenPosition.y - gBeams_CellHeights[cell] / 2,
                  gBeams_CellWidths[cell], gBeams_CellHeights[cell]);
        else
          draw[1](canvas, work->sheet + gBeams_Cells[cell],
                  screenPosition.x - gBeams_CellReaches[cell] + shake -
                      gBeams_CellWidths[cell] + 58,
                  screenPosition.y - gBeams_CellHeights[cell] / 2,
                  gBeams_CellWidths[cell], gBeams_CellHeights[cell]);
      }
    }
    // Frames 4 to 15: the yellow orb flashes over the target, six 40x48
    // cells, two frames each.
    if (frame >= 4 && frame <= 15)
      draw[0](canvas, Ram_MapCellBuffer + (frame - 4) / 2 * 0x780,
              anchor.x - 16, anchor.y - 24, 40, 48);
    EFFECT_VIEW_FROM_CAMERA(camera);
    // From frame 4 the sparks fly. Only the first 32 move: each is drawn
    // and moved twice a frame, so it flies at double speed and its life
    // runs out twice as fast. Even ones add, odd ones take the brighter
    // pixel.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          Render_ProjectPoint(&spark->x, EffectPosition_Coordinates(&point));
          point.x += shake;
          size >>= 3;
          size += 2;
          EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, point.x, point.y,
                            size);
          EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  BattleFx_HideBg1AndRestoreBackdrop();
}

// Play the selected ring-and-spark strike, restoring the user motion after
// it. Kinds above 199 first gather 64 motes of light into the user for 32
// frames while it stands still; then star burst and crescent rings flash
// over the canvas and sparks fly from the first target for 32 frames, and
// the background palette fades back in.
void BattleFx_RunRingAndSparkScene(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  struct EffectPosition anchor;
  struct EffectPosition point;
  struct EffectPosition spot;
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 acceleration;
  s32 strength;
  BlitterPair draw;
  DrawRectangle *slots;
  struct MotionObject *object;
  struct MotionObject *target;
  u16 *background;
  void *colors = (void *)BG_PLTT;
  s32 words = 0x4000;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  heapCache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = heapCache;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(heapCache, HEAP_SLOT_CAMERA);
  big = 1;
  if (effect->kind <= 199)
    big = 0;
  work->effect = effect;
  // Load the blitters and pictures a step a frame, keeping the transition
  // centred between the user and the first target meanwhile. The star
  // burst goes to the image storage and the crescent to the map cell
  // buffer; users numbered above 7 also take the small Mars Djinni's
  // palette.
  BattlePres_SetupTransitionAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  WaitFrames(1);
  BattleFx_SetupCanvasTileMap();
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  if (work->effect->side == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
  }
  slots = (DrawRectangle *)gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarBurstSheet,
                             work->imageStorage, 1, 0);
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentSheet, Ram_MapCellBuffer,
                             1, 1);
  if (work->effect->actor > 7)
    Iwram_CopyWords(
        colors, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
        128);
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattlePres_SetupTransitionAtPairMidpointFar(work->effect->actor,
                                              work->effect->firstTarget, 130);
  WaitFrames(1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  // The gathering: 64 motes, each in its own random rotation around the
  // user, close in 4 a frame, mote i from frame i / 4, while the palette
  // fades over 24 frames. The user is held still meanwhile: its
  // velocities, acceleration and vertical strength are saved, zeroed and
  // restored after.
  if (big == 1) {
    object = GetBattleObjectSlotFar(work->effect->actor)->object;
    for (i = 0; i != 64; i++) {
      struct EffectStep *mote = &work->particles[i];

      SET_POSITION(mote, (Random16() & 63) + 16, 0, 0);
      SET_VELOCITY(mote, Random16() & 0xffff, Random16() & 0xffff,
                   Random16() & 0xffff);
    }
    ObjectDispatch_ApplyValueToChildrenFar(object, 0);
    velocityX = object->velocityX;
    velocityY = object->velocityY;
    velocityZ = object->velocityZ;
    strength = object->verticalMotionStrength;
    acceleration = object->acceleration;
    SET_VELOCITY(object, 0, 0, 0);
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    shake = 64 - screenPosition.x;
    gBgScroll[1].x = shake;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, EFFECT_FADE_ORDER);
    AudioCommand_PlayFar(SE_212);
    for (frame = 0; frame != 32; frame++) {
      BattlePres_SetupTransitionAtPairMidpointFar(
          work->effect->actor, work->effect->firstTarget, 130);
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Render_ResetTransformState();
          SceneTransform_ApplyRoll(mote->velocityZ);
          SceneTransform_ApplyPitch(mote->velocityX);
          SceneTransform_ApplyYaw(mote->velocityY);
          EffectPosition_ApplyBaseAndYOffset(&mote->x, &point);
          point.x += 64;
          point.y = point.y + screenPosition.y + 24;
          CLAMP(point.depth, -60, 60);
          point.depth += 60;
          EFFECT_DRAW_FLARE(draw[0], canvas, sheet, point.x, point.y, size);
          mote->x -= 4;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    }
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    ObjectDispatch_ApplyValueToChildrenFar(object, 16);
    SET_VELOCITY(object, velocityX, velocityY, velocityZ);
    object->acceleration = acceleration;
    object->verticalMotionStrength = strength;
  }

  // The strike: clear the canvas, scroll BG1 so the first target is on
  // screen, and start 64 sparks at its middle, rising and flying away from
  // the centre of the field.
  Iwram_ClearWords(canvas, words);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), words);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
  if (work->effect->side == 0)
    shake = 32 - anchor.x;
  else
    shake = 96 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].y = 80;
  gBgScroll[1].x = shake;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  half = Battle_GetObjectTableValueFar(work->effect->actors[0]) / 2;
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &work->particles[i];

    SET_POSITION(spark, target->x, target->y + half, target->z);
    SET_VELOCITY(spark, (Random16() & 255) << 10, (Random16() & 255) << 10,
                 ((Random16() & 255) - 127) << 10);
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    if (frame == 5)
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    if (frame == 4)
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 0);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    screenPosition.y += 16;
    // The rings: two frames each of the two star burst cells, then of the
    // two crescent cells.
    if (frame <= 1)
      draw[0](canvas, work->imageStorage, 0, 0, 120, 120);
    else if (frame <= 3)
      draw[0](canvas, work->imageStorage + 0x3840, 0, 0, 120, 120);
    else if (frame <= 5)
      draw[0](canvas, Ram_MapCellBuffer, 0, 0, 120, 120);
    else if (frame <= 7)
      draw[0](canvas, Ram_MapCellBuffer + 0x3840, 0, 0, 120, 120);
    EFFECT_VIEW_FROM_CAMERA(camera);
    // From frame 4 the sparks fly, as in the beam sequence: only the first
    // 32 move, each drawn and moved twice a frame.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != 64; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          EffectPosition_ApplyBaseAndYOffset(&spark->x, &spot);
          spot.x += shake;
          size >>= 3;
          size += 2;
          spot.y += 16;
          EFFECT_DRAW_FLARE(draw[pair & 1], canvas, sheet, spot.x, spot.y,
                            size);
          EffectStep_AdvanceWithGravity3D(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  gBgScroll[1].y = 32;
  // Fade the background palette back in over 7 frames.
  for (frame = 0, background = &gBattleWork->background; frame != 7; frame++) {
    BattlePresentation_SetPaletteLevelFar(*background, 6 - frame);
    WaitFrames(1);
  }
  BattleFx_HideBg1AndRestoreBackdrop();
}
