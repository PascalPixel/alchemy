/// @file
/// Scene code of overlay 0x6a3, in ROM order, from s3474's offset up to the next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "call.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "field_coordinate.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_m.h"
#include "module/scene/scene.h"

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_020032b4(struct GustObject *object);
void Func_02003b7c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_020007A4(Func_02003474, Func_020046f4, Func_020032b4, Func_02003b7c)

DEFINE_FUNC_020008C0(Func_0200354c, Func_02003b7c)

DEFINE_FUNC_0200091C(Func_020035ec)

extern s16 gData_02005b84;

void Func_020046ac(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_FUNC_02000AAC(Func_0200377c, gData_02005a80, Data_02005ac4,
    gData_02005a82, gData_02005a84, gData_02005b84, gData_02005b86,
    Func_0200499c, Func_020046ac)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02003938, gData_02005a84, gData_02005a80,
    Func_0200377c, gData_02005a82, gData_02005b84, gData_02005b86)

DEFINE_FUNC_02000CE8(Func_020039b8, gData_02005a84, gData_02005a80,
    Func_0200377c, gData_02005a82, gData_02005b84, gData_02005b86)

DEFINE_FUNC_02000D68(Func_02003a38, gData_02005a84, gData_02005a80,
    Func_0200377c, gData_02005a82, gData_02005b84, gData_02005b86)

DEFINE_FUNC_02000DF0(Func_02003ac0, gData_02005a82, gData_02005a84)

void Func_02003af0(s32 value) { gData_02005b86 = value; }

#include "../../field/object/set_sprite_priority.c"
