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

extern const s32 gData_02002234[];

s32 Func_02000648(s32 *from, s32 *to);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Func_02000674, Func_02001bbc, Func_02000648,
    gData_02002234, Data_02002254)

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_02000f3c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Func_02000834, Func_02001a94, Func_02000674, Func_02000f3c)

DEFINE_ACTOR_EMIT_RISING_SPARKS_B(Func_0200090c, Func_02000f3c)

void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(Func_020009ac)

extern struct LightOam gData_020022dc[];

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(Func_02000b3c, gData_02002298, gData_020022dc,
    gData_0200229a, gData_0200229c, gData_0200239c, gData_0200239e,
    Func_02001bb4, Func_02001a64)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(Func_02000cf8, gData_0200229c, gData_02002298,
    Func_02000b3c, gData_0200229a, gData_0200239c, gData_0200239e)

DEFINE_LIGHT_SPRITES_INIT_B(Func_02000d78, gData_0200229c, gData_02002298,
    Func_02000b3c, gData_0200229a, gData_0200239c, gData_0200239e)

DEFINE_LIGHT_SPRITES_INIT_C(Func_02000df8, gData_0200229c, gData_02002298,
    Func_02000b3c, gData_0200229a, gData_0200239c, gData_0200239e)

DEFINE_LIGHT_SPRITES_ADD(Func_02000e80, gData_0200229a, gData_0200229c)

void Func_02000eb0(s32 value) { gData_0200239e = value; }

#include "../../field/object/set_sprite_priority.c"
