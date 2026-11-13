/// @file
/// Scene code of overlay 0x69f, in ROM order.

#include "../shared/routines.h"
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

void Func_02000694(struct GustObject *object);
void Func_02000f5c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_020007E8(Func_02000854, Func_02001ab4, Func_02000694, Func_02000f5c)

DEFINE_FUNC_020008C0(Func_0200092c, Func_02000f5c)

DEFINE_FUNC_0200091C(Func_020009cc)

extern struct Actor *gData_0200242c[];
/* VIEW(s0854): variable gData_0200252e as s16 gData_0200252e; module/scene/scene.h has u16 gData_0200252e; tried as the rest of the object declares it, Func_02000b5c moves: 99 of 200 instructions, Func_02000d18 moves: 0 of 36 instructions, Func_02000d98 moves: 0 of 36 instructions, 1 more in tla-ja. */
extern s16 gData_0200252e__s0854 __asm__("gData_0200252e");

void Func_02001a84(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_FUNC_02000AAC(Func_02000b5c, gData_02002428, Data_0200246c,
    gData_0200242a, gData_0200242c, gData_0200252c, gData_0200252e__s0854,
    Func_02001bd4, Func_02001a84)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02000d18, gData_0200242c, gData_02002428,
    Func_02000b5c, gData_0200242a, gData_0200252c, gData_0200252e__s0854)

DEFINE_FUNC_02000CE8(Func_02000d98, gData_0200242c, gData_02002428,
    Func_02000b5c, gData_0200242a, gData_0200252c, gData_0200252e__s0854)

DEFINE_FUNC_02000D68(Func_02000e18, gData_0200242c, gData_02002428,
    Func_02000b5c, gData_0200242a, gData_0200252c, gData_0200252e__s0854)

extern s32 gData_020023c0;

s32 Func_020018bc(s32 value);
void Func_02000108(void);
void Func_020003a4(s32 value);

DEFINE_FUNC_02000EA0(Func_02000ea0, gData_0200242c, gData_0200242a)

void Func_02000ed0(u16 value) { gData_0200252e__s0854 = value; }

#include "../../field/object/set_sprite_priority.c"
