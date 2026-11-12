#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_release.h"
#include "io_write_queue.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"
#include "effect_step.h"
#include "iwram_call.h"
#include "task.h"

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

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// The battle background's id, read through a union whose s32 member gives
// the read view->secondMode's alias set and not the scroll's, so those two
// stores stay in source order.
union BattleBackgroundId {
  u16 id;
  s32 word;
};
#define BACKGROUND(battle) (((union BattleBackgroundId *)(battle)->unk670)->id)

// Puts colour 2 at (x, y) in the 256x120 8bpp tile sheet (32 tiles wide) in
// the map cell buffer.
#define PLOT(x, y)                                                             \
  (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +            \
     (y) / 8 * 2048) = 2)

// The 33 pieces: each one's size, place in the sheet and starting point
// (The Broken Seal's gDeathPlunge_PieceWidths, Height, Offset, StartX and
// StartY).
extern u8 Data_08197609[];
extern u8 Data_0819762a[];
extern u16 Data_0819764c[];
extern u8 Data_08197690[];
extern u8 Data_081976b1[];

extern char ResourceId_TileAnimationMask;
extern char ResourceId_FlashBurstSheet;

void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ClearActorTimers(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter

void Sound_PlayFar(s32 cue);
u32 Random16(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);

static inline s32 ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_ClearWords, 14 of 1102 instructions of Func_0813eb70 differ in
   * tla-en. */
  return Iwram_ClearWords(destination, size);
}

static inline s32 FillWords(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_FillWords, 4 of 1102 instructions of Func_0813eb70 differ in
   * tla-en. */
  return Iwram_FillWords(destination, size, value);
}

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 47 of 1102 instructions of Func_0813eb70 differ in
   * tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

// The target's picture shatters, for the scenes.

// A grey circle spreads from (96, 60) over the battle background, 32 pixels
// of radius a frame for four frames, while the picture in the canvas breaks
// into 33 pieces that fly outward and fall. Colours 6 to 13 start white and
// step back to the battle background's own colours from frame 8; colour 1
// brightens from black to white. At frame 51 a flash sheet replaces the
// pieces, and 32 flashes rise from x 32-63, y 80-111, one more every four
// frames. At the end the background and the display are put back.

void Func_0813eb70(struct BattleEffectArgument *effect) {
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
  struct BattleBackgroundView *view;
  struct EffectStep *piece;
  s32 i;
  s32 px;
  s32 py;
  s32 x1;
  s32 left;
  s32 left2;
  s32 y1;
  s32 x2;
  s32 y2;
  s32 cx;
  s32 cy;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work->effect = effect;
  work->vblanksSinceTransfer = 1;

  // Make BG2 a 128x128 canvas, as BattleEffect_StartCanvasLayer does.
  BattleEffect_ClearActorTimers();
  view->secondMode = 1;
  gBgScroll[1].y = 32;
  BattleStaging_ConfigurePaletteFadeFar(1, BACKGROUND(battle), 0);
  REG_BG2CNT = 0x784;
  BattleCamera_SetViewFar(0, 0, 0, 100);
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
      // The map written through a volatile pointer, which keeps the ROM's
      // order of stores; as a plain pointer, 295 of 1102 instructions differ
      // in tla-en.
      *(volatile s16 *)(0x06003800 + i) = entry;
    }
  }

  ClearWords(canvas, 0x4000);
  ClearWords((void *)0x06004000, 0x4000);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TileAnimationMask, work->sheet, 1,
                             1);
  work->transferMode = 1; // each transfer clears the canvas
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The pieces fly out from (32, 60) at a quarter of their distance a
  // frame.
  r = 0;
  for (i = 0; i != 33; i++) {
    s32 x, y, vx, vy;

    piece = &work->particles[i];
    x = Data_08197690[i] << 16;
    y = Data_081976b1[i] << 16;
    piece->x = x;
    piece->y = y;
    vx = (x - 0x200000) >> 2;
    vy = (y - 0x3c0000) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
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
  Sound_PlayFar(212);
  Func_0814cd48(effect->actors[0], 7, 3, 0, 30);

  for (frame = 0; frame != 128; frame++) {
    if (frame == 2)
      Sound_PlayFar(212);
    if (frame == 3)
      Sound_PlayFar(212);
    if (frame == 28)
      Func_0814cd48(effect->actors[0], -1, 3, -1, 0);
    if (frame == 32)
      Sound_PlayFar(149);
    if (frame == 5)
      Sound_PlayFar(145);
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
          BattleEffectKernels_IntegrateVector2(piece, 64, 0x4000);
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
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet,
                                 1, 0);
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
          BattleEffectKernels_IntegrateVector2(piece, 60, -0x1000);
          piece->variant++;
        }
      }
    }

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Put the background and the display back.
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Func_0814cd48(effect->actors[0], -1, 1, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattleStaging_ConfigurePaletteFadeFar(2, BACKGROUND(battle), 0);
  wait(1);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  HeapSlot_Release(SLOT_BATTLE_SHEET);
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}

// Twin: The Broken Seal's BattleEffect_RunElementBurst, reached from the
// scenes through the far-call table (table_47.s). Like 0813e3d0 it sets up
// its own heap blocks, stores work->effect and marks the work active
// itself, and reads the actors from the argument. The corner sparks' four
// blitters are loaded through Func_08196404 directly, with all five
// arguments.

