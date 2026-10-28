#include "local.h"

/// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
/// block and no source, then schedules Func_020015f8.
void Func_020017bc(void) {
  /* FAKEMATCH: through a local pointer the resident clear's entry loads before
   * the size is finished, as in the native (direct: differ 2). */
  s32 (*clear)(void *destination, s32 size) = Iwram_ClearWords;
  s32 priority;

  clear(gData_02002aa0, 0x1004);
  gData_02002aa0[0] = Resource_FindFreeEntry();
  /* FAKEMATCH: the empty asm keeps the entry's store ahead of the size, as in
   * the native (plain: differ 2; a block pointer, a struct with the entry
   * first, a size temporary or the VramBlock_LoadCached_ inline: no closer). */
  asm("");
  VramCache_Load(gData_02002aa0[0], 0x200, 0);
  /* FAKEMATCH: held in a local, the priority is built before the callback's
   * address loads, as in the native and events_f.c (as a literal: differ 2). */
  priority = 0x480;
  Task_Add(Func_020015f8, priority);
}
