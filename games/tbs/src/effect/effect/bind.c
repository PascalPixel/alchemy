// Part of effect.c, the battle effect object, compiled only through it.
// Bind and Triangle Strike.

// Turn the screen into shades of blue, top down, with a ragged edge. Works
// on a copy of the screen in gMapCellBuffer. Each column starts up to 63
// lines late, and the edge speeds up a little every frame; after at most 27
// frames the picture moves onto a second blue ramp at colours 96 to 127.
// Takes 256 random numbers, one a column. Nothing calls it.
UNUSED void Graphics_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;
  // A 32-colour blue ramp at colours 32 to 63, red and green at half the blue.
  palette = (u16 *)&BG_PLTT_COLOR(2, 0);
  i = 0;
  do {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = *(u8 **)&gWorkSlot[HEAP_SLOT_BATTLE_SHEET];
  end = 0;
  speed = 16;
  Iwram_CopyWords(BLUE_RAMP_PIXELS, (void *)BG_CHAR_BLOCK(2), 0x7800);
  delayMask = 63;
  p = delays;
  do {
    *p++ = Random16() & delayMask;
  } while (p != delays + 256);
  row = 0;
  i = 0;
  do {
    end += speed / 4;
    speed++;
    while (row != end) {
      x = 0;
      do {
        y = row - delays[x];
        if (y >= 0) {
          if (y <= 119) {
            pixel = BLUE_RAMP_PIXELS + ((x & 7) + ((x / 8) << 6) +
                                        ((y & 7) << 3) + ((y / 8) << 11));
            // Column x reaches line row - delays[x]; the pixel there becomes
            // the brightest of its colour's channels, inverted, as an index
            // into the ramp.
            color = ((s16 *)BG_PLTT)[*pixel];
            red = color & 31;
            green = (color >> 5) & 31;
            blue = (color >> 10) & 31;
            if (red < green)
              red = green;
            if (red < blue)
              red = blue;
            *pixel = 63 - red;
          }
        }
        x++;
      } while (x != 256);
      row++;
    }
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, 0x7800);
    WaitFrames(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  // Then move every pixel up 64 colours, onto a second ramp at 96 to 127.
  palette = (u16 *)&BG_PLTT_COLOR(6, 0);
  i = 0;
  do {
    s32 half;
    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  i = 0;
  do {
    BLUE_RAMP_PIXELS[i] += 64;
    i++;
  } while (i != 0x7800);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), BLUE_RAMP_PIXELS, 0x7800);
  WaitFrames(1);
}

