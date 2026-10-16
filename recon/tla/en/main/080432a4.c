// Draft, differ 85 of 78: the native keeps the pass index times 64 in r4 (lsls
// r4, r5, #6 at the top of every pass), rebuilds the level offset 0x3058 on
// every pass and reads the level through the workspace base (ldrb r3, [r7,
// r3]); only the flag cursor is strength-reduced (r0, adds r0, #64). This
// draft, an explicit cursor and an explicit per-pass offset all reduce the
// level read to a second cursor and so keep the result out of r8 and the third
// flag address out of sl. The Broken Seal twin has no level test.
#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"

extern u16 gData_02003860;
extern u16 gData_020036d8;
extern u16 gData_02005350;

s32 Func_0801596c(void);
s32 SaveState_LoadSummaryRecords(void);
void SaveState_ReleaseWorkspace(void);

#define SUMMARY_TAIL(field)                                                    \
  ((u32)&((struct SaveSummary *)0)->field -                                    \
   (u32)&((struct SaveSummary *)0)->party[4])

s32 SaveState_ScanRecordFlags(void) {
  s32 err;
  s32 cnt;
  s32 ret;

  err = Func_0801596c();
  cnt = 0;
  ret = -9;
  if (err == 0) {
    s32 i;
    s8 *flags;
    struct SaveWorkspace *work;

    ret = SaveState_LoadSummaryRecords();
    work = Ram_HeapSlots->serialWork;
    flags = &work->summary[0].party[4];
    gData_02003860 = 0;
    gData_020036d8 = 0;
    gData_02005350 = 0;
    for (i = 0; i < 3; i++) {
      if (flags[i * sizeof(struct SaveSummary) + SUMMARY_TAIL(sendFlag)] != 0) {
        gData_02003860 = 1;
        cnt++;
      }
      if (flags[i * sizeof(struct SaveSummary) + SUMMARY_TAIL(flagCount)] != 0)
        gData_020036d8 = 1;
      if (work->summary[i].level != 0 &&
          flags[i * sizeof(struct SaveSummary) + SUMMARY_TAIL(hasFlag34)] == 0)
        gData_02005350 = 1;
    }
  }
  SaveState_ReleaseWorkspace();
  if (ret != 0 && cnt == ret)
    return ret + 100;
  return ret;
}
