#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_020023f0;
extern s16 gData_020023f2;
extern struct FieldActor *gData_020023f4[];
extern s16 gData_020024f4;
extern s16 gData_020024f6;

void *Heap_Alloc(s32 size);
void Func_02001bac(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02000c94(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000e50(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_020023f4, 64);
  Func_02001bac(gField_CachedTilesA, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  VramCache_Load(gData_020023f0, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4 = 0;
  gData_020024f6 = -1;
}

void Func_02000ed0(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_020023f4, 64);
  Func_02001bac(gField_CachedTilesB, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  VramCache_Load(gData_020023f0, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4 = 0;
  gData_020024f6 = -1;
}

void Func_02000f50(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_020023f4, 64);
  Func_02001bac(gField_CachedTilesC, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  VramCache_Load(gData_020023f0, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4 = 1;
  gData_020024f6 = -1;
}
