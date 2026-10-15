#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002bd8;
extern s16 gData_02002bda;
extern struct FieldActor *gData_02002bdc[];
extern s16 gData_02002cdc;
extern s16 gData_02002cde;

void *MemAlloc(s32 size);
void Func_02002174(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_0200125c(void);

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

void Func_02001418(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002bdc, 64);
  Func_02002174(gField_CachedTilesA, buffer);
  gData_02002bd8 = Resource_FindFreeEntry();
  VramLoad(gData_02002bd8, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_0200125c, 1152);
  gData_02002bda = 0;
  gData_02002cdc = 0;
  gData_02002cde = -1;
}

void Func_02001498(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002bdc, 64);
  Func_02002174(gField_CachedTilesB, buffer);
  gData_02002bd8 = Resource_FindFreeEntry();
  VramLoad(gData_02002bd8, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_0200125c, 1152);
  gData_02002bda = 0;
  gData_02002cdc = 0;
  gData_02002cde = -1;
}

void Func_02001518(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002bdc, 64);
  Func_02002174(gField_CachedTilesC, buffer);
  gData_02002bd8 = Resource_FindFreeEntry();
  VramLoad(gData_02002bd8, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_0200125c, 1152);
  gData_02002bda = 0;
  gData_02002cdc = 1;
  gData_02002cde = -1;
}
