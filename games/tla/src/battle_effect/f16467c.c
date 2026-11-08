#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_release.h"
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

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

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
  /* FAKEMATCH(barrier): the empty asm spills the canvas before x's copy; as
   * no asm, 2 of 430 instructions differ in tla-en. */
  __asm__("");
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
  /* FAKEMATCH(wrapper): the clear forwarded through ClearWords_, which loads
   * its address before the size's shift; as Iwram_ClearWords called directly,
   * 2 of 430 instructions differ in tla-en. */
  ClearWords_((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
}
