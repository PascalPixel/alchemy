#include "types.h"
#include "iwram_call.h"
#include "task.h"
#include "heap_release.h"

void Func_0813f8d4(void);
void BattleEffect_TransferHalvedCanvas(void);
void BattleEffect_DimBackgroundStep(void);

// The clear goes through an inline, as the Broken Seal's iwram_call.h writes
// it: called straight through the fixed entry, the build sets the size before
// it loads the routine's address.
#undef Iwram_ClearWords
static inline s32 Iwram_ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the clear through this inline; as a direct call to
   * Iwram_ClearWordsEntry, 2 of 18 instructions of
   * BattleEffect_EndElementStreaks differ in tla-en. */
  return Iwram_ClearWordsEntry(destination, size);
}

/// Ends the light-streak backdrop: removes its three callbacks, clears the
/// canvas in VRAM and frees heap blocks 96 and 92.
void BattleEffect_EndElementStreaks(void) {
  Task_Remove(Func_0813f8d4);
  Task_Remove(BattleEffect_TransferHalvedCanvas);
  Task_Remove(BattleEffect_DimBackgroundStep);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
}
