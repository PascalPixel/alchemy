// Draft, differ 803 of 1022 (draft 1015 instructions, no device): the body
// of the canvas burst family (BattleEffect_RunCanvasMode0,
// BattleEffect_RunCanvasModeByCount and BattleEffect_RunCanvasMode3WithAction in
// canvas_modes.c), written from the listing on The Broken Seal's canvas
// skeleton (twins: BattleEffect_RunRamses 0.44, by call
// order BattleEffect_RunSparkGroups 0.44).
// Every call, table and branch of the four modes is in the native's order;
// the four quarter blits of a disc reload the second blitter for each flip
// (flags 3, 7, 11, 15 with mode 1), as the native does.
// Left: the frame. The native's is 88 bytes: effect sp+60, mode sp+56, work
// sp+52, canvas sp+48, frame sp+44, from sp+76, to sp+64, draw sp+36,
// graphics sp+32, the disc table sp+28, frames sp+24, &ball sp+20, &to
// sp+16, &from sp+12, and j spilled at sp+8 across the rays' Trig_Sin;
// here it is 84 (graphics at sp+40, the rest 4 lower), and the loops'
// registers follow.
// Next: j's spill (the native keeps the ray index in memory across the two
// trig calls), the disc table and draw as the native's spills.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "bg_scroll.h"
#include "battle_work.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The integer (pixel) half of a 16.16 value, read as the halfword it is.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FlashBurstSheet;
extern u8 gMapCellBuffer[];
extern u16 gData_08197410[];
extern const u8 Data_08199ec5[];

void BattleEffect_StartCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_LoadBlitter(s32 slot, s32 value);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Palette_StepFadeTransfer(void);
void Func_0815b434(u8 *map, s32 size, s32 scale);
void Func_0815e22c(struct EffectPosition *position);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void HeapSlot_Release(s32 slot);
void WaitFrames(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 mode, s32 timer, s32 frames);
void Func_08118088(s32 actor, s32 mode);
void Func_08158ce0(s32 randomMask, u32 range);
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);
void SummonEffect_RunCanvasMode1(struct BattleEffectArgument *effect);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);

// Draws a disc of the given size from the disc sheet in the map cell buffer
// as four quarter blits about (x, y), with the second blitter loaded for
// each quarter's flip (flags 3, 7, 11 and 15 of the maximum mode).
#define DRAW_DISC_QUARTER(flags, left, top)                                    \
  do {                                                                         \
    BattleEffect_LoadBlitter(188, 1 << 4 | (flags));                                      \
    ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, gMapCellBuffer + *disc,   \
                                             (left), (top), half, size);       \
    HeapSlot_Release(188);                                             \
  } while (0)

