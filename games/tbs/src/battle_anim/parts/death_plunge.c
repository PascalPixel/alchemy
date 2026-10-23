/// @file
/// Death Plunge, the blade rain, the element pillar, the element burst and
/// the shattering picture.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// - Death Plunge runs the user up to the first target; both leap, turn upside
//   down, and on the hit bounce apart while a ring of flashes rises from the
//   user.
// - The blade rain drops 16 swords, one every 4 frames, slanting in on the
//   first target and sticking in the ground. No animation table entry points at
//   it.
// - The element pillar (BattleAnim_RunElementPillar) stands a mirrored pillar
//   of light at a unit's feet while motes spiral up around it.
// - The element burst (BattleAnim_RunElementBurst) flares sparks at four
// corners
//   around a unit, draws motes spiralling in, then plays a burst of light
//   over it.
// - BattleAnim_RunShatteringOpening shatters a picture into 33 pieces as a
//   battle opens.
// - The element streaks (BattleAnim_StartElementStreaks) draw lines of light
//   closing in on a unit from a task while the battle runs on.
//
// The element runners, the streaks and the shattering opening are reached
// from the battle through far calls, not the handler table, and allocate
// their own work, canvas and sheet instead of using the shared ones.

#define DEATH_PLUNGE_FRAMES 96

/// The user runs 9/10 of the way to the first target, then both leap and turn
/// upside down in the air; on frame 54 the target is hit and both bounce back,
/// while flashes burst from the user and rise in a widening ring.
///
/// Abilities: Death Plunge (BATTLE_ANIM_DEATH_PLUNGE).
void BattleAnim_RunDeathPlunge(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  /* FAKEMATCH(dead): draw as a BlitterPair whose second word nothing uses,
   * for 4 bytes of frame; as one DrawRectangle, 7 of 363 instructions differ
   * in tbs-en. */
  BlitterPair draw;
  struct BattleObject *first;
  struct BattleObject *second;
  struct MotionObject *user;
  struct MotionObject *target;
  s32 x;
  s32 z;
  s32 userHeight;
  s32 targetHeight;
  s32 point[3];
  struct ScreenPoint screen;
  s32 frame;
  s32 percent;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  // Trails keep 3/4 a frame.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75;
  draw[0] = ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);
  // The run: the user moves to 90% of the way to the target, with an
  // acceleration of 2 pixels a frame per frame and a top speed of 8 pixels a
  // frame, and the animation waits 20 frames for it.
  first = GetBattleObjectSlotFar(work->effect->user);
  second = GetBattleObjectSlotFar(work->effect->targets[0]);
  user = first->object;
  target = second->object;
  percent = 90;
  x = user->x + percent * (target->x - user->x) / 100;
  z = user->z + percent * (target->z - user->z) / 100;
  userHeight = BattleUnit_GetEffectHeightFar(work->effect->user);
  targetHeight = BattleUnit_GetEffectHeightFar(work->effect->targets[0]);
  ObjectDispatch_StartIdleFar((struct DispatchObject *)user);
  Object_SetMoveTargetFar((struct Object *)user, x, 0, z);
  ObjectDispatch_SelectAnimationFar(user, POSE_LEAP);
  user->snapToTarget = 1;
  user->autoFaceMotion = 1;
  user->acceleration = 2 << 16;
  user->speedLimit = 8 << 16;
  WaitFrames(20);
  for (frame = 0; frame != DEATH_PLUNGE_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gCameraWork;
    ANIM_VIEW_FROM_CAMERA(camera);

    // Frame 0: both leap at 15 pixels a frame. Frame 11: both turn upside
    // down, raised by their own heights so they flip about their middles.
    // XXX: verticalMotionStrength's units (0x91eb here, 0xab85 and 0x7851 on
    // the bounce); the object motion code that reads it would settle them.
    if (frame == 0) {
      target->velocityY = 15 << 16;
      target->verticalMotionStrength = 0x91eb;
      user->velocityY = 15 << 16;
      user->verticalMotionStrength = 0x91eb;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y += userHeight;
      target->y += targetHeight;
    }

    // Frame 54: the target flashes and both bounce; the user heads back
    // towards the middle of the field.
    if (frame == 54) {
      ObjectGroup_UpdateMembers(work->effect->targets[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 10);
      target->velocityY = 8 << 16;
      target->verticalMotionStrength = 0xab85;
      user->velocityY = 5 << 16;
      user->verticalMotionStrength = 0x7851;
      user->acceleration = 1 << 16;
      user->speedLimit = 2 << 16;
      user->autoFaceMotion = 0;
      ObjectDispatch_StartIdleFar((struct DispatchObject *)user);
      Object_SetMoveTargetFar((struct Object *)user, 0, 0, user->z);
    }
    ANIM_VIEW_FROM_CAMERA(camera);
    SET_VECTOR(point, user->x, user->y, user->z);
    Projection_Point(point, &screen);
    screen.x >>= 1;

    // Frames 54 and 55: one big flash on the user. Frames 56 to 67: 16
    // flashes in a ring that widens as it rises, through six images, two
    // frames each (32x64, 0x800 bytes a picture).
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

    // Frame 64: both turn right side up again.
    if (frame == 64) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y -= userHeight;
      target->y -= targetHeight;
      ObjectDispatch_SelectAnimationFar(user, 0);
    }
    // The cues: the damage with a heavy impact on the hit, and a shake on
    // the leap and on the frame before the hit.
    if (frame == 54)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 0) {
      Sound_PlaySongFar(SE_136);
      work->shakeFrames = 6;
    }
    if (frame == 53)
      work->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    ANIM_PRESENT_FRAME(work);
  }
  ANIM_FINISH_CANVAS_ONE_BLITTER();
}

