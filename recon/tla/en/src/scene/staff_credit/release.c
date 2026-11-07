#include "types.h"
#include "task.h"
#include "resource.h"

extern const u8 gWorldMapPackedTilesC[];
extern u8 gData_020040d8[];
extern s16 gData_02003fc4;
extern s16 gData_02003fc6;
extern s16 gData_02003fc8;
void Func_02001f54(const u8 *a, u8 *b);
void Task_RemoveFar(void (*func)(void));
void Func_02001d90(void);

void Func_02001e58(void) {
  Func_02001f54(gWorldMapPackedTilesC, gData_020040d8);
  gData_02003fc6 = Resource_FindFreeEntry();
  VramCache_Load(gData_02003fc6, 0x100, 0);
  gData_02003fc4 = -1;
  gData_02003fc8 = 10;
  Task_Add(Func_02001d90, TASK_PASS_VBLANK - 10);
}

void Func_02001eb8(void) {
  Func_02001f54(gWorldMapPackedTilesC, gData_020040d8);
  Resource_ResetEntry(gData_02003fc6);
  gData_02003fc4 = -1;
  Task_RemoveFar(Func_02001d90);
}
