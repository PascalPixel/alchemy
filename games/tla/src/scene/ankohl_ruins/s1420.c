#include "iwram_call.h"
#include "main_routine_work.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/task.h"
#include "types.h"
#include "event_inline.h"

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
s32 Resource_FindFreeEntry(void);

void Func_02001420(void) {
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  u32 *tiles = Heap_Alloc(128);
  u32 *pixel = tiles;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct AnkohlRuinsFloatSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(0x109) == 0)
    Iwram_ClearWords(work, 240);
  i = 31;
fill:
  *pixel++ = 0x11111111;
  i--;
  if (i >= 0)
    goto fill;
  // Spelled through a pointer: a member store shares its zero mask.
  *(s16 *)&work->tile = Resource_FindFreeEntry();
  Vram_Load(work->tile, 128, tiles);
  Heap_Release(tiles);
  work->unk08 = ((s32 *)layer->unk08)[0];
  work->unk0c = ((s32 *)layer->unk08)[1];
  /* FAKEMATCH(one-pass): the block builds the value after the stores before it;
   * as a plain store, 6 of 179 instructions differ in tla-en. */
  do {
    s32 winout = 0x1b1f;

    REG_WINOUT = winout;
  } while (0);
  REG_DISPCNT |= 0x8000;
  Task_AddFrame(Func_020012c4, 0x480);
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
  if (*(s16 *)&gMainRoutineWork.entrance == 3 ||
      *(s16 *)&gMainRoutineWork.entrance == 4) {
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
