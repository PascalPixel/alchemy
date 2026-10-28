#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "mem.h"
#include "ram_buffer.h"
#include "task.h"
#include "palbuf.h"
#include "fxblend.h"
#include "dma.h"
#include "io_reg.h"
#include "map.h"
#include "resource.h"
#include "edition.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

extern const u8 gData_080f385e[];

union MoteWord {
  u32 value;
  void *link;
};

/// One floating mote: its sprite's attribute words and its world position.
struct Mote {
  union MoteWord link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

/// Heap slot 0x74 while the motes run.
struct MoteWork {
  s32 vramEntry;
  s32 tileBase;
  struct Mote motes[64];
  u8 pad808[12];
};

void Func_080d6c60(void);

/// Starts the 64 floating motes (Func_080d6c60): loads their tiles and puts
/// each under the leader with staggered timers, drawn semi-transparent.
// As ☀️'s function of the same name.
void FieldMotes_Start(void) {
  struct MoteWork *work;
  struct Mote *mote;
  u8 *tiles;
  u32 i;
  u32 size;
  volatile u32 zero;
  volatile u16 *reg;
  struct HeapSlots *slots;
  /* FAKEMATCH: the register variable keeps the cleared word's zero inside
   * the loop, set on every pass, and in r6 for the last blend register
   * after it (differ 4 without it: loop.c hoists the zero). */
  register s32 clear asm("r6");

  size = sizeof(struct MoteWork);
  work = Runtime_AllocateBlock(116, size);
  mote = work->motes;
  /* FAKEMATCH: reusing the allocation size for the cleared word keeps it in
   * r1 (differ 8 without it). */
  size = 0;
  zero = size;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x205, REG_DMA3);
  tiles = Runtime_AllocateBlock(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++) {
    struct MapWork *view = slots->mapWork;
    s32 *pos;
    /* FAKEMATCH: reusing the integer field cursor for the height's x argument
     * keeps the stores in r1 (a word pointer with x of its own differs 18). */
    u32 p;
    s32 z;

    /* FAKEMATCH: keeps the map work load before the cursor copy (differ 4
     * without it). */
    __asm__("" : : "r"(view));
    p = (u32)mote;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    /* FAKEMATCH: keeps the first store before the origin load (differ 8
     * without it). */
    __asm__("" : : "r"(view));
    pos = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = Map_GetTerrainHeightFar(0, (s32)p, z);
    mote->timer = (i & 15) + 1;
    mote++;
  }
  /* FAKEMATCH: the one-pass block and the stepped register pointer write the
   * three blend registers as the native does (plain writes differ 33). */
  do {
    s32 v;
    v = 0x3f00;
    reg = &REG_BLDCNT;
    *reg = v;
    v = 0x1008;
    reg++;
    *reg = v;
    reg++;
    *reg = clear;
  } while (0);
  Task_Add(Func_080d6c60, TASK_PASS_VBLANK);
}
