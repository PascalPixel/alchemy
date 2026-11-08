// Matches: differ 0 (tla-en, 2026-11-08)
// Sets up a scrolling picture scene: clears the frame flags and the scroll
// words, resets the task table and adds Func_081a133c, then sets up BG2, BG3
// and blending as Func_081a06e0 does, with BLDALPHA 16, and calls
// Func_081a1830. The key was one chained assignment, gScrollTarget =
// gData_02007520 = Data_02007528: the native loads those three addresses
// nested (the map pointer's innermost, in r3, then r1, then r0), which no order
// of seven separate stores gives (12 at best). Types follow the credits files
// (f1a133c.c, f1a15dc.c): Data_02007528 is the map pointer, the halfwords s16;
// with Data_02007528 as u32 and no cast it matches too.
#include "types.h"
#include "io_reg.h"
#include "io_write.h"
#include "frame.h"
#include "display_scroll.h"
#include "task.h"

extern u8 gData_03001110;
extern u32 Data_02007518;
extern s32 gData_0200751c;
extern s16 gData_02007520;
extern u8 *Data_02007528;
extern s16 Data_02007524;
extern s16 Data_02007514;

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
  // One chained assignment: the three addresses are loaded nested, the map
  // pointer's innermost.
  gScrollTarget = gData_02007520 = (s32)(Data_02007528 = NULL);
  Data_02007524 = 1;
  Data_02007514 = 0;
  Task_Add(Func_081a133c, TASK_PASS_VBLANK);
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
