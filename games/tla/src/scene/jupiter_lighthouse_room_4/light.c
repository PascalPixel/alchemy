#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_0200275c;
extern s16 gData_0200275e;
extern struct FieldActor *gData_02002760[];
extern s16 gData_02002860;
extern s16 gData_02002862;

void *Heap_Alloc(s32 size);
void Func_02001d68(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void Heap_Release(void *block);
void Func_02000e58(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02001014(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gField_CachedTilesA, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 0;
  gData_02002862 = -1;
}

void Func_02001094(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gField_CachedTilesB, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 0;
  gData_02002862 = -1;
}

void Func_02001114(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002760, 64);
  Func_02001d68(gField_CachedTilesC, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramLoad(gData_0200275c, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 1;
  gData_02002862 = -1;
}
