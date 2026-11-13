/// @file
/// The scene code of overlay 0x664, in ROM order.

#include "shared/map_cells.h"
#include "shared/actors.h"
#include "airs_rock_inside.h"
#include "system.h"

#include "actor.h"
#include "module/scene/airs_rock_inside.h"
#include "module/scene/scene.h"

void Func_020006cc(struct Actor *object);

void Func_02000038(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    s32 id = (u16)entry[0];
    struct Actor *object = Actor_GetById(id);
    s32 kind = entry[1];
    s32 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02000128(id, (u16)kind, (u16)flag, BYTE_AT(object, 34));
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}

#include "types.h"
#include "ram_buffer.h"

// Finds the current target's table row and shrinks it.
void Func_020000a4(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
    s32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02000128(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  }
}

#include "field_event.h"

#include "map.h"

DEFINE_ACTOR_SET_CELL_OPEN(Func_02000128, Func_02005f64, Func_02005d5c)

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_IMPORTED_ACTOR(Func_02000268, 0x22)
