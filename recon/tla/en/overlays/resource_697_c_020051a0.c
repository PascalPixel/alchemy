// Measured under -Os, 5 November 2026: differ 555 (tla-en); notes below may predate it.
// Draft, differ 475 of 497, skeleton gap 153 (tla-en, 2026-11-01)
// Left: the two loops' entries, then allocation; 20 plain reshapes gained nothing
// more after the marker masks (x &= 511; y &= 255 before the word, which took
// differ 495 to 475). No device is left in the draft to remove.
// Loops: the native falls into both loops and keeps every invariant inside them.
// The gcse dump shows why this source cannot: PRE copies work + 218 to 222 onto
// the row loop's entry edge and x << 16 and tile + 8 onto the pair loop's, from
// any loop entered on an edge, whether a for or a goto. Only entering at the test
// stops it, because the expression is then not anticipated there, so both loops
// here are gotos entered at their tests, and the two `b` lines are the price.
// Falling into goto loops (if (y < 160) around them, bottom tests as the native)
// gives the native's frame of 40 but gap 184 and differ 516. Real for loops also
// let loop.c hoist 0 and 0xe400: gap 228, differ 558. Those two constants stay
// in the loop only because a goto loop has no notes. Nothing in reach of the C
// keeps PRE out of a fall-in loop; PRE is skipped only under optimize_size or
// setjmp, so what remains there is a compiler question (K1 to K3), not a source
// one. It is the same gap as the landing-burst family.
// pair: the native's copy sits after the y test, so it looks like a loop.c
// reduced giv, but neither cursor[n] nor u32 *pair = cursor in a for loop gets
// reduced: cse folds the copy, and the combined address giv is "not worth
// while" (-204 vs 34). A hand pair is what works here.
// Allocation, by global.c priority (floor_log2(refs) * refs / live):
// - The native has work r11, x r9, y r7, pair r4, zero r8, second r10, shifted
//   r6 and attributes r5, with i, flip, row, tile, entry and cursor on the stack.
//   Here pair (26 refs over 40, 2.6) takes r7 ahead of y (29 over 90, 1.29), and
//   no shape moved that: y would need pair's live length to double. Then x is
//   r10, i (0.40) is r11 and work is on the stack, which is most of the remaining
//   mov/ldr skeleton lines. work has 18 to 22 refs over about 300.
// - The .18.greg dispositions look like reload's retries, not global.c's first
//   choice: locals skip r6 and r10 there, which y and x then hold. Read them so.
// - base stays apart from work: dropping it (cursor, entry and work as copies of
//   one load, which is the native's first block, and which explains why the code
//   before the row loop reads [sp, 36]) gives gap 154, differ 512 until the
//   allocation above is fixed. Reading the row head through work: differ 486.
// Tried without gain: declaration order (spill slots do not follow it here),
// scopes of i, x, y and fade, shared marker pointers, reading the marker fields
// without pointer locals (gap 184), statement order of flip, i and y, an inline
// second (gap 169), y |= the word (differ 497, and a reused local), x and y masks
// in one statement. 576 is rebuilt for VramCache_Load in the native and shared
// here; s4188.c's matched Pool_Alloc wrapper is the known cure, and it is a
// tagged device, so it stays out of this round.
// Calls in both marker arms are right: cross-jumping makes the native's shared
// `mov r9, r2; bl`, with the argument set in each arm.
//
// The Trial Road link display's scheduled callback (Func_020055c4 starts it):
// its fade state falls during events or with flag 262 and otherwise rises to
// 2 (unke0 holds it at 2); rising from 0 to 1 it loads the palette and picture
// again, and at 0 it frees its VRAM. Otherwise it queues two rows of sprites,
// each up to four pairs 36 pixels apart down from the scrolled offset, sliding
// in 6 pixels per fade step (the right row mirrored), and on frames 5 to 15 of
// every 16 a marker over each of the two watched objects, projected at 1/16
// scale from their positions and the course side.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "frame.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "vram_tab.h"

struct TrialRoadLinkWork {
  u32 oam[54];
  s16 vramEntry;
  s16 fade;
  s16 scrollX;
  s16 scrollY;
  s16 unke0;
  s16 a;
  s16 b;
  u8 unke6[6];
  s32 originX;
  s32 originZ;
};

/// One queued sprite: the three words Func_02005790 adds to OAM.
struct TrialRoadSprite {
  u32 words[3];
};

extern const u16 Data_020062b4[];
extern const u8 gData_020062d4[];

void Func_02005770(const void *source, void *destination);
void Func_02005790(void *entry, s32 priority);
s32 Func_02002ce8(s32 side);

