#include "magma_rock.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "object_runtime.h"

extern s16 gData_02003d9e[];
extern s16 gData_02003db2[];
extern s16 gData_02003dc4[];
extern s16 gData_02003dd6[];
extern s16 gData_02003dee[];
extern s16 gData_02003e02[];

s32 Func_02001084(s16 *table);
void Func_02000fcc(struct ObjectRuntime *object);

/// Picks the cell table for the room and entrance, finds the target in it and,
/// in modes 2 and 3, sets the target's byte 99 to 4 or 2 and makes it blink
/// with Func_02000fcc.
void Func_02001110(s32 mode) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s16 *table = gData_02003d9e;
  struct PartyState *party = &gPartyState;

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
    struct ObjectRuntime *target;

    Func_02001084(table);
    target = work->target;
    ((u8 *)target)[99] = 4;
    target->update = (void (*)(void))Func_02000fcc;
  }
  if (mode == 3) {
    struct ObjectRuntime *target;

    Func_02001084(table);
    target = work->target;
    ((u8 *)target)[99] = 2;
    target->update = (void (*)(void))Func_02000fcc;
  }
}
