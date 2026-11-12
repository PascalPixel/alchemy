#include "types.h"
#include "magma_rock.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "actor.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);

s32 Func_020010cc(s32 x, s32 z, u16 *table) {
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, x >> 20, z >> 20) >> 8;

  while (*table != 0xffff) {
    table++;
    if (code == *table) {
      result = table[1];
      break;
    }
    table++;
    result = *table++;
  }
  return result;
}

extern s16 gData_02003d9e[];
extern s16 gData_02003db2[];
extern s16 gData_02003dc4[];
extern s16 gData_02003dd6[];
extern s16 gData_02003dee[];
extern s16 gData_02003e02[];

s32 Func_02001084(s16 *table);
void Func_02000fcc(struct Actor *object);

/// Picks the cell table for the room and entrance, finds the target in it and,
/// in modes 2 and 3, sets the target's byte 99 to 4 or 2 and makes it blink
/// with Func_02000fcc.
void Func_02001110(s32 mode) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s16 *table = gData_02003d9e;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_Ov691Room2) {
    if (party->entrance <= 3)
      table = gData_02003db2;
    else if (party->entrance <= 8)
      table = gData_02003dc4;
    else if (party->entrance <= 12)
      table = gData_02003dd6;
    else
      table = gData_02003dee;
  } else if (party->scene == (s32)&SceneId_Ov691Room3) {
    table = gData_02003e02;
  }
  if (mode == 2) {
    struct Actor *target;

    Func_02001084(table);
    target = work->target;
    ((u8 *)target)[99] = 4;
    target->update = (void (*)(void))Func_02000fcc;
  }
  if (mode == 3) {
    struct Actor *target;

    Func_02001084(table);
    target = work->target;
    ((u8 *)target)[99] = 2;
    target->update = (void (*)(void))Func_02000fcc;
  }
}

/// Adds the velocity to the position and decays it by one eighth each
/// update.
void Func_020011b8(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce >> 3;
  object->gravity -= object->gravity >> 3;
  object->unk4c -= object->unk4c >> 3;
}
