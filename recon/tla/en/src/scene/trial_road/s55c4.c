#include "types.h"
#include "call.h"
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
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, SPLIT_CONSTANT(0x3934));
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  s32 owner;
  u16 *unkda;
  u16 *unke0;
  s32 *height;
  s32 *unkf0;
  s16 *vramEntry;

  link->a = a;
  link->b = b;
  owner = gMainRoutineWork.currentOwner;
  height = &link->height;
  unkf0 = &link->unkf0;
  unkda = &link->unkda;
  unke0 = &link->unke0;
  vramEntry = &link->vramEntry;
  if (a == owner) {
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
  Task_Add(Func_020051a0, SPLIT_CONSTANT(0x476));
  Heap_Release(buffer);
}