#include "fixed_math.h"
#include "resource.h"

// The rising sparkles live in the map cell buffer.
#define gSparkles ((struct EffectStep *)Ram_MapCellBuffer)

// The four corner sparks: their flips and their place around the target
// (The Broken Seal's gDeathPlunge_DrawFlags, _X and _Y).
extern u8 Data_081976da[];
extern s8 Data_081976d2[];
extern s8 Data_081976d6[];

// The mote's cell for each size from 1 to 6 (gBattleEffect_FlareCells).
extern u16 Data_08197424[];

extern char ResourceId_CyanSparkSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowPaletteA;
extern char ResourceId_IceTileSheet;
extern char ResourceId_RedPaletteA;
extern char ResourceId_VioletPaletteA;

void BattleEffect_StartCanvasLayer(s32 control);

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter

// Light gathering on the user and bursting, for the scenes.

// For the first 15 frames four sparks flare at the user's corners, each
// mirrored to face the middle; 64 motes spiral in from a ring around the
// user, shrinking as they come. At frame 26 the first target flashes with a
// sound, and from frame 28 to 48 a 48x48 burst plays out over seven
// pictures. effect->kind picks the colours (yellow, ice, red or violet). The
// last 6 frames fade out. The 128 sparkles set up in the map cell buffer are
// never drawn.

void Func_0813f518(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  sheet = HeapSlot_Alloc(SLOT_BATTLE_SHEET, 0x60e);
  work->vblanksSinceTransfer = 1;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (effect->kind) {
  case 0:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case 1:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case 2:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  {
    // The entry held in a local loads before the palette's address; as a
    // plain Iwram_CopyWords call, 2 of 380 instructions differ in tla-en.
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, 128);
  }

  for (i = 0; i != 128; i++) {
    gSparkles[i].y = 0x800000;
    gSparkles[i].x = Random16() & 0xffff;
    gSparkles[i].z = (Random16() & 0x1ff) + 0x400;
    gSparkles[i].variant = -i;
  }

  // A mote's x is its angle and y its distance from the user; its variant
  // counts down its life.
  for (i = 0; i != 64; i++) {
    work->particles[i].x = Random16() & 0xffff;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant = (i & 15) + 16;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                           BLITTER(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Sound_PlayFar(140);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
    if (frame == 26) {
      Sound_PlayFar(212);
      Func_0814cd48(effect->actors[0], 7, -1, 0, 20);
    }

    // The burst, a picture every three frames.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40,
              ground.y - 24, 48, 48);

    // The corner sparks, each with its own flips.
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      // mode holds BLIT_MAX in r6 across the loop; as BLIT_MAX in the call,
      // 38 of 380 instructions differ in tla-en.
      s32 mode;

      for (i = 0, mode = BLIT_MAX; i != 4; i++) {
        Func_08196404(SLOT_BLITTER_ALTERNATE, 7, 7, Data_081976da[i] | CLIP,
                      mode);
        draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        draw[1](canvas, work->sheet + (picture << 10), Data_081976d2[i] + 32,
                ground.y + Data_081976d6[i] - 32, 32, 32);
        HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
      }
    }

    // The motes close in 2 pixels a frame.
    if (frame >= 0) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0 && mote->y > 0) {
          s32 size = (mote->variant >> 3) + 1;
          s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
          s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

          if (size <= 0)
            size = 1;
          draw[0](canvas, sheet + Data_08197424[size - 1], x - size, y - size,
                  size * 2, size * 2);
          mote->y -= 2;
          mote->variant--;
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(SLOT_BATTLE_SHEET);
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}

#include "irq_lock.h"

struct BattleFxFadeWork {
  s32 unk00;
  s32 halvePending;
  s32 framesLeft;
  s32 step;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);

/// Frame callback: when a transfer is pending, writes the canvas to BG
/// character block 1 at half brightness.
void BattleEffect_TransferHalvedCanvas(void) {
  struct BattleFxFadeWork *work =
      (struct BattleFxFadeWork *)Ram_HeapSlots->battleFxWork[0];
  void *buffer = Ram_HeapSlots->battleFxWork[1];
  u32 ime;

  IRQ_LOCK(ime);
  if (work->halvePending == 1) {
    work->halvePending = 0;
    IRQ_UNLOCK(ime);
    {
      void *destination = (void *)0x06004000;

      ColorBuffer_Halve(buffer, destination, 0x4000);
    }
  } else {
    IRQ_UNLOCK(ime);
  }
}

#include "system.h"

struct StreakWork {
  s32 frame;
  s32 transferPending;
};

struct Streak {
  s32 head;
  s32 tail;
  s32 unused08;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 unused18;
};

struct StreakPoint {
  s32 x;
  s32 y;
  s32 z;
};

void Render_ResetTransformState(void);
void Func_080150e4(s32 roll); // SceneTransform_ApplyRoll
void SceneTransform_ApplyPitch(s32 pitch);
void SceneTransform_ApplyYaw(s32 yaw);

void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

/// The Broken Seal's BattleEffect_DrawStreaks.
void Func_0813f8d4(void) {
  struct StreakWork *work = (struct StreakWork *)Ram_HeapSlots->battleFxWork[0];
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

      Render_ResetTransformState();
      Func_080150e4(streak->roll);
      SceneTransform_ApplyPitch(streak->pitch);
      SceneTransform_ApplyYaw(streak->yaw);
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
      Func_08143eb4(tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
      Func_08143eb4(tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
      Func_08143eb4(tail.x, tail.y, head.x, head.y, fade + 56);
    }
  }
  work->transferPending = 1;
}

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

