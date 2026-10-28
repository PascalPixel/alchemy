// Draft, differ 65 of 83 (was 67, a raw draft calling absolute addresses),
// rewritten from the listing: the Venus Lighthouse entrance's spark setup.
// It takes heap slot 220, unpacks resource 0x1e1 (directory.s's DustPuffA)
// into it, loads the tiles into a free VRAM entry, sets up the 96 sparks
// (each sprite set up, each frame -1) and schedules Func_0200258c. Every instruction
// but the loop's counter is the native's; the rest is registers: the native
// keeps the work in r6, the tile base in r10 and the counter in r8, while here
// the work takes r7, the tile base r8 and the counter r4, saved around the
// call (sub sp, #8). The state pointer as a local moved the -13 mask back
// into the loop as the native has it (was hoisted into r10). Measured: the
// sprite and state as pointers stepped by the loop (78), the counter pinned
// to r8 (64, the loop is no longer reversed), five minutes of psynergy
// permute (nothing valid under 65).
#include "types.h"
#include "ram_buffer.h"

struct SparkState {
  u8 unk00[24];
  s32 frame;
};

struct SparkSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite sprites[96];
  u16 unk2180;
  u16 tileBase;
  u16 resource;
};

extern char ResourceId_DustPuffA;

void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
void Func_02002824(void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_02002a0c(struct SparkSprite *sprite, s32 width, s32 height, s32 attr,
                   s32 tile);
s32 Task_Add(void (*callback)(void), s32 order);
void Func_0200258c(void);

void Func_020026f4(void) {
  struct SparkWork *work = HeapSlot_AllocFar(220, 0x2188);
  s32 resource;
  s32 tileBase;
  s32 i;

  Func_02002824(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  resource = Resource_FindFreeEntry();
  tileBase = VramCache_Load(resource, 0x800, work);
  work->tileBase = tileBase;
  work->resource = resource;
  for (i = 0; i < 96; i++) {
    struct SparkState *state = &work->states[i];
    struct SparkSprite *sprite = &work->sprites[i];

    Func_02002a0c(sprite, 8, 8, 0x40000000, tileBase);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    state->frame = -1;
  }
  work->unk2180 = 0;
  Task_Add(Func_0200258c, 0x480);
}
