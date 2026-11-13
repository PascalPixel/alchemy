/// @file
/// Scene code of overlay 0x69f, in ROM order, from s0854's offset up to the next object the overlay links.

#include "types.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "field_coordinate.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"
#include "main_routine_work.h"
#include "field.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_6.h"
#include "module/scene/scene.h"

/* VIEW(s0854): routine Func_02001ab4 as void Func_02001ab4(struct GustObject *object, s32 mode); games/tla/src/scene/jupiter_lighthouse_room_6/jupiter_lighthouse_room_6.h has void Func_02001ab4 ( struct JupiterLighthouseRoom6Object * , s32 ). */
extern void Func_02001ab4__s0854(struct GustObject *object, s32 mode) __asm__("Func_02001ab4");
/* VIEW(s0854): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags); field.h has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlags__s0854(struct GustObject *object, u32 flags) __asm__("Actor_SetSingleChildFlags");
void Func_02000694(struct GustObject *object);
void Func_02000f5c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_02000854(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                    position.z));
  Func_02001ab4__s0854(object, 2);
  Actor_SetSingleChildFlags__s0854(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02000694;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02000f5c(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

s32 Func_0200092c(struct Actor *object) {
  struct JupiterLighthouseAerieSpark spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    s32 x;
    s32 y;
    s32 speed;

    spark.word[0] = 3;
    spark.word[2] = 0xb333;
    spark.word[3] = 0xb333;
    spark.word[1] = 14;
    x = object->x + (((((u32)Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (((u32)Random16Far() * 20) >> 16)) << 16);
    speed = (s32)(((((u32)Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;
    Func_02000f5c(x, y, object->z, 0, speed, frame, 0xb0000, &spark);
  }
  return 0;
}

void Func_020009cc(void) {
  struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *actor10 =
      ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *actor23 =
      ((struct PositionedActor *)Actor_Get(23));
  struct FieldSprite *sprite = actor23->animation;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;
  actor23->motionFlags = 0;
  actor23->y.fixed = actor23->terrainHeight;
  actor9->motionFlags = 0;
  actor9->y.fixed = actor9->terrainHeight;
  actor10->motionFlags = 0;
  actor10->y.fixed = actor10->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    actor9->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(9)), 4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(11)),
                               4);
  }
  if (Event_GetFlag(0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(10)),
                               4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(12)),
                               4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
    actor9->y.fixed += 0x199900;
    actor10->y.fixed += 0x199900;
    {
      s32 step = 0x1b8480;

      actor23->y.fixed += step;
    }
    actor9->motionFlags = 4;
    actor10->motionFlags = 4;
    actor23->motionFlags = 4;
  }
}

/* VIEW(s0854): variable gData_0200242c as struct Actor *gData_0200242c[]; module/scene/sea_of_time.h has struct Trigger gData_0200242c[]. */
extern struct Actor *gData_0200242c__s0854[] __asm__("gData_0200242c");
/* VIEW(s0854): variable gData_0200252e as s16 gData_0200252e; module/scene/scene.h has u16 gData_0200252e. */
extern s16 gData_0200252e__s0854 __asm__("gData_0200252e");

void Func_02001a84(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02000b5c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02002428].offset >> 5;
  viewY = map->origin[1];
  oam = Data_0200246c;

  for (i = 0; i < gData_0200242a; i++) {
    struct Actor *actor = gData_0200242c__s0854[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_0200252c;
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
        if (gData_0200252e__s0854 == -1)
          oam->priority = Func_02001bd4(actor);
        else
          oam->priority = gData_0200252e__s0854;
        Func_02001a84(oam, depth);
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
        if (gData_0200252e__s0854 == -1)
          oam->priority = Func_02001bd4(actor);
        else
          oam->priority = gData_0200252e__s0854;
        Func_02001a84(oam, depth);
        oam++;
      }
    }
  }
}

/* VIEW(s0854): routine Resource_DecodeType01 as void Resource_DecodeType01(const void *source, void *destination); resource.h has s32 Resource_DecodeType01(const void *source, void *destination). */
extern void Resource_DecodeType01__s0854(const void *source, void *destination) __asm__("Resource_DecodeType01");

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000d18(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200242c__s0854, 64);
  Resource_DecodeType01__s0854(gField_CachedTilesA, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  VramCache_Load(gData_02002428, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e__s0854 = -1;
}

void Func_02000d98(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_0200242c__s0854, 64);
  Resource_DecodeType01__s0854(gField_CachedTilesB, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  Vram_Load(gData_02002428, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e__s0854 = -1;
}

void Func_02000e18(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_0200242c__s0854, 64);
  Resource_DecodeType01__s0854(gField_CachedTilesC, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  Vram_Load(gData_02002428, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 1;
  gData_0200252e__s0854 = -1;
}

extern s32 gData_020023c0;

s32 Func_020018bc(s32 value);
void Func_02000108(void);
void Func_020003a4(s32 value);

void Func_02000ea0(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    object->timer = value;
    gData_0200242c__s0854[gData_0200242a] = object;
    gData_0200242a++;
  }
}

void Func_02000ed0(u16 value) { gData_0200252e__s0854 = value; }

#include "../../field/object/set_sprite_priority.c"
