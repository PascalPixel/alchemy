#include "types.h"
#include "callback_scheduler.h"
#include "resource.h"

extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_020040d8[];
extern s16 gData_02003fc4;
extern s16 gData_02003fc6;
extern s16 gData_02003fc8;
void Func_02001f54(const u8 *a, u8 *b);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02001d90(void);

void Func_02001e58(void) {
  Func_02001f54(gWorldMapPackedTilesC, gData_020040d8);
  gData_02003fc6 = Resource_FindFreeEntry();
  VramBlock_LoadCached(gData_02003fc6, 0x100, 0);
  gData_02003fc4 = -1;
  gData_02003fc8 = 10;
  Scheduler_AddOrUpdateCallback(Func_02001d90, 0x476);
}

void Func_02001eb8(void) {
  Func_02001f54(gWorldMapPackedTilesC, gData_020040d8);
  Resource_ResetEntry(gData_02003fc6);
  gData_02003fc4 = -1;
  Scheduler_RemoveCallbackFar(Func_02001d90);
}
