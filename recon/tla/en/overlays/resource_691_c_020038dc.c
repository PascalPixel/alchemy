// Draft, differ 68 of 83, skeleton gap 9 (tla-en, 2026-11-01)
// The falling rocks' setup, overlay 691's copy of overlay 668's Func_02004004
// (also 68f, 692, 6a8, 6b9 with their own helpers): loads
// ResourceId_FallingRock's tiles into heap slot 220 and resets the 64 sprites
// and timers. No device is kept, as none reached differ 0, but every difference
// is now explained, and tagged devices take it to differ 6 with skeleton gap 0.
// Each step below was measured; apply them all in this order to return there.
// 1. Loop: a real do loop, not goto, as below. Only a real loop keeps the
// listing's -13 copy (adds r2, r1, #0), the QImode pseudo of the priority mask
// read through a paradoxical subreg, and its LOOP_END note stops CSE carrying
// the mask zero of unk1e past the loop to next (goto: differ 80).
// 2. Hoist: loop.c's second scan lifts -13 (life 2; threshold 15 * 1 * 2 >= 25
// real insns), and the native leaves it in. It needs at least 31 real insns at
// that scan, then flow must delete them: a dead `s32 pad = 0;` with `pad = (pad
// >> 1) ^ (pad << 2) ^ (pad >> 3) ^ (pad << 4);` at the end of the body does it
// (dead). Repeated stores add too few and hoist their constants.
// 3. Count: i takes r4 with a stack save, as 4 * 1 call < 7 weighted refs
// passes CALLER_SAVE_PROFITABLE; the native has it in r8 and the tile in r10.
// An `asm volatile("" : : : "r4");` as the first statement of the do body
// settles it (barrier); in the preheader the asm moves the r2 copy and the tile
// copy ahead of the split constants, as its dependencies outrank them. An asm
// use of i after Task_Add gives two crossed calls but 9 refs, so r4 stays.
// 4. Constant: 0x500 reused across the call for rocks; the static inline
// `VramLoad(entry, 0x500, work)` of overlay 668 keeps it apart (wrapper; alone
// it gives differ 41, gap 6/7). Call.h's Value3 measures the same.
// Left at differ 6, the tail only: r2 and r3 swapped between the address of
// next and its zero, with zero in r3 and the constant in r1 in the native. The
// constant reload takes r1 only while r2 and r3 are busy, so zero is set first
// (the local zero below), but local-alloc then ranks the shorter-lived address
// first and it takes r3. Plain `work->next = 0` allocates them right but builds
// 0x1600 in r3 (differ 8). Not settled by: a pointer local in either order, s16
// or s32 zero, bit-field or unsigned next, two stores, an offset local (combine
// folds it to strh [base, r1]; volatile keeps the add, but it ties to the
// offset's r1 and the longer tail sends work to r7). The native shape is what
// one store with (plus work 0x1600) would reload to; what produces that insn is
// the open lead.
#include "types.h"
#include "system/task.h"

extern char ResourceId_FallingRock;

struct Rock {
  u8 unk00[24];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

struct RockWork *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
void Func_02003a14(void *entry, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Func_02003c34(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_0200379c(void);

void Func_020038dc(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02003a14(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  tile = VramCache_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  i = 63;
  do {
    Func_02003c34(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
    i--;
    sprite++;
    rock++;
  } while (i >= 0);
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_Add(Func_0200379c, 0x480);
}
