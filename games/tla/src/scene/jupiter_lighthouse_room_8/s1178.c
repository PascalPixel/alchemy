/// @file
/// Scene code of overlay 0x6a1, in ROM order, from s1178's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"

struct IsletsVector {
  s32 x;
  s32 y;
  s32 z;
};

extern const u16 gData_020022d0[];

s32 Func_020021b8(struct Actor *object, struct IsletsVector *position);
struct Actor *Func_020022a8(struct IsletsVector *position, s32 mode);

void Func_02001178(struct Actor *object) {
  struct IsletsVector position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, &position);
  hit = Func_020022a8(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 ==
        gData_020022d0[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (Func_020021b8(object, &position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02002af0;
extern s16 gData_02002af2;
extern struct Actor *gData_02002af4[];
extern struct LightOam gData_02002b34[];
extern s16 gData_02002bf4;
extern s16 gData_02002bf6;

s32 Func_020022b0(struct Actor *actor);
void Func_02002130(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02001200(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02002af0].offset >> 5;
  viewY = map->origin[1];
  oam = gData_02002b34;

  for (i = 0; i < gData_02002af2; i++) {
    struct Actor *actor = gData_02002af4[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02002bf4;
      if (big == 0) {
        s32 x, sy;
        frame = actor->timer;
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
        if (gData_02002bf6 == -1)
          oam->priority = Func_020022b0(actor);
        else
          oam->priority = gData_02002bf6;
        Func_02002130(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = actor->timer;
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
        if (gData_02002bf6 == -1)
          oam->priority = Func_020022b0(actor);
        else
          oam->priority = gData_02002bf6;
        Func_02002130(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];

void *Heap_Alloc(s32 size);
void Func_02002118(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_020013bc(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWordsEntry(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesA, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  VramCache_Load(gData_02002af0, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001200, TASK_PASS_VBLANK);
  gData_02002af2 = 0;
  gData_02002bf4 = 0;
  gData_02002bf6 = -1;
}

void Func_0200143c(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWordsEntry(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesB, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  Vram_Load(gData_02002af0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001200, TASK_PASS_VBLANK);
  gData_02002af2 = 0;
  gData_02002bf4 = 0;
  gData_02002bf6 = -1;
}

void Func_020014bc(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWordsEntry(gData_02002af4, 64);
  Func_02002118(gField_CachedTilesC, buffer);
  gData_02002af0 = Resource_FindFreeEntry();
  Vram_Load(gData_02002af0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001200, TASK_PASS_VBLANK);
  gData_02002af2 = 0;
  gData_02002bf4 = 1;
  gData_02002bf6 = -1;
}

void Func_02001544(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02002af2;
    gData_02002af4[*count] = object;
    (*count)++;
  }
}

void Func_02001574(s32 value) { gData_02002bf6 = value; }

#include "../../field/object/set_sprite_priority.c"
