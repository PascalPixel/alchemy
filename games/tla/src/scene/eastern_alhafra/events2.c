#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

struct ObjectRuntime *Object_GetById();
void Motion_CamBounds();
void Object_AttachWorkTargetToObject();
void Func_02000190(s32, s32);
void Func_02000200(s32, s32);
s32 Func_02004c7c();

void Func_02000394(s32 unused, s32 id) {
  Func_02000190(unused, id);
  Value4(Func_02004c7c, 0x1400000, 0x1d00000, 2, 255);
  Value4(Func_02004c7c, 0x1400000, 0x1e00000, 2, 255);
  Value4(Func_02004c7c, 0x1400000, 0x1f00000, 2, 255);
  Value4(Func_02004c7c, 0x1400000, 0x2000000, 2, 255);
  Value4(Func_02004c7c, 0x1600000, 0x1d00000, 2, 255);
  Value4(Func_02004c7c, 0x1600000, 0x1e00000, 2, 255);
  Value4(Func_02004c7c, 0x1600000, 0x1f00000, 2, 255);
  Value4(Func_02004c7c, 0x1600000, 0x2000000, 2, 255);
  Value4(Func_02004c7c, 0x1500000, 0x2100000, 2, 0);
}

void Func_0200042c(s32 mode, s32 id) {
  struct ObjectRuntime *object;
  s32 x;
  s32 z;
  s32 open;

  object = Object_GetById(id);
  x = object->x >> 20;
  z = object->z >> 20;
  open = 0;
  if (x == 21 && z == 33)
    open = 1;
  Value4(Func_02004c7c, 0x1400000, 0x1d00000, 2, 0);
  Value4(Func_02004c7c, 0x1400000, 0x1e00000, 2, 0);
  Value4(Func_02004c7c, 0x1400000, 0x1f00000, 2, 0);
  Value4(Func_02004c7c, 0x1400000, 0x2000000, 2, 0);
  Value4(Func_02004c7c, 0x1600000, 0x1d00000, 2, 0);
  Value4(Func_02004c7c, 0x1600000, 0x1e00000, 2, 0);
  Value4(Func_02004c7c, 0x1600000, 0x1f00000, 2, 0);
  Value4(Func_02004c7c, 0x1600000, 0x2000000, 2, 0);
  Value4(Func_02004c7c, 0x1500000, 0x2100000, 2, 255);
  if (open)
    Motion_CamBounds(0x1500000, -1, 0x2300000, 1);
  Func_02000200(mode, id);
  if (open)
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 1);
}