/// Each frame while frames are left, darkens the battle background's 128
/// colours by one more step of 1092 into BG palette 0x050000c0.
void BattleEffect_DimBackgroundStep(void) {
  struct BattleFxFadeWork *work =
      (struct BattleFxFadeWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *battle = (u8 *)Ram_HeapSlots->battleWork;

  if (work->framesLeft > 0) {
    work->step++;
    ColorBuffer_ScaleFar(battle + 0x56c, (void *)&BG_PLTT_COLOR(6, 0),
                         0x10000 - work->step * 1092, 128);
    work->framesLeft--;
  }
}

#undef BACKGROUND

// The battle background's id.
#define BACKGROUND(battle) (*(u16 *)(battle)->unk670)

// The backdrop's work, in place of the battle effect work.
struct StreakSceneWork {
  s32 frame;        // the streaks' frame count (Func_0813f8d4)
  s32 halvePending; // the streaks are drawn: halve the canvas into VRAM
  s32 fadeFrames;   // frames of the background's fade left
                    // (BattleEffect_DimBackgroundStep)
  s32 fadeStep;
};

extern char ResourceId_YellowPaletteB;
extern char ResourceId_CrystalSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_VioletPaletteD;
extern char ResourceId_DustPuffSheet;

void BattleEffect_ConfigureEffectDisplay(void);
void Func_0814355c(void); // the BG2 canvas's registers

// Raises the light-streak backdrop in colour set kind (yellow, crystal,
// ember, violet or dust).

void Func_0813fa88(s32 kind) {
  struct StreakSceneWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 id;
  s32 i;
  s32 j;
  s32 k;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, sizeof *work);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;

  // Turn BG2 into a 128x128 canvas, one step a frame.
  BattleEffect_ConfigureEffectDisplay();
  view->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x1741, 0x20000);
  gBgScroll[1].y = 32;
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(1, BACKGROUND(battle), 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  wait(1);
  REG_BG2CNT = 0x784;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  view->secondMode = 0;
  wait(1);
  Func_0814355c();
  REG_BG2PA = 0x100;
  REG_WININ = 0x3537;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);

  i = 0;
  for (k = 0; k != 16; k++) {
    for (j = 0; j != 8; j++, i += 2) {
      s32 entry;

      entry = (s16)(((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      // The map written through a volatile pointer, which keeps the ROM's
      // order of stores; as a plain pointer, 97 of 280 instructions differ in
      // tla-en.
      *(volatile u16 *)(0x06003800 + i) = *(u16 *)&entry;
    }
  }
  /* FAKEMATCH(wrapper): the clears through ClearWords; as a direct call to
   * Iwram_ClearWords, 14 of 280 instructions of Func_0813fa88 differ in tla-en.
   */
  ClearWords(canvas, 0x4000);
  ClearWords((void *)0x06004000, 0x4000);
  wait(1);

  switch (kind) {
  case 0:
    id = (s32)&ResourceId_YellowPaletteB;
    break;
  case 1:
    id = (s32)&ResourceId_CrystalSheet;
    break;
  case 2:
    id = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 3:
    id = (s32)&ResourceId_VioletPaletteD;
    break;
  case 4:
  default:
    id = (s32)&ResourceId_DustPuffSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(id), 128);

  // Fade the background over 24 frames, and draw and halve the streaks
  // every frame.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(BattleEffect_DimBackgroundStep, 0x480);
  work->halvePending = 0;
  Task_Add(BattleEffect_TransferHalvedCanvas, 0xc80);
  work->frame = 0;
  Task_Add(Func_0813f8d4, 0x480);
}

// The clear goes through an inline, as the Broken Seal's iwram_call.h writes
// it: called straight through the fixed entry, the build sets the size before
// it loads the routine's address.
#undef Iwram_ClearWords
static inline s32 Iwram_ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the clear through this inline; as a direct call to
   * Iwram_ClearWordsEntry, 2 of 18 instructions of
   * BattleEffect_EndElementStreaks differ in tla-en. */
  return Iwram_ClearWordsEntry(destination, size);
}

/// Ends the light-streak backdrop: removes its three callbacks, clears the
/// canvas in VRAM and frees heap blocks 96 and 92.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(Func_0813f8d4);
  Task_Remove(BattleEffect_TransferHalvedCanvas);
  Task_Remove(BattleEffect_DimBackgroundStep);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
}

#undef HI

struct FxPartyList;
struct Model;
struct StripQuad;

#include "render_model.h"

struct ModelTextureB {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);

void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void **GetBattleObjectSlotFar(s32 objectId);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

#define HI(v) (((s16 *)&(v))[1])

#include "dma.h"
#include "input.h"
#include "battle.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct RuinObject {
  u8 unk0[8];
  union {
    struct {
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } bits;
    u16 all;
  } attr;
  u8 unk0a[4];
  u8 resource;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern char ResourceId_Data0B4, ResourceId_Data0C2, ResourceId_Data0C9;
