#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002298;
extern s16 gData_0200229a;
extern struct FieldActor *gData_0200229c[];
extern s16 gData_0200239c;
extern s16 gData_0200239e;

void *MemAlloc(s32 size);
void Func_02001a4c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000b3c(void);
struct FieldActor *Object_GetById(s32 id);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  VramCache_Load(entry, size, source);
}

void Func_02000cf8(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200229c, 64);
  Func_02001a4c(gField_CachedTilesA, buffer);
  gData_02002298 = Resource_FindFreeEntry();
  VramLoad(gData_02002298, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000b3c, 1152);
  gData_0200229a = 0;
  gData_0200239c = 0;
  gData_0200239e = -1;
}

void Func_02000d78(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200229c, 64);
  Func_02001a4c(gField_CachedTilesB, buffer);
  gData_02002298 = Resource_FindFreeEntry();
  VramLoad(gData_02002298, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000b3c, 1152);
  gData_0200229a = 0;
  gData_0200239c = 0;
  gData_0200239e = -1;
}

void Func_02000df8(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_0200229c, 64);
  Func_02001a4c(gField_CachedTilesC, buffer);
  gData_02002298 = Resource_FindFreeEntry();
  VramLoad(gData_02002298, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000b3c, 1152);
  gData_0200229a = 0;
  gData_0200239c = 1;
  gData_0200239e = -1;
}
