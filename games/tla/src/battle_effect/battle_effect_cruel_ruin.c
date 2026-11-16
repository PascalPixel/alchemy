/// @file
/// Cruel Ruin, the shattering opening and the element burst, and the
/// light-streak backdrop with its halved canvas and dimming background.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "resource_ids.h"

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// Puts colour 2 at (x, y) in the 256x120 8bpp tile sheet (32 tiles wide) in
/// the map cell buffer.
#define PLOT(x, y)                                                             \
  (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +            \
     (y) / 8 * 2048) = 2)

// The target's picture shatters, for the scenes.

#define SHATTERING_OPENING_FRAMES 128

#define SHATTERING_OPENING_FLASHES 32 ///< flashes thrown as the picture breaks
#define SHATTERING_OPENING_PIECES 33  ///< pieces the picture breaks into
#define SHATTERING_OPENING_DISC_X 96  ///< the centre of the opening disc
#define SHATTERING_OPENING_DISC_Y 60  ///< the centre of the opening disc

/// The screen turns white and the picture breaks into 33 pieces that fly apart
/// and fall, while a grey disc grows over it and the battle background's
/// colours return.
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
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;

  i = 0;
  for (k = 0; k != CANVAS_TILES; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
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
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
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

  // Copy the picture at 0x06008000 aside, fill it with colour 1, and make
  // colours 6 to 13 white.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2), 30720);
  Iwram_FillWords(Ram_MapCellBuffer, BATTLE_BACKGROUND_BYTES,
                  CANVAS_FILL_WORD(1));
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
                          BGCNT_SCREEN_BASE(31),
                      IO_WRITE_HALFWORD);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(6, 0), 256, COLOR_PAIR(RGB_WHITE));
  Sound_PlayFar(SE_POWER_UP);
  BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE, 3, 0, 30);

  for (frame = 0; frame != SHATTERING_OPENING_FRAMES; frame++) {
    if (frame == 2)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 3)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 28)
      BattleEffect_TintBattler(args->targets[0], TINT_KEEP, 3, TINT_KEEP, 0);
    if (frame == 32)
      Sound_PlayFar(SE_MONSTER_ROAR);
    if (frame == 5)
      Sound_PlayFar(SE_CRASH);
    if (frame == 5) {
      gBgScroll[1].x = work->savedBg1X;
      BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                               BATTLE_BACKGROUND_ID(battle), -1);
    }

    // From frame 8, step each of colours 6 to 13 one shade towards the
    // battle background's.
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
        while (px >= py) {
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
          x2 = px + 97;
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
          x2 = py + 97;
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
          py++;
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
      for (i = 1; i != 64; i++) {
        s32 v;

        v = i / 2;
        if (v < 0)
          v = 0;
        BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
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
      work->transferMode =
          CANVAS_COPY_AND_FADE; // each transfer halves the old picture,
      work->transferValue = CANVAS_FADE_HALF; // leaving a trail
    }

    // The flashes start i/4 frames late and rise, a picture every 4 frames.
    if (frame > 52) {
      for (i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
        piece = &work->particles[i];
        if (frame >= i / 4 + 52 && piece->variant <= 39) {
          s32 cell;

          cell = piece->variant / 4;
          if (cell > 5)
            cell = 5;
          draw[1](canvas, work->sheet + (cell << 11),
                  FIXED_INTEGER_PART(piece->x) - 16,
                  FIXED_INTEGER_PART(piece->y) - 32, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
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

// Reached from the scenes through the far-call table. Like
// BattleEffect_RunElementPillar it sets up its own heap blocks, stores
// work->effect and marks the work active itself, and reads the actors from the
// argument.

/// The rising sparkles live in the map cell buffer.
#define gSparkles ((struct Particle *)Ram_MapCellBuffer)

// Light gathering on the user and bursting, for the scenes.

#define ELEMENT_BURST_FRAMES 56    ///< frames of the element burst
#define ELEMENT_BURST_SPARKLES 128 ///< sparkles set up in the map cell buffer

/// CyanSparkSheet: five small sparks, then the burst's pictures.
#define ELEMENT_BURST_SPARK_WIDTH 32
#define ELEMENT_BURST_SPARK_HEIGHT 32
#define ELEMENT_BURST_BURST_WIDTH 48
#define ELEMENT_BURST_BURST_HEIGHT 48
#define ELEMENT_BURST_BURST (5 * PICTURE_BYTES(ELEMENT_BURST_SPARK))

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

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  sheet = HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  work->vblanksSinceTransfer = 1;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(12);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_CYAN_SPARK_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_D, sheet, 0, 0);
  switch (args->kind) {
  case 0:
    palette = Resource_GetTableEntry(RESOURCE_YELLOW_PALETTE_A);
    break;
  case 1:
    palette = Resource_GetTableEntry(RESOURCE_ICE_TILE_SHEET);
    break;
  case 2:
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

  for (i = 0; i != ELEMENT_BURST_SPARKLES; i++) {
    gSparkles[i].y = toFIXED(128);
    gSparkles[i].x = Random16() & TRIG_ANGLE_MASK;
    gSparkles[i].z = (Random16() & 0x1ff) + 1024;
    gSparkles[i].variant = -i;
  }

  // A mote's x is its angle and y its distance from the user; its variant
  // counts down its life.
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    work->particles[i].x = Random16() & TRIG_ANGLE_MASK;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant = (i & 15) + 16;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Sound_PlayFar(SE_SPIN);

  for (frame = 0; frame != ELEMENT_BURST_FRAMES; frame++) {
    EffectPosition_ProjectScaledBattler(args->user, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = BLEND_ALPHA(112 - frame * 2, BLEND_WEIGHT_MAX);
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
                            gElementBurst_GlintFlips[i] | CLIP, mode);
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

          if (size <= 0)
            size = 1;
          draw[0](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                  x - size, y - size, size * 2, size * 2);
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

      ColorBuffer_Halve(buffer, destination, 16384);
    }
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// Draws the streaks of the backdrop and halves the canvas into VRAM.
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
    for (i = 0; i != 256; i++) {
      struct Streak *seed = &((struct Streak *)Ram_MapCellBuffer)[i];
      s32 r = Random16() & 15;

      seed->head = r + 48;
      seed->tail = r + 40;
      seed->pitch = Random16();
      seed->yaw = Random16();
      seed->roll = Random16();
    }
  }
  point.y = 0;
  point.z = 0;
  i = 0;
  for (; i != 64; i++) {
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
/// colours by one more CANVAS_FADE_STEP into BG palettes 6 to 13.
void BattleEffect_DimBackgroundStep(void) {
  struct ElementStreaksWork *work = Ram_HeapSlots->battleEffect;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;

  if (work->fadeFrames > 0) {
    work->fadeStep++;
    ColorBuffer_ScaleFar(battle->palette, (void *)&BG_PLTT_COLOR(6, 0),
                         FIXED_ONE - work->fadeStep * CANVAS_FADE_STEP,
                         BATTLE_BACKGROUND_COLORS);
    work->fadeFrames--;
  }
}

// The battle background's id.

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

      entry = (s16)(((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      *(volatile u16 *)(BG_SCREEN_BLOCK(7) + i) = *(u16 *)&entry;
    }
  }
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);

  switch (kind) {
  case 0:
    id = RESOURCE_YELLOW_PALETTE_B;
    break;
  case 1:
    id = RESOURCE_CRYSTAL_SHEET;
    break;
  case 2:
    id = RESOURCE_EMBER_STREAK_SHEET;
    break;
  case 3:
    id = RESOURCE_VIOLET_PALETTE_D;
    break;
  case 4:
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
  Task_Add(BattleEffect_DimBackgroundStep, TASK_PASS_VBLANK);
  work->halvePending = 0;
  Task_Add(BattleEffect_TransferHalvedCanvas, TASK_PASS_FRAME);
  work->frame = 0;
  Task_Add(BattleEffect_DrawStreaks, TASK_PASS_VBLANK);
}

/// Ends the light-streak backdrop: removes its three callbacks, clears the
/// canvas in VRAM and frees heap blocks 96 and 92.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(BattleEffect_DrawStreaks);
  Task_Remove(BattleEffect_TransferHalvedCanvas);
  Task_Remove(BattleEffect_DimBackgroundStep);
  Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

#define ALT_BLITTER ((DrawRectangle)Ram_HeapSlots->ramCodePaired)
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                       \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

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
/// The pillar's strip mesh, in the map cell buffer.
#define CRUEL_RUIN_STRIP_MESH (Ram_MapCellBuffer + 8192)
/// The particles of the first act: rays of light from the caster, the motes
/// that rise around it, and the black sphere that falls on the foes.
#define CRUEL_RUIN_RAYS 24
#define CRUEL_RUIN_FIRST_MOTE 32
#define CRUEL_RUIN_MOTES 31
#define CRUEL_RUIN_SPHERE 63

/// Cruel Ruin: a pillar of dark light swallows the caster, a shadowed sky
/// rains ruin in six blasts while the ground splits, then a black sphere
/// bursts over the foes.
void BattleEffect_RunCruelRuin(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  FIXED place[4];
  struct ModelTexture texture;
  struct ModelTexture blastTexture;
  struct Scale req;
  struct ModelTexture burstTexture;
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
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
  s32 steps;
  s32 u;
  struct RuinObject *obj;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  backdrop = Ram_HeapSlots->battleBackground;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  draw[0] = Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  savedScroll = gBgScroll[1].x;
  scrollX = 0;
  phase = 0;
  actor = BattleObject_GetFar(args->user)->object;
  actor->verticalMotionStrength = phase;
  Graphics_LoadGreyRamp();
  Sound_PlayFar(SE_SPIN);
  for (frame = 0; frame != 36; frame++) {
    s32 size;

    if (frame == 24) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(6));
    }
    EffectPosition_ProjectBattler(args->user, &pos);
    size = frame * 6 + 2;
    if (size > 96) {
      size = 96;
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
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
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
  for (i = 0; i != 10; i++) {
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
  steps = 0;
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
          steps = 28;
        }
        if (frame == 124) {
          fadeRed = toFIXED(RGB_LEVEL_MAX);
          fadeGreen = toFIXED(8);
          fadeBlue = -toFIXED(4);
          redStep = 0;
          greenStep = 0;
          blueStep = 0;
          steps = 1;
        }
        if (frame == 128) {
          redStep = (-toFIXED(1) - fadeRed) / 16;
          greenStep = (toFIXED(6) - fadeGreen) / 16;
          blueStep = -fadeBlue / 16;
          steps = 16;
        }
        if (frame == 144) {
          redStep = (-toFIXED(9) - fadeRed) / 8;
          greenStep = (toFIXED(13) - fadeGreen) / 8;
          blueStep = (-toFIXED(5) - fadeBlue) / 8;
          steps = 8;
        }
        if (frame == 156) {
          redStep = (toFIXED(RGB_LEVEL_MAX) - fadeRed) / 16;
          greenStep = (toFIXED(8) - fadeGreen) / 16;
          blueStep = (-toFIXED(4) - fadeBlue) / 16;
          steps = 16;
        }
        if (steps > 0) {
          steps--;
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
          s32 size = 140;
          s32 y = 58;
          s32 scale = FIXED_ONE * 2;

          if (frame <= 29) {
            size -= frame * 4;
          } else {
            size = 114 - frame * 3;
          }
          if (frame > 7) {
            scale = toFIXED(9) / 4 - (frame << 11);
          }
          if (scale < FIXED_ONE / 2) {
            scale = FIXED_ONE / 2;
          }
          if (frame > 35) {
            y = 20;
          } else if (frame > 19) {
            y = (FIXEDtoINT(Trig_Cos((frame << 10) - TRIG_TURN * 5 / 16) *
                            44)) +
                14;
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

            if ((u32)p->variant <= 15) {
              s32 s = FIXEDtoINT(Trig_Sin(p->variant << 10) * p->z);

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

          for (s = (frame << 15) - toFIXED(32), t = frame - 64; i != 4;
               s -= toFIXED(5), i++, t -= 10) {
            if ((u32)t <= 15) {
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
        u = frame - 124;
        if ((u32)u <= 15) {
          s32 sc = u << 15;

          shade = 64 - u * 8;
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

          if ((u32)t <= 99) {
            s32 sc = toFIXED(3) - (t << 11);

            if (sc < FIXED_ONE) {
              sc = FIXED_ONE;
            }
            shade = -32;
            if (t <= 7) {
              shade = t * 4 - 64;
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
              s32 sy = p->variant << 13;

              if (sy > 80000) {
                sy = 80000;
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
            struct Particle *q = &work->particles[CRUEL_RUIN_SPHERE];
            s32 r = frame * 2 + -288;

            if (r > 64) {
              r = 64;
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
            Matrix_Scale(r * 9400, FIXED_ONE * 2, FIXED_ONE / 2);
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
    s32 count = -1;
    s32 fadeStep = 2;
    u8 *strip = CRUEL_RUIN_STRIP_MESH;
    void *verts;
    Render_BuildQuadStrip(strip, 8, 16, 32);
    for (k = 0; k != 2; k++) {
      s32 c;

      for (c = 0; c != 33; c++) {
        s32 w = gCruelRuin_PillarWidths[k];
        s8 *v;
        s32 acc;
        s32 height;
        s32 half;

        if (c > 15) {
          w = w + c * 2 - 32;
        }
        acc = 0;
        {
          s8 *cells = (s8 *)gMapGridCellBuffer;
          s32 cell = k * 32 + k + c;

          v = cells + cell * 8;
        }
        height = 126 - c * 7;
        half = w / 2;
        for (i = 0; i != 2; i++, acc += w, v += 4) {
          v[0] = height;
          v[1] = (FIXEDtoINT(Trig_Sin(c * 2348) * gCruelRuin_PillarSways[k])) +
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
      u8 *e = Resource_GetTableEntry(RESOURCE_FOREST_CANOPY_TILES);

      Iwram_FillWords((void *)&OBJ_PLTT_COLOR(15, 0), PALETTE_BANK_BYTES,
                      COLOR_PAIR(RGB_WHITE));
      Decompress_TaggedLz(e + PALETTE_BANK_BYTES, work->sheet);
    }
    {
      s32 q;
      for (q = 0; q != 2; q++) {
        u8 *src;
        struct RuinObject *obj;

        i = 0;
        src = work->sheet + q * 8 * OBJ_32X32_4BPP_BYTES;
        for (; i != 8; i++) {
          obj = EffectSprite_CreateWithTiles(32, 32, OBJ_SIZE_32X32,
                                             OBJ_PALETTE(15));
          work->sprites[q * 9 + i] = obj;
          {
            s32 b = ((u8 *)obj)[9];
            u8 x = b & -13;
            x |= 4;
            ((u8 *)obj)[9] = x;
          }
          Iwram_CopyWords((void *)OBJ_TILES(obj), src, OBJ_32X32_4BPP_BYTES);
          src += OBJ_32X32_4BPP_BYTES;
        }
        obj = EffectSprite_Acquire(32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(15));
        work->sprites[q * 9 + 8] = obj;
        Iwram_CopyWords(obj, work->sprites[q * 9], 24);
        for (i = 0; i != 8; i++) {
          work->particles[q * 9 + i].variant =
              ((struct RuinObject *)work->sprites[q * 9 + i])->attr.bits.tile;
        }
      }
    }
    wait(1);
    frame = 0;
    while (1) {
      struct RenderModel *model;

      if ((gInput.repeat & (KEY_A | KEY_B)) && frame <= 84 && count == -1) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN,
                            CANVAS_FILL_WORD(4));
        count = 17;
        REG_BG2CNT =
            BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
      }
      if (frame <= 17) {
        s32 g = 31 - frame * fadeStep;
        s32 r = g;
        s32 b = 31 - (frame * fadeStep + frame);

        if (g < 0) {
          r = 0;
        }
        if (g <= 5) {
          g = 6;
        }
        if (b < -18) {
          b = -18;
        }
        Palette_BrightenSavedBattleBg(r, g, b);
        if (frame <= 16) {
          u16 *e = Resource_GetTableEntry(RESOURCE_FOREST_CANOPY_TILES);
          u16 *dst = (u16 *)&OBJ_PLTT_COLOR(15, 0);
          s32 add;

          i = 0;
          add = 32 - frame * fadeStep;
          for (; i != 16; i++, e++, dst++) {
            s32 cr = RGB_RED(*e) + add;
            s32 cg = (u16)RGB_GREEN(*e) + add;
            s32 cb = (u16)RGB_BLUE(*e) + add;

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
      if (frame == 85) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_BRIGHTEN,
                            CANVAS_FILL_WORD(4));
      }
      verts = Heap_AllocEwram(528);
      model = Render_AllocateModelList(1);
      {
        s32 zero = 0;
        blastTexture.logWidth = 5;
        blastTexture.logHeight = 2;
        blastTexture.pixels = work->sheet + CRUEL_RUIN_BAND_A;
        model->shade = zero;
        model->mesh = gBattleEffect_SmallSquareMesh;
        model->texture = &blastTexture;
        model->vertices = verts;
        model->kind = MODEL_SHADED_MAX;
        for (i = zero; i != 6; i++) {
          s32 d = frame - gCruelRuin_BlastFrames[i];

          if (d == 0) {
            if (i == 5) {
              Sound_PlayFar(SE_CRASH);
            } else {
              Sound_PlayFar(SE_CLATTER);
            }
            work->shakeFrames = 8;
            Iwram_FillWords(canvas, CANVAS_BYTES,
                            CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          }
          if ((u32)d <= 15) {
            s32 r =
                (FIXEDtoINT(Trig_Sin(d << 11) * gCruelRuin_BlastSizes[i])) + 2;

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
      for (i = 0; i != 4; i++) {
        struct Particle *p = &work->particles[i + 18];
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
          scale = size * 9400;
          Matrix_Scale(scale, FIXED_ONE * 2, FIXED_ONE / 2);
          Render_TransformVertices(gCruelRuin_StreakCorners, verts, 4);
          BattleEffect_DrawShadedCircles(work->sheet, size, FIXED_ONE);
          Canvas_BlitAtFoot(work->sheet, FIXED_INTEGER_PART(p->x) - 1,
                            FIXED_INTEGER_PART(p->y), size);
          Render_DrawModelList(model);
        }
      }
      {
        s32 six = 6;
        blastTexture.logWidth = six;
        blastTexture.logHeight = 5;
        blastTexture.pixels = work->sheet + CRUEL_RUIN_PINK_BAR;
        Render_BuildQuadStrip(strip, 2, 32, 32);
        model->mesh = strip;
        model->kind = six;
        model->scrollU = 64;
      }
      {
        u8 *cells = gMapGridCellBuffer;
        s32 mask;

        for (i = 0, mask = 127; i != 2; cells += 264, i++) {
          s32 t = frame - i * 46 - 12;

          if ((u32)t <= 45) {
            s32 x;
            u8 *m;
            s32 z;
            s32 v;

            if (i == 0) {
              v = t * 2 + 63;
              v = mask & ~v;
              model->scrollU = v;
              if (t <= 32) {
                u8 *m = (u8 *)(t * 24 + (s32)strip);
                m[0] = i;
                m[1] = i;
                m[2] = i;
              }
            } else {
              v = t * 4 + 63;
              v = mask & ~v;
              model->scrollU = v;
              {
                s32 z = 0;
                if (t <= 15) {
                  u8 *m = (u8 *)(t * 48 + (s32)strip);
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
              if (t > 31) {
                x += INTtoFIXED(t * 7 - 224);
              }
            } else if (t > 15) {
              x += INTtoFIXED(t * 7 - 112);
            }
            Matrix_Translate(x, -toFIXED(16), 0);
            Matrix_Scale(45536, FIXED_ONE, FIXED_ONE / 2);
            if (i == 0) {
              Matrix_RotateZ(-3048);
            } else {
              Matrix_RotateZ(-(TRIG_TURN / 64));
            }
            Render_TransformVertices(cells, verts, 66);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
      req = gCruelRuin_UnitScale;
      place[3] = 0;
      place[1] = toFIXED(255);
      scrollX += toFIXED(24);
      if (scrollX > toFIXED(32) - 1) {
        phase = (phase + 7) % 8;
        scrollX -= toFIXED(32);
      }
      gBgScroll[1].x -= 8;
      for (j = 0; j != 2; j++) {
        s32 row = j * 9;
        void **last = &work->sprites[row + 8];

        for (i = 0; i != 9; i++) {
          s32 mask = ~0x3ff;
          s32 offset = 64;
          s32 tile = i + offset + phase;
          place[0] = INTtoFIXED(i * 32) + scrollX;
          place[2] = toFIXED(121) + INTtoFIXED(j * 32);
          /* FAKEMATCH(dead): a store both arms below overwrite. */
          if (i == 300 && row == j && last == 0)
            u = 0;
          if (i == 8) {
            obj = *last;
            {
              s32 cycle = phase + 71;
              s32 cycleStart = cycle;
              if (cycle < 0)
                cycleStart = phase + 78;
              cycleStart >>= 3;
              cycleStart <<= 3;
              u = cycle - cycleStart;
            }
          } else {
            /* FAKEMATCH(temporary): j * 9 written again for the row. */
            {
              s32 m = j * 9;
              m += i;
              obj = work->sprites[m];
            }
            {
              s32 next = phase + i;
              s32 cycleStart = tile;
              if (tile < 0)
                cycleStart = next + 71;
              cycleStart >>= 3;
              cycleStart <<= 3;
              u = tile - cycleStart;
            }
          }
          {
            s32 v = work->particles[row + u].variant & 0x3ff;
            s32 x = obj->attr.all & mask;
            obj->attr.all = x | v;
          }
          if (place[0] > toFIXED(256) - 1) {
            place[0] -= toFIXED(288);
          }
          Render_ApplyProjectedPlacementFar(work->sprites[j * 9 + i], place,
                                            (s32 *)&req, 0);
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
      if (frame == 102) {
        break;
      }
      if (count > 0 && --count == 0) {
        break;
      }
    }
  }
  gBgScroll[1].x = savedScroll;
  backdrop->busy = 0;
  {
    struct BattleWork *battle = Ram_HeapSlots->battleWork;

    gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
    BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  }
  wait(1);
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  for (i = 0; i != 18; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
  Iwram_FillWords(canvas, CANVAS_BYTES,
                  CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  args->targets[args->targetCount] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(args->targets, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
                             work->sheet + CRUEL_RUIN_BURST_BAND, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;
    u8 *c;

    if (v > CANVAS_LEVEL_BRIGHTEST) {
      v = CANVAS_LEVEL_BRIGHTEST;
    }
    /* FAKEMATCH(signature): the column's pointer computed as an integer. */
    for (j = 0,
        c = (u8 *)(i + (s32)work + offsetof(struct BattleEffectWork, sheet));
         j != 120; j++) {
      *c = v;
      c += 120;
    }
  }
  {
    for (frame = 0; frame != 140; frame++) {
      s32 grey = frame - 80;
      s32 blue = grey * 1024;
      s32 green = grey * 32;

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
          for (; i != 223; i++) {
            *dst++ = c;
          }
        } else if (frame > 47) {
          ColorBuffer_ScaleFar((void *)OBJ_PLTT, (void *)OBJ_PLTT,
                               FIXED_ONE * 15 / 16, 224);
        }
        if (frame > 47) {
          draw[0](canvas, work->sheet, 216 - frame * 2, 0, 120, 120);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        {
          s32 zero = 0;
          burstTexture.logWidth = 5;
          burstTexture.logHeight = 2;
          burstTexture.pixels = work->sheet + CRUEL_RUIN_BURST_BAND;
          model->shade = zero;
          model->mesh = gBattleEffect_SmallSquareMesh;
          model->texture = &burstTexture;
          model->vertices = verts;
          model->kind = MODEL_SHADED_MAX;
        }
        if (frame > 31) {
          struct Particle *q = &work->particles[CRUEL_RUIN_SPHERE];
          s32 r = frame * 2 - 32;

          if (r > 64) {
            r = 64;
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
          Matrix_Scale(r * 9400, FIXED_ONE * 2, FIXED_ONE / 2);
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
#undef BLITTER
#undef CLIP
#undef PLOT
#undef gSparkles
#undef ALT_BLITTER
#undef WAIT_TRANSFER
