/// @file
/// Scene code of overlay 0x69d, in ROM order, from s0b50's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "field_event.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "frame.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_4.h"
#include "module/scene/scene.h"

/* VIEW(s0b50): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags); field.h has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlags__s0b50(struct FieldEffect *effect, s32 flags) __asm__("Actor_SetSingleChildFlags");
void Func_02000990(struct GustObject *object);
void Func_02001258(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

struct GustObject *Func_02000b50(struct GustObject *source) {
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
  Func_02001da8(object, 2);
  Actor_SetSingleChildFlags__s0b50(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Func_02000990;
  object->unk30 = 0x18000;
  options.priority = 1;
  options.palette = 7;
  Func_02001258(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,
                &options);
  return object;
}

s32 Func_02000c28(struct Actor *object) {
  struct EffectOptions options;
  s32 x;
  s32 y;
  s32 velocityY;

  if ((gFrameCount & 7) == 0) {
    options.priority = 3;
    options.startScaleX = 0xb333;
    options.startScaleY = 0xb333;
    options.palette = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    velocityY = (s32)(((((Random16Far() * 5) >> 16) << 16) + 0x50000)) / 10;
    Func_02001258(x, y, object->z, 0, velocityY, 0,
                  EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                      EFFECT_USE_START_SCALE,
                  &options);
  }
  return 0;
}

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02000cc8(void) {
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  struct FieldSprite *sprite = rider->sprite;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  Actor_Get(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (Event_GetFlag(0xa0a)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Actor_Get(9), 4);
    Animation_ApplyChildValues(Actor_Get(11), 4);
  }
  if (Event_GetFlag(0xa0b)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Animation_ApplyChildValues(Actor_Get(10), 4);
    Animation_ApplyChildValues(Actor_Get(12), 4);
  }
  if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

/* VIEW(s0b50): variable gData_02002760 as struct Actor *gData_02002760[]; module/scene/alhafra.h has s16 gData_02002760[]; tried as the definition's extern s16 gData_02002760[], which the object already declares, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_4/s0b50.c:143: `gData_02002760' undeclared (first use in this function). */
extern struct Actor *gData_02002760__s0b50[] __asm__("gData_02002760");

s32 Func_02001ee0(struct Actor *actor);
void Func_02001d80(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
void Func_02000e58(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_0200275c].offset >> 5;
  viewY = map->origin[1];
  oam = gData_020027a0;

  for (i = 0; i < gData_0200275e; i++) {
    struct Actor *actor = gData_02002760__s0b50[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02002860;
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
        if (gData_02002862 == -1)
          oam->priority = Func_02001ee0(actor);
        else
          oam->priority = gData_02002862;
        Func_02001d80(oam, depth);
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
        if (gData_02002862 == -1)
          oam->priority = Func_02001ee0(actor);
        else
          oam->priority = gData_02002862;
        Func_02001d80(oam, depth);
        oam++;
      }
    }
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void Func_02001014(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWords(gData_02002760__s0b50, 64);
  Resource_DecodeType01(gField_CachedTilesA, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  VramCache_Load(gData_0200275c, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 0;
  gData_02002862 = -1;
}

void Func_02001094(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWords(gData_02002760__s0b50, 64);
  Resource_DecodeType01(gField_CachedTilesB, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  Vram_Load(gData_0200275c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 0;
  gData_02002862 = -1;
}

void Func_02001114(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWords(gData_02002760__s0b50, 64);
  Resource_DecodeType01(gField_CachedTilesC, buffer);
  gData_0200275c = Resource_FindFreeEntry();
  Vram_Load(gData_0200275c, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000e58, TASK_PASS_VBLANK);
  gData_0200275e = 0;
  gData_02002860 = 1;
  gData_02002862 = -1;
}

void Func_0200119c(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    s16 *count;

    object->timer = value;
    count = &gData_0200275e;
    gData_02002760__s0b50[*count] = object;
    (*count)++;
  }
}

void Func_020011cc(s32 value) { gData_02002862 = value; }

#include "../../field/object/set_sprite_priority.c"
