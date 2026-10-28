#include "ride.h"

void Func_02004d38(s32 a0, s32 a1);

s32 Func_02000038(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

void Func_02000044(void) { Func_02004d38(11, 57); }

void Func_02000050(void) { Func_02004d38(17, 36); }
