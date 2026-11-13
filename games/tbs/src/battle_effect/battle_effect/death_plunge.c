/// @file
/// Death Plunge, the blade rain, the element pillar, the element burst and
/// the shattering picture.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// The element runners, the streaks and the shattering opening are reached from
// the battle through far calls, not the handler table, and take their own work,
// canvas and sheet.

#define DEATH_PLUNGE_FRAMES 96

/// The user runs at the first target, both leap and turn upside down, and on
/// the hit they bounce apart while a ball of fire swells over them.
///
/// Abilities: Death Plunge.
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
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // The run to 9/10 of the way to the target.
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

/// Sixteen swords fall slanting in on the first target, one every 4 frames, and
/// stick in the ground.
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
  REG_BLDALPHA = BLEND_ALPHA(0, 16);

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  blit = draw;
  Resource_LoadAndDecompress(RESOURCE_SWORD_SLASH_SHEET, work->sheet, 1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Projection_ScaledUnit(work->args->targets[0], &screenPosition);
  REG_BG2X = (64 - screenPosition.x) << 8;

  // Each sword waits near the top of the canvas until its turn, then falls,
  // drifting towards the middle.
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

    // The picture follows how steeply the sword falls; one drifting left is
    // mirrored.
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

    // The target flashes and the screen shakes every 4 frames.
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

/// A pillar of light in an element's colours stands at a unit's feet while
/// motes spiral up around it.
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

  work = (struct BattleEffectWork *)HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT,
                                                   sizeof *work);
  canvas = (void *)HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = (u8 *)HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  camera = gBattleCamera;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_ICE_TILE_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (work->args->kind) {
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
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  for (i = 0; i != 128; i++) {
    PARTICLES[i].y = 0;
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + i * 2;
    PARTICLES[i].variant = -i;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
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

    // The pillar, its right half mirrored.
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

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
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
#define SHATTERING_OPENING_FLASHES 32 ///< flashes thrown as the picture breaks
#define SHATTERING_OPENING_PIECES 33  ///< pieces the picture breaks into
#define SHATTERING_OPENING_DISC_X 96  ///< the centre of the opening disc
#define SHATTERING_OPENING_DISC_Y 60  ///< the centre of the opening disc

/// As a battle opens on the storm at Venus Lighthouse's aerie, the screen turns
/// white and the picture breaks into 33 pieces that fly apart and fall, while a
/// grey disc grows over it and the battle background's colours return.
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
  BattleCamera_SetViewFar(0, 0, 0, 100);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;

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
  Resource_LoadAndDecompress(RESOURCE_TILE_ANIMATION_MASK, work->imageStorage,
                             1, 1);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  // The pieces fly out from the middle.
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

    // Ease palettes 6 to 13 back to the battle background's colours.
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

    // The pieces fly and fall.
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

    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > 31)
        v = 31;
      BG_PLTT[1] = RGB(v, v, v);
    }

    if (frame == 51) {
      Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
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
      work->transferMode = CANVAS_COPY_AND_FADE;
      work->transferValue = CANVAS_FADE_HALF;
    }

    // Flashes float up and fade.
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

/// One streak of light of the element streak burst, over a particle's entry.
struct Streak {
  s32 head;     ///< the head's distance from the centre, 48 to 63 at first
  s32 tail;     ///< the tail's distance, 8 less; both close in 4 a frame
  s32 unused08; ///< the Particle's z, which the streaks leave alone
  s32 pitch;    ///< the axis's turn, a random 16-bit angle
  s32 yaw;      ///< the axis's turn, a random 16-bit angle
  s32 roll;     ///< the axis's turn, a random 16-bit angle
  s32 unused18; ///< the Particle's variant, which the streaks leave alone
};

/// Sparks in an element's colours flare at four corners around a unit and motes
/// spiral in, then a burst of light plays over it.
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
  Resource_LoadAndDecompress(RESOURCE_CYAN_SPARK_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (work->args->kind) {
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
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

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

/// Draws a frame of the element streaks: lines of light closing in on the
/// centre of the canvas.
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

/// Starts the element streaks, which then draw by themselves while the battle
/// runs on.
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
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  work->frame = 0;
  work->transferMode = CANVAS_COPY_AND_DARKEN;
  work->transferValue = CANVAS_FILL_WORD(6);
  Task_Add(BattleEffect_DrawStreaks, TASK_PASS_FRAME);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
}

/// Ends the element streaks and frees what they took.
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
