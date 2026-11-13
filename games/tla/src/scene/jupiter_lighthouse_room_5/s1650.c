#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_inline.h"

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 speed;
  s32 age;
};

struct SparkSprite {
  u8 unk00[4];
  u8 y;
  u8 low : 5;
  u8 fullColor : 1;
  u8 shape : 2;
  u16 attr1;
  u8 tile;
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[0x14];
  u16 depth;
  u8 unk20[8];
};

struct SparkWork {
  u8 tiles[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
  u16 tile;
  s16 vramEntry;
};

s32 Math_Cosine(s32 angle);

/// Adds count sparks to the spark ring's 96 entries in heap slot 220, each
/// with a random angle from 0x6000 across three quarters of a turn, the
/// cosine and negated sine of it at 64 pixels as its direction, a random
/// speed of 16 to 31 and an age of 0.
void Func_02001650(s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 i;
  s32 radius;
  u64 cosine;
  s32 speed;

  if (count > 0) {
    i = count;
  next:
    spark = &work->sparks[work->next];
    angle = ((Random16Far() * 3) >> 2) + 0x6000;
    radius = 0x400000;
    speed = ((Random16Far() << 4) >> 16) + 16;
    /* FAKEMATCH(temporary): the cosine held in a 64-bit temporary; as
     * Iwram_MulQ16(Math_Cosine(angle), radius), 14 of 92 instructions differ
     * in tla-en. */
    cosine = Math_Cosine(angle);
    spark->dx = Iwram_MulQ16(cosine, radius) >> 16;
    spark->dy = -Iwram_MulQ16(Math_Sin(angle), radius) >> 16;
    spark->speed = speed;
    spark->age = 0;
    work->next++;
    work->next %= 96;
    if (--i != 0)
      goto next;
  }
}

#include "actor.h"
#include "main_routine_work.h"

void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
extern char ResourceId_ColouredOrbs;
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);
s32 Func_020029f0(struct Actor *object);
s32 Func_020029f8(struct Actor *object);
void Func_02001538(void);

void Func_02001718(const s32 *from, const s32 *to) {
  struct SparkWork *work = HeapSlot_AllocFar(220, sizeof(struct SparkWork));
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct SparkSprite *sprite;
  struct Spark *spark;
  s32 entry;
  s32 tile;
  s32 i;

  work->from[0] = from[0];
  work->from[1] = from[1];
  work->from[2] = from[2];
  work->to[0] = to[0];
  work->to[1] = to[1];
  work->to[2] = to[2];
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs), work);
  entry = Resource_FindFreeEntry();
  tile = Vram_Load(entry, 0x400, work);
  work->tile = tile;
  work->vramEntry = entry;
  i = 0;
  spark = work->sparks;
  sprite = work->sprites;
  for (; i <= 95; i++, sprite++, spark++) {
    Func_020029e0(sprite, 4, 4, 0, tile + (i & 15) * 2);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = Func_020029f0(leader) & 3;
    // Through a union, so this store may alias the age store after it.
    ((union {
       u16 v;
       s32 w;
     } *)&sprite->depth)
        ->v = Func_020029f8(leader) - 1;
    spark->age = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  /* FAKEMATCH(wrapper): the task through Value2; as a direct call, 8 of 145
   * instructions differ in tla-en. */
  Task_AddFrame(Func_02001538, 0x480);
}

#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"

s32 Resource_ResetEntry(u32 slot);

void Func_0200185c(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_02001538);
  Resource_ResetEntry(work->vramEntry);
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
extern struct Actor *gData_02003448[];
extern struct LightOam gData_02003488[];
extern s16 gData_02003548;
extern s16 gData_0200354a;

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
    struct Actor *actor = gData_02003448[i];
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
        if (gData_0200354a == -1)
          oam->priority = Func_020029f0(actor);
        else
          oam->priority = gData_0200354a;
        Func_020027d8(oam, depth);
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

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001a44(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003448, 64);
  Resource_DecodeType01(gField_CachedTilesA, buffer);
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
  Resource_DecodeType01(gField_CachedTilesB, buffer);
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
  Resource_DecodeType01(gField_CachedTilesC, buffer);
  gData_02003444 = Resource_FindFreeEntry();
  Vram_Load(gData_02003444, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001888, TASK_PASS_VBLANK);
  gData_02003446 = 0;
  gData_02003548 = 1;
  gData_0200354a = -1;
}

void Func_02001bcc(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02003446;
    gData_02003448[*count] = object;
    (*count)++;
  }
}

void Func_02001bfc(s32 value) { gData_0200354a = value; }

#include "../../field/object/set_sprite_priority.c"
