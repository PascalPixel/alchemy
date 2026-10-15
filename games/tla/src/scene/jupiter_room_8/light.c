#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002af0;
extern s16 gData_02002af2;
extern struct FieldActor *gData_02002af4[];
extern s16 gData_02002bf4;
extern s16 gData_02002bf6;

void *MemAlloc(s32 size);
void Func_02002118(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02001200(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  VramBlock_LoadCached(entry, size, source);
}

void Func_020013bc(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesA, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  VramLoad(gData_02002af0, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02001200, 1152);
  gData_02002af2 = 0;
  gData_02002bf4 = 0;
  gData_02002bf6 = -1;
}

void Func_0200143c(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesB, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  VramLoad(gData_02002af0, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02001200, 1152);
  gData_02002af2 = 0;
  gData_02002bf4 = 0;
  gData_02002bf6 = -1;
}

void Func_020014bc(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesC, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  VramLoad(gData_02002af0, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02001200, 1152);
  gData_02002af2 = 0;
  gData_02002bf4 = 1;
  gData_02002bf6 = -1;
}
