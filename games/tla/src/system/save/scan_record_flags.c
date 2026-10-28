#include "types.h"
#include "ram_buffer.h"
#include "save.h"

extern u16 gData_02003860;
extern u16 gData_020036d8;
extern u16 gData_02005350;

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

s32 Func_080432a4(void) {
  s32 err;
  s32 cnt;
  s32 ret;

  err = Func_0801596c();
  cnt = 0;
  ret = -9;
  if (err == 0) {
    s32 i;
    struct SaveWorkspace *work;

    ret = Save_LoadFileSummaries();
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
      if (work->summary[i].level != 0 && work->summary[i].hasFlag34 == 0)
        gData_02005350 = 1;
    }
  }
  Save_ReleaseWork();
  if (ret != 0 && cnt == ret)
    return ret + 100;
  return ret;
}
