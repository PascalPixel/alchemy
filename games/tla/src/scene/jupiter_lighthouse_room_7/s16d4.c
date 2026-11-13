/// @file
/// Scene code of overlay 0x6a0, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
#include "../shared/map_cells.h"
#include "../shared/effects.h"
#include "types.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../shared/jupiter_lighthouse_aerie.h"
#include "field_coordinate.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_7.h"
#include "module/scene/scene.h"

void Actor_SelectAnimation(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Whirlwind_Update(struct GustObject *object);
void Func_02001f74(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation, Whirlwind_Update,
                       Func_02001f74)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, Func_02001f74)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

DEFINE_ACTORS_SET_CELLS_OPEN(Actors_SetCellsOpen, Actor_SetCellOpen)

// Finds the current target's table row and shrinks it.
void Func_02001a48(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
  // A goto loop: as a do-while loop, 42 of 61 instructions differ in tla-en.
top:
  {
    u32 entry = *list;
    /* FAKEMATCH(dead): the effect work read through its own address; as
     * work->objectId, 21 of 61 instructions differ in tla-en. */
    s32 id = (*&work)->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Func_02002c1c,
                           Actor_SelectAnimation)

extern u32 gData_02003600[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, Data_02003640,
                          gData_020035fe, gData_02003600, gLightSpritesLarge,
                          gLightSpritePriority, BattleEffect_GetObjectPriority,
                          Import_OamAddSpriteAtDepth)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_02003600,
                            gLightSpriteTile, LightSprites_Draw, gData_020035fe,
                            gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_02003600,
                            gLightSpriteTile, LightSprites_Draw, gData_020035fe,
                            gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_02003600,
                            gLightSpriteTile, LightSprites_Draw, gData_020035fe,
                            gLightSpritesLarge, gLightSpritePriority)

extern const u8 gData_020031c4[];
extern s32 gData_02003594;

DEFINE_LIGHT_SPRITES_ADD_COUNTED(LightSprites_AddCounted, gData_02003600,
                                 gData_020035fe)

void Func_02001ee8(u16 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
