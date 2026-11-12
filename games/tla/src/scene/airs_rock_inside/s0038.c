/// @file
/// Scene code of overlay 0x664, in ROM order, from s0038's offset up to the next object the overlay links.

#include "airs_rock_inside.h"
#include "system.h"

#include "airs_rock_inside.h"
#include "actor.h"

s32 Func_02005f64(struct FieldActor *object);
void Func_020006cc(struct FieldActor *object);
void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02000038(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    s32 id = (u16)entry[0];
    struct FieldActor *object = Actor_GetById(id);
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

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

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

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02005f64(object);
  if (GameFlag_Test(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02005d5c(object, 2);
      break;
    case 1:
      Func_02005d5c(object, 4);
      break;
    case 2:
      Func_02005d5c(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02005d5c(object, 1);
      break;
    case 1:
      Func_02005d5c(object, 3);
      break;
    case 2:
      Func_02005d5c(object, 5);
      break;
    }
  }
}

#include "../../field/object/set_sprite_priority.c"
