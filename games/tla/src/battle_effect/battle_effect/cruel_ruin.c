/// @file
/// Cruel Ruin and the light-streak backdrop with its halved canvas and dimming
/// background, and two effects not yet understood.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/battle_effect.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "resource_ids.h"

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
/* VIEW(f13eb70): tag struct BattleBackgroundId as struct
 * BattleBackgroundId__f13eb70; battle_effect.c has another definition; tried
 * with the definition the rest of the object has,
 * BattleEffect_RunShatteringOpening moves: 1 of 1093 instructions in tla-ja. */
union BattleBackgroundId__f13eb70 {
  u16 id;
  s32 word;
};
#define BACKGROUND(battle)                                                     \
  (((union BattleBackgroundId__f13eb70 *)(battle)->unk670)->id)

// Puts colour 2 at (x, y) in the 256x120 8bpp tile sheet (32 tiles wide) in
// the map cell buffer.
#define PLOT(x, y)                                                             \
  (*(Ram_MapCellBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 +            \
     (y) / 8 * 2048) = 2)

// The target's picture shatters, for the scenes.

// A grey circle spreads from (96, 60) over the battle background, 32 pixels
// of radius a frame for four frames, while the picture in the canvas breaks
// into 33 pieces that fly outward and fall. Colours 6 to 13 start white and
// step back to the battle background's own colours from frame 8; colour 1
// brightens from black to white. At frame 51 a flash sheet replaces the
// pieces, and 32 flashes rise from x 32-63, y 80-111, one more every four
// frames. At the end the background and the display are put back.

