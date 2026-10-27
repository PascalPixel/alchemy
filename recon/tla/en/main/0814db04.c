// Near miss, differ 132 of 1375 (summon 10, the ice and rock scene).

// ☀️ twin: BattleEffect_RunBoreas (games/tbs/src/effect/effect.c), shape
// 0.915, structure 1.000, ported line for line: the 21 tables are the same
// data under ⚓️'s labels, a max and a transparent blitter from
// BattleEffect_LoadBlitter(104, 0x13) and (188, 0x23), formation 643, the
// Animation_ApplyChildArgumentFar, Render_ApplyProjectedPlacementFar and
// BattleEffectKernels_IntegrateVector2 routines, cue 390 where ☀️ plays 289,
// callbacks at 0xc80, and the camera parameter word 4 through an unsized
// array (sized, its address folds).

// Same frame and instruction count. What is left is order: about 80 rows
// of neighbouring instructions in the other order (sched2) and the pooled
// loads whose offsets move with them; the prologue spills the argument
// (str r0) before the 0x03000000 base in the native.

// Tried (differ):
// trial 2's port (effects trial, 2026-10-15), heap[n] reads          136
// BattleEffect_LoadBlitter declared returning s32 (plan 0c)                     134
// the heap slots as Ram_HeapSlots fields                             132
// the living targets selected from the effect argument in place of
// work->effect (the native reads the work)                         272
// trial 2: alchemy permute 6 minutes, 134 through a global type
// change (not kept); a declaration-order scan, nothing better

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "bg_scroll.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define FORMATION_ICE_BLOCKS 643 // nine objects in a 3x3 block

// Rocks, spray and puffs live in the map cell buffer; the spray's trails
// follow the first 128 particles.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define TRAILS ((struct EffectStep *)(Ram_MapCellBuffer + 0xe00))

struct Scale {
  s32 x;
  s32 y;
};

struct ScaledSprite;

// An object's sprite attributes, as far as this scene sets them.
struct SpriteAttributes {
  u8 unk00[9];
  u8 tileHi : 2;
  u8 priority : 2;
  u8 palette : 4;
};

extern u32 gCameraSceneParameters[];
extern char ResourceId_IceBlockSheet;
extern char ResourceId_SparkleDots; // directory row 0x17e
extern char ResourceId_VioletSkyBackdrop;
extern u8 Data_08198260[][2];      // ☀️ gBoreas_Points
extern s8 Data_08198280[];         // ☀️ gBoreas_ShakeOffsets
extern u16 Data_08198284[];        // ☀️ gBoreas_RockCells
extern u8 Data_0819828a[];         // ☀️ gBoreas_RockWidths
extern u8 Data_0819828d[];         // ☀️ gBoreas_RockHeights
extern u8 Data_08198290[];         // ☀️ gBoreas_Animations
extern struct Scale Data_08196e14; // ☀️ gBattleFx10_UnitScale
extern u16 Data_081982aa[];        // ☀️ gBoreas_DebrisCells
extern u8 Data_08198294[];         // ☀️ gBoreas_DebrisWidths
extern u8 Data_0819829f[];         // ☀️ gBoreas_DebrisHeights
extern u16 Data_081982c6[];        // ☀️ gBoreas_SprayCells
extern u8 Data_081982c0[];         // ☀️ gBoreas_SprayWidths
extern u8 Data_081982c3[];         // ☀️ gBoreas_SprayHeights
extern u8 Data_0819745e[];         // ☀️ gBattleEffect_PuffSizes
extern u16 Data_0819744c[];        // ☀️ gBattleEffect_PuffCells
extern u16 Data_081982cc[];        // ☀️ gBoreas_BoulderCells
extern u8 Data_081982d2[];         // ☀️ gBoreas_BoulderWidths
extern u8 Data_081982d5[];         // ☀️ gBoreas_BoulderHeights
extern u16 Data_081982de[];        // ☀️ gBoreas_FallCells
extern u8 Data_081982d8[];         // ☀️ gBoreas_FallWidths
extern u8 Data_081982db[];         // ☀️ gBoreas_FallHeights

void BattleEffect_StartCanvasLayer(s32 bgControl);
void Func_0813ba50(void);                // ☀️ BattleEffect_ConfigureEffectDisplay
void BattleEffect_ProcessPendingGraphicsTransfer(void);                // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_EndCanvasLayer(void);                // ☀️ BattleEffect_EndCanvasLayer
void Func_08163c2c(s32 mode, s32 value); // ☀️ Func_08163c2c
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);  // ☀️ BattleEffect_BuildBlitter
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // ☀️ SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 formation, s32 priority); // ☀️ SpawnObjects
void BattleBackground_LoadFar(s32, s32, s32);
void BattleEffect_BuildWindowEdgeTable(void);
void Audio_PlayCue(s32 cue);
void Animation_ApplyChildArgumentFar(void *, s32);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0814cca8(void); // ☀️ BattleEffect_SetupBlendedDisplay
void Render_ResetTransformState(void);
void Func_081180e8(s32 phase); // ☀️ BattleEvent_StartPhaseFar
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y);    // ☀️ Camera_ApplyShake

