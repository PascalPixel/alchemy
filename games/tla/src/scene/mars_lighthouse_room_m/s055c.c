#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "event_inline.h"

extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;


/// Scheduled by Func_02000934: steps room 1's blend (Data_02003a7c) one toward
/// its target (gData_02003a7e) each frame and queues the BLDALPHA write; as it
/// reaches 0 or 16, sets the cells the step (gData_02003a80) selects. The cell
/// calls go through Call6, as in Func_020006c8 (s06c8.c).
void Func_0200055c(void) {
  s32 step;

  if (Data_02003a7c == gData_02003a7e)
    return;
  step = gData_02003a7e - Data_02003a7c;
  if (step < 0)
    step = -1;
  else
    step = 1;
  Data_02003a7c += step;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      BLEND_ALPHA(Data_02003a7c, 16 - Data_02003a7c), 0x20000);
  if (Data_02003a7c == 0) {
    if (gData_02003a80 == 0)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gData_02003a80 == 1)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
    if (gData_02003a80 == 2)
      MapGrid_CopyCellAttributes(34, 8, 3, 3, 17, 8);
    if (gData_02003a80 == 3)
      MapGrid_CopyCellAttributes(30, 8, 3, 3, 8, 8);
  }
  if (Data_02003a7c == 16) {
    if (gData_02003a80 == 0)
      MapGrid_CopyCellAttributes(34, 0, 3, 3, 17, 8);
    if (gData_02003a80 == 1)
      MapGrid_CopyCellAttributes(30, 4, 3, 3, 8, 8);
    if (gData_02003a80 == 2)
      MapGrid_CopyCellAttributes(34, 4, 3, 3, 17, 8);
    if (gData_02003a80 == 3)
      MapGrid_CopyCellAttributes(30, 0, 3, 3, 8, 8);
  }
}