void BattleEffect_RunShatteringOpening(struct BattleEffectArgument *effect) {
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
  BattleBackground_SetModeFar(1, BACKGROUND(battle), 0);
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

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
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
    x = gDeathPlunge_PieceStartX[i] << 16;
    y = gDeathPlunge_PieceStartY[i] << 16;
    piece->x = x;
    piece->y = y;
    vx = (x - 0x200000) >> 2;
    vy = (y - 0x3c0000) >> 2;
    piece->velocityX = vx;
    piece->velocityY = vy;
  }

  // Copy the picture at 0x06008000 aside, fill it with colour 1, and make
  // colours 6 to 13 white.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
  Iwram_FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
  Iwram_FillWords((void *)0x050000c0, 0x100, 0x7fff7fff);
  Sound_PlayFar(SE_POWER_UP);
  BattleEffect_TintBattler(effect->actors[0], 7, 3, 0, 30);

  for (frame = 0; frame != 128; frame++) {
    if (frame == 2)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 3)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 28)
      BattleEffect_TintBattler(effect->actors[0], -1, 3, -1, 0);
    if (frame == 32)
      Sound_PlayFar(SE_MONSTER_ROAR);
    if (frame == 5)
      Sound_PlayFar(SE_CRASH);
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
      Iwram_FillWords((void *)0x06008000, 0x7800, 0x02020202);

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
      Iwram_CopyWords((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
    }

    // Up to frame 50, draw the pieces; from frame 4 they fall.
    if (frame <= 50) {
      for (i = 0; i != 33; i++) {
        piece = &work->particles[i];
        draw[0](canvas, work->sheet + gDeathPlunge_PiecePictures[i],
                HI(piece->x), HI(piece->y), gDeathPlunge_PieceWidths[i],
                gDeathPlunge_PieceHeights[i]);
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
  BattleEffect_TintBattler(effect->actors[0], -1, 1, -1, 0);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = 32;
  BattleBackground_SetModeFar(2, BACKGROUND(battle), 0);
  wait(1);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
  HeapSlot_Release(SLOT_BATTLE_SHEET);
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}

// Reached from the scenes through the far-call table (table_47.s). Like
// 0813e3d0 it sets up its own heap blocks, stores work->effect and marks the
// work active itself, and reads the actors from the argument. The corner
// sparks' four blitters are loaded through BattleEffect_BuildBlitter directly,
// with all five arguments.

// The rising sparkles live in the map cell buffer.
#define gSparkles ((struct EffectStep *)Ram_MapCellBuffer)

// Light gathering on the user and bursting, for the scenes.

// For the first 15 frames four sparks flare at the user's corners, each
// mirrored to face the middle; 64 motes spiral in from a ring around the
// user, shrinking as they come. At frame 26 the first target flashes with a
// sound, and from frame 28 to 48 a 48x48 burst plays out over seven
// pictures. effect->kind picks the colours (yellow, ice, red or violet). The
// last 6 frames fade out. The 128 sparkles set up in the map cell buffer are
// never drawn.

void BattleEffect_RunElementBurst(struct BattleEffectArgument *effect) {
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
  Sound_PlayFar(SE_SPIN);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
    if (frame == 26) {
      Sound_PlayFar(SE_POWER_UP);
      BattleEffect_TintBattler(effect->actors[0], 7, -1, 0, 20);
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
        BattleEffect_BuildBlitter(SLOT_BLITTER_ALTERNATE, 7, 7,
                                  gSpiderWeb_PictureHeights[i] | CLIP, mode);
        draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        draw[1](canvas, work->sheet + (picture << 10), gDeathPlunge_X[i] + 32,
                ground.y + gDeathPlunge_Y[i] - 32, 32, 32);
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
          draw[0](canvas, sheet + gBattleEffect_FlarePictures[size - 1],
                  x - size, y - size, size * 2, size * 2);
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

/// The Broken Seal's routine.
void BattleEffect_DrawStreaks(void) {
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

      Transform_Reset();
      Transform_ApplyRoll(streak->roll);
      Transform_ApplyPitch(streak->pitch);
      Transform_ApplyYaw(streak->yaw);
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
  work->transferPending = 1;
}

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

// Raises the light-streak backdrop in colour set kind (yellow, crystal,
// ember, violet or dust).

void BattleEffect_StartElementStreaks(s32 kind) {
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
  BattleBackground_SetModeFar(1, BACKGROUND(battle), 0);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  wait(1);
  REG_BG2CNT = 0x784;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7341, 0x20000);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  view->secondMode = 0;
  wait(1);
  BattleEffect_SetupCanvasWindows();
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
  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
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
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(id), 128);

  // Fade the background over 24 frames, and draw and halve the streaks
  // every frame.
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(BattleEffect_DimBackgroundStep, 0x480);
  work->halvePending = 0;
  Task_Add(BattleEffect_TransferHalvedCanvas, 0xc80);
  work->frame = 0;
  Task_Add(BattleEffect_DrawStreaks, 0x480);
}

/// Ends the light-streak backdrop: removes its three callbacks, clears the
/// canvas in VRAM and frees heap blocks 96 and 92.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(BattleEffect_DrawStreaks);
  Task_Remove(BattleEffect_TransferHalvedCanvas);
  Task_Remove(BattleEffect_DimBackgroundStep);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
}

#undef HI

struct Model;

void Render_DrawModelList__battle_effect(struct Model *model);

#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_Data0B4, ResourceId_Data0C2, ResourceId_Data0C9;
extern char ResourceId_Data0CA, ResourceId_Data104, ResourceId_Data134;
extern char ResourceId_CornerRuneASheet, ResourceId_ForestCanopyTiles;
extern char ResourceId_SunbeamCloudsBackdrop, ResourceId_VioletSkyBackdrop;
extern u8 gMapCellBuffer[];

struct SpriteObject *EffectSprite_CreateWithTiles__battle_effect(
    s32 width, s32 height, u32 shape,
    u32 attributes) __asm__("EffectSprite_CreateWithTiles");
#define EffectSprite_CreateWithTiles EffectSprite_CreateWithTiles__battle_effect
/* VIEW(battle_effect): routine EffectSprite_Acquire as struct RuinObject
 * *EffectSprite_Acquire(s32 width, s32 height, u32 shape, s32 mode);
 * battle_effect/ability_effects has struct EffectSprite__f15b3b0
 * *EffectSprite_Acquire(s32 kind, s32 value) {; tried as the definition's
 * struct EffectSprite__f15b3b0 *EffectSprite_Acquire(s32 kind, s32 value),
 * which the object already declares, it does not compile in tla-ja (make
 * modern): games/tla/include/module/battle_effect/battle_effect.h:210:64: note:
 * expected 'struct BattleEffectArgument *' but argument is of type 'u8 *'. */
struct RuinObject *
EffectSprite_Acquire__battle_effect(s32 width, s32 height, u32 shape,
                                    s32 mode) __asm__("EffectSprite_Acquire");
#define EffectSprite_Acquire EffectSprite_Acquire__battle_effect

#define SLOT_ALT 188 // unkb8[1]
#define ALT_BLITTER ((DrawRectangle)Ram_HeapSlots->unkb8[1])
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].offset)
// The listing reads the transfer count twice per pass of its wait.
#define WAIT_TRANSFER(work)                                                    \
  while (*(volatile u32 *)&(work)->vblanksSinceTransfer,                       \
         *(volatile u32 *)&(work)->vblanksSinceTransfer <= 1)

