#include "trial_road.h"

#include "iwram_call.h"

struct TrialRoadGauge {
  u16 handle;
  u16 field02;
  u8 unk004[0x1c0];
  void *buffer;
};

extern const u8 gField_Tiles64D[];

void *Heap_AllocEwram(s32 size);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 handle, s32 size, void *source);
void Heap_Release(void *block);
void Func_02003da0(void);
void Func_02003f68(void);
void Func_02005770(const void *source, void *destination);
s32 Func_02005a40(void);

static __inline__ void *Pool_Alloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_AllocEwram(size);
}

static __inline__ void Words_Clear(void *destination, s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Iwram_ClearWords(destination, size);
}

void Func_02004188(s32 a, s32 b) {
  struct TrialRoadGauge *gauge =
      (struct TrialRoadGauge *)(gData_020023c4 + 0x188);
  void *buffer;

  if (!GameFlag_Test(0x109))
    Words_Clear(gauge, 0x1c8);
  buffer = Pool_Alloc(0x800);
  Func_02005770(gField_Tiles64D, buffer);
  gauge->handle = Resource_FindFreeEntry();
  VramCache_Load((s16)gauge->handle, 0x800, buffer);
  Heap_Release(buffer);
  gauge->buffer = Heap_AllocEwram(0x3c0);
  gauge->field02 = b;
  Task_Add(Func_02003da0, 0x48d);
  Task_Add(Func_02003f68, 0x480);
  Actor_GetById(Func_02005a40())->priorityFlags |= 0x40;
}
