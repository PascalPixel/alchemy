#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "task.h"
#include "palette_buffers.h"
#include "field_blend.h"
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
  /* FAKEMATCH(pin): clear held in r6, which keeps its zero set inside the
   * loop on every pass and ready for the last blend register; as a plain
   * local, 4 of 92 instructions differ in tla-en. */
  register s32 clear asm("r6");

  size = sizeof(struct MoteWork);
  work = HeapSlot_AllocEwram(116, size);
  mote = work->motes;
  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x205, REG_DMA3);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++) {
    struct MapWork *view = slots->mapWork;
    s32 *pos;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 18 of 92 instructions differ in tla-en. */
    u32 p;
    s32 z;

    p = (u32)mote;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
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
  // The blend writes in a once-only block keep the task routine's address
  // ahead of the call's other argument; as a plain block, 4 of 92 instructions
  // differ in tla-en.
  do {
    /* FAKEMATCH(temporary): the values through v and the registers through
     * one stepped pointer, which builds each value from a small constant and
     * steps the address by 2; as REG_BLDCNT, REG_BLDALPHA and REG_BLDY
     * written directly, 31 of 92 instructions differ in tla-en. */
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
