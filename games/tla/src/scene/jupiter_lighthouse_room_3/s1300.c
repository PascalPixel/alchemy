/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/jupiter_lighthouse.h"
#include "../shared/jupiter_lighthouse_room_3.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

struct Actor *Func_02002438(s32 *position, s32 mode);

/// Stops the object where it stands (Actor_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Func_02002438) whose sprite shape is not in the footprint table, and again
/// when its own cell test (MapGrid_CheckCollision) is positive.
void PushTarget_UpdatePusher(struct Actor *object) {
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
        gPushKinds[i])
      return;
    i++;
    if (i < 6)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (MapGrid_CheckCollision(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
}

extern s16 gLightSpriteTile;

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Import_OamAddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
