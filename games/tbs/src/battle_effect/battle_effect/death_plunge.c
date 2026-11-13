/// @file
/// Death Plunge, the blade rain, the element pillar, the element burst and
/// the shattering picture.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// - Death Plunge runs the user up to the first target; both leap, turn upside
//   down, and on the hit bounce apart while a ball of fire swells over them.
// - The blade rain drops 16 swords, one every 4 frames, slanting in on the
//   first target and sticking in the ground. No effect table entry points at
//   it.
// - The element pillar (BattleEffect_RunElementPillar) stands a mirrored pillar
//   of light at a unit's feet while motes spiral up around it.
// - The element burst (BattleEffect_RunElementBurst) flares sparks at four
//   corners around a unit, draws motes spiralling in, then plays a burst of
//   light over it.
// - BattleEffect_RunShatteringOpening shatters a picture into 33 pieces as a
//   battle opens.
// - The element streaks (BattleEffect_StartElementStreaks) draw lines of light
//   closing in on a unit from a task while the battle runs on.
//
// The element runners, the streaks and the shattering opening are reached
// from the battle through far calls, not the handler table, and allocate
// their own work, canvas and sheet instead of using the shared ones.

#define DEATH_PLUNGE_FRAMES 96

/// The user runs 9/10 of the way to the first target, then both leap and turn
/// upside down in the air; on frame 54 the target is hit and both bounce back,
/// while a ring of flashes bursts from the user and, with its trails, swells
/// into a ball of fire over the two.
///
/// Abilities: Death Plunge (BATTLE_EFFECT_DEATH_PLUNGE).
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  /* LEFTOVER(frame): draw[1], the idle half of the pair; the ROM's frame
   * reserves 84 bytes (sub sp, #84), 4 more than one blitter needs, and no
   * instruction addresses the word at sp+52; BlitterPair is the blitter set
   * every effect declares, and BattleEffect_FetchRectangleBlitters and 28
   * battle-effect files fill or call its second half; as one DrawRectangle, 14
   * of 363 instructions differ in tbs-en. */
  BlitterPair draw;
  struct BattleObject *first;
  struct BattleObject *second;
  struct Actor *user;
  struct Actor *target;
  s32 x;
  s32 z;
  s32 userHeight;
  s32 targetHeight;
  s32 point[3];
  struct ScreenPoint screen;
  s32 frame;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  // Trails keep 3/4 a frame.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // The run: the user moves to 90% of the way to the target, with an
  // acceleration of 2 pixels a frame per frame and a top speed of 8 pixels a
  // frame, and the effect waits 20 frames for it.
  first = GetBattleObjectSlotFar(work->args->user);
  second = GetBattleObjectSlotFar(work->args->targets[0]);
  user = first->actor;
  target = second->actor;
  {
    s32 percent = 90;

    x = user->x.fixed + percent * (target->x.fixed - user->x.fixed) / 100;
    z = user->z.fixed + percent * (target->z.fixed - user->z.fixed) / 100;
  }
  userHeight = BattleUnit_GetEffectHeightFar(work->args->user);
  targetHeight = BattleUnit_GetEffectHeightFar(work->args->targets[0]);
  Actor_StartIdleFar(user);
  Actor_SetMoveTargetFar(user, x, 0, z);
  Actor_SelectAnimationFar(user, POSE_LEAP);
  user->snapToTarget = 1;
  user->steeringFlags = 1;
  user->acceleration = px(2);
  user->speedLimit = px(8);
  wait(20);
  for (frame = 0; frame != DEATH_PLUNGE_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);

    // Both leap at 15 pixels a frame.
    if (frame == 0) {
      target->velocityY = px(15);
      target->gravity = px(57) / 100;
      user->velocityY = px(15);
      user->gravity = px(57) / 100;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed += userHeight;
      target->y.fixed += targetHeight;
    }

    // The target flashes and both bounce; the user heads back towards the
    // middle of the field.
    if (frame == 54) {
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 10);
      target->velocityY = px(8);
      target->gravity = BATTLE_UNIT_GRAVITY;
      user->velocityY = px(5);
      user->gravity = px(47) / 100;
      user->acceleration = px(1);
      user->speedLimit = px(2);
      user->steeringFlags = 0;
      Actor_StartIdleFar(user);
      Actor_SetMoveTargetFar(user, 0, 0, user->z.fixed);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    SET_VECTOR(point, user->x.fixed, user->y.fixed, user->z.fixed);
    Projection_Point(point, &screen);
    screen.x >>= 1;

    // One big flash on the user.
    if (frame == 54 || frame == 55)
      draw[0](canvas, work->sheet, screen.x - 16, screen.y - 16, 32, 64);
    if (frame >= 56 && frame <= 67) {
      s32 i;

      for (i = 0; i != 16; i++) {
        s32 angle;
        s32 rx;
        s32 ry;

        angle = i << 12; // 22.5 degrees apart
        rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
        ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
        draw[0](canvas, work->sheet + (((frame - 56) / 2) << 11), rx - 16, ry,
                32, 64);
      }
    }

    // Both turn right side up again.
    if (frame == 64) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed -= userHeight;
      target->y.fixed -= targetHeight;
      Actor_SelectAnimationFar(user, 0);
    }
    // The cues: the damage with a heavy impact on the hit, and a shake on
    // the leap and on the frame before the hit.
    if (frame == 54)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 0) {
      Sound_PlayFar(SE_WHOOSH);
      work->shakeFrames = 6;
    }
    if (frame == 53)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define BLADE_RAIN_FRAMES 120

