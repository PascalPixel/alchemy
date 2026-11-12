// ---- games/tla/src/battle_effect/f13ba50.c ----
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "system.h"

/// Puts the battle screen back into the display state the effects start
/// from and end with: no blending, window 0 and window 1 both covering the
/// battle view (lines 16 to 135), and the effect display mode queued.
/// Writes BLDCNT, BLDALPHA, the window registers and WININ and WINOUT,
/// queues DISPCNT for VBlank and waits one frame.
void BattleEffect_ConfigureEffectDisplay(void) {
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}

#include "ram_buffer.h"
#include "bg_scroll.h"

struct ScrollStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
};

void Func_0813baec(void) {
  struct ScrollStepWork *work;

  work = (struct ScrollStepWork *)Ram_HeapSlots->battleFxWork[0];

  if (++work->counter == work->period) {
    gBgScroll[1].x += work->stepX;
    gBgScroll[1].y += work->stepY;
    work->counter = 0;
  }
}

#include "dma.h"

struct AffineStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
  u8 unk77a0[0x30];
  s32 x;
  s32 y;
};

/// A task: every period frames, writes the BG2 reference point to
/// the hardware, then moves it by (stepX, stepY).
void BattleEffect_AdvanceBg2OnInterval(void) {
  struct AffineStepWork *work;

  work = (struct AffineStepWork *)Ram_HeapSlots->battleFxWork[0];

  if (++work->counter == work->period) {
    REG_BG2X = work->x;
    REG_BG2Y = work->y;
    work->x += work->stepX;
    work->y += work->stepY;
    work->counter = 0;
  }
}

/// A task: restarts DMA0's HBlank copy of one WIN0H value per
/// scanline from the map cell buffer.
void BattleEffect_ArmWin0HBlankDma(void) {
  volatile u16 *dma = REG_DMA0;
  u16 control = dma[5];

  dma[5] = control & 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff;          // Stop.
  (void)dma[5];
  Dma_Set(Ram_MapCellBuffer, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "battle_display.h"
#include "task.h"

// The blitter pair's heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define PHASE_RESULTS 133

// Shards and frames for each variant (The Broken Seal's
// gFalling_IceCounts).
extern u8 Data_08197521[];
#define SHARD_COUNT(variant) Data_08197521[(variant) * 2]
#define FRAME_COUNT(variant) Data_08197521[(variant) * 2 + 1]

// The sparkle puff's size and cell for each of its nine pictures
// (gBattleEffect_PuffSizes and gBattleEffect_PuffCells).
extern u8 Data_0819745e[];
extern u16 Data_0819744c[];

extern char ResourceId_IceShardSheet;
extern char ResourceId_SparkleDots;
extern u8 gMapCellBuffer[];

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);

void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);

// Ice, Ice Horn and Ice Missile (effect kind 7, variants 0 to 2).

// Shards of ice fall in a slanting column onto the targets' side. Each one
// flies down at 8 pixels a frame until it reaches the ground line, then
// flashes every target, shakes the screen and shatters: fourteen frames of
// breaking pictures, with a burst of sparkle puffs in the middle of them.
// Window 0 is narrowed line by line through an HBlank DMA table into a
// slanting band, so the shards appear only over the targets.

// Particles 0-31 are the shards and 32-63 the puffs. A particle's variant is
// its age: -1 is a shard still falling or a free puff.

void BattleEffect_RunIce(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *graphics;
  void *draw[2];
  struct EffectStep *shard;
  struct EffectStep *puff;
  u16 *line;
  s32 i;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up the canvas, the shard pictures in the sheet and the puffs.
  BattleEffect_StartCanvasLayer(0x2001);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceShardSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, graphics, 0, 0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = 0x3f44;
  REG_WININ = 0x3337;

  // Line the shards up above the screen, each 8 pixels higher and further
  // out than the one before, with a little scatter.
  {
    s32 y;
    s32 slant;

    for (i = 0, y = -128, slant = -16; i != 32; y -= 64, i++, slant -= 8) {
      s32 x;

      shard = &work->particles[i];
      x = (Random16() & 63) + (Random16() & 7) + 24;
      if (effect->side == 1)
        x = x + slant + 24;
      else
        x = x - slant + 80;
      shard->x = x << 3;
      shard->variant = -1;
      shard->y = y;
    }
  }
  for (i = 0; i != 32; i++)
    work->particles[32 + i].variant = -1;

  // Falling shards draw at their maximum; the puffs add their light. Both
  // mirror for the other side.
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y));
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(
        SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
    BattleEffect_LoadBlitter(
        SLOT_BLITTER_ALTERNATE,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
  }
  draw[0] = Ram_HeapSlots->unk64[1];
  draw[1] = Ram_HeapSlots->unkb8[1];

  // Window 0's left and right edge for each line: a band slanting with the
  // shards' fall, then the targets' full width, then nothing.
  if (effect->side == 0) {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = (0xf0 - i) | ((0x70 - i) << 8);
      else if (i <= 135)
        line[i] = 0x888;
      else
        line[i] = 0x100;
    }
  } else {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 95)
        line[i] = (i - 8 + 160) | ((i + 0x18) << 8);
      else if (i <= 135)
        line[i] = 0x78f8;
      else
        line[i] = 0x100;
    }
  }

  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_FRAME);
  work->transferMode = 2;
  if (effect->variant == 1)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != FRAME_COUNT(effect->variant); frame++) {
    if (frame == FRAME_COUNT(effect->variant) - 16)
      Func_081180e8(PHASE_RESULTS);

    for (i = 0; i != SHARD_COUNT(effect->variant); i++) {
      shard = &work->particles[i];

      // A falling shard moves 8 pixels down and 8 across; at the ground
      // line it lands.
      if (shard->variant == -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        ((DrawRectangle)draw[effect->variant == 2 ? 1 : 0])(canvas, work->sheet,
                                                            x, y, 32, 32);
        if (shard->y <= 0x27f) {
          if (effect->side == 0)
            shard->x -= 64;
          else
            shard->x += 64;
          shard->y += 64;
        } else {
          if ((i & 3) == 0)
            Sound_PlayFar(115);
          work->shakeFrames = 2;
          shard->variant = 0;
          for (k = 0; k != effect->count; k++)
            Func_0814cd48(effect->actors[k], 9, 5, k, 8);
        }
      }

      // A landed shard breaks over fourteen frames, three to a picture,
      // and throws a sparkle puff on each of frames 9 to 11.
      if (shard->variant != -1) {
        s32 x = shard->x / 8;
        s32 y = shard->y / 8;

        if (shard->variant >= 1 && shard->variant <= 14) {
          ((DrawRectangle)draw[effect->variant == 2 ? 1 : 0])(
              canvas, work->sheet + 0x400 + ((shard->variant / 3) << 10), x, y,
              32, 32);
        }
        if (shard->variant >= 9 && shard->variant <= 11) {
          for (k = 0; k != 32; k++) {
            puff = &work->particles[32 + k];
            if (puff->variant == -1) {
              puff->variant = 18;
              puff->x = (((Random16() & 31) + shard->x / 8) << 3) + 8;
              puff->y = ((Random16() & 15) + shard->y / 8 - 15) << 3;
              break;
            }
          }
        }
        if (shard->variant <= 14)
          shard->variant++;
      }
    }

    // Each puff counts down from 18 through its nine pictures, two frames
    // apiece, centred on its point.
    for (i = 0; i != 32; i++) {
      puff = &work->particles[32 + i];
      if (puff->variant != -1) {
        if (puff->variant <= 17) {
          s32 image = puff->variant / 2;
          s32 x = puff->x / 8 - (Data_0819745e[image] >> 1);
          s32 y = puff->y / 8 - (Data_0819745e[image] >> 1);

          ((DrawRectangle)draw[effect->variant == 2 ? 1 : 0])(
              canvas, graphics + Data_0819744c[image], x, y,
              Data_0819745e[image], Data_0819745e[image]);
        }
        if (puff->variant > -1)
          puff->variant--;
      }
    }

    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(4, 4);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}

#undef FRAME_COUNT

// The map cell buffer holds WIN0's line table and then the splashes.
struct DropSplashBuffer {
  u16 lines[160];
  struct EffectStep splashes[512];
};

#define gDropSplashes ((struct DropSplashBuffer *)Ram_MapCellBuffer)

// Four bytes for each variant (The Broken Seal's gFalling_Counts).
extern u8 Data_08197527[];
#define DROP_COUNT(variant) Data_08197527[(variant) * 4]
#define SPLASH_COUNT(variant) Data_08197527[(variant) * 4 + 1]
#define DROP_SPACING(variant) Data_08197527[(variant) * 4 + 2]
#define FRAME_COUNT(variant) Data_08197527[(variant) * 4 + 3]

// The splash's cell for each size from 1 to 6 (gBattleEffect_FlareCells).
extern u16 Data_08197424[];

extern char ResourceId_BlueBeamSheet;
extern char ResourceId_ParticleSpritesD;

void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// Douse, Drench and Deluge (effect kind 9, variants 0 to 2).

// Drops of water fall in a slanting column onto the targets' side, 8 pixels
// down and 4 across a frame. At the ground line each one flashes every
// target and bursts: its splash pictures play for eight frames while a spray
// of droplets is thrown up and falls back under gravity. Window 0 is narrowed
// line by line through an HBlank DMA table into a slanting band over the
// targets. Deluge throws its droplets higher.

// Particles hold the drops; the splash droplets live in the map cell buffer
// after the line table. A variant of -1 is a drop still falling or a free
// droplet.

