#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002428;
extern s16 gData_0200242a;
extern struct FieldActor *gData_0200242c[];
extern s16 gData_0200252c;
extern s16 gData_0200252e;

void *MemAlloc(s32 size);
void Func_02001a6c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000b5c(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02000d18(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesA, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  VramLoad(gData_02002428, 0xc00, buffer);
  MemFree(buffer);
  Task_Add(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e = -1;
}

void Func_02000d98(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesB, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  VramLoad(gData_02002428, 0x800, buffer);
  MemFree(buffer);
  Task_Add(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e = -1;
}

void Func_02000e18(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesC, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  VramLoad(gData_02002428, 0x800, buffer);
  MemFree(buffer);
  Task_Add(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 1;
  gData_0200252e = -1;
}