/// Drops 16 swords, one every 4 frames, slanting in towards the first target
/// (the middle of the canvas, x 64) and sticking in the ground, where their
/// bottom 4 pixels are hidden.
UNUSED void BattleEffect_RunBladeRain(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct Particle *blade;
  s32 frame;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = *cursor++;
  canvas = *cursor;
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  // Invisible to start with.
  REG_BLDALPHA = BLEND_ALPHA(0, 16);

  // Setup: [0] draws as is, [1] mirrored; both skip colour 0.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1,
                             1);
  // Redraw from blank each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  // Scroll the canvas so its middle column sits on the first target.
  Projection_ScaledUnit(work->args->targets[0], &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Each sword waits at a random x from 16 to 111 near the top of the canvas
  // (sword i centred 24 - i / 4 pixels down, so the 16 drawn sit at rows 24 to
  // 21, each run of four a pixel higher than the last), drawn there until its
  // turn to fall, and drifts towards the middle faster (in steps of 2 pixels a
  // frame) the further out it starts.
  for (i = 0; i != 64; i++) {
    s32 column;

    blade = &work->particles[i];
    column = Random16() % 96 + 16;
    blade->x = column;
    blade->y = px(24 - i / 4);
    if (column <= 43)
      blade->velocityX = 3;
    else if (column <= 51)
      blade->velocityX = 2;
    else if (column <= 59)
      blade->velocityX = 1;
    else if (column <= 67)
      blade->velocityX = 0;
    else if (column <= 75)
      blade->velocityX = -1;
    else if (column <= 83)
      blade->velocityX = -2;
    else
      blade->velocityX = -3;
    blade->velocityX <<= 17;
    blade->velocityY = px(8);
    blade->x <<= 16;
  }
  Sound_PlayFar(SE_POWER_UP);
  for (frame = 0; frame != BLADE_RAIN_FRAMES; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = BLEND_ALPHA(frame, 16);
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = BLEND_ALPHA(120 - frame, 16);
      if (frame == 104)
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    }

    // The image depends on how steeply the sword falls, and a sword drifting
    // left is drawn mirrored (the drift's sign bit picks the blitter).
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 picture;

      blade = &work->particles[i];
      drift = blade->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      picture = speed >> 17;
      if (frame < i * 4 + 25) {
        u32 width;
        u32 height;

        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gDeathPlunge_SwordWidths[picture];
        height = gDeathPlunge_SwordHeights[picture];
        blit[drift < 0](canvas,
                        work->sheet + gDeathPlunge_SwordPictures[picture],
                        (blade->x >> 16) - (width >> 1),
                        (blade->y >> 16) - (height >> 1), width, height);
#else
        blit[(u32)drift >> 31](
            canvas, work->sheet + gDeathPlunge_SwordPictures[picture],
            (blade->x >> 16) -
                ((width = gDeathPlunge_SwordWidths[picture]) >> 1),
            (blade->y >> 16) -
                ((height = gDeathPlunge_SwordHeights[picture]) >> 1),
            width, height);
#endif
        if (frame >= i * 4 + 16) {
          blade->x += blade->velocityX;
          blade->y += blade->velocityY;
        }
      } else {
        u32 width;
        u32 height;

#ifdef UBFIX
        width = gDeathPlunge_SwordWidths[picture];
        height = gDeathPlunge_SwordHeights[picture];
        blit[drift < 0](canvas,
                        work->sheet + gDeathPlunge_SwordPictures[picture],
                        (blade->x >> 16) - (width >> 1),
                        (blade->y >> 16) - (height >> 1), width, height - 4);
#else
        blit[(u32)drift >> 31](
            canvas, work->sheet + gDeathPlunge_SwordPictures[picture],
            (blade->x >> 16) -
                ((width = gDeathPlunge_SwordWidths[picture]) >> 1),
            (blade->y >> 16) -
                ((height = gDeathPlunge_SwordHeights[picture]) >> 1),
            width, height - 4);
#endif
      }
    }

    // Every 4 frames from 24 to 84 the target flashes white in its damage pose
    // for 2 frames and the screen shakes.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        Sound_PlayFar(SE_IMPACT);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define ELEMENT_PILLAR_FRAMES 56

