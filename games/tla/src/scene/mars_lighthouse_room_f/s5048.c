#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_020074cc;
extern s16 gData_020074ce;
extern struct FieldActor *gData_020074d0[];
extern struct LightOam Data_0200f510[];
extern s16 gData_020075d0;
extern s16 gData_020075d2;

s32 Func_02005698(struct FieldActor *actor);
void Func_02005440(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02005048(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_020074cc].offset >> 5;
  viewY = map->origin[1];
  oam = Data_0200f510;

  for (i = 0; i < gData_020074ce; i++) {
    struct FieldActor *actor = gData_020074d0[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_020075d0;
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
        if (gData_020075d2 == -1)
          oam->priority = Func_02005698(actor);
        else
          oam->priority = gData_020075d2;
        Func_02005440(oam, depth);
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
        if (gData_020075d2 == -1)
          oam->priority = Func_02005698(actor);
        else
          oam->priority = gData_020075d2;
        Func_02005440(oam, depth);
        oam++;
      }
    }
  }
}

#include "iwram_call.h"
#include "task.h"
#include "event_inline.h"

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];

void *Heap_Alloc(s32 size);
void Func_02005420(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02005204(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_020074d0, 64);
  Func_02005420(gField_CachedTilesA, buffer);
  gData_020074cc = Resource_FindFreeEntry();
  VramCache_Load(gData_020074cc, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02005048, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 0;
  gData_020075d2 = -1;
}

void Func_02005284(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_020074d0, 64);
  Func_02005420(gField_CachedTilesB, buffer);
  gData_020074cc = Resource_FindFreeEntry();
  Vram_Load(gData_020074cc, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02005048, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 0;
  gData_020075d2 = -1;
}

void Func_02005304(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_020074d0, 64);
  Func_02005420(gField_CachedTilesC, buffer);
  gData_020074cc = Resource_FindFreeEntry();
  Vram_Load(gData_020074cc, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02005048, TASK_PASS_VBLANK);
  gData_020074ce = 0;
  gData_020075d0 = 1;
  gData_020075d2 = -1;
}

void Func_0200538c(s32 id, s32 value) {
  struct FieldActor *object = (struct FieldActor *)Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_020074ce;
    gData_020074d0[*count] = object;
    (*count)++;
  }
}

void Func_020053bc(s32 value) { gData_020075d2 = value; }
