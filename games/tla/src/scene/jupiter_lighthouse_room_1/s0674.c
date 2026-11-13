/// @file
/// Scene code of overlay 0x69a, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
#include "../shared/effects.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "irq.h"
#include "event_work.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "call.h"
#include "field_effect.h"
#include "frame.h"
#include "field_event.h"
#include "field_coordinate.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_1.h"
#include "module/scene/scene.h"

extern const s32 gWhirlwindEndScript[];

s32 Position_FlatDistance(s32 *from, s32 *to);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, Func_02001bbc, Position_FlatDistance,
    gWhirlwindEndScript, gWhirlwindRiding)

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_02000f3c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Func_02001a94, Whirlwind_UpdateLower, Func_02000f3c)

DEFINE_ACTOR_EMIT_RISING_SPARKS_B(Actor_EmitRisingSparksB, Func_02000f3c)

void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(JupiterLighthouse_PlaceRaisedActors)

extern struct LightOam gData_020022dc[];

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gData_020022dc,
    gLightSpriteCount, gData_0200229c, gLightSpritesLarge, gLightSpritePriority,
    Func_02001bb4, Func_02001a64)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_0200229c, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_0200229c, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_0200229c, gLightSpriteTile,
    LightSprites_Draw, gLightSpriteCount, gLightSpritesLarge, gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount, gData_0200229c)

void Func_02000eb0(s32 value) { gLightSpritePriority = value; }

#include "../../field/object/set_sprite_priority.c"
