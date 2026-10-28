// Draft, differ 66 of 81: starts an effect between objects a and b: allocates its
// work in heap slot 236, sets its two ids and a height (0xa00000 when a is the
// leader, else 0x700000), decompresses its picture (gData_020062d4) into a
// temporary buffer, loads it into VRAM and schedules Func_020051a0.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

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
void Func_020051a0(void);

void Func_020055c4(s32 a, s32 b) {
  struct TrialRoadLink *link = HeapSlot_AllocEwram(236, 0x3934);
  void *buffer = Heap_AllocEwram(576);

  link->a = a;
  link->b = b;
  if (a == gMainRoutineWork.currentOwner)
    link->height = 0xa00000;
  else
    link->height = 0x700000;
  link->unkf0 = 0xf9a00000;
  Actor_GetById(a);
  Actor_GetById(b);
  link->unkda = 0;
  link->unke0 = 0;
  Func_02005770(gData_020062d4, buffer);
  link->vramEntry = Resource_FindFreeEntry();
  VramCache_Load(link->vramEntry, 576, buffer);
  Task_Add(Func_020051a0, 0x476);
  Heap_Release(buffer);
}
