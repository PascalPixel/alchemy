/// @file
/// Cruel Ruin, the shattering opening and the element burst, and the
/// light-streak backdrop with its halved canvas and dimming background.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "resource_ids.h"

/// Puts colour 2 at (x, y) in the 256x120 8bpp tile sheet (32 tiles wide) in
/// the map cell buffer.
#define PLOT(x, y)                                                             \
  (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +            \
     (y) / 8 * 2048) = 2)

#define SHATTERING_OPENING_FRAMES 128

#define SHATTERING_OPENING_FLASHES 32 ///< flashes thrown as the screen breaks
/// How long a flash floats up, its six pictures four frames each and the last
/// held.
#define SHATTERING_OPENING_FLASH_FRAMES 40
#define SHATTERING_OPENING_PIECES 33 ///< pieces the screen breaks into
#define SHATTERING_OPENING_DISC_X 96 ///< the centre of the opening disc
#define SHATTERING_OPENING_DISC_Y 60 ///< the centre of the opening disc

/// As the battle with the Doom Dragon opens over the Serpent's stars, the
/// screen turns white and breaks into pieces that fly apart and
/// fall, while a grey disc grows over it and the battle background's colours
/// return.
void BattleEffect_RunShatteringOpening(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleWork *battle;
  s32 j;
  s32 k;
  s32 rmax;
  s32 r;
  s32 d;
  struct BattleBackgroundWork *view;
  struct Particle *piece;
  s32 i;
  s32 px;
  s32 py;
  s32 x1;
  s32 left;
  s32 spanLeft;
  s32 y1;
  s32 x2;
  s32 y2;
  s32 cx;
  s32 cy;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  battle = Ram_HeapSlots->battleWork;
  view = Ram_HeapSlots->battleBackground;
  work->args = args;
  work->vblanksSinceTransfer = 1;

  // Make BG2 a 128x128 canvas, as BattleEffect_StartCanvasLayer does.
  BattleEffect_ClearActorTimers();
  view->secondMode = BATTLE_BACKGROUND_FIXED;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED,
                              BATTLE_BACKGROUND_ID(battle), 0);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  REG_BG2X = 0;
  REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;

  i = 0;
  for (k = 0; k != CANVAS_TILES; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * CANVAS_TILES + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
      *(volatile s16 *)(BG_SCREEN_BLOCK(7) + i) = entry;
    }
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress(RESOURCE_TILE_ANIMATION_MASK, work->sheet, 1, 1);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // each transfer clears the canvas
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  // The pieces fly out from (32, 60) at a quarter of their distance a
  // frame.
  r = 0;
  for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
    FIXED x, y;
    s32 vx, vy;

    piece = &work->particles[i];
    x = INTtoFIXED(gDeathPlunge_PieceStartX[i]);
    y = INTtoFIXED(gDeathPlunge_PieceStartY[i]);
    piece->x = x;
    piece->y = y;
    vx = (x - toFIXED(32)) >> 2;
    vy = (y - toFIXED(60)) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
  }

  // Copy the battle background's image aside, fill it with colour 1, and
  // make its colours white.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2),
                  BATTLE_BACKGROUND_BYTES);
  Iwram_FillWords(Ram_MapCellBuffer, BATTLE_BACKGROUND_BYTES,
                  CANVAS_FILL_WORD(1));
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
                          BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK),
                      IO_WRITE_HALFWORD);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(6, 0), BATTLE_BACKGROUND_COLORS * 2,
                  COLOR_PAIR(RGB_WHITE));
  Sound_PlayFar(SE_POWER_UP);
  BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE, POSE_CAST, 0,
                           30);

  for (frame = 0; frame != SHATTERING_OPENING_FRAMES; frame++) {
    if (frame == 2)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 3)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 28)
      BattleEffect_TintBattler(args->targets[0], TINT_KEEP, POSE_CAST,
                               TINT_KEEP, 0);
    if (frame == 32)
      Sound_PlayFar(SE_MONSTER_ROAR);
    if (frame == 5)
      Sound_PlayFar(SE_CRASH);
    if (frame == 5) {
      gBgScroll[1].x = work->savedBg1X;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               BATTLE_BACKGROUND_ID(battle), -1);
    }

    // From frame 8, step each colour of BG palettes 6 to 13 one shade towards
    // the battle background's.
    if (frame > 7) {
      u16 *pal;

      pal = (u16 *)&BG_PLTT_COLOR(6, 0);
      for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
        s32 red = RGB_RED(*pal);
        s32 green = RGB_GREEN(*pal);
        s32 blue = RGB_BLUE(*pal);
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
        *pal++ = RGB(red, green, blue);
      }
    }

    if (frame == 4)
      Iwram_FillWords((void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES,
                      CANVAS_FILL_WORD(2));

    // Frames 0 to 3: widen the circle by 32 pixels of radius, brightening
    // its grey.
    if (frame <= 3) {
      s32 v;

      v = frame * 4 + 8;
      rmax = frame * 32;
      BG_PLTT_COLOR(0, 2) = RGB(v, v, v);
      cx = SHATTERING_OPENING_DISC_X;
      cy = SHATTERING_OPENING_DISC_Y;
      for (; r != rmax; r++) {
        // The midpoint circle: plot each eighth by symmetry.
        px = r;
        py = 0;
        d = r;
        for (; px >= py; py++) {
          // BUG: j and k are the tile map's finished counters, 8 and 16, not
          // the circle's centre.
          x2 = px + j;
          x1 = j - px;
          y2 = py + k;
          y1 = k - py;
          if (y1 < 0)
            y1 = 0;
          if (y2 > 119)
            y2 = 119;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          PLOT(x2, y2);
          PLOT(x2, y1);
          PLOT(x1, y2);
          PLOT(x1, y1);
          left = cx - px;
          x1 = left + 1;
          x2 = px + (SHATTERING_OPENING_DISC_X + 1);
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          PLOT(x2, y2);
          PLOT(x2, y1);
          PLOT(x1, y2);
          PLOT(x1, y1);

          x2 = py + cx;
          x1 = cx - py;
          spanLeft = x1;
          y2 = px + cy;
          y1 = cy - px;
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          if (y1 < 0)
            y1 = 0;
          if (y2 > 119)
            y2 = 119;
          PLOT(x2, y2);
          PLOT(x2, y1);
          PLOT(x1, y2);
          PLOT(x1, y1);
          x1 = spanLeft + 1;
          x2 = py + (SHATTERING_OPENING_DISC_X + 1);
          if (x1 < 0)
            x1 = 0;
          if (x2 > 255)
            x2 = 255;
          PLOT(x2, y2);
          PLOT(x2, y1);
          PLOT(x1, y2);
          PLOT(x1, y1);

          d -= py * 2 + 1;
          if (d < 0) {
            d += px * 2 - 2;
            px--;
          }
        }
      }
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapCellBuffer,
                      BATTLE_BACKGROUND_BYTES);
    }

    // Up to frame 50, draw the pieces; from frame 4 they fall.
    if (frame <= 50) {
      for (i = 0; i != SHATTERING_OPENING_PIECES; i++) {
        piece = &work->particles[i];
        draw[0](canvas, work->sheet + gDeathPlunge_PiecePictures[i],
                FIXED_INTEGER_PART(piece->x), FIXED_INTEGER_PART(piece->y),
                gDeathPlunge_PieceWidths[i], gDeathPlunge_PieceHeights[i]);
        if (frame > 3)
          Particle_AdvanceWithGravity2D(piece, PARTICLE_DAMPING_NONE,
                                        toFIXED(1) / 4);
      }
    }

    // Frames 8 to 50: colour 1 from black to white.
    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > RGB_LEVEL_MAX)
        v = RGB_LEVEL_MAX;
      BG_PLTT_COLOR(0, 1) = RGB(v, v, v);
    }

    // Frame 51: the flash pictures and colours, and 32 flashes placed at
    // x 32-63, y 80-111.
    if (frame == 51) {
      Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 0);
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
        piece = &work->particles[i];
        piece->x = INTtoFIXED((Random16() & 31) + 32);
        piece->y = INTtoFIXED((Random16() & 31) + 80);
        piece->velocityX = ((Random16() & 0x1ff) - 256) << 12;
        piece->velocityY = 0;
        piece->variant = 0;
      }
      // From here each transfer halves the canvas, leaving a trail.
      work->transferMode = CANVAS_COPY_AND_FADE;
      work->transferValue = CANVAS_FADE_HALF;
    }

    // The flashes start i/4 frames late and rise, a picture every 4 frames.
    if (frame > 52) {
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        if (frame >= i / 4 + 52 &&
            piece->variant < SHATTERING_OPENING_FLASH_FRAMES) {
          s32 picture;

          picture = piece->variant / 4;
          if (picture > 5)
            picture = 5;
          draw[1](canvas, work->sheet + picture * PICTURE_BYTES(FLASH_BURST),
                  FIXED_INTEGER_PART(piece->x) - FLASH_BURST_WIDTH / 2,
                  FIXED_INTEGER_PART(piece->y) - FLASH_BURST_HEIGHT / 2,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(piece, 60, -toFIXED(1) / 16);
          piece->variant++;
        }
      }
    }

    EFFECT_END_FRAME(work);
  }

  // Put the background and the display back.
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_TintBattler(args->targets[0], TINT_KEEP, POSE_STANDING,
                           TINT_KEEP, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED,
                              BATTLE_BACKGROUND_ID(battle), 0);
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

