#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02005a80;
extern s16 gData_02005a82;
extern struct FieldActor *gData_02005a84[];
extern s16 gData_02005b84;
extern s16 gData_02005b86;

void *MemAlloc(s32 size);
void Func_0200468c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_0200377c(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02003938(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02005a84, 64);
  Func_0200468c(gField_CachedTilesA, buffer);
  gData_02005a80 = Resource_FindFreeEntry();
  VramLoad(gData_02005a80, 0xc00, buffer);
  MemFree(buffer);
  Task_Add(Func_0200377c, TASK_PASS_VBLANK);
  gData_02005a82 = 0;
  gData_02005b84 = 0;
  gData_02005b86 = -1;
}

void Func_020039b8(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02005a84, 64);
  Func_0200468c(gField_CachedTilesB, buffer);
  gData_02005a80 = Resource_FindFreeEntry();
  VramLoad(gData_02005a80, 0x800, buffer);
  MemFree(buffer);
  Task_Add(Func_0200377c, TASK_PASS_VBLANK);
  gData_02005a82 = 0;
  gData_02005b84 = 0;
  gData_02005b86 = -1;
}

void Func_02003a38(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02005a84, 64);
  Func_0200468c(gField_CachedTilesC, buffer);
  gData_02005a80 = Resource_FindFreeEntry();
  VramLoad(gData_02005a80, 0x800, buffer);
  MemFree(buffer);
  Task_Add(Func_0200377c, TASK_PASS_VBLANK);
  gData_02005a82 = 0;
  gData_02005b84 = 1;
  gData_02005b86 = -1;
}
