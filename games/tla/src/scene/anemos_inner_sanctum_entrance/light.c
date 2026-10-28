#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "task.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesD[];
extern s16 gData_02001cdc;
extern s16 gData_02001cde;
extern struct FieldActor *gData_02001ce0[];
extern s16 gData_02001de0;
extern s16 gData_02001de2;
extern s16 gData_02001de4;

void *MemAlloc(s32 size);
void Func_02000c24(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000858(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02000a98(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02001ce0, 64);
  Func_02000c24(gField_CachedTilesA, buffer);
  gData_02001cdc = Resource_FindFreeEntry();
  VramLoad(gData_02001cdc, 0xc00, buffer);
  MemFree(buffer);
  Task_Add(Func_02000858, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2 = -1;
}

void Func_02000b18(void) {
  u8 *buffer = BumpAlloc(0x1800);

  Iwram_ClearWords(gData_02001ce0, 64);
  Func_02000c24(gField_CachedTilesD, buffer);
  gData_02001cdc = Resource_FindFreeEntry();
  VramLoad(gData_02001cdc, 0x1800, buffer);
  MemFree(buffer);
  Task_Add(Func_02000858, TASK_PASS_VBLANK);
  gData_02001cde = 0;
  gData_02001de0 = 0;
  gData_02001de2 = -1;
  gData_02001de4 = 0;
}
