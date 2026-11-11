#include "local.h"

/// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
/// block and no source, then schedules Func_020015f8.
void Func_020017bc(void) {
  /* FAKEMATCH(temporary): the resident clear called through a local pointer, so
   * its entry loads before the size is finished, as in the ROM; as a direct
   * call, 2 of 21 instructions differ in tla-en. */
  s32 (*clear)(void *destination, s32 size) = Iwram_ClearWords;

  clear(gData_02002aa0, 0x1004);
  gData_02002aa0[0] = Resource_FindFreeEntry();
  Vram_Load(gData_02002aa0[0], 0x200, 0);
  FieldTask_AddFrame(Func_020015f8, 0x480);
}
