#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02006a6c;
extern s16 gData_02006a6e;
extern struct FieldActor *gData_02006a70[];
extern s16 gData_02006b70;
extern s16 gData_02006b72;

void *MemAlloc(s32 size);
void Func_02005704(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_020047d4(void);

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

void Func_02004990(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02006a70, 64);
  Func_02005704(gField_CachedTilesA, buffer);
  gData_02006a6c = Resource_FindFreeEntry();
  VramLoad(gData_02006a6c, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_020047d4, 1152);
  gData_02006a6e = 0;
  gData_02006b70 = 0;
  gData_02006b72 = -1;
}

void Func_02004a10(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02006a70, 64);
  Func_02005704(gField_CachedTilesB, buffer);
  gData_02006a6c = Resource_FindFreeEntry();
  VramLoad(gData_02006a6c, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_020047d4, 1152);
  gData_02006a6e = 0;
  gData_02006b70 = 0;
  gData_02006b72 = -1;
}

void Func_02004a90(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02006a70, 64);
  Func_02005704(gField_CachedTilesC, buffer);
  gData_02006a6c = Resource_FindFreeEntry();
  VramLoad(gData_02006a6c, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_020047d4, 1152);
  gData_02006a6e = 0;
  gData_02006b70 = 1;
  gData_02006b72 = -1;
}
