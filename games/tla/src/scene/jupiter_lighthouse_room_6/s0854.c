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

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

s32 Math_Cosine(s32 angle);
void Func_02001ab4(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_02000694(struct GustObject *object);
void Func_02000f5c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_02000854(struct GustObject *source) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 69 of 92 instructions differ in
   * tla-en. */
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
  Func_02001ab4(object, 2);
  Actor_SetSingleChildFlags(object, 0);
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

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

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

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02002428;
extern s16 gData_0200242a;
extern struct FieldActor *gData_0200242c[];
extern struct LightOam Data_0200246c[];
extern s16 gData_0200252c;
extern s16 gData_0200252e;

s32 Func_02001bd4(struct FieldActor *actor);
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
    struct FieldActor *actor = gData_0200242c[i];
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
        if (gData_0200252e == -1)
          oam->priority = Func_02001bd4(actor);
        else
          oam->priority = gData_0200252e;
        Func_02001a84(oam, depth);
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
        if (gData_0200252e == -1)
          oam->priority = Func_02001bd4(actor);
        else
          oam->priority = gData_0200252e;
        Func_02001a84(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];
extern s16 gData_02002428;
extern s16 gData_0200242a;
extern struct FieldActor *gData_0200242c[];
extern s16 gData_0200252c;
extern s16 gData_0200252e;

void *Heap_Alloc(s32 size);
void Func_02001a6c(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);
void Func_02000b5c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000d18(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesA, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  VramCache_Load(gData_02002428, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e = -1;
}

void Func_02000d98(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesB, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  Vram_Load(gData_02002428, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 0;
  gData_0200252e = -1;
}

void Func_02000e18(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_0200242c, 64);
  Func_02001a6c(gField_CachedTilesC, buffer);
  gData_02002428 = Resource_FindFreeEntry();
  Vram_Load(gData_02002428, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000b5c, TASK_PASS_VBLANK);
  gData_0200242a = 0;
  gData_0200252c = 1;
  gData_0200252e = -1;
}

extern s32 gData_020023c0;
extern s16 gData_0200242a;

s32 Func_020018bc(s32 value);
void Func_020000b4(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

void Func_02000ea0(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    object->action = value;
    gData_0200242c[gData_0200242a] = object;
    gData_0200242a++;
  }
}

void Func_02000ed0(u16 value) { gData_0200252e = value; }