#define BLADE_RAIN_FRAMES 120

/// Drops 16 swords, one every 4 frames, slanting in towards the first target
/// (the middle of the canvas, x 64) and sticking in the ground, where their
/// bottom 4 pixels are hidden.
///
/// Sword i falls from frame 4i + 16 and sticks at frame 4i + 25. Every 4
/// frames from 24 to 84 the target flashes white in its damage pose for 2
/// frames and the screen shakes; every 8 frames the hit sound plays.
///
/// Nothing reaches it: no handler table entry or call names it. Places 64
/// swords in work->particles but draws only the first 16, and moves BG2X to
/// centre the canvas on the target.
UNUSED void BattleAnim_RunBladeRain(struct BattleAnimArgs *effect) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct Particle *blade;
  s32 frame;
  s32 i;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  ANIM_START_NARROW_CANVAS(1);
  // Invisible to start with.
  REG_BLDALPHA = BLEND_ALPHA(0, 16);

  // Setup: [0] draws as is, [1] mirrored; both skip colour 0.
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work->sheet, 1,
                             1);
  // Redraw from blank each frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  // Scroll the canvas so its middle column sits on the first target.
  Projection_ScaledUnit(work->effect->targets[0], &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Each sword waits at a random x from 16 to 111 near the top of the
  // canvas (sword i centred 24 - i / 4 pixels down, so the 16 drawn sit at
  // rows 24 to 21, each run of four a pixel higher than the last), drawn
  // there until its turn to fall, and drifts towards
  // the middle faster (in steps of 2 pixels a frame) the further out it
  // starts. The drift's size, 0 to 3 steps, also picks the sword's picture.
  for (i = 0; i != 64; i++) {
    s32 column;

    blade = &work->particles[i];
    column = Random16() % 96 + 16;
    blade->x = column;
    blade->y = (24 - i / 4) << 16;
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
    blade->velocityY = 8 << 16;
    blade->x <<= 16;
  }
  Sound_PlaySongFar(SE_212);
  for (frame = 0; frame != BLADE_RAIN_FRAMES; frame++) {
    // Frame loop. Fade in over the first 16 frames, with blending off once
    // opaque, and out over the last 16.
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

    // The image depends on how steeply the sword falls, and a sword
    // drifting left is drawn mirrored (the drift's sign bit picks the
    // blitter). Sword i falls from frame 4i + 16 and sticks in the ground
    // at frame 4i + 25; the last drawn first, so earlier swords lie on top.
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 cell;

      blade = &work->particles[i];
      drift = blade->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      cell = speed >> 17;
      if (frame < i * 4 + 25) {
        u32 width;
        u32 height;

        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gDeathPlunge_CellWidths[cell];
        height = gDeathPlunge_CellHeights[cell];
        blit[(u32)drift >> 31](canvas, work->sheet + gDeathPlunge_Cells[cell],
                               (blade->x >> 16) - (width >> 1),
                               (blade->y >> 16) - (height >> 1), width, height);
#else
        blit[(u32)drift >> 31](
            canvas, work->sheet + gDeathPlunge_Cells[cell],
            (blade->x >> 16) - ((width = gDeathPlunge_CellWidths[cell]) >> 1),
            (blade->y >> 16) - ((height = gDeathPlunge_CellHeights[cell]) >> 1),
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
        width = gDeathPlunge_CellWidths[cell];
        height = gDeathPlunge_CellHeights[cell];
        blit[(u32)drift >> 31](canvas, work->sheet + gDeathPlunge_Cells[cell],
                               (blade->x >> 16) - (width >> 1),
                               (blade->y >> 16) - (height >> 1), width,
                               height - 4);
#else
        blit[(u32)drift >> 31](
            canvas, work->sheet + gDeathPlunge_Cells[cell],
            (blade->x >> 16) - ((width = gDeathPlunge_CellWidths[cell]) >> 1),
            (blade->y >> 16) - ((height = gDeathPlunge_CellHeights[cell]) >> 1),
            width, height - 4);
#endif
      }
    }

    // Every 4 frames from 24 to 84 the target flashes white in its damage
    // pose for 2 frames and the screen shakes; every 8 frames the hit
    // sound plays.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      ObjectGroup_UpdateMembers(work->effect->targets[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        Sound_PlaySongFar(SE_IMPACT);
    }
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }
  ANIM_FINISH_CANVAS();
}