extern char ResourceId_Data0CA, ResourceId_Data104, ResourceId_Data134;
extern char ResourceId_CornerRuneASheet, ResourceId_ForestCanopyTiles;
extern char ResourceId_SunbeamCloudsBackdrop, ResourceId_VioletSkyBackdrop;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02012000[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196dd8;
extern const u16 Data_08197438[];
extern const u8 Data_081976e0[];
extern const u16 Data_081976f0[];
extern const u16 Data_081976f8[];
extern const u8 Data_08197700[];
extern const u8 Data_08197702[];
extern const u8 Data_08197704[];
extern const u8 Data_0819770a[];
extern const u8 Data_08197710[];
extern const u8 Data_08197716[];
extern const u8 Data_0819771c[];
extern const u8 Data_08197720[];
extern const u8 Data_08197724[];
extern const u8 Data_08197728[];
extern const u8 Data_0819772c[];
extern const u8 Data_08197730[];
extern const s32 Data_08197734[];
extern const u8 Data_081991c0[];
extern const u8 Data_081991e0[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);
struct RuinObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);

void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815b510(u8 *map, s32 size, s32 scale);
void Func_08164a4c(s32 r, s32 g, s32 b);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);

#define SLOT_ALT 188 // unkb8[1]
#define ALT_BLITTER ((DrawRectangle)Ram_HeapSlots->unkb8[1])
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)
// The listing reads the transfer count twice per pass of its wait.
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                       \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

// Forwarded so that a constant size reaches the call's register at each call.
static __inline__ void FillWords_(void *destination, s32 size, u32 value) {
  Iwram_FillWords(destination, size, value);
}
static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