/// A symmetrical pillar of light in an element's colours stands at a unit's
/// feet while motes spiral up around it, for 56 frames, and the first target
/// flashes.
void BattleEffect_RunElementPillar(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct ScreenPoint screenPosition;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  // The sheet slot takes ParticleSpritesD's flare pictures, which
  // EFFECT_DRAW_FLARE reads: the ten squares of 2x2 to 20x20 are 1540 of its
  // 1550 bytes.
  work = (struct BattleEffectWork *)HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT,
                                                   sizeof *work);
  canvas = (void *)HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = (u8 *)HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  camera = gBattleCamera;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  // Palette_StepFadeTransfer darkens the battle background for 24 frames.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  // The narrow canvas, 12/16 over all of the background.
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (work->args->kind) {
  case ELEMENT_VENUS:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case ELEMENT_MERCURY:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case ELEMENT_MARS:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The spiral's motes: x holds the angle and z the radius in 1/16
  // pixels; mote i waits i frames before it starts to rise.
  for (i = 0; i != 128; i++) {
    PARTICLES[i].y = 0;
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
    PARTICLES[i].variant = -i;
  }
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  // The mote size, written back into the caller's argument.
  work->args->variant++;
  if (work->args->variant <= 0)
    work->args->variant = 1;
  if (work->args->variant > 4)
    work->args->variant = 4;
  Sound_PlayFar(SE_POWER_UP);

  for (frame = 0; frame != ELEMENT_PILLAR_FRAMES; frame++) {
    Projection_Unit(work->args->user, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = BLEND_ALPHA(112 - frame * 2, 16);
    if (frame == 16)
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               TINT_KEEP, 0, 20);

    // The pillar: its left half at x 47 and its mirrored right half at x 64,
    // keeping the brighter pixel.
    if (frame <= 55) {
      s32 n;
      u8 *glow;

      n = frame / 2 % 4;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
      draw[1](canvas, work->sheet + n * (17 * 64), 47, ground.y - 64, 17, 64);
      n = frame / 4 % 3;
      glow = work->sheet + n * 1032 + 4352;
      draw[1](canvas, glow, 40, ground.y - 36, 24, 43);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
      n = frame / 2 % 4;
      draw[1](canvas, work->sheet + n * (17 * 64), 64, ground.y - 64, 17, 64);
      draw[1](canvas, glow, 64, ground.y - 36, 24, 43);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
    }
    // DEAD: a lookup whose result is dropped; the ROM keeps the call.
    GetBattleObjectSlotFar(work->args->user);
    EFFECT_VIEW_FROM_CAMERA(camera);

    // The first 32 motes, each starting i frames in.
    for (i = 0; i != 32; i++) {
      struct Particle *fleck = &PARTICLES[i];

      if (fleck->variant >= 0) {
        s32 size;
        s32 turnSteps = 64; // frames a mote takes to circle once

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += TRIG_TURN / turnSteps;
        fleck->y += px(5);
        fleck->z += 64;
        Projection_Point(point, &screenPosition);
        screenPosition.x /= 2;
        size = (i & 1) + work->args->variant;
        EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screenPosition.x,
                          screenPosition.y, size);
      }
      fleck->variant++;
    }
    EFFECT_END_FRAME(work);
  }

  // Finish: the blitter, the task and the layer, then the three slots
  // in the reverse of the order they were taken.
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Sets the pixel at (x, y) of the 256x120 8bpp tile picture in
/// Ram_MapGridCellBuffer (32 tiles across) to colour 2.
#define SHATTERING_OPENING_PLOT_PIXEL(x, y)                                    \
  (*(Ram_MapGridCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +        \
     (y) / 8 * 2048) = 2)

