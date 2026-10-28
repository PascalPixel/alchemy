#include "types.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "task.h"

struct BucketEntry {
  void *next;
  u32 attributes;
  s32 tile;
};

extern struct BucketEntry *gData_0200752c;

void Func_081a1810(void);

/// Allocates the 24 sprite entries of the scrolling picture's border strips,
/// fills their tiles with colour 1 and adds Func_081a1810 to draw them; the
/// entries are written as words, as The Broken Seal's Credits_InitTextSprites
/// writes its strips.
void Func_081a1830(void) {
  s32 slot = Resource_FindFreeEntry();
  struct BucketEntry *entries;
  u32 *words;
  s32 tile;
  u32 i;

  /* FAKEMATCH(temporary): the allocation kept in entries; as
   * gData_0200752c = Heap_AllocEwram(0x120), 1 of 82
   * instructions differ in tla-en. */
  entries = Heap_AllocEwram(0x120);
  gData_0200752c = entries;
  tile = VramCache_Load(slot, 0x200, 0);
  Dma_Fill32(0x11111111, (void *)(0x06010000 + tile * 32), 0x200);
  words = (u32 *)gData_0200752c;
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | 0x80004078;
    *words++ = tile;
  }
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | 0x80004088;
    *words++ = tile | 0x800;
  }
  for (i = 0; i < 8; i++) {
    *words++ = 0;
    *words++ = (i << 21) | 0x40004098;
    *words++ = tile | 0x800;
  }
  Task_Add(Func_081a1810, TASK_PASS_VBLANK);
}
