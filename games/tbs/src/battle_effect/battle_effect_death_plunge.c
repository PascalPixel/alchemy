/// @file
/// Death Plunge, the blade rain, the element pillar, the element burst and
/// the shattering opening.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// A word of two palette entries, both `color`.
#define COLOR_PAIR(color) ((color) << 16 | (color))

#define ELEMENT_BURST_FRAMES 56 ///< frames of the element burst
/// The battle background's first BG palette, of the eight its colours take.
#define BATTLE_BACKGROUND_PALETTE 6

// The element runners, the streaks and the shattering opening are reached from
// the battle through far calls, not the handler table, and take their own
// state, canvas and sprite sheet.

#define DEATH_PLUNGE_FRAMES 96
/// The plunge strikes: the target flashes, the user flashes and the damage
/// shows.
#define DEATH_PLUNGE_STRIKE_FRAME 54
/// How far toward the target the user runs, in hundredths of the way.
#define DEATH_PLUNGE_RUN_PERCENT 90
/// The flashes thrown in a ring after the strike.
#define DEATH_PLUNGE_RING_FLASHES 16

/// The user runs at the first target, both leap and turn upside down, and on
/// the hit they bounce apart while a ball of fire swells over them.
///
/// Abilities: Death Plunge.
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  struct BattleObject *first;
  struct BattleObject *second;
  struct Actor *user;
  struct Actor *target;
  FIXED x;
  FIXED z;
  s32 userHeight;
  s32 targetHeight;
  s32 point[3];
  struct ScreenPoint screen;
  s32 frame;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  state->transferMode = CANVAS_COPY_AND_FADE;
  state->transferValue = CANVAS_FADE_THREE_QUARTERS;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  // The run, most of the way to the target.
  first = BattleObject_GetFar(state->args->user);
  second = BattleObject_GetFar(state->args->targets[0]);
  user = first->actor;
  target = second->actor;
  {
    s32 percent = DEATH_PLUNGE_RUN_PERCENT;

    x = user->x.fixed + percent * (target->x.fixed - user->x.fixed) / 100;
    z = user->z.fixed + percent * (target->z.fixed - user->z.fixed) / 100;
  }
  userHeight = BattleUnit_GetEffectHeightFar(state->args->user);
  targetHeight = BattleUnit_GetEffectHeightFar(state->args->targets[0]);
  Actor_StartIdleFar(user);
  Actor_SetMoveTargetFar(user, x, 0, z);
  Actor_SelectAnimationFar(user, POSE_LEAP);
  user->snapToTarget = 1;
  user->steeringFlags = 1;
  user->acceleration = toFIXED(2);
  user->speedLimit = toFIXED(8);
  wait(20);
  for (frame = 0; frame != DEATH_PLUNGE_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);

    // Both leap at 15 pixels a frame.
    if (frame == 0) {
      target->velocityY = toFIXED(15);
      target->gravity = toFIXED(57) / 100;
      user->velocityY = toFIXED(15);
      user->gravity = toFIXED(57) / 100;
    }
    if (frame == 11) {
      target->scaleY = -target->scaleY;
      user->scaleY = -user->scaleY;
      user->y.fixed += userHeight;
      target->y.fixed += targetHeight;
    }

    // The target flashes and both bounce; the user heads back towards the
    // middle of the field.
    if (frame == DEATH_PLUNGE_STRIKE_FRAME) {
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 10);
      target->velocityY = toFIXED(8);
      target->gravity = BATTLE_UNIT_GRAVITY;
      user->velocityY = toFIXED(5);
      user->gravity = toFIXED(47) / 100;
      user->acceleration = toFIXED(1);
      user->speedLimit = toFIXED(2);
      user->steeringFlags = 0;
      Actor_StartIdleFar(user);
      Actor_SetMoveTargetFar(user, 0, 0, user->z.fixed);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    SET_VECTOR(point, user->x.fixed, user->y.fixed, user->z.fixed);
    Projection_Point(point, &screen);
    screen.x >>= 1;

    // One big flash on the user.
    if (frame == DEATH_PLUNGE_STRIKE_FRAME ||
        frame == DEATH_PLUNGE_STRIKE_FRAME + 1)
      draw[0](canvas, state->sheet, screen.x - FLASH_BURST_WIDTH / 2,
              screen.y - 16, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
    if (frame >= 56 && frame <= 67) {
      s32 i;

      for (i = 0; i != DEATH_PLUNGE_RING_FLASHES; i++) {
        s32 angle;
        s32 rx;
        s32 ry;

        angle = i * (TRIG_TURN / DEATH_PLUNGE_RING_FLASHES);
        rx = screen.x + FIXEDtoINT(Trig_Sin(angle) * (frame - 46));
        ry = FIXEDtoINT(Trig_Cos(angle) * (frame - 46)) - frame + 100;
        draw[0](canvas,
                state->sheet + (frame - 56) / 2 * PICTURE_BYTES(FLASH_BURST),
                rx - FLASH_BURST_WIDTH / 2, ry, FLASH_BURST_WIDTH,
                FLASH_BURST_HEIGHT);
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
    if (frame == DEATH_PLUNGE_STRIKE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 0) {
      Sound_PlayFar(SE_WHOOSH);
      state->shakeFrames = 6;
    }
    if (frame == 53)
      state->shakeFrames = 6;
    BattleCamera_Shake(16, 16);
    EFFECT_PRESENT_CANVAS(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define BLADE_RAIN_FRAMES 120
#define BLADE_RAIN_SWORDS 16
/// A sword drifts sideways 0 to 3 steps of two pixels a frame, and its picture
/// is its step's: velocityX is the step << BLADE_RAIN_STEP_SHIFT.
#define BLADE_RAIN_STEP_SHIFT 17
/// Where on the canvas the first sword waits for its turn; each four after it
/// wait a pixel higher.
#define BLADE_RAIN_WAIT_Y 24

/// Swords fall slanting in on the first target, one every 4 frames, and stick
/// in the ground.
UNUSED void BattleEffect_RunBladeRain(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle *blit;
  struct Particle *blade;
  s32 frame;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  state = *cursor++;
  canvas = *cursor;
  state->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  blit = draw;
  Resource_LoadAndDecompress(RESOURCE_SWORD_SLASH_SHEET, state->sheet, 1, 1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  Projection_ScaledUnit(state->args->targets[0], &screenPosition);
  REG_BG2X = (CANVAS_SIZE / 2 - screenPosition.x) << 8;

  // Each sword waits near the top of the canvas until its turn, then falls,
  // drifting towards the middle.
  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    s32 column;

    blade = &state->particles[i];
    column = Random16() % 96 + 16;
    blade->x = column;
    blade->y = toFIXED(BLADE_RAIN_WAIT_Y - i / 4);
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
    blade->velocityX <<= BLADE_RAIN_STEP_SHIFT;
    blade->velocityY = toFIXED(8);
    blade->x <<= 16;
  }
  Sound_PlayFar(SE_POWER_UP);
  for (frame = 0; frame != BLADE_RAIN_FRAMES; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame);
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(120 - frame);
      if (frame == 104)
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    }

    // The picture follows how steeply the sword falls; one drifting left is
    // mirrored.
    for (i = BLADE_RAIN_SWORDS - 1; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 picture;

      blade = &state->particles[i];
      drift = blade->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      picture = speed >> BLADE_RAIN_STEP_SHIFT;
      if (frame < i * 4 + 25) {
        u32 width;
        u32 height;

        // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
        width = gBladeRain_SwordWidths[picture];
        height = gBladeRain_SwordHeights[picture];
        blit[drift < 0](canvas,
                        state->sheet + gBladeRain_SwordPictures[picture],
                        FIXEDtoINT(blade->x) - (width >> 1),
                        FIXEDtoINT(blade->y) - (height >> 1), width, height);
#else
        blit[(u32)drift >> 31](
            canvas, state->sheet + gBladeRain_SwordPictures[picture],
            FIXEDtoINT(blade->x) -
                ((width = gBladeRain_SwordWidths[picture]) >> 1),
            FIXEDtoINT(blade->y) -
                ((height = gBladeRain_SwordHeights[picture]) >> 1),
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
        width = gBladeRain_SwordWidths[picture];
        height = gBladeRain_SwordHeights[picture];
        blit[drift < 0](
            canvas, state->sheet + gBladeRain_SwordPictures[picture],
            FIXEDtoINT(blade->x) - (width >> 1),
            FIXEDtoINT(blade->y) - (height >> 1), width, height - 4);
#else
        blit[(u32)drift >> 31](
            canvas, state->sheet + gBladeRain_SwordPictures[picture],
            FIXEDtoINT(blade->x) -
                ((width = gBladeRain_SwordWidths[picture]) >> 1),
            FIXEDtoINT(blade->y) -
                ((height = gBladeRain_SwordHeights[picture]) >> 1),
            width, height - 4);
#endif
      }
    }

    // The target flashes and the screen shakes every 4 frames.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 2);
      state->shakeFrames = 1;
      if ((frame & 7) == 0)
        Sound_PlayFar(SE_IMPACT);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS();
}

#define ELEMENT_PILLAR_FRAMES 56
/// IceTileSheet: the pillar's half in four pictures, then its glow in three.
#define ELEMENT_PILLAR_HALF_WIDTH 17
#define ELEMENT_PILLAR_HALF_HEIGHT 64
#define ELEMENT_PILLAR_GLOW_WIDTH 24
#define ELEMENT_PILLAR_GLOW_HEIGHT 43
#define ELEMENT_PILLAR_GLOW (4 * PICTURE_BYTES(ELEMENT_PILLAR_HALF))
/// The element pillar's and burst's canvas weight over the battle, in
/// sixteenths.
#define ELEMENT_BLEND_LEVEL 12
/// The motes both set up in PARTICLES; the pillar spirals the first
/// ELEMENT_PILLAR_MOTES up around it, each circling once in
/// ELEMENT_PILLAR_TURN_FRAMES.
#define ELEMENT_SPIRAL_MOTES 128
#define ELEMENT_PILLAR_MOTES 32
#define ELEMENT_PILLAR_TURN_FRAMES 64

/// A pillar of light in an element's colours stands at a unit's feet while
/// motes spiral up around it.
void BattleEffect_RunElementPillar(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct ScreenPoint screenPosition;
  s32 point[3];
  struct BattleEffectState *state;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  state = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  camera = gBattleCamera;
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  state->fadeStep = 0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_ICE_TILE_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (state->args->kind) {
  case ELEMENT_VENUS:
    palette = Resource_GetTableEntry(RESOURCE_YELLOW_PALETTE_A);
    break;
  case ELEMENT_MERCURY:
    palette = Resource_GetTableEntry(RESOURCE_ICE_TILE_SHEET);
    break;
  case ELEMENT_MARS:
    palette = Resource_GetTableEntry(RESOURCE_RED_PALETTE_A);
    break;
  default:
    palette = Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_A);
    break;
  }
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);

  for (i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
    PARTICLES[i].y = 0;
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
    PARTICLES[i].variant = -i;
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  state->args->variant++;
  if (state->args->variant <= 0)
    state->args->variant = 1;
  if (state->args->variant > 4)
    state->args->variant = 4;
  Sound_PlayFar(SE_POWER_UP);

  for (frame = 0; frame != ELEMENT_PILLAR_FRAMES; frame++) {
    Projection_Unit(state->args->user, &ground);
    REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frame * 2);
    if (frame == 16)
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                               TINT_KEEP, 0, 20);

    // The pillar, its right half mirrored.
    if (frame <= 55) {
      s32 n;
      u8 *glow;

      n = frame / 2 % 4;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
      draw[1](canvas, state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF), 47,
              ground.y - ELEMENT_PILLAR_HALF_HEIGHT, ELEMENT_PILLAR_HALF_WIDTH,
              ELEMENT_PILLAR_HALF_HEIGHT);
      n = frame / 4 % 3;
      glow = state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_GLOW) +
             ELEMENT_PILLAR_GLOW;
      draw[1](canvas, glow, 40, ground.y - 36, ELEMENT_PILLAR_GLOW_WIDTH,
              ELEMENT_PILLAR_GLOW_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
      n = frame / 2 % 4;
      draw[1](canvas, state->sheet + n * PICTURE_BYTES(ELEMENT_PILLAR_HALF), 64,
              ground.y - ELEMENT_PILLAR_HALF_HEIGHT, ELEMENT_PILLAR_HALF_WIDTH,
              ELEMENT_PILLAR_HALF_HEIGHT);
      draw[1](canvas, glow, 64, ground.y - 36, ELEMENT_PILLAR_GLOW_WIDTH,
              ELEMENT_PILLAR_GLOW_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    // DEAD: a lookup whose result is dropped; the ROM keeps the call.
    BattleObject_GetFar(state->args->user);
    EFFECT_VIEW_FROM_CAMERA(camera);

    for (i = 0; i != ELEMENT_PILLAR_MOTES; i++) {
      struct Particle *fleck = &PARTICLES[i];

      if (fleck->variant >= 0) {
        s32 size;
        s32 turnSteps = ELEMENT_PILLAR_TURN_FRAMES;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += TRIG_TURN / turnSteps;
        fleck->y += toFIXED(5);
        fleck->z += 64;
        Projection_Point(point, &screenPosition);
        screenPosition.x /= 2;
        size = (i & 1) + state->args->variant;
        EFFECT_DRAW_FLARE(draw[0], canvas, sheet, screenPosition.x,
                          screenPosition.y, size);
      }
      fleck->variant++;
    }
    EFFECT_END_FRAME(state);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Sets one pixel of the disc's mask to colour 2.
#define SHATTERING_OPENING_PLOT_PIXEL(x, y)                                    \
  (*(Ram_MapGridCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +        \
     (y) / 8 * 2048) = 2)

/// Plots one point of the disc's circle in four octants.
#define SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2)                        \
  (SHATTERING_OPENING_PLOT_PIXEL(x2, y2),                                      \
   SHATTERING_OPENING_PLOT_PIXEL(x2, y1),                                      \
   SHATTERING_OPENING_PLOT_PIXEL(x1, y2),                                      \
   SHATTERING_OPENING_PLOT_PIXEL(x1, y1))

#define SHATTERING_OPENING_FRAMES 128
#define SHATTERING_OPENING_FLASHES 32 ///< flashes thrown as the screen breaks
/// How long a flash floats up, its six pictures four frames each and the last
/// held.
#define SHATTERING_OPENING_FLASH_FRAMES 40
#define SHATTERING_OPENING_PIECES 33 ///< pieces the screen breaks into
#define SHATTERING_OPENING_DISC_X 96 ///< the centre of the opening disc
#define SHATTERING_OPENING_DISC_Y 60 ///< the centre of the opening disc

/// As a battle opens on the storm at Venus Lighthouse's aerie, the screen turns
/// white and breaks into pieces that fly apart and fall, while a
/// grey disc grows over it and the battle background's colours return.
void BattleEffect_RunShatteringOpening(struct BattleEffectParams *args) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED struct ScreenPoint position;
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleState *battle;
  struct BattleBackgroundState *view;
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

  state = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*state));
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  state->args = args;

  BattleEffect_ClearTintTimers();
  view->secondMode = BATTLE_BACKGROUND_FIXED;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;

  i = 0;
  for (k = 0; k != CANVAS_TILES; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * CANVAS_TILES + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + i) = entry;
    }
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_SHATTERING_OPENING_SHEET,
                             state->imageStorage, 1, 1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

  // The pieces fly out from the middle.
  startY = gShatteringOpening_PieceStartY;
  startX = gShatteringOpening_PieceStartX;
  radius = 0;
  piece = state->particles;
  for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
    FIXED x, y;
    s32 vx, vy;

    x = INTtoFIXED(*startX++);
    y = INTtoFIXED(*startY++);
    piece->x = x;
    piece->y = y;
    vx = (x - toFIXED(32)) >> 2;
    vy = (y - toFIXED(60)) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
    piece++;
  }

  Iwram_CopyWords(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  Iwram_FillWords(Ram_MapGridCellBuffer, BATTLE_BACKGROUND_BYTES,
                  CANVAS_FILL_WORD(1));
  view->busy = 1;
  state->savedBg1X = gBgScroll[1].x;
  state->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) |
                          BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31),
                      IO_WRITE_HALFWORD);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_PALETTE, 0),
                  BATTLE_BACKGROUND_COLORS * sizeof(u16),
                  COLOR_PAIR(RGB_WHITE));
  Sound_PlayFar(SE_POWER_UP);
  BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                           POSE_CAST, 0, 30);

  for (frame = 0; frame != SHATTERING_OPENING_FRAMES; frame++) {
    if (frame == 2)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 3)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 28)
      BattleEffect_TintBattler(state->args->targets[0], TINT_KEEP, POSE_CAST,
                               TINT_KEEP, 0);
    if (frame == 32)
      Sound_PlayFar(SE_MONSTER_ROAR);
    if (frame == 5)
      Sound_PlayFar(SE_CRASH);
    // Bring the battle background back on BG1.
    if (frame == 5) {
      gBgScroll[1].x = state->savedBg1X;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, -1);
    }

    // Ease the white back to the battle background's colours.
    if (frame > 7) {
      u16 *color;

      color = (u16 *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_PALETTE, 0);
      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
        s32 red = RGB_RED(*color);
        s32 green = RGB_GREEN(*color);
        s32 blue = RGB_BLUE(*color);
        s32 goalRed = RGB_RED(battle->palette[i]);
        s32 goalGreen = RGB_GREEN(battle->palette[i]);
        s32 goalBlue = RGB_BLUE(battle->palette[i]);

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

    // Grow the grey disc.
    if (frame <= 3) {
      s32 v;

      v = frame * 4 + 8;
      targetRadius = frame * 32;
      BG_PLTT[2] = RGB(v, v, v);
      for (; radius != targetRadius; radius++) {
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
          if (y2 > BATTLE_BACKGROUND_HEIGHT - 1)
            y2 = BATTLE_BACKGROUND_HEIGHT - 1;
          if (x1 < 0)
            x1 = 0;
          if (x2 > BATTLE_BACKGROUND_WIDTH - 1)
            x2 = BATTLE_BACKGROUND_WIDTH - 1;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);
          x1 = leftX + 1;
          x2 = octantX + SHATTERING_OPENING_DISC_X + 1;
          if (x1 < 0)
            x1 = 0;
          if (x2 > BATTLE_BACKGROUND_WIDTH - 1)
            x2 = BATTLE_BACKGROUND_WIDTH - 1;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);

          x2 = octantY + centreX;
          x1 = centreX - octantY;
          innerLeftX = x1;
          y2 = octantX + centreY;
          y1 = centreY - octantX;
          if (x1 < 0)
            x1 = 0;
          if (x2 > BATTLE_BACKGROUND_WIDTH - 1)
            x2 = BATTLE_BACKGROUND_WIDTH - 1;
          if (y1 < 0)
            y1 = 0;
          if (y2 > BATTLE_BACKGROUND_HEIGHT - 1)
            y2 = BATTLE_BACKGROUND_HEIGHT - 1;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);
          x1 = innerLeftX + 1;
          x2 = octantY + SHATTERING_OPENING_DISC_X + 1;
          if (x1 < 0)
            x1 = 0;
          if (x2 > BATTLE_BACKGROUND_WIDTH - 1)
            x2 = BATTLE_BACKGROUND_WIDTH - 1;
          SHATTERING_OPENING_PLOT_CORNERS(x1, x2, y1, y2);

          error -= octantY * 2 + 1;
          if (error < 0) {
            error += octantX * 2 - 2;
            octantX--;
          }
          octantY++;
        }
      }
      Iwram_CopyWords(BG_CHAR_BLOCK(2), Ram_MapGridCellBuffer,
                      BATTLE_BACKGROUND_BYTES);
    }

    // The pieces fly and fall.
    if (frame <= 50) {
      for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
        piece = &state->particles[i];
        draw[0](canvas,
                state->imageStorage + gShatteringOpening_PiecePictures[i],
                FIXEDtoINT(piece->x), FIXEDtoINT(piece->y),
                gShatteringOpening_PieceWidths[i],
                gShatteringOpening_PieceHeights[i]);
        if (frame > 3)
          Particle_AdvanceWithGravity2D(piece, PARTICLE_DAMPING_NONE,
                                        toFIXED(1) / 4);
      }
    }

    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > RGB_LEVEL_MAX)
        v = RGB_LEVEL_MAX;
      BG_PLTT[1] = RGB(v, v, v);
    }

    if (frame == 51) {
      Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1,
                                 0);
      for (i = 1; i != SHEET_PALETTE_COLORS; i++) {
        s32 v;

        v = i / 2;
        // DEAD: v is never below 0; the ROM keeps the test.
        if (v < 0)
          v = 0;
        BG_PLTT[i] = RGB(v, v / 2, v);
      }
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &state->particles[i];
        piece->x = INTtoFIXED((Random16() & 31) + 32);
        piece->y = INTtoFIXED((Random16() & 31) + 80);
        piece->velocityX = ((Random16() & 0x1ff) - 256) << 12;
        piece->velocityY = 0;
        piece->variant = 0;
      }
      state->transferMode = CANVAS_COPY_AND_FADE;
      state->transferValue = CANVAS_FADE_HALF;
    }

    // Flashes float up and fade.
    if (frame > 52) {
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &state->particles[i];
        if (frame >= i / 4 + 52 &&
            piece->variant < SHATTERING_OPENING_FLASH_FRAMES) {
          s32 picture;

          picture = piece->variant / 4;
          if (picture > 5)
            picture = 5;
          draw[1](canvas, state->sheet + picture * PICTURE_BYTES(FLASH_BURST),
                  FIXEDtoINT(piece->x) - FLASH_BURST_WIDTH / 2,
                  FIXEDtoINT(piece->y) - FLASH_BURST_HEIGHT / 2,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(piece, 60, -toFIXED(1) / 16);
          piece->variant++;
        }
      }
    }

    EFFECT_END_FRAME(state);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_TintBattler(state->args->targets[0], TINT_KEEP, POSE_STANDING,
                           TINT_KEEP, 0);
  gBgScroll[1].x = state->savedBg1X;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  wait(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// The element streaks seeded at the start, and those drawn.
#define STREAK_SEEDS 256
#define STREAKS 64
/// A streak's head starts 48 to 63 from the centre, its tail STREAK_LENGTH
/// behind it.
#define STREAK_START_DISTANCE 48
#define STREAK_LENGTH 8

/// One streak of light of the element streak burst, over a particle's entry.
struct Streak {
  s32 head;          ///< the head's distance from the centre
  s32 tail;          ///< the tail's distance; both close in 4 a frame
  s32 unusedZ;       ///< the Particle's z, which the streaks leave alone
  s32 pitch;         ///< the axis's turn, a random 16-bit angle
  s32 yaw;           ///< the axis's turn, a random 16-bit angle
  s32 roll;          ///< the axis's turn, a random 16-bit angle
  s32 unusedVariant; ///< the Particle's variant, which the streaks leave alone
};

/// CyanSparkSheet: five small sparks, then the burst's pictures.
#define ELEMENT_BURST_SPARK_WIDTH 32
#define ELEMENT_BURST_SPARK_HEIGHT 32
#define ELEMENT_BURST_BURST_WIDTH 48
#define ELEMENT_BURST_BURST_HEIGHT 48
#define ELEMENT_BURST_BURST (5 * PICTURE_BYTES(ELEMENT_BURST_SPARK))
/// A burst mote's shortest life in frames; by its index a mote lives up to
/// twice as long, less one.
#define ELEMENT_BURST_MOTE_LIFE 16

/// Sparks in an element's colours flare at four corners around a unit and motes
/// spiral in, then a burst of light plays over it.
void BattleEffect_RunElementBurst(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct BattleEffectState *state;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  state = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  // Setup, as for the element pillar.
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  state->fadeStep = 0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_CYAN_SPARK_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (state->args->kind) {
  case ELEMENT_VENUS:
    palette = Resource_GetTableEntry(RESOURCE_YELLOW_PALETTE_A);
    break;
  case ELEMENT_MERCURY:
    palette = Resource_GetTableEntry(RESOURCE_ICE_TILE_SHEET);
    break;
  case ELEMENT_MARS:
    palette = Resource_GetTableEntry(RESOURCE_RED_PALETTE_A);
    break;
  default:
    palette = Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_A);
    break;
  }
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);

  for (i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
    PARTICLES[i].y = toFIXED(128);
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + 1024;
    PARTICLES[i].variant = -i;
  }
  // A mote's x is its angle and y its distance from the user; its variant
  // counts down its life.
  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    state->particles[i].x = Random16() & TRIG_ANGLE_MASK;
    state->particles[i].y = (Random16() & 31) + 16;
    state->particles[i].variant =
        (i & (ELEMENT_BURST_MOTE_LIFE - 1)) + ELEMENT_BURST_MOTE_LIFE;
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Sound_PlayFar(SE_SPIN);

  for (frame = 0; frame != ELEMENT_BURST_FRAMES; frame++) {
    Projection_ScaledUnit(args->user, &ground);
    REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frame * 2);
    if (frame == 26) {
      Sound_PlayFar(SE_POWER_UP);
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                               TINT_KEEP, 0, 20);
    }
    if (frame >= 28 && frame <= 48)
      draw[0](canvas,
              state->sheet +
                  (frame - 28) / 3 * PICTURE_BYTES(ELEMENT_BURST_BURST) +
                  ELEMENT_BURST_BURST,
              40, ground.y - 24, ELEMENT_BURST_BURST_WIDTH,
              ELEMENT_BURST_BURST_HEIGHT);
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;

      for (i = 0; i != 4; i++) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            gElementBurst_SparkFlips[i] | EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
        draw[1](canvas,
                state->sheet + picture * PICTURE_BYTES(ELEMENT_BURST_SPARK),
                gElementBurst_SparkX[i] + 32,
                ground.y + gElementBurst_SparkY[i] - 32,
                ELEMENT_BURST_SPARK_WIDTH, ELEMENT_BURST_SPARK_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }
    if (frame >= 0) {
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        struct Particle *mote = &state->particles[i];

        if (mote->variant >= 0 && mote->y > 0) {
          s32 size = (mote->variant >> 3) + 1;
          s32 x = FIXEDtoINT(mote->y * Trig_Sin(mote->x)) + 64;
          s32 y = FIXEDtoINT(mote->y * Trig_Cos(mote->x)) + ground.y;

          // DEAD: never true, as the variant is not negative here; the ROM
          // keeps the test and the store.
          if (size <= 0)
            size = 1;
          EFFECT_DRAW_FLARE(draw[0], canvas, sheet, x, y, size);
          mote->y -= 2;
          mote->variant--;
        }
      }
    }
    EFFECT_END_FRAME(state);
  }

  // Finish, as for the element pillar.
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Draws a frame of the element streaks: lines of light closing in on the
/// centre of the canvas.
void BattleEffect_DrawStreaks(void) {
  struct BattleEffectState *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  s32 frame;
  s32 i;
  struct Streak *streak;
  struct Vector3 point;
  struct ScreenPoint head;
  struct ScreenPoint tail;

  frame = (state->frame)++;
  if (frame == 0) {
    for (i = 0; i != STREAK_SEEDS; i++) {
      struct Streak *seed = &((struct Streak *)Ram_MapGridCellBuffer)[i];
      s32 r = Random16() & 15;

      seed->head = r + STREAK_START_DISTANCE;
      seed->tail = r + STREAK_START_DISTANCE - STREAK_LENGTH;
      seed->pitch = Random16() & TRIG_ANGLE_MASK;
      seed->yaw = Random16() & TRIG_ANGLE_MASK;
      seed->roll = Random16() & TRIG_ANGLE_MASK;
    }
  }
  point.y = 0;
  point.z = 0;
  for (i = 0; i != STREAKS; i++) {
    streak = &((struct Streak *)Ram_MapGridCellBuffer)[i];
    if (frame > i / 4 && streak->head > 0) {
      s32 fade;

      Matrix_Initialize();
      Matrix_RotateZ(streak->roll);
      Matrix_RotateX(streak->pitch);
      Matrix_RotateY(streak->yaw);
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
  state->transferPending = 1;
}

/// Starts the element streaks, which then draw by themselves while the battle
/// runs on.
void BattleEffect_StartElementStreaks(s32 element) {
  struct BattleEffectState *state;
  void *palette;
  s32 id;

  state = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  BattleEffect_StartCanvasLayer(0);
  state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  switch (element) {
  case ELEMENT_VENUS:
    id = RESOURCE_YELLOW_PALETTE_B;
    break;
  case ELEMENT_MERCURY:
    id = RESOURCE_CRYSTAL_SHEET;
    break;
  case ELEMENT_MARS:
    id = RESOURCE_EMBER_STREAK_SHEET;
    break;
  case ELEMENT_JUPITER:
    id = RESOURCE_VIOLET_PALETTE_D;
    break;
  case ELEMENT_NONE:
  default:
    id = RESOURCE_DUST_PUFF_SHEET;
    break;
  }
  palette = Resource_GetTableEntry(id);
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  state->frame = 0;
  state->transferMode = CANVAS_COPY_AND_DARKEN;
  state->transferValue = CANVAS_FILL_WORD(6);
  Task_Add(BattleEffect_DrawStreaks, TASK_PASS_FRAME);
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
}

/// Ends the element streaks and frees what they took.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(BattleEffect_DrawStreaks);
  Task_Remove(BattleEffect_TransferCanvas);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(BattleEffect_DarkenBackgroundStep);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

#undef COLOR_PAIR
#undef BATTLE_BACKGROUND_PALETTE
#undef ELEMENT_BURST_FRAMES
#undef DEATH_PLUNGE_STRIKE_FRAME
#undef DEATH_PLUNGE_FRAMES
#undef DEATH_PLUNGE_RUN_PERCENT
#undef DEATH_PLUNGE_RING_FLASHES
#undef BLADE_RAIN_FRAMES
#undef BLADE_RAIN_SWORDS
#undef BLADE_RAIN_STEP_SHIFT
#undef BLADE_RAIN_WAIT_Y
#undef ELEMENT_PILLAR_FRAMES
#undef ELEMENT_BLEND_LEVEL
#undef ELEMENT_SPIRAL_MOTES
#undef ELEMENT_PILLAR_MOTES
#undef ELEMENT_PILLAR_TURN_FRAMES
#undef SHATTERING_OPENING_PLOT_PIXEL
#undef SHATTERING_OPENING_PLOT_CORNERS
#undef SHATTERING_OPENING_FRAMES
#undef SHATTERING_OPENING_FLASHES
#undef SHATTERING_OPENING_FLASH_FRAMES
#undef SHATTERING_OPENING_PIECES
#undef SHATTERING_OPENING_DISC_X
#undef SHATTERING_OPENING_DISC_Y
#undef STREAK_SEEDS
#undef STREAKS
#undef STREAK_START_DISTANCE
#undef STREAK_LENGTH
#undef ELEMENT_BURST_MOTE_LIFE
