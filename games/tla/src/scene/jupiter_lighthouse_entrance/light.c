#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002200;
extern s16 gData_02002202;
extern struct FieldActor *gData_02002204[];
extern s16 gData_02002304;
extern s16 gData_02002306;

void *Heap_Alloc(s32 size);
void Func_020019bc(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void Heap_Release(void *block);
void Func_02000aac(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02000c68(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002204, 64);
  Func_020019bc(gField_CachedTilesA, buffer);
  gData_02002200 = Resource_FindFreeEntry();
  VramLoad(gData_02002200, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 0;
  gData_02002306 = -1;
}

void Func_02000ce8(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002204, 64);
  Func_020019bc(gField_CachedTilesB, buffer);
  gData_02002200 = Resource_FindFreeEntry();
  VramLoad(gData_02002200, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 0;
  gData_02002306 = -1;
}

void Func_02000d68(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002204, 64);
  Func_020019bc(gField_CachedTilesC, buffer);
  gData_02002200 = Resource_FindFreeEntry();
  VramLoad(gData_02002200, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000aac, TASK_PASS_VBLANK);
  gData_02002202 = 0;
  gData_02002304 = 1;
  gData_02002306 = -1;
}