void Func_020051a0(void) {
  struct TrialRoadLinkWork *base = Ram_HeapSlots->unkec[0];
  u32 *cursor = base->oam;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)base->oam;
  struct TrialRoadLinkWork *work = base;
  s32 tile;
  u8 *event;
  struct Actor *object;
  u32 flip;
  s32 x;
  s32 y;
  s32 lift;
  u32 row;
  u32 i;
  void *buffer;
  s16 *scrollY;

  {
    s16 *vramEntry = &work->vramEntry;
    s16 *fade;
    tile = gResourceTableEntries[*vramEntry].offset >> 5;
    event = Ram_HeapSlots->eventWork;
    fade = &work->fade;
    if (work->unke0 != 0) {
      work->fade = 2;
    } else if (GameFlag_Test(262) || *(s16 *)(event + 358) != 0) {
      if (*fade > 0)
        --*fade;
    } else if (*fade <= 1) {
      if (++*fade == 1) {
        Dma_Set(Data_020062b4, (void *)0x050003c0, 0x80000010, REG_DMA3);
        buffer = Heap_AllocEwram(576);
        Func_02005770(gData_020062d4, buffer);
        VramCache_Load(*vramEntry, 576, buffer);
        Heap_Release(buffer);
      }
    }
  }
  if (((struct TrialRoadLinkWork *)cursor)->fade == 0) {
    VramCache_ReleaseVram(((struct TrialRoadLinkWork *)cursor)->vramEntry);
    return;
  }
  row = 0;
  goto rowTest;
nextRow: {
    s16 *scrollX;
    s16 *fade;
    scrollX = &base->scrollX;
    scrollY = &work->scrollY;
    fade = &base->fade;
    y = (*scrollY + 8) & 255;
    if (row == 0) {
      flip = 0;
      x = *fade * 6 + *scrollX - 12;
      x &= 511;
    } else {
      x = 236 - *fade * 6 - *scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    i = 0;
    {
      u32 *pair = cursor;
      goto pairTest;
    nextPair: {
        struct TrialRoadSprite *second;
        pair[0] = 0;
        pair[1] = y | (x << 16) | flip | 0x80008000;
        pair[2] = tile | 0xe400;
        second = entry + 1;
        Func_02005790(entry, 255);
        pair[3] = 0;
        pair[4] = (y + 32) | (x << 16) | flip | 0x4000;
        pair[5] = (tile + 8) | 0xe400;
        pair += 6;
        cursor += 6;
        entry += 2;
        Func_02005790(second, 255);
      }
      y += 36;
      i++;
      if (i >= 4)
        goto pairsDone;
    pairTest:
      if (y < 160)
        goto nextPair;
    pairsDone:;
    }
  }
  row++;
rowTest:
  if (row < 2)
    goto nextRow;
  if ((gFrameCount & 15) < 5)
    return;
  object = ObjectTable_Get(work->a);
  if (object != NULL) {
    u32 side = gMainRoutineWork.entrance & 1;
    u32 *next = cursor + 1;
    struct TrialRoadSprite *nextEntry = entry + 1;
    s32 sprite = tile + 10;
    s16 *fade = &work->fade;
    s16 *scrollX = &work->scrollX;
    s32 *originX = &work->originX;
    s32 *originZ = &work->originZ;
    s32 objectX = object->x;
    if (side) {
      lift = Func_02002ce8(0);
      x = (objectX - *originX) / PIXELS(16) + *fade * 6 + *scrollX - 19;
    } else {
      x = (objectX - *originX) / PIXELS(16) - *fade * 6 - *scrollX + 232;
      lift = Func_02002ce8(0);
    }
    y = (object->z - *originZ) / PIXELS(16) + *scrollY - lift * 36 + 4;
    x &= 511;
    y &= 255;
    *cursor = 0;
    cursor = next;
    *cursor++ = y | (x << 16) | 0x40000000;
    *cursor++ = sprite | 0xe400;
    entry = nextEntry;
    Func_02005790(entry, 255);
  }
  object = ObjectTable_Get(work->b);
  if (object == NULL || object->x == 0)
    return;
  // XXX: this side's offset adds scrollX where the first marker's subtracts
  // it; the native does so, and a render of the course's far side would show
  // whether it is a slip.
  {
    u32 side = gMainRoutineWork.entrance & 1;
    u32 *next = cursor + 1;
    s32 sprite = tile + 14;
    s16 *scrollYMarker = &work->scrollY;
    s16 *fade = &work->fade;
    s16 *scrollX = &work->scrollX;
    s32 *originX = &work->originX;
    s32 *originZ = &work->originZ;
    s32 objectX = object->x;
    if (side) {
      lift = Func_02002ce8(1);
      x = (objectX - *originX) / PIXELS(16) + *fade * 6 + *scrollX + 189;
    } else {
      x = (objectX - *originX) / PIXELS(16) - *fade * 6 + *scrollX + 23;
      lift = Func_02002ce8(1);
    }
    y = (object->z - *originZ) / PIXELS(16) + *scrollYMarker - lift * 36 + 4;
    x &= 511;
    y &= 255;
    *cursor = 0;
    cursor = next;
    *cursor++ = y | (x << 16) | 0x40000000;
    *cursor = sprite | 0xe400;
    Func_02005790(entry, 255);
  }
}
