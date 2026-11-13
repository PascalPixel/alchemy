/// @file
/// Scene code of overlay 0x6a0, in ROM order, from s16d4's offset up to the next object the overlay links.

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
#include "field_event.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_7.h"
#include "module/scene/scene.h"

void Func_02002ad4(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Func_02001514(struct GustObject *object);
void Func_02001f74(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_020007E8(Func_020016d4, Func_02002ad4, Func_02001514, Func_02001f74)

DEFINE_FUNC_020008C0(Func_020017ac, Func_02001f74)

DEFINE_FUNC_0200091C(Func_0200184c)

DEFINE_FUNC_020019DC(Func_020019dc, Func_02001acc)

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
        Func_02001acc(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

DEFINE_FUNC_02000128__S00A4(Func_02001acc, Func_02002c1c, Func_02002ad4)

extern u32 gData_02003600[];
extern s16 gData_02003702;

s32 Func_02002c2c(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_FUNC_02000AAC(Func_02001b74, gData_020035fc, Data_02003640,
    gData_020035fe, gData_02003600, gData_02003700, gData_02003702,
    Func_02002c2c, Func_02002aa4)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02001d30, gData_02003600, gData_020035fc,
    Func_02001b74, gData_020035fe, gData_02003700, gData_02003702)

DEFINE_FUNC_02000CE8(Func_02001db0, gData_02003600, gData_020035fc,
    Func_02001b74, gData_020035fe, gData_02003700, gData_02003702)

DEFINE_FUNC_02000D68(Func_02001e30, gData_02003600, gData_020035fc,
    Func_02001b74, gData_020035fe, gData_02003700, gData_02003702)

extern const u8 gData_020031c4[];
extern s32 gData_02003594;

DEFINE_FUNC_02000EA0(Func_02001eb8, gData_02003600, gData_020035fe)

void Func_02001ee8(u16 value) { gData_02003702 = value; }

#include "../../field/object/set_sprite_priority.c"
