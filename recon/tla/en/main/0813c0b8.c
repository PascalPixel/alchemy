// Near miss, differ 213 of 563.

// Twin: The Broken Seal's BattleEffect_RunDouse (effect kind 9 in both
// games), ported as is apart from ⚓️'s calling conventions: the effect read
// from the argument, the heap blocks as Ram_HeapSlots fields, BattleEffect_LoadBlitter
// with ⚓️'s mode numbers, BattleEffect_ArmWin0HBlankDma for the WIN0 HBlank DMA, callback
// order 0xc80, results phase 133 and BattleEffectKernels_IntegrateVector2 for
// EffectStep_AdvanceWithGravity2D.

// Left: register choice in the drop and spark loops (the native reloads the
// effect into r7 each turn and keeps the counts table in r6, where this keeps
// the table in r7 and the drop in r6), and the line tables' literal pools,
// placed so that this compiles three instructions more.

// Tried:
// the trial port, slots read as fields                       213
// alchemy permute, 6 minutes (trial 1): its best moved the heap base's
// assignment into a loop, an uninitialised read            rejected

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"

// The blitters' heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define TASK_PASS_FRAME 0xc80
#define PHASE_RESULTS 133

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
extern u16 gMapCellBuffer[];

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 BattleEffect_ProcessPendingGraphicsTransfer(void);  // the canvas transfer callback
void BattleEffect_ArmWin0HBlankDma(void); // arms WIN0's HBlank DMA
s32 Task_Add(void *func, s32 order);
s32 Task_Remove(void *func);
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
u32 Random16(void);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);

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
  DrawRectangle drawSplash;
  DrawRectangle drawDrop;
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
  Func_0813ba50();
  REG_BLDCNT = 0x3f44;
  REG_WININ = 0x3337;

  // Drops draw at their maximum; droplets add their light.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_Y));
  drawDrop = heapCache[26];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_Y));
  drawSplash = heapCache[47];

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
        drawDrop(canvas, work->sheet + 0x604, x, y, 24, 24);
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
          drawDrop(canvas, work->sheet + 0x844, x, y, 24, 24);
        else if (drop->variant <= 7)
          drawDrop(canvas, work->sheet + 0xa84, x - 9, y - 9, 42, 42);
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
        drawSplash(canvas, work->sheet + Data_08197424[size - 1],
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
  Func_0813ba50();
}
