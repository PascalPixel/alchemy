// Draft, differ 84 of 179, skeleton gap 16 (tla-en, 2026-11-01)
// Control flow, stores, constants and both sprite tables are the native's; the
// spill of pixel accounts for nearly all that is left. The native keeps pixel at
// sp+0 (its whole 4-byte frame), so work is r7, tiles r6, sprite r5 and layer r8.
// Here pixel wins a low register, work falls to r8 and layer to r10, and the
// push, the moves through high registers and the scratch registers around the
// WINOUT store and the entrance tests all follow from that.
// Measured: the lreg dump gives pixel 5 refs over 22 insns, priority 0.45, against
// work 0.15 and tiles 0.25; it would lose only at 3 refs, as if the loop had no
// depth weight. A diagnostic that exhausted the low registers gave the native's
// ldr, stmia, adds, str sequence, so pixel is a spilled pseudo, and a volatile
// cursor does not give that sequence, so it is not a memory variable. Loop forms
// (for, do, for(;;) with break, top test, --i in the test, i-- first), pixel as
// u8 * or s32, pixel = Heap_Alloc with tiles copied from it and *p then p++ all
// keep 5 refs. A file-level question left: what lowers pixel's priority without
// a device. Not tried as a fix, because they are devices: a taken address and
// extra live variables.
// Forms kept because they matched: the second entrance test reads through
// *(s16 *)& (the same type), which stops PRE copying the load into the first
// arm's tail; a member read there gives that copy, differ 102. The tile store
// does the same job: a member store takes the bit-field path, and CSE then
// shares its zero mask across the function, differ 157. winout is a one-constant
// local because a HImode store of it loads from the pool.
#include "iwram_call.h"
#include "main_routine_work.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/task.h"
#include "types.h"

struct AnkohlRuinsFloatSprite {
  s32 x;
  s32 z;
  s32 y;
  u32 kind;
  u32 oam[3];
};

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
  struct AnkohlRuinsFloatSprite sprites[8];
};

extern struct AnkohlRuinsFloatWork Data_02004030;
void Func_020012c4(void);
s32 GameFlag_Test(s32 flag);
s32 Resource_FindFreeEntry(void);

void Func_02001420(void) {
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  u32 *tiles = Heap_Alloc(128);
  u32 *pixel = tiles;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct AnkohlRuinsFloatSprite *sprite = work->sprites;
  s32 i;
  s32 winout = 0x1b1f;

  if (GameFlag_Test(0x109) == 0)
    Iwram_ClearWords(work, 240);
  i = 31;
fill:
  *pixel++ = 0x11111111;
  i--;
  if (i >= 0)
    goto fill;
  // Spelled through a pointer: a member store shares its zero mask.
  *(s16 *)&work->tile = Resource_FindFreeEntry();
  VramCache_Load(work->tile, 128, tiles);
  Heap_Release(tiles);
  work->unk08 = ((s32 *)layer->unk08)[0];
  work->unk0c = ((s32 *)layer->unk08)[1];
  REG_WINOUT = winout;
  REG_DISPCNT |= 0x8000;
  Task_Add(Func_020012c4, 0x480);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    sprite->x = 0x280000;
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite++;
    sprite->x = 0x380000;
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite++;
    sprite->x = 0xd80000;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite->kind = 2;
    sprite++;
    sprite->x = 0xe80000;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite->kind = 2;
    sprite++;
    sprite->x = 0xb80000;
    sprite->z = 0x580000;
    sprite->y = 0;
    sprite->kind = 4;
    sprite++;
  }
  // Spelled through a pointer: a member read is copied up into the arm above.
  if (*(s16 *)&gMainRoutineWork.entrance == 3 || *(s16 *)&gMainRoutineWork.entrance == 4) {
    sprite->x = 0x280000;
    sprite->z = 0x1b80000;
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = 0x380000;
    sprite->z = 0x1b80000;
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = 0xd80000;
    sprite->y = 0;
    sprite->z = 0x1980000;
    sprite->kind = 0;
    sprite++;
    sprite->x = 0xe80000;
    sprite->y = 0;
    sprite->z = 0x1980000;
    sprite->kind = 1;
    sprite++;
    sprite->x = 0x480000;
    sprite->z = 0x1780000;
    sprite->y = 0;
    sprite->kind = 4;
  }
}
