/// @file
/// Scene code of overlay 0x6a2, in ROM order.

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
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_9.h"
#include "module/scene/scene.h"

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_02000750(struct GustObject *object);
void Func_02001018(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN_UPPER(Func_02000910, Func_02001b68, Func_02000750, Func_02001018)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Func_020009e8, Func_02001018)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(Func_02000a88)

/* VIEW(s0910): variable gData_0200265e as s16 gData_0200265e; module/scene/scene.h has u8 gData_0200265e[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_9/s0910.c:229: `gData_0200265e' has an incomplete type. */
extern s16 gData_0200265e__s0910 __asm__("gData_0200265e");
extern struct LightOam gData_020026a0[];
extern s16 gData_02002760;
extern s16 gData_02002762;

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(Func_02000c18, gData_0200265c, gData_020026a0,
    gData_0200265e__s0910, gData_02002660, gData_02002760, gData_02002762,
    Func_02001c80, Func_02001b40)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(Func_02000dd4, gData_02002660, gData_0200265c,
    Func_02000c18, gData_0200265e__s0910, gData_02002760, gData_02002762)

DEFINE_LIGHT_SPRITES_INIT_B(Func_02000e54, gData_02002660, gData_0200265c,
    Func_02000c18, gData_0200265e__s0910, gData_02002760, gData_02002762)

DEFINE_LIGHT_SPRITES_INIT_C(Func_02000ed4, gData_02002660, gData_0200265c,
    Func_02000c18, gData_0200265e__s0910, gData_02002760, gData_02002762)

DEFINE_LIGHT_SPRITES_ADD(Func_02000f5c, gData_0200265e__s0910, gData_02002660)

void Func_02000f8c(s32 value) { gData_02002762 = value; }

#include "../../field/object/set_sprite_priority.c"
