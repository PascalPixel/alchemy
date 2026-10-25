#include "types.h"
#include "object_runtime.h"
#include "call.h"

extern s32 gData_02004808;

s32 GameFlag_Test(s32 flag);
void Func_02003fe0(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
s32 Func_020021f0(struct ObjectRuntime *object);

void Func_02002208(struct ObjectRuntime *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
  case 0:
    Call4((void (*)())Func_02003fe0, (s32)object, 140 << 17, 0, 200 << 16);
    *state = 1;
    break;
  case 1:
    if (Func_020021f0(object))
      *state = 2;
    break;
  case 2:
    Call4((void (*)())Func_02003fe0, (s32)object, 140 << 17, 0, 132 << 17);
    *state = 3;
    break;
  case 3:
    if (Func_020021f0(object))
      *state = 4;
    break;
  case 4:
    Call4((void (*)())Func_02003fe0, (s32)object, 184 << 16, 0, 132 << 17);
    *state = 5;
    break;
  case 5:
    if (Func_020021f0(object))
      *state = 6;
    break;
  case 6:
    Call4((void (*)())Func_02003fe0, (s32)object, 184 << 16, 0, 248 << 16);
    *state = 7;
    break;
  case 7:
    if (Func_020021f0(object)) {
      if (GameFlag_Test(0x203)) {
        object->update = NULL;
        gData_02004808 = 200;
      } else {
        *state = 8;
      }
    }
    break;
  case 8:
    Call4((void (*)())Func_02003fe0, (s32)object, 184 << 16, 0, 200 << 16);
    *state = 9;
    break;
  case 9:
    if (Func_020021f0(object))
      *state = 0;
    break;
  }
}