// The canvas burst family (wrappers in canvas_modes.c). A glowing ball (or
// in mode 0 a growing glow at the target, with sixteen rays that close on
// it) hits the target; at the hit the targets flash and recoil, sixteen
// flames rise and 47 sparks scatter. Modes:
//   0  (BattleEffect_RunCanvasMode0): 78 frames, the glow at the target; the
//      hit at frame 54, then the canvas is cleared and SummonEffect_RunCanvasMode1
//      runs on;
//   1  (by count, kinds up to 199): 66 frames, a small ball (24 pixels),
//      half-speed sparks, hit at 32, no flames;
//   2  (by count, kinds 200 and up): 66 frames, a large ball (40), hit at
//      32;
//   3  (BattleEffect_RunCanvasMode3WithAction): 58 frames, the ball flies to the
//      midpoint of the targets, its own palette, hit at 24.
// Modes 1 and 2 close by restoring the battle display themselves.
void Func_0818f620(struct BattleEffectArgument *effect, u32 mode) {
  struct EffectPosition from;
  struct EffectPosition to;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw;
  s16 *discs;
  struct EffectStep *ball;
  struct EffectStep *step;
  s32 frames;
  s32 hitFrame;
  s32 frame;
  s32 offset;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  if (mode <= 1)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  discs = (s16 *)work->sheet;
  frames = 78;
  if (mode != 0) {
    frames = 58;
    if (mode != 3)
      frames = 66;
  }
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);

  // Discs of sizes 2 to 64 one after another in the map cell buffer, with
  // their offsets at the start of the sheet.
  offset = 0;
  for (i = 0; i != 32; i++) {
    s32 size = i * 2 + 2;

    Func_0815b434(gMapCellBuffer + offset, size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);

  ball = &work->particles[63];
  for (frame = 0; frame != frames; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &from);
    if (mode == 3)
      Func_0815e22c(&to);
    else
      EffectPosition_ProjectBattler(effect->actors[0], &to);
    if (mode == 0 && frame == 0)
      Sound_PlayFar(140);

    if (frame == 0) {
      if (mode == 0) {
        // The rays: one every 1/16 turn, each starting after its own delay.
        for (i = 0; i != 16; i++) {
          step = &work->particles[i];
          step->z = i << 12;
          step->variant = (Random16() & 31) + 16;
        }
      } else {
        // The ball leaves the caster's chest, aimed to arrive in 128
        // frames' worth of steps, already seven steps along.
        ball->x = (from.x / 2) << 16;
        ball->y = (from.y - 36) << 16;
        ball->velocityX = (to.x - from.x) << 10;
        ball->velocityY = (to.y - from.y) << 10;
        ball->x += ball->velocityX * 7;
        ball->y += ball->velocityY * 7;
      }
    }

    if (mode == 0) {
      // Frames 18-54: the glow grows at the target, up to 64 pixels.
      if ((u32)(frame - 18) <= 36) {
        s32 size = frame * 2 - 32;
        s32 half;
        s32 x;
        s32 y;
        s16 *disc;

        if (size > 64)
          size = 64;
        x = to.x / 2;
        y = to.y - 32;
        disc = &discs[(size - 2) / 2];
        half = size >> 1;
        DRAW_DISC_QUARTER(3, x - half, y - size);
        DRAW_DISC_QUARTER(7, x, y - size);
        DRAW_DISC_QUARTER(11, x - half, y);
        DRAW_DISC_QUARTER(15, x, y);
      }
    } else if ((u32)(frame - 2) <= 30) {
      // Frames 2-32: the ball flies, growing to 24 (mode 1) or 40 pixels.
      s32 size = frame / 4 * 2 + 16;
      s32 half;
      s32 x;
      s32 y;
      s16 *disc;

      if (mode == 1) {
        if (size > 24)
          size = 24;
      } else if (size > 40) {
        size = 40;
      }
      x = ball->x >> 16;
      y = ball->y >> 16;
      ball->x += ball->velocityX;
      ball->y += ball->velocityY;
      disc = &discs[(size - 2) / 2];
      half = size >> 1;
      DRAW_DISC_QUARTER(3, x - half, y - size);
      DRAW_DISC_QUARTER(7, x, y - size);
      DRAW_DISC_QUARTER(11, x - half, y);
      DRAW_DISC_QUARTER(15, x, y);
    }

    // Mode 0, frames 0-53: sixteen rays of motes close on the target.
    if (mode == 0 && frame <= 53) {
      step = work->particles;
      for (i = 0; i != 16; i++) {
        if ((u32)(--step->variant + 32) <= 47) {
          for (j = 0; j != 16; j++) {
            s32 radius = (step->variant * 2 + j) * 2;

            if (radius >= 0) {
              s32 x = to.x / 2 + ((radius * Trig_Sin(step->z)) >> 16);
              s32 y = to.y + ((radius * Trig_Cos(step->z)) >> 16);

              draw(canvas, graphics + gData_08197410[8 - 1], x - 2, y - 36, 4,
                   8);
            }
          }
        }
        step++;
      }
    }

    if (mode == 0)
      hitFrame = 54;
    else if (mode == 3)
      hitFrame = 24;
    else
      hitFrame = 32;
    if (frame >= hitFrame) {
      if (frame == hitFrame) {
        // The hit: the targets recoil (mode 1 keeps them still) and flash.
        if (mode == 0 || mode == 2 || mode == 3) {
          Func_081180e8(145);
          for (i = 0; i != effect->count; i++)
            Func_08118088(effect->actors[i], 4);
        } else {
          Func_081180e8(133);
          for (i = 0; i != effect->count; i++)
            Func_08118088(effect->actors[i], 0);
        }
        for (i = 0; i != effect->count; i++)
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        work->shakeFrames = 8;

        // Sixteen flames, faster one by one.
        step = work->particles;
        for (i = 0; i != 16; i++) {
          s32 angle = Random16() & 0xffff;
          s32 speed = i * 4 + 4;

          step->x = (to.x / 2) << 16;
          step->y = (to.y - 32) << 16;
          step->velocityX = (speed * Trig_Sin(angle)) >> 3;
          step->velocityY = -(speed * Trig_Cos(angle)) >> 3;
          step->variant = i / 3 + 17;
          step++;
        }

        // 47 sparks, at half speed in mode 1.
        step = &work->particles[16];
        for (i = 0; i != 47; i++) {
          s32 speed = Random16() & 255;
          s32 angle = Random16() & 0xffff;

          speed += 128;
          step->x = (to.x / 2) << 16;
          step->y = (to.y - 32) << 16;
          step->velocityX = (speed * Trig_Sin(angle)) >> 6;
          step->velocityY = -(speed * Trig_Cos(angle)) >> 5;
          if (mode == 1) {
            step->velocityX /= 2;
            step->velocityY /= 2;
          }
          step->variant = i / 3 + 17;
          step++;
        }
      }

      // The flames: six frames of the burst sheet, three frames each.
      if (mode == 0 || mode == 2 || mode == 3) {
        step = work->particles;
        for (i = 0; i != 16; i++) {
          s32 x = HI(step->x);
          s32 y = HI(step->y);

          BattleEffectKernels_IntegrateVector2(step, 60, 0);
          if ((u32)step->variant <= 17)
            draw(canvas,
                 work->sheet + 0x800 + (Data_08199ec5[step->variant / 3] << 11),
                 x - 16, y - 32, 32, 64);
          step->variant = step->variant > 0 ? step->variant - 1 : -1;
          step++;
        }
      }

      // The sparks shrink as they fly.
      step = &work->particles[16];
      for (i = 0; i != 47; i++) {
        if (step->variant >= 0) {
          s32 size = (step->variant >> 2) + 1;

          draw(canvas, graphics + gData_08197410[size - 1],
               HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(step, 60, 0);
          step->variant--;
        }
        step++;
      }
    }

    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  if (mode == 0) {
    Iwram_ClearWords((void *)0x06004000, 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode1(effect);
  } else if (mode == 3) {
    BattleEffect_EndCanvasLayer();
  } else {
    // Back to the battle display: BG1's scroll, BG2 as before, the palette
    // fade off and the backgrounds on again.
    struct BattleWork *battle = Ram_HeapSlots->battleWork;

    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    REG_BG2CNT = 0x787;
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Task_Remove(Palette_StepFadeTransfer);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    REG_BLDCNT = 0;
    WaitFrames(1);
    BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
    WaitFrames(1);
  }
}
