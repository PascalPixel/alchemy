/// @file
/// The Izumo Ruins' rubble, sprayed and burst from the leader as a pillar
/// crumbles (Rubble_Spray, Rubble_Burst).

#include "../shared/map_cells.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "map.h"
#include "system/ram_buffer.h"
#include "izumo_ruins.h"
#include "field.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/tla.h"

struct Actor;
struct FieldSprite;
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Leader_SetCellShapeAt(s32 a, s32 b);
u32 Sprite_ShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

extern const s32 gIzumoRuins_RubbleSprayScript[];

void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

void Rubble_Spray(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gIzumoRuins_RubbleSprayScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

extern const s32 gIzumoRuins_RubbleBurstScript[];

void Rubble_Burst(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation((struct Actor *)effect, 2);
      Actor_StartScript(effect, gIzumoRuins_RubbleBurstScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

s32 MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);

DEFINE_ACTOR_SET_CELL_SHAPE_AT(Actor_SetCellShapeAt, MapGrid_GetTerrainKind,
                               MapCollision_FindOrAddTag)

void Leader_SetCellShapeAt(s32 a, s32 b) {
  Actor_SetCellShapeAt(a, b, gMainRoutineWork.currentOwner);
}

#include "../../field/object/set_sprite_priority.c"
