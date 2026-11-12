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

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define TASK_PASS_FRAME 0xc80
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

void BattleEffect_ArmWin0HBlankDma(void); // arms WIN0's HBlank DMA

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