#define ELEMENT_BURST_FRAMES 56 ///< frames of the element burst

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
/// spiral in, then a burst of light plays over it. The battle's element glow
/// runs it, and it sets up its own heap blocks.
void BattleEffect_RunElementBurst(struct BattleEffectParams *args) {
  struct ScreenPoint ground;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  work->vblanksSinceTransfer = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_CYAN_SPARK_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (args->kind) {
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
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }

  for (i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
    PARTICLES[i].y = toFIXED(128);
    PARTICLES[i].x = Random16() & TRIG_ANGLE_MASK;
    PARTICLES[i].z = (Random16() & 0x1ff) + 1024;
    PARTICLES[i].variant = -i;
  }

  // A mote's x is its angle and y its distance from the user; its variant
  // counts down its life.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    work->particles[i].x = Random16() & TRIG_ANGLE_MASK;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant =
        (i & (ELEMENT_BURST_MOTE_LIFE - 1)) + ELEMENT_BURST_MOTE_LIFE;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Sound_PlayFar(SE_SPIN);

  for (frame = 0; frame != ELEMENT_BURST_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(args->user, &ground);
    REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frame * 2);
    if (frame == 26) {
      Sound_PlayFar(SE_POWER_UP);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE, TINT_KEEP,
                               0, 20);
    }

    // The burst, a picture every three frames.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas,
              work->sheet +
                  (frame - 28) / 3 * PICTURE_BYTES(ELEMENT_BURST_BURST) +
                  ELEMENT_BURST_BURST,
              40, ground.y - 24, ELEMENT_BURST_BURST_WIDTH,
              ELEMENT_BURST_BURST_HEIGHT);

    // The corner sparks, each with its own flips.
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      s32 mode;

      for (i = 0, mode = BLITTER_MAX; i != 4; i++) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            gElementBurst_GlintFlips[i] | EFFECT_CLIP_BOUNDS,
                            mode);
        draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
        draw[1](canvas,
                work->sheet + picture * PICTURE_BYTES(ELEMENT_BURST_SPARK),
                gDeathPlunge_X[i] + 32, ground.y + gDeathPlunge_Y[i] - 32,
                ELEMENT_BURST_SPARK_WIDTH, ELEMENT_BURST_SPARK_HEIGHT);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }

    // The motes close in 2 pixels a frame.
    if (frame >= 0) {
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant >= 0 && mote->y > 0) {
          s32 size = (mote->variant >> 3) + 1;
          s32 x = (FIXEDtoINT(mote->y * Trig_Sin(mote->x))) + 64;
          s32 y = (FIXEDtoINT(mote->y * Trig_Cos(mote->x))) + ground.y;

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
    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Frame callback: when a transfer is pending, writes the canvas to BG
/// character block 1 at half brightness.
void BattleEffect_TransferHalvedCanvas(void) {
  struct ElementStreaksWork *work = Ram_HeapSlots->battleEffect;
  void *buffer = Ram_HeapSlots->battleCanvas;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->halvePending == 1) {
    work->halvePending = 0;
    IRQ_UNLOCK(ime);
    {
      void *destination = (void *)BG_CHAR_BLOCK(1);

      ColorBuffer_Halve(buffer, destination, CANVAS_BYTES);
    }
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// The element streaks seeded at the start, and those drawn.
#define STREAK_SEEDS 256
#define STREAKS 64
/// A streak's head starts 48 to 63 from the centre, its tail STREAK_LENGTH
/// behind it.
#define STREAK_START_DISTANCE 48
#define STREAK_LENGTH 8

/// Draws a frame of the element streaks, lines of light closing in on the
/// centre of the canvas, and has the canvas halved into VRAM.
void BattleEffect_DrawStreaks(void) {
  struct ElementStreaksWork *work = Ram_HeapSlots->battleEffect;
  s32 frame;
  s32 i;
  struct Streak *streak;
  struct StreakPoint point;
  struct StreakPoint head;
  struct StreakPoint tail;

  frame = (work->frame)++;
  if (frame == 0) {
    for (i = 0; i != STREAK_SEEDS; i++) {
      struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
      s32 r = Random16() & 15;

      seed->head = r + STREAK_START_DISTANCE;
      seed->tail = r + STREAK_START_DISTANCE - STREAK_LENGTH;
      seed->pitch = Random16();
      seed->yaw = Random16();
      seed->roll = Random16();
    }
  }
  point.y = 0;
  point.z = 0;
  for (i = 0; i != STREAKS; i++) {
    streak = &((struct Streak *)Ram_MapCellBuffer)[i];
    if (frame > i / 4 && streak->head > 0) {
      s32 fade;

      Matrix_Initialize();
      Matrix_RotateZ(streak->roll);
      Matrix_RotateX(streak->pitch);
      Matrix_RotateY(streak->yaw);
      point.x = streak->head;
      EffectPosition_ProjectPoint((s32 *)&point, &head);
      head.x += 64;
      head.y += 80;
      point.x = streak->tail;
      EffectPosition_ProjectPoint((s32 *)&point, &tail);
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
  work->halvePending = 1;
}

/// Each frame while frames are left, darkens the battle background's 128
/// colours by one more BACKGROUND_LEVEL_STEP into BG palettes 6 to 13.
void BattleEffect_DimBackgroundStep(void) {
  struct ElementStreaksWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;

  if (work->fadeFrames > 0) {
    work->fadeStep++;
    ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                         FIXED_ONE - work->fadeStep * BACKGROUND_LEVEL_STEP,
                         BATTLE_BACKGROUND_COLORS);
    work->fadeFrames--;
  }
}

/// Starts the element streaks, which then draw by themselves while the battle
/// runs on.
void BattleEffect_StartElementStreaks(s32 kind) {
  struct ElementStreaksWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundWork *view;
  s32 id;
  s32 i;
  s32 j;
  s32 k;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  battle = Ram_HeapSlots->battleWork;
  view = Ram_HeapSlots->battleBackground;

  // Turn BG2 into a 128x128 canvas, one step a frame.
  BattleEffect_ConfigureEffectDisplay();
  view->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  view->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);
  BattleEffect_SetupCanvasWindows();
  REG_BG2PA = Q_8_8(1);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);

  i = 0;
  for (k = 0; k != CANVAS_TILES; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s32 entry;

      entry = (s16)(((k * CANVAS_TILES + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      *(volatile u16 *)(BG_SCREEN_BLOCK(7) + i) = *(u16 *)&entry;
    }
  }
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);

  switch (kind) {
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
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(id),
                       SHEET_PALETTE_BYTES);

  // Fade the background over 24 frames, and draw and halve the streaks
  // every frame.
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  Task_Add(BattleEffect_DimBackgroundStep, TASK_PASS_FRAME);
  work->halvePending = 0;
  Task_Add(BattleEffect_TransferHalvedCanvas, TASK_PASS_VBLANK);
  work->frame = 0;
  Task_Add(BattleEffect_DrawStreaks, TASK_PASS_FRAME);
}

/// Ends the element streaks: removes their three tasks, clears the canvas in
/// VRAM and frees what they took.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(BattleEffect_DrawStreaks);
  Task_Remove(BattleEffect_TransferHalvedCanvas);
  Task_Remove(BattleEffect_DimBackgroundStep);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Where Cruel Ruin's pictures are built or decoded in the effect's pictures:
/// SwirlTexture; GlowBandTextureA (GlowBandTextureB in the second act) and
/// GlowBandTextureB; PinkBarSheet; the ten shaded circles, 2 to 20 wide, at
/// gCruelRuin_CirclePictures; and, in the last act, GlowBandTextureB with the
/// shaded circle drawn after it.
#define CRUEL_RUIN_SWIRL 9800
#define CRUEL_RUIN_BAND_A 13896
#define CRUEL_RUIN_BAND_B 13960
#define CRUEL_RUIN_PINK_BAR 14024
#define CRUEL_RUIN_CIRCLES 14920
#define CRUEL_RUIN_BURST_BAND 14400
#define CRUEL_RUIN_BURST_CIRCLE 14528
/// CornerRuneASheet's 64x64 rune mirrored into a 128x128 one, in the map cell
/// buffer.
#define CRUEL_RUIN_RUNE_CIRCLE 16384
/// The ribbons' strip mesh, in the map cell buffer.
#define CRUEL_RUIN_STRIP_MESH (Ram_MapCellBuffer + 8192)
/// The particles of the first act: rays of light from the orb, the motes that
/// swell below it, and the ball of light that falls, once in the sky and again
/// on the foes.
#define CRUEL_RUIN_RAYS 24
#define CRUEL_RUIN_FIRST_MOTE 32
#define CRUEL_RUIN_MOTES 31
#define CRUEL_RUIN_BALL_PARTICLE 63
/// The glow that swells at the caster's feet as Cruel Ruin starts grows to this
/// size.
#define CRUEL_RUIN_GLOW_SIZE 96
/// The shaded circles built for the motes and blasts: CRUEL_RUIN_CIRCLE_SIZES
/// of them, 2 to 20 wide.
#define CRUEL_RUIN_CIRCLE_SIZES 10
/// The glow in the sky starts this large and shrinks into the orb. Its foot
/// starts at y CRUEL_RUIN_SKY_GLOW_Y, and from frame 20 it rises along a
/// quarter of a cosine towards CRUEL_RUIN_SKY_GLOW_RISE pixels higher, to hold
/// at y 20 from frame 36.
#define CRUEL_RUIN_SKY_GLOW_SIZE 140
#define CRUEL_RUIN_SKY_GLOW_Y 58
#define CRUEL_RUIN_SKY_GLOW_RISE 44
/// Each mote swells in place over CRUEL_RUIN_MOTE_FRAMES, then is gone.
#define CRUEL_RUIN_MOTE_FRAMES 16
/// The rings of fire (FireRingTexture): four fly out from the orb, one every
/// CRUEL_RUIN_RING_INTERVAL frames, each for CRUEL_RUIN_RING_FRAMES, and a
/// fifth swells as the rays burst, shining for CRUEL_RUIN_RING_SHINE_FRAMES
/// before it fades 8 levels a frame.
#define CRUEL_RUIN_RINGS 4
#define CRUEL_RUIN_RING_INTERVAL 10
#define CRUEL_RUIN_RING_FRAMES 16
#define CRUEL_RUIN_RING_SHINE_FRAMES 8
/// The circle of runes turns round the orb from frame 24 for
/// CRUEL_RUIN_RUNE_FRAMES, until the flash.
#define CRUEL_RUIN_RUNE_FRAMES 100
/// The rays' length grows to CRUEL_RUIN_RAY_LENGTH_MAX.
#define CRUEL_RUIN_RAY_LENGTH_MAX (FIXED_ONE * 625 / 512)
/// The ball of light that falls grows to this size; its streak's width scales
/// by CRUEL_RUIN_STREAK_SCALE for each pixel of the ball's size, as the falling
/// streaks' do.
#define CRUEL_RUIN_BALL_SIZE 64
#define CRUEL_RUIN_STREAK_SCALE 9400
/// The two ribbons of light (PinkBarSheet): a strip of CRUEL_RUIN_RIBBON_POINTS
/// points each, two corners a point, swaying CRUEL_RUIN_RIBBON_SWAY_STEP
/// further round a sine at each point, each standing for
/// CRUEL_RUIN_RIBBON_FRAMES. The first leans by CRUEL_RUIN_FIRST_RIBBON_ROLL
/// (about 17 degrees), the second by TRIG_TURN / 64.
#define CRUEL_RUIN_RIBBON_POINTS 33
#define CRUEL_RUIN_RIBBON_QUADS (CRUEL_RUIN_RIBBON_POINTS - 1)
#define CRUEL_RUIN_RIBBON_POINT_BYTES 8
#define CRUEL_RUIN_STRIP_QUAD_BYTES (2 * sizeof(struct MeshTriangle))
#define CRUEL_RUIN_RIBBON_SWAY_STEP 2348
#define CRUEL_RUIN_RIBBON_FRAMES 46
#define CRUEL_RUIN_FIRST_RIBBON_ROLL 3048
/// The ribbons' texture scrolls back from ~CRUEL_RUIN_RIBBON_SCROLL, 2 texels a
/// frame on the first and 4 on the second.
#define CRUEL_RUIN_RIBBON_SCROLL 63
/// The forest rushing by below (ForestCanopyTiles, in its own sprite palette):
/// two rows of CRUEL_RUIN_CANOPY_COLUMNS sprites, the last of each row sharing
/// the first's tiles, their pictures cycling through
/// CRUEL_RUIN_CANOPY_PICTURE_COUNT.
#define CRUEL_RUIN_CANOPY_PALETTE 15
#define CRUEL_RUIN_CANOPY_COLUMNS 9
#define CRUEL_RUIN_CANOPY_SPRITES (2 * CRUEL_RUIN_CANOPY_COLUMNS)
#define CRUEL_RUIN_CANOPY_SIZE 32
#define CRUEL_RUIN_CANOPY_PICTURE_COUNT 8
/// Whole cycles of the pictures added to a sprite's picture number before it
/// is taken modulo CRUEL_RUIN_CANOPY_PICTURE_COUNT.
#define CRUEL_RUIN_CANOPY_CYCLE_BIAS (8 * CRUEL_RUIN_CANOPY_PICTURE_COUNT)
/// The falling streaks' particles, after the canopy's pictures.
#define CRUEL_RUIN_FIRST_STREAK CRUEL_RUIN_CANOPY_SPRITES
#define CRUEL_RUIN_STREAKS 4
/// The forest act lasts CRUEL_RUIN_FOREST_FRAMES, its canvas brightening to
/// white over the last CRUEL_RUIN_FOREST_BRIGHTEN_FRAMES; A or B before then
/// starts the brightening at once.
#define CRUEL_RUIN_FOREST_FRAMES 102
#define CRUEL_RUIN_FOREST_BRIGHTEN_FRAMES 17
/// The six blasts, each swelling and shrinking over CRUEL_RUIN_BLAST_FRAMES.
#define CRUEL_RUIN_BLASTS 6
#define CRUEL_RUIN_BLAST_FRAMES 16
/// The wall of light that sweeps over the foes at the end: a gradient this many
/// pixels square, built in the effect's pictures.
#define CRUEL_RUIN_SWEEP_SIZE 120
/// The battle sprites' colours, OBJ palettes 0 to 13, which the foes'
/// silhouettes take.
#define CRUEL_RUIN_SPRITE_COLORS (14 * PALETTE_COLORS)

/// Cruel Ruin: light swells at the caster's feet as the caster rises out of
/// sight; under a sky of sunbeams a glow gathers into an orb amid a turning
/// circle of runes, rings of fire fly out from it, rays burst and a ball of
/// light falls; then in a sky clearing to gold over a forest rushing by, six
/// blasts go off, streaks rain and two ribbons of light sway past; last the
/// ball falls on the foes, who darken to silhouettes, and a wall of light
/// sweeps over them.
void BattleEffect_RunCruelRuin(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  FIXED place[4];
  struct ModelTexture texture;
  struct ModelTexture streakTexture;
  struct Scale canopyScale;
  struct ModelTexture burstTexture;
  u8 *canvas;
  struct BattleEffectWork *work;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  s32 frame;
  u8 *motePictures;
  struct BattleBackgroundWork *backdrop;
  s32 savedScroll;
  FIXED scrollX;
  s32 phase;
  struct MotionObject *actor;
  s32 i;
  s32 j;
  s32 k;
  FIXED fadeRed;
  FIXED fadeGreen;
  FIXED fadeBlue;
  FIXED redStep;
  FIXED greenStep;
  FIXED blueStep;
  s32 fadeFrames;
  s32 step;
  struct RuinObject *obj;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  motePictures = Ram_HeapSlots->battleSheet;
  backdrop = Ram_HeapSlots->battleBackground;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  draw[0] = Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  savedScroll = gBgScroll[1].x;
  scrollX = 0;
  phase = 0;
  actor = BattleObject_GetFar(args->user)->object;
  actor->verticalMotionStrength = phase;
  Graphics_LoadGreyRamp();
  Sound_PlayFar(SE_SPIN);
  // The glow swells at the caster's feet, and from frame 16 the caster rises
  // out of sight.
  for (frame = 0; frame != 36; frame++) {
    s32 size;

    if (frame == 24) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(6));
    }
    EffectPosition_ProjectBattler(args->user, &pos);
    size = frame * 6 + 2;
    if (size > CRUEL_RUIN_GLOW_SIZE) {
      size = CRUEL_RUIN_GLOW_SIZE;
    }
    BattleEffect_DrawShadedCircles(work->sheet, size, FIXED_ONE);
    Canvas_BlitAtFoot(work->sheet, pos.x / 2, pos.y - 16, size);
    if (frame > 15) {
      actor->y += toFIXED(16);
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  actor->verticalMotionStrength = toFIXED(67) / 100;
  actor->y = 0;
  Iwram_FillWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_SelectLivingTargets(work->args);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_SUNBEAM_CLOUDS_BACKDROP, 0);
  backdrop->busy = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = Ram_HeapSlots->ramCode;
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motePictures, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, gMapGridCellBuffer, 0,
                             0);
  Resource_LoadAndDecompress(RESOURCE_CORNER_RUNE_A_SHEET, work->sheet, 1, 0);
  Picture_MirrorIntoQuadrants(
      work->sheet, Ram_MapCellBuffer + CRUEL_RUIN_RUNE_CIRCLE, 64, 64);
  Resource_LoadAndDecompress(RESOURCE_SWIRL_TEXTURE,
                             work->sheet + CRUEL_RUIN_SWIRL, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                             work->sheet + CRUEL_RUIN_BAND_A, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + CRUEL_RUIN_BAND_B, 0, 0);
  for (i = 0; i != CRUEL_RUIN_CIRCLE_SIZES; i++) {
    s32 size = i * 2 + 2;

    BattleEffect_DrawShadedCirclesMirrored(work->sheet + CRUEL_RUIN_CIRCLES +
                                               gCruelRuin_CirclePictures[i],
                                           size, FIXED_ONE);
  }
  Graphics_LoadGreyRamp();
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  fadeRed = 0;
  fadeGreen = 0;
  fadeBlue = 0;
  redStep = 0;
  greenStep = 0;
  blueStep = 0;
  fadeFrames = 0;
  for (i = 0; i != CRUEL_RUIN_RAYS; i++) {
    struct Particle *p = &work->particles[i];
    p->z = i * 2730 + (Random16() & 0xfff);
    p->variant = -(Random16() & 15);
  }
  for (i = 0; i != CRUEL_RUIN_MOTES; i++) {
    struct Particle *p = &work->particles[CRUEL_RUIN_FIRST_MOTE + i];
    SET_POSITION(p, Random16() & 127, (Random16() & 31) + 88,
                 (Random16() & 3) + 5);
    p->variant = i * -2;
  }
  {
    {
      u8 *pixels = gMapGridCellBuffer;

      // The sky: the glow gathers into the orb and rises, the circle of runes
      // turns round it, rings of fire fly out, rays burst and the ball falls;
      // A or B skips to its landing.
      for (frame = 0; frame != 180; frame++) {
        struct RenderModel *model;
        void *verts;
        s32 shade;

        if ((gInput.repeat & (KEY_A | KEY_B)) && frame <= 168) {
          frame = 169;
        }
        if (frame == 4) {
          Sound_PlayFar(SE_SHIMMER);
        }
        if (frame == 40) {
          Sound_PlayFar(SE_CAST);
        }
        if (frame == 64) {
          Sound_PlayFar(SE_DJINNI_SET);
        }
        if (frame == 74) {
          Sound_PlayFar(SE_DJINNI_SET);
        }
        if (frame == 84) {
          Sound_PlayFar(SE_DJINNI_SET);
        }
        if (frame == 94) {
          Sound_PlayFar(SE_DJINNI_SET);
        }
        if (frame == 124) {
          Sound_PlayFar(SE_CLATTER);
        }
        for (i = 0; i != 6; i++) {
          if (frame == 140 + i * 4) {
            Sound_PlayFar(SE_LAUNCH);
          }
        }
        if (frame == 169) {
          Sound_PlayFar(SE_CRASH);
          work->shakeFrames = 16;
        }
        if (frame == 0) {
          Palette_BrightenSavedBattleBg(0, 0, 0);
        }
        if (frame == 32) {
          fadeRed = 0;
          fadeGreen = 0;
          fadeBlue = 0;
          redStep = -toFIXED(4) / 28;
          greenStep = -toFIXED(12) / 28;
          blueStep = -toFIXED(4) / 28;
          fadeFrames = 28;
        }
        if (frame == 124) {
          fadeRed = toFIXED(RGB_LEVEL_MAX);
          fadeGreen = toFIXED(8);
          fadeBlue = -toFIXED(4);
          redStep = 0;
          greenStep = 0;
          blueStep = 0;
          fadeFrames = 1;
        }
        if (frame == 128) {
          redStep = (-toFIXED(1) - fadeRed) / 16;
          greenStep = (toFIXED(6) - fadeGreen) / 16;
          blueStep = -fadeBlue / 16;
          fadeFrames = 16;
        }
        if (frame == 144) {
          redStep = (-toFIXED(9) - fadeRed) / 8;
          greenStep = (toFIXED(13) - fadeGreen) / 8;
          blueStep = (-toFIXED(5) - fadeBlue) / 8;
          fadeFrames = 8;
        }
        if (frame == 156) {
          redStep = (toFIXED(RGB_LEVEL_MAX) - fadeRed) / 16;
          greenStep = (toFIXED(8) - fadeGreen) / 16;
          blueStep = (-toFIXED(4) - fadeBlue) / 16;
          fadeFrames = 16;
        }
        if (fadeFrames > 0) {
          fadeFrames--;
          fadeRed += redStep;
          fadeGreen += greenStep;
          fadeBlue += blueStep;
          Palette_BrightenSavedBattleBg(
              FIXEDtoINT(fadeRed), FIXEDtoINT(fadeGreen), FIXEDtoINT(fadeBlue));
        }
        if (frame == 124) {
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        }
        if (frame <= 123) {
          s32 size = CRUEL_RUIN_SKY_GLOW_SIZE;
          s32 y = CRUEL_RUIN_SKY_GLOW_Y;
          s32 scale = FIXED_ONE * 2;

          if (frame <= 29) {
            size -= frame * 4;
          } else {
            size = 114 - frame * 3;
          }
          if (frame > 7) {
            scale = toFIXED(9) / 4 - frame * (FIXED_ONE / 32);
          }
          if (scale < FIXED_ONE / 2) {
            scale = FIXED_ONE / 2;
          }
          if (frame > 35) {
            y = 20;
          } else if (frame > 19) {
            y = (FIXEDtoINT(
                    Trig_Cos(frame * (TRIG_TURN / 64) - TRIG_TURN * 5 / 16) *
                    CRUEL_RUIN_SKY_GLOW_RISE)) +
                (CRUEL_RUIN_SKY_GLOW_Y - CRUEL_RUIN_SKY_GLOW_RISE);
          }
          if (size <= 7) {
            size = 8;
          }
          if (frame > 67) {
            size += (frame - 68) / 6;
          }
          BattleEffect_DrawShadedCircles(work->sheet, size, scale);
          Canvas_BlitAtFoot(work->sheet, 60, y, size);
        }
        if (frame > 127) {
          for (i = 0; i != CRUEL_RUIN_MOTES; i++) {
            struct Particle *p = &work->particles[CRUEL_RUIN_FIRST_MOTE + i];

            if ((u32)p->variant < CRUEL_RUIN_MOTE_FRAMES) {
              s32 s =
                  FIXEDtoINT(Trig_Sin(p->variant * (TRIG_TURN / 64)) * p->z);

              draw[0](canvas,
                      work->sheet + CRUEL_RUIN_CIRCLES +
                          gCruelRuin_CirclePictures[s - 1],
                      p->x - s, p->y - s * 2, s * 2, s * 4);
            }
            p->variant++;
          }
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        {
          s32 s;
          s32 t;
          texture.logWidth = 7;
          texture.logHeight = 7;
          model->kind = MODEL_SHADED_MAX;
          model->texture = &texture;
          model->mesh = gBattleEffect_LargeSquareMesh;
          model->vertices = verts;
          texture.pixels = pixels;
          shade = 0;
          model->shade = shade;
          i = shade;

          for (s = frame * (FIXED_ONE / 2) - toFIXED(32), t = frame - 64;
               i != CRUEL_RUIN_RINGS;
               s -= toFIXED(5), i++, t -= CRUEL_RUIN_RING_INTERVAL) {
            if ((u32)t < CRUEL_RUIN_RING_FRAMES) {
              s32 sc = toFIXED(8) - s;

              Matrix_Initialize();
              Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
              Matrix_Scale(sc / 2, sc, sc);
              Matrix_RotateY(gCruelRuin_DiscYaws[i]);
              Matrix_RotateX(gCruelRuin_DiscPitches[i]);
              Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
              Render_DrawModelList(model);
            }
          }
        }
        step = frame - 124;
        if ((u32)step < CRUEL_RUIN_RING_FRAMES) {
          s32 sc = step * (FIXED_ONE / 2);

          shade = CRUEL_RUIN_RING_SHINE_FRAMES * 8 - step * 8;
          if (shade > 0) {
            shade = 0;
          }
          model->shade = shade;
          Matrix_Initialize();
          Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
          Matrix_Scale(sc >> 1, sc, sc);
          Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
          Render_DrawModelList(model);
        }
        {
          s32 t = frame - 24;

          if ((u32)t < CRUEL_RUIN_RUNE_FRAMES) {
            s32 sc = toFIXED(3) - t * (FIXED_ONE / 32);

            if (sc < FIXED_ONE) {
              sc = FIXED_ONE;
            }
            shade = -32;
            if (t <= 7) {
              shade = t * 4 + MODEL_SHADE_INVISIBLE;
            }
            if (frame > 63) {
              shade = 224 - frame * 4;
            }
            model->shade = shade;
            texture.pixels = Ram_MapCellBuffer + CRUEL_RUIN_RUNE_CIRCLE;
            t <<= 9;
            Matrix_Initialize();
            Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
            Matrix_Scale(sc, sc / 3, sc >> 1);
            Matrix_RotateZ(t);
            if (shade > MODEL_SHADE_INVISIBLE) {
              Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
              Render_DrawModelList(model);
            }
            texture.pixels = work->sheet + CRUEL_RUIN_SWIRL;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->mesh = gBattleEffect_SquareMesh;
            if (frame > 63) {
              shade = frame * 4 + -320;
              if (shade > 0) {
                shade = 0;
              }
              model->shade = shade;
              Matrix_Initialize();
              Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
              Matrix_Scale(sc, sc >> 1, sc >> 1);
              Matrix_RotateZ(t);
              Render_TransformVertices(gBattleEffect_SquareCorners, verts, 4);
              Render_DrawModelList(model);
            }
          }
        }
        if (frame > 123) {
          texture.logWidth = 5;
          texture.logHeight = 2;
          texture.pixels = work->sheet + CRUEL_RUIN_BAND_A;
          model->mesh = gBattleEffect_SmallSquareMesh;
          model->kind = MODEL_SHADED_MAX;
          shade = -16;
          model->shade = shade;
          for (i = 0; i != CRUEL_RUIN_RAYS; i++) {
            struct Particle *p = &work->particles[i];

            if (p->variant >= 0) {
              s32 sy = p->variant * (FIXED_ONE / 8);

              if (sy > CRUEL_RUIN_RAY_LENGTH_MAX) {
                sy = CRUEL_RUIN_RAY_LENGTH_MAX;
              }
              Matrix_Initialize();
              Matrix_Translate(-toFIXED(4), -toFIXED(44), 0);
              Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
              Matrix_RotateZ(p->z);
              Matrix_Scale(30384, sy, FIXED_ONE / 2);
              Render_TransformVertices(gBattleEffect_BeamCorners, verts, 4);
              Render_DrawModelList(model);
            }
            p->variant++;
          }
          if (frame > 163) {
            EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                                CANVAS_FILL_WORD(1));
            Palette_BrightenBgEntries(2, 2, 2);
          }
          if (frame > 159) {
            struct Particle *q = &work->particles[CRUEL_RUIN_BALL_PARTICLE];
            s32 r = frame * 2 + -288;

            if (r > CRUEL_RUIN_BALL_SIZE) {
              r = CRUEL_RUIN_BALL_SIZE;
            }
            if (frame == 160) {
              q->x = toFIXED(128);
              q->y = -toFIXED(32);
            }
            if (frame <= 171) {
              q->x += -toFIXED(8);
              q->y += toFIXED(16);
              BattleEffect_DrawShadedCircles(work->sheet, r, FIXED_ONE);
              Canvas_BlitAtFoot(work->sheet, FIXED_INTEGER_PART(q->x),
                                FIXED_INTEGER_PART(q->y), r);
            }
            model->kind = MODEL_SHADED_MAX;
            shade = 0;
            model->shade = shade;
            texture.pixels = work->sheet + CRUEL_RUIN_BAND_B;
            Matrix_Initialize();
            Matrix_Translate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
            Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
            Matrix_RotateZ(TRIG_TURN * 5 / 8);
            Matrix_Scale(r * CRUEL_RUIN_STREAK_SCALE, FIXED_ONE * 2,
                         FIXED_ONE / 2);
            Render_TransformVertices(gCruelRuin_StreakCorners, verts, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
        if (work->shakeFrames > 0) {
          {
            s32 shakeY = Random16() % 8 + 28;
            gBgScroll[1].y = shakeY;
          }
          work->shakeFrames--;
        } else {
          gBgScroll[1].y = work->savedBg1Y;
        }
        BattleEffect_CountDownBattlerTints();
        WAIT_TRANSFER(work);
        EFFECT_PRESENT_CANVAS(work);
      }
    }
  }
  {
    s32 skipFrames = -1;
    s32 fadeStep = 2;
    u8 *strip = CRUEL_RUIN_STRIP_MESH;
    void *verts;
    Render_BuildQuadStrip(strip, 8, 16, CRUEL_RUIN_RIBBON_QUADS);
    for (k = 0; k != 2; k++) {
      s32 c;

      for (c = 0; c != CRUEL_RUIN_RIBBON_POINTS; c++) {
        s32 w = gCruelRuin_PillarWidths[k];
        s8 *v;
        s32 acc;
        s32 height;
        s32 half;

        if (c > CRUEL_RUIN_RIBBON_QUADS / 2 - 1) {
          w = w + (c - CRUEL_RUIN_RIBBON_QUADS / 2) * 2;
        }
        acc = 0;
        {
          s8 *cells = (s8 *)gMapGridCellBuffer;
          s32 cell = k * CRUEL_RUIN_RIBBON_QUADS + k + c;

          v = cells + cell * CRUEL_RUIN_RIBBON_POINT_BYTES;
        }
        height = 126 - c * 7;
        half = w / 2;
        for (i = 0; i != 2; i++, acc += w, v += 4) {
          v[0] = height;
          v[1] = (FIXEDtoINT(Trig_Sin(c * CRUEL_RUIN_RIBBON_SWAY_STEP) *
                             gCruelRuin_PillarSways[k])) +
                 acc - half;
          v[2] = 0;
        }
      }
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    Iwram_FillWords(canvas, CANVAS_BYTES,
                    CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    EFFECT_PRESENT_CANVAS(work);
    Iwram_FillWords(canvas, CANVAS_BYTES, 0);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                             RESOURCE_VIOLET_SKY_BACKDROP, 0);
    Palette_BrightenBattleBg(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX);
    Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                               work->sheet + CRUEL_RUIN_BAND_A, 0, 0);
    Resource_LoadAndDecompress(RESOURCE_PINK_BAR_SHEET,
                               work->sheet + CRUEL_RUIN_PINK_BAR, 0, 0);
    Graphics_LoadGreyRamp();
    {
      u8 *canopy = Resource_GetTableEntry(RESOURCE_FOREST_CANOPY_TILES);

      Iwram_FillWords((void *)&OBJ_PLTT_COLOR(CRUEL_RUIN_CANOPY_PALETTE, 0),
                      PALETTE_BANK_BYTES, COLOR_PAIR(RGB_WHITE));
      Decompress_TaggedLz(canopy + PALETTE_BANK_BYTES, work->sheet);
    }
    {
      s32 q;
      for (q = 0; q != 2; q++) {
        u8 *src;
        struct RuinObject *obj;

        i = 0;
        src = work->sheet + q * 8 * OBJ_32X32_4BPP_BYTES;
        for (; i != 8; i++) {
          obj = EffectSprite_CreateWithTiles(
              CRUEL_RUIN_CANOPY_SIZE, CRUEL_RUIN_CANOPY_SIZE, OBJ_SIZE_32X32,
              OBJ_PALETTE(CRUEL_RUIN_CANOPY_PALETTE));
          work->sprites[q * CRUEL_RUIN_CANOPY_COLUMNS + i] = obj;
          // The sprite takes priority 1, in bits 2 and 3 of the high byte of
          // its OAM attribute 2.
          {
            s32 b = ((u8 *)obj)[9];
            u8 x = b & -13;
            x |= 4;
            ((u8 *)obj)[9] = x;
          }
          Iwram_CopyWords((void *)OBJ_TILES(obj), src, OBJ_32X32_4BPP_BYTES);
          src += OBJ_32X32_4BPP_BYTES;
        }
        obj = EffectSprite_Acquire(CRUEL_RUIN_CANOPY_SIZE,
                                   CRUEL_RUIN_CANOPY_SIZE, OBJ_SIZE_32X32,
                                   OBJ_PALETTE(CRUEL_RUIN_CANOPY_PALETTE));
        work->sprites[q * CRUEL_RUIN_CANOPY_COLUMNS + 8] = obj;
        Iwram_CopyWords(obj, work->sprites[q * CRUEL_RUIN_CANOPY_COLUMNS],
                        offsetof(struct Sprite, lastNo));
        for (i = 0; i != 8; i++) {
          work->particles[q * CRUEL_RUIN_CANOPY_COLUMNS + i].variant =
              ((struct RuinObject *)
                   work->sprites[q * CRUEL_RUIN_CANOPY_COLUMNS + i])
                  ->attr.bits.tile;
        }
      }
    }
    wait(1);
    // The sky, clearing from white through violet to gold, over the forest:
    // the blasts, the falling streaks and the two ribbons; A or B ends it
    // early.
    frame = 0;
    while (1) {
      struct RenderModel *model;

      if ((gInput.repeat & (KEY_A | KEY_B)) &&
          frame <
              CRUEL_RUIN_FOREST_FRAMES - CRUEL_RUIN_FOREST_BRIGHTEN_FRAMES &&
          skipFrames == -1) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN,
                            CANVAS_FILL_WORD(4));
        skipFrames = CRUEL_RUIN_FOREST_BRIGHTEN_FRAMES;
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      }
      if (frame <= 17) {
        s32 green = RGB_LEVEL_MAX - frame * fadeStep;
        s32 red = green;
        s32 blue = RGB_LEVEL_MAX - (frame * fadeStep + frame);

        if (green < 0) {
          red = 0;
        }
        if (green <= 5) {
          green = 6;
        }
        if (blue < -18) {
          blue = -18;
        }
        Palette_BrightenSavedBattleBg(red, green, blue);
        if (frame <= 16) {
          u16 *canopy = Resource_GetTableEntry(RESOURCE_FOREST_CANOPY_TILES);
          u16 *dst = (u16 *)&OBJ_PLTT_COLOR(CRUEL_RUIN_CANOPY_PALETTE, 0);
          s32 add;

          i = 0;
          add = 32 - frame * fadeStep;
          for (; i != PALETTE_COLORS; i++, canopy++, dst++) {
            s32 cr = RGB_RED(*canopy) + add;
            s32 cg = (u16)RGB_GREEN(*canopy) + add;
            s32 cb = (u16)RGB_BLUE(*canopy) + add;

            if (cr > RGB_LEVEL_MAX) {
              cr = RGB_LEVEL_MAX;
            }
            if (cg > RGB_LEVEL_MAX) {
              cg = RGB_LEVEL_MAX;
            }
            if (cb > RGB_LEVEL_MAX) {
              cb = RGB_LEVEL_MAX;
            }
            *dst = RGB(cr, cg, cb);
          }
        }
      }
      if (frame == 0) {
        REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                     BGCNT_SCREEN_BASE(7);
      }
      if (frame == 75) {
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      }
      if (frame ==
          CRUEL_RUIN_FOREST_FRAMES - CRUEL_RUIN_FOREST_BRIGHTEN_FRAMES) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN,
                            CANVAS_FILL_WORD(4));
      }
      verts = Heap_AllocEwram(528);
      model = Render_AllocateModelList(1);
      {
        s32 shade = 0;
        streakTexture.logWidth = 5;
        streakTexture.logHeight = 2;
        streakTexture.pixels = work->sheet + CRUEL_RUIN_BAND_A;
        model->shade = shade;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->texture = &streakTexture;
        model->vertices = verts;
        model->kind = MODEL_SHADED_MAX;
        for (i = 0; i != CRUEL_RUIN_BLASTS; i++) {
          s32 d = frame - gCruelRuin_BlastFrames[i];

          if (d == 0) {
            if (i == CRUEL_RUIN_BLASTS - 1) {
              Sound_PlayFar(SE_CRASH);
            } else {
              Sound_PlayFar(SE_CLATTER);
            }
            work->shakeFrames = 8;
            Iwram_FillWords(canvas, CANVAS_BYTES,
                            CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          }
          if ((u32)d < CRUEL_RUIN_BLAST_FRAMES) {
            s32 r = (FIXEDtoINT(Trig_Sin(d * (TRIG_TURN / 32)) *
                                gCruelRuin_BlastSizes[i])) +
                    2;

            if (r > 1) {
              s32 x = gCruelRuin_BlastX[i] + d * 8;
              s32 y = gCruelRuin_BlastY[i];
              s32 half;
              s32 flags = EFFECT_CLIP_BOUNDS;

              BattleEffect_DrawShadedCircles(work->sheet, r, FIXED_ONE);
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags,
                                  BLITTER_MAX);
              half = r / 2;
              y -= r;
              ALT_BLITTER(canvas, work->sheet, x - half, y + 8, half, r);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags,
                                  BLITTER_MAX);
              ALT_BLITTER(canvas, work->sheet, x, y + 8, half, r);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
          }
        }
      }
      for (i = 0; i != CRUEL_RUIN_STREAKS; i++) {
        struct Particle *p = &work->particles[i + CRUEL_RUIN_FIRST_STREAK];
        s32 start = gCruelRuin_StreakStarts[i] >> 1;

        if (frame == 0) {
          p->y = frame;
          p->variant = frame;
          p->x = INTtoFIXED(gCruelRuin_StreakX[i]);
        }
        if (frame >= start && frame < start + 32) {
          s32 size = 0;
          FIXED px = 0;
          FIXED py = 0;
          s32 angle = 0;
          s32 scale = 0;
          size = gCruelRuin_StreakSizes[i];

          if (p->y < INTtoFIXED(gCruelRuin_StreakEndY[i])) {
            p->x -= INTtoFIXED(gCruelRuin_StreakStepX[i]);
            p->y += INTtoFIXED(gCruelRuin_StreakStepY[i]);
          } else {
            p->variant++;
          }
          p->x += toFIXED(8);
          Matrix_Initialize();
          px = p->x - toFIXED(64);
          py = p->y - toFIXED(64);
          Matrix_Translate(px, py, 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          angle = gCruelRuin_StreakRolls[i];
          Matrix_RotateZ(angle);
          scale = size * CRUEL_RUIN_STREAK_SCALE;
          Matrix_Scale(scale, FIXED_ONE * 2, FIXED_ONE / 2);
          Render_TransformVertices(gCruelRuin_StreakCorners, verts, 4);
          BattleEffect_DrawShadedCircles(work->sheet, size, FIXED_ONE);
          Canvas_BlitAtFoot(work->sheet, FIXED_INTEGER_PART(p->x) - 1,
                            FIXED_INTEGER_PART(p->y), size);
          Render_DrawModelList(model);
        }
      }
      {
        streakTexture.logWidth = 6;
        streakTexture.logHeight = 5;
        streakTexture.pixels = work->sheet + CRUEL_RUIN_PINK_BAR;
        Render_BuildQuadStrip(strip, 2, 32, CRUEL_RUIN_RIBBON_QUADS);
        model->mesh = strip;
        model->kind = MODEL_SHADED_MAX;
        model->scrollU = ~CRUEL_RUIN_RIBBON_SCROLL & TEXTURE_SCROLL_MASK;
      }
      {
        u8 *cells = gMapGridCellBuffer;
        s32 mask;

        for (i = 0, mask = TEXTURE_SCROLL_MASK; i != 2;
             cells += CRUEL_RUIN_RIBBON_POINTS * CRUEL_RUIN_RIBBON_POINT_BYTES,
            i++) {
          s32 t = frame - i * CRUEL_RUIN_RIBBON_FRAMES - 12;

          if ((u32)t < CRUEL_RUIN_RIBBON_FRAMES) {
            s32 x;
            u8 *m;
            s32 z;
            s32 v;

            if (i == 0) {
              v = t * 2 + CRUEL_RUIN_RIBBON_SCROLL;
              v = mask & ~v;
              model->scrollU = v;
              if (t <= CRUEL_RUIN_RIBBON_QUADS) {
                u8 *m = strip + t * CRUEL_RUIN_STRIP_QUAD_BYTES;
                m[0] = i;
                m[1] = i;
                m[2] = i;
              }
            } else {
              v = t * 4 + CRUEL_RUIN_RIBBON_SCROLL;
              v = mask & ~v;
              model->scrollU = v;
              {
                s32 z = 0;
                if (t < CRUEL_RUIN_RIBBON_QUADS / 2) {
                  u8 *m = strip + t * 2 * CRUEL_RUIN_STRIP_QUAD_BYTES;
                  m[0] = z;
                  m[1] = z;
                  m[2] = z;
                }
              }
            }
            Matrix_Initialize();
            x = -toFIXED(24);
            if (t >= 0) {
              x = INTtoFIXED(t) - toFIXED(24);
            }
            if (i == 0) {
              if (t > CRUEL_RUIN_RIBBON_QUADS - 1) {
                x += INTtoFIXED((t - CRUEL_RUIN_RIBBON_QUADS) * 7);
              }
            } else if (t > CRUEL_RUIN_RIBBON_QUADS / 2 - 1) {
              x += INTtoFIXED((t - CRUEL_RUIN_RIBBON_QUADS / 2) * 7);
            }
            Matrix_Translate(x, -toFIXED(16), 0);
            Matrix_Scale(45536, FIXED_ONE, FIXED_ONE / 2);
            if (i == 0) {
              Matrix_RotateZ(-CRUEL_RUIN_FIRST_RIBBON_ROLL);
            } else {
              Matrix_RotateZ(-(TRIG_TURN / 64));
            }
            Render_TransformVertices(cells, verts,
                                     CRUEL_RUIN_RIBBON_POINTS * 2);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
      canopyScale = gCruelRuin_UnitScale;
      place[3] = 0;
      place[1] = toFIXED(255);
      // The forest rushes by, a sprite's width every so often, when each
      // sprite takes the picture of the one before it.
      scrollX += toFIXED(24);
      if (scrollX > toFIXED(CRUEL_RUIN_CANOPY_SIZE) - 1) {
        phase = (phase + CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1) %
                CRUEL_RUIN_CANOPY_PICTURE_COUNT;
        scrollX -= toFIXED(CRUEL_RUIN_CANOPY_SIZE);
      }
      gBgScroll[1].x -= 8;
      for (j = 0; j != 2; j++) {
        s32 row = j * CRUEL_RUIN_CANOPY_COLUMNS;
        void **last = &work->sprites[row + CRUEL_RUIN_CANOPY_COLUMNS - 1];

        for (i = 0; i != CRUEL_RUIN_CANOPY_COLUMNS; i++) {
          s32 mask = ~OBJ_TILE_MASK;
          s32 offset = CRUEL_RUIN_CANOPY_CYCLE_BIAS;
          s32 tile = i + offset + phase;
          place[0] = INTtoFIXED(i * CRUEL_RUIN_CANOPY_SIZE) + scrollX;
          place[2] = toFIXED(121) + INTtoFIXED(j * CRUEL_RUIN_CANOPY_SIZE);
          /* FAKEMATCH(dead): a store both arms below overwrite. */
          if (i == 300 && row == j && last == 0)
            step = 0;
          // The sprite's picture: tile % CRUEL_RUIN_CANOPY_PICTURE_COUNT, the
          // last sprite taking the one before the first's.
          if (i == CRUEL_RUIN_CANOPY_COLUMNS - 1) {
            obj = *last;
            /* FAKEMATCH(temporary): cycle and cycleStart, the modulo by
             * CRUEL_RUIN_CANOPY_PICTURE_COUNT written out. */
            {
              s32 cycle = phase + (CRUEL_RUIN_CANOPY_CYCLE_BIAS +
                                   CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1);
              s32 cycleStart = cycle;
              if (cycle < 0)
                cycleStart = cycle + (CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1);
              cycleStart >>= 3;
              cycleStart <<= 3;
              step = cycle - cycleStart;
            }
          } else {
            /* FAKEMATCH(temporary): j * CRUEL_RUIN_CANOPY_COLUMNS written again
             * for the row. */
            {
              s32 m = j * CRUEL_RUIN_CANOPY_COLUMNS;
              m += i;
              obj = work->sprites[m];
            }
            /* FAKEMATCH(temporary): phase + i, held for the negative case. */
            {
              s32 next = phase + i;
              s32 cycleStart = tile;
              if (tile < 0)
                cycleStart = next + (CRUEL_RUIN_CANOPY_CYCLE_BIAS +
                                     CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1);
              cycleStart >>= 3;
              cycleStart <<= 3;
              step = tile - cycleStart;
            }
          }
          /* FAKEMATCH(dead): a once-only block round the tile's update,
           * declaring v and x. */
          {
            s32 v = work->particles[row + step].variant & OBJ_TILE_MASK;
            s32 x = obj->attr.all & mask;
            obj->attr.all = x | v;
          }
          if (place[0] > toFIXED(256) - 1) {
            place[0] -= toFIXED(288);
          }
          Render_ApplyProjectedPlacementFar(
              work->sprites[j * CRUEL_RUIN_CANOPY_COLUMNS + i], place,
              (s32 *)&canopyScale, 0);
        }
      }
      BattleEffect_CountDownBattlerTints();
      WAIT_TRANSFER(work);
      if (work->shakeFrames > 0) {
        {
          s32 shakeY = Random16() % 8 + 28;
          gBgScroll[1].y = shakeY;
        }
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
      }
      EFFECT_PRESENT_CANVAS(work);
      frame++;
      if (frame == CRUEL_RUIN_FOREST_FRAMES) {
        break;
      }
      if (skipFrames > 0 && --skipFrames == 0) {
        break;
      }
    }
  }
  gBgScroll[1].x = savedScroll;
  backdrop->busy = 0;
  {
    struct BattleWork *battle = Ram_HeapSlots->battleWork;

    gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                             BACKGROUND_DARKENED_LEVEL);
  }
  wait(1);
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  for (i = 0; i != CRUEL_RUIN_CANOPY_SPRITES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT,
          (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
      BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
  Iwram_FillWords(canvas, CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  args->targets[args->targetCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(args->targets, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + CRUEL_RUIN_BURST_BAND, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (i = 0; i != CRUEL_RUIN_SWEEP_SIZE; i++) {
    s32 v = i / 2;

    if (v > CANVAS_LEVEL_BRIGHTEST) {
      v = CANVAS_LEVEL_BRIGHTEST;
    }
    for (j = 0; j != CRUEL_RUIN_SWEEP_SIZE; j++) {
      work->sheet[i + j * CRUEL_RUIN_SWEEP_SIZE] = v;
    }
  }
  // The ball falls on the foes, who darken to silhouettes as the wall of
  // light sweeps over them.
  {
    for (frame = 0; frame != 140; frame++) {
      s32 grey = frame - 80;
      s32 blue = RGB(0, 0, grey);
      s32 green = RGB(0, grey, 0);

      if (frame == 0) {
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_NORMAL,
                                   TINT_KEEP, i, 16);
        }
      }
      if (frame == 127) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      if (frame == 107) {
        Iwram_FillWords(canvas, CANVAS_BYTES,
                        CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      }
      if (frame > 107) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        Palette_BrightenSavedBattleBg(0, 0, 0);
      }
      if (frame == 32) {
        Sound_PlayFar(SE_SPIN);
      }
      if (frame == 48) {
        work->shakeFrames = 90;
        Sound_PlayFar(SE_CRASH);
      }
      if (frame == 72) {
        Sound_PlayFar(SE_BANE_ESCAPE);
      }
      if (frame <= 109) {
        struct RenderModel *model;
        void *verts;

        if (frame > 31) {
          s32 a = (frame - 32) / 4;
          s32 b = 0;

          if (frame > 47) {
            b = (frame - 48) / 2;
          }
          Palette_BrightenBattleBg(a, a / 2, b);
        }
        if ((u32)(frame - 80) <= 31) {
          u32 c;
          u16 *dst = (u16 *)&OBJ_PLTT_COLOR(0, 1);

          i = 0;
          c = blue | green | (frame - 80);
          for (; i != CRUEL_RUIN_SPRITE_COLORS - 1; i++) {
            *dst++ = c;
          }
        } else if (frame > 47) {
          ColorBuffer_ScaleFar((void *)OBJ_PLTT, (void *)OBJ_PLTT,
                               FIXED_ONE * 15 / 16, CRUEL_RUIN_SPRITE_COLORS);
        }
        if (frame > 47) {
          draw[0](canvas, work->sheet, 216 - frame * 2, 0,
                  CRUEL_RUIN_SWEEP_SIZE, CRUEL_RUIN_SWEEP_SIZE);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        {
          s32 shade = 0;
          burstTexture.logWidth = 5;
          burstTexture.logHeight = 2;
          burstTexture.pixels = work->sheet + CRUEL_RUIN_BURST_BAND;
          model->shade = shade;
          model->mesh = gBattleEffect_SmallSquareMesh;
          model->texture = &burstTexture;
          model->vertices = verts;
          model->kind = MODEL_SHADED_MAX;
        }
        if (frame > 31) {
          struct Particle *q = &work->particles[CRUEL_RUIN_BALL_PARTICLE];
          s32 r = frame * 2 - 32;

          if (r > CRUEL_RUIN_BALL_SIZE) {
            r = CRUEL_RUIN_BALL_SIZE;
          }
          if (frame == 32) {
            q->x = toFIXED(128);
            q->y = -toFIXED(32);
          }
          if (frame <= 43) {
            q->x += -toFIXED(8);
            q->y += toFIXED(16);
            BattleEffect_DrawShadedCircles(
                work->sheet + CRUEL_RUIN_BURST_CIRCLE, r, FIXED_ONE);
            Canvas_BlitAtFoot(work->sheet + CRUEL_RUIN_BURST_CIRCLE,
                              FIXED_INTEGER_PART(q->x),
                              FIXED_INTEGER_PART(q->y), r);
          }
          Matrix_Initialize();
          Matrix_Translate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          Matrix_RotateZ(TRIG_TURN * 5 / 8);
          Matrix_Scale(r * CRUEL_RUIN_STREAK_SCALE, FIXED_ONE * 2,
                       FIXED_ONE / 2);
          Render_TransformVertices(gCruelRuin_StreakCorners, verts, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      {
        s32 r = 8;

        if (frame > 130) {
          r = 2;
        } else if (frame > 120) {
          r = 4;
        } else if (frame > 110) {
          r = 6;
        }
        BattleCamera_Shake(r, r);
      }
      EFFECT_PRESENT_CANVAS(work);
    }
  }
  Palette_LoadBasicSpriteColors();
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef PLOT
#undef STREAK_SEEDS
#undef STREAKS
#undef STREAK_START_DISTANCE
#undef STREAK_LENGTH
