// Draft, differ 106 of 241: opens the passages on each side of the hall by how
// many of its two pairs of flags (0x9bc and 0x9bd, 0x9be and 0x9bf) are set:
// both open the wide way, one a side way, and a single side opened alone also
// opens its connecting cells. This is the English layout; Japanese differs.
#include "types.h"
#include "lib/call.h"

s32 GameFlag_Test(s32 flag);
void Func_02004e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02002d30(void) {
  s8 first = GameFlag_Test(0x9bc);
  u8 third = GameFlag_Test(0x9be);
  s8 left = GameFlag_Test(0x9bd) + first;
  s8 right = (s8)third + GameFlag_Test(0x9bf);

  if (left != 0) {
    if (left == 2) {
      Call6((void (*)())Func_02004e04, 74, 115, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        Call6((void (*)())Func_02004e04, 71, 110, 1, 3, 30, 85);
      else if ((s8)third != 0)
        Call6((void (*)())Func_02004e04, 67, 110, 1, 3, 30, 85);
      else
        Call6((void (*)())Func_02004e04, 79, 110, 1, 3, 30, 85);
    } else if (first != 0) {
      Call6((void (*)())Func_02004e04, 66, 95, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        Call6((void (*)())Func_02004e04, 81, 110, 1, 3, 30, 85);
      else if ((s8)third != 0)
        Call6((void (*)())Func_02004e04, 75, 110, 1, 3, 30, 85);
      else
        Call6((void (*)())Func_02004e04, 65, 110, 1, 3, 30, 85);
    } else {
      Call6((void (*)())Func_02004e04, 69, 82, 5, 12, 26, 76);
      if (right == 0)
        return;
      if (right == 2)
        Call6((void (*)())Func_02004e04, 69, 110, 1, 3, 30, 85);
      else if ((s8)third != 0)
        Call6((void (*)())Func_02004e04, 73, 110, 1, 3, 30, 85);
      else
        Call6((void (*)())Func_02004e04, 77, 110, 1, 3, 30, 85);
    }
  }
  if (right == 0)
    return;
  if (right == 2) {
    Call6((void (*)())Func_02004e04, 83, 115, 6, 12, 31, 76);
    if (left == 0)
      Call6((void (*)())Func_02004e04, 82, 124, 1, 3, 30, 85);
  } else if ((s8)third != 0) {
    Call6((void (*)())Func_02004e04, 76, 82, 4, 12, 31, 76);
    if (left == 0)
      Call6((void (*)())Func_02004e04, 75, 91, 1, 3, 30, 85);
  } else {
    Call6((void (*)())Func_02004e04, 82, 82, 6, 12, 31, 76);
    if (left == 0)
      Call6((void (*)())Func_02004e04, 81, 91, 1, 3, 30, 85);
  }
}