/// Plots the four corners (x1 or x2, y1 or y2): one point of the circle
/// mirrored into four octants.
#define SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2)                        \
  (SHATTERING_OPENING_PLOT_PIXEL(x2, y2),                                      \
   SHATTERING_OPENING_PLOT_PIXEL(x2, y1),                                      \
   SHATTERING_OPENING_PLOT_PIXEL(x1, y2),                                      \
   SHATTERING_OPENING_PLOT_PIXEL(x1, y1))

#define SHATTERING_OPENING_FRAMES 128
#define SHATTERING_OPENING_FLASHES 32 ///< flashes thrown as the picture breaks
#define SHATTERING_OPENING_PIECES 33  ///< pieces the picture breaks into
#define SHATTERING_OPENING_DISC_X 96  ///< the centre of the opening disc
#define SHATTERING_OPENING_DISC_Y 60  ///< the centre of the opening disc

/// As a battle opens on the Venus aerie storm battle background, the screen
/// turns white and a grey disc grows from (96, 60) by 32 pixels a frame while
/// the TileAnimationMask picture breaks into 33 pieces that fly apart and fall;
/// background palettes 6 to 13 then ease from white back to the battle
/// background's colours, and from frame 51 32 flashes rise. Runs for 128
/// frames.
void BattleEffect_RunShatteringOpening(struct BattleEffectParams *args) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 56 bytes it reserves (sub sp, #56), no instruction
   * addresses the 12 at sp+44 to sp+55, a ScreenPoint's size; as without it, 4
   * of 1051 instructions differ in tbs-en. */
  UNUSED struct ScreenPoint position;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleWork *battle;
  struct BattleBackgroundWork *view;
  s32 targetRadius;
  struct Particle *piece;
  s32 radius;
  s32 i;
  s32 octantX;
  s32 j;
  s32 k;
  s32 error;
  s32 octantY;
  s32 x1;
  s32 leftX;
  s32 innerLeftX;
  s32 y1;
  s32 x2;
  s32 y2;
  u8 *startY;
  u8 *startX;
  s32 centreX = SHATTERING_OPENING_DISC_X;
  s32 centreY = SHATTERING_OPENING_DISC_Y;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*work));
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  work->args = args;

  BattleEffect_ClearActorTimers();
  view->secondMode = BATTLE_BACKGROUND_FIXED;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // Aim the camera at the field's centre at 100% zoom, lining the battle
  // zoomed background up with it.
  BattleCamera_SetViewFar(0, 0, 0, 100);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;

  // BG2's affine map, at BG_CANVAS_MAP (screen block 7), shows tiles 0 to 255
  // in order, so the canvas appears as one picture.
  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + i) = entry;
    }
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_16);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask,
                             work->imageStorage, 1, 1);
  // Redraw from blank each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  // Each piece flies out from (32, 60) at a quarter of its distance from
  // there a frame, in work->particles 0 to 32.
  startY = gDeathPlunge_PieceStartY;
  startX = gDeathPlunge_PieceStartX;
  radius = 0;
  piece = work->particles;
  for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
    s32 x, y, vx, vy;

    x = px(*startX++);
    y = px(*startY++);
    piece->x = x;
    piece->y = y;
    vx = (x - px(32)) >> 2;
    vy = (y - px(60)) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
    piece++;
  }

  // The disc's mask in Ram_MapGridCellBuffer starts as colour 1 everywhere (the
  // copy of the screen into it is overwritten at once), and background palettes
  // 6 to 13 turn white.
  Iwram_CopyWords(Ram_MapGridCellBuffer, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  Iwram_FillWords(Ram_MapGridCellBuffer, BATTLE_BACKGROUND_BYTES,
                  CANVAS_FILL_WORD(1));
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) |
                          BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31),
                      IO_WRITE_16);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(6, 0), 256,
                  RGB(31, 31, 31) << 16 | RGB(31, 31, 31));
  Sound_PlayFar(SE_POWER_UP);
  BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                           POSE_CAST, 0, 30);

  for (frame = 0; frame != SHATTERING_OPENING_FRAMES; frame++) {
    if (frame == 2)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 3)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 28)
      BattleEffect_TintBattler(work->args->targets[0], TINT_KEEP, POSE_CAST,
                               TINT_KEEP, 0);
    if (frame == 32)
      Sound_PlayFar(SE_MONSTER_ROAR);
    if (frame == 5)
      Sound_PlayFar(SE_CRASH);
    // Bring the battle background back on BG1.
    if (frame == 5) {
      gBgScroll[1].x = work->savedBg1X;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, -1);
    }

    // From frame 8, move every colour of palettes 6 to 13 one step towards the
    // battle background's colour.
    if (frame > 7) {
      u16 *color;

      color = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != 128; i++) {
        s32 red = *color & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 blue = (*color >> 10) & 0x1f;
        s32 goalRed = battle->palette[i] & 0x1f;
        s32 goalGreen = (battle->palette[i] >> 5) & 0x1f;
        s32 goalBlue = (battle->palette[i] >> 10) & 0x1f;

        if (red < goalRed)
          red++;
        else if (red > goalRed)
          red--;
        if (green < goalGreen)
          green++;
        else if (green > goalGreen)
          green--;
        if (blue < goalBlue)
          blue++;
        else if (blue > goalBlue)
          blue--;
        *color++ = RGB(red, green, blue);
      }
    }

    // The whole screen turns the disc's grey.
    if (frame == 4)
      Iwram_FillWords((void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES,
                      CANVAS_FILL_WORD(2));

    // Grow the disc in the mask by 32 pixels of radius, brighten its grey
    // (colour 2), and copy the mask to the screen.
    if (frame <= 3) {
      s32 v;

      v = frame * 4 + 8;
      targetRadius = frame * 32;
      BG_PLTT[2] = RGB(v, v, v);
      for (; radius != targetRadius; radius++) {
        // The midpoint circle algorithm: one point plots all eight octants, and
        // each is drawn twice, one pixel apart, to fill gaps.
        octantX = radius;
        octantY = 0;
        error = radius;
        while (octantX >= octantY) {
          x2 = octantX + centreX;
          x1 = centreX - octantX;
          leftX = x1;
          y1 = centreY - octantY;
          y2 = octantY + centreY;
          if (y1 < 0)
            y1 = 0;
          if (y2 > 119)
            y2 = 119;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);
          x1 = leftX + 1;
          x2 = octantX + SHATTERING_OPENING_DISC_X + 1;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);

          x2 = octantY + centreX;
          x1 = centreX - octantY;
          innerLeftX = x1;
          y2 = octantX + centreY;
          y1 = centreY - octantX;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          if (y1 < 0)
            y1 = 0;
          if (y2 > 119)
            y2 = 119;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);
          x1 = innerLeftX + 1;
          x2 = octantY + SHATTERING_OPENING_DISC_X + 1;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);

          error -= octantY * 2 + 1;
          if (error < 0) {
            error += octantX * 2 - 2;
            octantX--;
          }
          octantY++;
        }
      }
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapGridCellBuffer,
                      BATTLE_BACKGROUND_BYTES);
    }

    // Draw the pieces up to frame 50; they start to fly and fall on frame
    // 4, undamped, under a gravity of a quarter pixel a frame.
    if (frame <= 50) {
      for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
        piece = &work->particles[i];
        draw[0](canvas, work->imageStorage + gDeathPlunge_PiecePictures[i],
                Q_16_16_TO_INT(piece->x), Q_16_16_TO_INT(piece->y),
                gDeathPlunge_PieceWidths[i], gDeathPlunge_PieceHeights[i]);
        if (frame > 3)
          Particle_AdvanceWithGravity2D(piece, 64, px(1) / 4);
      }
    }

    // Colour 1 goes from black to white, one step a frame, and is white from
    // frame 39.
    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > 31)
        v = 31;
      BG_PLTT[1] = RGB(v, v, v);
    }

    if (frame == 51) {
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet,
                                 1, 0);
      for (i = 1; i != 64; i++) {
        s32 v;

        v = i / 2;
        // DEAD: v is never below 0; the ROM keeps the test.
        if (v < 0)
          v = 0;
        BG_PLTT[i] = RGB(v, v / 2, v);
      }
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        piece->x = ((Random16() & 31) + 32) << 16;
        piece->y = ((Random16() & 31) + 80) << 16;
        piece->velocityX = ((Random16() & 0x1ff) - 256) << 12;
        piece->velocityY = 0;
        piece->variant = 0;
      }
      // Trails keep half a frame.
      work->transferMode = CANVAS_COPY_AND_FADE;
      work->transferValue = CANVAS_FADE_HALF;
    }

    // Flashes: flashes 0 to 7 appear on frame 53 and four more each frame
    // after, and each floats up (a gravity of -1/16 pixel a frame per frame)
    // for 40 frames, through six images of 4 frames, holding the last.
    if (frame > 52) {
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        if (frame >= i / 4 + 52 && piece->variant <= 39) {
          s32 picture;

          picture = piece->variant / 4;
          if (picture > 5)
            picture = 5;
          draw[1](canvas, work->sheet + (picture << 11),
                  Q_16_16_TO_INT(piece->x) - 16, Q_16_16_TO_INT(piece->y) - 32,
                  32, 64);
          Particle_AdvanceWithGravity2D(piece, 60, -px(1) / 16);
          piece->variant++;
        }
      }
    }

    EFFECT_END_FRAME(work);
  }

  // Finish: restore the unit, BG1's scroll and the battle background's palette
  // fade, then free the slots in the reverse of the order they were taken.
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_TintBattler(work->args->targets[0], TINT_KEEP, POSE_STANDING,
                           TINT_KEEP, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  wait(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_16);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// One streak of light of the element streak burst, laid over the same
/// 28-byte entries as PARTICLES: a line along a randomly turned axis from
/// tail to head, in distance from the centre.
struct Streak {
  s32 head;     ///< the head's distance from the centre, 48 to 63 at first
  s32 tail;     ///< the tail's distance, 8 less; both close in 4 a frame
  s32 unused08; ///< the Particle's z, which the streaks leave alone
  s32 pitch;    ///< the axis's turn, a random 16-bit angle
  s32 yaw;      ///< the axis's turn, a random 16-bit angle
  s32 roll;     ///< the axis's turn, a random 16-bit angle
  s32 unused18; ///< the Particle's variant, which the streaks leave alone
};

/// For 56 frames, sparks in an element's colours flare at the four corners
/// around a unit, motes spiral in, and from frame 28 a burst of light plays
/// over the unit while the first target flashes.
void BattleEffect_RunElementBurst(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  // Setup, as for the element pillar.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (work->args->kind) {
  case ELEMENT_VENUS:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case ELEMENT_MERCURY:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case ELEMENT_MARS:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // PARTICLES are set up as for the element pillar but never drawn here.
  for (i = 0; i != 128; i++) {
    PARTICLES[i].y = px(128);
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + 1024;
    PARTICLES[i].variant = -i;
  }
  for (i = 0; i != 64; i++) {
    work->particles[i].x = Random16() & TRIG_ANGLE_MASK;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant = (i & 15) + 16;
  }
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Sound_PlayFar(SE_SPIN);

  for (frame = 0; frame != 56; frame++) {
    Projection_ScaledUnit(args->user, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = BLEND_ALPHA(112 - frame * 2, 16);
    if (frame == 26) {
      Sound_PlayFar(SE_POWER_UP);
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                               TINT_KEEP, 0, 20);
    }
    // The burst of light (48x48), seven images, three frames each.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas, work->sheet + (frame - 28) / 3 * (48 * 48) + 5120, 40,
              ground.y - 24, 48, 48);
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      DrawRectangle *slot =
          &((DrawRectangle *)gHeapSlots)[HEAP_SLOT_RAM_CODE_2];

      for (i = 0; i != 4; i++) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            gDeathPlunge_DrawFlags[i] | EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
        draw[1](canvas, work->sheet + (picture << 10), gDeathPlunge_X[i] + 32,
                ground.y + gDeathPlunge_Y[i] - 32, 32, 32);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      }
    }
    // The motes, shrinking as they close in.
    if (frame >= 0) {
      for (i = 0; i != 64; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant >= 0 && mote->y > 0) {
          s32 size = (mote->variant >> 3) + 1;
          s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
          s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

          /* FAKEMATCH(dead): size is at least 1 here; without this test, 224
           * of 349 instructions differ in tbs-en. */
          if (size <= 0)
            size = 1;
          EFFECT_DRAW_FLARE(draw[0], canvas, sheet, x, y, size);
          mote->y -= 2;
          mote->variant--;
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  // Finish, as for the element pillar.
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Draws a frame of the element streak burst: on its first frame seeds 256
/// streaks with random axes, then each frame draws the first 64 (streak i
/// from frame i / 4) as lines three pixels thick that shrink 4 pixels a
/// frame towards the centre of the canvas, a little below its middle.
void BattleEffect_DrawStreaks(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  s32 frame;
  s32 i;
  struct Streak *streak;
  struct Vector3 point;
  struct ScreenPoint head;
  struct ScreenPoint tail;

  frame = (work->frame)++;
  if (frame == 0) {
    for (i = 0; i != 256; i++) {
      struct Streak *seed = &((struct Streak *)Ram_MapGridCellBuffer)[i];
      s32 r = Random16() & 15;

      seed->head = r + 48;
      seed->tail = r + 40;
      seed->pitch = Random16() & TRIG_ANGLE_MASK;
      seed->yaw = Random16() & TRIG_ANGLE_MASK;
      seed->roll = Random16() & TRIG_ANGLE_MASK;
    }
  }
  point.y = 0;
  point.z = 0;
  for (i = 0; i != 64; i++) {
    streak = &((struct Streak *)Ram_MapGridCellBuffer)[i];
    if (frame > i / 4 && streak->head > 0) {
      s32 fade;

      Transform_Reset();
      Transform_ApplyRoll(streak->roll);
      Transform_ApplyPitch(streak->pitch);
      Transform_ApplyYaw(streak->yaw);
      point.x = streak->head;
      Projection_Point(&point, &head);
      head.x += 64;
      head.y += 80;
      point.x = streak->tail;
      Projection_Point(&point, &tail);
      tail.x += 64;
      tail.y += 80;
      streak->tail -= 4;
      streak->head -= 4;
      if (streak->tail < 0)
        streak->tail = 0;
      // The tail stops at the centre, and the head goes out once it passes it.
      fade = -streak->tail / 2;
      BattleEffect_DrawClippedCanvasLine(tail.x - 1, tail.y, head.x - 1, head.y,
                                         fade + 48);
      BattleEffect_DrawClippedCanvasLine(tail.x, tail.y - 1, head.x, head.y - 1,
                                         fade + 48);
      BattleEffect_DrawClippedCanvasLine(tail.x, tail.y, head.x, head.y,
                                         fade + 56);
    }
  }
  work->transferPending = 1;
}

/// Starts the element streak burst and returns at once: opens the canvas and
/// adds BattleEffect_DrawStreaks as a task, with a canvas that darkens each
/// frame.
void BattleEffect_StartElementStreaks(s32 element) {
  struct BattleEffectWork *work;
  void *palette;
  s32 id;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  switch (element) {
  case ELEMENT_VENUS:
    id = (s32)&ResourceId_YellowPaletteB;
    break;
  case ELEMENT_MERCURY:
    id = (s32)&ResourceId_CrystalSheet;
    break;
  case ELEMENT_MARS:
    id = (s32)&ResourceId_EmberStreakSheet;
    break;
  case ELEMENT_JUPITER:
    id = (s32)&ResourceId_VioletPaletteD;
    break;
  case ELEMENT_NONE:
  default:
    id = (s32)&ResourceId_DustPuffSheet;
    break;
  }
  palette = Resource_GetTableEntry(id);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  work->frame = 0;
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  // 6 levels off every pixel each frame, four pixels a word.
  work->transferValue = CANVAS_FILL_WORD(6);
  Task_Add(BattleEffect_DrawStreaks, TASK_PASS_FRAME);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
}

/// Ends the element streak burst that BattleEffect_StartElementStreaks
/// started: stops its tasks and the background palette fade, clears the
/// canvas in VRAM and frees the work and the canvas.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(BattleEffect_DrawStreaks);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(Palette_StepFadeTransfer);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

#undef DEATH_PLUNGE_FRAMES
#undef BLADE_RAIN_FRAMES
#undef ELEMENT_PILLAR_FRAMES
#undef SHATTERING_OPENING_PLOT_PIXEL
#undef SHATTERING_OPENING_PLOT_CORNERS
#undef SHATTERING_OPENING_FRAMES
#undef SHATTERING_OPENING_FLASHES
#undef SHATTERING_OPENING_PIECES
#undef SHATTERING_OPENING_DISC_X
#undef SHATTERING_OPENING_DISC_Y