#define ELEMENT_PILLAR_FRAMES 56

/// A symmetrical pillar of light in an element's colours stands at a unit's
/// feet while motes spiral up around it, for 56 frames, and the first target
/// flashes.
///
/// Called through BattleAnim_RunElementPillarFar by
/// ElementGlow_Play (GLOW_PILLAR) and four times in a row, one
/// for each element, by AutoBattle_RunAttackOrItem. Allocates and frees
/// its own work, canvas and sheet slots; loads the element's colours over the
/// first 64 of the background palette, fills PARTICLES 0 to 127 and moves
/// BG2X with the unit. Increments effect->variant in place, so
/// AutoBattle_RunAttackOrItem's four calls draw ever larger motes.
///
/// effect->kind    : the element (enum Element), which picks the palette:
///                   Venus yellow, Mercury blue, Mars red, others Jupiter
///                   violet.
/// effect->variant : the mote size less one, kept to 1 to 4 after adding 1.
void BattleAnim_RunElementPillar(struct BattleAnimArgs *effect) {
  struct ScreenPoint ground;
  struct ScreenPoint screenPosition;
  s32 point[3];
  struct BattleAnimWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  // The sheet slot takes ParticleSpritesD's flare cells, which
  // ANIM_DRAW_FLARE reads: the ten squares of 2x2 to 20x20 are 1540 of its
  // 0x60e (1550) bytes.
  work = (struct BattleAnimWork *)AllocHeapSlot(HEAP_SLOT_BATTLE_ANIM,
                                                sizeof *work);
  canvas = (void *)AllocHeapSlot(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = (u8 *)AllocHeapSlot(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  camera = gCameraWork;
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  // Palette_StepFadeTransfer darkens the battle background for 24 frames.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  // The narrow canvas, 12/16 over all of the background.
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (work->effect->kind) {
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
    PARTICLES[i].x = Random16() & 0xffff;
    PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
    PARTICLES[i].variant = -i;
  }
  /* FAKEMATCH(dead): two updates of work that cancel, raising its priority in
   * allocation; without them, 25 of 420 instructions differ in tbs-en. */
  work++;
  work--;
  // Trails keep 3/4 a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
  draw[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
  // The mote size, written back into the caller's argument.
  work->effect->variant++;
  if (work->effect->variant <= 0)
    work->effect->variant = 1;
  if (work->effect->variant > 4)
    work->effect->variant = 4;
  Sound_PlaySongFar(SE_212);

  for (frame = 0; frame != ELEMENT_PILLAR_FRAMES; frame++) {
    // Frame loop. Keep the canvas centred on the unit's feet, fade out over
    // the last 6 frames, and flash the first target from frame 16 for 20
    // frames.
    Projection_Unit(work->effect->user, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = BLEND_ALPHA(112 - frame * 2, 16);
    if (frame == 16)
      ObjectGroup_UpdateMembers(work->effect->targets[0], SPRITE_COLOR_WHITE,
                                -1, 0, 20);

    // The pillar: its left half at x 47 and its mirrored right half at x
    // 64, keeping the brighter pixel. The pillar (17x64) cycles through four
    // images, two frames each; the glow at its foot (24x43) through three,
    // four frames each.
    if (frame <= 55) {
      s32 n;
      u8 *glow;

      i = frame / 2;
      n = i % 4;
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, BLITTER_MAX);
      draw[1] = ((DrawRectangle *)gHeapSlots)[HEAP_SLOT_RAM_CODE_2];
      draw[1](canvas, work->sheet + n * (17 * 64), 47, ground.y - 64, 17, 64);
      n = frame / 4 % 3;
      glow = work->sheet + n * 1032 + 0x1100;
      draw[1](canvas, glow, 40, ground.y - 36, 24, 43);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      draw[1] = ((DrawRectangle *)gHeapSlots)[HEAP_SLOT_RAM_CODE_2];
      n = i % 4;
      draw[1](canvas, work->sheet + n * (17 * 64), 64, ground.y - 64, 17, 64);
      draw[1](canvas, glow, 64, ground.y - 36, 24, 43);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
    }
    // A lookup with no side effect whose result is dropped; the ROM keeps
    // the call.
    GetBattleObjectSlotFar(work->effect->user);
    ANIM_VIEW_FROM_CAMERA(camera);

    // The first 32 motes, each starting i frames in: each frame a mote
    // turns 0x400 (about 5.6 degrees), rises 5 pixels and moves 4 pixels
    // further out. The other 96 are placed but never drawn.
    for (i = 0; i != 32; i++) {
      struct Particle *fleck = &PARTICLES[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += 0x400;
        fleck->y += 5 << 16;
        fleck->z += 64;
        Projection_Point(point, &screenPosition);
        screenPosition.x /= 2;
        size = (i & 1) + work->effect->variant;
        ANIM_DRAW_FLARE(draw[0], canvas, sheet, screenPosition.x,
                        screenPosition.y, size);
      }
      fleck->variant++;
    }
    ANIM_END_FRAME(work);
  }

  // Finish: the blitter, the callback and the layer, then the three slots
  // in the reverse of the order they were taken.
  FreeHeapSlot(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
  FreeHeapSlot(HEAP_SLOT_BATTLE_SHEET);
  FreeHeapSlot(HEAP_SLOT_BATTLE_CANVAS);
  FreeHeapSlot(HEAP_SLOT_BATTLE_ANIM);
  /* FAKEMATCH(dead): two updates of frame that cancel, which order two stack
   * slots; without them, 9 of 420 instructions differ in tbs-en. */
  frame ^= 1;
  frame ^= 1;
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
/// battle background's colours, and from frame 51 32 flashes rise. Runs for 128
/// frames.
///
/// Called through BattleAnim_RunShatteringOpeningFar by
/// BattleTransition_Run, as a battle opens. Allocates and
/// frees its own work, canvas and sheet slots; sets up BG2 as the canvas
/// itself, as BattleAnim_StartCanvasLayer would; draws the disc in
/// Ram_MapGridCellBuffer and copies it over 0x7800 bytes of BG character block
/// 2; writes background colours 1 to 63 and palettes 6 to 13; reloads the
/// battle background on BG1 on frame 5, and on its way out restores
/// gBgScroll[1], starts the battle's palette fade to BATTLE_BACKGROUND_ZOOMED
/// and queues a DISPCNT without BG1.
///
/// effect : only effect->actors[0], the unit that flashes, is read.
void BattleAnim_RunShatteringOpening(struct BattleAnimArgs *effect) {
  /* FAKEMATCH(dead): an unused position for the frame's 12 bytes at sp+44;
   * without it, 2 of 1051 instructions differ in tbs-en. */
  UNUSED struct ScreenPoint position;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleWork *battle;
  struct BattleViewWork *view;
  void **slots;
  void **slotTable;
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
  s32 size;
  s32 leftX;
  s32 innerLeftX;
  s32 y1;
  s32 x2;
  s32 y2;
  u8 *startY;
  u8 *startX;
  s32 centreX;
  s32 centreY;

  work = AllocHeapSlot(HEAP_SLOT_BATTLE_ANIM, sizeof(*work));
  size = CANVAS_BYTES;
  canvas = AllocHeapSlot(HEAP_SLOT_BATTLE_CANVAS, size);
  AllocHeapSlot(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  slots = gHeapSlots + HEAP_SLOT_BATTLE;
  battle = slots[0];
  view = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
  work->effect = effect;

  // Setup. Make BG2 the 128x128 canvas, as BattleAnim_StartCanvasLayer does
  // but without its waits: twice the width, 16 lines down, inside both
  // windows.
  BattleAnim_ClearActorTimers();
  view->secondMode = BATTLE_BACKGROUND_FIXED;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  // Aim the camera at the field's centre at 100% zoom, lining the battle
  // background's perspective plane up with it.
  BattleCamera_SetViewFar(0, 0, 0, 100);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  ANIM_SET_CANVAS_AFFINE();
  ANIM_SET_BATTLE_WINDOWS();
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = ANIM_WINOUT;

  // BG2's affine map, at 0x06003800 (screen block 7), shows tiles 0 to 255
  // in order, so the canvas appears as one picture; each halfword holds two
  // one-byte tile numbers.
  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
      *(volatile s16 *)(0x06003800 + i) = entry;
    }
  }

  Iwram_ClearWords(canvas, size);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), size);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, ANIM_DISPCNT, 0x20000);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask,
                             work->imageStorage, 1, 1);
  // Redraw from blank each frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  slotTable = gHeapSlots;
  draw[0] = slotTable[HEAP_SLOT_RAM_CODE];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  draw[1] = slotTable[HEAP_SLOT_RAM_CODE_2];

  // Each piece flies out from (32, 60) at a quarter of its distance from
  // there a frame, in work->particles 0 to 32.
  startY = gDeathPlunge_PieceStartY;
  startX = gDeathPlunge_PieceStartX;
  radius = 0;
  piece = work->particles;
  for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
    s32 x, y, vx, vy;

    x = *startX++ << 16;
    y = *startY++ << 16;
    piece->x = x;
    piece->y = y;
    vx = (x - 0x200000) >> 2;
    vy = (y - 0x3c0000) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
    piece++;
  }

  // The disc's mask in Ram_MapGridCellBuffer starts as colour 1 everywhere (the
  // copy of the screen into it is overwritten at once), and background
  // palettes 6 to 13 turn white. BG1's scroll is saved and the battle's
  // background view marked busy while the battle background is replaced.
  Iwram_CopyWords(Ram_MapGridCellBuffer, (void *)BG_CHAR_BLOCK(2), 0x7800);
  Iwram_FillWords(Ram_MapGridCellBuffer, 0x7800, 0x01010101);
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) |
                          BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31),
                      0x20000);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(6, 0), 0x100, 0x7fff7fff);
  Sound_PlaySongFar(SE_212);
  ObjectGroup_UpdateMembers(work->effect->targets[0], SPRITE_COLOR_WHITE,
                            POSE_CAST, 0, 30);

  for (frame = 0; frame != SHATTERING_OPENING_FRAMES; frame++) {
    // Frame loop. The cues: the shatter sound three times by frame 3, the
    // unit's flash ending on frame 28, and two more sounds on 5 and 32.
    if (frame == 2)
      Sound_PlaySongFar(SE_212);
    if (frame == 3)
      Sound_PlaySongFar(SE_212);
    if (frame == 28)
      ObjectGroup_UpdateMembers(work->effect->targets[0], -1, POSE_CAST, -1, 0);
    if (frame == 32)
      Sound_PlaySongFar(SE_149);
    if (frame == 5)
      Sound_PlaySongFar(SE_145);
    // Frame 5: bring the battle background back on BG1. Level -1 leaves
    // palettes 6 to 13 white for the fade below, which steps them towards
    // battle->palette, the colours the load fills.
    if (frame == 5) {
      gBgScroll[1].x = work->savedBg1X;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, -1);
    }

    // From frame 8, move every colour of palettes 6 to 13 one step towards
    // the battle background's colour; from white, 31 frames bring every
    // channel home.
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

    // Frame 4: the whole screen turns the disc's grey.
    if (frame == 4)
      Iwram_FillWords((void *)BG_CHAR_BLOCK(2), 0x7800, 0x02020202);

    // Frames 0 to 3: grow the disc in the mask by 32 pixels of radius,
    // brighten its grey (colour 2), and copy the mask to the screen.
    if (frame <= 3) {
      s32 v;

      v = frame * 4 + 8;
      targetRadius = frame * 32;
      BG_PLTT[2] = RGB(v, v, v);
      centreX = SHATTERING_OPENING_DISC_X;
      centreY = SHATTERING_OPENING_DISC_Y;
      for (; radius != targetRadius; radius++) {
        // The midpoint circle algorithm: one point plots all eight
        // octants, and each is drawn twice, one pixel apart, to fill gaps.
        // Points are clamped to the 256x120 picture; the second copy's x2
        // is measured from SHATTERING_OPENING_DISC_X, centreX's value.
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
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapGridCellBuffer, 0x7800);
    }

    // Draw the pieces up to frame 50; they start to fly and fall on frame
    // 4, undamped, under a gravity of a quarter pixel a frame.
    if (frame <= 50) {
      for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
        piece = &work->particles[i];
        draw[0](canvas, work->imageStorage + gDeathPlunge_PieceCells[i],
                Q_16_16_TO_INT(piece->x), Q_16_16_TO_INT(piece->y),
                gDeathPlunge_PieceWidths[i], gDeathPlunge_PieceHeights[i]);
        if (frame > 3)
          Particle_AdvanceWithGravity2D(piece, 64, 0x4000);
      }
    }

    // Frames 8 to 50: colour 1 goes from black to white, one step a frame,
    // and is white from frame 39.
    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > 31)
        v = 31;
      BG_PLTT[1] = RGB(v, v, v);
    }

    // Frame 51: load the flash images and a violet ramp into colours 1 to
    // 63, turn blending on, and place 32 flashes at random in x 32 to 63, y
    // 80 to 111, reusing the pieces' particles. Each is thrown sideways at up
    // to 16 pixels a frame either way, which the 60/64 damping below soon
    // slows.
    if (frame == 51) {
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet,
                                 1, 0);
      for (i = 1; i != 64; i++) {
        s32 v;

        v = i / 2;
        if (v < 0)
          v = 0;
        BG_PLTT[i] = RGB(v, v / 2, v);
      }
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        piece->x = ((Random16() & 31) + 32) << 16;
        piece->y = ((Random16() & 31) + 80) << 16;
        piece->velocityX = ((Random16() & 0x1ff) - 0x100) << 12;
        piece->velocityY = 0;
        piece->variant = 0;
      }
      // Trails keep half a frame.
      work->transferMode = CANVAS_COPY_AND_FADE;
      work->transferValue = 50;
    }

    // Flashes: flashes 0 to 7 appear on frame 53 and four more each frame
    // after, and each floats up (a gravity of -1/16 pixel a frame per frame)
    // for 40 frames, through six images of 4 frames, holding the last.
    if (frame > 52) {
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        if (frame >= i / 4 + 52 && piece->variant <= 39) {
          s32 cell;

          cell = piece->variant / 4;
          if (cell > 5)
            cell = 5;
          draw[1](canvas, work->sheet + (cell << 11),
                  Q_16_16_TO_INT(piece->x) - 16, Q_16_16_TO_INT(piece->y) - 32,
                  32, 64);
          Particle_AdvanceWithGravity2D(piece, 60, -0x1000);
          piece->variant++;
        }
      }
    }

    ANIM_END_FRAME(work);
  }

  // Finish: restore the unit, BG1's scroll and the battle background's palette
  // fade, then free the slots in the reverse of the order they were taken.
  ANIM_RELEASE_BLITTERS();
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  ObjectGroup_UpdateMembers(work->effect->targets[0], -1, POSE_STANDING, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  WaitFrames(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  FreeHeapSlot(HEAP_SLOT_BATTLE_SHEET);
  FreeHeapSlot(HEAP_SLOT_BATTLE_CANVAS);
  FreeHeapSlot(HEAP_SLOT_BATTLE_ANIM);
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
///
/// Called through BattleAnim_RunElementBurstFar by
/// ElementGlow_Play (GLOW_BURST). Allocates and frees its own
/// work, canvas and sheet slots; loads the element's colours over the first
/// 64 of the background palette, fills PARTICLES 0 to 127 and
/// work->particles, and moves BG2X with the unit.
///
/// effect->kind : the element (enum Element), which picks the palette: Venus
///                yellow, Mercury blue, Mars red, others Jupiter violet.
void BattleAnim_RunElementBurst(struct BattleAnimArgs *effect) {
  struct ScreenPoint ground;
  struct BattleAnimWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  work = AllocHeapSlot(HEAP_SLOT_BATTLE_ANIM, sizeof *work);
  canvas = AllocHeapSlot(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = AllocHeapSlot(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  // Setup, as for the element pillar: the battle background darkens for 24
  // frames, and the narrow canvas goes 12/16 over all of the background.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (work->effect->kind) {
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
  {
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // PARTICLES are set up as for the element pillar but never drawn here.
  // The motes that are drawn, work->particles, start 16 to 47 pixels out
  // (y) at a random angle (x) and close in 2 pixels a frame for 16 to 31
  // frames (variant).
  for (i = 0; i != 128; i++) {
    PARTICLES[i].y = 0x800000;
    PARTICLES[i].x = Random16() & 0xffff;
    PARTICLES[i].z = (Random16() & 0x1ff) + 0x400;
    PARTICLES[i].variant = -i;
  }
  for (i = 0; i != 64; i++) {
    work->particles[i].x = Random16() & 0xffff;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant = (i & 15) + 16;
  }
  // Trails keep 3/4 a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  Sound_PlaySongFar(SE_140);

  for (frame = 0; frame != 56; frame++) {
    // Frame loop. Follow the unit, fade out over the last 6 frames, and on
    // frame 26 flash the first target for 20 frames.
    Projection_ScaledUnit(effect->user, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = BLEND_ALPHA(0x70 - frame * 2, 16);
    if (frame == 26) {
      Sound_PlaySongFar(SE_212);
      ObjectGroup_UpdateMembers(work->effect->targets[0], SPRITE_COLOR_WHITE,
                                -1, 0, 20);
    }
    // Frames 28 to 48: the burst of light (48x48), seven images, three
    // frames each.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas, work->sheet + (frame - 28) / 3 * (48 * 48) + 0x1400, 40,
              ground.y - 24, 48, 48);
    // Frames 0 to 14: a spark (32x32) at each corner, flipped to face
    // outwards (gDeathPlunge_DrawFlags), five images, three frames each; a
    // blitter is built for each corner and freed at once.
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      DrawRectangle *slot =
          &((DrawRectangle *)gHeapSlots)[HEAP_SLOT_RAM_CODE_2];

      for (i = 0; i != 4; i++) {
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          gDeathPlunge_DrawFlags[i] | ANIM_CLIP_BOUNDS,
                          BLITTER_MAX);
        draw[1] = *slot;
        draw[1](canvas, work->sheet + (picture << 10), gDeathPlunge_X[i] + 32,
                ground.y + gDeathPlunge_Y[i] - 32, 32, 32);
        FreeHeapSlot(HEAP_SLOT_RAM_CODE_2);
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

          if (size <= 0)
            size = 1;
          ANIM_DRAW_FLARE(draw[0], canvas, sheet, x, y, size);
          mote->y -= 2;
          mote->variant--;
        }
      }
    }
    ANIM_END_FRAME(work);
  }

  // Finish, as for the element pillar.
  FreeHeapSlot(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
  FreeHeapSlot(HEAP_SLOT_BATTLE_SHEET);
  FreeHeapSlot(HEAP_SLOT_BATTLE_CANVAS);
  FreeHeapSlot(HEAP_SLOT_BATTLE_ANIM);
}

/// Draws a frame of the element streak burst: on its first frame seeds 256
/// streaks with random axes, then each frame draws the first 64 (streak i
/// from frame i / 4) as lines three pixels thick that shrink 4 pixels a
/// frame towards the centre of the canvas, a little below its middle.
///
/// Runs each frame as a task added by
/// BattleAnim_StartElementStreaks and removed by
/// BattleAnim_EndElementStreaks. Counts work->frame, keeps the streaks in
/// Ram_MapGridCellBuffer, draws into the canvas through
/// BattleAnim_DrawClippedCanvasLine, resets the render transform and sets
/// work->transferPending.
void BattleAnim_DrawStreaks(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
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
      seed->pitch = Random16() & 0xffff;
      seed->yaw = Random16() & 0xffff;
      seed->roll = Random16() & 0xffff;
    }
  }
  point.y = 0;
  point.z = 0;
  i = 0;
  for (; i != 64; i++) {
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
      // The tail stops at the centre, and the head goes out once it
      // passes it. The colour level is 48 (the outer two lines) or 56
      // (the middle one) less half the tail's distance, so a line
      // brightens as its tail closes in.
      fade = -streak->tail / 2;
      BattleAnim_DrawClippedCanvasLine(tail.x - 1, tail.y, head.x - 1, head.y,
                                       fade + 48);
      BattleAnim_DrawClippedCanvasLine(tail.x, tail.y - 1, head.x, head.y - 1,
                                       fade + 48);
      BattleAnim_DrawClippedCanvasLine(tail.x, tail.y, head.x, head.y,
                                       fade + 56);
    }
  }
  work->transferPending = 1;
}

