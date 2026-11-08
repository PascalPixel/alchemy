#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Func_020017bc(s32 id);

/// Sets up actors 19 to 33: their modes, Func_020017bc for 19 to 26, and child
/// and collision flags for 27 to 33.
void Func_020017f4(void) {
  Actor_SetModeById(19, 0);
  Actor_SetModeById(20, 1);
  Actor_SetModeById(21, 2);
  Actor_SetModeById(22, 0);
  Actor_SetModeById(23, 1);
  Actor_SetModeById(24, 2);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(26, 4);
  Func_020017bc(19);
  Func_020017bc(20);
  Func_020017bc(21);
  Func_020017bc(22);
  Func_020017bc(23);
  Func_020017bc(24);
  Func_020017bc(25);
  Func_020017bc(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_SetSingleChildFlags(Actor_GetById(28), 0);
  Actor_SetSingleChildFlags(Actor_GetById(29), 0);
  Actor_SetSingleChildFlags(Actor_GetById(30), 0);
  Actor_SetSingleChildFlags(Actor_GetById(31), 0);
  Actor_SetSingleChildFlags(Actor_GetById(32), 0);
  Actor_SetSingleChildFlags(Actor_GetById(33), 0);
  Actor_GetById(27)->collisionFlags |= 8;
  Actor_GetById(28)->collisionFlags |= 8;
  Actor_GetById(29)->collisionFlags |= 8;
  Actor_GetById(30)->collisionFlags |= 8;
  Actor_GetById(31)->collisionFlags |= 8;
  Actor_GetById(32)->collisionFlags |= 8;
  Actor_GetById(33)->collisionFlags |= 8;
  Actor_SetModeById(27, 0);
  Actor_SetModeById(32, 0);
  Actor_SetModeById(33, 0);
}
