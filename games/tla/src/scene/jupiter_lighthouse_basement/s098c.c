#include "types.h"
#include "system.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"

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
void Func_02001bec(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct GustObject *object, u32 flags);
void Func_020007cc(struct GustObject *object);
void Func_02001094(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

struct GustObject *Func_0200098c(struct GustObject *source) {
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
  Func_02001bec(object, 2);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_020007cc;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02001094(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

#include "frame.h"
#include "field_coordinate.h"
#include "field_sprite.h"
#include "field_event.h"
#include "task.h"

s32 Func_02000a64(struct Actor *object) {
  struct EffectOptions params;

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params.priority = 3;
    params.startScaleX = 0xb333;
    params.startScaleY = 0xb333;
    params.palette = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    Func_02001094(x, y, object->z, 0,
                  (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, &params);
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

void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);

void Func_02000b04(void) {
  struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9));
  struct PositionedActor *actor10 = ((struct PositionedActor *)Actor_Get(10));
  struct PositionedActor *actor23 = ((struct PositionedActor *)Actor_Get(23));
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
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(11)), 4);
  }
  if (Event_GetFlag(0xa0b)) {
    actor10->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      actor23->y.fixed += step;
    }
    actor23->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(10)), 4);
    Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(12)), 4);
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

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
#include "ram_buffer.h"
#include "map.h"
#include "vram_tab.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_020023f0;
extern s16 gData_020023f2;
extern struct Actor *gData_020023f4[];
extern struct LightOam Data_02002434[];
extern s16 gData_020024f4;
extern s16 gData_020024f6;

s32 Func_02001d24(struct Actor *actor);
void Func_02001bc4(struct LightOam *oam, s32 depth);

void Func_02000c94(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_020023f0].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02002434;

  for (i = 0; i < gData_020023f2; i++) {
    struct Actor *actor = gData_020023f4[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_020024f4;
      big = gData_020024f4;
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
        if (gData_020024f6 == -1)
          oam->priority = Func_02001d24(actor);
        else
          oam->priority = gData_020024f6;
        Func_02001bc4(oam, depth);
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
        if (gData_020024f6 == -1)
          oam->priority = Func_02001d24(actor);
        else
          oam->priority = gData_020024f6;
        Func_02001bc4(oam, depth);
        oam++;
      }
    }
  }
}

extern u8 gField_CachedTilesA[];
extern u8 gField_CachedTilesB[];
extern u8 gField_CachedTilesC[];

void Func_02001bac(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000e50(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_020023f4, 64);
  Func_02001bac(gField_CachedTilesA, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  VramCache_Load(gData_020023f0, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4 = 0;
  gData_020024f6 = -1;
}

void Func_02000ed0(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_020023f4, 64);
  Func_02001bac(gField_CachedTilesB, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  Vram_Load(gData_020023f0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4 = 0;
  gData_020024f6 = -1;
}

void Func_02000f50(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_020023f4, 64);
  Func_02001bac(gField_CachedTilesC, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  Vram_Load(gData_020023f0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4 = 1;
  gData_020024f6 = -1;
}

void Func_02000fd8(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_020023f2;
    gData_020023f4[*count] = object;
    (*count)++;
  }
}

void Func_02001008(s32 value) { gData_020024f6 = value; }

#include "../../field/object/set_sprite_priority.c"
