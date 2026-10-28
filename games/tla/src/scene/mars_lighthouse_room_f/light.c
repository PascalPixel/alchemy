#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_020074cc;
extern s16 gData_020074ce;
extern struct FieldActor *gData_020074d0[];
extern s16 gData_020075d0;
extern s16 gData_020075d2;

void *Heap_Alloc(s32 size);
void Func_02005420(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void Heap_Release(void *block);
void Func_02005048(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02005204(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_020074d0, 64);
  Func_02005420(gField_CachedTilesA, buffer);
  gData_020074cc = Resource_FindFreeEntry();
  VramLoad(gData_020074cc, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02005048, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 0;
  gData_020075d2 = -1;
}

void Func_02005284(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_020074d0, 64);
  Func_02005420(gField_CachedTilesB, buffer);
  gData_020074cc = Resource_FindFreeEntry();
  VramLoad(gData_020074cc, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02005048, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 0;
  gData_020075d2 = -1;
}

void Func_02005304(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_020074d0, 64);
  Func_02005420(gField_CachedTilesC, buffer);
  gData_020074cc = Resource_FindFreeEntry();
  VramLoad(gData_020074cc, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02005048, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 1;
  gData_020075d2 = -1;
}
