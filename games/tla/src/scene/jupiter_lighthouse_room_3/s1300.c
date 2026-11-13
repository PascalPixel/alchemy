/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/routines.h"
#include "../shared/jupiter_lighthouse_room_3.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "iwram_call.h"
#include "task.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"


struct Actor *Func_02002438(s32 *position, s32 mode);

/// Stops the object where it stands (Actor_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Func_02002438) whose sprite shape is not in the footprint table, and again
/// when its own cell test (Func_02002338) is positive.
void Func_02001300(struct Actor *object) {
  s32 position[3];
  struct Actor *other;
  u32 i;
  s32 angle;

  angle = object->facing >> 12;
  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  other = Func_02002438(position, 1);
  if (other != NULL) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)other->sprite)->value28 ==
        gData_02002478[i])
      return;
    i++;
    if (i < 6)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02002338(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

extern s16 gData_02002d2c;

void Func_020022b8(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_FUNC_02000AAC(Func_02001388, gData_02002d2c, Data_02002d70,
    gData_02002d2e, gData_02002d30, gData_02002e30, gData_02002e32,
    Func_02002448, Func_020022b8)


// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02001544, gData_02002d30, gData_02002d2c,
    Func_02001388, gData_02002d2e, gData_02002e30, gData_02002e32)

DEFINE_FUNC_02000CE8(Func_020015c4, gData_02002d30, gData_02002d2c,
    Func_02001388, gData_02002d2e, gData_02002e30, gData_02002e32)

DEFINE_FUNC_02000D68(Func_02001644, gData_02002d30, gData_02002d2c,
    Func_02001388, gData_02002d2e, gData_02002e30, gData_02002e32)

DEFINE_FUNC_02000DF0(Func_020016cc, gData_02002d2e, gData_02002d30)

void Func_020016fc(s32 value) { gData_02002e32 = value; }

#include "../../field/object/set_sprite_priority.c"