// Summon 10: a window opens over a violet sky on a field of rocks; a 3x3
// block of ice rises, a great ice block falls into it and throws up debris
// and spray (A or B skips to the second half). Then ice blocks rain on the
// targets, boulders pile up and burst, and their pieces bounce on the
// ground among puffs of smoke.
void BattleEffect_RunBoreas(struct BattleEffectArgument *effect) {
  s32 pts[32][2];
  s32 pos[4];
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle draw;
  DrawRectangle drawAlt;
  u8 *dots;
  s32 step;
  s32 row;
  s32 count;
  s32 i;
  s32 j;
  s32 ox;
  s32 oy;
  s32 y;
  s32 t;
  struct EffectStep *p;
  struct EffectStep *q;
  struct Scale scale;

  // Set up: a blank canvas over a black backdrop, the ice sheet and the
  // sparkle dots, a max and a transparent blitter, the violet sky and the
  // nine ice objects.
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  dots = Ram_HeapSlots->unk64[0];
  step = 1;
  BattleEffect_StartCanvasLayer(0);
  Func_0813ba50();
  *(u16 *)0x05000000 = 0;
  *(u16 *)0x05000002 = 0;
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);
  REG_WININ = 0x2137;
  Func_08163c2c(1, 0);
  REG_WIN0H = 0xf0f0;
  Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  drawAlt = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  WaitTicks(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
  BattleEffect_SpawnObjects(9, FORMATION_ICE_BLOCKS, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 50;

  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = (Random16() & 63) + 32;
    PARTICLES[i].y = (Random16() & 31) + 120;
    PARTICLES[i].variant = -1;
  }

  // A working copy of the rock points (the scene draws from the table).
  for (i = 0; i != 32; i++) {
    pts[i][0] = Data_08198260[i][0];
    pts[i][1] = Data_08198260[i][1];
  }

  Audio_PlayCue(141);

  // The first half: rocks, the great ice block and its spray.
  for (frame = 0; frame != 288; frame++) {
    count = 16;
    if ((gInput.repeat & 3) && frame > 16)
      break;

    // Open the window from the left: its right edge speeds up, and each
    // line is pulled in by up to 63 pixels.
    if (frame >= 0 && frame < 16) {
      u16 *edge = (u16 *)Ram_MapCellBuffer;

      if (frame == 1) {
        u8 *inset = Ram_MapCellBuffer + 2;
        s32 n;

        for (n = 0; n != 128; n++)
          inset[n] = Random16() & 63;
        *edge = 0;
        Scheduler_AddOrUpdateCallback(BattleEffect_BuildWindowEdgeTable,
                                      CALLBACK_ORDER);
      }
      *edge += step;
      step += 3;
      if (frame == 15)
        Scheduler_RemoveCallback(BattleEffect_BuildWindowEdgeTable);
    }

    // The rocks still standing.
    if (frame > 103)
      count = 0;
    else if (frame > 63)
      count = 6;
    else if (frame > 31)
      count = 10;

    // The ground trembles until frame 167; at 176-179 it jolts as the
    // table says.
    if (frame <= 167) {
      ox = (Random16() & 3) - 1;
      oy = (Random16() & 3) - 1;
      gBgScroll[1].x = ox;
      gBgScroll[1].y = oy + 32;
    } else {
      ox = 0;
      oy = 0;
      gBgScroll[1].x = 0;
      gBgScroll[1].y = 32;
    }
    if (frame >= 176 && frame < 180) {
      ox = -Data_08198280[frame - 176];
      oy = Data_08198280[frame - 176];
      gBgScroll[1].x = ox;
      gBgScroll[1].y = oy + 32;
    }

    for (i = 0; i != count; i++) {
      s32 n = i % 3;

      draw(canvas, work->sheet + Data_08198284[n], Data_08198260[i][0] - ox,
           Data_08198260[i][1] - Data_0819828d[n] - oy, Data_0819828a[n],
           Data_0819828d[n]);
    }

    // The nine ice objects in a 3x3 block, 32 pixels apart.
    scale = Data_08196e14;
    if (frame == 174) {
      for (i = 0; i != 9; i++) {
        ((struct SpriteAttributes *)work->objects[i])->priority = 0;
      }
    }
    if (frame > 208)
      Animation_ApplyChildArgumentFar(work->objects[2],
                                      Data_08198290[(frame / 4) & 3]);

    scale.x = 0x10000;
    scale.y = 0x10000;
    pos[3] = 0;
    pos[1] = 255 << 16;
    for (row = 0; row != 3; row++) {
      for (j = 0; j != 3; j++) {
        pos[0] = (j * 32 + 144 - ox) << 16;
        pos[2] = (row * 32 + 76 - oy) << 16;
        Render_ApplyProjectedPlacementFar(work->objects[row * 3 + j], pos,
                                          &scale.x, 0);
      }
    }

    // The great ice block (24x48) drops in from the upper right at 160-175
    // and sinks slowly from 208.
    t = frame - 160;
    if (t >= 0 && t < 158) {
      s32 u = frame - 208;
      s32 sx = 80;
      s32 sy = 8;

      if (frame <= 175) {
        sx = 96 - t;
        sy = t * 4 - 56;
      } else if (frame > 208) {
        sy = u / 4 + 8;
      }
      drawAlt(canvas, work->sheet + 0xc46, sx, sy, 24, 48);
    }

    if (frame == 32)
      Audio_PlayCue(134);
    if (frame == 64)
      Audio_PlayCue(134);
    if (frame == 104)
      Audio_PlayCue(134);
    if (frame == 176)
      Audio_PlayCue(134);
    if (frame == 226)
      Audio_PlayCue(145);
    Render_ResetTransformState();

    // Each time rocks crumble, 32 pieces of debris fly up.
    if (frame == 32) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 127) - 64) << 11;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 64) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
        PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
        PARTICLES[i + 32].velocityX = ((Random16() & 127) - 63) << 12;
        PARTICLES[i + 32].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i + 32].variant = (Random16() & 15) + 32;
      }
    }
    if (frame == 104) {
      for (i = 0; i != 32; i++) {
        PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
        PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
        PARTICLES[i].velocityX = ((Random16() & 127) - 63) << 11;
        PARTICLES[i].velocityY = ((-Random16() & 31) - 32) << 13;
        PARTICLES[i].variant = (Random16() & 15) + 32;
      }
    }

    if (frame >= 32 && frame < 208) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0) {
          s32 n;

          if (frame > 191)
            n = i % 7 + 4;
          else
            n = i & 3;
          drawAlt(canvas, work->sheet + Data_081982aa[n], HI(PARTICLES[i].x),
                  HI(PARTICLES[i].y), Data_08198294[n], Data_0819829f[n]);
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          PARTICLES[i].velocityY += 0x2000;
        }
      }
    }

    // The spray: 128 drops from (72, 56) down to the left; from 228 each
    // leaves a trail of puffs.
    if (frame > 223) {
      if (frame == 224) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = 72 << 16;
          PARTICLES[i].y = 56 << 16;
          PARTICLES[i].velocityX = (-(Random16() & 127) - 64) << 11;
          PARTICLES[i].velocityY = ((Random16() & 127) + 16) << 11;
          PARTICLES[i].variant = Random16();
        }
      }
      for (i = 0; i != 128; i++) {
        if (frame >= i / 4 + 224) {
          s32 n = i % 3;

          if ((i & 1) == 0) {
            draw(canvas, work->sheet + Data_081982c6[n], HI(PARTICLES[i].x),
                 HI(PARTICLES[i].y), Data_081982c0[n], Data_081982c3[n]);
          }
          PARTICLES[i].x += PARTICLES[i].velocityX;
          PARTICLES[i].y += PARTICLES[i].velocityY;
          if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
            PARTICLES[i].x = 72 << 16;
            PARTICLES[i].y = 56 << 16;
          }
          PARTICLES[i].variant++;
        }
      }
      if (frame == 228) {
        for (i = 0; i != 128; i++) {
          p = &PARTICLES[i];
          q = &TRAILS[i];
          q->x = p->x;
          q->y = p->y;
          q->variant = 0;
        }
      }
      for (i = 0, q = TRAILS, p = PARTICLES; i != 128; i++, q++, p++) {
        if (frame >= i + 228) {
          s32 n = q->variant / 2 % 9;
          s32 sz = Data_0819745e[n];

          draw(canvas, dots + Data_0819744c[n], HI(q->x) - sz / 2,
               HI(q->y) - sz / 2, sz, sz);
          if (++q->variant == 18) {
            q->x = p->x;
            q->y = p->y;
            q->variant = 0;
          }
        }
      }
    }

    work->transferPending = 1;
    WaitTicks(1);
  }

  // The second half: ice rains on the targets, boulders pile up and burst.
  for (i = 0; i != 9; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  Func_0814cca8();
  REG_BLDALPHA = 0x1010;
  Audio_PlayCue(390);

  for (i = 0; i != 64; i++) {
    work->particles[i].x = ((Random16() & 127) + 64) << 16;
    work->particles[i].z = -(16 << 16);
    work->particles[i].y = (-(Random16() & 127) - 64) << 16;
    work->particles[i].velocityX = (-(Random16() & 63) - 127) << 12;
    work->particles[i].velocityY = ((Random16() & 63) + 127) << 12;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != 64; i++) {
    PARTICLES[i].x = Random16() & 127;
    PARTICLES[i].y = (Random16() & 63) + i / 2;
    PARTICLES[i].variant = -i / 2;
  }

  for (frame = 0; frame != 146; frame++) {
    count = 0;
    if (frame == 96)
      Func_081180e8(134);
    if (frame == 16)
      work->shakeFrames = 32;
    // One boulder every two frames, up to 16.
    if (frame > 16) {
      count = (frame - 16) / 2;
      if (count > 16)
        count = 16;
    }
    if (frame >= 9 && frame < 72 && (frame & 3) == 0)
      Audio_PlayCue(132);
    if (frame == 72)
      Audio_PlayCue(145);
    // A block (40x80) falls in from the upper right.
    if (frame > 64)
      draw(canvas, work->sheet + 0x14f9, (64 - frame) * 7 + 88,
           frame * 14 - 992, 40, 80);
    if (frame <= 71) {
      for (i = 0; i != count; i++) {
        s32 n = i % 3;

        draw(canvas, work->sheet + Data_081982cc[n], Data_08198260[i][0] - 56,
             Data_08198260[i][1] - Data_081982d5[n], Data_081982d2[n],
             Data_081982d5[n]);
      }
    }
    // The boulders burst: 64 pieces fly up from where they stood.
    if (frame == 72) {
      for (i = 0; i != 64; i++) {
        work->particles[i].x = (Data_08198260[i & 15][0] - 56) << 16;
        work->particles[i].y = Data_08198260[i & 15][1] << 16;
        work->particles[i].velocityX = ((Random16() & 127) - 63) << 13;
        work->particles[i].velocityY = (-(Random16() & 31) - 16) << 14;
      }
      if (frame == 72)
        work->shakeFrames = 4;
    }
    // The pieces: those falling fast bounce at a quarter speed at y = 120.
    if (frame > 71) {
      count = 32;
      if (frame != 72)
        count = 64;
      for (i = 0; i != count; i++) {
        q = &work->particles[i];
        y = HI(q->y);
        if (y <= 135) {
          s32 n = i % 3;

          draw(canvas, work->sheet + Data_081982de[n], HI(q->x),
               y - Data_081982db[n], Data_081982d8[n], Data_081982db[n]);
          BattleEffectKernels_IntegrateVector2(q, 64, 1 << 16);
          if (HI(q->y) > 120 && q->velocityY > 8 << 16) {
            q->velocityY = -q->velocityY / 4;
            q->y = 120 << 16;
            work->shakeFrames = 1;
          }
        }
      }
    }
    // The falling ice blocks (24x48), sent up again until frame 47.
    if (frame <= 71) {
      for (i = 0; i != 64; i++) {
        draw(canvas, work->sheet, HI(work->particles[i].x) - 12,
             HI(work->particles[i].y) - 24, 24, 48);
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
        if ((work->particles[i].y >> 16) > 120 && frame <= 47) {
          work->particles[i].x = ((Random16() & 127) + 64) << 16;
          work->particles[i].y = -(16 << 16);
        }
      }
    }

    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 32)
        Func_0814cd48(effect->actors[i], 9, 5, -1, 0);
    }

    // Puffs of smoke, sent up again until frame 127.
    if (frame > 72) {
      for (i = 0; i != 64; i++) {
        if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18) {
          s32 n = PARTICLES[i].variant / 2 % 9;
          s32 sz = Data_0819745e[n];

          draw(canvas, dots + Data_0819744c[n], PARTICLES[i].x - sz / 2,
               PARTICLES[i].y - sz / 2, sz, sz);
        }
        if (++PARTICLES[i].variant == 18 && frame <= 127) {
          PARTICLES[i].x = Random16() & 127;
          PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
          PARTICLES[i].variant = 0;
        }
      }
    }

    if (frame >= 72 && frame < 80)
      Func_08158ce0(8, 8);
    else
      Func_08158ce0(2, 2);
    work->transferPending = 1;
    WaitTicks(1);
  }

  // Tear down.
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
