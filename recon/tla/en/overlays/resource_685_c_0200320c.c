// Near miss, differ 4 of 68 (tla-en, 2026-10-31): scheduler order. In sched2
// the 0x14ccc store is a leaf of cost 1 (store1 under arm9tdmi tuning), so its
// chain ranks 4-3-2-1 against 999's 3-2-1 and 999 starts before the store's
// last add; the native order (add, then 999's first, then the store) needs the
// store ranked 2 or 3, which only a following dependant or a dearer store
// gives. A barrier after the store puts 999 after it (differ 2). Also tried
// (all 4): a for loop with wait(1) as its step, a default case, the sum in a
// temporary, `<= ... break` before the clamp, a break inside the if, an exit
// test inside the loop; permute 4 minutes again found only the one-pass block.
// Draft of overlay 685's Func_0200320c, differ 4 of 68: allocates the
// 0x1078-byte work in heap slot 55, starts Func_02002de4 and raises field106c
// in two steps until it reaches 0x14ccc. The priority temporary is needed (the
// native finishes 0x480 before it loads the task function). Left: in the second step
// the native finishes 0x14ccc (adds r3, #204) before it starts 999 (movs r5,
// #186), then stores; here 999 starts first. sched2 ranks the store one lower
// than the native must. Tried: state as u32, s16 or u16, the value through a
// pointer, >= forms, an if chain (19), state = 999 first (6), a volatile store,
// a register pin, an empty asm or a break after it (4), a one-pass block round
// the store or round state = 999 (2, the store then goes before 999); permute 3
// minutes found only the one-pass block.
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct ChampaCliffCavesWork {
  u8 unk0000[0x1000];
  s32 field1000;
  s32 field1004;
  s32 field1008;
  u8 unk100c[0x58];
  s32 field1064;
  u8 unk1068[4];
  s32 field106c;
  s32 field1070;
  u16 field1074;
  u8 unk1076[2];
};

void *HeapSlot_AllocFar(s32 slot, s32 size);
void Task_Add(void (*func)(void), s32 priority);
void wait(s32 frames);
void Func_02002d34(void);
void Func_02002de4(void);

void Func_0200320c(void) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_AllocFar(220, sizeof(struct ChampaCliffCavesWork));
  s32 state;

  Func_02002d34();
  {
    s32 priority = 0x480;

    Task_Add(Func_02002de4, priority);
  }
  Func_0200362c(144);
  state = 0;
  do {
    switch (state) {
    case 0:
      work->field106c += 0x1000;
      if (work->field106c > 0x6665) {
        work->field106c = 0x6666;
        state = 1;
      }
      break;
    case 1:
      work->field106c += 0x3000;
      if (work->field106c > 0x14ccb) {
        work->field106c = 0x14ccc;
        state = 999;
      }
      break;
    }
    wait(1);
  } while (state != 999);
}
