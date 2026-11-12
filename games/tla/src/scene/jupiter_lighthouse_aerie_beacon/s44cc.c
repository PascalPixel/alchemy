/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s44cc's offset up to the next object the overlay links.

#include "types.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "frame.h"
#include "field_event.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"

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
void Func_02005754(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_0200430c(struct GustObject *object);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_020044cc(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Math_Cosine(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                    position.z));
  Func_02005754(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_0200430c;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02004bd4(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

u32 Random16Far(void);

s32 Func_020045a4(struct FieldActor *object) {
  s32 params[10];

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params[0] = 3;
    params[2] = 0xb333;
    params[3] = 0xb333;
    params[1] = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    Func_02004bd4(x, y, object->z, 0,
                  (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, params);
  }
  return 0;
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);

static __inline__ void Func_02001c10_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

void Func_02004644(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 165 of 165 instructions differ in
   * tla-en. */
  struct Actor *left = Actor_GetById(9);
  struct Actor *right = Actor_GetById(10);
  struct Actor *rider = Actor_GetById(23);
  struct FieldSprite *sprite = rider->animation;

  Func_02001c10_(24, 0x2000000, 0xf80000);
  Func_02001c10_(23, 0x2000000, 0xf80000);
  Func_02001c10_(9, 0x1d80000, 0x1000000);
  Func_02001c10_(10, 0x2280000, 0x1000000);
  Actor_GetById(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (GameFlag_Test_(0xa0a)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_GetById(9), 4);
    Animation_ApplyChildValues(Actor_GetById(11), 4);
  }
  if (GameFlag_Test_(0xa0b)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_GetById(10), 4);
    Animation_ApplyChildValues(Actor_GetById(12), 4);
  }
  if (GameFlag_Test_(0xa0a) && GameFlag_Test_(0xa0b)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
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

extern s16 gData_02006a6c;
extern s16 gData_02006a6e;
extern struct FieldActor *gData_02006a70[];
extern struct LightOam Data_02006ab0[];
extern s16 gData_02006b70;
extern s16 gData_02006b72;

s32 Func_020059bc(struct FieldActor *actor);
void Func_0200571c(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_020047d4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02006a6c].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02006ab0;

  for (i = 0; i < gData_02006a6e; i++) {
    struct FieldActor *actor = gData_02006a70[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02006b70;
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
        if (gData_02006b72 == -1)
          oam->priority = Func_020059bc(actor);
        else
          oam->priority = gData_02006b72;
        Func_0200571c(oam, depth);
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
        if (gData_02006b72 == -1)
          oam->priority = Func_020059bc(actor);
        else
          oam->priority = gData_02006b72;
        Func_0200571c(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];

void *Heap_Alloc(s32 size);
void Func_02005704(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Heap_Release(void *block);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02004990(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02006a70, 64);
  Func_02005704(gField_CachedTilesA, buffer);
  gData_02006a6c = Resource_FindFreeEntry();
  VramCache_Load(gData_02006a6c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_020047d4, TASK_PASS_VBLANK);
  gData_02006a6e = 0;
  gData_02006b70 = 0;
  gData_02006b72 = -1;
}

void Func_02004a10(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02006a70, 64);
  Func_02005704(gField_CachedTilesB, buffer);
  gData_02006a6c = Resource_FindFreeEntry();
  Vram_Load(gData_02006a6c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_020047d4, TASK_PASS_VBLANK);
  gData_02006a6e = 0;
  gData_02006b70 = 0;
  gData_02006b72 = -1;
}

void Func_02004a90(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02006a70, 64);
  Func_02005704(gField_CachedTilesC, buffer);
  gData_02006a6c = Resource_FindFreeEntry();
  Vram_Load(gData_02006a6c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_020047d4, TASK_PASS_VBLANK);
  gData_02006a6e = 0;
  gData_02006b70 = 1;
  gData_02006b72 = -1;
}

void Func_02004b18(s32 id, s32 value) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (object != NULL) {
    object->timer = value;
    gData_02006a70[gData_02006a6e] = object;
    gData_02006a6e++;
  }
}

void Func_02004b48(s32 value) { gData_02006b72 = value; }

#include "../../field/object/set_sprite_priority.c"
