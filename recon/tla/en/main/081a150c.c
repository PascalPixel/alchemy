// Differ 12 of 69 (tla-en, 2026-11-08, -mtune=gba). The listing's pool gives the
// native's address order: 7518, 751c, 7520, 7528, gScrollTarget, 7524, 7514.
// Written in that order: 32 (sched1 then hoists 7524's address and
// Func_081a133c into r1 and r0); 7524 as s16 32, u8 or s32 35; 7524 = 1
// before 7520 = 0 with gScrollTarget after 7528: 12.
// Draft, differ 12 of 69 (was 32): sets up a scrolling picture scene:
// clears the frame flags and the scroll words, resets the task table and adds
// Func_081a133c, then sets up BG2, BG3 and blending as Func_081a06e0 does,
// with BLDALPHA 16, and calls Func_081a1830. gData_02007520 is cleared after
// Data_02007524 is set, which puts the native's loads of the scroll words'
// addresses one store ahead. Left: instruction order in that run: the native
// loads Data_02007520's address into r1 before Data_02007528's store, the
// rest one step earlier; the pool follows the load order. A barrier after
// Data_02007514's store gives 14; no second barrier helps.
#include "types.h"
#include "io_reg.h"
#include "io_write.h"
#include "frame.h"
#include "display_scroll.h"
#include "task.h"

extern u8 gData_03001110;
extern u32 Data_02007518;
extern u32 gData_0200751c;
extern u16 gData_02007520;
extern u32 Data_02007528;
extern u16 Data_02007524;
extern u16 Data_02007514;

void Func_081a133c(void);
void DisplayScroll_BuildHblankWordTable(u32 *table);
void Graphics_ClearCharacterBlockAndPalette(s32 block);
void Func_081a1830(void);

void Func_081a150c(void) {
  gCpuLoadDisplayEnabled = 0;
  gData_03001110 = 0;
  gAutoSleepEnabled = 0;
  Task_ResetAll();
  Data_02007518 = 0;
  gData_0200751c = 0;
  Data_02007528 = 0;
  gScrollTarget = 0;
  Data_02007524 = 1;
  gData_02007520 = 0;
  Data_02007514 = 0;
  Task_Add((s32)Func_081a133c, TASK_PASS_VBLANK);
  Io_Write16(0x40, &REG_DISPCNT);
  DisplayScroll_BuildHblankWordTable((u32 *)0x06007800);
  DisplayScroll_BuildHblankWordTable((u32 *)0x0600f800);
  Graphics_ClearCharacterBlockAndPalette(0);
  Graphics_ClearCharacterBlockAndPalette(1);
  Io_Write16(0x1f8a, &REG_BG2CNT);
  Io_Write16(0x0f83, &REG_BG3CNT);
  Io_Write16(0x1c40, &REG_DISPCNT);
  Io_Write16(0x2844, &REG_BLDCNT);
  Io_Write16(16, &REG_BLDALPHA);
  Func_081a1830();
}
