/// @file
/// Scene code of overlay 0x69b, in ROM order, from s0d94's offset up to the
/// next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
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
#include "../jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "field_coordinate.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_2.h"
#include "module/scene/scene.h"

extern const s32 gData_02002b3c[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_FUNC_020005E4(Func_02000d94, Func_020022fc, Func_02000d68,
    gData_02002b3c, Data_02002b94)

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_0200165c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_020007E8(Func_02000f54, Func_020021b4, Func_02000d94, Func_0200165c)

DEFINE_FUNC_020008C0(Func_0200102c, Func_0200165c)

DEFINE_FUNC_0200091C(Func_020010cc)

extern s16 gData_02002cdc;

void Func_0200218c(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_FUNC_02000AAC(Func_0200125c, gData_02002bd8, Data_02002c1c,
    gData_02002bda, gData_02002bdc, gData_02002cdc, gData_02002cde,
    Func_020022f4, Func_0200218c)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02001418, gData_02002bdc, gData_02002bd8,
    Func_0200125c, gData_02002bda, gData_02002cdc, gData_02002cde)

DEFINE_FUNC_02000CE8(Func_02001498, gData_02002bdc, gData_02002bd8,
    Func_0200125c, gData_02002bda, gData_02002cdc, gData_02002cde)

DEFINE_FUNC_02000D68(Func_02001518, gData_02002bdc, gData_02002bd8,
    Func_0200125c, gData_02002bda, gData_02002cdc, gData_02002cde)

void Func_020015a0(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object) {
    object->timer = value;
    {
      s16 *count;

      count = &gData_02002bda;
      gData_02002bdc[*count] = object;
      (*count)++;
    }
  }
}

void Func_020015d0(s32 a0) { gData_02002cde = a0; }

#include "../../field/object/set_sprite_priority.c"
