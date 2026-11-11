#include "local.h"

/// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
/// block and no source, then schedules Func_020015f8.
void Func_020017bc(void) {
  Iwram_ClearWords(gData_02002aa0, 0x1004);
  gData_02002aa0[0] = Resource_FindFreeEntry();
  Vram_Load(gData_02002aa0[0], 0x200, 0);
  FieldTask_AddFrame(Func_020015f8, 0x480);
}
