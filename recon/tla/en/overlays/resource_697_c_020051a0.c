// Differ 16 of 497 (tla-en, 2026-11-06), down from 334: x &= 511 in place and
// before y &= 255 (the native ties x & 511 to x); the pointer to originZ held
// in r6 and set just before the lift call (unpinned, tile + n outranks it in
// global.c, 0.129 against 0.111, takes r6 and leaves frame 32 where the native
// spills entry + 12 and the second &scrollY for frame 40); side read inside the
// marker block; entry++ inside the marker call; the markers' size bits held in
// one function-wide local (permute found it; as a literal, 95). Three tagged
// pins (originZ r6, x r9, arg r0). Left: the 576 size kept in r6 across
// Heap_AllocEwram where the native loads it twice (CSE shares the constant and
// local-alloc gives it a register; 6 instructions), and the branches past it.
// Before 6 November (the notes below, with frame and pins since settled):
// Tried 2026-11-06: the row loop built from a label and goto (no change: both
// compiles keep &scrollY in a slot from before the loop); 10 minutes of permute
// reached 327 only by moving the fade check below the rows, which changes what
// the code does.
// Near miss: allocation and stack slots
// (frame 32, native 40), and &scrollY hoisted above the row loop where the
// native recomputes it each row. Under -Os, with two untagged pins (x r9, arg
// r0). Down from 397 (5 November): the markers' offsets open with their sign,
// so x is q - fade * 6 ... subtracted as the native does rather than adding a
// negated fade; px is taken after y &= 255; entry steps after its call. Earlier
// the figure rose from 264 on purpose: the 102 instructions of duplicated
// marker tails are gone, so what is left is allocation and slots, not shape.
// The native's arms are one tail. Sched2 runs before jump2 in this compiler,
// and the tail starts at mov r9, r2 with movs r0, #n still inside each arm, so
// the source has two whole tails which jump2 cross-jumps. Here find_cross_jump
// refuses them: flow.c's find_basic_blocks puts a (use 0) after the call that
// ends the fall-through arm, and none after the call that the first arm's jump
// follows. An empty asm after the call is refused too (ASM_INPUT), an arm's
// dead store goes before flow2, and a label after the first arm's call is
// tensioned or merged away with its (use 0). So one tail is written here, and
// arg's pin keeps movs r0 in the arms, because gcse's cprop moves a plain arg =
// n to the join. Tie evidence: mov r0, r9; ands r0, r3 and mov r1, r8; orrs r1,
// r3 are in-out reloads of x & 511 and (tile + n) | 0xe400 tied by regmove to
// high registers. x ranks above object
// (.385 against .242) without the pin, and takes r5 where the native has
// object. Left: native order is object r5, ptr240 r6, tile+n r8, cursor+4 r10,
// x r9, entry+12 and the second &scrollY spilled (frame 40); here tile+n r6,
// ptr240 r7, cursor+4 r8, entry+12 r10 (frame 32). The do { } while (0) is what
// hoists those invariants above the side test. The Trial Road link display's
// scheduled callback (Func_020055c4 starts it): its fade state falls during
// events or with flag 262 and otherwise rises to 2 (unke0 holds it at 2);
// rising from 0 to 1 it loads the palette and picture again, and at 0 it frees
// its VRAM. Otherwise it queues two rows of sprites, each up to four pairs 36
// pixels apart down from the scrolled offset, sliding in 6 pixels per fade step
// (the right row mirrored), and on frames 5 to 15 of every 16 a marker over
// each of the two watched objects, projected at 1/16 scale from their positions
// and the course side.
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

// left and right are the arms' offsets, each opening with its sign, so the
// subtractions compile in the order the native has them.
#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    /* FAKEMATCH(pin): originZ in r6; as a plain s32 *, 316 of 497 */          \
    /* instructions differ in tla-en. */                                       \
    register s32 *originZ asm("r6");                                           \
    /* FAKEMATCH(pin): x in r9; as a plain s32, 377 of 497 instructions */     \
    /* differ in tla-en. */                                                    \
    register s32 x asm("r9");                                                  \
    /* FAKEMATCH(pin): arg in r0; as a plain s32, 60 of 497 instructions */    \
    /* differ in tla-en. */                                                    \
    register s32 arg asm("r0");                                                \
    s32 side = gMainRoutineWork.entrance & 1;                                  \
    if (side) {                                                                \
      x = (object->x - work->originX) / PIXELS(16) left;                       \
      arg = which;                                                             \
    } else {                                                                   \
      x = (object->x - work->originX) / PIXELS(16) right;                      \
      arg = which;                                                             \
    }                                                                          \
    originZ = &work->originZ;                                                  \
    lift = Func_02002ce8(arg);                                                 \
    y = (object->z - *originZ) / PIXELS(16) + work->scrollY - lift * 36 + 4;   \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= x << 16;                                                              \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) | 0xe400;                                   \
    Func_02005790(entry++, 255);                                               \
  } while (0)

void Func_020051a0(void) {
  struct TrialRoadLinkWork *work = Ram_HeapSlots->unkec[0];
  u32 *cursor = (u32 *)work;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work;
  s16 *vramEntry = &work->vramEntry;
  u32 row, i;
  s32 tile = gResourceTableEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 y;
  // One constant for its whole life: the markers' OAM size bits (attribute 1
  // size 1, 16 by 16), held in a register across both markers.
  u32 markerSize = 0x40000000;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;

  if (work->unke0 != 0) {
    work->fade = 2;
  } else if (GameFlag_Test(262) || *(s16 *)(event + 358) != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      void *buffer;
      Dma_Set(Data_020062b4, (void *)0x050003c0, 0x80000010, REG_DMA3);
      buffer = Heap_AllocEwram(576);
      Func_02005770(gData_020062d4, buffer);
      VramCache_Load(*vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (((struct TrialRoadLinkWork *)cursor)->fade == 0) {
    VramCache_ReleaseVram(((struct TrialRoadLinkWork *)cursor)->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s32 x;
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = work->fade * 6 + work->scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - work->fade * 6 - work->scrollX;
      x &= 511;
      flip = 0x10000000;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | (x << 16) | flip | 0x80008000;
      *cursor++ = tile | 0xe400;
      Func_02005790(entry++, 255);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | (x << 16) | flip | 0x4000;
      *cursor++ = (tile + 8) | 0xe400;
      Func_02005790(entry++, 255);
      y += 36;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  object = ObjectTable_Get(work->a);
  if (object != NULL) {
    MARKER(+work->fade * 6 + work->scrollX - 19,
           -work->fade * 6 - work->scrollX + 232, 0, 10);
  }
  object = ObjectTable_Get(work->b);
  if (object == NULL || object->x == 0)
    return;
  {
    // XXX: this side's offset adds scrollX where the first marker's subtracts
    // it; the native does so, and a render of the course's far side would show
    // whether it is a slip.
    MARKER(+work->fade * 6 + work->scrollX + 189,
           -work->fade * 6 + work->scrollX + 23, 1, 14);
  }
}