// Abilities: Bind (kind 38); Luff (kind 218).
//
// Battle effects 38 and 218: a triangle of light spins and shrinks onto the
// first target for 64 frames, then a rune stands on the target; the battle
// shows the result on frame 64. When effect->prelude is 1 a Jupiter Djinni
// appears first, sways and rises away. The canvas darkens a little faster
// every 16 frames.
void BattleFx_RunBind(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  // World input and projected vertices reuse one coordinate workspace.
  union {
    struct Vector3 world;
    struct EffectPosition projected;
  } position;
  struct EffectPosition screen;
  struct Vector3 scale;
  s32 originX;
  s32 originY;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct MotionObject *object;
  s32 tick;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;

  slots = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  if (work->effect->prelude == 1)
    BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side, 0, &originX,
                                 &originY);
  REG_BG2PA = Q_8_8(1);

  // The rune, the motes at 0x1000 and the Djinni at 0x2000.
  palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD),
      work->sheet + 0x1000);
  palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + 0x2000);
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = 0x04040404; // 4 off every pixel each frame
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  AudioCommand_PlayFar(SE_142);

  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    if (frame == 64)
      BattleEventRuntime_BeginPhaseFar(0);
    Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);

    // The Djinni sways 20 pixels either way and from frame 32 rises 2
    // pixels a frame.
    if (work->effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + originX + shift - 20;
      y = (((Trig_Cos(frame << 11) * 4)) >> 16) + originY - 24;
      BattleFx_FetchRectangleBlitters(work->effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + 0x2000, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + 0x2000, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }
    // Load the plain and mirrored max blitters, darken the canvas a step
    // faster every 16 frames, spin the triangle onto the first target and
    // stand the rune there, then present the frame.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] =
        (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    // Only the first target is drawn.
    for (member = 0; member != 1; member++) {
      tick = frame - member * 8;
      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      if (tick >= 0 && tick < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        position.world.x = object->x;
        position.world.y = object->y;
        position.world.z = object->z;
        EffectPosition_ApplyBaseAndYOffset(&position.world, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle: its three corners, turned a third of a circle apart and
        // shrinking as they spin, are projected into the particles' velocity
        // fields; each side is 24 flares.
        if (tick <= 67) {
          for (j = 0; j != 3; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (tick <= 63) {
              scale.x = 0x2a000 - (tick * 3 << 9);
              scale.y = 0x2a000 - (tick * 3 << 9);
              scale.z = 0x2a000 - (tick * 3 << 9);
              SceneTransform_ApplyScale(Vector3_Coordinates(&scale));
              SceneTransform_ApplyRoll((64 - tick) << 9);
              SceneTransform_ApplyYaw((64 - tick) << 9);
            }
            SceneTransform_ApplyRoll(j * 0x5555);
            EffectPosition_ApplyBaseAndYOffset(gBind_Vertex,
                                               &position.projected);
            point->velocityX = position.projected.x + screen.x;
            point->velocityY = position.projected.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - tick / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleFx_FlareCells[size - 1] + 0x1000,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (tick > 63) {
          draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

// Battle effect 67: the spinning triangle and rune of
// BattleFx_RunBind without the Djinni; on frame 46 the user
// steps up to the first target.
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  // World input and projected vertices reuse one coordinate workspace.
  union {
    struct Vector3 world;
    struct EffectPosition projected;
  } position;
  struct EffectPosition screen;
  struct Vector3 scale;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  s32 shift;
  u8 *graphics;
  u8 *palette;
  struct MotionObject *object;
  s32 tick;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;

  slots = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0);

  // The rune, and the motes at 0x1000.
  palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
  palette = Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD);
  Resource_DecodeType01(palette, work->sheet + 0x1000);
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = 0x04040404; // 4 off every pixel each frame
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  AudioCommand_PlayFar(SE_142);

  for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
    if (frame == 64)
      BattleEventRuntime_BeginPhaseFar(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(work->effect->actor,
                                     work->effect->actors[0], 16, 0);
    Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    // Load the plain and mirrored max blitters, darken the canvas a step
    // faster every 16 frames, spin the triangle onto the first target and
    // stand the rune there, then present the frame.
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] =
        (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    // Only the first target is drawn.
    for (member = 0; member != 1; member++) {
      tick = frame - member * 8;
      object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
      if (tick >= 0 && tick < 96) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        position.world.x = object->x;
        position.world.y = object->y;
        position.world.z = object->z;
        EffectPosition_ApplyBaseAndYOffset(&position.world, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;
        // The triangle: its three corners, turned a third of a circle apart and
        // shrinking as they spin, are projected into the particles' velocity
        // fields; each side is 24 flares.
        if (tick <= 67) {
          for (j = 0; j != 3; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (tick <= 63) {
              scale.x = 0x2a000 - (tick * 3 << 9);
              scale.y = 0x2a000 - (tick * 3 << 9);
              scale.z = 0x2a000 - (tick * 3 << 9);
              SceneTransform_ApplyScale(Vector3_Coordinates(&scale));
              SceneTransform_ApplyRoll((64 - tick) << 9);
              SceneTransform_ApplyYaw((64 - tick) << 9);
            }
            SceneTransform_ApplyRoll(j * 0x5555);
            EffectPosition_ApplyBaseAndYOffset(gTriangleStrike_Vertex,
                                               &position.projected);
            point->velocityX = position.projected.x + screen.x;
            point->velocityY = position.projected.y + screen.y + 16;
          }
          for (j = 0; j != 3; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - tick / 16;
            for (k = 0; k != 24; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + gBattleFx_FlareCells[size - 1] + 0x1000,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }
        // Then the rune, in two mirrored halves.
        if (tick > 63) {
          draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    EFFECT_PRESENT_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
