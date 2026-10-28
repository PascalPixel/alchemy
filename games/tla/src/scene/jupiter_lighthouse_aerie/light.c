#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02003380;
extern s16 gData_02003382;
extern struct FieldActor *gData_02003384[];
extern s16 gData_02003484;
extern s16 gData_02003486;

void *Heap_Alloc(s32 size);
void Func_020029a0(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void Heap_Release(void *block);
void Func_02001a78(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02001c34(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003384, 64);
  Func_020029a0(gField_CachedTilesA, buffer);
  gData_02003380 = Resource_FindFreeEntry();
  VramLoad(gData_02003380, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001a78, TASK_PASS_VBLANK);
  gData_02003382 = 0;
  gData_02003484 = 0;
  gData_02003486 = -1;
}

void Func_02001cb4(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003384, 64);
  Func_020029a0(gField_CachedTilesB, buffer);
  gData_02003380 = Resource_FindFreeEntry();
  VramLoad(gData_02003380, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001a78, TASK_PASS_VBLANK);
  gData_02003382 = 0;
  gData_02003484 = 0;
  gData_02003486 = -1;
}

void Func_02001d34(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02003384, 64);
  Func_020029a0(gField_CachedTilesC, buffer);
  gData_02003380 = Resource_FindFreeEntry();
  VramLoad(gData_02003380, 0x800, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001a78, TASK_PASS_VBLANK);
  gData_02003382 = 0;
  gData_02003484 = 1;
  gData_02003486 = -1;
}
