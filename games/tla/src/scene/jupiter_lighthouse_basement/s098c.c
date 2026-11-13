#include "types.h"
#include "system.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "item_icon.h"
#include "module/scene/clear.h"
#include "module/scene/jupiter_lighthouse_basement.h"
#include "module/scene/scene.h"

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_020007cc(struct GustObject *object);

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

/* VIEW(s098c): variable gData_020024f4 as s16 gData_020024f4; module/scene/clear.h has u16 gData_020024f4[]; tried as the definition's extern u16 gData_020024f4[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_basement/s098c.c:225: `gData_020024f4' has an incomplete type. */
extern s16 gData_020024f4__s098c __asm__("gData_020024f4");

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
      big = gData_020024f4__s098c;
      big = gData_020024f4__s098c;
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

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02000e50(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_020023f4, 64);
  Resource_DecodeType01(gField_CachedTilesA, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  VramCache_Load(gData_020023f0, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4__s098c = 0;
  gData_020024f6 = -1;
}

void Func_02000ed0(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_020023f4, 64);
  Resource_DecodeType01(gField_CachedTilesB, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  Vram_Load(gData_020023f0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4__s098c = 0;
  gData_020024f6 = -1;
}

void Func_02000f50(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_020023f4, 64);
  Resource_DecodeType01(gField_CachedTilesC, buffer);
  gData_020023f0 = Resource_FindFreeEntry();
  Vram_Load(gData_020023f0, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000c94, TASK_PASS_VBLANK);
  gData_020023f2 = 0;
  gData_020024f4__s098c = 1;
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
