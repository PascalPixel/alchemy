/// @file
/// Scene code of overlay 0x6a1, in ROM order.

#include "../shared/jupiter_lighthouse.h"
#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_8.h"
#include "module/scene/scene.h"



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
        gPushKinds[i])
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

extern s16 gLightSpriteTile;
extern struct Actor *gData_02002af4[];
extern struct LightOam gData_02002b34[];

void Import_OamAddSpriteAtDepth(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gData_02002b34,
    gLightSpriteCount, gData_02002af4, gLightSpritesLarge, gLightSpritePriority,
    Func_020022b0, Import_OamAddSpriteAtDepth)


// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_02002af4, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_02002af4, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_02002af4, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount, gData_02002af4)

void Func_02001574(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
