// Measured under -Os, 5 November 2026: differ 16 (tla-en); notes below may predate it.
// Draft, differ 16 of 81, skeleton gap 6 (tla-en, 2026-11-01)
// Starts an effect between objects a and b: allocates its work in heap slot 236,
// sets its two ids and a height (0xa00000 when a is the leader, else 0x700000),
// decompresses its picture (gData_020062d4) into a temporary buffer, loads it
// into VRAM and schedules Func_020051a0.
// Left: one scheduling and allocation block, all before the leader test. The
// native loads gMainRoutineWork first (scratch r3) and sets unkda in r8 then
// unke0 in r10; here the field addresses take r3 first, and unke0 has to be set
// before unkda for unkda to outrank it. Every order of the five address locals,
// before and after the id stores, was measured: this is the best, and unkda
// before unke0 gives 18.
// One step closes it: `s32 owner; owner = gMainRoutineWork.currentOwner;` above
// the address locals, in the order height, unkf0, unkda, unke0, vramEntry, with
// `a == owner`, gives differ 0. owner is read once and only moves the load, so it
// is a forced temporary (S2 `temporary`), left out while no device may be added.
// Tried instead: s32 *owner, where cse folds the pool base into one literal
// (differ 5), a MainRoutineWork pointer (14), the compare reversed (18).
// What reached the rest: the addresses are one-address locals, so the native
// computes them before the branch; zero is one constant, set at its stores, which
// gives movs rather than a pool load; both arms are written in full and
// cross-jumped, which keeps the unkf0 store ahead of the call's argument; Task_Add
// returns s32, and trial_road.h's void moves an argument load, so this draft takes
// the plain headers.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct TrialRoadLink {
  u8 unk00[0xd8];
  s16 vramEntry;
  u16 unkda;
  u8 unkdc[4];
  u16 unke0;
  u16 a;
  u16 b;
  u8 unke6[6];
  s32 height;
  s32 unkf0;
};

extern const u8 gData_020062d4[];

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void *Heap_AllocEwram(s32 size);
void Func_02005770(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Heap_Release(void *block);
s32 Task_Add(void (*func)(void), s32 order);
struct Actor *Actor_GetById(s32 id);
void Func_020051a0(void);

void Func_020055c4(s32 a, s32 b) {
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, 0x3934);
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  u16 *unkda;
  u16 *unke0;
  s32 *height;
  s32 *unkf0;
  s16 *vramEntry;

  link->a = a;
  link->b = b;
  height = &link->height;
  unkf0 = &link->unkf0;
  unke0 = &link->unke0;
  unkda = &link->unkda;
  vramEntry = &link->vramEntry;
  if (a == gMainRoutineWork.currentOwner) {
    *height = 0xa00000;
    *unkf0 = 0xf9a00000;
  } else {
    *height = 0x700000;
    *unkf0 = 0xf9a00000;
  }
  Actor_GetById(a);
  Actor_GetById(b);
  zero = 0;
  *unkda = zero;
  *unke0 = zero;
  Func_02005770(gData_020062d4, buffer);
  *vramEntry = Resource_FindFreeEntry();
  VramCache_Load(*vramEntry, 576, buffer);
  Task_Add(Func_020051a0, 0x476);
  Heap_Release(buffer);
}
