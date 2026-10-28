#include "types.h"
#include "static_assert.h"
#include "ram_buffer.h"
#include "battle_party.h"
#include "b5_context.h"
#include "save.h"

u32 Save_FindFreeFile(void) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSummary *p;
  u32 i;

  /* FAKEMATCH: the native issues the workspace load before the summary offset.
   */
  __asm__("");
  p = work->summary;
  for (i = 0; i < 3; i++, p++) {
    if (p->level == 0)
      return i;
  }
  return 999;
}

s32 Func_0801596c(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

s32 Save_CountFilesExcludingSent(s32 flag) {
  s32 i;
  s32 cnt;
  struct SaveSummary *summary;
  struct SaveWorkspace *work;

  if (Func_0801596c() != 0) {
    cnt = -9;
  } else {
    cnt = Save_LoadFileSummaries();
    if (flag != 0) {
      work = Ram_HeapSlots->serialWork;
      /* FAKEMATCH: Without it the summary offset starts before the workspace
       * loads (differ 2). */
      __asm__("" : : "r"(work));
      summary = work->summary;
      for (i = 0; i < 3; i++) {
        if (summary[i].sendFlag != 0)
          cnt--;
      }
    }
  }
  Save_ReleaseWork();
  return cnt;
}