void BattleEffect_RunDouse(struct BattleEffectArgument *effect) {
  void **heapCache;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct EffectStep *drop;
  struct EffectStep *splash;
  u16 *line;
  s32 i;
  s32 k;

  heapCache = (void **)Ram_HeapSlots;
  work = heapCache[23];
  canvas = heapCache[24];

  // Set up the canvas, the drop pictures and the droplets.
  BattleEffect_StartCanvasLayer(0x2001);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                             work->sheet + 0x604, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, work->sheet, 0,
                             0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BLDCNT = 0x3f44;
  REG_WININ = 0x3337;

  // Drops draw at their maximum; droplets add their light.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y));
  draw[0] = heapCache[26];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y));
  draw[1] = heapCache[47];

  for (i = 0; i != 512; i++)
    gDropSplashes->splashes[i].variant = -1;

  // Line the drops up above the screen along the slant, each the variant's
  // spacing higher than the one before.
  for (i = 0; i != 64; i++) {
    s32 x;
    s32 y;

    drop = &work->particles[i];
    x = Random16() & 63;
    y = -(i * DROP_SPACING(effect->variant) + 16);
    if (effect->side == 1)
      x = x + y / 2 - 48;
    else
      x = x - y / 2 + 72;
    drop->x = x << 3;
    drop->y = y << 3;
    drop->variant = -1;
  }

  // Window 0's left and right edge for each line: a band slanting with the
  // drops' fall, then the targets' width, then nothing.
  if (effect->side == 0) {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = ((0x34 - i / 2) << 8) | (0xb4 - i / 2);
      else if (i <= 135)
        line[i] = 0x80;
      else
        line[i] = 0x100;
    }
  } else {
    line = gMapCellBuffer;
    for (i = 0; i != 160; i++) {
      if (i >= 8 && i <= 103)
        line[i] = ((i / 2 + 60) << 8) | (i / 2 + 188);
      else if (i <= 135)
        line[i] = 0x70f0;
      else
        line[i] = 0x100;
    }
  }

  // Douse copies the canvas plainly; Drench and Deluge blend it in.
  Task_Add(BattleEffect_ArmWin0HBlankDma, TASK_PASS_FRAME);
  if (effect->variant == 0) {
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (effect->variant == 1) {
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != FRAME_COUNT(effect->variant); frame++) {
    if (frame == FRAME_COUNT(effect->variant) - 64)
      Func_081180e8(PHASE_RESULTS);

    for (i = 0; i != DROP_COUNT(effect->variant); i++) {
      s32 x;
      s32 y;

      drop = &work->particles[i];
      x = drop->x / 8;
      y = drop->y / 8;
      if (drop->variant == -1) {
        // Falling: draw the drop and move it on, or land it.
        draw[0](canvas, work->sheet + 0x604, x, y, 24, 24);
        if (drop->y <= 0x27f) {
          if (effect->side == 0)
            drop->x -= 32;
          else
            drop->x += 32;
          drop->y += 64;
        } else {
          // Landed: throw this drop's droplets up from its middle in 16.16
          // fixed point, sound every fourth landing and flash the targets.
          drop->variant = 0;
          for (k = 0; k != SPLASH_COUNT(effect->variant); k++) {
            splash =
                &gDropSplashes->splashes[i * SPLASH_COUNT(effect->variant) + k];
            splash->x = (x + 12) << 16;
            splash->y = y << 16;
            splash->velocityX = ((Random16() & 255) - 128) << 9;
            if (effect->variant == 2)
              splash->velocityY = ((Random16() & 0x1ff) - 0x180) << 10;
            else
              splash->velocityY = ((Random16() & 255) - 255) << 10;
            splash->variant = (Random16() & 15) + 16;
          }
          if ((i & 3) == 0)
            Sound_PlayFar(132);
          for (k = 0; k != effect->count; k++)
            Func_0814cd48(effect->actors[k], 7, 5, k, 2);
        }
      } else {
        // Landed: four frames of the small splash, four of the large one.
        if (drop->variant >= 0 && drop->variant <= 3)
          draw[0](canvas, work->sheet + 0x844, x, y, 24, 24);
        else if (drop->variant <= 7)
          draw[0](canvas, work->sheet + 0xa84, x - 9, y - 9, 42, 42);
        if (drop->variant <= 14)
          drop->variant++;
      }
    }

    // Each droplet shrinks as its life runs out, centred on its point.
    for (i = 0; i != 512; i++) {
      splash = &gDropSplashes->splashes[i];
      if (splash->variant != -1) {
        s32 size = splash->variant + 1;

        if (size > 6)
          size = 6;
        draw[1](canvas, work->sheet + Data_08197424[size - 1],
                ((s16 *)&splash->x)[1] - size, ((s16 *)&splash->y)[1] - size,
                size * 2, size * 2);
        BattleEffectKernels_IntegrateVector2(splash, 60, 0x2000);
        splash->variant--;
      }
    }

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ArmWin0HBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
  BattleEffect_ConfigureEffectDisplay();
}

struct BattleEffectArgument;

void Func_0813c5f4(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Sonic Wave (kind 300).
void BattleEffect_RunSonicWave(void *object) { Func_0813c5f4(object, 0); }

/// Abilities: Shriek, Harpy (kind 301).
void BattleEffect_RunShriek(void *object) { Func_0813c5f4(object, 1); }

/// Abilities: Banshee Howl (kind 302).
void BattleEffect_RunBansheeHowl(void *object) { Func_0813c5f4(object, 2); }

/// Abilities: War Cry (kind 304).
void BattleEffect_RunWarCry(void *object) { Func_0813c5f4(object, 4); }

/// Abilities: Wicked Howl (kind 305).
void BattleEffect_RunWickedHowl(void *object) { Func_0813c5f4(object, 3); }

/// The streak volley in lime (mode 1). UNUSED: no effect table entry
/// points here.
void BattleEffect_RunLimeStreaks(void *object) { Func_0813c5f4(object, 1); }

#include "motobj.h"

#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "transform.h"
#include "resource.h"

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// Four signed bytes for each mode (The Broken Seal's gStreaks_Styles).
extern s8 Data_08197533[];
#define RECORD(mode, field) Data_08197533[(mode) * 4 + (field)]
#define RECORD_ANIMATE 0 // cycle the streak through its three pictures
#define RECORD_HIT 1     // sound the hit and flash the target
#define RECORD_MOTION 2  // the target's recoil motion, or -1
#define RECORD_SHAKE 3   // tremble the target after the hit

// The struck target's sideways tremble, in pixels (gStreaks_Tremble).
extern s8 Data_08197547[];

#define STREAK_MAX 3 // streaks for each target

extern char ResourceId_FiveModeImage;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_VioletPaletteD;
extern char ResourceId_RedPaletteB;
extern char ResourceId_CyanPalette;

void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// Sonic Wave (mode 0), Shriek (1), Banshee Howl (2), Wicked Howl (3) and War
// Cry (4): effect kinds 300 to 305.

// Three streaks rise 20 pixels above the user and fly at each target in turn,
// 6 frames apart, reaching it in 24 frames; each target starts 32 frames
// after the one before. A streak is a 20x40 picture drawn twice, its upper
// half mirrored, and the canvas wobbles line by line on a sine wave while the
// transfer halves the old picture each frame into a trail. The mode chooses
// the colour, whether the streak animates, the hit's sound and flash, a
// recoil, and a tremble of the target.

void Func_0813c5f4(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  s32 resource;
  s32 frame;
  s32 j;
  s32 streaks = STREAK_MAX;
  s32 lift = 20 << 16;

  heap = Ram_HeapSlots;
  /* FAKEMATCH(barrier): an empty asm through heap hides that it holds the
   * heap slots' fixed address, so later reads go through its register, not
   * the address built again; without it, 16 of 580 instructions differ in
   * tla-en. */
  __asm__("" : "=r"(heap) : "0"(heap));
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  camera = heap->cameraWork;
  BattleEffect_StartCanvasLayer(1);

  // The upper half draws mirrored top to bottom; the far side mirrors
  // left to right as well.
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_Y));
    drawUpper = heap->unk64[1];
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
    drawLower = heap->unkb8[1];
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
    drawUpper = heap->unk64[1];
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    drawLower = heap->unkb8[1];
  }

  // The streak pictures, and the mode's colours.
  Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work->sheet, 0, 0);
  switch (mode) {
  case 0:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_VioletPaletteD;
    break;
  case 3:
    resource = (s32)&ResourceId_RedPaletteB;
    break;
  case 4:
  default:
    resource = (s32)&ResourceId_CyanPalette;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;   // each transfer halves the old picture,
  work->transferValue = 50; // leaving a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  source = GetBattleObjectSlotFar(effect->actor)->object;
  {
    s32 total = streaks * effect->count * 6 + 48;

    // Each streak flies from 20 pixels above the user to 20 pixels above
    // its target in 24 frames.
    for (k = 0; k != effect->count; k++) {
      target = GetBattleObjectSlotFar(effect->actors[k])->object;
      for (j = 0; j != streaks; j++) {
        struct EffectStep *streak = &work->particles[k * streaks + j];

        streak->x = source->x;
        streak->y = source->y + lift;
        streak->z = source->z;
        streak->velocityX = (target->x - streak->x) / 24;
        streak->velocityY = (target->y + lift - streak->y) / 24;
        streak->velocityZ = (target->z - streak->z) / 24;
        streak->variant = 0; // frames since it set out
      }
    }

    for (frame = 0; frame != total; frame++) {
      s32 *row = work->bg2xLines;

      // Wobble BG2's 160 lines on a sine wave, and fade the last 16
      // frames out.
      for (k = 0; k != 160; k++)
        *row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
      if (frame > total - 16)
        REG_BLDALPHA = (total - frame) | 0x1000;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      for (k = 0; k != effect->count; k++) {
        // Each target's streaks set out with the mode's cry.
        if (frame == k * 32) {
          if (mode == 1)
            Sound_PlayFar(192);
          if (mode == 2)
            Sound_PlayFar(189);
          if (mode == 3)
            Sound_PlayFar(148);
          if (mode == 4)
            Sound_PlayFar(138);
        }
        if (frame >= k * 32) {
          for (j = 0; j != streaks; j++) {
            // lift is reused for the streak's start frame; a local of its own
            // differs.
            lift = k * 32 + j * 6;
            if (frame >= lift) {
              struct EffectStep *streak = &work->particles[k * streaks + j];
              s32 step;

              // The picture changes every 8 frames, stopping at the sixth.
              EffectPosition_ProjectPoint((s32 *)streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (RECORD(mode, RECORD_ANIMATE)) {
                u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              } else {
                u8 *cell = &work->sheet[step * 800 + 2 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              }
              BattleEffectKernels_IntegrateVector3(streak, 64, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the hit, tremble the target sideways for
          // 32 frames.
          if (RECORD(mode, RECORD_SHAKE) && frame >= k * 32 + 30 &&
              frame < k * 32 + 62) {
            struct MotionObject *struck =
                GetBattleObjectSlotFar(effect->actors[k])->object;

            struck->x += Data_08197547[(frame - k * 32 - 30) % 8] << 16;
            if (struck->x > 0)
              struck->x += 0x8000;
            else
              struck->x -= 0x8000;
            Func_0814cd48(effect->actors[k], -1, 5, -1, 0);
          }

          // The hit: a sound and two flashes, 16 frames apart. The first
          // hit lets the battle show its results.
          if (RECORD(mode, RECORD_HIT)) {
            if (frame == k * 32 + 24) {
              Sound_PlayFar(133);
              if (k == 0)
                Func_081180e8(-1);
              Func_0814cd48(effect->actors[k], 7, 5, k, 8);
            }
            if (frame == k * 32 + 40)
              Func_0814cd48(effect->actors[k], 7, 5, k, 8);
          }
          if (RECORD(mode, RECORD_MOTION) != -1 && frame == k * 32 + 24) {
            work->shakeFrames = 4;
            Func_08118088(effect->actors[k], RECORD(mode, RECORD_MOTION));
          }
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0813cb34(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Sleep Star (kind 349).
void BattleEffect_RunSleepStar(void *object) { Func_0813cb34(object, 1); }

/// Abilities: Soothing Star (kind 344).
void BattleEffect_RunSoothingStar(void *object) { Func_0813cb34(object, 0); }

/// Abilities: Slaver (kind 395).
void BattleEffect_RunSlaver(void *object) { Func_0813cb34(object, 2); }

typedef DrawRectangle BlitterPair[2];

enum { MODE_SOOTHING_STAR, MODE_SLEEP_STAR, MODE_SLAVER };

// The motes live in the map cell buffer.
#define EFFECT_STEPS ((struct EffectStep *)Ram_MapCellBuffer)

// The mote's cell for each size from 1 to 6 (The Broken Seal's
// gBattleEffect_MoteCells).
extern u16 gData_08197410[];

extern char ResourceId_Data134; // The Broken Seal's ParticleSpritesA
extern char ResourceId_OrangePaletteA;
extern char ResourceId_EarthWallSheet;
extern char ResourceId_GreenPalette;

void SummonEffect_RunCanvasMode1(void *effect);

// The IWRAM routines are called through one inline each, as The Broken Seal's
// iwram_call.h layer calls them: called directly, CSE shares the size
// between the two clears.
static inline s32 ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_ClearWords, 16 of 477 instructions of Func_0813cb34 differ in
   * tla-en. */
  return Iwram_ClearWords(destination, size);
}

// Soothing Star (mode 0, effect kind 344), Sleep Star (mode 1, kind 349) and
// Slaver (mode 2, kind 395).

// 256 motes burst from 5 pixels above the user and spread in every
// direction; the first 128 are drawn, a quarter of them starting every 8
// frames, each swaying on a sine wave and sized by its depth. Soothing Star's
// burst is half as wide and sounds as it touches each target; Sleep Star's
// drifts away from the user's side. From frame 48 the targets flash one
// after another, 8 frames apart, and the first flash lets the battle show its
// results. Slaver instead throws green drops up in a half-circle fan, turned
// by the user's side, with a sound at the start; they fall back and bounce
// on the ground for 96 frames, and then Impair's canvas effect runs on the
// targets.

void Func_0813cb34(struct BattleEffectArgument *effect, s32 mode) {
  s32 point[3];
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *sheet;
  s32 frames;
  struct MotionObject *source;
  struct EffectStep *mote;
  s32 resource;
  s32 frame;
  s32 i;
  s32 group = 32; // motes that start together

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  if (mode == MODE_SOOTHING_STAR)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);

  // Motes draw at their maximum; the second blitter, mirrored top to
  // bottom, is loaded but not used.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == MODE_SOOTHING_STAR)
    resource = (s32)&ResourceId_OrangePaletteA;
  else if (mode == MODE_SLEEP_STAR)
    resource = (s32)&ResourceId_EarthWallSheet;
  else
    resource = (s32)&ResourceId_GreenPalette;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Launch every mote from the user in 16.16 fixed point, with a speed and
  // a direction of its own. Slaver throws them up, faster, in a half-circle
  // fan turned by the user's side, from up to 16 pixels higher.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  for (i = 0; i != 256; i++) {
    s32 speed;
    s32 angle;

    mote = &EFFECT_STEPS[i];
    if (mode == MODE_SLAVER) {
      if (effect->side == 0)
        angle = (Random16() & 0x7fff) + 0x8000;
      else
        angle = Random16() & 0x7fff;
      speed = (Random16() & 0x3ff) + 0x800;
      mote->y = source->y + (Random16() << 4) + 0x50000;
    } else {
      angle = Random16();
      speed = (Random16() & 0x3ff) + 32;
      mote->y = source->y + 0x50000;
    }
    mote->x = source->x;
    mote->z = source->z;
    mote->velocityX = Trig_Sin(angle) * speed >> 8;
    if (mode == MODE_SLAVER) {
      mote->velocityY = ((Random16() & 255) + 128) << 10;
      mote->velocityZ = -(Trig_Cos(angle) * speed) >> 9;
    } else {
      mote->velocityY = ((Random16() & 255) - 32) << 9;
      mote->velocityZ = -(Trig_Cos(angle) * speed << 1) >> 8;
    }
    mote->variant = (Random16() & 31) + 48;
    if (mode == MODE_SOOTHING_STAR) {
      mote->velocityX /= 2;
      mote->velocityZ /= 2;
    }
  }

  frames = 96;
  if (mode != MODE_SLAVER)
    frames = 128;
  for (frame = 0; frame != frames; frame++) {
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (mode == MODE_SLAVER && frame == 0)
      Sound_PlayFar(103);

    for (i = 0; i != 128; i++) {
      mote = &EFFECT_STEPS[i];
      if (frame >= i / group * 8 && mote->variant >= 0) {
        s32 size;

        // Sway the mote, project it and size it by its depth: 6 pixels
        // near, 1 far.
        point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
        point[1] = mote->y;
        point[2] = mote->z;
        EffectPosition_ProjectPoint(point, &position);
        position.x >>= 1;
        if (position.depth <= 313)
          position.depth = 314;
        if (position.depth > 634)
          position.depth = 634;
        size = 6 - (position.depth - 314) / 64;
        draw[0](canvas, sheet + gData_08197410[size - 1], position.x - size / 2,
                position.y - size, size, size * 2);

        // Slaver's drops fall and bounce off the ground; the others rise
        // slowly. Sleep Star's drift away from the user's side.
        if (mode == MODE_SLAVER) {
          BattleEffectKernels_IntegrateVector3(mote, 62, -0x2000);
          if (mote->y <= 0x4ffff)
            mote->velocityY = -mote->velocityY;
        } else {
          BattleEffectKernels_IntegrateVector3(mote, 62, 0x400);
        }
        if (mode == MODE_SLEEP_STAR) {
          if (source->x < 0)
            mote->velocityX += 0x2000;
          else
            mote->velocityX -= 0x2000;
        }
        mote->variant--;
      }
    }

    // From frame 48, flash the targets one after another, 8 frames apart.
    if (mode == MODE_SLEEP_STAR) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 8 + 48) {
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        }
      }
    } else if (mode == MODE_SOOTHING_STAR) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i * 8 + 48) {
          Sound_PlayFar(126);
          Func_081180e8(-1);
          Func_0814cd48(effect->actors[i], 7, -1, i, 8);
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  if (mode == MODE_SLAVER) {
    // Clear the canvas and run Impair's effect on the targets.
    ClearWords((void *)0x06004000, 0x3c00);
    ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode1(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}

void Func_0813d098(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Numbing Sting, Paralytail (kind 313).
void BattleEffect_RunNumbingSting(void *object) { Func_0813d098(object, 6); }

/// Abilities: Cannibal Fang (kind 316).
void BattleEffect_RunCannibalFang(void *object) { Func_0813d098(object, 3); }

/// Abilities: Poisonous Bite (kind 318); Poison Fang (kind 323).
void BattleEffect_RunPoisonousBite(void *object) { Func_0813d098(object, 1); }

/// Abilities: Rabid Fang (kind 336).
void BattleEffect_RunRabidFang(void *object) { Func_0813d098(object, 10); }

/// Abilities: Electric Bite (kind 324).
void BattleEffect_RunElectricBite(void *object) { Func_0813d098(object, 5); }

/// Abilities: Poison Tail (kind 325); Poison Sting (kind 440).
void BattleEffect_RunPoisonTail(void *object) { Func_0813d098(object, 9); }

/// Abilities: Vampiric Fang (kind 327).
void BattleEffect_RunVampiricFang(void *object) { Func_0813d098(object, 4); }

/// Abilities: Double Fang (kind 332).
void BattleEffect_RunDoubleFang(void *object) { Func_0813d098(object, 0); }

/// Abilities: Mortal Blow (kind 333).
void BattleEffect_RunMortalBlow(void *object) { Func_0813d098(object, 8); }

/// Abilities: Twin Beaks, Gryphon (kind 335).
void BattleEffect_RunTwinBeaks(void *object) { Func_0813d098(object, 7); }

/// Abilities: Acid Bite (kind 337).
void BattleEffect_RunAcidBite(void *object) { Func_0813d098(object, 2); }

/// Abilities: Drain Fang (kind 386).
void BattleEffect_RunDrainFang(void *object) { Func_0813d098(object, 11); }

/// Abilities: Angle Spike (kind 399).
void BattleEffect_RunAngleSpike(void *object) { Func_0813d098(object, 16); }

/// Abilities: Heat Stun (kind 458).
void BattleEffect_RunHeatStun(void *object) { Func_0813d098(object, 14); }

/// Abilities: Maneater (kind 421).
void BattleEffect_RunManeater(void *object) { Func_0813d098(object, 15); }

/// Effect kind 438: the target strike in style 17.
void BattleEffect_RunTargetStrikeMode17(void *object) {
  Func_0813d098(object, 17);
}

/// Effect kind 439: the target strike in style 18.
void BattleEffect_RunTargetStrikeMode18(void *object) {
  Func_0813d098(object, 18);
}

/// Abilities: Kill Sting (kind 441).
void BattleEffect_RunKillSting(void *object) { Func_0813d098(object, 19); }

/// Effect kind 456: the target strike in style 20.
void BattleEffect_RunTargetStrikeMode20(void *object) {
  Func_0813d098(object, 20);
}

void BattleEffect_SetApproachMotion(s32 x, s32 y, s32 width, s32 height);

/// Abilities: Mad Zephyr (kind 117).
void BattleEffect_RunMadZephyr(u8 *object) {
  BattleEffect_SetApproachMotion(*(s32 *)(object + 8), *(s16 *)(object + 36), 4,
                                 20);
  wait(2);
  if (*(s32 *)(object + 24) == 0)
    Func_0813d098(object, 12);
  else
    Func_0813d098(object, 13);
}

#undef RECORD

// The body of 21 strike and column effects.

#include "heap_release.h"

struct Model;

#define gBlitter ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define gBlitterAlternate ((DrawRectangle)Ram_HeapSlots->unkb8[1])

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// Seven bytes for each mode (The Broken Seal's gTargetStrike_Styles, grown
// from 12 modes to 21).
extern u8 Data_0819754f[][7];
#define RECORD(mode, field) Data_0819754f[(mode)][(field)]
#define RECORD_KIND 0    // 0: strikes across the target, 1: columns on it
#define RECORD_COUNT 1   // strikes or columns
#define RECORD_FLAGS 2   // the parts drawn (FLAG_*)
#define RECORD_PALETTE 3 // the colours
#define RECORD_RATE 4    // frames for each picture
#define RECORD_BLEND 5   // the columns' blitter mode
#define RECORD_FRAMES 6  // the effect's length

#define FLAG_WISPS 1  // smoke wisps rise from the target
#define FLAG_MOTES 2  // motes leave the target and fly back to the user
#define FLAG_GLINTS 4 // glints flicker around the target
#define FLAG_SPARKS 8 // the hits throw sparks
#define FLAG_HEAVY 16 // the hits shake harder and knock the target back
#define FLAG_FLASH 32 // as heavy, with flash pictures for wisps
#define FLAG_STAR 64  // a star model swells on the target

// The strike pictures (gTargetStrike_Widths, Heights, Offsets, Reach)
// and the column stages (gTargetStrike_ColumnStages).
extern u8 Data_081975e2[];
extern u8 Data_081975e8[];
extern u16 Data_081975ee[];
extern u8 Data_081975fa[];
extern u8 Data_08197600[];

// The glint pictures (gBattleEffect_GlintCellWidths, Heights, Offsets) and
// their flips (gTargetStrike_GlintDrawFlags).
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern u16 gData_08197486[];
extern u8 Data_08197605[];

// The star model and its four projected corners.
extern const u8 gData_08199340[];
extern const u8 gData_08199210[];

// The motes live in the map cell buffer, the sparks 0x3800 bytes on.
#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)
#define gSparks ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))

extern char ResourceId_TornadoSheet;
extern char ResourceId_BeamImage;
extern char ResourceId_TwelveModeImage;

extern char ResourceId_PinkPalette;

extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_StarCrossSheet;
extern char ResourceId_SmokeSheet;

// The star's texture, 64x64 pixels.
struct StarTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

// One model for the IWRAM renderer.
struct ModelDraw {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct StarTexture *texture;
  s32 unk14;
  s32 tail;
};

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter

void EffectPosition_ProjectBattler(s32 actor, struct EffectPosition *out);

struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080151e4(s32 x, s32 y, s32 z);
void SceneTransform_ApplyPitch(s32 pitch);
void SceneTransform_ApplyYaw(s32 yaw);

#define BUILD_BLITTER(slot, width, height, flags, mode)                        \
  do {                                                                         \
    Func_08196404(slot, width, height, flags, mode);                           \
  } while (0)

/// Strikes (kind 0) are six-picture slashes across the first target, one
/// every six pictures' time; columns (kind 1) are a pair of mirrored 18x48
/// pillars that rise and fall on it. Each hit flashes and shakes the target,
/// starts eight wisps of smoke and a burst of sparks, and the battle moves on
/// at the first. Around them the mode's flags add rising wisps, motes that
/// stream back to the user, glints, sparks, a flash or a star.
void Func_0813d098(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition ground;
  struct EffectPosition targetPosition;
  struct EffectPosition alternate;
  struct EffectPosition center;
  struct EffectPosition position;
  BlitterPair draw;
  struct StarTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *sheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *source;
  s32 resource;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  if (mode == 8)
    BattleEffect_StartCanvasLayer(0);
  else
    BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  if (mode == 8) {
    work->fadeFrames = 24;
    work->fadeStep = 0;
  }

  // The sheets: motes, the tornado, the strikes or columns, the mode's
  // colours and the wisps (smoke, flash or star).
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (RECORD(mode, RECORD_KIND) == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_BeamImage, work->sheet + 0xc80,
                               0, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage,
                               work->sheet + 0xc80, 0, 0);
  switch (RECORD(mode, RECORD_PALETTE)) {
  case 0:
    resource = (s32)&ResourceId_BlueBeamSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_PinkPalette;
    break;
  case 3:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 4:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 5:
  default:
    resource = (s32)&ResourceId_LightningBoltSheet;
    break;
  case 6:
    resource = (s32)&ResourceId_OrangePaletteB;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + 0x3200, 1, 0);
  else if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR)
    Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet,
                               work->sheet + 0x3200, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + 0x3200, 1, 0);

  for (k = 0; k != 1; k++) {
    // The wisps start at the target's feet, waiting (-1) until a hit.
    target = GetBattleObjectSlotFar(effect->actors[k])->object;
    EffectPosition_ProjectBattler(effect->actors[k], &ground);
    for (i = 0; i != 64; i++) {
      struct EffectStep *step = &work->particles[i];

      step->x = (ground.x / 2) << 16;
      step->y = 0x500000;
      step->z = 0;
      step->velocityX = ((Random16() & 255) - 128) << 9;
      step->velocityY = ((Random16() & 255) - 128) << 9;
      step->velocityZ = 0;
      step->variant = -1;
    }

    // The motes burst from 20 pixels above the target.
    for (i = 0; i != 128; i++) {
      struct EffectStep *step = &gMotes[k * 128 + i];

      step->x = target->x;
      step->y = 0x140000;
      step->z = target->z;
      step->velocityX = ((Random16() & 255) - 128) << 11;
      step->velocityY = ((Random16() & 255) - 128) << 11;
      step->velocityZ = ((Random16() & 255) - 128) << 11;
      step->variant = -1;
    }

    // The sparks fly away from the hit: heavy hits throw them in a fan
    // away from the user, the others towards the middle of the field.
    for (i = 0; i != 256; i++) {
      gSparks[i].x = target->x;
      gSparks[i].y = 0x140000;
      gSparks[i].z = target->z;
      if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
        s32 angle = Random16();
        s32 speed = Random16() & 255;

        gSparks[i].velocityX = (Random16() & 0x1ff) << 11;
        if (effect->side == 1)
          gSparks[i].velocityX = -gSparks[i].velocityX;
        gSparks[i].velocityY = Trig_Sin(angle) * speed >> 4;
        gSparks[i].velocityZ = Trig_Cos(angle) * speed >> 4;
      } else {
        if (RECORD(mode, RECORD_KIND) == 1) {
          gSparks[i].velocityX = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityY = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityZ = ((Random16() & 255) - 128) << 11;
        } else {
          gSparks[i].velocityX = (Random16() & 255) << 11;
          gSparks[i].velocityY = ((Random16() & 255) - 128) << 11;
          gSparks[i].velocityZ = ((Random16() & 255) - 128) << 11;
        }
        if (gSparks[i].x > 0)
          gSparks[i].velocityX = -gSparks[i].velocityX;
      }
      gSparks[i].variant = -1;
    }
  }

  work->transferMode = 2;
  if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR)
    work->transferValue = 50;
  else
    work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Aim at the middle of the first target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPosition);
  EffectPosition_ProjectBattler(effect->actors[0], &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != RECORD(mode, RECORD_FRAMES); frame++) {
    s32 span = RECORD(mode, RECORD_COUNT) * RECORD(mode, RECORD_RATE);
    s32 length = span * 4;
    s32 glint = 0;

    if (RECORD(mode, RECORD_KIND) == 0) {
      // Strikes: six pictures each, crossing the target from the user's
      // side.
      if (frame < span * 6) {
        s32 picture = frame / RECORD(mode, RECORD_RATE) % 6;

        if (frame % (RECORD(mode, RECORD_RATE) * 6) == 0 &&
            (u32)(mode - 12) <= 1) {
          EffectPosition_ProjectScaledBattler(effect->actors[0],
                                              &targetPosition);
          EffectPosition_ProjectBattler(effect->actors[0], &alternate);
          targetPosition.y += (alternate.y - targetPosition.y) / 2;
          targetPosition.y += (Random16() & 15) - 8;
        }
        if (effect->side == 1) {
          BattleEffect_LoadBlitter(SLOT_BLITTER,
                                   BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
          gBlitter(canvas, work->sheet + Data_081975ee[picture],
                   targetPosition.x / 2 - (Data_081975fa[picture] / 2) -
                       Data_081975e2[picture] + 8,
                   targetPosition.y - (Data_081975e8[picture] / 2),
                   Data_081975e2[picture], Data_081975e8[picture]);
        } else {
          BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
          gBlitter(canvas, work->sheet + Data_081975ee[picture],
                   targetPosition.x / 2 + (Data_081975fa[picture] / 2) - 8,
                   targetPosition.y - (Data_081975e8[picture] / 2),
                   Data_081975e2[picture], Data_081975e8[picture]);
        }
        HeapSlot_Release(SLOT_BLITTER);

        // A star sounds four frames before its hit.
        if (frame % (RECORD(mode, RECORD_RATE) * 6) ==
                RECORD(mode, RECORD_RATE) * 4 - 4 &&
            (RECORD(mode, RECORD_FLAGS) & FLAG_STAR))
          Sound_PlayFar(212);

        // The hit, on the strike's fifth picture.
        if (frame % (RECORD(mode, RECORD_RATE) * 6) ==
            RECORD(mode, RECORD_RATE) * 4) {
          if (mode == 12) {
            if (frame / RECORD(mode, RECORD_RATE) / 6 <= 2)
              Sound_PlayFar(133);
            else
              Func_081180e8(133);
          } else if (mode == 7) {
            if (frame <= 8)
              Sound_PlayFar(133);
            else
              Func_081180e8(133);
          } else if (mode == 13) {
            if (frame / RECORD(mode, RECORD_RATE) / 6 > 6)
              Func_081180e8(134);
            else
              Sound_PlayFar(134);
          } else if ((RECORD(mode, RECORD_FLAGS) & FLAG_STAR) ||
                     (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH)) {
            Func_081180e8(144);
          } else if (mode == 8) {
            Func_081180e8(134);
          } else {
            Func_081180e8(133);
          }

          if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
            work->shakeFrames = 8;
            Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
            Func_08118088(effect->actors[0], 4);
            for (i = 0; i != 32; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
            work->shakeFrames = 8;
            Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
            Func_08118088(effect->actors[0], 4);
            for (i = 0; i != 256; i++)
              gSparks[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
            for (i = 0; i != 32; i++) {
              struct EffectStep *step =
                  &gSparks[frame / (RECORD(mode, RECORD_RATE) * 6) * 32 + i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          if (mode == 16) {
            for (i = 0; i != 4; i++) {
              struct EffectStep *step =
                  &work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) *
                                       16 +
                                   i];

              step->variant = 0;
            }
          } else {
            for (i = 0; i != 8; i++) {
              struct EffectStep *step =
                  &work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) *
                                       16 +
                                   i];

              step->variant = 0;
            }
          }
        }
      }
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Columns: a mirrored pair rising through its stages.
      s32 stage;

      if (frame < length) {
        s32 picture = frame / RECORD(mode, RECORD_RATE);

        while (picture > 4)
          picture -= 4;
        stage = Data_08197600[picture];
      } else {
        stage = 3;
      }
      BUILD_BLITTER(SLOT_BLITTER, 7, 7, CLIP, RECORD(mode, RECORD_BLEND));
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80,
               targetPosition.x / 2 - 18, 56, 18, 48);
      HeapSlot_Release(SLOT_BLITTER);
      Func_08196404(SLOT_BLITTER, 7, 7, CLIP | COMPOSITE_FLIP_X,
                    RECORD(mode, RECORD_BLEND));
      do {
        // Empty: its loop notes end CSE's block after the blitter call.
      } while (0);
      gBlitter(canvas, work->sheet + stage * 864 + 0xc80, targetPosition.x / 2,
               56, 18, 48);
      HeapSlot_Release(SLOT_BLITTER);

      // The hit, on each column's fourth picture.
      if (frame % (RECORD(mode, RECORD_RATE) * 4) ==
          RECORD(mode, RECORD_RATE) * 3) {
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        work->shakeFrames = 4;
        if ((RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) ||
            (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY))
          Func_081180e8(144);
        else if (frame > (RECORD(mode, RECORD_COUNT) * 4 - 4) *
                             RECORD(mode, RECORD_RATE))
          Func_081180e8(133);
        else
          Sound_PlayFar(133);
        if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
          Func_08118088(effect->actors[0], 4);
          for (i = 0; i != 32; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        } else if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY) {
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 12);
          Func_08118088(effect->actors[0], 4);
          for (i = 0; i != 256; i++)
            gSparks[i].variant = (Random16() & 15) + 15;
        }
        for (i = 0; i != 64; i++) {
          struct EffectStep *step =
              &gSparks[frame / (RECORD(mode, RECORD_RATE) * 6) * 64 + i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct EffectStep *step =
              &work->particles[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 +
                               i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          gMotes[frame / (RECORD(mode, RECORD_RATE) * 6) * 16 + i].variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Three glints a frame around the target's middle.
    if ((RECORD(mode, RECORD_FLAGS) & FLAG_GLINTS) && glint) {
      EffectPosition_ProjectScaledBattler(effect->actors[0], &center);
      EffectPosition_ProjectBattler(effect->actors[0], &alternate);
      center.y += (alternate.y - center.y) / 2;
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & 0xffff;
        s32 range = 31;
        s32 radius = Random16() & range;
        s32 x;
        s32 y;

        x = center.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17) -
            (gData_08197492[image] / 2);
        y = center.y - (Trig_Cos(angle) * (radius + 4) >> 17) -
            (gData_08197498[image] / 2);
        Func_08196404(SLOT_BLITTER_ALTERNATE, 7, 7,
                      Data_08197605[Random16() & 3] | CLIP, BLIT_ADDITIVE);
        do {
          // Empty: its loop notes end CSE's block after the blitter call.
        } while (0);
        gBlitterAlternate(canvas, work->sheet + gData_08197486[image], x, y,
                          gData_08197492[image], gData_08197498[image]);
        HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = gBlitter;
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                             BLITTER(BLIT_ADDITIVE, CLIP));
    draw[1] = gBlitterAlternate;
    for (k = 0; k != 1; k++) {
      source = GetBattleObjectSlotFar(effect->actor)->object;

      // Wisps of smoke rise and spread for 24 frames.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_WISPS) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas, work->sheet + picture * 1152 + 0x3200,
                        HI(step->x) - 12, HI(step->y) - 24, 24, 48);
            BattleEffectKernels_IntegrateVector2(step, 62, -0x400);
            step->variant++;
          }
        }
      }

      // Flashes rise twice as fast, 32x64.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_FLASH) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            draw[0](canvas, work->sheet + (step->variant / 4 << 11) + 0x3200,
                    HI(step->x) - 16, HI(step->y) - 32, 32, 64);
            BattleEffectKernels_IntegrateVector2(step, 62, -0x800);
            BattleEffectKernels_IntegrateVector2(step, 62, -0x800);
            step->variant++;
          }
        }
      }

      // Motes burst out, then stream back to the user, who flashes.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_MOTES) {
        s32 size = 3;

        if (mode == 11)
          size = 8;
        if (frame == 55)
          Func_0814cd48(effect->actor, 7, -1, -1, 0);
        if (frame == 90)
          Func_0814cd48(effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &gMotes[i];

          if (step->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            draw[1](canvas, sheet + gData_08197410[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            BattleEffectKernels_IntegrateVector3(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x - step->x) >> 8;
              s32 dy = (source->y - step->y + 0x140000) >> 8;
              s32 dz = (source->z - step->z) >> 8;

              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }

      // Sparks fall away, shrinking.
      if (RECORD(mode, RECORD_FLAGS) & FLAG_SPARKS) {
        for (i = 0; i != 256; i++) {
          struct EffectStep *step = &gSparks[i];
          s32 size = step->variant;

          if (size >= 0) {
            EffectPosition_ProjectPoint((s32 *)step, &position);
            position.x >>= 1;
            BattleEffectKernels_IntegrateVector3(step, 60, 0);
            size >>= 3;
            size++;
            draw[1](canvas, sheet + gData_08197410[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            step->variant--;
          }
        }
      }
    }
    HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
    HeapSlot_Release(SLOT_BLITTER);

    // The star: a textured square, swelling on a sine from frame 8 to 17
    // over the target's middle (the canvas is cleared to colour 0x3f first).
    if (RECORD(mode, RECORD_FLAGS) & FLAG_STAR) {
      void *corners = Heap_AllocEwram(32);
      struct ModelDraw *model = Render_AllocateModelList(1);

      model->unk14 = 0;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + 0x3200;
      model->kind = 6;
      model->model = gData_08199340;
      model->texture = &texture;
      model->corners = corners;
      if (frame == 8)
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      if (frame >= 8 && frame <= 17) {
        s32 scale = Trig_Sin(((frame - 8) << 11) + 0x3000) * 612 >> 8;

        Render_ResetTransformState();
        Func_08015160((targetPosition.x / 2 - 64) << 16,
                      (targetPosition.y - 64) << 16, 0);
        SceneTransform_ApplyPitch(0x4000);
        SceneTransform_ApplyYaw(0x2000);
        Func_080151e4(scale, scale / 2, scale / 2);
        Render_TransformVertices(gData_08199210, corners, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    if (RECORD(mode, RECORD_FLAGS) & FLAG_HEAVY)
      BattleCamera_Shake(8, 8);
    else
      BattleCamera_Shake(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

extern char ResourceId_CrossedDaggersSheet;

// A rain of daggers on the first target (unused).

// Sixteen daggers fall 8 pixels a frame, one every 4 frames, from columns
// 16 to 111, drifting towards the middle (x 64) the further out they start,
// and stick in the ground where they stop. The canvas fades in over 16
// frames and out from frame 104; from frame 24 to 84 the target flashes and
// the screen shakes every 4 frames, with a sound every 8.

void Func_0813e114(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct EffectStep *dagger;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1000;

  // [0] as drawn, [1] mirrored left to right; both skip clear pixels.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                           BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  work->transferMode = 1; // each transfer clears the canvas
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre BG2 on the first target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
  REG_BG2X = (64 - pos.x) << 8;

  // Daggers start in columns 16 to 111; the further from the middle, the
  // faster they drift towards it (in steps of 2 pixels a frame).
  for (i = 0; i != 64; i++) {
    s32 column;

    dagger = &work->particles[i];
    column = Random16() % 96 + 16;
    dagger->x = column;
    dagger->y = (24 - i / 4) << 16;
    if (column <= 43)
      dagger->velocityX = 3;
    else if (column <= 51)
      dagger->velocityX = 2;
    else if (column <= 59)
      dagger->velocityX = 1;
    else if (column <= 67)
      dagger->velocityX = 0;
    else if (column <= 75)
      dagger->velocityX = -1;
    else if (column <= 83)
      dagger->velocityX = -2;
    else
      dagger->velocityX = -3;
    dagger->velocityX <<= 17;
    dagger->velocityY = 8 << 16;
    dagger->x <<= 16;
  }
  Sound_PlayFar(212);

  for (frame = 0; frame != 120; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = frame | 0x1000;
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = (120 - frame) | 0x1000;
      if (frame == 104)
        REG_BLDCNT = 0x3f44;
    }

    // The drift picks the picture; a dagger going left is drawn mirrored.
    // Each one falls from 16 frames after its turn until it lands at 25.
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 cell;

      dagger = &work->particles[i];
      drift = dagger->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      cell = speed >> 17;
      if (frame < i * 4 + 25) {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               (dagger->x >> 16) - 16, (dagger->y >> 16) - 16,
                               32, 32);
        if (frame >= i * 4 + 16) {
          dagger->x += dagger->velocityX;
          dagger->y += dagger->velocityY;
        }
      } else {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               (dagger->x >> 16) - 16, (dagger->y >> 16) - 16,
                               32, 32);
      }
    }

    // From frame 24 to 84, every 4 frames: flash the target for 2 frames
    // and shake the screen; every 8, a sound.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        Sound_PlayFar(133);
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]
#define SLOT_BATTLE_SHEET 100 // unk64[0]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

// The flecks live in the map cell buffer.
#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_IceTileSheet;

extern char ResourceId_YellowPaletteA;
extern char ResourceId_RedPaletteA;
extern char ResourceId_VioletPaletteA;

// A pillar of ice rising at the user's feet with flecks spiralling up
// around it, for the scenes.

// The pillar is two mirrored halves of a 17x64 picture with a 24x43 glow,
// drawn at their maximum: four pillar pictures two frames apart and three
// glow pictures four frames apart. 32 flecks spiral up around it, one more
// each frame, turning 0x400 and rising 5 pixels a frame while the spiral
// widens. effect->kind picks the colours (yellow, ice, red or violet) and
// effect->variant + 1, from 1 to 4, the flecks' size. At frame 16 the first
// target flashes; the last 6 frames fade out.

void Func_0813e3d0(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  sheet = HeapSlot_Alloc(SLOT_BATTLE_SHEET, 0x60e);
  camera = Ram_HeapSlots->cameraWork;
  work->vblanksSinceTransfer = 1;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = 0x100c;
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
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
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // A fleck's x is its angle and z its radius, in 1/16 pixels; fleck i
  // waits i frames before it rises.
  for (i = 0; i != 128; i++) {
    gFlecks[i].y = 0;
    gFlecks[i].x = Random16() & 0xffff;
    gFlecks[i].z = (Random16() & 0x1ff) + i * 2;
    gFlecks[i].variant = -i;
  }
  work->transferMode = 2;   // each transfer leaves 3/4 of the old picture,
  work->transferValue = 75; // a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  effect->variant++;
  if (effect->variant <= 0)
    effect->variant = 1;
  if (effect->variant > 4)
    effect->variant = 4;
  Sound_PlayFar(212);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (112 - frame * 2) | 0x1000;
    if (frame == 16)
      Func_0814cd48(effect->actors[0], 7, -1, 0, 20);

    // The pillar: the left half at x 47, then the right half mirrored at
    // x 64, each with its glow.
    if (frame <= 55) {
      s32 n;

      n = frame / 2 % 4; // a pillar picture every 2 frames
      BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
      // The left half's pillar draw and glow index in a once-only block,
      // which ends CSE's block so the mirrored half works out both pictures
      // again; as plain statements, 235 of 456 instructions differ in tla-en.
      do {
        draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        draw[1](canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
        n = frame / 4 % 3; // a glow picture every 4 frames
      } while (0);
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 40, ground.y - 36, 24,
              43);
      HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
      BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      n = frame / 2 % 4;
      draw[1](canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
      n = frame / 4 % 3;
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 64, ground.y - 36, 24,
              43);
      HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The first 32 flecks turn 0x400, rise 5 pixels and widen 4 pixels a
    // frame.
    for (i = 0; i != 32; i++) {
      struct EffectStep *fleck = &gFlecks[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += 0x400;
        fleck->y += 5 << 16;
        fleck->z += 64;
        EffectPosition_ProjectPoint(point, &pos);
        pos.x /= 2;
        size = (i & 1) + effect->variant;
        draw[0](canvas, sheet + Data_08197424[size - 1], pos.x - size,
                pos.y - size, size * 2, size * 2);
      }
      fleck->variant++;
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

#include "input.h"

#include "battle_work.h"

// The battle background's id, read through a union view of the bytes that
// begin with it. A union's member has the union's alias set, which conflicts
// with every store, so the read waits for view->secondMode's store as well
// as the scroll's and the scheduler keeps the two stores in source order; as
// *(u16 *)(battle)->unk670, 4 of 376 instructions differ in tla-en.
union BattleBackgroundId {
  u16 id;
  u8 bytes[6];
};
#define BACKGROUND(battle) (((union BattleBackgroundId *)(battle)->unk670)->id)

void BattleEffect_ClearActorTimers(void);
void Func_08191530(struct BattleEffectArgument *effect);

void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Func_08164b2c(s32 redDelta, s32 greenDelta, s32 blueDelta);

static inline s32 FillWords(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_FillWords, 2 of 376 instructions of Func_0813e7e0 differ in
   * tla-en. */
  return Iwram_FillWords(destination, size, value);
}

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 27 of 376 instructions of Func_0813e7e0 differ in
   * tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

// Shows the battle background fading in, until A (unused).

void Func_0813e7e0(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  s32 frame;
  s32 i;
  s32 j;
  s32 k;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  battle = Ram_HeapSlots->battleWork;
  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work->effect = effect;
  work->vblanksSinceTransfer = 1;
  Func_08191530(effect);

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
      s32 entry;

      entry = (s16)(((k * 16 + j * 2 + 1) << 8) | ((k * 8 + j) * 2));
      // The map written through a volatile pointer, which keeps the ROM's
      // order of stores; as a plain pointer, 185 of 376 instructions differ in
      // tla-en.
      *(volatile u16 *)(0x06003800 + i) = *(u16 *)&entry;
    }
  }

  /* FAKEMATCH(wrapper): the clears through ClearWords; as a direct call to
   * Iwram_ClearWords, 14 of 376 instructions of Func_0813e7e0 differ in tla-en.
   */
  ClearWords(canvas, 0x4000);
  ClearWords((void *)0x06004000, 0x4000);
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));

  // Keep the picture at 0x06008000 aside, show the battle background with
  // its colours black, and flash the first target.
  CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
  FillWords(Ram_MapCellBuffer, 0x7800, 0x01010101);
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  BattleBackground_LoadFar(1, BACKGROUND(battle), -1);
  Func_08164b2c(-31, -31, -31);
  Func_0814cd48(effect->actors[0], 7, 3, 0, 30);

  // Fade the colours up a shade every two frames; B starts again, A ends.
  for (frame = 0; frame != 32096; frame++) {
    // The A test reads the held keys through a volatile pointer, a load apart
    // from the B test's; as gInput.held, 10 of 376 instructions differ in
    // tla-en.
    if (gInput.held & 2)
      frame = 0;
    if (*(volatile u32 *)&gInput.held & 1)
      break;
    if (frame <= 63)
      Func_08164b2c(frame / 2 - 31, frame / 2 - 31, frame / 2 - 31);
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
#undef SLOT_BLITTER
#undef SLOT_BLITTER_ALTERNATE
#undef BLITTER
#undef PHASE_RESULTS
#undef SHARD_COUNT
#undef FRAME_COUNT
#undef gDropSplashes
#undef DROP_COUNT
#undef SPLASH_COUNT
#undef DROP_SPACING
#undef CLIP
#undef RECORD
#undef RECORD_ANIMATE
#undef RECORD_HIT
#undef RECORD_MOTION
#undef RECORD_SHAKE
#undef STREAK_MAX
#undef EFFECT_STEPS
#undef gBlitter
#undef gBlitterAlternate
#undef HI
#undef RECORD_KIND
#undef RECORD_COUNT
#undef RECORD_FLAGS
#undef RECORD_PALETTE
#undef RECORD_RATE
#undef RECORD_BLEND
#undef RECORD_FRAMES
#undef FLAG_WISPS
#undef FLAG_MOTES
#undef FLAG_GLINTS
#undef FLAG_SPARKS
#undef FLAG_HEAVY
#undef FLAG_FLASH
#undef FLAG_STAR
#undef gMotes
#undef gSparks
#undef BUILD_BLITTER
#undef SLOT_BATTLE_EFFECT
#undef SLOT_BATTLE_CANVAS
#undef SLOT_BATTLE_SHEET
#undef BATTLE_EFFECT_WORK_SIZE
#undef gFlecks
#undef BACKGROUND

// ---- games/tla/src/battle_effect/f13eb70.c ----

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
/* VIEW(f13eb70): tag struct BattleBackgroundId as struct BattleBackgroundId__f13eb70; f13ba50.c has another definition. */
union BattleBackgroundId__f13eb70 {
  u16 id;
  s32 word;
};
#define BACKGROUND(battle) (((union BattleBackgroundId__f13eb70 *)(battle)->unk670)->id)

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

/* VIEW(f13eb70): static ClearWords as ClearWords__f13eb70; f13ba50 has static inline s32 ClearWords(void *destination, s32 size) { return Iwram_ClearWords(destination, size); }. */
static inline s32 ClearWords__f13eb70(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_ClearWords, 14 of 1102 instructions of Func_0813eb70 differ in
   * tla-en. */
  return Iwram_ClearWords(destination, size);
}

/* VIEW(f13eb70): static FillWords as FillWords__f13eb70; f13ba50 has static inline s32 FillWords(void *destination, s32 size, u32 value) { return Iwram_FillWords(destination, size, value); }. */
static inline s32 FillWords__f13eb70(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_FillWords, 4 of 1102 instructions of Func_0813eb70 differ in
   * tla-en. */
  return Iwram_FillWords(destination, size, value);
}

/* VIEW(f13eb70): static CopyWords as CopyWords__f13eb70; f13ba50 has static inline s32 CopyWords(void *destination, const void *source, s32 size) { return Iwram_CopyWords(destination, source, size); }. */
static inline s32 CopyWords__f13eb70(void *destination, const void *source, s32 size) {
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

  ClearWords__f13eb70(canvas, 0x4000);
  ClearWords__f13eb70((void *)0x06004000, 0x4000);
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
  CopyWords__f13eb70(Ram_MapCellBuffer, (void *)0x06008000, 0x7800);
  FillWords__f13eb70(Ram_MapCellBuffer, 0x7800, 0x01010101);
  view->busy = 1;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  gBgScroll[1].x = 0;
  IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f81, 0x20000);
  FillWords__f13eb70((void *)0x050000c0, 0x100, 0x7fff7fff);
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
      FillWords__f13eb70((void *)0x06008000, 0x7800, 0x02020202);

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
      CopyWords__f13eb70((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
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
  ClearWords__f13eb70(canvas, 0x4000);
  ClearWords__f13eb70((void *)0x06004000, 0x4000);
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
/* VIEW(f13eb70): routine GetBattleObjectSlotFar as void **GetBattleObjectSlotFar(s32 objectId); motobj.h has struct BattleObj *GetBattleObjectSlotFar(s32 objectId). */
extern void **GetBattleObjectSlotFar__f13eb70(s32 objectId) __asm__("GetBattleObjectSlotFar");

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

/* VIEW(f13eb70): variable gResourceTableEntries as extern struct ResourceTableEntry gResourceTableEntries[]; vram_tab.h has extern struct VramBlockCacheEntry gResourceTableEntries[]. */
extern struct ResourceTableEntry gResourceTableEntries__f13eb70[] __asm__("gResourceTableEntries");
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
  (0x06010000 + gResourceTableEntries__f13eb70[(obj)->resource].tile)
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
  actor = *GetBattleObjectSlotFar__f13eb70(effect->actor);
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

// ---- games/tla/src/battle_effect/f14153c.c ----
struct FxPartyList;
struct MeshTriangle;
struct MeshVertex;
struct Model;
struct ResourceObjectWork;
struct StripQuad;

// A texture for the IWRAM renderer.
/* VIEW(f14153c): tag struct ModelTexture as struct ModelTexture__f14153c; render_model.h has another definition. */
struct ModelTexture__f14153c {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
};

/* VIEW(f14153c): tag struct RenderModel as struct RenderModel__f14153c; render_model.h has another definition. */
struct RenderModel__f14153c {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture__f14153c *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world); // BattleEffect_RunDjinniEntrance
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

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

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#include "vram_tab.h"
#include "item_icon.h"
struct EclipseObject {
  u8 unk0[9];
  u8 unk09 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[6];
  u8 slot;
};

struct EclipseVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};


// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
extern char ResourceId_EarthHorizonBackdrop, ResourceId_AerialViewBackdrop;
extern char ResourceId_GoldenDragonTiles, ResourceId_GoldenDragonHeadTiles;
extern char ResourceId_BlueFlameSheet, ResourceId_DustBlobSheet;
extern char ResourceId_Data134, ResourceId_TornadoSheet, ResourceId_Data0B4;
extern char ResourceId_Data0C1, ResourceId_Data0C9, ResourceId_Data0CA;

extern u8 gMapCellBuffer[];
/* VIEW(f14153c): variable gData_02014000 as extern struct EffectStep gData_02014000[]; f13eb70 has extern u8 gData_02014000[]. */
extern struct EffectStep gData_02014000__f14153c[] __asm__("gData_02014000");
extern u8 Data_02010400[];
extern u8 Data_02010800[];
extern u8 Data_02011000[];
extern u8
    Data_02012000[]; // the crystal texture and the flashes, after the map cells
extern u8 Data_02012800[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196de0;
extern const u8 Data_08197744[];
extern const u8 Data_08197748[];
extern const u8 Data_0819774f[];
extern const u32 Data_08197760[];
extern const u8 Data_081977a4[];
extern const u8 Data_081977c4[];
extern const u8 Data_081977e4[];
extern const s8 Data_081977e8[];
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells: dot sizes 1 to 10,
                             // cell offsets in the sheet
extern u16 gData_08197486[]; // ☀️ gBattleEffect_GlintCells
extern u8 gData_08197492[];  // ☀️ gBattleEffect_GlintCellWidths
extern u8 gData_08197498[];  // ☀️ gBattleEffect_GlintCellHeights
extern const u8 gData_081991b0[];
extern const u8 Data_081991c0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);
void ResourceObject_ReleaseFar(struct ResourceObjectWork *work);
void Func_08179e6c(void);
void Func_0813baec(void);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);

void VramBlock_Reset93Far(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter
void Func_08020388(void);
s32 VramCache_AllocEntry(s32 size);
void Palette_LoadBasicObjColors(void);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
void Func_081885f0(struct MeshVertex *vertices, struct MeshTriangle *triangles,
                   s32 segments);
void Graphics_LoadGreyRamp(void);
void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows,
                   s32 cellWidth, s32 cellHeight);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj) (0x06010000 + gResourceTableEntries[(obj)->slot].offset)

// Forwarded so that a constant size reaches the call's register at each call.

// Eclipse: the battlefield scrolls up, thirty-two objects (31 with
// Summon_GoldenDragonTiles, 1 with Summon_GoldenDragonHeadTiles) rise over
// the foes, a ring flares and collapses, then a beam strikes and a sphere
// bursts.
// XXX: what the player sees in each stage; a tbs-en-style render of the
// effect would settle it.
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  s32 frame;
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  u8 *battle;
  s32 i;
  s32 j;
  s32 k;
  s32 scroll;
  s32 baseX;
  s32 baseY;
  s32 velX;
  s32 velY;
  s32 cx;
  s32 cy;
  u32 u;
  s32 count;
  s32 ring;
  s32 shade;
  s32 offset;
  u8 *cells;
  s32 fade;
  s16 list[14];
  s32 place[4];
  struct WordPair req;
  struct ModelTexture__f14153c texture;
  struct ModelTexture__f14153c texture2;
  struct ModelTexture__f14153c texture3;
  struct RenderModel__f14153c *model;
  struct EffectStep *p;
  struct EclipseObject *obj;

  /* FAKEMATCH(wrapper): every fill through FillWords_, which sets a constant
   * size in the call's register at each call; as direct Iwram_FillWords calls,
   * 501 of 2159 instructions differ in tla-en. */
  /* FAKEMATCH(wrapper): every copy through CopyWords_, which sets a constant
   * size in the call's register at each call; as direct Iwram_CopyWords calls,
   * 133 of 2159 instructions differ in tla-en. */
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  battle = (u8 *)Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  scroll = -80;
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *tiles;

      list[0] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      Resource_ResetEntry(*(s32 *)(battle + 84));
      VramBlock_Reset93Far();
      gCameraSceneParameters[4] = 240;
      BattleBackground_LoadFar(1, (s32)&ResourceId_EarthHorizonBackdrop, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(Func_0813baec, 0x480);
      transition[4] = 1;
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonTiles);
      CopyWords_((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      tiles = gMapCellBuffer;
      for (i = 0; i != 31; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->priority = 2;
        work->objects[i] = obj;
        CopyWords_((void *)OBJ_TILES(obj), tiles, 0x400);
        tiles += 0x400;
      }
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonHeadTiles);
      CopyWords_((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      work->objects[31] = obj;
      obj->priority = 2;
      CopyWords_((void *)OBJ_TILES(obj), gMapCellBuffer, 0x400);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  FillWords_(canvas, 0x3c00, 0);
  FillWords_((void *)0x06004000, 0x3c00, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  REG_BG2X = 0;
  baseX = 0;
  baseY = 0;
  velX = 0;
  velY = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit[0] = Ram_HeapSlots->unk64[1];
  CopyWords_((void *)BG_PLTT,
             Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  {
    const u8 *texturePixels = (const u8 *)gData_02014000__f14153c;

    for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
      if (frame == 0) {
        baseX = -0x300000;
        baseY = 0xd40000;
        velX = 0x40000;
        velY = -0xa0000;
        work->fadeFrames = 1;
        work->fadeStep = frame;
        work->particles[16].variant = frame;
        Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, gData_02014000__f14153c, 0,
                                   0);
      }
      if (frame == 136) {
        u8 *dst = Data_02010400;
        for (i = 0; i != 7; i++) {

          void *o = (void *)OBJ_TILES(
              (struct EclipseObject *)work->objects[Data_08197748[i]]);
          // Default: set for real below; as s32 size = 0x400, 762 differ.
          s32 size = 0;
          size = 0x400;
          Iwram_CopyWordsEntry(o, &dst[i * 0x400], size);
        }
      }
      if (frame == 48) {
        Sound_PlayFar(139);
      }
      if (frame == 136) {
        Sound_PlayFar(140);
      }
      if (frame == 172) {
        Sound_PlayFar(144);
      }
      for (i = 0; i != 17; i++) {
        if (frame == Data_0819774f[i]) {
          FillWords_(canvas, 0x3c00, Data_08197760[i]);
        }
      }
      req = Data_08196de0;
      place[3] = 0;
      place[1] = 0xff0000;
      baseX += velX;
      baseY += velY;
      if (frame > 171) {
        if (frame > 183) {
          velX += -0x200;
          velY += 0x400;
        }
        if (frame <= 205) {
          velX = velX * 54 / 64;
          velY = velY * 54 / 64;
        }
      } else {
        if (velY < 0) {
          velY += 0x4000;
        }
        if (velX > 0) {
          velX += -0x2000;
        }
        if (frame > 31) {
          baseY += Trig_Sin((frame - 32) << 9) << 2 >> 4;
          if (frame <= 159) {
            baseX += 0x4000;
          }
        }
      }
      for (i = 0; i != 32; i++) {
        if (i == 25 && frame > 135) {
          place[0] = baseX + 0x200000;
          place[2] = baseY + 0x400000;
        } else {
          place[0] = baseX + (Data_081977a4[i] << 16);
          place[2] = baseY + (Data_081977c4[i] << 16);
        }
        if (place[2] <= 0xc7ffff) {
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)&req, 0);
        }
      }
      if (frame == 136) {
        work->fadeFrames = 24;
        work->fadeStep = 0;
      }
      cx = baseX / 2 - (work->particles[16].variant << 13) + 0x1c0000;
      cy = baseY + (work->particles[16].variant << 14) + 0x380000;
      u = frame - 136;
      if (u <= 84) {
        if (frame == 136) {
          work->particles[16].variant = 4;
          work->particles[16].x = 0;
          work->particles[16].z = 0;
          Func_0815b434(work->sheet, work->particles[16].variant * 2, 0x10000);
        }
        if (frame > 171) {
          if (frame == 172) {
            work->particles[16].variant = 64;
            Func_0815b434(work->sheet, 128, 0x10000);
            velX = 0x30000;
            velY = -0x30000;
          }
          if (frame > 203 && work->particles[16].variant > 1) {
            work->particles[16].variant -= 4;
            if (work->particles[16].variant <= 1) {
              work->particles[16].variant = 2;
            }
            Func_0815b434(work->sheet, work->particles[16].variant * 2,
                          0x10000);
          }
        } else if (frame > 135) {
          velX += -0x800;
          velY += 0x800;
          if (++work->particles[16].variant <= 32) {
            Func_0815b434(work->sheet, work->particles[16].variant * 2,
                          0x10000);
          } else {
            work->particles[16].variant = 32;
          }
        }
        {
          /* FAKEMATCH(temporary): the variant copied into size in a block of
           * its own; as Func_0818caa8(..., work->particles[16].variant * 2), 50
           * of 2159 instructions differ in tla-en. */
          s32 size = work->particles[16].variant;

          Func_0818caa8(work->sheet, cx >> 16, cy >> 16, size * 2);
        }
      }
      if (frame > 47) {
        struct RenderModel__f14153c *model;
        void *verts;
        s32 modelShade;

        if (frame == 48) {
          for (i = 0; i != 16; i++) {
            work->particles[i].x = Random16() & 0xffff;
            work->particles[i].y = (Random16() & 255) + 128;
            if (Random16() & 1) {
              work->particles[i].y = -work->particles[i].y;
            }
            work->particles[i].variant = i * -8;
          }
          Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                     work->sheet + 0x2000, 1, 0);
          Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                     work->sheet + 0x4000, 0, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        modelShade = 0;
        model->shade = modelShade;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + 0x4000;
        model->kind = 6;
        model->mesh = Data_08199220;
        model->texture = &texture;
        model->vertices = verts;
        for (i = 0; i != 8; i++) {
          work->particles[i].variant++;
          work->particles[i].x += work->particles[i].y;
          if (work->particles[i].variant >= 0 &&
              work->particles[i].variant <= 47) {
            Render_ResetTransformState();
            {
              s32 x = ((i & 3) << 17) + 0x20000;
              s32 o = -0x400000;
              Func_08015160(cx + x + o, cy - (((i & 3) + 2) << 16) + o, 0);
            }
            Func_080151e4(0x8000, 0x10000, 0x10000);
            Func_080150e4(work->particles[i].x);
            Func_080151e4(Trig_Sin(work->particles[i].variant * 682) + 0x4000,
                          0x20000, 0x10000);
            Render_TransformVertices(gData_081991b0, verts, 4);
            Render_DrawModelList(model);
          }
        }
        if (frame >= 136 && frame <= 171) {
          s32 s = 0x40000 - u * 6000;

          modelShade = u * 4 - 64;
          if (modelShade > -16) {
            modelShade = -16;
          }
          texture.logWidth = 6;
          model->shade = modelShade;
          texture.logHeight = 6;
          model->kind = 7;
          texture.pixels = texturePixels;
          model->texture = &texture;
          model->mesh = gData_08199340;
          model->vertices = verts;
          Render_ResetTransformState();
          Func_08015160(cx + -0x3e0000, cy + -0x420000, 0);
          Func_080151e4(s >> 1, s, s);
          SceneTransform_ApplyPitch(0x2000);
          Func_080150e4(0x2800);
          SceneTransform_ApplyYaw(-frame << 10);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
        if (frame >= 172 && frame <= 220) {
          Render_ResetTransformState();
          Func_08015160(cx + -0x3e0000, cy + -0x420000, 0);
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(0x2800);
          if (frame > 203) {
            Func_080151e4(0x80000 - ((frame - 204) << 15), 0x20000, 0x10000);
          } else {
            Func_080151e4(0x80000, 0x20000, 0x10000);
          }
          Render_TransformVertices(Data_081991c0, verts, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      if (frame >= 148 && frame <= 219) {
        for (j = 0; j != 4; j++) {
          s32 t = (frame + j) & 3;
          // Default: set for real below.
          s32 x = 0;
          s32 y = 0;

          if (frame <= 171) {
            s32 size = work->particles[16].variant * 2;

            x = Random16() % size + (cx >> 16) - (size >> 1) -
                (gData_08197492[t] >> 1);
            y = Random16() % size + (cy >> 16) - (size >> 1) -
                (gData_08197498[t] >> 1);
          } else {
            u32 a = Random16() & 63;
            s32 r = work->particles[16].variant;

            x = (cx >> 16) - (a >> 1) - (gData_08197492[t] >> 1) +
                Random16() % r - r / 2 - 8;
            y = a + (cy >> 16) - (gData_08197498[t] >> 1);
          }
          Func_08196404(SLOT_ALT, 7, 7, Data_08197744[Random16() & 3] | 3, 1);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, work->sheet + 0x2000 + gData_08197486[t], x, y,
              gData_08197492[t], gData_08197498[t]);
          HeapSlot_Release(SLOT_ALT);
        }
      }
      HeapSlot_Release(SLOT_ALT);
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(Func_0813baec);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  for (i = 0; i != 32; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Func_08020388();
  *(s32 *)(battle + 84) = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->effect);
  for (i = 0; i != effect->count; i++) {
    Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
    Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
  }
  Palette_LoadBasicObjColors();
  REG_DISPCNT = 1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  // Two empty once-only blocks before the fills: without them, 18 differ.
  do {
  } while (0);
  FillWords_(canvas, 0x4000, 0);
  do {
  } while (0);
  FillWords_((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
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

    if (v > 63) {
      v = 63;
    }
    for (j = 0; j != 120; j++) {
      work->sheet[j * 120 + i] = v;
    }
  }
  fade = 0;
  for (frame = 0; frame != 48; frame++) {
    if (frame > 39) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
    }
    if (frame == 16) {
      Sound_PlayFar(140);
    }
    if (frame == 18) {
      work->shakeFrames = 90;
      Sound_PlayFar(145);
      FillWords_(canvas, 0x4000, 0x3f3f3f3f);
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 4);
      }
    }
    if (frame <= 109) {
      // Default: set for real below.
      struct RenderModel__f14153c *model = 0;
      void *verts = 0;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture2.logWidth = 5;
      texture2.logHeight = 2;
      texture2.pixels = work->sheet + 0x3840;
      model->mesh = Data_08199220;
      model->shade = fade;
      model->texture = &texture2;
      model->vertices = verts;
      model->kind = 6;
      if (frame > 15) {
        struct EffectStep *q = &work->particles[63];

        if (frame == 16) {
          q->x = 0x800000;
          q->y = -0x280000;
        }
        if (frame <= 19) {
          q->x += -0x200000;
          q->y += 0x400000;
        }
        if (frame <= 27) {
          cells = work->sheet + 0x38c0;
          Func_0815b434(cells, 96, 0x10000);
          Func_0818caa8(cells, q->x >> 16, q->y >> 16, 96);
        }
        Render_ResetTransformState();
        Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4(0x9800);
        Func_080151e4(0xdc500, 0x29c40, 0x8000);
        Render_TransformVertices(Data_081991c0, verts, 4);
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
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  count = -1;
  transition[4] = 1;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  Func_08164b2c(31, 31, 31);
  gBgScroll[1].x = 0;
  gCameraSceneParameters[4] = 240;
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x400, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0,
                             0);
  {
    s32 *v = &gData_02014000__f14153c[0].variant;
    s32 c = -1;

    for (i = 0; i != 512; i++) {
      *v = c;
      v += 7;
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  gBgScroll[1].y = 32;
  {
    const void *mesh = Data_02011000;
    void *vertices = gMapCellBuffer;
    const void *mesh2 = Data_02012800;

    for (frame = 0; frame != 26; frame++) {
      if ((gInput.repeat & 3) && count == -1) {
        count = 8;
      }
      if (frame <= 6 && count == -1) {
        s32 b = 31 - frame * 8;
        s32 a = b;
        s32 c = b;

        if (c < -10) {
          a = -10;
        }
        if (c < -15) {
          b = -15;
        }
        if (c <= 3) {
          c = 4;
        }
        Func_08164b2c(a, b, c);
      }
      if (frame > 17 || count > 0) {
        Func_08164abc(4, 4, 4);
        if (--count == 0) {
          break;
        }
      }
      for (i = 0; i != 16; i++) {
        // A constant here and in p below, so both share one pool word (as
        // gData_02014000, 70 to 786 differ).
        struct EffectStep *q =
            (struct EffectStep *)(Ram_MapCellBuffer + 0x4000) +
            ((frame * 16 + i) & 511);
        // Default: set for real below.
        s32 a = 0;
        s32 r = 0;

        a = Random16();
        r = (Random16() & 15) + frame * 2;
        q->x = Trig_Sin(a) * r + 0x3c0000;
        q->y = Trig_Cos(a) * r + 0x3c0000;
        q->velocityX = 0;
        q->velocityY = 0;
        q->variant = (Random16() & 15) + 16;
      }
      {
        const u16 *sizeOffsets = gData_08197410;
        for (i = 0; i != 512; i++) {
          p = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000) + i;
          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 1;
            // sizeOffsets[n - 1] spelled as a byte offset: plain, 49 differ.
            s32 off = n * 2 - 2;

            blit[0](canvas,
                    sprites + *(const u16 *)((const u8 *)sizeOffsets + off),
                    (p->x >> 16) - n / 2, (p->y >> 16) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
            p->variant--;
          }
        }
      }
      model = Render_AllocateModelList(1);
      shade = 0;
      if (frame <= 31) {
        s32 s = 0x10000;

        if (frame > 15) {
          s += 0x10000 - (frame << 12);
        }
        texture3.logWidth = 5;
        texture3.logHeight = 2;
        texture3.pixels = work->sheet + 0x3840;
        model->mesh = Data_08199220;
        model->shade = shade;
        model->vertices = gMapCellBuffer;
        model->kind = 6;
        model->texture = &texture3;
        Render_ResetTransformState();
        Func_08015160(-0x20000, 0x100000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Func_080150e4(0x9800);
        Func_080151e4(s, 0x20000, 0x8000);
        Render_TransformVertices(Data_081991c0, gMapCellBuffer, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        Sound_PlayFar(208);
        Func_0818de3c(Data_02011000, 16, 8, 8, 8);
        {
          s32 ringAngle = 0;
          for (ring = 0, offset = 0; ring != 8;
               ringAngle += 0x888, offset += 64, ring++) {
            s32 y = -Trig_Cos(ringAngle) * 96 >> 16;
            s32 rr = Trig_Sin(ringAngle) * 48 >> 16;
            struct EclipseVertex *v = (struct EclipseVertex *)Data_02010800;

            k = 0;
            v = (struct EclipseVertex *)((u8 *)v + offset);
            for (; k != 16; k++) {
              v->x = Trig_Sin(k << 12) * rr >> 16;
              v->y = y;
              v->z = Trig_Cos(k << 12) * rr >> 16;
              v++;
            }
          }
        }
        Func_081885f0(Data_02012000, Data_02012800, 32);
      }
      texture3.logWidth = 5;
      texture3.logHeight = 5;
      texture3.pixels = work->sheet;
      {
        model->kind = 6;
        model->mesh = mesh;
        model->vertices = vertices;
        model->texture = &texture3;
        model->shade = shade;
        model->unk04 = 3;
      }
      {
        s32 s = (frame << 9) + 0x8000;

        if (s > 0x10000) {
          s = 0x10000;
        }
        Render_ResetTransformState();
        Func_08015160(-0x20000, 0xc0000, 0);
        SceneTransform_ApplyPitch(0x2000);
        SceneTransform_ApplyYaw(-frame << 10);
        Func_0801521c(s);
        Render_TransformVertices(Data_02010800, vertices, 128);
        Render_DrawModelList(model);
      }
      texture3.logHeight = 5;
      texture3.logWidth = 7;
      texture3.pixels = work->sheet + 0x400;
      model->texture = &texture3;
      model->kind = 7;
      model->mesh = mesh2;
      model->unk04 = shade;
      model->vertices = vertices;
      model->scrollU = shade;
      model->scrollV = shade;
      {
        for (i = 0; i != 4; i++) {
          s32 t = frame + 24 - i * 6;

          if (t >= 0 && t <= 63) {
            s32 s = (Data_081977e4[i] * t << 10) + 0x4000;

            shade = 0;
            if (t <= 15) {
              shade = t * 4 - 64;
            }
            if (t > 40) {
              shade = (40 - t) * 4;
            }
            model->shade = shade;
            Render_ResetTransformState();
            Func_08015160(-0x20000, 0x200000, 0);
            Func_080151e4(s / 4, s / 4, s / 4);
            SceneTransform_ApplyPitch(10000);
            SceneTransform_ApplyYaw((frame + i * 8) << 10);
            Func_080151e4(0x10000, 0x60000, 0x10000);
            Func_08015160(0, Data_081977e8[i] << 16, 0);
            Render_TransformVertices(Data_02012000, vertices, 64);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      work->transferPending = 1;
      wait(1);
    }
  }
  FillWords_((void *)0x06004000, 0x3c00, 0x3f3f3f3f);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  gCameraSceneParameters[4] = 120;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  wait(1);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Graphics_LoadGreyRamp();
  FillWords_(canvas, 0x3c00, 0x3f3f3f3f);
  Func_081180e8(134);
  for (frame = 0; frame != 32; frame++) {
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Unused: two empty functions the ROM keeps after Eclipse; nothing calls them.
void Func_0814293c(void) {}

void Func_08142940(void) {}


struct Model;

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// The three kinds the scenes ask for.
enum { KIND_RIGHT = 78, KIND_LEFT = 79, KIND_FINALE = 80 };

// One model for the IWRAM renderer.
/* VIEW(f14153c): tag struct ModelDraw as struct ModelDraw__f14153c; f13ba50.c has another definition. */
struct ModelDraw__f14153c {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct ModelTexture__f14153c *texture;
  s32 depthBias;
  u8 unk18;
  u8 shade;
  u8 unk1a[2];
};

// The four crystals: the frame each starts after 48, how fast it grows and
// how tall.
extern u8 Data_081977ec[];
extern u8 Data_081977f0[];
extern u8 Data_081977f4[];

// The models: the rings' quad (Data_08199220), its corners, the crystal and its
// corners.
extern const u8 gData_081990d0[];
extern const u8 gData_08199090[];

extern char ResourceId_FlashBurstSheet; // directory row 0x13e
extern char ResourceId_Data0DA;
extern char ResourceId_OrangePaletteB; // directory row 0x148
extern char ResourceId_EffectPalette03;

void Func_08164a4c(s32 blueDelta, s32 greenDelta,
                   s32 redDelta); // Palette_BrightenBgEntries
void Func_08118150(s32 kind);

// Rings and crystals gather on the user, for the scenes.

// A glow on the canvas map widens over the user for 56 frames while 16
// spinning rings rise around it, four abreast. From frame 48 the screen
// shakes, flashes burst upward and four crystals grow out of the ground one
// after another, spinning. The user flashes at frames 32 to 68. Kinds 78
// and 79 stand to either side and hand over to the battle (Func_08118150)
// at frame 62; kind 80 runs 100 frames, fades to white and restores the
// scene's view itself.

void Func_08142944(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition center;
  struct ModelTexture__f14153c texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 index;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  work->vblanksSinceTransfer = 1;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02012000, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frames = 100;
  if (kind != KIND_FINALE)
    frames = 76;
  for (frame = 0; frame != frames; frame++) {
    index = (frame - 48) * 2;
    // The effect's centre, beside or above the user.
    EffectPosition_ProjectBattler(effect->actor, &center);
    if (kind == KIND_RIGHT) {
      center.x += 40;
      center.y -= 24;
    } else if (kind == KIND_LEFT) {
      center.x -= 16;
      center.y -= 24;
    } else {
      center.y -= 48;
      /* FAKEMATCH(barrier): an empty asm taking the centre's address gives it
       * two more references, so the register allocator ranks it above the
       * corners; without it, 189 of 734 instructions differ in tla-en. */
      __asm__("" : : "r"(&center));
    }

    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
      if (kind == KIND_LEFT)
        Iwram_CopyWords((void *)BG_PLTT,
                        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                        128);
      else
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EffectPalette03), 128);
      if (frame == 0)
        Sound_PlayFar(140);
    }
    if (frame == 48)
      Sound_PlayFar(144);
    if (frame == 56)
      Sound_PlayFar(148);
    if (frame == 32)
      Func_0814cd48(effect->actor, 7, -1, -1, -1);
    if (frame == 68)
      Func_0814cd48(effect->actor, 0, -1, -1, -1);
    if (kind == KIND_FINALE) {
      // The finale fades to white from frame 62.
      if (frame > 61) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        Func_08164a4c(2, 2, 2);
      }
    } else if (frame == 62) {
      Func_08118150(kind);
    }

    // From frame 48: shake, clear the canvas, and for 8 frames burst two
    // flashes a frame from a point near the centre.
    if (frame >= 48) {
      struct EffectStep *flashes;

      if (frame == 48) {
        {
          s32 *variant = &gData_02014000__f14153c[0].variant;

          for (i = 0; i != 64; i++) {
            *variant = -1;
            variant += 7;
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      }
      flashes = gData_02014000__f14153c;
      if (frame >= 48 && frame <= 55) {
        s32 x = ((Random16() & 63) + center.x / 2 - 32) << 16;
        s32 y = ((Random16() & 31) + center.y) << 16;
        s32 k = 0;
        s32 spread = 15;
        struct EffectStep *burst = &flashes[index];

        for (; k != 2; k++) {
          struct EffectStep *flash = &burst[k];
          s32 speed = 31; // Set first: the mask is the tied operand.
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + (((Random16() & spread) - 8) << 16);
          flash->y = y + (((Random16() & spread) - 8) << 16);
          flash->velocityX = Trig_Sin(angle) * (speed + 32) >> 3;
          flash->velocityY = Trig_Cos(angle) * (speed + 32) >> 3;
          flash->variant = 0;
        }
      }
      {
        struct EffectStep *flash = flashes;

        for (i = 0; i != 16; i++, flash++) {
          if (flash->variant >= 0 && flash->variant <= 17) {
            draw[0](canvas, work->sheet + (flash->variant / 3 << 11),
                    HI(flash->x) - 16, HI(flash->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(flash, 62, -0x800);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      s32 cx = center.x / 2 << 16;
      s32 cy = center.y << 16;
      struct ModelDraw__f14153c *model;
      void *corners;
      s32 bias;

      // The glow on the canvas map widens for 56 frames.
      if (frame <= 63) {
        s32 size = 128;
        s32 width = frame + 8;

        if (width <= 64) {
          size = width * 2;
          Func_0815b434(Ram_MapCellBuffer, size, 0x10000);
        }
        Func_0818caa8(Ram_MapCellBuffer, cx >> 16, cy >> 16, size);
      }

      // Each ring has an angle (x), a spin (y) and an age (variant), the
      // later ones 4 frames apart.
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *ring = &work->particles[i];

          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1)
            ring->y = -ring->y;
          ring->variant = -4 * i;
        }
      }

      corners = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      bias = 0;
      model->depthBias = bias;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x3000;
      model->kind = 7;
      model->texture = &texture;
      model->model = Data_08199220;
      model->corners = corners;

      // The rings rise for 24 frames, spinning and swelling.
      for (i = 0; i != 16; i++) {
        struct EffectStep *ring = &work->particles[i];

        ring->variant += 2;
        ring->x += ring->y * 2;
        if (ring->variant >= 0 && ring->variant <= 47) {
          Render_ResetTransformState();
          {
            s32 column = i & 3;

            Func_08015160(cx + ((column * 2 + 2) << 16) - 0x400000,
                          cy - ((column + 2) << 16) - 0x400000, 0);
          }
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(ring->x);
          Func_080151e4(Trig_Sin(ring->variant * 682) + 0x10000, 0x20000,
                        0x10000);
          Render_TransformVertices(gData_081991b0, corners, 4);
          Render_DrawModelList(model);
        }
      }

      // The four crystals grow out of the ground from frame 48 on.
      model->texture = &texture;
      model->corners = corners;
      texture.logWidth = 6;
      texture.pixels = Data_02012000;
      model->kind = 7;
      texture.logHeight = 6;
      model->model = gData_081990d0;
      for (i = 0; i != 4; i++) {
        s32 start = Data_081977ec[i] + 48;

        model->shade = (frame * 8 + i * 32) & 127;
        if (frame > start) {
          bias = (start - frame) * 8 + 56;
          if (bias > -16)
            bias = -16;
          if (bias > -64) {
            s32 growth = (frame - start) * Data_081977f0[i] * 2000 + 0x4000;

            model->depthBias = bias;
            Render_ResetTransformState();
            Func_08015160((center.x / 2 - 60) << 16, 0x200000, 0);
            Func_080151e4(growth * 2, Data_081977f4[i] * growth + 0x40000,
                          growth / 4);
            SceneTransform_ApplyPitch(1000);
            if (i & 1)
              SceneTransform_ApplyYaw((i * 8 + frame) << 10);
            else
              SceneTransform_ApplyYaw((i * 8 - frame) << 10);
            Render_TransformVertices(gData_08199090, corners, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  if (kind == KIND_FINALE) {
    // Put the scene's view back by hand.
    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    Task_Remove(Palette_StepFadeTransfer);
  } else {
    BattleEffect_EndCanvasLayer();
  }
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}


/// The battle effect work, as far as the canvas transfer and the canvas layer
/// read it.
struct CanvasWork {
  u8 unk0000[0x7780];
  s32 transferMode;
  s32 transferValue;
  u8 unk7788[0x18];
  s32 savedBg1X;
  s32 savedBg1Y;
  s32 shakeFrames;
  u8 unk77ac[8];
  s32 fadeFrames;
  s32 fadeStep;
  u8 unk77bc[0x128];
  s32 vblanksSinceTransfer;
  s32 transferPending;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_Brighten(void *source, s32 amount, void *destination,
                          s32 size);

/// The frame callback that copies the effect canvas to BG character block 1
/// when a transfer is pending, in the way the transfer mode asks for.
void BattleEffect_ProcessPendingGraphicsTransfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source = Ram_HeapSlots->battleFxWork[1];
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, which keeps
       * each call's own constants, as the scene layer does; as direct
       * Iwram_CopyWords calls in this callback and in
       * BattleEffect_ProcessPendingStampTransfer,
       * BattleEffect_FlushPendingGraphicsTransfer and
       * BattleEffect_ProcessPendingBlock2Transfer, 128 of 369 instructions
       * differ in tla-en. */
      CopyWords_((void *)0x06004000, source, 0x4000);
      break;
    case 1:
      CopyWords_((void *)0x06004000, source, 0x4000);
      /* FAKEMATCH(wrapper): the fill forwarded through FillWords_, which keeps
       * each call's own constants, as the scene layer does; as direct
       * Iwram_FillWords calls in this callback and in the same three
       * callbacks, 16 of 369 instructions differ in tla-en. */
      FillWords_(source, 0x4000, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06004000, 0x4000);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06004000,
                         0x4000);
      break;
    case 4:
      ColorBuffer_Brighten(source, work->transferValue, (void *)0x06004000,
                           0x4000);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// When a transfer is pending, copies the 0x1000-byte canvas at work 0x6780
/// to the stamp's tiles (the pointer at work 0x7b74), then clears it.
void BattleEffect_ProcessPendingStampTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  u32 ime;
  s32 *pending;

  IRQ_LOCK(ime);
  pending = (s32 *)(work + 0x78ec);
  if (*pending == 1) {
    *pending = 0;
    IRQ_UNLOCK(ime);
    CopyWords_(*(void **)(work + 0x7b74), work + 0x6780, 0x1000);
    FillWords_(work + 0x6780, 0x1000, 0);
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// The same callback for the 0x7800-byte canvas in the map cell buffer, copied
/// to BG character block 2.
void BattleEffect_FlushPendingGraphicsTransfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source = Ram_MapCellBuffer;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      CopyWords_((void *)0x06008000, source, 0x7800);
      break;
    case 1:
      CopyWords_((void *)0x06008000, source, 0x7800);
      FillWords_(source, 0x7800, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06008000, 0x7800);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000,
                         0x7800);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// The same for the canvas in the second effect work block.
void BattleEffect_ProcessPendingBlock2Transfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    source = Ram_HeapSlots->battleFxWork[1];
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      CopyWords_((void *)0x06008000, source, 0x7800);
      break;
    case 1:
      CopyWords_((void *)0x06008000, source, 0x7800);
      FillWords_(source, 0x3c00, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06008000, 0x3c00);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x3c00);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000,
                         0x3c00);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

void Func_08143354(void) {
  u32 *map = (u32 *)0x0600f800;
  u32 tiles;
  s32 row;
  s32 column;

  /* FAKEMATCH(wrapper): both fills through FillWords_, which sets each call's
   * own constants anew; as direct Iwram_FillWords calls, 33 of 40 instructions
   * differ in tla-en. */
  FillWords_((void *)map, 0x100, -1);
  map += 0x40;
  FillWords_((void *)map, 0x80, 0x03ff03ff);
  map += 0x20;
  tiles = (u32)(Ram_MapCellBuffer + 0x200);
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 8; column++) {
      *map++ = tiles;
      tiles += 0x00020002;
    }
    for (column = 0; column != 8; column++)
      *map++ = 0x03ff03ff;
  }
}

#include "battle_fx_work.h"

// The scenes' frame callbacks that copy the display registers the effect
// keeps in its work to the hardware.

/// Writes the window, display and blend registers, all at once.
void Graphics_ApplyWindowBlendRegisters(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 31 of 55 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->win0h;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 55 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_WIN0H = value;
  }
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes background 2's scale (BG2PA and BG2PD) and its reference point.
void Display_ApplyBg2Affine(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 32 of 30 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->bg2pa;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 30 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_BG2PA = value;
  }
  REG_BG2PD = work->bg2pd;
  /* FAKEMATCH(barrier): an empty asm keeps the next offset's step after the
   * store before it; without it, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Writes background 2's reference point.
void Display_ApplyBg2Reference(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 20 of 20 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  u32 hold;

  /* FAKEMATCH(barrier): an empty asm sets hold, which keeps r1 until the
   * first value is read, so the offset takes r0; without hold and its asm,
   * 8 of 20 instructions differ in tla-en. */
  __asm__ volatile("" : "=l"(hold));
  {
    u32 value = work->bg2x;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built, and ends hold; as an asm on hold
     * alone, 14 of 20 instructions differ in tla-en. */
    __asm__ volatile("" : "+l"(value) : "l"(hold));
    REG_BG2X = value;
  }
  REG_BG2Y = work->bg2y;
}

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

void Palette_StepFadeTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s32 *framesLeft;
  u8 *battle;

  framesLeft = (s32 *)(work + 0x77b4);
  battle = (u8 *)Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = (s32 *)(work + 0x77b8);

    (*step)++;
    ColorBuffer_ScaleFar(battle + 0x56c, (void *)&BG_PLTT_COLOR(6, 0),
                         0x10000 - *step * 1092, 128);
    (*framesLeft)--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  Iwram_ClearWords(Ram_HeapSlots->battleFxWork[0] + 0x78dc, 8);
}

/// Counts down each target's flash timer; when one runs out, the target gets
/// its own palette back.
void BattleEffect_CountDownBattlerTints(void) {
  u8 *base;
  s32 i;
  s32 index;

  base = Ram_HeapSlots->battleFxWork[0];
  i = 0;
  do {
    if (base[0x78dc + i] != 0) {
      if ((base[0x78dc + i] = base[0x78dc + i] - 1) == 0) {
        index = i * 2 + 36;
        Func_0814cd48(*(s16 *)(*(u8 **)(base + 0x78f0) + index), 0, -1, -1, 0);
      }
    }
    i += 1;
  } while (i != 8);
}

void Func_0814355c(void) {
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
}

#undef HI


static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

void BattleEffect_ConfigureEffectDisplay(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);

// Twin: The Broken Seal's BattleEffect_StartCanvasLayer. The Lost Age's
// changes: BattleEffect_ConfigureEffectDisplay for the display set-up, DISPCNT
// 0x1741, Palette_StepFadeTransfer at 0x480, Func_0814355c for the display
// registers and one more work word cleared.
void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);

  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->transferPending = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  wait(1);
}

void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);

/// Opens the effect canvas layer: BG2 becomes a 2x-wide 128x128 canvas of
/// 16x16 characters, blended over the battle background, whose palette
/// starts fading.
void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);

  // The canvas map: 16 rows of 8 character pairs.
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  /* FAKEMATCH(wrapper): the clear forwarded through an inline, which keeps each
   * call's own constants and the routine in a callee-saved register; as direct
   * Iwram_ClearWords calls, 24 of 259 instructions of
   * BattleEffect_OpenCanvasLayer differ in tla-en. */
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->transferPending = 0;
  wait(1);
}

void BattleEffect_StartTiledCanvas(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0;
    }
  }
}

void BattleEffect_StartTiledCanvasFilled(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
    }
  }
}

/// Closes the effect canvas layer: BG1 back where it was, BG2 and its
/// characters cleared, then the background palette faded back in.
void BattleEffect_EndCanvasLayer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  REG_BG2CNT = BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1) |
               BGCNT_PRIORITY(3);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  REG_BLDCNT = 0;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(*(u16 *)battle->unk670, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}

/// Hides the canvas layer: marks the background view, queues DISPCNT 0x1541,
/// fades the background palette back and queues BG1's control word.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)
              ->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = 1;
  QueueIoWriteDelay2((u32)&REG_DISPCNT,
                     DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                         DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1);
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                          BGCNT_PRIORITY(3),
                      IO_WRITE_16);
  wait(1);
}

void Func_08143d80(void) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 off;

  BattleEffect_ClearActorTimers();
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)screen->unk670, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  ClearWords_((void *)0x06003fc0, 64);
  FillWords_((void *)0x0600f900, 0x200, -1);
  off = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 32; col++, off += 2) {
      if (col > 15)
        *(volatile u16 *)(0x0600fb00 + off) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
    }
  }
  REG_DISPCNT = DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  REG_BG1CNT = BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS | BGCNT_PRIORITY(1);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  wait(1);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// The Broken Seal's BattleEffect_DrawClippedCanvasLine, with the fraction in
/// bit 31 (started at 0x80 << 23, carried when negative and cleared with a
/// shift pair) and dx and dy reused as the step direction.
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80 << 23;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  if (y0 < 0)
    y0 = 0;
  if (y0 > 127)
    y0 = 127;
  if (y1 < 0)
    y1 = 0;
  if (y1 > 127)
    y1 = 127;

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    step <<= 23;
    x = x0;
    if (dx < 0)
      dx = -1;
    else
      dx = 1;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        x += dx;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    step <<= 23;
    y = y0;
    if (dy < 0)
      dy = -1;
    else
      dy = 1;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        y += dy;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  }
}

#include "lib.h"
#include "sound.h"

// The skulls live in the map buffer, 64 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

void SceneTransform_ApplyPosition(const s32 *pos);

extern char ResourceId_SkullSheet;
extern char ResourceId_PinkBurstSheet; // directory row 0x17f

#define SKULLS 8 // per target
#define SKULL_SIZE 24
#define SKULL_CELL (24 * 24)

// Closing skulls: for each target in turn, 32 frames apart, eight skulls
// appear one every four frames from all around, up to 64 pixels away, and
// close in on a point 20 pixels above the target while the view of them
// turns; the background waves line by line throughout.
void BattleEffect_RunHaunt(struct BattleEffectArgument *effect) {
  s32 k;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the skull sheet with its palette, the pink burst palette, the
  // max and additive blitters, the wave's HBlank DMA and a transfer that
  // fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet), 128);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Scatter every skull up to 64 pixels from its centre.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      Func_081180e8(0);

    // The background waves sideways, one sine step per line.
    row = work->bg2xLines;
    if (effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;

      // Each target's skulls turn about a point 20 pixels above it.
      Render_ResetTransformState();
      // The look-at call in a once-only block, so the eye loads before the
      // point it looks at; as the plain call, 2 of 355 instructions differ in
      // tla-en.
      do {
        Transform_SetLookAt(camera, camera + 3);
      } while (0);
      point[0] = target->x;
      point[1] = 20 << 16;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > k * 32) {
        SceneTransform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          Func_0814cd48(effect->actors[k], 7, 5, k, 32);

        // A skull moves one part in its distance toward the centre each
        // frame, and is gone once it arrives.
        for (i = 0; i != SKULLS; i++) {
          struct EffectStep *skull = &PARTICLES[k * 64 + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint((s32 *)skull, &pos);
              pos.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * SKULL_CELL,
                      pos.x - SKULL_SIZE / 2, pos.y - SKULL_SIZE / 2,
                      SKULL_SIZE, SKULL_SIZE);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

extern char ResourceId_SpiralSheet;

#define SPIRAL_SIZE 32
#define SPIRAL_CELS 4

// Member orbit: over each target in turn, sixteen frames apart, four
// spirals circle a point 40 pixels above it for 60 frames, the blitter
// flipping every sixteen frames; the background waves line by line.
void BattleEffect_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the spiral sheet with its palette, the two blitters, the wave's
  // HBlank DMA and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // On the far side the background and the spirals move 112 pixels left.
  if (effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }

  for (frame = 0; frame != (effect->count * 16) + 48; frame++) {
    s32 *camera;
    s32 *row;
    s32 i;

    // The background waves sideways line by line.
    camera = Ram_HeapSlots->cameraWork;
    row = work->bg2xLines;
    if (effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
        angle += 1024;
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    member = 0;
    if (effect->count != 0) {
      while (member != effect->count) {
        struct MotionObject *target;

        target = GetBattleObjectSlotFar(effect->actors[member])->object;
        if (frame > member * 16 && frame < (member * 16) + 60) {
          s32 spin;

          // Halfway through, the target's flash stops.
          if (frame == (member * 16) + 32)
            Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
          point[0] = target->x;
          point[1] = 40 << 16;
          point[2] = target->z;
          EffectPosition_ProjectPoint(point, &screen);

          // The four spirals, a quarter turn apart, each a 32 x 32 cel of
          // the sheet's four-frame animation.
          for (i = 0; i != SPIRAL_CELS; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame + i * 32) << 9;
            x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + offsetX;
            y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - SPIRAL_SIZE / 2, y - SPIRAL_SIZE / 2,
                           SPIRAL_SIZE, SPIRAL_SIZE);
          }
        }
        member++;
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#include "battle_effect_argument.h"

void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Demon Night (kind 50).
DEFINE_EFFECT_KIND(BattleEffect_RunDemonNight, Func_08144628, 0)

/// Abilities: Rumble (kind 308).
DEFINE_EFFECT_KIND(BattleEffect_RunRumble, Func_08144628, 1)

/// Abilities: Haunting (kind 351).
DEFINE_EFFECT_KIND(BattleEffect_RunHaunting, Func_08144628, 2)

/// Abilities: Thorny Grave (kind 127).
DEFINE_EFFECT_KIND(BattleEffect_RunThornyGrave, Func_08144628, 3)

void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Func_0815b634(void);                // mode 3 only

extern char ResourceId_MemberBurstImage;

extern char ResourceId_MarsDjinnSheet; // directory row 0x150
extern char ResourceId_MercuryDjinnSheet;
// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 Data_08197864[];

#define MOTES_PER_TARGET 128
#define MOTE_CELL (20 * 32)

// Member burst (kinds through four wrappers; mode picks the colours): from
// the caster's body, 128 motes per target scatter every way, and from 30
// frames on each is pulled toward the middle of its target's body, vanishing
// once it has crossed it. The background sways sideways, swelling and dying
// away over the effect. In mode 3 every mote that arrives also pushes its
// target back.
void Func_08144628(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 *camera;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the mote picture, the mode's palette, and the max and additive
  // blitters for the caster's side.
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_MemberBurstImage, work->sheet, 0,
                             0);
  if (mode == 0 || mode == 3)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_MarsDjinnSheet;
  else
    palette = (s32)&ResourceId_MercuryDjinnSheet;
  // The entry held in a local loads before the arguments; as a plain
  // Iwram_CopyWords call, 2 of 511 instructions differ in tla-en.
  {
    const void *source_ = Resource_GetTableEntry(palette);
    __typeof__(Iwram_CopyWords) routine = Iwram_CopyWords;
    routine((void *)BG_PLTT, source_, 128);
  }
  if (mode == 3)
    Func_0815b634();
  BattleEffect_FetchRectangleBlitters(effect->side, draw);

  // The motes start at the caster's body height and scatter every way at up
  // to two pixels a frame.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *source = GetBattleObjectSlotFar(effect->actor)->object;
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    for (i = 0; i != MOTES_PER_TARGET; i++) {
      struct EffectStep *mote = &PARTICLES[k * 128 + i];

      mote->x = source->x;
      mote->y = height;
      mote->z = source->z;
      mote->velocityX = ((Random16() & 255) - 128) << 10;
      mote->velocityY = ((Random16() & 255) - 128) << 10;
      mote->velocityZ = ((Random16() & 255) - 128) << 10;
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg1HBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(146);

  for (frame = 0; frame != Data_08197864[mode * 2 + 1] + effect->count * 20;
       frame++) {
    s32 *row;
    s32 sway;
    s32 scrollX;

    if (frame == 80) {
      if (mode == 0)
        Func_081180e8(134);
      else if (mode == 3)
        Func_081180e8(134);
      else
        Func_081180e8(133);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // BG1 sways line by line, by up to 16 pixels at the effect's midpoint.
    // A negative scroll wraps into the halfword one short of 0x10000.
    /* FAKEMATCH(barrier): an empty asm after the look-at call keeps its
     * arguments loaded in order, r0 before r1; without it, 2 of 511
     * instructions differ in tla-en. */
    __asm__("");
    row = work->bg2xLines;
    sway = Trig_Sin((frame << 15) /
                    (Data_08197864[mode * 2 + 1] + effect->count * 20)) *
               16 >>
           16;
    scrollX = gBgScroll[1].x;
    for (i = 0; i != 160; i++) {
      s32 x = (Trig_Sin((frame + i) << 10) * sway >> 16) + scrollX;

      if (x < 0)
        x += 0xffff;
      *row++ = x | 32 << 16;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 half = BattleUnit_GetEffectHeightFar(effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          Sound_PlayFar(134);
        else if (mode != 3)
          Sound_PlayFar(133);
      }
      if (frame == k * 20 + 70)
        Func_0814cd48(effect->actors[k], 7, 5, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != Data_08197864[mode * 2]; i++) {
          struct EffectStep *mote = &PARTICLES[k * 128 + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * MOTE_CELL, pos.x - 10,
                    pos.y - 16, 20, 32);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);

            // Thirty frames on, the mote is pulled toward the middle of its
            // target's body.
            if (frame > k * 20 + i + 30) {
              s32 x = mote->x;
              s32 dx = (target->x - x) >> 8;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 8;
              s32 targetDistance;
              s32 moteDistance;
              s32 crossed;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              // It has crossed once it is as far out from the middle as its
              // target is, on the target's side.
              targetDistance = target->x < 0 ? -target->x : target->x;
              moteDistance = x;
              /* FAKEMATCH(barrier): an empty asm through moteDistance keeps it
               * a copy apart from x, so the sign test and the negation read the
               * copy; as x < 0 ? -x : x, 4 of 511 instructions differ in
               * tla-en. */
              __asm__("" : "=r"(moteDistance) : "0"(moteDistance));
              if (moteDistance < 0)
                moteDistance = -moteDistance;
              /* FAKEMATCH(dead): each arm stores its test in crossed, never
               * read; as the tests without the stores, 81 of 511 instructions
               * differ in tla-en. */
              if (targetDistance <= moteDistance &&
                  ((x < 0 && (crossed = target->x < 0)) ||
                   (x >= 0 && (crossed = target->x >= 0)))) {
                mote->variant = -1;
                if (mode == 3) {
                  BattleEffect_StartActorMotion(effect->actors[k], 1, 0x20000,
                                                0x20000, 0x20000, i * 8 + 100);
                  if ((i & 1) && frame != 80)
                    Sound_PlayFar(134);
                }
              }
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ArmBg1HBlankDma);
  BattleEffect_EndCanvasLayer();
}

/// Builds the two usual routines that draw onto the 128x128 canvas, clipped,
/// mirrored when flip is nonzero. out[0] keeps the higher colour index (heap
/// slot 26), out[1] adds the colours (slot 47). BattleEffect_LoadBlitter takes
/// each slot's heap offset and the blitter flags.
// The Broken Seal's BattleEffect_FetchRectangleBlitters.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out) {
  // Unused: gives the stack frame its 4 bytes.
  UNUSED u8 unused[4];

  if (flip == 0) {
    u32 heap = 0xc0;
    s32 flags = 19;
    void **slots;

    /* FAKEMATCH(barrier): an empty asm on heap and flags, so both are set
     * before the heap base is shifted and the slot offset built; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 3);
    out[1] = slots[47];
  } else {
    u32 heap = 0xc0;
    s32 flags = 23;
    void **slots;

    /* FAKEMATCH(barrier): the same asm for the mirrored blitters; without it,
     * 2 of 30 instructions differ in tla-en. */
    __asm__("" : "+l"(heap), "+l"(flags));
    slots = (void **)(heap << 18);
    BattleEffect_LoadBlitter(104, flags);
    out[0] = slots[26];
    BattleEffect_LoadBlitter(188, 7);
    out[1] = slots[47];
  }
}

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Revive, Water of Life, Lich (kind 25); Quartz (kind 201).
DEFINE_EFFECT_KIND(BattleEffect_RunRevive, Func_08144b14, 0)

/// Abilities: Dew (kind 209).
DEFINE_EFFECT_KIND(BattleEffect_RunDew, Func_08144b14, 1)

/// Abilities: Spark (kind 255).
DEFINE_EFFECT_KIND(BattleEffect_RunSpark, Func_08144b14, 2)


extern char ResourceId_EarthWallSheet;
extern char ResourceId_CrescentMoonSheet;
extern char ResourceId_VenusDjinnSheet;

extern u8 Data_0819786c[];  // the mote's picture for each step of its sway
extern u16 Data_08197874[]; // the beam's head: three pictures, offsets
extern u8 Data_0819787a[];  // ... their widths
extern u8 Data_0819787d[];  // ... their heights

// The pictures in the work sheet, after the beam (EarthWallSheet).
#define PIC_MOTE 0x2710  // the crescent motes, 24 x 24
#define PIC_DJINN 0x65c0 // the Djinn, 40 x 40
#define MOTES 8          // per target

// Member beam (kinds through three wrappers; the mode picks the Djinn:
// 0 Venus, 1 Mercury, 2 Mars): with a prelude, the Djinn appears, sways and
// rises away; for each target in turn, sixteen frames apart, eight motes
// drift down swaying; from frame 72 a beam of light falls on each target,
// flashing it, and narrows from frame 88.
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct EffectPosition one;
  struct EffectPosition pos;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 102
  // of 652 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the canvas for the Djinn when there is a prelude, then the beam,
  // the motes and the mode's Djinn, each with its palette.
  BattleEffect_StartCanvasLayer(0);
  if (effect->prelude == 1)
    Func_0815585c(effect, (effect->side ^ effect->prelude) << 4 | mode, djinn,
                  djinnSize);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentMoonSheet,
                             work->sheet + PIC_MOTE, 1, 1);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else if (mode == 1)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands at x = 64.
  REG_BG2PA = 0x100;
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 16 + 116; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Sound_PlayFar(212);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinn sways 20 pixels either way and rises from frame 32.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn[0] + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn[1] - 24;

      BattleEffect_FetchRectangleBlitters(effect->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];

    // A mote every eight frames drifts down a pixel a frame, swaying 16
    // pixels, until it is 8 pixels from the ground.
    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 start = k * 16;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = target->x;
      point[1] = 0;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != MOTES; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              pos.x -= sway;
            else
              pos.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            Data_0819786c[mote->variant / 8 & 7] * 576 +
                            PIC_MOTE,
                        pos.x - 12, pos.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The beam: from the top of the screen to the target's feet, drawn in
    // two mirrored halves, six pixels wide until frame 88, then narrowing a
    // pixel every three frames; its head flickers at the feet.
    for (i = 0; i != effect->count; i++) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        Render_ResetTransformState();
        Transform_SetLookAt(camera->eye, camera->pos);
        if (frame == start + 72)
          Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
        if (frame == start + 88)
          Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
        ground[0] = target->x;
        ground[1] = 0;
        ground[2] = target->z;
        EffectPosition_ProjectPoint(ground, &pos);
        pos.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + Data_08197874[pic],
                    pos.x - Data_0819787a[pic], pos.y - Data_0819787d[pic] + 8,
                    Data_0819787a[pic], Data_0819787d[pic]);
            draw[1](canvas, work->sheet + Data_08197874[pic], pos.x,
                    pos.y - Data_0819787d[pic] + 8, Data_0819787a[pic],
                    Data_0819787d[pic]);
          }
          for (k = 0; k != pos.y; k++) {
            draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
            draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
          }
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Mad Spatter (kind 347).
DEFINE_EFFECT_KIND(BattleEffect_RunMadSpatter, Func_0814512c, 4)

/// Abilities: Mud (kind 237).
DEFINE_EFFECT_KIND(BattleEffect_RunMud, Func_0814512c, 7)

/// Abilities: Decompose (kind 350).
DEFINE_EFFECT_KIND(BattleEffect_RunDecompose, Func_0814512c, 5)

/// Abilities: Bone Chiller (kind 309).
DEFINE_EFFECT_KIND(BattleEffect_RunBoneChiller, Func_0814512c, 0)

/// Abilities: Sticky Goo (kind 315).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyGoo, Func_0814512c, 1)

/// Abilities: Poison Ink (kind 340).
DEFINE_EFFECT_KIND(BattleEffect_RunPoisonInk, Func_0814512c, 6)

/// Abilities: Sticky Poison (kind 322).
DEFINE_EFFECT_KIND(BattleEffect_RunStickyPoison, Func_0814512c, 2)

/// Abilities: Rotten Blood (kind 330).
DEFINE_EFFECT_KIND(BattleEffect_RunRottenBlood, Func_0814512c, 3)

/// Abilities: Rising Venom (kind 430).
DEFINE_EFFECT_KIND(BattleEffect_RunRisingVenom, Func_0814512c, 8)

/// Abilities: Formic Acid (kind 453).
DEFINE_EFFECT_KIND(BattleEffect_RunFormicAcid, Func_0814512c, 9)

// The shots live in the map buffer, 16 for each target, and the bursts
// after them, 32 for each target.

#define BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);

extern char ResourceId_GoldShellSheet;
extern char ResourceId_SmokeSheet; // directory row 0x161
extern char ResourceId_BluePalette;
extern char ResourceId_SpiderWebSheet;
extern char ResourceId_LimePalette; // directory row 0x163
extern char ResourceId_GreenPalette;

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];
#define RECORD_SHARDS 0
#define RECORD_SHOT_BLITTER 1
#define RECORD_SHOTS 2
#define RECORD_FRAMES 3
extern u8 Data_081978a8[];  // shard widths
extern u8 Data_081978ba[];  // shard heights
extern u16 Data_081978cc[]; // shard cell offsets

#define SMOKE_SHEET 0x3e80 // the smoke, after the gold shells
#define SHARD_SHEET 0x1e00 // the shards, among the gold shells

// Shell shot (the body of ten kinds; the mode picks the colours and the
// record above the rest): shots arc from the caster to each target, one or
// four per target eight frames apart, turning as they fly; each bursts where
// it lands into smoke or shards, flashing the target. Mode 7 prepares a
// canvas for its prelude first; modes 3 and 5 hand the canvas to a summon
// canvas effect at the end.
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  s32 preludeSize[3];
  s32 preludeOrigin[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  s32 k;
  s32 total;
  struct MotionObject *source;
  s32 *camera;
  u8 *palette;
  s32 height;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the canvas (mode 7 with its prelude), the gold shells and the
  // smoke with their palettes, then the mode's palette.
  if (mode == 7) {
    BattleEffect_StartCanvasLayer(0);
    Func_0815585c(effect, effect->side << 4 | 0x44, preludeOrigin, preludeSize);
    REG_BG2CNT = 0x785;
  } else {
    BattleEffect_StartCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_GoldShellSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SMOKE_SHEET, 1, 1);
  if (mode == 0)
    palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
  else if (mode == 1)
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
  else if (mode == 2)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 8)
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  else if (mode == 3)
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  else if (mode == 4 || mode == 7)
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
  else if (mode == 6)
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  else if (mode == 9)
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  else
    palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

  // The shots leave the caster at the mode's height, each aimed at its
  // target with a little spread in depth.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  source = GetBattleObjectSlotFar(effect->actor)->object;
  if (mode == 7)
    height = 0x280000;
  else if (mode == 8)
    height = 0x140000;
  else
    height = 0x40000;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[k % effect->count])->object;

    for (i = 0; i != 16; i++) {
      struct EffectStep *shot = &PARTICLES[k * 16 + i];
      s32 aimZ;

      shot->x = source->x;
      shot->y = height;
      shot->z = source->z;
      aimZ = target->z + (((Random16() & 31) - 16) << 17);
      shot->velocityX = (target->x - source->x) >> 4;
      shot->velocityY = 0x40000;
      shot->velocityZ = (aimZ - source->z) >> 4;
      shot->variant = 0;
    }
  }
  for (i = 0; i != 256; i++)
    BURSTS[i].variant = -1;

  // The blitter pair for the caster's side; modes 4 and 7 clear the canvas
  // each frame and turn blending off, the others fade it to three quarters.
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  if (mode == 4 || mode == 7) {
    work->transferMode = 1;
    work->transferValue = 0;
    REG_BLDCNT = 0;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = Data_08197880[mode * 4 + RECORD_FRAMES] + effect->count * 8;
  Sound_PlayFar(103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;
    struct EffectStep *piece;

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // The shots: each starts four frames after the last, flies under
    // gravity and turns through ten pictures; at the ground it bursts.
    for (k = 0; k != effect->count; k++) {
      for (i = 0; i != Data_08197880[mode * 4 + RECORD_SHOTS]; i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct EffectStep *shot = &PARTICLES[k * 16 + i];

          if (frame == start + 17) {
            Func_0814cd48(effect->actors[k % effect->count], 7, 5, k, 16);
            Func_081180e8(133);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint((s32 *)shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 16,
                  position.y - 12, 32, 24);
            else
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 12,
                  position.y - 16, 24, 32);
            if (shot->variant == 0)
              BattleEffectKernels_IntegrateVector3(shot, 63, -0x8000);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
                count = 4;
              else
                count = 16;
              for (n = 0; n != count; n++) {
                piece = &BURSTS[(k * 4 + i) * 8 + n];
                piece->x = shot->x;
                piece->y = shot->y;
                piece->z = shot->z;
                if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
                  piece->velocityX = ((Random16() & 63) - 32) << 11;
                  piece->velocityY = 0;
                  piece->velocityZ = ((Random16() & 63) - 32) << 11;
                } else {
                  piece->velocityX = ((Random16() & 63) - 32) << 13;
                  piece->velocityY = ((Random16() & 31) + 32) << 12;
                  piece->velocityZ = ((Random16() & 63) - 32) << 13;
                }
                piece->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move. Each lives 45 frames.
    for (i = 0, burst = BURSTS; i != 256; burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint((s32 *)burst, &position);
        position.x >>= 1;
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
          draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + SMOKE_SHEET,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = effect->side;
          draw[burst->velocityX > 0 ? flip ^ 1 : flip](
              canvas, work->sheet + Data_081978cc[picture] + SHARD_SHEET,
              position.x - Data_081978a8[picture] / 2,
              position.y - Data_081978ba[picture] / 2, Data_081978a8[picture],
              Data_081978ba[picture]);
        }
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
          BattleEffectKernels_IntegrateVector3(burst, 62, 0x800);
        else
          BattleEffectKernels_IntegrateVector3(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);

  // Modes 3 and 5 clear both canvases and go on into a summon canvas effect.
  if (mode == 3 || mode == 5) {
    /* FAKEMATCH(wrapper): both clears through ClearWords_, which builds the
     * size anew for each; as direct Iwram_ClearWords calls, 16 of 718
     * instructions differ in tla-en. */
    ClearWords_((void *)0x06004000, 0x3c00);
    ClearWords_(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    if (mode == 3)
      SummonEffect_RunCanvasMode1(effect);
    else
      SummonEffect_RunCanvasMode3(effect);
  } else {
    BattleEffect_EndCanvasLayer();
  }
}

extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern u8 Data_081978f0[]; // each panel's x, by side (7 a side)
extern u8 Data_081978fe[]; // each panel's y

// The seven panels of the counter picture: the first two in the work sheet,
// the rest in the map buffer.
#define PANEL_B 0x15d2
#define PANEL_C 0x3081
#define PANEL_D 0x0000
#define PANEL_E 0x2d80
#define PANEL_F 0x4b00
#define PANEL_G 0x58d2

// Counter reveal (kind 40): a shimmering image circles in, fades out and in
// again twice as it closes on the target, then splits into its panels one
// after another with a white flash at frame 72, when the target is thrown
// and the screen shakes. The background waves while the image fades.
void BattleEffect_RunCondemn(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  // Unused: the second position gives the stack frame its 72 bytes, as in
  // The Broken Seal's twin (one position: 4 of 549 differ, 2026-11-08).
  struct EffectPosition pos[2];
  s32 curve[2];
  s32 visible;
  s32 column;
  s32 row;
  s32 zero;
  u8 *cells;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // The zero is held in r8 through the setup calls, as in The Broken Seal's
  // twin (the literal 0 stored directly: 81 of 549 differ, 2026-11-08).
  zero = 0;

  // Set up: half-and-half blending, the image with its palette, the crescent
  // panels into the map buffer, two transparent blitters (the second
  // flipped), the wave's HBlank DMA and a transfer that clears the canvas.
  BattleEffect_StartTiledCanvas(0x2000);
  REG_BG2PA = 0x100;
  cells = gMapCellBuffer;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, cells, 1, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 1;
  work->transferValue = zero;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The image starts off to the side, above the field.
  visible = 1;
  if (effect->side == 1)
    curve[0] = -0x500000;
  else
    curve[0] = 0x700000;
  curve[1] = -0x200000;

  for (frame = 0; frame != 104; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 amp;
    s32 rowBase;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame << 9;
    screenX = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
    screenY = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;

    // It jumps closer at frame 24 and onto the target at frame 48, hidden
    // for the frame of each jump.
    if (frame == 24) {
      if (effect->side == 1)
        curve[0] = -0x200000;
      else
        curve[0] = 0x480000;
      curve[1] = 0x180000;
      visible = 0;
    }
    if (frame == 25) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }
    if (frame == 48) {
      EffectPosition_ProjectScaledBattler(effect->actors[0], pos);
      if (effect->side == 1)
        curve[0] = (pos[0].x - 128) << 16;
      else
        curve[0] = (pos[0].x - 64) << 16;
      curve[1] = 0;
      visible = 0;
    }
    if (frame == 49) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }

    // Before each jump the image fades out over eight frames while the
    // background waves harder.
    scanline = work->bg2xLines;
    amp = 0;
    if (frame <= 23) {
      if (frame > 15) {
        amp = (frame * 2) - 32;
        REG_BLDALPHA = (31 - frame) | 0x1000;
      }
    } else if (frame <= 47) {
      if (frame > 31) {
        amp = (frame * 2) - 64;
        REG_BLDALPHA = (47 - frame) | 0x1000;
      }
    }
    if (amp < 0)
      amp = 0;

    i = 0;
    rowBase = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = rowBase - ((Trig_Sin(angle) * amp) >> 10);
      angle += 0x800;
    }

    // The image, then from frame 72 its panels, each shown for two frames.
    if (visible != 0) {
      s32 side = effect->side;

      if (side == 0) {
        column = 0;
        row = 0;
      } else {
        column = 1;
        /* FAKEMATCH(dead): i is 160 here, so i >> 31 folds to 0 and only
         * changes allocation; as row = 0, 477 of 549 instructions differ in
         * tla-en. */
        row = i >> 31;
        /* FAKEMATCH(dead): side = 0, never read, keeps the side's copy from
         * replacing the draws' index, so the test and the draws share one
         * register; without it, 31 of 549 instructions differ in tla-en. */
        side = 0;
      }
      if (frame <= 71) {
        draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                           Data_081978fe[row * 7] + screenY, 57, 98);
      } else {
        if (frame <= 75)
          draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                             Data_081978fe[row * 7] + screenY, 57, 98);
        draw[effect->side](canvas, work->sheet + PANEL_B,
                           Data_081978f0[column * 7 + 1],
                           Data_081978fe[row * 7 + 1] + screenY, 99, 69);
        if ((u32)(frame - 72) <= 1)
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if ((u32)(frame - 74) <= 1)
          draw[effect->side](canvas, work->sheet + PANEL_C,
                             Data_081978f0[column * 7 + 2],
                             Data_081978fe[row * 7 + 2] + screenY, 128, 91);
        if ((u32)(frame - 76) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_D,
                             Data_081978f0[column * 7 + 3],
                             Data_081978fe[row * 7 + 3] + screenY, 128, 91);
        if ((u32)(frame - 78) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_E,
                             Data_081978f0[column * 7 + 4],
                             Data_081978fe[row * 7 + 4] + screenY, 128, 59);
        if ((u32)(frame - 80) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_F,
                             Data_081978f0[column * 7 + 5],
                             Data_081978fe[row * 7 + 5] + screenY, 122, 29);
        if ((u32)(frame - 82) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_G,
                             Data_081978f0[column * 7 + 6],
                             Data_081978fe[row * 7 + 6] + screenY, 76, 25);
      }
    }

    // The strike: a cue at 68; at 72 the target is thrown, the screen
    // shakes and the results begin.
    if (frame == 68)
      Sound_PlayFar(212);
    if (frame == 72) {
      Func_08118088(effect->actors[0], 0);
      work->shakeFrames = 8;
      Func_081180e8(144);
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  // The battle's own background (its id leads unk670) comes back.
  BattleBackground_LoadFar(1, *(u16 *)Ram_HeapSlots->battleWork->unk670, 24);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define PIXEL_BUFFER Ram_MapCellBuffer

/// Turns the background canvas into a blue ramp, row by row from the top, each
/// column starting after its own random delay.
void Graphics_ConvertBackgroundToBlueRamp(void) {
  s32 i, x, y, row, end, speed;
  s32 red, green, blue;
  u16 color;
  u8 *delays, *p, *pixel;
  u16 *palette;
  u32 delayMask;

  palette = (u16 *)0x05000040;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  delays = Ram_HeapSlots->unk64[0];
  end = 0;
  speed = 16;
  /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, which loads the
   * routine's entry before the arguments, as the native does at all three
   * copies; as direct Iwram_CopyWords calls, 8 of 181 instructions differ in
   * tla-en. */
  CopyWords_(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
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
            pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) +
                                    ((y / 8) << 11));
            color = ((s16 *)0x05000000)[*pixel];
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
    CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
    wait(1);
    if (end > 248)
      break;
    i++;
  } while (i != 27);
  palette = (u16 *)0x050000c0;
  i = 0;
  do {
    s32 half;

    half = i / 2;
    *palette++ = RGB(half, half, i);
    i++;
  } while (i != 32);
  {
    u8 *buffer = PIXEL_BUFFER;

    i = 0;
    do {
      *buffer++ += 64;
      i++;
    } while (i != 0x7800);
  }
  CopyWords_((void *)0x06008000, PIXEL_BUFFER, 0x7800);
  wait(1);
}

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase,
                   s32 bluePhase); // Graphics_UpdatePhasePalette
void Func_080151ac(s32 *scale);    // SceneTransform_ApplyScale

extern char ResourceId_RuneSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_JupiterDjinnSheet;
extern s32 Data_0819790c[]; // the triangle's first corner: (0, 24, 0)
extern u16 Data_08197424[]; // flare sizes 1 to 5: cell offsets
#define FLARE_SHEET 0x1000  // the flares, after the rune picture
#define DJINN_SHEET 0x2000  // the Jupiter Djinn, after the flares
#define DJINN_SIZE 40

#define CORNERS 3
#define EDGE_STEPS 24 // flares drawn along each edge

// Spinning triangle (kinds 38 and 218): as BattleEffect_RunTriangleStrike, a
// triangle of flares spins down onto the target and leaves the rune picture
// where it lands; when the effect has a prelude, the Jupiter Djinn also circles
// above its place for the first frames and rises away after frame 32.
void BattleEffect_RunBind(struct BattleEffectArgument *effect) {
  s32 origin[3];
  s32 originSize[3];
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *motion;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 28
  // of 518 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Djinn with its palette, and a transfer that fades the canvas by 4 each
  // frame.
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    Func_0815585c(effect, effect->side << 4 | 3, origin, originSize);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                             work->sheet + DJINN_SHEET, 1, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // The prelude: the Djinn circles where the effect was prepared, and
    // rises away after frame 32; its blitters live for this frame only.
    if (effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE, DJINN_SIZE);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE,
                DJINN_SIZE);
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      motion = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = motion->x;
        vector[1] = motion->y;
        vector[2] = motion->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - memberFrame) << 9);
              SceneTransform_ApplyYaw((64 - memberFrame) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_0819790c,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

extern s32 Data_08197918[]; // the triangle's first corner: (0, 24, 0)

// Triangle strike (kind 67): a triangle of flares spins down onto the
// target, shrinking, for 64 frames; then the rune picture is stamped in two
// halves where it landed. The caster approaches the target at frame 46, the
// palette pulses throughout and the trail fades a step every 16 frames.
void BattleEffect_RunTriangleStrike(struct BattleEffectArgument *effect) {
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 memberFrame;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  // heap holds the heap slots from here for the second blitter's read, so the
  // 0x03000000 base is built again there; as Ram_HeapSlots at that read, 32
  // of 428 instructions differ in tla-en.
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    if (frame == 46)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      memberFrame = frame - member * 8;
      if (memberFrame >= 0 && memberFrame < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (memberFrame <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (memberFrame <= 63) {
              scale[0] = 0x2a000 - (memberFrame * 3 << 9);
              scale[1] = 0x2a000 - (memberFrame * 3 << 9);
              scale[2] = 0x2a000 - (memberFrame * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - memberFrame) << 9);
              SceneTransform_ApplyYaw((64 - memberFrame) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_08197918,
                                        (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - memberFrame / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (memberFrame > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#include "actor.h"

// The camera's heading, as the camera work holds it.
struct CameraView {
  u8 unk00[0x36];
  u16 yaw;
};

extern s32 gData_08197924[]; // the two ring points, alternating

#define BOLT_COUNT 3
#define RING_POINTS 10
#define RING_STEPS 16 // dots drawn between two ring points
#define FAR_DEPTH 399999

#define ACTOR(id) ((struct Actor *)GetBattleObjectSlotFar(id)->object)

// Three rings of light fly one after another from the caster to the target,
// each growing as it spins, while the camera swings round; each ring that
// lands throws the target back, and the last begins the results.
void BattleEffect_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition base;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct Actor *caster;
  struct Actor *source;
  struct Actor *target;
  struct EffectStep *point;
  struct EffectStep *bolt;
  s32 start;
  s32 nearest;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  caster = ACTOR(effect->actor);

  // Set up: the rune palette, the dot sheet, the caster's casting pose, one
  // max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Actor_SetMode(caster, 2);
  Actor_SetAnimationSpeedFar(caster, 48);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  work->transferValue = 75;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rings leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = ACTOR(effect->actor);
  target = ACTOR(effect->actors[0]);
  for (i = 0, point = work->particles; i != BOLT_COUNT; i++) {
    point->x = source->x;
    point->y = source->y + 0x280000;
    point->z = source->z;
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 60; frame++) {
    // The camera turns toward the caster's side, slowing from frame 40.
    if (frame <= 47) {
      struct CameraView *view = (struct CameraView *)camera;
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }

    // A new ring every twelve frames: it spins and grows from a dot to
    // full size, and is drawn while it is near enough to the camera.
    for (i = 0; i != BOLT_COUNT; i++) {
      start = i * 12;
      if (frame >= start) {
        bolt = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        SceneTransform_ApplyPosition(bolt);
        nearest = 0;
        for (j = 0; j != RING_POINTS; j++) {
          s32 depth;

          Transform_Push();
          Func_080150e4((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale[0] = ((frame - start) << 12) + 0x1000;
          if (scale[0] > 0x10000)
            scale[0] = 0x10000;
          scale[1] = scale[0];
          scale[2] = scale[0];
          Func_080151ac(scale);
          Func_080150e4(j * 0x199a);
          depth =
              EffectPosition_ProjectPoint(&gData_08197924[(j & 1) * 3], &pos);
          if (nearest < depth)
            nearest = depth;
          pos.x >>= 1;
          // The first two stores read base, which nothing sets, and are
          // overwritten at once; both games' code keeps them.
          ring[i * 10 + j].velocityX = pos.x + base.x;
          ring[i * 10 + j].velocityY = pos.y + base.y;
          ring[i * 10 + j].velocityX = pos.x;
          ring[i * 10 + j].velocityY = pos.y;
          Transform_Pop();
        }
        if (nearest <= FAR_DEPTH) {
          for (j = 0; j != RING_POINTS; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != RING_STEPS; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              draw[0](canvas, graphics + gData_08197410[size - 1], x - size / 2,
                      y - size, size, size * 2);
            }
          }
        }
        bolt->x += bolt->velocityX;
        bolt->y += bolt->velocityY;
        bolt->z += bolt->velocityZ;

        // Ten frames after it starts (plus one per ring) the ring lands: the
        // target is thrown back 40 pixels and up, and flashes; the third ring
        // begins the results instead.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->gravity = 0xab85;
          target->actionFlags = 0;
          Actor_ResetMotion(target);
          if (target->x < 0)
            Actor_SetTargetPosition(target, target->x - 0x280000, 0, target->z);
          else
            Actor_SetTargetPosition(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            Func_081180e8(134);
          } else {
            Sound_PlayFar(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef RING_STEPS

// Shining Star's rings of stars.

extern u8 gData_0819793c[]; // ring radii, alternating: 32, 16

#define STAR_COUNT 8

#define RING_STEPS 12 // dots drawn between two ring points
#define DOT_SIZE 2
#define GROUND_Y 0x1dffff

/// Eight rings of stars fly from the caster to the target, one every eight
/// frames and spinning as they go; each bounces where it meets the ground,
/// shaking the screen and flashing the targets.
void BattleEffect_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *star;
  struct EffectPosition pos;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the rune palette, the dot sheet, one max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0, point = work->particles; i != STAR_COUNT; i++) {
    s32 delay = i * 8;

    point->x = source->x / 2;
    point->y = source->y + 0x780000;
    point->z = source->z;
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = DOT_SIZE;
  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 128; frame++) {
    Func_081496c8(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      Func_081180e8(134);
    for (i = 0; i != STAR_COUNT; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        // Project the star and, while it is on screen, lay out its ring of
        // ten points (the velocity fields of the map buffer's steps), then
        // join each point to the next with a row of dots.
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        if (pos.x >= -8 && pos.x <= 127) {
          if (pos.y <= 127) {
            if (pos.y >= -8) {
              for (j = 0; j != RING_POINTS; j++) {
                ring[i * 10 + j].velocityX =
                    pos.x +
                    ((Trig_Sin(j * 0x199a - ((frame - star->variant) << 11)) *
                      gData_0819793c[j & 1]) /
                         2 >>
                     16);
                ring[i * 10 + j].velocityY =
                    pos.y -
                    (Trig_Cos(j * 0x199a - ((frame - star->variant) << 11)) *
                         gData_0819793c[j & 1] >>
                     16);
              }
              for (j = 0; j != RING_POINTS; j++) {
                struct EffectStep *from = &ring[j + i * 10];
                struct EffectStep *to = &ring[i * 10 + (j + 1) % 10];

                for (k = 0; k != RING_STEPS; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  draw[0](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
                }
              }
            }
          }
        }

        // At the ground the star bounces up at half its sideways speed.
        if (star->y <= GROUND_Y) {
          star->velocityY = -star->velocityY;
          star->velocityX /= 2;
          star->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlayFar(134);
          for (j = 0; j != effect->count; j++)
            Func_0814cd48(effect->actors[j], 7, 5, j, 8);
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Summon 8 (BattleEffect_RunSceneEffect's case 8). Twin: The Broken Seal's
// BattleEffect_RunProcne, ported by trial 2 and rewritten here. The
// Lost Age's changes: BattleEffect_FetchRectangleBlitters for the pair; the
// whirlwind unpacked with its palette by Resource_LoadAndDecompress into the
// work sheet, the comets into work->sheet + 0x3600; frame / 4 % 3 written
// through Math_UnsignedMulHighAdd(x, 0x55555556, -3) and the depth divisions by
// 36 and 90 through Math_UnsignedMulHigh with their reciprocals (calls the
// source makes: agscc emits __modsi3 and __divsi3 for the operators);
// Iwram_TransformVector at the comet sparks; formation resource 646; the effect
// argument for the actor loops; gCameraSceneParameters declared unsized (with
// the shared u32[3], word 3 folds into the address).

// What fixed the frame and slots: screen.y >> 16 and screen.depth >>= 16 as the
// twin writes them (combine makes the ldrsh off the pointer to screen; a cast
// to s16 * gave screen + 6 its own register and two spills); the twin's
// declaration order; wave and drift in a block opened after the blitter fetch,
// whose spill slots fall below the pointer to draw.

void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay

s32 ArcTan2(s32 x, s32 y);
u32 Math_UnsignedMulHigh(u32 value, u32 magic);
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

struct Point2D {
  s32 x;
  s32 y;
};

struct Scale {
  s32 x;
  s32 y;
};

extern char ResourceId_WhirlwindSheet;

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_VioletCometSheet;

extern struct Scale Data_08196df8; // the comets' unit scale
extern u8 Data_0819793e[];         // the nine comets' grid: x
extern u8 Data_08197947[];         // ... and depth
extern u16 Data_08197950[];        // the four comet strikes' frames
extern u8 Data_08197958[];         // a strike's three pictures: widths
extern u8 Data_0819795b[];         // ... heights
extern u8 Data_0819795e[];         // ... raise above the mark
extern u16 Data_08197962[];        // ... cell offsets

// frame / 4 % 3, as the source writes it: the multiply-high routine with the
// reciprocal of 3, where a plain % calls __modsi3.
#define MOD3(v) Math_UnsignedMulHighAdd((v), 0x55555556, -3)

#define WHIRLWIND_CEL (48 * 64) // one of the whirlwind's three frames
#define DUST_SHEET 0x2400       // the dust motes, after the whirlwind
#define COMET_SHEET 0x3600      // the violet comets
#define COMETS 9
#define FORMATION_RESOURCE 646

// Summon 8: a whirlwind rises under the enemies and lifts them, spinning,
// while dust motes swirl up through it (frames 0 to 159; A or B skips to
// 160). Then the sky turns violet and a formation of nine comets wheels
// in; a cloud of sparks gathers on its centre, and from frame 330 four
// comets strike one after another, each with a burst of sparks, throwing
// the enemies down. A or B skips frames 161 to 394.
void BattleEffect_RunProcne(struct BattleEffectArgument *effect) {
  void *canvas;
  void *camera;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *graphics;
  struct MotionObject *object;
  struct BattleObj *context;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *mark;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  s32 vector[3];
  struct EffectPosition screen;
  s32 shake[3];
  s32 place[4];
  struct Scale scale;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: the max and additive pair, the whirlwind with its palette, the
  // dots, and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    struct Point2D wave;
    struct Point2D drift;
    Resource_LoadAndDecompress((s32)&ResourceId_WhirlwindSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
    work->transferMode = 2;
    work->transferValue = 50;
    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    wave.x = 0x800000;
    wave.y = 0x280000;
    drift.x = 0;
    drift.y = 0;

    // Each enemy's place, heading, distance and facing, to restore at the
    // end; its bobbing stops.
    for (index = 0; index != effect->count; index++) {
      context = GetBattleObjectSlotFar(effect->actors[index]);
      object = context->object;
      home[index].x = object->x;
      home[index].y = object->z;
      baseAngle[index] = object->angle;
      heading[index] = (u16)ArcTan2(object->x, object->z);
      radius[index] = Iwram_Sqrt((object->x >> 8) * (object->x >> 8) +
                                 (object->z >> 8) * (object->z >> 8)) >>
                      7;
      spin[index] = 0;
      object->verticalMotionStrength = 0;
    }

    // The dust motes start at the whirlwind's foot, each after its own wait.
    index = 0;
    do {
      point = &work->particles[index];
      point->z = 120;
      point->y = 0;
      Random16();
      point->velocityY = 0;
      point->velocityX = 0;
      point->variant = Random16() & 63;
      index++;
    } while (index != 64);

    frame = 0;
    do {
      if ((gInput.repeat & 3) != 0) {
        if (frame <= 159)
          frame = 160;
        else if (frame <= 394)
          frame = 395;
      }

      Render_ResetTransformState();
      Transform_SetLookAt(camera, (s32 *)((u8 *)camera + 12));

      if (frame == 16)
        Sound_PlayFar(141);
      if (frame == 256)
        Sound_PlayFar(140);
      if (frame == 334)
        Sound_PlayFar(212);
      if (frame == 347)
        Sound_PlayFar(212);
      if (frame == 359)
        Sound_PlayFar(212);
      if (frame == 372)
        Sound_PlayFar(212);

      // The enemies, one every 16 frames, spiral in toward the centre and
      // rise, spinning faster; each comet strike knocks them down.
      for (index = 0; index != effect->count; index++) {
        if (frame > index * 16) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
          object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

          if (frame <= 159) {
            if (frame > index * 16 + 16)
              spin[index] += 48;
            if (radius[index] <= 31)
              object->y += 0x60000;
            else
              object->y += 0x8000;
            if (object->y > 0x7c0000)
              object->y = 0x7c0000;
          }

          if (frame <= 490) {
            heading[index] += spin[index];
            if (heading[index] > 0x10000)
              heading[index] -= 0x10000;
            object->angle += spin[index] / 4;
          }

          if (frame == 395)
            object->verticalMotionStrength = 0xab85;

          slot = 0;
          do {
            if (frame == Data_08197950[slot]) {
              object->velocityY = 0;
              Func_0814cd48(effect->actors[index], 7, 5, index, 8);
            }
            slot++;
          } while (slot != 4);

          if (radius[index] > 16)
            radius[index] -= 2;
        }
      }

      // The whirlwind: its foot grows from frame 16, its top from 48, and
      // from 160 it stands whole, two cels high.
      if ((u32)(frame - 16) <= 143) {
        size = frame * 2 - 32;
        if (size > 48)
          size = 48;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (48 - size) * 48],
            32, 112 - size, 48, size);
      }
      if ((u32)(frame - 48) <= 111) {
        size = frame * 2 - 96;
        if (size > 64)
          size = 64;
        draw[0](
            canvas,
            &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL + (64 - size) * 48],
            32, 64 - size, 48, size);
      }
      if ((u32)(frame - 160) <= 239) {
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 0, 48,
                64);
        draw[0](canvas, &work->sheet[MOD3(frame / 4) * WHIRLWIND_CEL], 8, 64,
                48, 64);
      }

      // The dust motes swirl up the whirlwind, speeding up, and start again
      // from the foot when they pass the top.
      if (frame <= 159) {
        index = 0;
        point = work->particles;
        do {
          if (point[index].variant == 0) {
            vector[0] = point[index].z * Trig_Sin(point[index].velocityY);
            vector[1] = point[index].y;
            vector[2] = point[index].z * Trig_Cos(point[index].velocityY);
            EffectPosition_ProjectPoint(vector, &screen);
            screen.x >>= 1;
            if (point[index].y <= 0x3fffff)
              draw[0](canvas, work->sheet + DUST_SHEET, screen.x - 12,
                      screen.y - 12, 16, 16);
            if (point[index].z > 24)
              point[index].z -= 4;
            point[index].velocityY += point[index].velocityX * 2;
            if (point[index].velocityY > 0x10000)
              point[index].velocityY -= 0x10000;
            point[index].velocityX += 50;
            if (point[index].velocityX > 4096)
              point[index].velocityX = 4096;
            point[index].velocityY += 2048;
            point[index].y += point[index].velocityY * 2;
            if (point[index].y > 0x300000) {
              point[index].y = 0;
              point[index].z = 100;
              point[index].velocityY = 0;
              point[index].velocityX = 0;
            }
          } else {
            point[index].variant--;
          }
          index++;
        } while (index != 24);
      }

      // Frame 160: the violet sky, the formation of comets, sparks scattered
      // round its centre, and the enemies dropped back at random heights.
      if (frame == 160) {
        s32 speed;
        s32 angle;

        BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
        BattleStaging_ConfigurePaletteFadeFar(
            1, (s32)&ResourceId_VioletSkyBackdrop, 8);
        BattleEffect_SelectLivingTargets(work->effect);
        BattleEffect_SpawnObjects(COMETS, FORMATION_RESOURCE, 2);
        Resource_LoadAndDecompress((s32)&ResourceId_VioletCometSheet,
                                   work->sheet + COMET_SHEET, 1, 1);

        index = 0;
        do {
          PARTICLES[index].variant = 0;
          index++;
        } while (index != 512);

        for (index = 0; index != effect->count; index++) {
          context = GetBattleObjectSlotFar(effect->actors[index]);
          object = context->object;
          object->y = ((Random16() & 15) + 8) << 16;
        }

        gCameraSceneParameters[3] = 72;

        index = 0;
        do {
          speed = Random16() & 127;
          angle = (Random16() & 0x7fff) + 0x9fff;
          work->particles[index].x = Trig_Sin(angle) * speed;
          work->particles[index].y = Trig_Cos(angle) * speed;
          work->particles[index].z = (Random16() % 200 - 100) << 16;
          work->particles[index].variant = 0;
          index++;
        } while (index != 32);

        for (index = 0; index != effect->count; index++) {
          radius[index] = 16;
          spin[index] = 6000;
        }
      }

      if (frame > 159) {
        // The formation sways on a slow wave, pushed back by each strike.
        scale = Data_08196df8;
        size = 16;
        if (frame - 160 <= 64)
          size = 32;
        wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
        wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
        wave.x += drift.x;
        wave.y += drift.y;
        drift.x /= 2;
        drift.y /= 2;

        scale.x = 0x10000;
        scale.y = 0x10000;
        place[3] = 0;
        index = 0;
        do {
          place[0] = (Data_0819793e[index] << 16) + wave.x + 0x280000;
          place[1] = 0xff0000 + index * 0x400000;
          place[2] = ((Data_08197947[index] + index * 64) << 16) + wave.y;
          Render_ApplyProjectedPlacementFar(
              (struct ScaledSprite *)work->objects[index], place, &scale.x, 0);
          index++;
        } while (index != COMETS);

        if (frame > 255) {
          s32 threshold;

          shake[2] = 0x1000000;
          alive = 0;
          shake[0] = 0;
          shake[1] = 0;
          Render_ResetTransformState();
          SceneTransform_ApplyPosition(shake);

          // Until frame 330 the scattered sparks gather on the centre, each
          // a part of its distance a frame; those that have arrived make one
          // spark that grows with their number.
          if (frame <= 329) {
            index = 0;
            do {
              s32 xx;
              s32 yy;
              s32 zz;

              point = &work->particles[index];
              xx = (point->x >> 8) * (point->x >> 8);
              yy = (point->y >> 8) * (point->y >> 8);
              zz = (point->z >> 8) * (point->z >> 8);
              dist = Iwram_Sqrt(xx + yy + zz) >> 9;
              if (dist != 0) {
                EffectPosition_ProjectPoint(&point->x, &screen);
                screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
                screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 6 - Math_UnsignedMulHigh(screen.depth - 170, 0x071c71c8);
                draw[1](canvas, graphics + gData_08197410[span - 1],
                        screen.x - span / 2, screen.y - span, span, span * 2);
                point->x -= point->x / dist;
                point->y -= point->y / dist;
                point->z -= point->z / dist;
              } else {
                alive++;
              }
              index++;
            } while (index != 32);

            if (alive > 0) {
              span = alive / 10 + 1;
              draw[1](canvas, graphics + gData_08197410[span - 1],
                      (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                      span, span * 2);
            }
          }

          // Each strike throws out 96 sparks that fly and fade.
          slot = 0;
          do {
            if (frame == Data_08197950[slot]) {
              s32 speed;
              s32 angle;

              index = 0;
              do {
                speed = (Random16() & 127) + 16;
                angle = Random16() & 0xffff;
                spark = &PARTICLES[slot * 128 + index];
                spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
                spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
                spark->velocityY = ((Random16() & 255) - 128) << 10;
                spark->x = 0;
                spark->y = 0;
                spark->z = 0;
                spark->variant = (Random16() & 15) + 64;
                index++;
              } while (index != 96);
            }

            if (frame >= Data_08197950[0]) {
              spark = PARTICLES;
              index = 0;
              do {
                if (spark[index].variant > 0) {
                  // The transform in a once-only block, which ends CSE's block
                  // before it; as a plain block, 6 of 1493 instructions differ
                  // in tla-en.
                  do {
                    void (*transform)(const void *, s32 *) =
                        Iwram_TransformVector;
                    /* FAKEMATCH(pin): the result pointer in r1, set before the
                     * point in r0; as a plain argument, 2 of 1493 instructions
                     * differ in tla-en. */
                    register s32 *result __asm__("r1") = &screen.x;
                    transform(&spark[index].x, result);
                  } while (0);
                  screen.x = (screen.x >> 17) + 32;
                  screen.y = (screen.y >> 16) + 56;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gData_08197410[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  spark[index].x += spark[index].velocityX;
                  spark[index].y += spark[index].velocityY;
                  spark[index].z += spark[index].velocityY;
                  spark[index].variant--;
                }
                index++;
              } while (index != 512);
            }
            slot++;
          } while (slot != 4);

          // From frame 330, a comet every twelve frames: it strikes at the
          // formation's centre, the screen shakes and the formation is
          // pushed back; a ring of 28 sparks opens round the mark.
          index = 0;
          do {
            mark = &work->particles[index + 60];
            threshold = index * 12 + 330;
            if (frame == threshold) {
              mark->x = (wave.x >> 17) + 32;
              mark->y = (wave.y >> 16) - 4;
              mark->velocityX = (wave.x >> 17) + 32;
              mark->velocityY = (wave.y >> 16) - 4;

              slot = 0;
              do {
                work->particles[slot + 32].variant = 4;
                slot++;
              } while (slot != 28);

              drift.x = 0x80000;
              drift.y = -0x20000;
              work->shakeFrames = 8;
            }

            if (frame >= threshold) {
              s32 cel;

              cel = (frame - threshold) / 2;
              if (cel > 2)
                cel = 2;
              draw[1](canvas, work->sheet + Data_08197962[cel], mark->x,
                      mark->y - Data_0819795e[cel], Data_08197958[cel],
                      Data_0819795b[cel]);
              mark->x -= 8;
              mark->y += 2;

              if (frame < threshold + 8) {
                Func_080150e4(-2048);
                SceneTransform_ApplyYaw(-4096);

                slot = 0;
                spark = &work->particles[32];
                do {
                  struct EffectStep *ring;

                  ring = &spark[slot];
                  vector[0] = 0;
                  vector[1] = ring->variant * Trig_Cos(slot * 2340);
                  vector[2] = ring->variant * Trig_Sin(slot * 2340);
                  ring->variant += 2;
                  EffectPosition_ProjectPoint(vector, &screen);
                  screen.x =
                      (screen.x >> 17) + work->particles[index + 60].velocityX;
                  screen.y =
                      (screen.y >> 16) + work->particles[index + 60].velocityY;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas, graphics + gData_08197410[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  slot++;
                } while (slot != 28);
              }
            }
            index++;
          } while (index != 4);
        }
      }

      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != 400);

    // The results; the enemies go back where they stood, facing as before.
    Func_081180e8(134);
    for (index = 0; index != effect->count; index++) {
      struct MotionObject *enemy;

      context = GetBattleObjectSlotFar(effect->actors[index]);
      enemy = context->object;
      enemy->x = home[index].x;
      enemy->z = home[index].y;
      enemy->angle = baseAngle[index];
    }
    gCameraSceneParameters[3] = 120;
    Func_0814cca8();

    index = 0;
    do {
      ResourceObject_ReleaseFar(work->objects[index]);
      index++;
    } while (index != COMETS);

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    BattleEffect_EndCanvasLayer();
  }
}

void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Dragon Driver (kind 385).
DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, Func_08147aec, 1)

/// Abilities: Tiamat (scene kind 7).
DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, Func_08147aec, 0)

#undef FORMATION_RESOURCE

// The rising grains live in the map buffer.

// The high halfword of a 16.16 value: its whole pixels.
#define HI(v) (((s16 *)&(v))[1])

void Func_0816442c(s32 formation, s32 x, s32 y); // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y); // ..RunImpactBurst
void Animation_ApplyChildArgumentFar(void *object, s32 animation);

extern char ResourceId_EmberStreakSheet;

extern char ResourceId_LightningBoltSheet; // directory row 0x188
extern u8 Data_08197968[];                 // a grain's added speed, by grain
extern u8 Data_0819796c[];                 // a spark's two pixels
extern u8 Data_0819796e[];                 // a grain's size, by grain
extern u8 Data_08197972[];                 // the streak's six pictures: widths
extern u8 Data_08197978[];                 // ... heights
extern u8 Data_0819797e[];                 // ... drops below the streak
extern u16 Data_08197984[];                // ... cell offsets

#define STREAK_SHEET 0x3000 // the ember streaks, after the flashes
#define FORMATION_RESOURCE 647

// Summon 7 (mode 0; mode 1 is the same scene cast from the caster's side,
// without the formation): streaks rain down and splash, sparks and grains
// rise from where the formation stands, a burst of flashes at frame 128,
// then the screen whitens and the canvas fades away. In mode 0 a formation
// of eight drops in at frame 40 and bursts at the end. A or B skips frames
// 49 to 159.
void Func_08147aec(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition origin;
  DrawRectangle draw[2];
  void *canvas;
  s32 *camera;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive pair, the flashes
  // with their palette, the streaks, the dots, and in mode 1 the lightning
  // palette.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x784;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             work->sheet + STREAK_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 1) {
    const void *lightning =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 1072 instructions differ in tla-en.
    copy((void *)BG_PLTT, lightning, 128);
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    s32 dir;
    s32 alternate;

    // When the first target's animation kind is 2 the formation stands 8
    // pixels further right and lower, with its sparks and flashes.
    alternate = 0;

    if (((struct Actor *)GetBattleObjectSlotFar(effect->actors[0])->object)
            ->animationKind == 2)
      alternate = 1;

    // Mode 1 on the far side mirrors everything sideways, and starts from
    // the caster.
    if (mode == 0)
      dir = 1;
    else if (effect->side == 1)
      dir = -1;
    else
      dir = 1;
    if (mode == 1) {
      EffectPosition_ProjectScaledBattler(effect->actor, &origin);
      origin.x /= 2;
      origin.y = 66;
      if (effect->side == 1)
        origin.x = 76;
      else
        origin.x = 44;
    }

    // The formation enters from above the right edge.
    if (alternate == 1) {
      focus.x = 220 << 16;
      focus.y = -(52 << 16);
    } else {
      focus.x = 212 << 16;
      focus.y = -(60 << 16);
    }
    for (i = 0; i != 64; i++)
      work->particles[i].variant = -1;
    for (i = 0; i != 16; i++) {
      struct EffectStep *streak = &work->particles[24 + i];

      if (dir == 1)
        streak->x = (Random16() & 127) + 128;
      else
        streak->x = (Random16() & 127) - 128;
      streak->y = (Random16() & 7) - 72;
      streak->variant = -(Random16() & 31); // frames before it falls
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].variant = -1;
    if (mode == 0) {
      BattleEffect_SelectLivingTargets(work->effect);
      wait(1);
      BattleEffect_SpawnObjects(8, FORMATION_RESOURCE, 2);
    }

    for (frame = 0; frame != 208; frame++) {
      if ((gInput.repeat & 3) && frame > 48 && frame <= 159) {
        if (mode == 0) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
        frame = 160;
      }
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 178)
        Func_081180e8(134);

      // From frame 128 the trails halve; from 160 to 175 the canvas is filled
      // and the palette brightens; from 176 the canvas fades by 2 a frame.
      if (frame == 128)
        work->transferValue = 50;
      if (frame == 176) {
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        Iwram_CopyWords(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet), 128);
      } else if (frame >= 160 && frame <= 175) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        if (frame > 173)
          work->transferValue = 0x3f3f3f3f;
        Func_08164a4c(2, 2, 2);
      }

      // Grains rise from y = 112: one a frame from 33, eight from 104.
      if (frame >= 33 && frame <= 175) {
        s32 spawned = 0;
        s32 limit = 1;

        if (frame > 103)
          limit = 8;
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant == -1) {
            grain->x = ((Random16() & 255) - 32) << 16;
            grain->y = 112 << 16;
            grain->velocityX = ((Random16() & 127) + Data_08197968[i & 3]) << 9;
            grain->velocityY = -((Random16() & 127) + Data_08197968[i & 3])
                               << 11;
            grain->variant = 0;
            spawned++;
            if (spawned == limit)
              break;
          }
        }
      }

      // Sparks fly up and out from the formation, one every other frame.
      if (frame >= 41 && frame <= 127) {
        s32 spawned = 0;

        if (frame & 1) {
          for (i = 0; i != 24; i++) {
            struct EffectStep *spark = &work->particles[40 + i];

            if (spark->variant == -1) {
              s32 speed = (Random16() & 255) + 128;
              s32 angle = (Random16() & 0x1fff) + 20000;

              if (mode == 0) {
                if (alternate == 1) {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 78 << 16;
                } else {
                  spark->x = ((Random16() & 7) + 78) << 16;
                  spark->y = 70 << 16;
                }
              } else {
                spark->x = ((Random16() & 7) + origin.x - 8) << 16;
                spark->y = origin.y << 16;
              }
              spark->velocityX = speed * Trig_Sin(angle) >> 9;
              spark->velocityY = speed * Trig_Cos(angle) >> 9;
              spark->variant = 0;
              spawned++;
              if (spawned == 1)
                break;
            }
          }
        }
      }
      if (frame == 48)
        Sound_PlayFar(141);
      if (frame == 128)
        Sound_PlayFar(145);

      // Flashes fly out sideways, one a frame from 129.
      if (frame >= 129 && frame <= 175) {
        s32 spawned = 0;

        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) - 20000;

            if (mode == 0) {
              if (alternate == 1) {
                flash->x = 72 << 16;
                flash->y = 72 << 16;
              } else {
                flash->x = 68 << 16;
                flash->y = 64 << 16;
              }
            } else {
              flash->x = origin.x << 16;
              flash->y = origin.y << 16;
            }
            flash->velocityX = speed * Trig_Sin(angle) >> 6;
            flash->velocityY = speed * Trig_Cos(angle) >> 6;
            flash->variant = 0;
            spawned++;
            if (spawned == 1)
              break;
          }
        }
      }

      // A flash steps its picture every four frames and lives 24.
      if (frame <= 175) {
        for (i = 0; i != 24; i++) {
          struct EffectStep *flash = &work->particles[i];

          if (flash->variant >= 0) {
            draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                    HI(flash->x) - 16, HI(flash->y) - 32, 32, 64);
            flash->x += flash->velocityX * dir;
            flash->y += flash->velocityY;
            flash->variant++;
            if (flash->variant == 24)
              flash->variant = -1;
          }
        }
      }

      // A spark is two pixels, speeding upward, and lives 48 frames.
      for (i = 0; i != 24; i++) {
        struct EffectStep *spark = &work->particles[40 + i];
        s32 size = 1;

        if (spark->variant >= 0) {
          draw[1](canvas, Data_0819796c, HI(spark->x), HI(spark->y) - size,
                  size, size * 2);
          spark->x += spark->velocityX * dir;
          spark->y += spark->velocityY;
          spark->velocityY += -0x400;
          spark->variant++;
          if (spark->variant == 48)
            spark->variant = -1;
        }
      }

      // Grains speed upward until frame 128, then bend away sideways (the odd
      // ones harder); each lives 256 frames.
      if (frame <= 175) {
        for (i = 0; i != 512; i++) {
          struct EffectStep *grain = &PARTICLES[i];

          if (grain->variant >= 0) {
            u8 size = Data_0819796e[i & 3];
            s32 height = size * 2;

            draw[0](canvas, sheet + gData_08197410[size - 1],
                    HI(grain->x) - size / 2, HI(grain->y) - size, size, height);
            grain->x += grain->velocityX * dir;
            grain->y += grain->velocityY;
            if (frame > 128) {
              if (i & 1)
                grain->velocityX += -0x8000;
              else
                grain->velocityX += -0x2000;
            } else {
              grain->velocityY += -0x400;
            }
            grain->variant++;
            if (grain->variant == 256)
              grain->variant = -1;
          }
        }
      }

      // The shakes, and the formation: it drops in over frames 40 to 47 and
      // turns to face the targets from 128 to 175.
      if (frame == 128)
        work->shakeFrames = 48;
      if (mode == 0 && frame == 48)
        work->shakeFrames = 8;
      if (frame >= 40 && frame <= 47) {
        focus.x += -(8 << 16);
        focus.y += 16 << 16;
      }
      if (mode == 0) {
        if (frame == 128) {
          Animation_ApplyChildArgumentFar(work->objects[0], 8);
          Animation_ApplyChildArgumentFar(work->objects[1], 9);
          Animation_ApplyChildArgumentFar(work->objects[3], 10);
          Animation_ApplyChildArgumentFar(work->objects[4], 11);
        }
        if (frame == 176) {
          Animation_ApplyChildArgumentFar(work->objects[0], 0);
          Animation_ApplyChildArgumentFar(work->objects[1], 1);
          Animation_ApplyChildArgumentFar(work->objects[3], 3);
          Animation_ApplyChildArgumentFar(work->objects[4], 4);
        }
        if (alternate == 1)
          Func_0816442c(6, focus.x, focus.y);
        else
          Func_0816442c(3, focus.x, focus.y);
      }
      if (frame == 138) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 10, 5, -1, 0);
          Func_08118088(effect->actors[i], 4);
        }
      }

      // A streak falls six pixels a frame after its wait, then below y = 55
      // splashes through six pictures, two frames each, over and over.
      if (frame <= 175) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *streak = &work->particles[24 + i];

          if (streak->y > 55) {
            if (streak->variant >= 0 && streak->variant <= 11) {
              s32 cel = streak->variant / 2;

              draw[0](canvas, work->sheet + Data_08197984[cel] + STREAK_SHEET,
                      streak->x - Data_08197972[cel] / 2,
                      streak->y + Data_0819797e[cel], Data_08197972[cel],
                      Data_08197978[cel]);
            }
            streak->variant++;
            if (streak->variant == 12)
              streak->variant = 0;
          } else if (streak->variant == 0) {
            s32 size = 10;

            streak->x -= dir * 6;
            streak->y += 6;
            if (frame <= 47 && streak->y > 55)
              Sound_PlayFar(136);
            draw[0](canvas, sheet + gData_08197410[size - 1],
                    streak->x - size / 2, streak->y + 30, size, size * 2);
          } else {
            streak->variant++;
          }
        }
      }
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    if (mode == 0) {
      if (alternate == 1)
        Func_0816467c(6, focus.x, focus.y);
      else
        Func_0816467c(3, focus.x, focus.y);
    }
    if (mode == 0) {
      for (i = 0; i != 8; i++)
        ResourceObject_ReleaseFar(work->objects[i]);
    }
  }
  BattleEffect_EndCanvasLayer();
}

