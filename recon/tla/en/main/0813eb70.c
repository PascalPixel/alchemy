/* Near miss, differ 378 of 1102 (two instructions short).
 *
 * Twin: The Broken Seal's BattleEffect_RunTileAndPaletteAnimation, reached
 * from the scenes through the far-call table (table_47.s). It sets up its
 * own heap blocks (only the work and the canvas: the sheet block it releases
 * at the end is never taken), stores work->effect, marks the work active,
 * and reads the target from the argument. The twin's 12 unused frame bytes
 * are gone, and the IWRAM clears keep their own sizes (the twin's size
 * local is gone with them).
 *
 * Left: allocation through the whole body: the tile map's loop keeps its
 * counters on the stack and a constant in lr, the circle reloads cx and cy
 * from stack slots at each use, and the palette loop holds its masks in
 * high registers. Control flow and calls follow the listing.
 *
 * Tried:
 *   the twin as ported                                         378
 *   alchemy permute, 15 minutes: left and cy combined into one
 *     two-word array, so both live in memory as the native's
 *     reloads suggest (not kept: an array no reader would write) 307
 *   the same run's other bests: a temporary (341), an
 *     inequality turned round (326)
 */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "io_write_queue.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"
#include "effect_step.h"
#include "iwram_call.h"

typedef DrawRectangle BlitterPair[2];

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92      // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96      // battleFxWork[1]
#define SLOT_BATTLE_SHEET 100      // unk64[0]
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define CALLBACK_ORDER 0xc80

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// The battle background's id.
#define BACKGROUND(battle) (*(u16 *)(battle)->unk670)

// Puts colour 2 at (x, y) in the 256x120 8bpp tile sheet (32 tiles wide) in
// the map cell buffer.
#define PLOT(x, y)                                                             \
  (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +            \
     (y) / 8 * 2048) = 2)

// The 33 pieces: each one's size, place in the sheet and starting point
// (The Broken Seal's gBattlePieceWidth, Height, Offset, StartX and StartY).
extern u8 Data_08197609[];
extern u8 Data_0819762a[];
extern u16 Data_0819764c[];
extern u8 Data_08197690[];
extern u8 Data_081976b1[];

extern char ResourceId_TileAnimationMask;
extern char ResourceId_FlashBurstSheet;

