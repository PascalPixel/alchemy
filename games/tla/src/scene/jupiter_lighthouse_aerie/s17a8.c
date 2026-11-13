/// @file
/// Scene code of overlay 0x6a4, in ROM order, from s17a8's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "event_inline.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"
#include "gamepak/far_call/system.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_aerie.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "resource.h"

struct BeamWork {
  u8 tiles[0x200];
  struct EffectSprite sprites[32];
  struct BeamSpark sparks[32];
  struct EffectSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 start[3];
  u8 unkae8[0x18];
  s16 step;
  s16 unkb02;
  u16 largeTile;
  u16 smallTile;
  u8 unkb08;
  u8 unkb09;
  u8 unkb0a;
  u8 side;
  struct Actor *first;
  struct Actor *second;
};

void Func_02002a20(struct Actor *object);

static __inline__ void SetUp(struct EffectSprite *sprite, s32 pivotX,
                             s32 pivotY, u32 attributes, s32 tile) {
  /* FAKEMATCH(wrapper): the setup forwarded inline; as a direct call, 2 of 316
   * instructions differ in tla-en. */
  Func_02002b30(sprite, pivotX, pivotY, attributes, tile);
}

void Func_020017a8(s32 a, s32 b, s32 side) {
  struct BeamWork *work = HeapSlot_AllocFar(220, sizeof(struct BeamWork));
  struct Actor *carrier =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 460);
  struct EffectSprite *sprite;
  struct EffectSprite *large;
  struct BeamSpark *spark;
  s32 smallEntry;
  s32 largeEntry;
  s32 smallTile;
  s32 largeTile;
  s32 i;
  s32 age;

  work->first = Actor_Get(a);
  work->second = Actor_Get(b);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
  smallEntry = Resource_FindFreeEntry();
  smallTile = Vram_Load(smallEntry, 256, work);
  work->smallTile = smallTile;
  spark = work->sparks;
  age = 0;
  sprite = work->sprites;
  i = 31;
  for (; i >= 0; age -= 2, i--, sprite++, spark++) {
    Func_02002b30(sprite, 4, 4, 0, smallTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    *(u16 *)&sprite->depth = 240;
    spark->age = age;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GoldenTrident), work);
  largeEntry = Resource_FindFreeEntry();
  {
    /* FAKEMATCH(temporary): sprite also holds the size; as a plain argument, 7
     * of 316 instructions differ in tla-en. */
    sprite = (struct EffectSprite *)512;
    largeTile = Vram_Load(largeEntry, (s32)sprite, work);
  }
  work->largeTile = largeTile;
  large = &work->large;
  SetUp(large, 30, 7, 0x80004000, largeTile);
  *(u16 *)&large->depth = 240;
  large->oam.priority = 1;
  large->oam.palette256 = 1;
  large->oam.palette = 0;
  large->scaleX = 0;
  large->scaleY = 0;
  if (side == 0) {
    work->from[0] = 152 << 16;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 235 << 17;
    work->to[1] = 0x200000;
    work->to[2] = 164 << 16;
  } else {
    work->from[0] = 218 << 18;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 0x2260000;
    work->to[1] = 0x200000;
    work->to[2] = 164 << 16;
  }
  work->start[0] = work->from[0];
  work->start[1] = work->from[1];
  work->start[2] = work->from[2];
  carrier->speedLimit = 0x8000;
  carrier->acceleration = 0x8000;
  Func_02002a20(carrier);
  FieldActor_SetMoveTarget(carrier, work->start[0], work->start[1], work->start[2]);
  Event_PlaySound(142);
  Func_0200105c(work->first);
  FieldEvent_Wait(40);
  work->side = side;
  work->step = 0;
  work->unkb02 = 0;
  work->unkb09 = 1;
  work->unkb0a = 0;
  Task_AddFrame(Func_02001080, 0x480);
  while (work->step != 999)
    System_Wait(1);
  FieldTask_Remove(Func_02001080);
  Resource_ResetEntry(largeEntry);
  Resource_ResetEntry(smallEntry);
  HeapSlot_Release(220);
}

extern s16 gData_02003380;

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02001a78(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02003380].offset >> 5;
  viewY = map->origin[1];
  oam = Data_020033c4;

  for (i = 0; i < gData_02003382; i++) {
    struct Actor *actor = gData_02003384[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02003484;
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
        if (gData_02003486 == -1)
          oam->priority = Func_02002b48(actor);
        else
          oam->priority = gData_02003486;
        Func_020029c0(oam, depth);
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
        if (gData_02003486 == -1)
          oam->priority = Func_02002b48(actor);
        else
          oam->priority = gData_02003486;
        Func_020029c0(oam, depth);
        oam++;
      }
    }
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001c34(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02003384, 64);
  Resource_DecodeType01(gField_CachedTilesA, buffer);
  gData_02003380 = Resource_FindFreeEntry();
  VramCache_Load(gData_02003380, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02001a78, TASK_PASS_VBLANK);
  gData_02003382 = 0;
  gData_02003484 = 0;
  gData_02003486 = -1;
}

void Func_02001cb4(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02003384, 64);
  Resource_DecodeType01(gField_CachedTilesB, buffer);
  gData_02003380 = Resource_FindFreeEntry();
  Vram_Load(gData_02003380, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001a78, TASK_PASS_VBLANK);
  gData_02003382 = 0;
  gData_02003484 = 0;
  gData_02003486 = -1;
}

void Func_02001d34(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02003384, 64);
  Resource_DecodeType01(gField_CachedTilesC, buffer);
  gData_02003380 = Resource_FindFreeEntry();
  Vram_Load(gData_02003380, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001a78, TASK_PASS_VBLANK);
  gData_02003382 = 0;
  gData_02003484 = 1;
  gData_02003486 = -1;
}

void Func_02001dbc(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_02003382;
    gData_02003384[*count] = object;
    (*count)++;
  }
}

void Func_02001dec(s32 value) { gData_02003486 = value; }

#include "../../field/object/set_sprite_priority.c"
