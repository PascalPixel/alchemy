#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_020035fc;
extern s16 gData_020035fe;
extern struct FieldActor *gData_02003600[];
extern s16 gData_02003700;
extern s16 gData_02003702;

void *MemAlloc(s32 size);
void Func_02002a8c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02001b74(void);

static inline void *BumpAlloc(s32 size) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  return MemAlloc(size);
}

static inline void VramLoad(s32 entry, s32 size, void *source) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each size's
   * constant */
  VramCache_Load(entry, size, source);
}

void Func_02001d30(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003600, 64);
  Func_02002a8c(gField_CachedTilesA, buffer);
  gData_020035fc = Resource_FindFreeEntry();
  VramLoad(gData_020035fc, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02001b74, 1152);
  gData_020035fe = 0;
  gData_02003700 = 0;
  gData_02003702 = -1;
}

void Func_02001db0(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003600, 64);
  Func_02002a8c(gField_CachedTilesB, buffer);
  gData_020035fc = Resource_FindFreeEntry();
  VramLoad(gData_020035fc, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02001b74, 1152);
  gData_020035fe = 0;
  gData_02003700 = 0;
  gData_02003702 = -1;
}

void Func_02001e30(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02003600, 64);
  Func_02002a8c(gField_CachedTilesC, buffer);
  gData_020035fc = Resource_FindFreeEntry();
  VramLoad(gData_020035fc, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02001b74, 1152);
  gData_020035fe = 0;
  gData_02003700 = 1;
  gData_02003702 = -1;
}