void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_081434d8(void); // BattleFx_ClearActorTimers
s32 Func_081963ec(s32 slot, s32 flags);
s32 Func_08143000(void); // the canvas transfer callback
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
u32 Random16(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081434f8(void);           // ObjectGroup_TickMemberTimers
void WaitFrames(s32 frames);

// The IWRAM routines are called through one inline each, as The Broken
// Seal's iwram_call.h layer calls them: called directly, CSE shares the
// sizes between calls.
static inline s32 ClearWords(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

static inline s32 FillWords(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

/*
 * The target's picture shatters, for the scenes.
 *
 * A grey circle spreads from (96, 60) over the battle background, 32 pixels
 * of radius a frame for four frames, while the picture in the canvas breaks
 * into 33 pieces that fly outward and fall. Colours 6 to 13 start white and
 * step back to the battle background's own colours from frame 8; colour 1
 * brightens from black to white. At frame 51 a flash sheet replaces the
 * pieces, and 32 flashes rise from x 32-63, y 80-111, one more every four
 * frames. At the end the background and the display are put back.
 */
void Func_0813eb70(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 rmax;
  struct EffectStep *piece;
  s32 r;
  s32 i;
  s32 px;
  s32 j;
  s32 k;
  s32 d;
  s32 py;
  s32 x1;
  s32 left;
  s32 left2;
  s32 y1;
  s32 x2;
  s32 y2;
  u8 *startY;
  u8 *startX;
  s32 cx;
  s32 cy;

  work = Runtime_AllocateHeapBlock(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = Runtime_AllocateHeapBlock(SLOT_BATTLE_CANVAS, 0x4000);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work->effect = effect;
  work->framesSinceTransfer = 1;

  // Make BG2 a 128x128 canvas, as BattleFx_BeginCanvasLayer does.
  Func_081434d8();
  view->secondMode = 1;
  gBgScroll[1].y = 32;
  BattlePresentation_ConfigurePaletteFadeFar(1, BACKGROUND(battle), 0);
  REG_BG2CNT = 0x784;
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  view->secondMode = 0;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = 0x80;
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = 0x100;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;

  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s16 entry;

      entry = ((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
      *(volatile s16 *)(0x06003800 + i) = entry;
    }
  }

  ClearWords(canvas, 0x4000);
  ClearWords((void *)0x06004000, 0x4000);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work->sheet,
                             1, 1);
  work->transferMode = 1; // each transfer clears the canvas
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(Func_08143000, CALLBACK_ORDER);
  Func_081963ec(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Func_081963ec(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The pieces fly out from (32, 60) at a quarter of their distance a
  // frame.
  startY = Data_081976b1;
  startX = Data_08197690;
  r = 0;
  piece = work->particles;
  for (i = 0; i != 33; i++) {
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

  // Copy the picture at 0x06008000 aside, fill it with colour 1, and make
  // colours 6 to 13 white.
  CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
  FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
  FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
  Audio_PlayCue(212);
  Func_0814cd48(effect->actors[0], 7, 3, 0, 30);

  for (frame = 0; frame != 128; frame++) {
    if (frame == 2)
      Audio_PlayCue(212);
    if (frame == 3)
      Audio_PlayCue(212);
    if (frame == 28)
      Func_0814cd48(effect->actors[0], -1, 3, -1, 0);
    if (frame == 32)
      Audio_PlayCue(149);
    if (frame == 5)
      Audio_PlayCue(145);
    if (frame == 5) {
      gBgScroll[1].x = work->savedBg1X;
      BattleBackground_LoadFar(1, BACKGROUND(battle), -1);
    }

    // From frame 8, step each of colours 6 to 13 one shade towards the
    // battle background's.
    if (frame > 7) {
      u16 *pal;

      pal = (u16 *)0x050000c0;
      for (i = 0; i != 128; i++) {
        s32 red = *pal & 0x1f;
        s32 green = (*pal >> 5) & 0x1f;
        s32 blue = (*pal >> 10) & 0x1f;
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
        *pal++ = (blue << 10) | (green << 5) | red;
      }
    }

    if (frame == 4)
      FillWords((void *)0x06008000, 0x7800, 0x02020202);

    // Frames 0 to 3: widen the circle by 32 pixels of radius, brightening
    // its grey.
    if (frame <= 3) {
      s32 v;

      v = frame * 4 + 8;
      rmax = frame * 32;
      BG_PLTT[2] = (v << 10) | (v << 5) | v;
      cx = 96;
      cy = 60;
      for (; r != rmax; r++) {
        // The midpoint circle: plot each eighth by symmetry.
        px = r;
        py = 0;
        d = r;
        while (px >= py) {
          x2 = px + cx;
          x1 = cx - px;
          left = x1;
          y1 = cy - py;
          y2 = py + cy;
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
          left2 = x1;
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
          x1 = left2 + 1;
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
      CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
    }

    // Up to frame 50, draw the pieces; from frame 4 they fall.
    if (frame <= 50) {
      for (i = 0; i != 33; i++) {
        piece = &work->particles[i];
        draw[0](canvas, work->sheet + Data_0819764c[i], HI(piece->x),
                HI(piece->y), Data_08197609[i], Data_0819762a[i]);
        if (frame > 3)
          BattleFxKernels_IntegrateVector2(piece, 64, 0x4000);
      }
    }

    // Frames 8 to 50: colour 1 from black to white.
    if (frame >= 8 && frame <= 50) {
      s32 v;

      v = frame - 8;
      if (v > 31)
        v = 31;
      BG_PLTT[1] = (v << 10) | (v << 5) | v;
    }

    // Frame 51: the flash pictures and colours, and 32 flashes placed at
    // x 32-63, y 80-111.
    if (frame == 51) {
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                                 work->sheet, 1, 0);
      for (i = 1; i != 64; i++) {
        s32 v;

        v = i / 2;
        if (v < 0)
          v = 0;
        BG_PLTT[i] = (v << 10) | ((v / 2) << 5) | v;
      }
      REG_BLDCNT = 0x3f44;
      for (i = 0; i != 32; i++) {
        piece = &work->particles[i];
        piece->x = ((Random16() & 31) + 32) << 16;
        piece->y = ((Random16() & 31) + 80) << 16;
        piece->velocityX = ((Random16() & 0x1ff) - 0x100) << 12;
        piece->velocityY = 0;
        piece->variant = 0;
      }
      work->transferMode = 2;   // each transfer halves the old picture,
      work->transferValue = 50; // leaving a trail
    }

    // The flashes start i/4 frames late and rise, a picture every 4 frames.
    if (frame > 52) {
      for (i = 0; i != 32; i++) {
        piece = &work->particles[i];
        if (frame >= i / 4 + 52 && piece->variant <= 39) {
          s32 cell;

          cell = piece->variant / 4;
          if (cell > 5)
            cell = 5;
          draw[1](canvas, work->sheet + (cell << 11), HI(piece->x) - 16,
                  HI(piece->y) - 32, 32, 64);
          BattleFxKernels_IntegrateVector2(piece, 60, -0x1000);
          piece->variant++;
        }
      }
    }

    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Put the background and the display back.
  Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Scheduler_RemoveCallback(Func_08143000);
  Func_0814cd48(effect->actors[0], -1, 1, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattlePresentation_ConfigurePaletteFadeFar(2, BACKGROUND(battle), 0);
  WaitFrames(1);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_EFFECT);
}
