// Draft, differ 22 of 139: what the leader faces in Jupiter Lighthouse room 7: the
// cell 8 pixels ahead (its facing rounded to a quarter turn) holding object 18
// or 19 on the ground gives the push (Func_02002c24); otherwise, unless that
// cell is one of the three spots (35, 15), (31, 16) or (33, 10) whose
// pair of flags (2577/2576, 2578/2579, 2580/2581) has one set, the plain
// check (Func_02002bec) runs.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);
void Func_02002bec(void);
void Func_02002c24(void);

void Func_02000cec(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *first = Object_GetById(18);
  struct ObjectRuntime *second = Object_GetById(19);
  s32 position[3];
  s32 found = 0;
  s32 x;
  s32 z;

  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffsetFar(0x80000, (leader->angle + 0x2000) & 0xc000,
                           position);
  x = position[0] >> 20;
  z = position[2] >> 20;
  if (first->y == 0 && x == first->x >> 20 && z == first->z >> 20)
    found = 1;
  if (second->y == 0 && x == second->x >> 20 && z == second->z >> 20)
    found = 1;
  if (found == 0) {
    if (x == 35 && z == 15 && (GameFlag_Test(2576) || GameFlag_Test(2577)))
      found = 2;
    if (x == 31 && z == 16 && (GameFlag_Test(2578) || GameFlag_Test(2579)))
      found = 2;
    if (x == 33 && z == 10 && (GameFlag_Test(2580) || GameFlag_Test(2581)))
      found = 2;
    if (found == 0) {
      Func_02002bec();
      return;
    }
  }
  if (found == 1)
    Func_02002c24();
}
