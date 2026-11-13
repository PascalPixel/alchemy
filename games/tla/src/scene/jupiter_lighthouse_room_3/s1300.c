/// @file
/// Scene code of overlay 0x69c, in ROM order, from s1300's offset up to the next object the overlay links.

#include "local.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"
#include "event_inline.h"

extern const u16 gData_02002478[];

struct Actor *Func_02002438(s32 *position, s32 mode);
s32 Func_02002338(struct Actor *object, s32 *position);

/// Stops the object where it stands (Actor_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Func_02002438) whose sprite shape is not in the footprint table, and again
/// when its own cell test (Func_02002338) is positive.
void Func_02001300(struct Actor *object) {
  s32 position[3];
  struct Actor *other;
  u32 i;
  s32 angle;

  angle = object->facing >> 12;
  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  other = Func_02002438(position, 1);
  if (other != NULL) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)other->sprite)->value28 ==
        gData_02002478[i])
      return;
    i++;
    if (i < 6)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02002338(object, position) > 0)
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

extern s16 gData_02002d2c;
extern s16 gData_02002d2e;
extern struct Actor *gData_02002d30[];
extern struct LightOam Data_02002d70[];
extern s16 gData_02002e30;
extern s16 gData_02002e32;

s32 Func_02002448(struct Actor *actor);
void Func_020022b8(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02001388(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02002d2c].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02002d70;

  for (i = 0; i < gData_02002d2e; i++) {
    struct Actor *actor = gData_02002d30[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02002e30;
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
        if (gData_02002e32 == -1)
          oam->priority = Func_02002448(actor);
        else
          oam->priority = gData_02002e32;
        Func_020022b8(oam, depth);
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
        if (gData_02002e32 == -1)
          oam->priority = Func_02002448(actor);
        else
          oam->priority = gData_02002e32;
        Func_020022b8(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];

void *Heap_Alloc(s32 size);
void Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001544(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002d30, 64);
  Resource_DecodeType01(gField_CachedTilesA, buffer);
  gData_02002d2c = Resource_FindFreeEntry();
  VramCache_Load(gData_02002d2c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001388, TASK_PASS_VBLANK);
  gData_02002d2e = 0;
  gData_02002e30 = 0;
  gData_02002e32 = -1;
}

void Func_020015c4(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002d30, 64);
  Resource_DecodeType01(gField_CachedTilesB, buffer);
  gData_02002d2c = Resource_FindFreeEntry();
  Vram_Load(gData_02002d2c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001388, TASK_PASS_VBLANK);
  gData_02002d2e = 0;
  gData_02002e30 = 0;
  gData_02002e32 = -1;
}

void Func_02001644(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002d30, 64);
  Resource_DecodeType01(gField_CachedTilesC, buffer);
  gData_02002d2c = Resource_FindFreeEntry();
  Vram_Load(gData_02002d2c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001388, TASK_PASS_VBLANK);
  gData_02002d2e = 0;
  gData_02002e30 = 1;
  gData_02002e32 = -1;
}

void Func_020016cc(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02002d2e;
    gData_02002d30[*count] = object;
    (*count)++;
  }
}

void Func_020016fc(s32 value) { gData_02002e32 = value; }

#include "../../field/object/set_sprite_priority.c"
