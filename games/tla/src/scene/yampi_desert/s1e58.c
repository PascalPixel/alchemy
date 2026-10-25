#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"
#include "party_state.h"

extern s32 gInput;

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
s32 Func_020041e0(struct ObjectRuntime *object);
void Func_02001cc4(struct ObjectRuntime *object, s32 dx, s32 dz);

void Func_02001e58(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 speed = 0x18000;
  struct MapWork *map;
  struct MapCell *here, *ahead, *under;
  s32 index, column, row;
  struct MapCell *cells;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0) {
    ObjectDispatch_SetAnimationSpeed(leader, 0);
    Map_EnableUpdateCallback();
    return;
  }
  map = Ram_HeapSlots->mapWork;
  index = Func_020041e0(leader);
  column = leader->x / 0x100000;
  row = (leader->z + 0x40000) / 0x100000;
  cells = map->layers[0].cells;
  here = cells + index;
  ahead = cells + (column + row * 128);
  under = map->layers[1].cells + index;
  ObjectDispatch_SetAnimationSpeed(leader, 16);
  Map_DisableUpdateCallback();
  if ((u8)(under->collisionCode - 10) <= 4) {
    if (gPartyState.renderMode == 5)
      speed = 0;
    else
      ObjectDispatch_SetSingleChildFlags(
          Object_GetById(gPartyState.currentOwner), 1);
  }
  if (ahead->collisionCode == 255)
    speed = 0;
  switch (here->collisionCode) {
  case 10:
    if (gInput & 64) {
      Func_02001cc4(leader, 0, -speed >> 2);
      return;
    }
    break;
  case 11:
    if (gInput & 128) {
      Func_02001cc4(leader, 0, speed >> 2);
      return;
    }
    break;
  case 12:
    if (gInput & 32) {
      Func_02001cc4(leader, -speed >> 2, 0);
      return;
    }
    break;
  case 13:
    if (gInput & 16) {
      Func_02001cc4(leader, speed >> 2, 0);
      return;
    }
    break;
  }
  switch (under->collisionCode) {
  case 10:
    Func_02001cc4(leader, 0, -speed);
    break;
  case 11:
    Func_02001cc4(leader, 0, speed);
    break;
  case 12:
    Func_02001cc4(leader, -speed, 0);
    break;
  case 13:
    Func_02001cc4(leader, speed, 0);
    break;
  }
}
