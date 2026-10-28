// Under -mthumb-slow-loads (2026-10-26): differ 6 of 430, with no device:
// the BG2PA store is plain and the empty asm at the start is gone (both
// matched no better). Left, two places: the native spills formation and y
// (str r0, str r2) before it starts the RAM base, here after the base's
// first half, and spills the canvas (str r1, [sp, #24]) right after copying
// the work, here three rows later; at the end it loads Iwram_ClearWords'
// address (ldr r3) before the size's shift, here after. An empty asm or
// one-pass block at the start stores the arguments first but moves x's copy
// to fp up as well (6); the heap reads in the other order (18).

// Draft, differ 44 of 430 (2026-10-15; 46 before BattleEffect_LoadBlitter returned its
// loader's s32, as nibble_pair.c now does): The Broken Seal's
// BattleEffect_RunImpactBurst with The Lost Age's blitter loads
// (BattleEffect_LoadBlitter(104, 19) and (188, 3), read back from the heap slots at 0x68
// and 0xbc), its resources (0x134, 0x120, 0x121) and callback (BattleEffect_ProcessPendingGraphicsTransfer at
// 0xc80). Everything matches but reordered rows, all of one kind: the native
// puts a store, a load or a pool load before an independent constant (the
// arguments' spills before 0x03000000, the spills of the heap reads, movs r1,
// #19 before movs r0, #104, the 0xffff angle mask after the particle stores,
// the pooled sheet offsets), and the shifted literal pools follow. The empty
// asm at the start and the BG2PA halfword pinned in a one-pass block (as
// BattleEffect_RunUndeadSword) take it from 54 to 48, and the tables declared without const
// to 46.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "system.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "task.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])
#define SHARD_E 0x59d8

extern char ResourceId_Data134;
extern char ResourceId_FlareSheet;
extern char ResourceId_FlareImage;
extern u16 gData_081989f2[];
extern u8 gData_081989e2[];
extern u8 gData_081989ea[];
extern u16 gData_08197410[];

void BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 kind);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Sound_PlayFar(s32 cue);
void Func_0816442c(s32 formation, s32 x, s32 y);

void Func_0816467c(s32 formation, s32 x, s32 y) {
  struct BattleFxWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct EffectStep *p;
  s32 frame;
  s32 i;

  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  orgX = x;
  x += 40 << 16;
  x /= 2;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BLDCNT = 0x3f46;
  BattleEffect_LoadBlitter(104, 19);
  maxBlend = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, 3);
  additive = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareImage, work->sheet + SHARD_E,
                             0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (i = 0; i != 64; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & 0xffff;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  for (i = 0; i != 3; i++) {
    work->particles[61 + i].x = x;
    work->particles[61 + i].y = y;
    work->particles[61 + i].velocityX = (Trig_Sin(i * 0x5555) * 32) >> 6;
    work->particles[61 + i].velocityY = -((Trig_Cos(i * 0x5555) * 32) >> 5);
  }

  for (i = 0; i != 64; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & 0xffff;
    struct EffectStep *cells = (struct EffectStep *)Ram_MapCellBuffer;

    cells[i].x = x;
    cells[i].y = y;
    cells[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    cells[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    cells[i].variant = (Random16() & 15) + 20;
  }

  for (frame = 0; frame != 72; frame++) {
    if (frame == 4)
      Sound_PlayFar(154);
    if (frame == 32)
      Sound_PlayFar(212);

    if (frame <= 47) {
      s32 cell = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (cell < 0)
        cell = 0;
      maxBlend(canvas, work->sheet + gData_081989f2[cell],
               (x >> 16) - ((width = gData_081989e2[cell]) >> 1),
               (y >> 16) - ((height = gData_081989ea[cell]) >> 1), width,
               height);
    }

    for (i = 0, p = work->particles; i != 30; i++, p++) {
      if (frame > i / 2 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        BattleEffectKernels_IntegrateVector2(p, 60, 0);
        size = p->variant / 16 + 3;
        px = HI(p->x);
        py = HI(p->y);
        additive(canvas, sheet + gData_08197410[size - 1], px - size / 2,
                 py - size, size, size * 2);
      }
    }

    for (i = 0, p = (struct EffectStep *)Ram_MapCellBuffer; i != 60; i++, p++) {
      if (frame > 35 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        BattleEffectKernels_IntegrateVector2(p, 60, 0);
        size = p->variant / 16 + 1;
        px = HI(p->x);
        py = HI(p->y);
        additive(canvas, sheet + gData_08197410[size - 1], px - size / 2,
                 py - size, size, size * 2);
      }
    }

    for (i = 0; i != 3; i++) {
      struct EffectStep *shard = &work->particles[61 + i];

      if (frame >= 36 && frame <= 63) {
        s32 cell;
        s32 px;
        s32 py;

        BattleEffectKernels_IntegrateVector2(shard, 64, 0);
        cell = (frame - 36) / 7;
        px = HI(shard->x);
        py = HI(shard->y);
        maxBlend(canvas, work->sheet + cell * 0x120 + SHARD_E, px - 6, py - 12,
                 12, 24);
      }
    }

    if (frame <= 35)
      Func_0816442c(formation, orgX, y);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
}
