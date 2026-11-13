/// @file
/// Scene code of overlay 0x69f, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
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
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"
#include "main_routine_work.h"
#include "field.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_6.h"
#include "module/scene/scene.h"

void Whirlwind_Update(struct GustObject *object);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Import_ActorSelectAnimation,
                       Whirlwind_Update, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern struct Actor *gData_0200242c[];
/* VIEW(s0854): variable gData_0200252e as s16 gData_0200252e;
 * module/scene/scene.h has u16 gData_0200252e; tried as the rest of the object
 * declares it, LightSprites_Draw moves: 99 of 200 instructions,
 * LightSprites_InitA moves: 0 of 36 instructions, LightSprites_InitB moves: 0
 * of 36 instructions, 1 more in tla-ja. */
extern s16 gData_0200252e__s0854 __asm__("gData_0200252e");

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, Data_0200246c,
                          gData_0200242a, gData_0200242c, gLightSpritesLarge,
                          gData_0200252e__s0854, BattleEffect_GetObjectPriority,
                          Import_OamAddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gData_0200242c,
                            gLightSpriteTile, LightSprites_Draw, gData_0200242a,
                            gLightSpritesLarge, gData_0200252e__s0854)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gData_0200242c,
                            gLightSpriteTile, LightSprites_Draw, gData_0200242a,
                            gLightSpritesLarge, gData_0200252e__s0854)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gData_0200242c,
                            gLightSpriteTile, LightSprites_Draw, gData_0200242a,
                            gLightSpritesLarge, gData_0200252e__s0854)

extern s32 gData_020023c0;

s32 Teleporters_Step(s32 value);
void JupiterLighthouse_CloseDoor(void);
void JupiterLighthouse_LoadPalettes(s32 value);

DEFINE_LIGHT_SPRITES_ADD_COUNTED(LightSprites_AddCounted, gData_0200242c,
                                 gData_0200242a)

void Func_02000ed0(u16 value) { gData_0200252e__s0854 = value; }

#include "../../field/object/set_sprite_priority.c"
