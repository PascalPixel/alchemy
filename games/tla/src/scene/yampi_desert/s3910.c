/// @file
/// Scene code of overlay 0x661, in ROM order, from s3910's offset up to the next object the overlay links.

#include "scene.h"
#include "map.h"
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "call.h"

void Func_02003910(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_020041e0(object);
  if (GameFlag_Test(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02003fa8(object, 2);
      break;
    case 1:
      Func_02003fa8(object, 4);
      break;
    case 2:
      Func_02003fa8(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02003fa8(object, 1);
      break;
    case 1:
      Func_02003fa8(object, 3);
      break;
    case 2:
      Func_02003fa8(object, 5);
      break;
    }
  }
}

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

/// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

extern u8 gData_020023c4[];

s32 Math_Sine(s32 angle);

// The rising platforms callback: for each of the 16 entries it raises or lowers
// the platform on its timer, writes its height to the collision and cell maps,
// and carries its rider with a bob.
void Func_020039b8(void) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct RideEntry *entry = work->entries;
  s32 value = work->value;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision = (s32 *)Ram_MapCollision + value;

  for (; i <= 15; collision++, entry++, i++) {
    struct Actor *object;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    object = Actor_GetById(entry->id);
    if (object->x == 0 && object->z == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Func_020041f8(206);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Func_020041f8(395);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Test(0x104)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (object->x >> 20) + (object->z >> 20) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((PIXELS(object->y) + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y = object->y + -0x100000;
    } else {
      s32 wave;

      entry->phase += 0x1000;
      wave = Iwram_MulQ16(0x20000, Math_Sine(entry->phase)) + -0xe0000;
      rider->y = object->y + (entry->height << 16) + wave;
    }
    rider->x = object->x;
    rider->z = object->z;
  }
}

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

extern struct ResourceTableEntry gResourceTableEntries[];

void Func_02003f88(u32 *oam, s32 depth);

// Draws the rising platforms' columns as sprites, one 16-pixel piece per step
// of height and a bobbing top, up to 80 of them, each an OAM entry queued by
// Func_02003f88.
void Func_02003b80(void) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = work->entries;
  s32 i;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, originY;
  struct Actor *object;
  s32 h, x, y, sy, z, depth;
  u32 *p;

  viewX = view[0];
  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->id == 0 || entry->height == 0)
      continue;
    object = Actor_GetById(entry->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    // A goto loop: the ROM moves nothing out of it; as a for loop, 272 of 252
    // instructions differ in tla-en (the sprite's third word moves out).
    h = 0;
    if (h >= entry->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = object->x - viewX;
    y = object->y + (h << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    x = (x >> 16) - 8;
    sy = (sy >> 16) - 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      goto next;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame) | 0x800;
    Func_02003f88(oam, depth);
    oam += 3;
    count++;
  next:
    h += 16;
    if (h < entry->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = object->x - viewX;
    y = object->y + (entry->height << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    x = (x >> 16) - 8;
    /* FAKEMATCH(wrapper): forwarded, so -0x20000 is built for the call and again
     * for the sum; as a direct call, 276 of 252 instructions differ in tla-en. */
    sy = ((sy + Value2((s32 (*)())Iwram_MulQ16, -0x20000, Math_Sine(entry->phase)) + -0x20000) >> 16) - 4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Func_02003f88(oam, depth);
    oam += 3;
    count++;
  }
}

struct YampiDesertRideWork {
  s16 entry;
  u16 value;
  u8 unk004[0x1c0];
  void *block;
};

extern const u8 gField_Tiles64D[];

void *Heap_AllocEwram(s32 size);
void Func_02003f70(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Heap_Release(void *block);

s32 Func_020041f0(void);

void Func_02003da0(s32 unused, s32 value) {
  struct YampiDesertRideWork *work =
      (struct YampiDesertRideWork *)(gData_020023c4 + 0x188);
  void *buffer;

  if (!GameFlag_Test(0x109))
    Call2((void (*)())Iwram_ClearWordsEntry, (s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Func_02003f70(gField_Tiles64D, buffer);
  work->entry = Resource_FindFreeEntry();
  VramCache_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(0x3c0);
  work->value = value;
  Task_Add((s32)Func_020039b8, 0x48d);
  Task_Add((s32)Func_02003b80, 0x480);
  Actor_GetById(Func_020041f0())->priorityFlags |= 0x40;
}

struct Actor *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);

struct RideEntry *Func_02003e3c(s32 slot, s32 id, s32 timer, s32 low,
                                s32 high) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *rider;

  Actor_GetById(id);
  Actor_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(0x109)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= 2;
  }
  rider = Func_02003fc0(0x8000, object->x, object->y, object->z);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}

#undef PIXELS

#include "scene_tables.h"

// clang-format off
u32 gData_02004200[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_0200423c[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004278[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020042b4[] = {
    0x9800e0, 0x9800e8, 0x9800f0, 0xa000d8, 0xa000e0, 0xa000e8,
    0xa000f0, 0xa000f8, 0xa800d8, 0xa800e0, 0xa800e8, 0xa800f0,
    0xa800f8, 0xb000d8, 0xb000f8,
};

u32 gData_020042f0[] = {
    0, 25, 22, 38, 0, 3,
    0, 1, 38,
};
// clang-format on
