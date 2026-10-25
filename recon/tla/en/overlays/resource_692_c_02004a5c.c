// Draft, differ 9 of 88: the falling rocks' spawn, overlay 692's copy of overlay 6a8's falling
// rocks (resource_6a8_c_02004114.c, whose header says what differs there and
// why the loops are goto loops).
#include "types.h"
#include "ram_buffer.h"

struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

extern char ResourceId_FallingRock;

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02004c58(void *resource, void *destination);
void Func_02004f10(struct RockSprite *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_02004f18(struct RockSprite *sprite, struct Rock *rock);
void Func_02004f20(struct Rock *rock, s32 a, s32 b);

static __inline__ s32 VramLoad(s32 entry, s32 size, void *data) {
  /* FAKEMATCH: as in the scene layer, the inline parameters keep the size its
   * own constant; called directly, the rock offset reuses it across the call.
   */
  return VramCache_Load(entry, size, data);
}

void Func_02004a5c(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work = Ram_HeapSlots->menuRuntime;
  s32 i;

  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the offsets on every pass, and a for loop hoists them. */
  if (count > 0) {
    i = count;
  loop:;
    {
      s32 angle = Random16Far();
      struct Rock *rock = &work->rocks[work->next];

      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      Vector_AddPolarOffsetFar((Random16Far() << 3) + 0x80000, angle,
                               rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = 0x50000;
      rock->velocity[2] = 0;
      Vector_AddPolarOffsetFar((Random16Far() << 1) + 0x20000, angle,
                               rock->velocity);
      work->next = (work->next + 1) & 63U;
    }
    i--;
    if (i != 0)
      goto loop;
  }
}
