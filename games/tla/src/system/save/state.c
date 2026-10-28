#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "ram_buffer.h"
#include "save.h"
#include "dma.h"
#include "flash.h"

s32 Save_CompareBytes(u8 *left, u8 *right, s32 count) {
  s32 difference = 0;

  while (count != 0) {
    difference = *left - *right;
    if (difference != 0)
      break;
    count--;
    left++;
    right++;
  }
  return difference;
}

u32 Save_GetLatestCounter(s32 partId) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  u32 latestCounter;
  u32 i;

  latestCounter = 0;
  for (i = 0; i <= 14; i += 3) {
    if (work->occupied[i] != 0 && partId == work->partId[i]) {
      if (latestCounter < work->counter[i])
        latestCounter = work->counter[i];
    }
  }
  return latestCounter;
}

/// Reads the summary of the latest save in each of the three files, clearing
/// it first; returns how many files have one.
s32 Save_LoadFileSummaries(void) {
  struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
  struct SaveSummary *summary;
  s32 count;
  u32 group;

  /* FAKEMATCH: the empty asm holds r2 until the workspace is loaded, as ⚓️
   * does before it starts the summary offset 0x303c; plainly, 2 instructions
   * differ in tla-en. */
  asm("" : "+l"(work) : : "r2");
  summary = work->summary;
  count = 0;
  for (group = 0; group <= 2; group++) {
    u32 slot;

    Dma_Fill32(0, summary, sizeof(*summary));
    slot = Save_FindLatestSector(group);
    if (slot <= 14) {
      ReadFlash(slot, 0, (u8 *)summary, sizeof(*summary));
      ReadFlash(slot + 1, 16, summary->unk38, sizeof(summary->unk38));
      count++;
    }
    summary++;
  }
  return count;
}
