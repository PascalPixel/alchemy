// Draft, differ 76 of 76 (all register choice): the listing tests the count
// once with ldrsh before the loop, then loads it again unextended (ldrh r2,
// [r6, #10]) and sign-extends it on every pass (lsls, asrs); here the extended
// count is kept in ip, which moves the route to r7 and the index to r6. A
// signed field and an unsigned field with a signed compare give the same code.
#include "../../../../games/tla/src/scene/mars_star/mars_star.h"

struct MarsStarStop {
  s16 column;
  s16 row;
  s16 flag;
  s16 unk06;
};

struct MarsStarRoute {
  s16 unk00;
  s16 flagBase;
  s16 unk04;
  s16 column;
  s16 row;
  u16 count;
  s16 current;
  u8 unk0e[18];
  struct MarsStarStop stops[1];
};

extern struct MarsStarRoute gData_02004cf4;

void GameFlag_SetByte(s32 flag, s32 value);
s32 GameFlag_GetByte(s32 flag);
void Func_02003bfc(struct MarsStarRoute *route, struct MarsStarStop *stop);
void Func_02003c6c(struct MarsStarRoute *route, struct MarsStarStop *stop);

s32 Func_02003f9c(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  struct MarsStarRoute *route = &gData_02004cf4;
  struct MarsStarStop *stop = route->stops;
  s32 i;

  for (i = 0; i < (s16)route->count; i++, stop++) {
    if (stop->column == column && stop->row == row) {
      if (GameFlag_Test(stop->flag) == 0) {
        Func_02003bfc(route, stop);
        GameFlag_SetBit(stop->flag);
        route->current = i;
        break;
      }
      if (i == route->current)
        break;
      route->current = i;
      Func_02003c6c(route, stop);
      GameFlag_SetByte(route->flagBase, column);
      GameFlag_SetByte(route->flagBase + 8, row);
      return 1;
    }
  }
  return 0;
}