// Forwarded so that a constant size reaches the call's register at each call.

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
  actor = BattleObject_GetFar(effect->actor)->object;
  actor[18] = phase;
  Graphics_LoadGreyRamp();
  Sound_PlayFar(SE_SPIN);
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
    BattleEffect_DrawShadedCircles(work->sheet, size, 0x10000);
    Canvas_BlitAtFoot(work->sheet, pos.x / 2, pos.y - 16, size);
    if (frame > 15) {
      actor[3] += 0x100000;
    }
    work->transferPending = 1;
    wait(1);
  }
  actor[18] = 0xab85;
  actor[3] = 0;
  Iwram_FillWords((void *)0x06004000, 0x4000, 0x3f3f3f3f);
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
  Picture_MirrorIntoQuadrants(work->sheet, gData_02014000, 64, 64);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->sheet + 0x2648, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3648, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3688, 0,
                             0);
  for (i = 0; i != 10; i++) {
    s32 size = i * 2 + 2;

    BattleEffect_DrawShadedCirclesMirrored(
        work->sheet + 0x3a48 + Data_08197438[i], size, 0x10000);
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
          Sound_PlayFar(SE_SHIMMER);
        }
        if (frame == 40) {
          Sound_PlayFar(SE_CAST);
        }
        if (frame == 64) {
          Sound_PlayFar(SE_139);
        }
        if (frame == 74) {
          Sound_PlayFar(SE_139);
        }
        if (frame == 84) {
          Sound_PlayFar(SE_139);
        }
        if (frame == 94) {
          Sound_PlayFar(SE_139);
        }
        if (frame == 124) {
          Sound_PlayFar(SE_CLATTER);
        }
        for (i = 0; i != 6; i++) {
          if (frame == 140 + i * 4) {
            Sound_PlayFar(SE_104);
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
          Palette_BrightenSavedBattleBg(fr >> 16, fg >> 16, fb >> 16);
        }
        if (frame == 124) {
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
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
          BattleEffect_DrawShadedCircles(work->sheet, size, scale);
          Canvas_BlitAtFoot(work->sheet, 60, y, size);
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

              Transform_Reset();
              SceneTransform_ApplyPosition3(-0x40000, -0x2c0000, 0);
              SceneTransform_ApplyScale3(sc / 2, sc, sc);
              Transform_ApplyYaw(gSpiderWeb_PictureBiasY[i]);
              Transform_ApplyPitch(Data_081976f0[i]);
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
          Transform_Reset();
          SceneTransform_ApplyPosition3(-0x40000, -0x2c0000, 0);
          SceneTransform_ApplyScale3(sc >> 1, sc, sc);
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
            Transform_Reset();
            SceneTransform_ApplyPosition3(-0x40000, -0x2c0000, 0);
            SceneTransform_ApplyScale3(sc, sc / 3, sc >> 1);
            Transform_ApplyRoll(t);
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
              Transform_Reset();
              SceneTransform_ApplyPosition3(-0x40000, -0x2c0000, 0);
              SceneTransform_ApplyScale3(sc, sc >> 1, sc >> 1);
              Transform_ApplyRoll(t);
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
              Transform_Reset();
              SceneTransform_ApplyPosition3(-0x40000, -0x2c0000, 0);
              SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
              Transform_ApplyRoll(p->z);
              SceneTransform_ApplyScale3(30384, sy, 0x8000);
              Render_TransformVertices(Data_081991c0, verts, 4);
              Render_DrawModelList(model);
            }
            p->variant++;
          }
          if (frame > 163) {
            work->transferMode = 1;
            work->transferValue = 0x01010101;
            Palette_BrightenBgEntries(2, 2, 2);
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
              BattleEffect_DrawShadedCircles(work->sheet, r, 0x10000);
              Canvas_BlitAtFoot(work->sheet, HI(q->x), HI(q->y), r);
            }
            model->kind = 6;
            shade = 0;
            model->shade = shade;
            texture.pixels = work->sheet + 0x3688;
            Transform_Reset();
            SceneTransform_ApplyPosition3(q->x + -0x3f0000, q->y + -0x400000,
                                          0);
            SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
            Transform_ApplyRoll(0xa000);
            SceneTransform_ApplyScale3(r * 9400, 0x20000, 0x8000);
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
    Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    work->transferPending = 1;
    wait(1);
    Iwram_FillWords(canvas, 0x4000, 0);
    REG_BG2CNT = 0x786;
    BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
    Palette_BrightenBattleBg(31, 31, 31);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3648,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data104, work->sheet + 0x36c8,
                               0, 0);
    Graphics_LoadGreyRamp();
    {
      u8 *e = Resource_GetTableEntry((s32)&ResourceId_ForestCanopyTiles);

      Iwram_FillWords((void *)&OBJ_PLTT_COLOR(15, 0), 32, 0x7fff7fff);
      Decompress_TaggedLz(e + 32, work->sheet);
    }
    {
      s32 q;
      for (q = 0; q != 2; q++) {
        u8 *src;
        struct RuinObject *obj;

        i = 0;
        src = work->sheet + q * 0x1000;
        for (; i != 8; i++) {
          obj = EffectSprite_CreateWithTiles(32, 32, 0x80000000, 0xf000);
          work->objects[q * 9 + i] = obj;
          {
            s32 b = ((u8 *)obj)[9];
            u8 x = b & -13;
            x |= 4;
            ((u8 *)obj)[9] = x;
          }
          Iwram_CopyWords((void *)OBJ_TILES(obj), src, 512);
          src += 512;
        }
        obj = EffectSprite_Acquire(32, 32, 0x80000000, 0xf000);
        work->objects[q * 9 + 8] = obj;
        Iwram_CopyWords(obj, work->objects[q * 9], 24);
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
        Palette_BrightenSavedBattleBg(r, g, b);
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
          s32 d = frame - gGatheringLights_Counts[i];

          if (d == 0) {
            if (i == 5) {
              Sound_PlayFar(SE_CRASH);
            } else {
              Sound_PlayFar(SE_CLATTER);
            }
            work->shakeFrames = 8;
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          }
          if ((u32)d <= 15) {
            s32 r = (Trig_Sin(d << 11) * gMirroredWall_Pictures[i] >> 16) + 2;

            if (r > 1) {
              s32 x = gDjinnBeams_HeadPictures[i] + d * 8;
              s32 y = gProcne_Pictures[i];
              s32 half;

              BattleEffect_DrawShadedCircles(work->sheet, r, 0x10000);
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
        s32 start = gTiamat_EmberSpeeds[i] >> 1;

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
          size = gTiamat_EmberSizes[i];

          if (p->y < Data_08197730[i] << 16) {
            p->x -= Data_08197728[i] << 16;
            p->y += Data_0819772c[i] << 16;
          } else {
            p->variant++;
          }
          p->x += 0x80000;
          Transform_Reset();
          px = p->x + -0x400000;
          py = p->y + -0x400000;
          SceneTransform_ApplyPosition3(px, py, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          angle = Data_08197734[i];
          Transform_ApplyRoll(angle);
          scale = size * 9400;
          SceneTransform_ApplyScale3(scale, 0x20000, 0x8000);
          Render_TransformVertices(Data_081976e0, verts, 4);
          BattleEffect_DrawShadedCircles(work->sheet, size, 0x10000);
          Canvas_BlitAtFoot(work->sheet, HI(p->x) - 1, HI(p->y), size);
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
            Transform_Reset();
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
            SceneTransform_ApplyPosition3(x, -0x100000, 0);
            SceneTransform_ApplyScale3(45536, 0x10000, 0x8000);
            if (i == 0) {
              Transform_ApplyRoll(-0xbe8);
            } else {
              Transform_ApplyRoll(-0x400);
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
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  for (i = 0; i != 18; i++) {
    Sprite_DestroyFar(work->objects[i]);
  }
  Palette_LoadBasicSpriteColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
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
          BattleEffect_TintBattler(effect->actors[i], 0, -1, i, 16);
        }
      }
      if (frame == 127) {
        BattlePlayback_StartFar(134);
      }
      if (frame == 107) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      }
      if (frame > 107) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
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
            BattleEffect_DrawShadedCircles(work->sheet + 0x38c0, r, 0x10000);
            Canvas_BlitAtFoot(work->sheet + 0x38c0, HI(q->x), HI(q->y), r);
          }
          Transform_Reset();
          SceneTransform_ApplyPosition3(q->x + -0x3f0000, q->y + -0x400000, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          Transform_ApplyRoll(0xa000);
          SceneTransform_ApplyScale3(r * 9400, 0x20000, 0x8000);
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
  Palette_LoadBasicSpriteColors();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
#undef SLOT_BATTLE_EFFECT
#undef SLOT_BATTLE_CANVAS
#undef SLOT_BATTLE_SHEET
#undef SLOT_BLITTER
#undef SLOT_BLITTER_ALTERNATE
#undef BATTLE_EFFECT_WORK_SIZE
#undef BLITTER
#undef CLIP
#undef HI
#undef BACKGROUND
#undef PLOT
#undef gSparkles
#undef SLOT_ALT
#undef ALT_BLITTER
#undef OBJ_TILES
#undef WAIT_TRANSFER
