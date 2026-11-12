/// @file
/// Scene code of overlay 0x69e, in ROM order, from s185c's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"

struct SparkWork {
  u8 unk0000[0x1d9c];
  s16 resource;
};

void Resource_ResetEntry(s32 entry);
void HeapSlot_Release(s32 slot);
void Func_02001538(void);

void Func_0200185c(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_02001538);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02003444;
extern s16 gData_02003446;
extern struct FieldActor *gData_02003448[];
extern struct LightOam gData_02003488[];
extern s16 gData_02003548;
extern s16 gData_0200354a;

s32 Func_020029f0(struct FieldActor *actor);
void Func_020027d8(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02001888(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02003444].offset >> 5;
  viewY = map->origin[1];
  oam = gData_02003488;

  for (i = 0; i < gData_02003446; i++) {
    struct FieldActor *actor = gData_02003448[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02003548;
      if (big == 0) {
        s32 x, sy;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_0200354a == -1)
          oam->priority = Func_020029f0(actor);
        else
          oam->priority = gData_0200354a;
        Func_020027d8(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_0200354a == -1)
          oam->priority = Func_020029f0(actor);
        else
          oam->priority = gData_0200354a;
        Func_020027d8(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02003444;
extern s16 gData_02003446;
extern struct FieldActor *gData_02003448[];
extern s16 gData_02003548;
extern s16 gData_0200354a;

void *Heap_Alloc(s32 size);
void Func_020027b8(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02001888(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001a44(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003448, 64);
  Func_020027b8(gField_CachedTilesA, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  VramCache_Load(gData_02003444, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 0;
  gData_0200354a = -1;
}

void Func_02001ac4(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02003448, 64);
  Func_020027b8(gField_CachedTilesB, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  Vram_Load(gData_02003444, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 0;
  gData_0200354a = -1;
}

void Func_02001b44(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02003448, 64);
  Func_020027b8(gField_CachedTilesC, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  Vram_Load(gData_02003444, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 1;
  gData_0200354a = -1;
}

extern s16 gData_02003446;
extern struct FieldActor *gData_02003448[];
extern s16 gData_0200354a;

void Func_02001bcc(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02003446;
    gData_02003448[*count] = object;
    (*count)++;
  }
}

void Func_02001bfc(s32 value) { gData_0200354a = value; }