// Cruel Ruin: a pillar of dark light swallows the caster, a shadowed sky
// rains ruin in six blasts while the ground splits, then a black sphere
// bursts over the foes.
void BattleEffect_RunCruelRuin(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  s32 place[4];
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct WordPair req;
  struct ModelTexture texture3;
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 *transition;
  s32 savedScroll;
  s32 scrollX;
  s32 phase;
  s32 *actor;
  s32 i;
  s32 j;
  s32 k;
  s32 fr;
  s32 fg;
  s32 fb;
  s32 dr;
  s32 dg;
  s32 db;
  s32 steps;
  s32 u;
  struct RuinObject *obj;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  work->transferMode = 2;
  work->transferValue = 50;
  draw[0] = Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  savedScroll = gBgScroll[1].x;
  scrollX = 0;
  phase = 0;
  actor = *GetBattleObjectSlotFar(effect->actor);
  actor[18] = phase;
  Graphics_LoadGreyRamp();
  Sound_PlayFar(140);
  for (frame = 0; frame != 36; frame++) {
    s32 size;

    if (frame == 24) {
      work->transferMode = 4;
      work->transferValue = 0x06060606;
    }
    EffectPosition_ProjectBattler(effect->actor, &pos);
    size = frame * 6 + 2;
    if (size > 96) {
      size = 96;
    }
    Func_0815b434(work->sheet, size, 0x10000);
    Func_0818caa8(work->sheet, pos.x / 2, pos.y - 16, size);
    if (frame > 15) {
      actor[3] += 0x100000;
    }
    work->transferPending = 1;
    wait(1);
  }
  actor[18] = 0xab85;
  actor[3] = 0;
  FillWords_((void *)0x06004000, 0x4000, 0x3f3f3f3f);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, (s32)&ResourceId_SunbeamCloudsBackdrop, 0);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  draw[0] = Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet, work->sheet, 1,
                             0);
  Func_0816ae40(work->sheet, gData_02014000, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2648, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3648, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3688, 0,
                             0);
  for (i = 0; i != 10; i++) {
    s32 size = i * 2 + 2;

    Func_0815b510(work->sheet + 0x3a48 + Data_08197438[i], size, 0x10000);
  }
  Graphics_LoadGreyRamp();
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BLDALPHA = 0x1010;
  fr = 0;
  fg = 0;
  fb = 0;
  dr = 0;
  dg = 0;
  db = 0;
  steps = 0;
  for (i = 0; i != 24; i++) {
    struct EffectStep *p = &work->particles[i];
    p->z = i * 0xaaa + (Random16() & 0xfff);
    p->variant = -(Random16() & 15);
  }
  for (i = 0; i != 31; i++) {
    struct EffectStep *p = &work->particles[i + 32];
    p->x = Random16() & 127;
    p->y = (Random16() & 31) + 88;
    p->z = (Random16() & 3) + 5;
    p->variant = i * -2;
  }
  {
    {
      u8 *pixels = gMapCellBuffer;
      for (frame = 0; frame != 180; frame++) {
        struct RenderModel *model;
        void *verts;
        s32 shade;

        if ((gInput.repeat & 3) && frame <= 168) {
          frame = 169;
        }
        if (frame == 4) {
          Sound_PlayFar(220);
        }
        if (frame == 40) {
          Sound_PlayFar(154);
        }
        if (frame == 64) {
          Sound_PlayFar(139);
        }
        if (frame == 74) {
          Sound_PlayFar(139);
        }
        if (frame == 84) {
          Sound_PlayFar(139);
        }
        if (frame == 94) {
          Sound_PlayFar(139);
        }
        if (frame == 124) {
          Sound_PlayFar(144);
        }
        for (i = 0; i != 6; i++) {
          if (frame == 140 + i * 4) {
            Sound_PlayFar(104);
          }
        }
        if (frame == 169) {
          Sound_PlayFar(145);
          work->shakeFrames = 16;
        }
        if (frame == 0) {
          Func_08164b2c(0, 0, 0);
        }
        if (frame == 32) {
          fr = 0;
          fg = 0;
          fb = 0;
          dr = -0x2492;
          dg = -0x6db6;
          db = -0x2492;
          steps = 28;
        }
        if (frame == 124) {
          fr = 0x1f0000;
          fg = 0x80000;
          fb = -0x40000;
          dr = 0;
          dg = 0;
          db = 0;
          steps = 1;
        }
        if (frame == 128) {
          dr = (-0x10000 - fr) / 16;
          dg = (0x60000 - fg) / 16;
          db = -fb / 16;
          steps = 16;
        }
        if (frame == 144) {
          dr = (-0x90000 - fr) / 8;
          dg = (0xd0000 - fg) / 8;
          db = (-0x50000 - fb) / 8;
          steps = 8;
        }
        if (frame == 156) {
          dr = (0x1f0000 - fr) / 16;
          dg = (0x80000 - fg) / 16;
          db = (-0x40000 - fb) / 16;
          steps = 16;
        }
        if (steps > 0) {
          steps--;
          fr += dr;
          fg += dg;
          fb += db;
          Func_08164b2c(fr >> 16, fg >> 16, fb >> 16);
        }
        if (frame == 124) {
          FillWords_(canvas, 0x4000, 0x3f3f3f3f);
        }
        if (frame <= 123) {
          s32 size = 140;
          s32 y = 58;
          s32 scale = 0x20000;

          if (frame <= 29) {
            size -= frame * 4;
          } else {
            size = 114 - frame * 3;
          }
          if (frame > 7) {
            scale = 0x24000 - (frame << 11);
          }
          if (scale <= 0x7fff) {
            scale = 0x8000;
          }
          if (frame > 35) {
            y = 20;
          } else if (frame > 19) {
            y = (Trig_Cos((frame << 10) + -0x5000) * 44 >> 16) + 14;
          }
          if (size <= 7) {
            size = 8;
          }
          if (frame > 67) {
            size += (frame - 68) / 6;
          }
          Func_0815b434(work->sheet, size, scale);
          Func_0818caa8(work->sheet, 60, y, size);
        }
        if (frame > 127) {
          for (i = 0; i != 31; i++) {
            struct EffectStep *p = &work->particles[i + 32];

            if ((u32)p->variant <= 15) {
              s32 s = Trig_Sin(p->variant << 10) * p->z >> 16;

              draw[0](canvas, work->sheet + 0x3a48 + Data_08197438[s - 1],
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
          model->kind = 6;
          model->texture = &texture;
          model->mesh = gData_08199364;
          model->vertices = verts;
          texture.pixels = pixels;
          shade = 0;
          model->shade = shade;
          i = shade;

          for (s = (frame << 15) + -0x200000, t = frame - 64; i != 4;
               s += -0x50000, i++, t -= 10) {
            if ((u32)t <= 15) {
              s32 sc = 0x80000 - s;

              Render_ResetTransformState();
              Func_08015160(-0x40000, -0x2c0000, 0);
              Func_080151e4(sc / 2, sc, sc);
              SceneTransform_ApplyYaw(Data_081976f8[i]);
              SceneTransform_ApplyPitch(Data_081976f0[i]);
              Render_TransformVertices(Data_081991e0, verts, 4);
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
          Render_ResetTransformState();
          Func_08015160(-0x40000, -0x2c0000, 0);
          Func_080151e4(sc >> 1, sc, sc);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
        {
          s32 t = frame - 24;

          if ((u32)t <= 99) {
            s32 sc = 0x30000 - (t << 11);

            if (sc <= 0xffff) {
              sc = 0x10000;
            }
            shade = -32;
            if (t <= 7) {
              shade = t * 4 - 64;
            }
            if (frame > 63) {
              shade = 224 - frame * 4;
            }
            model->shade = shade;
            texture.pixels = gData_02014000;
            t <<= 9;
            Render_ResetTransformState();
            Func_08015160(-0x40000, -0x2c0000, 0);
            Func_080151e4(sc, sc / 3, sc >> 1);
            Func_080150e4(t);
            if (shade > -64) {
              Render_TransformVertices(Data_081991e0, verts, 4);
              Render_DrawModelList(model);
            }
            texture.pixels = work->sheet + 0x2648;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->mesh = gData_08199340;
            if (frame > 63) {
              shade = frame * 4 + -320;
              if (shade > 0) {
                shade = 0;
              }
              model->shade = shade;
              Render_ResetTransformState();
              Func_08015160(-0x40000, -0x2c0000, 0);
              Func_080151e4(sc, sc >> 1, sc >> 1);
              Func_080150e4(t);
              Render_TransformVertices(Data_081991e0, verts, 4);
              Render_DrawModelList(model);
            }
          }
        }
        if (frame > 123) {
          ((struct ModelTextureB *)&texture)->logWidth = 5;
          texture.logHeight = 2;
          texture.pixels = work->sheet + 0x3648;
          model->mesh = Data_08199220;
          model->kind = 6;
          shade = -16;
          model->shade = shade;
          for (i = 0; i != 24; i++) {
            struct EffectStep *p = &work->particles[i];

            if (p->variant >= 0) {
              s32 sy = p->variant << 13;

              if (sy > 80000) {
                sy = 80000;
              }
              Render_ResetTransformState();
              Func_08015160(-0x40000, -0x2c0000, 0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4(p->z);
              Func_080151e4(30384, sy, 0x8000);
              Render_TransformVertices(Data_081991c0, verts, 4);
              Render_DrawModelList(model);
            }
            p->variant++;
          }
          if (frame > 163) {
            work->transferMode = 1;
            work->transferValue = 0x01010101;
            Func_08164a4c(2, 2, 2);
          }
          if (frame > 159) {
            struct EffectStep *q = &work->particles[63];
            s32 r = frame * 2 + -288;

            if (r > 64) {
              r = 64;
            }
            if (frame == 160) {
              q->x = 0x800000;
              q->y = -0x200000;
            }
            if (frame <= 171) {
              q->x += -0x80000;
              q->y += 0x100000;
              Func_0815b434(work->sheet, r, 0x10000);
              Func_0818caa8(work->sheet, HI(q->x), HI(q->y), r);
            }
            model->kind = 6;
            shade = 0;
            model->shade = shade;
            texture.pixels = work->sheet + 0x3688;
            Render_ResetTransformState();
            Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
            Func_080151e4(0x8000, 0x10000, 0x8000);
            Func_080150e4(0xa000);
            Func_080151e4(r * 9400, 0x20000, 0x8000);
            Render_TransformVertices(Data_081976e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
        if (work->shakeFrames > 0) {
          {
            s32 y_ = Random16() % 8 + 28;
            gBgScroll[1].y = y_;
          }
          work->shakeFrames--;
        } else {
          gBgScroll[1].y = work->savedBg1Y;
        }
        BattleEffect_CountDownBattlerTints();
        WAIT_TRANSFER(work);
        work->transferPending = 1;
        wait(1);
      }
    }
  }
  {
    s32 count = -1;
    s32 fadeStep = 2;
    u8 *strip = Data_02012000;
    void *verts;
    Render_BuildQuadStrip(strip, 8, 16, 32);
    for (k = 0; k != 2; k++) {
      s32 c;

      for (c = 0; c != 33; c++) {
        s32 w = Data_08197702[k];
        s8 *v;
        s32 acc;
        s32 height;
        s32 half;

        if (c > 15) {
          w = w + c * 2 - 32;
        }
        acc = 0;
        {
          s8 *cells = (s8 *)gMapCellBuffer;
          s32 cell = k * 32 + k + c;

          v = cells + cell * 8;
        }
        height = 126 - c * 7;
        half = w / 2;
        for (i = 0; i != 2; i++, acc += w, v += 4) {
          v[0] = height;
          v[1] = (Trig_Sin(c * 0x92c) * Data_08197700[k] >> 16) + acc - half;
          v[2] = 0;
        }
      }
    }
    work->transferMode = 2;
    work->transferValue = 50;
    FillWords_(canvas, 0x4000, 0x3f3f3f3f);
    work->transferPending = 1;
    wait(1);
    FillWords_(canvas, 0x4000, 0);
    REG_BG2CNT = 0x786;
    BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
    Func_08164abc(31, 31, 31);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3648,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data104, work->sheet + 0x36c8,
                               0, 0);
    Graphics_LoadGreyRamp();
    {
      u8 *e = Resource_GetTableEntry((s32)&ResourceId_ForestCanopyTiles);

      FillWords_((void *)&OBJ_PLTT_COLOR(15, 0), 32, 0x7fff7fff);
      Resource_DecodeType01(e + 32, work->sheet);
    }
    {
      s32 q;
      for (q = 0; q != 2; q++) {
        u8 *src;
        struct RuinObject *obj;

        i = 0;
        src = work->sheet + q * 0x1000;
        for (; i != 8; i++) {
          obj = Func_0815b290(32, 32, 0x80000000, 0xf000);
          work->objects[q * 9 + i] = obj;
          {
            s32 b = ((u8 *)obj)[9];
            u8 x = b & -13;
            x |= 4;
            ((u8 *)obj)[9] = x;
          }
          CopyWords_((void *)OBJ_TILES(obj), src, 512);
          src += 512;
        }
        obj = Func_0815b3b0(32, 32, 0x80000000, 0xf000);
        work->objects[q * 9 + 8] = obj;
        CopyWords_(obj, work->objects[q * 9], 24);
        for (i = 0; i != 8; i++) {
          work->particles[q * 9 + i].variant =
              ((struct RuinObject *)work->objects[q * 9 + i])->attr.bits.tile;
        }
      }
    }
    wait(1);
    frame = 0;
    while (1) {
      struct RenderModel *model;

      if ((gInput.repeat & 3) && frame <= 84 && count == -1) {
        work->transferMode = 4;
        work->transferValue = 0x04040404;
        count = 17;
        REG_BG2CNT = 0x784;
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
        Func_08164b2c(r, g, b);
        if (frame <= 16) {
          u16 *e = Resource_GetTableEntry((s32)&ResourceId_ForestCanopyTiles);
          u16 *dst = (u16 *)&OBJ_PLTT_COLOR(15, 0);
          s32 add;

          i = 0;
          add = 32 - frame * fadeStep;
          for (; i != 16; i++, e++, dst++) {
            s32 cr = (*e & 31) + add;
            s32 cg = (u16)((*e >> 5) & 31) + add;
            s32 cb = (u16)((*e >> 10) & 31) + add;

            if (cr > 31) {
              cr = 31;
            }
            if (cg > 31) {
              cg = 31;
            }
            if (cb > 31) {
              cb = 31;
            }
            *dst = cb << 10 | cg << 5 | cr;
          }
        }
      }
      if (frame == 0) {
        REG_BG2CNT = 0x786;
      }
      if (frame == 75) {
        REG_BG2CNT = 0x784;
      }
      if (frame == 85) {
        work->transferMode = 4;
        work->transferValue = 0x04040404;
      }
      verts = Heap_AllocEwram(528);
      model = Render_AllocateModelList(1);
      {
        s32 zero = 0;
        texture2.logWidth = 5;
        texture2.logHeight = 2;
        texture2.pixels = work->sheet + 0x3648;
        model->shade = zero;
        model->mesh = Data_08199220;
        model->texture = &texture2;
        model->vertices = verts;
        model->kind = 6;
        for (i = zero; i != 6; i++) {
          s32 d = frame - Data_08197704[i];

          if (d == 0) {
            if (i == 5) {
              Sound_PlayFar(145);
            } else {
              Sound_PlayFar(144);
            }
            work->shakeFrames = 8;
            FillWords_(canvas, 0x4000, 0x3f3f3f3f);
          }
          if ((u32)d <= 15) {
            s32 r = (Trig_Sin(d << 11) * Data_0819770a[i] >> 16) + 2;

            if (r > 1) {
              s32 x = Data_08197710[i] + d * 8;
              s32 y = Data_08197716[i];
              s32 half;

              Func_0815b434(work->sheet, r, 0x10000);
              /* FAKEMATCH(dead): a test that is always true here (the block is
               * entered only when r > 1); its two arms end common subexpression
               * elimination's path at their join, so the circle's work->sheet
               * and the first blit's stay two sums, and the join is gone before
               * scheduling; as BattleEffect_LoadBlitter(SLOT_ALT, 19), 440 of
               * 2579 instructions differ in tla-en. */
              BattleEffect_LoadBlitter(SLOT_ALT, r > 1 ? 19 : 0);
              half = r / 2;
              y -= r;
              ALT_BLITTER(canvas, work->sheet, x - half, y + 8, half, r);
              HeapSlot_Release(SLOT_ALT);
              /* FAKEMATCH(dead): the same always-true test, keeping the two
               * blits' work->sheet apart; as BattleEffect_LoadBlitter(SLOT_ALT,
               * 23), 436 of 2579 instructions differ in tla-en. */
              BattleEffect_LoadBlitter(SLOT_ALT, r > 1 ? 23 : 0);
              ALT_BLITTER(canvas, work->sheet, x, y + 8, half, r);
              HeapSlot_Release(SLOT_ALT);
            }
          }
        }
      }
      for (i = 0; i != 4; i++) {
        struct EffectStep *p = &work->particles[i + 18];
        s32 start = Data_0819771c[i] >> 1;

        if (frame == 0) {
          p->y = frame;
          p->variant = frame;
          p->x = Data_08197724[i] << 16;
        }
        if (frame >= start && frame < start + 32) {
          s32 size = 0;
          s32 px = 0;
          s32 py = 0;
          s32 angle = 0;
          s32 scale = 0;
          size = Data_08197720[i];

          if (p->y < Data_08197730[i] << 16) {
            p->x -= Data_08197728[i] << 16;
            p->y += Data_0819772c[i] << 16;
          } else {
            p->variant++;
          }
          p->x += 0x80000;
          Render_ResetTransformState();
          px = p->x + -0x400000;
          py = p->y + -0x400000;
          Func_08015160(px, py, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          angle = Data_08197734[i];
          Func_080150e4(angle);
          scale = size * 9400;
          Func_080151e4(scale, 0x20000, 0x8000);
          Render_TransformVertices(Data_081976e0, verts, 4);
          Func_0815b434(work->sheet, size, 0x10000);
          Func_0818caa8(work->sheet, HI(p->x) - 1, HI(p->y), size);
          Render_DrawModelList(model);
        }
      }
      {
        s32 six = 6;
        texture2.logWidth = six;
        texture2.logHeight = 5;
        texture2.pixels = work->sheet + 0x36c8;
        Render_BuildQuadStrip(strip, 2, 32, 32);
        model->mesh = strip;
        model->kind = six;
        model->scrollU = 64;
      }
      {
        u8 *cells = gMapCellBuffer;
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
            Render_ResetTransformState();
            x = -0x180000;
            if (t >= 0) {
              x = (t << 16) + -0x180000;
            }
            if (i == 0) {
              if (t > 31) {
                x += (t * 7 - 224) << 16;
              }
            } else if (t > 15) {
              x += (t * 7 - 112) << 16;
            }
            Func_08015160(x, -0x100000, 0);
            Func_080151e4(45536, 0x10000, 0x8000);
            if (i == 0) {
              Func_080150e4(-0xbe8);
            } else {
              Func_080150e4(-0x400);
            }
            Render_TransformVertices(cells, verts, 66);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(verts);
      req = Data_08196dd8;
      place[3] = 0;
      place[1] = 0xff0000;
      scrollX += 0x180000;
      if (scrollX > 0x1fffff) {
        phase = (phase + 7) % 8;
        scrollX += -0x200000;
      }
      gBgScroll[1].x += 0xfff8;
      for (j = 0; j != 2; j++) {
        s32 row = j * 9;
        void **last = &work->objects[row + 8];

        for (i = 0; i != 9; i++) {
          s32 mask = ~0x3ff;
          s32 k_ = 64;
          s32 tile = i + k_ + phase;
          place[0] = (i << 21) + scrollX;
          place[2] = 0x790000 + (j << 21);
          /* FAKEMATCH(dead): a store both arms below overwrite, behind a test
           * that is never true. i == 300 builds 300 in a reload register, the
           * one pick after the place stores that leaves reload's round-robin
           * on r0, so the else arm's reloads take r1, r2 and r0; the reads of
           * row, j and last raise their reference counts, so global.c ranks
           * last, j, row and then the Apply pointer and they take r6, r7, r4
           * and the stack; as no test, 197 of 2579 instructions differ in
           * tla-en. */
          if (i == 300 && row == j && last == 0)
            u = 0;
          if (i == 8) {
            obj = *last;
            {
              s32 t_ = phase + 71;
              s32 q_ = t_;
              if (t_ < 0)
                q_ = phase + 78;
              q_ >>= 3;
              q_ <<= 3;
              u = t_ - q_;
            }
          } else {
            // j * 9 written again: this product, used twice, is the one loop.c
            // keeps as row, so row is set up and stepped before last.
            {
              s32 m = j * 9;
              m += i;
              obj = work->objects[m];
            }
            {
              s32 x_ = phase + i;
              s32 q_ = tile;
              if (tile < 0)
                q_ = x_ + 71;
              q_ >>= 3;
              q_ <<= 3;
              u = tile - q_;
            }
          }
          {
            s32 v = work->particles[row + u].variant & 0x3ff;
            s32 x = obj->attr.all & mask;
            obj->attr.all = x | v;
          }
          if (place[0] > 0xffffff) {
            place[0] += -0x1200000;
          }
          Render_ApplyProjectedPlacementFar(work->objects[j * 9 + i], place,
                                            (s32 *)&req, 0);
        }
      }
      BattleEffect_CountDownBattlerTints();
      WAIT_TRANSFER(work);
      if (work->shakeFrames > 0) {
        {
          s32 y_ = Random16() % 8 + 28;
          gBgScroll[1].y = y_;
        }
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
      }
      work->transferPending = 1;
      wait(1);
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
  transition[4] = 0;
  {
    u16 *background = (u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670);

    // A once-only block: the listing stores 32 after the backdrop address and
    // sets up the call while the store settles.
    do {
      gBgScroll[1].y = 32;
      BattleBackground_LoadFar(1, *background, 24);
    } while (0);
  }
  wait(1);
  gCameraSceneParameters[4] = 120;
  Sound_PlayFar(390);
  for (i = 0; i != 18; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  FillWords_(canvas, 0x4000, 0x3f3f3f3f);
  REG_BG2CNT = 0x784;
  effect->actors[effect->count] = 0xff;
  BattleActor_SpawnObjectsForListFar(effect->actors, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0,
                             0);
  REG_BLDALPHA = 0x1010;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;
    u8 *c;

    if (v > 63) {
      v = 63;
    }
    for (j = 0, c = (u8 *)(i + (s32)work + 1792); j != 120; j++) {
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
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 0, -1, i, 16);
        }
      }
      if (frame == 127) {
        Func_081180e8(134);
      }
      if (frame == 107) {
        FillWords_(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame > 107) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Func_08164b2c(0, 0, 0);
      }
      if (frame == 32) {
        Sound_PlayFar(140);
      }
      if (frame == 48) {
        work->shakeFrames = 90;
        Sound_PlayFar(145);
      }
      if (frame == 72) {
        Sound_PlayFar(208);
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
          Func_08164abc(a, a / 2, b);
        }
        if ((u32)(frame - 80) <= 31) {
          u32 c;
          u16 *dst = (u16 *)&OBJ_PLTT[1];

          i = 0;
          c = blue | green | (frame - 80);
          for (; i != 223; i++) {
            *dst++ = c;
          }
        } else if (frame > 47) {
          ColorBuffer_ScaleFar((void *)OBJ_PLTT, (void *)OBJ_PLTT, 0xf000, 224);
        }
        if (frame > 47) {
          draw[0](canvas, work->sheet, 216 - frame * 2, 0, 120, 120);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        {
          s32 zero = 0;
          texture3.logWidth = 5;
          texture3.logHeight = 2;
          texture3.pixels = work->sheet + 0x3840;
          model->shade = zero;
          model->mesh = Data_08199220;
          model->texture = &texture3;
          model->vertices = verts;
          model->kind = 6;
        }
        if (frame > 31) {
          struct EffectStep *q = &work->particles[63];
          s32 r = frame * 2 - 32;

          if (r > 64) {
            r = 64;
          }
          if (frame == 32) {
            q->x = 0x800000;
            q->y = -0x200000;
          }
          if (frame <= 43) {
            q->x += -0x80000;
            q->y += 0x100000;
            Func_0815b434(work->sheet + 0x38c0, r, 0x10000);
            Func_0818caa8(work->sheet + 0x38c0, HI(q->x), HI(q->y), r);
          }
          Render_ResetTransformState();
          Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(0xa000);
          Func_080151e4(r * 9400, 0x20000, 0x8000);
          Render_TransformVertices(Data_081976e0, verts, 4);
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
      work->transferPending = 1;
      wait(1);
    }
  }
  Palette_LoadBasicObjColors();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
