#include "types.h"
#include "layout_guard.h"
#include "ram_buffer.h"
#include "battle_party.h"
#include "b5_context.h"
#include "save_state.h"

u32 SaveState_FindFreeSummarySlot(void) {
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
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);

s32 SaveState_CountRecordsExcludingFlagged(s32 flag) {
  s32 i;
  s32 cnt;
  struct SaveSummary *summary;
  struct SaveWorkspace *work;

  if (Func_0801596c() != 0) {
    cnt = -9;
  } else {
    cnt = SaveState_LoadSummaryRecords();
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
  SaveState_ReleaseWorkspace();
  return cnt;
}