#undef FORMATION_RESOURCE

// Nereid, summon 2.

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA; // directory row 0x146

extern u8 Data_08197990[];  // the three columns' feet, x and y
extern s32 Data_08197998[]; // a falling spark's gravity, by spark

#define SPARK_COUNT 1024
#define FORMATION_SIZE 12
#define FORMATION_RESOURCE 649
#define COLUMN_SHEET 0x6e4 // the fire pillar, after the water spray band
#define COLUMN_WIDTH 34
#define COLUMN_HEIGHT 104

/// Summon 2: a formation of twelve sweeps in from the right and settles over
/// the enemies while a band of spray rises from below. At frame 28, and every
/// frame from 32 to 79, sparks burst up from beside the formation and fall
/// back; three columns of fire stream up meanwhile, and every twelve frames
/// the targets flash and jump. After frame 80 the sparks rise away. A or B
/// skips frames 33 to 97.
/// Nereid (summon 2, BattleEffect_RunSceneEffect's case 2), ported call for
/// call from The Broken Seal's BattleEffect_RunNereid. This game reads its heap
/// blocks canvas, work, sheet; loads three resources where The Broken Seal
/// copies a palette and decodes two sheets; takes its blitter pair from
/// BattleEffect_FetchRectangleBlitters and its formation from resource 649; and
/// reads the effect argument in the hit loop.
void BattleEffect_RunNereid(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *graphics;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  graphics = Ram_HeapSlots->unk64[0];

  // Set up: half-and-half blending, the max and additive blitters, the
  // spray band and the fire column into the work sheet, the sparks into the
  // sheet slot, and the transfer at three quarters.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA,
                             work->sheet + COLUMN_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *sparks;
    s32 *life;

    // The formation enters from (256, 88), moving left and up.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }

    // Every spark in the map buffer starts free (variant -1).
    life = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
    for (k = 0; k != SPARK_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / 4;
    }
    sparks = (struct EffectStep *)Ram_MapCellBuffer;
    BattleEffect_SelectLivingTargets(work->effect);
    wait(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION_RESOURCE, 2);

    for (frame = 0; frame != 124; frame++) {
      if ((gInput.repeat & 3) && frame > 32 && frame <= 97)
        frame = 98;
      if (frame == 120)
        Func_081180e8(134);
      if (frame <= 15)
        rise += 2;

      // The formation slows (x 58/64, y 56/64 each frame) and is pushed
      // back right once it passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      Func_0816442c(1, focus.x, focus.y);

      // Sparks burst up from around (32, 96): every free one at frame 28,
      // then sixteen a frame from 32 to 79.
      if (frame == 28) {
        for (k = 0; k != 256; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
          }
        }
      }
      if (frame >= 32 && frame < 80) {
        s32 spawned = 0;

        for (k = 0; k != SPARK_COUNT; k++) {
          spark = &sparks[k];
          if (spark->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            spark->velocityX = ((Random16() & 63) - 32) << 14;
            spark->velocityY = (-(Random16() & 63) - 8) << 13;
            spark->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(164);
      if (frame == 32)
        Sound_PlayFar(145);
      if (frame == 80)
        Sound_PlayFar(144);

      // Three columns of fire scroll up while the sparks burst.
      if (frame >= 32 && frame < 80) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % COLUMN_HEIGHT;

          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span - COLUMN_HEIGHT, COLUMN_WIDTH,
                  COLUMN_HEIGHT);
          draw[0](canvas, work->sheet + COLUMN_SHEET,
                  Data_08197990[k * 2] - COLUMN_WIDTH / 2,
                  Data_08197990[k * 2 + 1] - span, COLUMN_WIDTH, span);
        }
      }

      // The spray band rises sixteen pixels and drifts right.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // Each live spark: rising ones are drawn larger and by the max
      // blitter, falling ones by the additive one; all grow from frame 68.
      // They fall under gravity until frame 80, then accelerate upward, and
      // a falling spark that reaches y = 104 is freed.
      for (k = 0; k != SPARK_COUNT; k++) {
        spark = &sparks[k];
        if (spark->variant >= 0) {
          s32 size = k % 3 + 2;

          if (spark->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[spark->velocityY > 0](
              canvas, graphics + gData_08197410[size - 1],
              HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX;
          spark->y += spark->velocityY;
          if (frame > 80)
            spark->velocityY += -0x8000;
          else
            spark->velocityY += Data_08197998[k & 3];
          spark->velocityX = spark->velocityX * 62 / 64;
          spark->velocityY = spark->velocityY * 62 / 64;
          spark->variant++;
          if (spark->velocityY > 0 && HI(spark->y) > 104)
            spark->variant = -1;
        }
      }

      // From frame 30 to 79 the targets flash and are knocked upward every
      // twelve frames, and stop flashing six frames later.
      if (frame <= 79) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  GetBattleObjectSlotFar(effect->actors[k])->object;

              Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = 0x48000;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    Func_0816467c(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE

// Heap slots The Lost Age addresses by byte offset in Ram_HeapSlots.
#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define MOTE_COUNT 30
#define SEAT_FIRST 32 // the particles that hold each target's seat

extern char ResourceId_VortexSheet;

// ☀️ gVortices_Counts: motes, then frames, for each variant.
extern u8 gData_081979a8[];

void Camera_ApplyPhasedDelta(void);

// Thirty motes rise from the caster, swirl, and home in on the targets; each
// one that reaches the ground flashes and knocks back its target and plays a
// three-cel splash where it landed.
void BattleEffect_RunFroth(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct MotionObject *source;
  struct MotionObject *object;
  struct EffectStep *point;
  struct EffectStep *seat;
  struct EffectStep *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *landed;

  // Set up: the canvas layer, the vortex sheet and two blitters, the one on
  // the right mirrored.
  landed = (s8 *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The motes start at the caster's chest with random velocities.
  source = GetBattleObjectSlotFar(effect->actor)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(effect->actor);
  for (i = 0; i != MOTE_COUNT; i++) {
    point = &work->particles[i];
    point->x = source->x;
    point->y = y;
    point->z = source->z;
    point->velocityX = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->velocityY = ((s32)((Random16() & 127) - 16) << 16) >> 6;
    point->velocityZ = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->variant = -1;
    landed[i] = 0;
  }

  // Each target's seat on the ground.
  for (i = 0; i != effect->count; i++) {
    seat = &work->particles[SEAT_FIRST + i];
    object = GetBattleObjectSlotFar(effect->actors[i])->object;
    seat->x = object->x;
    seat->y = 0;
    seat->z = object->z;
  }

  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(Camera_ApplyPhasedDelta, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(164);

  for (frame = 0; frame != gData_081979a8[effect->variant * 2 + 1]; frame++) {
    // The camera turns between frames 17 and 63.
    camera = Ram_HeapSlots->cameraWork;
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    if (frame == gData_081979a8[effect->variant * 2 + 1] - 16)
      Func_081180e8(133);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    for (i = 0; i != gData_081979a8[effect->variant * 2]; i++) {
      point = &work->particles[i];

      // A mote leaves the caster two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &pos);
        pos.x >>= 1;
        if (pos.depth < 160)
          pos.depth = 160;
        if (pos.depth > 0x31f)
          pos.depth = 0x31f;
        draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }

      // After 48 frames it steers toward its target's seat, damped for the
      // first 37 of them, and lands.
      if (frame > i * 2 + 48 && landed[i] == 0) {
        target = &work->particles[SEAT_FIRST + i % effect->count];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          point->velocityX = point->velocityX * 60 / 64;
          point->velocityY = point->velocityY * 60 / 64;
          point->velocityZ = point->velocityZ * 60 / 64;
        }
        if (point->y < 0) {
          landed[i] = 1;
          point->variant = 0;
          point->x = pos.x;
          point->y = pos.y + (Random16() & 31) - 16;
          Func_0814cd48(effect->actors[i % effect->count], 7, 5,
                        i % effect->count, 4);
          Func_08118088(effect->actors[i % effect->count], 0);
          work->shakeFrames = 4;
          Sound_PlayFar(132);
        }
      }

      // The splash: three cels, two frames each, drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 2, effect->variant * 2 + 2);
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(Camera_ApplyPhasedDelta);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define TORNADO_FRAMES 80
#define GLINT_COUNT 16 // four glints for each of up to four funnels
#define GLINT_CELS 6   // three cels of two frames each

// ☀️ gVortices_Shapes: funnels, sway radius and shake, for each variant.
extern u8 gData_081979ae[];

// One to four funnels of wind sway across the targets for 80 frames, each
// drawn as three stacked segments with glints swirling around it; the canvas
// is wiped every eight frames, and each target is struck seven times.
void BattleEffect_RunWhirlwind(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  struct EffectStep *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  // Set up: the canvas layer, the tornado sheet and two max blitters, the
  // second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The glints start at random places in their funnel, staggered in time.
  for (j = 0; j != GLINT_COUNT; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BLDALPHA = 0x1000;
  wait(1);
  Sound_PlayFar(141);

  for (frame = 0; frame != TORNADO_FRAMES; frame++) {
    wave = Trig_Sin(frame << 10) << 4;
    if (frame == 32)
      Func_081180e8(133);

    // Wipe the canvas every eight frames from frame 16, leaving a trail.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWords(canvas, 0x4000, 0x08080808);
    }

    // Sway toward the targets' side; fade in over 16 frames and out over the
    // last 16.
    if (effect->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    if (frame <= 16)
      REG_BLDALPHA = frame | 0x1000;
    if (frame > 63)
      REG_BLDALPHA = (0x4f - frame) | 0x1000;

    for (i = 0; i != gData_081979ae[effect->variant * 3]; i++) {
      // Each funnel circles its own phase, a quarter turn from the next.
      x = ((gData_081979ae[effect->variant * 3 + 1] *
                Trig_Sin((frame << 11) + i * 0x4000) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
      draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40,
              32);
      draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40,
              32);

      // Its four glints, the second of each pair mirrored; each restarts at
      // a new place when its cels run out.
      for (j = 0; j != 4; j++) {
        glint = &work->particles[i * 4 + j];
        if (glint->variant >= 0) {
          cell = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gData_08197486[cell], glint->x + x,
                      glint->y + y, gData_08197492[cell], gData_08197498[cell]);
        }
        glint->variant++;
        if (glint->variant == GLINT_CELS) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }

    // Strike each target seven times, every eight frames, three frames
    // after the target before it.
    for (j = 0; j != effect->count; j++) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          Func_08118088(effect->actors[j], 6);
        }
      }
    }

    work->shakeFrames = 1;
    BattleCamera_Shake(gData_081979ae[effect->variant * 3 + 2],
                       gData_081979ae[effect->variant * 3 + 2] * 2);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHARDS_PER_CRYSTAL 16
#define GROUND_Y (56 << 16)
#define SHATTERED 1 // a crystal's z once it has hit the ground

extern char ResourceId_BoulderSheet; // directory row 0x14d
extern char ResourceId_CrystalSheet; // directory row 0x193

// ☀️ gPrism_Counts, with a third byte: crystals, frames and the frame
// that begins phase 134, for each variant.
extern u8 Data_081979b7[];
extern s32 gData_08197834[]; // ☀️ gPrism_ShardOffsets
extern u8 gData_0819781a[];  // ☀️ gPrism_ShardWidths
extern u8 gData_08197826[];  // ☀️ gPrism_ShardHeights
extern u8 gData_081977f8[];  // ☀️ gPrism_ShardStarts: x, y for each shard

// Crystals fall one after another, eight frames apart, onto the targets'
// ground and shatter into sixteen spinning shards each; the screen darkens
// over the last eight frames.
void BattleEffect_RunPrism(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *cry;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;
  s32 k;

  // Set up: the canvas layer, the two sheets and the blitter pair.
  shards = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (k = 0; k != Data_081979b7[effect->variant * 3]; k++) {
    seed = &work->particles[k];
    x = Random16() & 31;
    seed->y = -0x400000;
    if (effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = k * 8;
  }

  for (frame = 0; frame != Data_081979b7[effect->variant * 3 + 1]; frame++) {
    // The strongest variant turns the camera, slowing from frame 96.
    if (effect->variant == 2 && frame <= 103) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == Data_081979b7[effect->variant * 3 + 2])
      Func_081180e8(134);

    // Darken over the last eight frames.
    if (frame == Data_081979b7[effect->variant * 3 + 1] - 8) {
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }

    if (frame <= Data_081979b7[effect->variant * 3 + 1] - 8) {
      for (i = 0; i != Data_081979b7[effect->variant * 3]; i++) {
        cry = &work->particles[i];
        if (cry->z == SHATTERED) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
            cell = (j % 5) * 3 +
                   (shards[i * SHARDS_PER_CRYSTAL + j].variant / 96) % 3;
            shard = &shards[i * SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](canvas, work->sheet + gData_08197834[cell] + 0x800,
                         HI(shard->x) - gData_0819781a[cell] / 2,
                         HI(shard->y) - gData_08197826[cell] / 2,
                         gData_0819781a[cell], gData_08197826[cell]);
            BattleEffectKernels_IntegrateVector2(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= cry->variant) {
          // A falling crystal (32x64), until it reaches the ground.
          draw[i & 1](canvas, work->sheet, HI(cry->x) - 16, HI(cry->y), 32, 64);
          BattleEffectKernels_IntegrateVector2(cry, 64, 0x10000);
          if (cry->y > GROUND_Y) {
            cry->z = SHATTERED;
            cry->y = GROUND_Y;
            for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * SHARDS_PER_CRYSTAL + j];
              shard->x = ((gData_081977f8[j * 2] - 40) << 16) + cry->x;
              shard->y = gData_081977f8[j * 2 + 1] << 16;
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            Sound_PlayFar(144);
            for (j = 0; j != effect->count; j++)
              Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          }
        }
      }
    }

    BattleCamera_Shake(effect->variant * 2 + 4, effect->variant * 4 + 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef SPARK_COUNT
#undef GROUND_Y

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase, s32 bluePhase) {
  u16 palette[64];
  s32 phase;
  s32 redOffset;
  s32 greenOffset;
  s32 blueOffset;
  s32 index;

  phase = frame * 0x400;
  redOffset = (Trig_Sin(phase + redPhase) * 16) >> 15;
  greenOffset = (Trig_Sin(phase + greenPhase) * 16) >> 15;
  blueOffset = (Trig_Sin(phase + bluePhase) * 16) >> 15;
  palette[0] = 0;

  index = 1;
  do {
    s32 red;
    s32 green;
    s32 blue;

    red = (redOffset + 16) * index / 64;
    green = (greenOffset + 16) * index / 64;
    blue = (blueOffset + 16) * index / 64;

    if (red < 0)
      red = 0;
    if (red > 31)
      red = 31;
    if (green < 0)
      green = 0;
    if (green > 31)
      green = 31;
    if (blue < 0)
      blue = 0;
    if (blue > 31)
      blue = 31;

    palette[index] = (blue << 10) | (green << 5) | red;
    index++;
  } while (index != 64);

  /* FAKEMATCH(wrapper): the copy through CopyWords_, which builds the palette's
   * address anew for it; as a direct Iwram_CopyWords call, 47 of 96
   * instructions differ in tla-en. */
  CopyWords_((void *)0x05000002, palette, sizeof(palette));
}

#define SPARK_MAX 1024
#define PILLAR_BASE_Y 104

// The camera work, as far as this effect reads it.
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

extern char ResourceId_LightningPillarSheet; // directory row 0x194
extern char ResourceId_VioletPaletteB;       // directory row 0x122

extern u8 Data_081979da[];  // ☀️ gPlasma_Counts
extern u8 Data_081979cc[];  // ☀️ gPlasma_Columns, 7 for each side
extern u16 Data_081979c0[]; // ☀️ gPlasma_Sparks: sparks, shake

// Pillars of lightning strike the targets' row one after another, eight
// frames apart; each flashes the screen, throws a fan of sparks up from its
// base and shakes the camera, and the sparks bounce on the ground.
void BattleEffect_RunPlasma(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *variant;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  // Set up: the canvas layer, an additive blitter and a mirrored max one,
  // the pillar sheet and the particle sprites, and a violet palette for all
  // but the strongest variant.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  if (effect->variant != 2) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 457 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }

  // Every spark starts at rest.
  variant = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
  for (k = 0; k != SPARK_MAX; k++) {
    *variant = 0;
    variant += 7;
  }
  sparks = (struct EffectStep *)Ram_MapCellBuffer;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  count = Data_081979da[effect->variant];

  for (frame = 0; frame != count * 8 - count + 42; frame++) {
    // The strongest variant pans the camera over the first 64 frames.
    if (effect->variant == 2 && frame <= 63) {
      struct CameraWork *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (effect->side == 1)
        camera->scroll -= speed;
      else
        camera->scroll += speed;
    }
    if (frame == 24)
      Func_081180e8(134);

    // Pillar i strikes at frame 8i: a flash, then its first cel for one
    // frame and its second for two.
    for (i = 0; i != count; i++) {
      start = i * 8;
      if (frame == start) {
        Sound_PlayFar(134);
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work->sheet, Data_081979cc[i + effect->side * 7] - 24,
                  0, 48, 112);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + 0x1500,
                  Data_081979cc[i + effect->side * 7] - 24, 0, 48, 112);

        // Throw sparks up from the pillar's base, within 90 degrees of
        // straight up, and shake the camera.
        if (frame == start + 2) {
          s32 thrown = 0;

          for (k = 0; k != SPARK_MAX; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;

              spark->x = Data_081979cc[i + effect->side * 7] << 16;
              spark->y = PILLAR_BASE_Y << 16;
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              thrown++;
              if (thrown == Data_081979c0[effect->variant * 2])
                break;
            }
          }
          work->shakeFrames = Data_081979c0[effect->variant * 2 + 1];
        }
      }

      // Four frames after each strike the targets recoil and flash.
      if (frame == start + 4) {
        for (k = 0; k != effect->count; k++) {
          Func_08118088(effect->actors[k], 1);
          Func_0814cd48(effect->actors[k], 7, 5, k, 8);
        }
      }
    }

    // The sparks fall and bounce at half speed off the ground, drawn
    // smaller as they die out.
    for (k = 0; k != SPARK_MAX; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > PILLAR_BASE_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = spark->x >> 16;
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = spark->y >> 16;
            size = spark->variant / 8 + 1;
            draw[k & 1](canvas, sprites + gData_08197410[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    BattleCamera_Shake(8, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb
/// (kind 15).
DEFINE_EFFECT_KIND(BattleEffect_RunNova, BattleEffect_RunSparkGroups, 0)

/// Abilities: Dynamite, Angry Mine (kind 338).
DEFINE_EFFECT_KIND(BattleEffect_RunDynamite, BattleEffect_RunSparkGroups, 1)

#define FLASH_POINTS 16 // flash points kept for each group
#define FLASH_DRAWN 12  // of which the first 12 are drawn
#define FLASH_FRAMES 18 // six cels of three frames
#define GROUND_Y (108 << 16)

// The kinds of spark group, chosen by the effect's entry.
enum {
  SPARKS_FIXED,     // from a fixed point, canvas at priority 1
  SPARKS_ORANGE,    // from a lower point, orange, with a palette fade
  SPARKS_FROM_ACTOR // from the caster, lightning colours, sparks rising
};

// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];
extern u8 Data_08197a1a[]; // ☀️ gPlasma_FlashCells

#define SHAPE(n) Data_081979de[effect->variant][n]
#define SPARK_COUNT SHAPE(0)
#define GROUP_COUNT SHAPE(1)
#define GROUP_X(i) SHAPE((i) + 2)

// One to three groups of sparks burst one after another, eight frames
// apart: each opens with a big flash, a ring of small flashes and a spray
// of sparks that fall (or, from the caster, rise) and bounce; six frames
// after each burst the targets flash and recoil.
void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect,
                                 s32 kind) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct EffectStep *sparks;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // The burst's origin on the canvas.
  if (kind == SPARKS_FIXED) {
    BattleEffect_StartCanvasLayer(1);
    originX = 60;
    originY = 48;
  } else if (kind == SPARKS_ORANGE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
    work->fadeFrames = 24;
    work->fadeStep = 0;
  } else {
    BattleEffect_StartCanvasLayer(0);
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    originX = pos.x / 2;
    originY = pos.y + 48;
  }

  // The flash sheet, the particle sprites and the kind's palette.
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  if (kind == SPARKS_ORANGE) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 783 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (kind == SPARKS_FROM_ACTOR) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 783 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }

  sparks = gMapCellBuffer;

  for (i = 0; i != GROUP_COUNT; i++) {
    // The group's ring of flash points, wider and later further out.
    for (j = 0; j != FLASH_POINTS; j++) {
      struct EffectStep *point;
      s32 radius;
      s32 angle;

      point = &work->particles[i * FLASH_POINTS + j];
      radius = j * 2;
      angle = Random16() & 0xffff;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }

    // Its sparks, thrown in every direction from the group's centre.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark;
      s32 speed;
      s32 angle;

      spark = sparks + i * SPARK_COUNT + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & 0xffff;
      if (effect->side == 1)
        spark->x = (originX - GROUP_X(i) + 28) << 16;
      else
        spark->x = (originX + GROUP_X(i) - 28) << 16;
      spark->y = originY << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != GROUP_COUNT * 8 + 40; frame++) {
    struct CameraWork *camera = Ram_HeapSlots->cameraWork;

    // The strongest variant pans the camera; the next one wipes the canvas
    // at frame 4.
    if (effect->variant == 2 && frame <= 51) {
      if (effect->side == 0)
        camera->scroll += 256;
      else
        camera->scroll -= 256;
    }
    if (effect->variant == 3 && frame == 4)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);

    if (kind == SPARKS_ORANGE || kind == SPARKS_FROM_ACTOR) {
      if (frame == 2) {
        Sound_PlayFar(145);
        Func_081180e8(145);
      }
    } else {
      if (frame == 2)
        Sound_PlayFar(145);
      if (frame == 24)
        Func_081180e8(134);
    }

    for (i = 0; i != GROUP_COUNT; i++) {
      start = i * 8;
      if (frame == start)
        work->shakeFrames = 12;

      // The big flash, for two frames.
      if (frame >= start && frame < start + 2) {
        if (effect->side == 1)
          draw[0](canvas, work->sheet, originX - GROUP_X(i) + 12, originY - 32,
                  32, 64);
        else
          draw[0](canvas, work->sheet, originX + GROUP_X(i) - 44, originY - 32,
                  32, 64);
      }

      // The ring of small flashes, each counting down through six cels.
      if (frame >= start) {
        for (j = 0; j != FLASH_DRAWN; j++) {
          struct EffectStep *point;
          s32 x;
          s32 y;

          point = &work->particles[i * FLASH_POINTS + j];
          y = HI(point->y) + originY;
          if (effect->side == 1)
            x = HI(point->x) + originX - GROUP_X(i) + 28;
          else
            x = HI(point->x) + originX + GROUP_X(i) - 28;
          if (point->variant >= 0 && point->variant < FLASH_FRAMES)
            draw[0](canvas,
                    work->sheet + (Data_08197a1a[point->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }

      // From the sixth frame the sparks fly, bounce off the ground at half
      // speed, and shrink as they die out.
      if (frame > start + 5) {
        s32 gravity;

        if (kind == SPARKS_FROM_ACTOR)
          gravity = -0x1000;
        else
          gravity = 0x1000;
        for (j = 0; j != SPARK_COUNT; j++) {
          struct EffectStep *spark = sparks + i * SPARK_COUNT + j;
          if (spark->variant > 0) {
            BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
            spark->variant--;
            if (spark->y > GROUND_Y) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
              s16 y = spark->y >> 16;
              s16 x = spark->x >> 16;
              s32 size = spark->variant / 5 + 1;

              draw[j & 1](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      // The targets flash and recoil six frames after the burst.
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 10);
          Func_08118088(effect->actors[j], 4);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define PILLAR_FRAMES 96

extern char ResourceId_RedPaletteB; // directory row 0x147

extern u8 Data_08197a20[];    // ☀️ gVolcano_Counts, for each variant
extern s8 Data_08197a23[][4]; // ☀️ gVolcano_Depths
extern u8 Data_08197a2f[];    // ☀️ gVolcano_StartFrames

// One, two or four pillars of fire rise in front of the targets, one after
// another; each burns for the rest of the first 64 frames, scrolling its
// flames upward, throws a burst of sparks when it starts and a few more
// every frame after its sixteenth, and strikes every target as it starts.
void BattleEffect_RunVolcano(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  // Set up: the canvas layer, the pillar sheet, the particle sprites, the
  // variant's palette and two max blitters, the second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (effect->variant == 0) {
    const void *palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (effect->variant == 2) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  for (i = 0; i != SPARK_MAX; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand in front of the first target, on its side of the
  // field, at the variant's depths.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 4; i++) {
    struct EffectStep *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->x = x;
    base->y = 0;
    base->z = Data_08197a23[effect->variant][i] << 16;
    if (object->x < 0)
      base->x = -x;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != PILLAR_FRAMES; frame++) {
    // The strongest variant turns the camera for 64 frames.
    camera = Ram_HeapSlots->cameraWork;
    if (effect->variant == 2 && frame < 64) {
      if (effect->side == 0)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      Func_081180e8(134);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    if (frame < 64) {
      for (pillar = 0; pillar != Data_08197a20[effect->variant]; pillar++) {
        EffectPosition_ProjectPoint((s32 *)&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == Data_08197a2f[pillar])
          Sound_PlayFar(145);

        // Four frames after it starts the pillar burns: its flames scroll
        // up through two copies, and its base flickers between two cels.
        if (frame >= Data_08197a2f[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + 0xdd0, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0xdd0, screen.x, screen.y - 24, 20,
                    37);
          } else {
            draw[0](canvas, work->sheet + 0x10b4, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0x10b4, screen.x, screen.y - 24, 20,
                    37);
          }
        }

        // Sparks: 200 as the pillar starts, then four a frame from its
        // sixteenth frame on.
        if (frame == Data_08197a2f[pillar] ||
            frame >= Data_08197a2f[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != SPARK_MAX; i++) {
            struct EffectStep *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;
              spark->x = screen.x << 8;
              spark->y = (screen.y << 8) + 0x1000;
              spark->z = (Trig_Sin(angle) * speed) >> 15;
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == Data_08197a2f[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == 200)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == 4)
                  break;
              }
            }
          }
        }

        // The pillar strikes every target as it starts.
        if (frame == Data_08197a2f[pillar]) {
          work->shakeFrames = 2;
          for (i = 0; i != effect->count; i++) {
            Func_0814cd48(effect->actors[i], 10, 5, i, 8);
            Func_08118088(effect->actors[i], 1);
          }
        }
      }
    }

    // The sparks: 8.8 fixed point, z holding the x speed; they slow, fall,
    // bounce at half speed below line 120, and shrink as they burn out.
    for (i = 0; i != SPARK_MAX; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          draw[i & 1](canvas, sheet + gData_08197410[size - 1], px - size / 2,
                      py - size, size, size * 2);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Battle effects: particle effect entry points.

/// Each entry passes a particle type (color and count) to
/// BattleEffect_RunTargetMotes. effect: effect arguments.
void BattleEffect_RunTaint(struct BattleEffectArgument *effect) {
  if (effect->variant == 0) {
    BattleEffect_RunTargetMotes(effect, 0);
    return;
  }
  BattleEffect_RunTargetMotes(effect, 1);
}

DEFINE_EFFECT_KIND(BattleEffect_RunTargetMotesMode1,
                   BattleEffect_RunTargetMotes, 1)

DEFINE_EFFECT_KIND(BattleEffect_RunDelude, BattleEffect_RunTargetMotes, 2)

DEFINE_EFFECT_KIND(BattleEffect_RunParalyze, BattleEffect_RunTargetMotes, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunHeartrender, BattleEffect_RunTargetMotes, 4)

DEFINE_EFFECT_KIND(BattleEffect_RunMysticFlame, BattleEffect_RunTargetMotes, 5)

DEFINE_EFFECT_KIND(BattleEffect_RunDemonFire, BattleEffect_RunTargetMotes, 7)

DEFINE_EFFECT_KIND(BattleEffect_RunDrown, BattleEffect_RunTargetMotes, 6)

#define MOTE_MAX 512

// The motes live in the map cell buffer, 64 for each target.

// Modes whose motes close in on the target and start again; the others
// drift up and are thrown back at random. Mode 7 closes in once.
#define CONVERGES(mode)                                                        \
  ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6 || (mode) == 7)
#define DECAYS(mode) ((mode) <= 1 || (mode) == 4 || (mode) == 5 || (mode) == 6)

// Where mode 3 unpacks the tornado's glints in the sheet.
enum { RENDER_MODE_TORNADO = 0x2580 };

extern char ResourceId_CyanPalette;  // directory row 0x166
extern char ResourceId_BlastSheet;   // directory row 0x184
extern char ResourceId_GlowOrbSheet; // directory row 0x17b
extern char ResourceId_HeartSheet;   // directory row 0x16e

extern u8 Data_08197a34[]; // ☀️ gTargetMotes_GlintFlips

// Motes swirl around each target in turn, eight frames apart, over a
// rippling canvas: smoke, hearts, glowing orbs or vortex motes by mode,
// in the mode's palette; mode 3 adds the tornado's glints, and mode 7 sends
// the caster at the first target for the final blow.
void BattleEffect_RunTargetMotes(struct BattleEffectArgument *effect,
                                 u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 total;
  s32 member;
  s32 count;
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition base;
  DrawRectangle draw[2];
  void *palette;
  s32 *row;
  s32 i;

  // Set up: the canvas layer and the mode's sheet and palette.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  count = 16;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  if (mode == 0) {
    palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
  } else if (mode == 1) {
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_CyanPalette);
  } else if (mode == 3) {
    palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
  } else if (mode == 4) {
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  } else if (mode == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_GlowOrbSheet);
  } else if (mode == 7) {
    count = 24;
    Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet, 1,
                               0);
    palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  } else {
    count = 32;
    Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 0);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
  }
  CopyWords_((void *)BG_PLTT, palette, 128);
  if (mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
  if (mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                               work->sheet + RENDER_MODE_TORNADO, 1, 0);

  // Scatter the motes around the origin.
  for (i = 0; i != MOTE_MAX; i++) {
    struct EffectStep *mote = &PARTICLES[i];

    if (CONVERGES(mode)) {
      mote->x = ((Random16() & 0xff) - 127) << 15;
      mote->y = ((Random16() & 0xff) - 127) << 14;
      mote->z = ((Random16() & 0xff) - 127) << 15;
    } else {
      mote->x = ((Random16() & 0xff) - 127) << 13;
      mote->y = ((Random16() & 0xff) - 255) << 13;
      mote->z = ((Random16() & 0xff) - 127) << 13;
    }
    mote->variant = 0;
  }

  if (CONVERGES(mode))
    total = effect->count * 8 + 64;
  else
    total = effect->count * 8 + 32;
  if (!(mode <= 1 || mode == 3))
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != total; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // Mode 7 sends the caster at the first target 46 frames before the
    // end and strikes it 14 frames later; the others begin phase 133.
    if (mode == 7) {
      if (frame == total - 46)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
      if (frame == total - 32) {
        Func_081180e8(134);
        Func_08118088(effect->actors[0], 4);
        work->shakeFrames = 8;
      }
    } else if (frame == total - 32) {
      Func_081180e8(133);
    }

    // Ripple the canvas: one horizontal offset for each line.
    row = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *row++ = (0x40000 - (Trig_Sin((frame << 12) + i * 0x800) << 2)) >> 10;

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;

      // Mode 3: two tornado glints a frame around the target, each drawn
      // with a blitter of its own, mirrored at random.
      if (mode == 3 && frame > member * 8 && frame < member * 8 + 32) {
        EffectPosition_ProjectScaledBattler(effect->actors[member], &base);

        for (i = 0; i != 2; i++) {
          s32 angle;
          s32 radius;
          s32 x;
          s32 y;
          const u8 *h;
          const u8 *w;

          /* FAKEMATCH(temporary): w and h set inside statement expressions at
           * their first reads; as (w = gData_08197492)[frame & 3] and its twin
           * for h, 4 of 755 instructions differ in tla-en. */
          angle = Random16() & 0xffff;
          radius = (Random16() & 31) + 4;
          x = base.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
              (({
                 w = gData_08197492;
                 w;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through w hides that it holds the
           * width table's address, so the draw reads through w's register;
           * without it, 233 of 755 instructions differ in tla-en. */
          __asm__("" : "=r"(w) : "0"(w));
          y = base.y - ((Trig_Cos(angle) * radius) >> 16) -
              (({
                 h = gData_08197498;
                 h;
               })[frame & 3] >>
               1);
          /* FAKEMATCH(barrier): an empty asm through h, the same for the
           * height table; without it, 331 of 755 instructions differ in
           * tla-en. */
          __asm__("" : "=r"(h) : "0"(h));
          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                            Data_08197a34[Random16() & 3],
                        BLITTER_MAX);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas,
              work->sheet + gData_08197486[frame & 3] + RENDER_MODE_TORNADO, x,
              y + 16, w[frame & 3], h[frame & 3]);
          HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        }
      }

      // An additive and a max blitter for this target's motes.
      BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE << 4 |
                                                 COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 |
                                                           COMPOSITE_CLIP_X |
                                                           COMPOSITE_CLIP_Y);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      point[0] = object->x;
      point[1] = 160 << 13;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);

      // From its turn on, the target's motes turn around it.
      if (frame > member * 8) {
        SceneTransform_ApplyYaw(frame << 9);
        if (mode <= 1 || mode == 4)
          SceneTransform_ApplyPitch(frame << 9);
        for (i = 0; i != count; i++) {
          struct EffectStep *mote = &PARTICLES[member * 64 + i];
          s32 distance;

          if (frame > member * 8 + i) {
            s32 xx = (mote->x >> 8) * (mote->x >> 8);
            s32 yy = (mote->y >> 8) * (mote->y >> 8);
            s32 zz = (mote->z >> 8) * (mote->z >> 8);

            distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0 && mote->variant <= 23) {
              s32 cel = mote->variant / 4;

              EffectPosition_ProjectPoint((s32 *)mote, &screen);
              screen.x >>= 1;
              if (mode == 5 || mode == 7)
                draw[1](canvas, work->sheet + cel * 1600, screen.x - 20,
                        screen.y - 20, 40, 40);
              else if (mode == 6)
                draw[1](canvas, work->sheet + 0xc00, screen.x - 6,
                        screen.y - 12, 12, 24);
              else if (mode == 4)
                draw[1](canvas, work->sheet, screen.x - 11, screen.y - 21, 22,
                        42);
              else
                draw[(i & 3) != 0](canvas, work->sheet + cel * 1152,
                                   screen.x - 12, screen.y - 24, 24, 48);

              // Close in by a step of the distance, or drift up.
              if (DECAYS(mode)) {
                mote->x -= mote->x / distance;
                mote->y -= mote->y / distance;
                mote->z -= mote->z / distance;
              } else {
                mote->y += 0x10000;
              }
              mote->variant++;
              if (mote->variant == 24) {
                if (DECAYS(mode)) {
                  mote->variant = 0;
                } else {
                  mote->x = ((Random16() & 0xff) - 127) << 13;
                  mote->y = ((Random16() & 0xff) - 255) << 12;
                  mote->z = ((Random16() & 0xff) - 127) << 13;
                }
              }
            }
          }
        }
      }
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);

      // The target flashes 16 frames into its turn, until the end.
      if (frame == member * 8 + 16) {
        s32 delay = total - frame;

        if (delay > 31)
          delay = 31;
        Func_0814cd48(effect->actors[member], 7, 5, member, delay);
      }
    }

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (!(mode <= 1 || mode == 3))
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleEffect_EndCanvasLayer();
}

#undef GROUND_Y

#define SHEEP_COUNT 12
#define SHEEP_FRAMES 98
#define GROUND_Y (92 << 16)

extern char ResourceId_SheepCardSheet; // directory row 0xe3

void Func_0815e22c(struct EffectPosition *middle); // the targets' middle

// A flock of sheep cards drops in over the targets, one every four frames,
// and bounces on the ground, tumbling through eight cels; at frame 80 the
// next phase begins, and each target turns back to its own palette in turn.
void BattleEffect_RunSleep(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition middle;
  s32 record[3];
  struct EffectStep *one;
  struct EffectStep *sheep;
  s32 i;
  s32 frame;
  DrawRectangle drawB;
  DrawRectangle drawA;
  s32 scroll;
  s32 member;

  // Set up: the canvas layer, the sheep sheet and a transparent blitter
  // pair, the second mirrored.
  sheep = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  scroll = 0;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = scroll;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepCardSheet, work->sheet, 1,
                             1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  drawA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  drawB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The sheep wait above the screen with a random spin and starting cel.
  for (i = 0; i != 32; i++) {
    one = &sheep[i];
    one->x = ((Random16() & 63) + 32) << 16;
    one->y = -0x200000;
    one->velocityY = Random16() & 0;
    one->z = Random16() & 3;
    one->variant = Random16() & 255;
  }

  // Centre the canvas on the targets; what BG2 cannot reach moves the sheep.
  Func_0815e22c(&middle);
  middle.x -= 64;
  if (middle.x < 0) {
    scroll = middle.x;
    middle.x = 0;
  } else if (middle.x > 112) {
    scroll = middle.x - 112;
    middle.x = 112;
  }
  REG_BG2X = -middle.x << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != SHEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 80)
      Func_081180e8(0);

    // Place each target, and from frame 64 restore its palette, one
    // target every 16 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;

      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      record[0] = object->x;
      record[1] = 160 << 14;
      record[2] = object->z;
      SceneTransform_ApplyPosition(record);
      if (frame == member * 16 + 64)
        Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
    }

    // The sheep fall and bounce, each spinning through its eight cels:
    // the first four drawn as they are, the last four mirrored.
    one = sheep;
    for (i = 0; i != SHEEP_COUNT; i++, one++) {
      if (frame > i * 4 && one->y <= 0x7fffff) {
        s32 cel;

        cel = (one->variant / 16) & 7;
        if (cel < 4)
          drawA(canvas, work->sheet + (cel << 10) + 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        else
          drawB(canvas, work->sheet + (cel << 10) - 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        one->y += one->velocityY;
        one->velocityY += 0x2000;
        one->variant += one->z;
        if (one->y > GROUND_Y && one->velocityY == 0) {
          s32 speed = one->velocityY + 1;
          one->y = GROUND_Y;
          one->z += 4;
          one->velocityY = -speed / 2;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define MARKER_FRAMES 72 // how long each marker stays
#define MARKER_STAGGER 16
#define MARKER_CELS 9

extern char ResourceId_FlameColumnSheet;

// As in The Broken Seal's twin, which loads the same sheet: a small flame
// burns above each target in turn, MARKER_STAGGER frames apart, for up to
// MARKER_FRAMES frames each, over a marker at its feet.
void BattleEffect_RunCurse(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  // Unused: the second blitter gives the stack frame its 4 bytes (as one, 154
  // of 222 differ in tla-en); the family's pair, as in The Broken Seal.
  DrawRectangle draw[2];
  struct EffectStep *marker;
  s32 i;
  s32 frame;
  s32 start;
  s32 cel;
  s32 y;
  struct EffectPosition pos;

  // Set up: the canvas layer, unscaled; for side 1 the canvas shows the
  // right half of the field.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work->sheet, 1,
                             1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_TRANSPARENT << 4 |
                                             COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  work->transferMode = 1;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each flame starts at a random point in its turn.
  for (i = 0; i != effect->count; i++) {
    work->particles[i].variant = Random16() & 63;
  }

  frame = 0;
  while (frame != (effect->count << 5) + 32) {
    if (frame == 32) {
      Func_081180e8(0);
    }
    i = 0;
    if (effect->count != 0) {
      do {
        start = i * MARKER_STAGGER;
        if (frame == start) {
          Sound_PlayFar(143);
        }
        if (frame >= start && frame < start + MARKER_FRAMES) {
          // The marker at the target's feet, then the flame, six frames a cel.
          marker = &work->particles[i];
          EffectPosition_ProjectScaledBattler(effect->actors[i], &pos);
          if (effect->side == 1) {
            pos.x -= 112;
          }
          y = pos.y;
          pos.y = y - 16;
          draw[0](canvas, work->sheet + 1728, pos.x - 8, y - 20, 16, 20);
          // DEAD: always true here; the ROM keeps the test.
          if (frame >= start) {
            cel = ((frame - start) + marker->variant) / 6 % MARKER_CELS;
            draw[0](canvas, work->sheet + cel * 192, pos.x - 8, pos.y - 16, 16,
                    12);
          }
        }
        i++;
      } while (i != effect->count);
    }
    work->transferPending = 1;
    wait(1);
    frame += 1;
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

extern char ResourceId_MercuryDjinnSmallSheet; // directory row 0x155

extern char ResourceId_SparkleDots; // directory row 0x17e

extern u16 Data_0819744c[]; // ☀️ gBattleEffect_PuffCells
extern u8 Data_0819745e[];  // ☀️ gBattleEffect_PuffSizes

// A Djinn rides a swarm of sparks from the caster to the first target: 32
// sparks spin around it in four planes while it travels, then it hovers
// over the targets while puffs of sparkle gather on each in turn.
void Func_0814b350(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  DrawRectangle *draw;
  struct MotionObject *actor;
  struct MotionObject *goal;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 rider[3];
  struct EffectPosition screen;
  s32 point[3];
  struct EffectPosition view;
  DrawRectangle callbacks[2];

  // Set up: the canvas layer, the Djinn and the particle sprites, and the
  // blitter pair for the side the Djinn faces.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw = callbacks);
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  goal = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The swarm: 64 sparks thrown out level from the Djinn.
  for (i = 0; i != 64; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 heading = Random16() & 0xffff;
    s32 speed = (Random16() & 0xff) + 128;

    spark->x = 0;
    spark->y = ((Random16() & 31) + 20) << 16;
    spark->z = 0;
    spark->velocityX = (Trig_Sin(heading) * speed) >> 5;
    spark->velocityY = 0;
    spark->velocityZ = (Trig_Cos(heading) * speed) >> 5;
    spark->variant = 0;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The Djinn travels from the caster to above the first target in 40
  // frames.
  position[0] = actor->x;
  position[1] = 0;
  position[2] = actor->z;
  target[0] = goal->x;
  target[1] = 180 << 15;
  target[2] = 0;
  delta[0] = (target[0] - position[0]) / 40;
  delta[1] = (target[1] - position[1]) / 40;
  delta[2] = (target[2] - position[2]) / 40;

  for (frame = 0; frame != 98; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 8)
      Sound_PlayFar(212);
    if (frame == 80)
      Sound_PlayFar(142);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    SceneTransform_ApplyPosition(position);

    // The caster flashes for the first 24 frames.
    if (frame == 0)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 24)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);

    // Each spark spins in one of four planes, smaller further away, and
    // after 30 frames is pulled back toward the Djinn.
    for (i = 0; i != 32; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (frame > i && spark->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case 2:
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i * 32 + 256));
          Func_080150e4(-frame * (i * 32 + 256));
          break;
        }
        EffectPosition_ProjectPoint((s32 *)spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        if (screen.depth < 250)
          screen.depth = 250;
        if (screen.depth > 634)
          screen.depth = 634;
        size = 9 - (screen.depth - 250) / 64;
        callbacks[0](canvas, sheet + gData_08197410[size - 1],
                     screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }

    // The Djinn bobs over the target for the last 15 frames.
    if (frame > 82) {
      rider[0] = 0;
      rider[1] = Trig_Sin(frame << 10) << 2;
      rider[2] = 0;
      EffectPosition_ProjectPoint(rider, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    work->transferPending = 1;
    wait(1);
  }

  // The second part: 64 puffs of sparkle for each target.
  for (i = 0; i != 512; i++) {
    struct EffectStep *spark = &PARTICLES[i];

    spark->x = ((Random16() & 0xff) - 127) << 15;
    spark->y = ((Random16() & 0x7f) + 64) << 15;
    spark->z = ((Random16() & 0xff) - 127) << 15;
    spark->variant = 0;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, sheet, 0, 0);

  for (frame = 0; frame != effect->count * 8 + 72; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The Djinn sways over the targets, then rises away for the last 32
    // frames.
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame >= effect->count * 8 + 40)
      target[1] += 0x40000;
    point[0] = target[0];
    point[1] = target[1];
    point[2] = target[2] + Trig_Sin(frame << 11) * 40;
    EffectPosition_ProjectPoint(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);

    // Each target in turn, eight frames apart: its puffs close in around
    // it, and it flashes from 40 to 64 frames into its turn.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          GetBattleObjectSlotFar(effect->actors[member])->object;
      s32 start = member * 8;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      position[0] = object->x;
      position[1] = 160 << 14;
      position[2] = object->z;
      SceneTransform_ApplyPosition(position);
      if (frame == start + 30)
        Sound_PlayFar(126);
      if (frame == start + 40)
        Func_0814cd48(effect->actors[member], 7, -1, -1, 0);
      if (frame == start + 64)
        Func_0814cd48(effect->actors[member], 0, -1, -1, 0);
      if (frame > start) {
        SceneTransform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct EffectStep *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

            if (distance != 0) {
              s32 cel;

              EffectPosition_ProjectPoint((s32 *)spark, &view);
              view.x >>= 1;
              if (view.depth < 314)
                view.depth = 314;
              if (view.depth > 634)
                view.depth = 634;
              // DEAD: BattleEffect_RunHealingMotes' size by depth, copied with
              // its code; the next line sets cel for real.
              cel = 3 - (view.depth - 314) / 128;
              cel = (i * 4 + frame) % 9;
              {
                u8 *cell = sheet + Data_0819744c[cel];
                u32 edge = Data_0819745e[cel];
                u32 half = edge >> 1;

                draw[1](canvas, cell, view.x - half, view.y - half, edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Balm (kind 230).
void BattleEffect_RunBalm(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 0);
}

/// Abilities: Tinder (kind 231).
void BattleEffect_RunTinder(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 1);
}

/// Abilities: Phoenix, Regen Dance (kind 436).
void BattleEffect_RunPhoenix(struct BattleEffectArgument *effect) {
  Func_0814b9e8(effect, 2);
}
#undef SLOT_BLITTER
#undef BLITTER
#undef SLOT_ALT
#undef OBJ_TILES
#undef SLOT_BATTLE_EFFECT
#undef SLOT_BATTLE_CANVAS
#undef BATTLE_EFFECT_WORK_SIZE
#undef CLIP
#undef HI
#undef PARTICLES
#undef SKULLS
#undef SKULL_SIZE
#undef SKULL_CELL
#undef HEAP_OFFSET
#undef HEAP_BLITTER
#undef HEAP_BLITTER_ALTERNATE
#undef SPIRAL_SIZE
#undef SPIRAL_CELS
#undef MOTES_PER_TARGET
#undef MOTE_CELL
#undef PIC_MOTE
#undef PIC_DJINN
#undef MOTES
#undef BURSTS
#undef RECORD_SHARDS
#undef RECORD_SHOT_BLITTER
#undef RECORD_SHOTS
#undef RECORD_FRAMES
#undef SMOKE_SHEET
#undef SHARD_SHEET
#undef PANEL_B
#undef PANEL_C
#undef PANEL_D
#undef PANEL_E
#undef PANEL_F
#undef PANEL_G
#undef PIXEL_BUFFER
#undef FLARE_SHEET
#undef DJINN_SHEET
#undef DJINN_SIZE
#undef CORNERS
#undef EDGE_STEPS
#undef BOLT_COUNT
#undef RING_POINTS
#undef RING_STEPS
#undef FAR_DEPTH
#undef ACTOR
#undef STAR_COUNT
#undef DOT_SIZE
#undef GROUND_Y
#undef MOD3
#undef WHIRLWIND_CEL
#undef DUST_SHEET
#undef COMET_SHEET
#undef COMETS
#undef FORMATION_RESOURCE
#undef STREAK_SHEET
#undef SPARK_COUNT
#undef FORMATION_SIZE
#undef COLUMN_SHEET
#undef COLUMN_WIDTH
#undef COLUMN_HEIGHT
#undef MOTE_COUNT
#undef SEAT_FIRST
#undef TORNADO_FRAMES
#undef GLINT_COUNT
#undef GLINT_CELS
#undef SHARDS_PER_CRYSTAL
#undef SHATTERED
#undef SPARK_MAX
#undef PILLAR_BASE_Y
#undef FLASH_POINTS
#undef FLASH_DRAWN
#undef FLASH_FRAMES
#undef SHAPE
#undef GROUP_COUNT
#undef GROUP_X
#undef PILLAR_FRAMES
#undef MOTE_MAX
#undef CONVERGES
#undef DECAYS
#undef SHEEP_COUNT
#undef SHEEP_FRAMES
#undef MARKER_FRAMES
#undef MARKER_STAGGER
#undef MARKER_CELS

// ---- games/tla/src/battle_effect/f14b9e8.c ----
struct Model;

/* VIEW(f14b9e8): static CopyWords_ as CopyWords___f14b9e8; the definition has static __inline__ void CopyWords_(void *destination, const void *source, s32 size). */
static __inline__ s32 CopyWords___f14b9e8(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

#define HEAP_BLITTER 104 // ☀️ HEAP_SLOT_BLITTER

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
/* VIEW(f14b9e8): enumerator BLITTER_ADDITIVE as BLITTER_ADDITIVE__f14b9e8; f14153c has enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY }. */
/* VIEW(f14b9e8): enumerator BLITTER_MAX as BLITTER_MAX__f14b9e8; f14153c has enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY }. */
/* VIEW(f14b9e8): enumerator BLITTER_TRANSPARENT as BLITTER_TRANSPARENT__f14b9e8; f14153c has enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY }. */
/* VIEW(f14b9e8): enumerator BLITTER_COPY as BLITTER_COPY__f14b9e8; f14153c has enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY }. */
enum { BLITTER_ADDITIVE__f14b9e8, BLITTER_MAX__f14b9e8, BLITTER_TRANSPARENT__f14b9e8, BLITTER_COPY__f14b9e8 };

#define QUAD_FRAMES 64
#define QUAD_COUNT 16

// Where the quads' texture and the Djinn sit in the sheet.
#define SHEET_TEXTURE 0x2710
#define SHEET_DJINN 0x65c0

// The kinds this body draws, chosen by the effect's entry.
enum {
  QUADS_MERCURY, // over a prepared canvas, with the Mercury Djinn
  QUADS_MARS,    // over a prepared canvas, with the Mars Djinn
  QUADS_RISING   // rising from the first target
};

// A texture as the render jobs take it: 1 << logWidth by 1 << logHeight.
struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  void *pixels;
};

// A render job (the first of Render_AllocateModelList's entries).
struct RenderJob {
  s32 kind;
  s32 next;
  const void *faces;
  void *vertices;
  struct Texture *texture;
  s32 fade;
  s32 tail;
};

extern char ResourceId_Data0D0;           // directory row 0xd0
extern char ResourceId_FeatherSheet;      // directory row 0xd1
extern char ResourceId_MercuryDjinnSheet; // directory row 0x154
extern char ResourceId_BlueFlameSheet;    // directory row 0x130
extern char ResourceId_MarsDjinnSheet;    // directory row 0x150

extern const u8 gData_08199210[]; // a quad's four corners, x y z for each
extern const u8 gData_08199340[]; // the quad's face

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 flags); // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world); // the canvas preparer
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *pos);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z); // translate
void Func_080151e4(s32 x, s32 y, s32 z); // scale
void Func_080150e4(s32 angle);           // roll
void SceneTransform_ApplyPitch(s32 angle);
void Sound_PlayFar(s32 cue);

// Sixteen textured quads tumble down over the targets (or, from the first
// target, rise), three frames apart, for 64 frames; with a prelude a Djinn
// circles over the canvas and a fan of light lines spreads from it. At
// frame 56 every target flashes.
void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode) {
  s32 anchor[3];
  s32 size[3];
  struct EffectPosition pos;
  struct Texture texture;
  s32 y;
  struct BattleEffectWork *work;
  struct EffectStep *quad;
  s32 x;
  s32 frame;
  struct RenderJob *job;
  void *vertices;
  void *canvas;
  // Unused: draws[1] gives the stack frame its 4 bytes.
  DrawRectangle draws[2];
  s32 i;
  s32 height;
  s32 fade;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);

  // The mode's canvas, sheets and palette.
  if (mode == QUADS_MERCURY) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 33, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D0,
                               work->sheet + SHEET_TEXTURE, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 0);
    /* FAKEMATCH(wrapper): the palette copy through CopyWords_; as a direct
     * Iwram_CopyWords call, 2 of 549 instructions differ in tla-en. */
    CopyWords___f14b9e8((void *)BG_PLTT,
               Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  } else if (mode == QUADS_MARS) {
    Func_0815585c(effect, (effect->side ^ 1) << 4 | 2, anchor, size);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SHEET_DJINN, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  } else {
    EffectPosition_ProjectBattler(effect->actors[0], &pos);
    anchor[0] = pos.x;
    Resource_LoadAndDecompress((s32)&ResourceId_FeatherSheet,
                               work->sheet + SHEET_TEXTURE, 1, 1);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Centre the canvas on the anchor; the blitter mirrors for side 0.
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - anchor[0]) << 8;
  if (effect->side == 0)
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                             BLITTER_ADDITIVE__f14b9e8 << 4 | COMPOSITE_CLIP_X |
                                 COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  else
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_ADDITIVE__f14b9e8 << 4 |
                                               COMPOSITE_CLIP_X |
                                               COMPOSITE_CLIP_Y);
  draws[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  for (frame = 0; frame != QUAD_FRAMES; frame++) {
    if (frame == 8)
      Sound_PlayFar(246);

    // The quads start above the screen (or at the target for mode 2), each
    // with its own heading, spin and size.
    if (frame == 0) {
      height = -0x200000;
      for (i = 0; i != 32; i++) {
        quad = &work->particles[i];
        quad->x = Random16() & 0xffff;
        Random16();
        quad->z = 1 << 16;
        if (mode == QUADS_RISING)
          quad->y = 32 << 16;
        else
          quad->y = height;
        quad->velocityY = ((Random16() & 0xffff) << 1) + 0x10000;
        quad->velocityX = Random16() & 0xffff;
        quad->velocityZ = Random16() & 0xffff;
        quad->variant = (Random16() & 0x3fff) + 0x2000;
        height += -0x40000;
      }
    }

    // Frame 56: the next phase, and every target flashes.
    if (frame == 56) {
      Sound_PlayFar(212);
      Func_081180e8(-1);
      for (i = 0; i != effect->count; i++)
        Func_0814cd48(effect->actors[i], 7, -1, i, 8);
    }

    // With a prelude the Djinn circles over the anchor and climbs away
    // after frame 32, and a fan of light lines spreads from the canvas's
    // middle while the canvas fades.
    if (effect->prelude == 1) {
      x = ((-Trig_Sin(frame << 11) * 20) >> 16) + 44;
      y = ((Trig_Cos(frame << 11) * 4) >> 16) + anchor[1] - 24;
      if (frame > 32)
        y = y - frame * 4 + 128;
      draws[0](canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draws[0](canvas, work->sheet + SHEET_DJINN, x, y, 40, 40);
      // DEAD: frame never passes 63 here; the source keeps the test.
      if (frame <= 79) {
        s32 angle;
        s32 lineX;
        s32 lineY;

        Iwram_FillWords(work->sheet, 128, 0x28282828 - frame / 2 * 0x01010101);
        i = 0;
        angle = frame << 9;
        for (; i != 16; i++) {
          lineX = ((Trig_Sin(angle) * frame) >> 16) + 64;
          lineY = ((Trig_Cos(angle) * frame) / 8 >> 16) - 16;
          draws[0](canvas, work->sheet, lineX, lineY, 1, 128);
          angle += 0x1000;
        }
      }
    }

    // The quads: each a render job of one textured face, placed by its own
    // translation, scale, pitch and roll.
    Render_ResetTransformState();
    gCameraSceneParameters[4] = 104;
    // DEAD: frame never passes 63 here; the source keeps the test.
    if (frame <= 63) {
      vertices = Heap_AllocEwram(32);
      job = Render_AllocateModelList(1);
      fade = 0;
      if (frame > 55)
        fade = (56 - frame) * 8;
      job->fade = fade;
      texture.logWidth = 6;
      texture.logHeight = 6;
      texture.pixels = work->sheet + SHEET_TEXTURE;
      job->kind = 6;
      job->texture = &texture;
      job->faces = gData_08199340;
      job->vertices = vertices;

      for (i = 0; i != QUAD_COUNT; i++) {
        s32 scale;
        s32 delay;

        quad = &work->particles[i];
        delay = i * 3;
        scale = quad->variant;

        if (frame >= delay) {
          s32 across;
          s32 deep;

          Render_ResetTransformState();
          SceneTransform_ApplyPitch(3000);
          if (mode == QUADS_RISING)
            quad->y -= quad->velocityY;
          else
            quad->y += quad->velocityY;
          if (quad->velocityY < -0x20000)
            quad->z += 0x10000;
          else
            quad->z += 0xc000;
          across = Trig_Sin(quad->x) * ((s16 *)&quad->z)[1];
          deep = Trig_Cos(quad->x) * ((s16 *)&quad->z)[1];
          quad->x += 0x200;
          Func_08015160(across, quad->y, deep);
          Func_080151e4(scale, scale, scale);
          SceneTransform_ApplyPitch(quad->velocityX);
          Func_080150e4(quad->velocityZ);
          quad->velocityX += 0x400;
          quad->velocityZ += 0x400;
          Render_TransformVertices(gData_08199210, vertices, 4);
          Render_DrawModelList(job);
        }
      }
      Heap_Release(job);
      Heap_Release(vertices);
    }
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

struct FxPartyList;

struct StripQuad;


struct RequestPair {
  s32 x, y;
};

void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);

void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);

void Func_0801521c(s32 scale);

void SceneTransform_ApplyYaw(s32 angle);

void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets

void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar

void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

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

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])


void Func_08179e6c(void);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);

extern char ResourceId_Data134;
extern char ResourceId_PetalSheet;
extern char ResourceId_CrescentRowSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C1;
extern u8 gData_02014000[];
extern u16 gData_08197410[];
extern const struct RequestPair Data_08196e0c;
extern const u16 Data_08197a38[][2]; // the charging Zagan's parts
extern const u16 Data_08197a64[][2]; // the galloping Zagan's
extern const u8 Data_08197a94[];     // each ring's start
extern const u8 Data_08197a98[];     // growth
extern const s8 Data_08197a9c[];     // height
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];

#define ZAGAN_PARTS 12

// Abilities: Zagan.
//
// The screen slides over to a white field and petals blow across it; Zagan
// (twelve objects) gallops in from the right as the backdrop darkens, rears,
// and at frame 157 swings its axe across the targets, striking four times
// while the screen shakes and rings of dust spread. Holding A or B draws
// only every other frame.
void BattleEffect_RunZagan(struct BattleEffectArgument *effect) {
  u8 *battle;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  s32 *transition;
  s32 savedScroll;
  DrawRectangle *blit;
  struct RequestPair position;
  struct RequestPair velocity;
  s32 skip;
  DrawRectangle blitters[2];
  struct RequestPair request;
  s32 placement[4];
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  s32 none;
  u8 *ringPixels;
  s32 size;
  s32 scroll;
  s32 frame;
  s32 i;
  s32 j;

  none = 0;
  ringPixels = gData_02014000; // the rings' texture, for the whole function
  battle = (u8 *)Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  savedScroll = gBgScroll[1].x;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = none;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The screen slides over to the field.
  for (scroll = -80, frame = 0; frame != 28; frame++) {
    if (frame == 27) {
      gCameraSceneParameters[4] = 240;
      BattleEffect_SelectLivingTargets(work->effect);
      work->scrollStepX = -8;
      transition[4] = 1;
      Iwram_FillWords((void *)(BG_PLTT + 96), 256, 0x7fff7fff);
      Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->sheet, 0, 0);
      // Ten ever-fainter copies of one mote.
      for (j = 0; j != 10; j++) {
        size = 4;
        for (i = 0; i != size * size * 2; i++) {
          s32 offset = gData_08197410[size - 1] + i + 0x700;
          s32 value = ((u8 *)work)[offset];

          if (value != 0) {
            value -= j * 7;
            if (value <= 0) {
              value = 1;
            }
          }
          sprites[size * size * j * 2 + i] = value;
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      for (i = 0; i != 32; i++) {
        work->sheet[0x5dc0 + i] = i + 16;
        work->sheet[0x5de0 + i] = i + 16;
        work->sheet[0x5e00 + i] = i + 16;
      }
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  blit = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)blit);
  work->transferMode = 2;
  work->transferValue = 50;
  skip = 0;
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);
  BattleEffect_SpawnObjects(ZAGAN_PARTS, 658, 1);
  REG_BG2X = skip;
  REG_BG2PA = 0x80;

  for (frame = 0; frame != 200; frame++) {
    if (gInput.repeat & 3) {
      skip = 1;
    }
    if (frame == 0) {
      Sound_PlayFar(212);
    }
    if (frame == 32) {
      Sound_PlayFar(191);
    }
    if ((u32)(frame - 78) <= 31) {
      REG_BLDALPHA = (16 - (frame - 78) / 2) | 0x1000;
    }
    if (frame == 140) {
      REG_BLDALPHA = 0x1010;
    }
    if (frame == 157) {
      Func_081180e8(145);
      goto strike;
    }
    if (frame == 161 || frame == 165 || frame == 169) {
    strike:
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, -1, i, 4);
      }
    }
    if (frame == 0) {
      transition[4] = 1;
      position.x = 240 << 16;
      position.y = 0x400000;
      velocity.x = -0x140000;
      velocity.y = 0;
      for (i = 0; i != ZAGAN_PARTS; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 12);
      }
      REG_BLDALPHA = 0x1010;
      Iwram_FillWords((void *)(BG_PLTT + 96), 256, 0x7fff7fff);
      work->scrollStepX = -8;
      gCameraSceneParameters[3] = -968;
      gCameraSceneParameters[4] = 120;
      gBgScroll[1].x = savedScroll + 1128;
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, work->sheet, 1,
                                 1);
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
      work->transferMode = 2;
      work->transferValue = 50;
    }
    gCameraSceneParameters[3] -= work->scrollStepX;
    gBgScroll[1].x += work->scrollStepX;

    // The backdrop darkens toward the battle's palette.
    if (!(frame & 1) && (u32)(frame - 32) <= 111) {
      u16 *from = Resource_GetTableEntry(*(u16 *)(battle + 0x670));
      u16 *to = (u16 *)(BG_PLTT + 96);

      for (i = 0; i != 128; i++, from++, to++) {
        s32 b = (*to >> 10) & 31;
        s32 g = (*to >> 5) & 31;
        s32 r = *to & 31;
        s32 blue = ((*from >> 10) & 31) - 2;
        s32 green = ((*from >> 5) & 31) - 10;
        s32 red = (*from & 31) - 6;

        if (blue < 0) {
          blue = 0;
        }
        if (green < 0) {
          green = 0;
        }
        if (red < 0) {
          red = 0;
        }
        if (b > blue) {
          b--;
        }
        if (g > green) {
          g--;
        }
        if (r > red) {
          r--;
        }
        *to = (b << 10) | (g << 5) | r;
      }
    }
    if ((u32)(frame - 64) <= 35) {
      velocity.x += 0x8000;
      velocity.y += -0x1000;
    }
    if (frame > 177) {
      velocity.x += -0x20000;
      velocity.y += -0x10000;
    } else if (frame > 139) {
      velocity.x += -40000;
      velocity.y += 0x2000;
    } else if (frame > 99) {
      velocity.x += -0x2000;
      velocity.y += 0x800;
    }
    if (frame == 156) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_CrescentRowSheet, work->sheet,
                                 1, 0);
      work->shakeFrames = 16;
      for (i = 0; i != ZAGAN_PARTS - 1; i++) {
        Animation_ApplyChildArgumentFar(work->objects[i], i + 24);
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if ((u32)(frame - 156) <= 7) {
      blitters[0](canvas, work->sheet, 12, 0, 58, 120);
    }
    for (i = 0; i != 8; i++) {
      if (frame == i * 6 + 114) {
        work->scrollStepX++;
      }
    }
    if (frame == 163) {
      gBgScroll[1].x += -20;
    }
    if (frame == 167) {
      gBgScroll[1].x += -10;
    }
    if (frame == 171) {
      gBgScroll[1].x += -10;
    }

    // Petals on the wind.
    if (frame == 0) {
      s32 row;

      for (i = 0, row = 3, p = work->particles; i != 20; i++, p++, row += 6) {
        p->x = (Random16() & 127) - 160;
        p->y = row;
        p->variant = Random16() & 1;
        p->velocityX = (Random16() & 3) + p->variant * 8 + 8;
      }
    }
    if ((u32)frame <= 119) {
      s32 odd;

      for (i = 0, odd = frame & 1, p = work->particles; i != 20; i++, p++) {
        if (!odd || !skip) {
          blitters[0](canvas, work->sheet + 0x5dc0, p->x, p->y, 32,
                      p->variant + 1);
        }
        p->x += p->velocityX;
        if (p->x > 128) {
          p->x -= 160;
        }
      }
    }
    if ((u32)frame > 195) {
      goto shake;
    }

    // Zagan.
    request = Data_08196e0c;
    placement[3] = 0;
    placement[1] = 0;
    if (!(frame & 1) || !skip) {
      if (frame > 156) {
        for (i = 0; i != ZAGAN_PARTS - 1; i++) {
          placement[0] = (Data_08197a38[i][0] << 16) + position.x - 0x200000;
          placement[2] = (Data_08197a38[i][1] << 16) + position.y + 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&request, 0);
        }
      } else {
        for (i = 0; i != ZAGAN_PARTS; i++) {
          placement[0] = (Data_08197a64[i][0] << 16) + position.x;
          placement[2] = (Data_08197a64[i][1] << 16) + position.y - 0x100000;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&request, 0);
        }
      }
    }
    if (frame > 177) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 63 / 64;
      velocity.y = velocity.y * 63 / 64;
    } else if (frame > 149) {
      if (position.y > 0x37ffff) {
        velocity.x = 0;
        velocity.y = 0;
      }
      position.x += velocity.x;
      position.y += velocity.y;
    } else if (frame <= 59) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 58 / 64;
      velocity.y = velocity.y * 62 / 64;
    } else {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 58 / 64;
      velocity.y = velocity.y * 61 / 64;
    }
    if ((!(frame & 1) || !skip) && frame <= 119) {
      blit[1](canvas, work->sheet + ((frame / 3) & 3) * 6000,
              (position.x / 2 >> 16) - 12, (position.y >> 16) - 58, 60, 100);
    }
    if (frame == 157) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, ringPixels, 0, 0);
    }
    if ((frame & 1) && skip) {
      goto shake;
    }

    // Rings of dust from the blow.
    if (frame <= 156) {
      goto next;
    }
    {
      s32 k;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = ringPixels;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;
      for (k = 0; k != 4; k++) {
        s32 start = Data_08197a94[k] + 157;
        s32 scale;
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (((frame - start) * Data_08197a98[k] * 125) << 4) + 0x8000;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(-0x100000, Data_08197a9c[k] << 16, 0);
        Func_080151e4(scale, scale * 4 / 3, scale);
        SceneTransform_ApplyPitch(3000);
        model->scrollU = ((frame + k * 5) & 7) * 16;
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
  shake:
    if (frame > 156) {
      if (work->shakeFrames > 0) {
        s32 offsetX = (Random16() & 3) - 8;
        s32 randomY = Random16() & 15;
        s32 offsetY = randomY - 8;

        gBgScroll[1].y = randomY + 24;
        gCameraSceneParameters[3] = 120 - offsetX;
        gCameraSceneParameters[4] = 120 - offsetY;
        work->shakeFrames--;
      } else {
        gBgScroll[1].y = work->savedBg1Y;
        gCameraSceneParameters[3] = 120;
        gCameraSceneParameters[4] = 120;
      }
    }
  next:
    BattleEffect_CountDownBattlerTints();
    if (!(frame & 1) || !skip) {
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  transition[4] = 0;
  for (i = 0; i != ZAGAN_PARTS; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

struct ShakeWork {
  u8 unk0000[54];
  u16 offset;
  u8 unk0038[0x7774];
  s32 amount;
  s32 phase;
};

/// A task: turns the camera by the effect's delta, the whole
/// delta on phase 1 and half of it on the frames in between.
void Camera_ApplyPhasedDelta(void) {
  struct ShakeWork *work = (struct ShakeWork *)Ram_HeapSlots->battleFxWork[0];
  struct ShakeWork *camera = Ram_HeapSlots->cameraWork;

  if (work->phase == 1) {
    camera->offset += work->amount;
    work->phase = 0;
  } else {
    camera->offset += work->amount / 2;
    if (work->phase == 2)
      work->phase = 0;
    else
      work->phase = 2;
  }
}

// Battle effects: repeat an empty effect.

/// Calls the empty BattleEffect_DoNothing cnt times without waiting for frames.
/// cnt: call count.
void BattleEffect_CallDoNothing(s32 cnt) {
  s32 n;

  n = 0;
  if (cnt != 0) {
    do {
      n += 1;
      BattleEffect_DoNothing();
    } while (n != cnt);
  }
}


#include "macros.h"

struct SummonArgument;

struct FxRunWork {
  u8 unk0000[0x78e4];
  s32 active;
  u8 unk78e8[8];
  s32 *script;
};

struct BattleEffectArgument;
void BattleEffect_RunRamses(struct BattleEffectArgument *effect);
void BattleEffect_RunNereid(struct BattleEffectArgument *effect);
void BattleEffect_RunKirin(struct BattleEffectArgument *effect);
void BattleEffect_RunAtalanta(struct BattleEffectArgument *effect);

void BattleEffect_RunNeptune(struct BattleEffectArgument *effect);
/* VIEW(f14b9e8): routine BattleEffect_RunTiamat as void BattleEffect_RunTiamat(s32 *arg); the definition has DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, Func_08147aec, 1) DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, Func_08147aec, 0) void Func_08.... */
extern void BattleEffect_RunTiamat__f14b9e8(s32 *arg) __asm__("BattleEffect_RunTiamat");
void BattleEffect_RunProcne(struct BattleEffectArgument *effect);
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect);
void BattleEffect_RunBoreas(struct BattleEffectArgument *effect);
void BattleEffect_RunThor(struct BattleEffectArgument *effect);
void BattleEffect_RunMegaera(struct BattleEffectArgument *effect);
void BattleEffect_RunFlora(struct BattleEffectArgument *effect);
void BattleEffect_RunMoloch(struct BattleEffectArgument *effect);
void BattleEffect_RunUlysses(struct BattleEffectArgument *effect);
void BattleEffect_RunHaures(struct BattleEffectArgument *effect);
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect);
void BattleEffect_RunCoatlicue(struct BattleEffectArgument *effect);
void BattleEffect_RunDaedalus(struct SummonArgument *arg);
void BattleEffect_RunAzul(struct BattleEffectArgument *effect);
void BattleEffect_RunCatastrophe(struct BattleEffectArgument *effect);
void BattleEffect_RunCharon(struct BattleEffectArgument *arg);
void BattleEffect_RunIris(struct BattleEffectArgument *effect);
void BattleEffect_RunDaedalusAlt(struct BattleEffectArgument *effect);
void BattleEffect_RunTrident(struct BattleEffectArgument *effect);
void BattleEffect_RunCruelRuin(struct BattleEffectArgument *effect);
void BattleEffect_RunMeteorBlow(struct BattleEffectArgument *effect);

extern void (*gData_08197aa0[])(s32 *script);
extern u8 gMapCellBuffer[];

/// Runs the battle effect of mode *arg (1 to 29; 0 and 11 the default one)
/// with the effect's three work blocks taken.
void BattleEffect_RunSceneEffect(s32 *arg) {
  struct FxRunWork *work;

  HeapSlot_Alloc(100, 0x302);
  HeapSlot_Alloc(92, 0x7b7c);
  HeapSlot_Alloc(96, 0x4000);
  work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  work->script = arg;
  work->active = 1;
  if (*arg == 0) {
    BattleEffect_RunMeteor((struct BattleEffectArgument *)arg);
  } else {
    switch (*arg) {
    case 1:
      BattleEffect_RunRamses(arg);
      break;
    case 2:
      BattleEffect_RunNereid(arg);
      break;
    case 3:
      BattleEffect_RunKirin(arg);
      break;
    case 4:
      BattleEffect_RunAtalanta(arg);
      break;
    case 5:
      BattleEffect_RunCybele(arg);
      break;
    case 6:
      BattleEffect_RunNeptune(arg);
      break;
    case 7:
      BattleEffect_RunTiamat__f14b9e8(arg);
      break;
    case 8:
      BattleEffect_RunProcne(arg);
      break;
    case 9:
      BattleEffect_RunJudgment(arg);
      break;
    case 10:
      BattleEffect_RunBoreas(arg);
      break;
    case 11:
      BattleEffect_RunMeteor((struct BattleEffectArgument *)arg);
      break;
    case 12:
      BattleEffect_RunThor(arg);
      break;
    case 13:
      BattleEffect_RunZagan(arg);
      break;
    case 14:
      BattleEffect_RunMegaera(arg);
      break;
    case 15:
      BattleEffect_RunFlora(arg);
      break;
    case 16:
      BattleEffect_RunMoloch(arg);
      break;
    case 17:
      BattleEffect_RunUlysses(arg);
      break;
    case 18:
      BattleEffect_RunHaures(arg);
      break;
    case 19:
      BattleEffect_RunEclipse(arg);
      break;
    case 20:
      BattleEffect_RunCoatlicue(arg);
      break;
    case 21:
      BattleEffect_RunDaedalus(arg);
      break;
    case 22:
      BattleEffect_RunAzul(arg);
      break;
    case 23:
      BattleEffect_RunCatastrophe(arg);
      break;
    case 24:
      BattleEffect_RunCharon(arg);
      break;
    case 25:
      BattleEffect_RunIris(arg);
      break;
    case 26:
      BattleEffect_RunDaedalusAlt(arg);
      break;
    case 27:
      BattleEffect_RunTrident(arg);
      break;
    case 28:
      BattleEffect_RunCruelRuin(arg);
      break;
    case 29:
      BattleEffect_RunMeteorBlow(arg);
      break;
    }
  }
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  HeapSlot_Release(100);
}

/// Runs one battle effect script: takes the effect's three work blocks, calls
/// the handler its first word selects, then gives the blocks back.
void BattleEffect_RunTableEffect(s32 *script) {
  struct FxRunWork *work;

  HeapSlot_Alloc(100, 0x302);
  HeapSlot_Alloc(92, 0x7b7c);
  HeapSlot_Alloc(96, 0x4000);
  work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  work->script = script;
  work->active = 1;
  gData_08197aa0[*script - 1](script);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  HeapSlot_Release(100);
}

void BattleEffect_BuildWindowEdgeTable(void) {
  u16 *edge;
  u16 *line;
  u8 *inset;
  s32 i;
  s32 right;

  edge = (u16 *)gMapCellBuffer;
  line = (u16 *)(Ram_MapCellBuffer + 0x82);
  inset = Ram_MapCellBuffer + 2;
  for (i = 0; i != 160; i++) {
    if ((u32)(i - 8) <= 127) {
      right = *edge - inset[i - 8];
      CLAMP(right, 0, 240);
      line[i] = right;
    } else {
      line[i] = 0xfff1;
    }
  }
  {
    volatile u16 *channel = REG_DMA0;
    u16 control;

    control = channel[5];
    channel[5] = control & 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(gMapCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}


struct FxPartyList {
  u8 unk00[20];
  s32 count;
  u8 unk18[12];
  s16 ids[8];
};

struct FxUnit {
  u8 unk00[56];
  s16 hp;
};

struct FxBgScroll {
  u8 unk00[6];
  u16 y;
};

void *Unit_Get(s32 owner);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleBackground_LoadFar(s32, s32, s32);

/// Spawns the battle objects of each unit in the list that has HP left.
void BattleEffect_SelectLivingTargets(struct FxPartyList *party) {
  s16 list[14];
  s32 count = 0;
  s32 i;

  for (i = 0; i != party->count; i++) {
    if (((struct FxUnit *)Unit_Get(party->ids[i]))->hp > 0)
      list[count++] = party->ids[i];
  }
  list[count] = 0xff;
  BattleActor_SpawnObjectsForListFar(list, 0);
}
#undef HEAP_BLITTER
#undef QUAD_FRAMES
#undef QUAD_COUNT
#undef SHEET_TEXTURE
#undef SHEET_DJINN
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef ZAGAN_PARTS

// ---- games/tla/src/battle_effect/f14cca8.c ----

/* VIEW(f14cca8): variable gBgScroll as extern struct FxBgScroll gBgScroll[]; bg_scroll.h has extern struct BgScroll gBgScroll[]. */
extern struct FxBgScroll gBgScroll__f14cca8[] __asm__("gBgScroll");
extern u32 gCameraSceneParameters[];

void BattleBackground_LoadFar(s32, s32, s32);
void wait(s32 frames);

/* VIEW(f14cca8): static FillWords_ as FillWords___f14cca8; f13eb70 has static __inline__ void FillWords_(void *destination, s32 size, u32 value) { Iwram_FillWords(destination, size, value); }. */
static __inline__ void FillWords___f14cca8(void *destination, s32 size, u32 value) {
  // The call in a once-only block, so the entry loads before the size; as
  // the plain call, 6 of 59 instructions of Func_0814cca8 differ in tla-en.
  do {
    Iwram_FillWords(destination, size, value);
  } while (0);
}

void Func_0814cca8(void) {
  u8 *battle = Ram_HeapSlots->battleWork;
  void *buffer = Ram_HeapSlots->battleFxWork[1];

  REG_DISPCNT = 1;
  gBgScroll__f14cca8[0].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  /* FAKEMATCH(wrapper): both fills through FillWords_, which sets each call's
   * own size anew; as direct Iwram_FillWords calls, 43 of 59 instructions
   * differ in tla-en. */
  FillWords___f14cca8(buffer, 0x4000, 0);
  FillWords___f14cca8((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
  Sound_PlayFar(390);
}


struct FxGroupEntry {
  u8 unk00[5];
  u8 param;
  u8 unk06[16];
  u8 frame;
};

struct FxGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct FxGroupEntry *entries[4];
};

struct FxSlot {
  void *object;
  u8 unk04[28];
  struct FxGroupEntry *animationEntry;
  struct FxGroupEntry *effectEntry;
  u8 unk28[2];
  u8 fading;
};

struct FxTimerWork {
  u8 unk0000[0x78dc];
  u8 actorTimers[8];
};

void *GetMotionRecordFar(void *object, s32 index);
s32 BattleMotion_GetSlotPaletteFar(s32 id);
void Animation_ApplyChildArgumentFar(void *group, s32 value);

static __inline__ u8 GroupCount(struct FxGroup *group) { return group->count; }

/// The Broken Seal's BattleEffect_TintBattler as The Lost Age has it: the
/// member count is a byte at 0x1b, the timers sit at 0x78dc of the effect
/// work, and a member's frame is reset only when its parameter changes.
void Func_0814cd48(s32 setId, s32 objectValue, s32 groupValue, s32 stateSlot,
                   s32 stateValue) {
  struct FxSlot *set;
  struct FxGroup *group;
  struct FxTimerWork *state;
  s32 groupIndex;

  set = GetBattleObjectSlotFar(setId);
  state = (struct FxTimerWork *)Ram_HeapSlots->battleFxWork[0];
  groupIndex = 0;

  while ((group = GetMotionRecordFar(set->object, groupIndex)) != NULL) {
    if (stateSlot != -1)
      state->actorTimers[stateSlot] = ((u8 *)&stateValue)[0];
    if (set->fading == 0) {
      if (objectValue != -1) {
        s32 objectIndex;

        objectIndex = 0;
        if (GroupCount(group) != 0) {
          struct FxGroupEntry **objects;

          objects = group->entries;
          do {
            struct FxGroupEntry *object;

            object = *objects++;
            if (object != NULL && object != set->effectEntry &&
                object != set->animationEntry) {
              u8 old = object->param;

              if (objectValue == 0)
                object->param = BattleMotion_GetSlotPaletteFar(setId);
              else
                object->param = objectValue;
              if (object->param != old)
                object->frame = 0xff;
            }
            objectIndex++;
          } while (objectIndex != GroupCount(group));
        }
      }
      if (groupValue != -1)
        Animation_ApplyChildArgumentFar(group, groupValue);
    }
    groupIndex++;
  }
}