/// Starts the element streak burst and returns at once: opens the canvas and
/// adds BattleAnim_DrawStreaks as a task, with a canvas that darkens each
/// frame.
///
/// Called through BattleAnim_StartElementStreaksFar by
/// ElementGlow_Play (GLOW_STREAKS), which keeps the canvas on
/// its unit and ends the burst with BattleAnim_EndElementStreaks. Allocates
/// the work and canvas slots, which the end frees; loads the element's
/// colours over the first 64 of the background palette; adds
/// BattleAnim_DrawStreaks and the canvas transfer as tasks, beside
/// the battle background's palette fade BattleAnim_StartCanvasLayer adds.
///
/// element: the element (enum Element), which picks the palette: Venus
///          yellow, Mercury blue, Mars red, Jupiter violet, and a dusty grey
///          for none.
void BattleAnim_StartElementStreaks(s32 element) {
  struct BattleAnimWork *work;
  void *palette;
  s32 id;

  work = AllocHeapSlot(HEAP_SLOT_BATTLE_ANIM, sizeof *work);
  AllocHeapSlot(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  BattleAnim_StartCanvasLayer(0);
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
  work->transferValue = 0x06060606;
  Task_Add(BattleAnim_DrawStreaks, ANIM_PLAYBACK_ORDER);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);
}

/// Ends the element streak burst that BattleAnim_StartElementStreaks
/// started: stops its callbacks and the background palette fade, clears the
/// canvas in VRAM and frees the work and the canvas.
///
/// Called through BattleAnim_EndElementStreaksFar by
/// ElementGlow_Play. Unlike BattleAnim_EndCanvasLayer it leaves
/// BG2's settings and the battle background's colours as they are.
void BattleAnim_EndElementStreaks(void) {
  Task_Remove(BattleAnim_DrawStreaks);
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  {
    Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  }
  Task_Remove(Palette_StepFadeTransfer);
  FreeHeapSlot(HEAP_SLOT_BATTLE_CANVAS);
  FreeHeapSlot(HEAP_SLOT_BATTLE_ANIM);
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
