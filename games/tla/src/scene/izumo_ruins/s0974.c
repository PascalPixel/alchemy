/// @file
/// The overlay code of scene/izumo_ruins between two shared objects, in ROM order.

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
#include "module/scene/izumo_ruins.h"
#include "module/scene/scene.h"
#include "module/tla.h"

extern const s32 gData_02001540[];

void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

void Func_02000974(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Func_020014f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags((struct Actor *)effect, 0);
      Func_02001428((struct Actor *)effect, 2);
      Actor_StartScript(effect, gData_02001540);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Math_Cosine(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16Far() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16Far() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

extern const s32 gData_02001570[];

void Func_02000aa8(void) {
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
      frame = Func_020014f0(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags((struct Actor *)effect, 0);
      Func_02001428((struct Actor *)effect, 2);
      Actor_StartScript(effect, gData_02001570);
      effect->velocity.x = Math_Cosine(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

s32 Func_02001528(s32 terrainId, s32 value, s32 height);

DEFINE_ACTOR_SET_CELL_SHAPE_AT(Func_02000bc0, Func_02001478, Func_02001528)

void Func_02000c4c(s32 a, s32 b) {
  Func_02000bc0(a, b, gMainRoutineWork.currentOwner);
}

#include "../../field/object/set_sprite_priority.c"
