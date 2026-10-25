// Near miss, differ 4 of 78 (85 before, 2026-10-25): the flags read as
// fields of each summary, so that loop.c reduces them to one cursor and
// leaves the level read as work + i * 64 + 0x3058, as the native does. Left:
// the native interleaves the cursor's offset (movs; lsls; adds) with the
// copy of the first flag's address and the load of the 1; this builds it
// in one run. The Broken Seal twin has no level test.
#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"

extern u16 gData_02003860;
extern u16 gData_020036d8;
extern u16 gData_02005350;

s32 Func_0801596c(void);
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);

s32 SaveState_ScanRecordFlags(void) {
  s32 err;
  s32 cnt;
  s32 ret;

  err = Func_0801596c();
  cnt = 0;
  ret = -9;
  if (err == 0) {
    s32 i;
    struct SaveWorkspace *work;

    ret = SaveState_LoadSummaryRecords();
    work = Ram_HeapSlots->serialWork;
    gData_02003860 = 0;
    gData_020036d8 = 0;
    gData_02005350 = 0;
    for (i = 0; i < 3; i++) {
      if (work->summary[i].sendFlag != 0) {
        gData_02003860 = 1;
        cnt++;
      }
      if (work->summary[i].flagCount != 0)
        gData_020036d8 = 1;
      if (work->summary[i].level != 0 &&
          work->summary[i].hasFlag34 == 0)
        gData_02005350 = 1;
    }
  }
  SaveState_ReleaseWorkspace();
  if (ret != 0 && cnt == ret)
    return ret + 100;
  return ret;
}
