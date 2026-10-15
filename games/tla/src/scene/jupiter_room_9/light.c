#include "types.h"
#include "iwram_call.h"
#include "field_event.h"
#include "callback_scheduler.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_0200265c;
extern s16 gData_0200265e;
extern struct FieldActor *gData_02002660[];
extern s16 gData_02002760;
extern s16 gData_02002762;

void *MemAlloc(s32 size);
void Func_02001b28(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void MemFree(void *block);
void Func_02000c18(void);

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

void Func_02000dd4(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002660, 64);
  Func_02001b28(gField_CachedTilesA, buffer);
  gData_0200265c = Resource_FindFreeEntry();
  VramLoad(gData_0200265c, 0xc00, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000c18, 1152);
  gData_0200265e = 0;
  gData_02002760 = 0;
  gData_02002762 = -1;
}

void Func_02000e54(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002660, 64);
  Func_02001b28(gField_CachedTilesB, buffer);
  gData_0200265c = Resource_FindFreeEntry();
  VramLoad(gData_0200265c, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000c18, 1152);
  gData_0200265e = 0;
  gData_02002760 = 0;
  gData_02002762 = -1;
}

void Func_02000ed4(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002660, 64);
  Func_02001b28(gField_CachedTilesC, buffer);
  gData_0200265c = Resource_FindFreeEntry();
  VramLoad(gData_0200265c, 0x800, buffer);
  MemFree(buffer);
  Scheduler_AddOrUpdateCallback(Func_02000c18, 1152);
  gData_0200265e = 0;
  gData_02002760 = 1;
  gData_02002762 = -1;
}
