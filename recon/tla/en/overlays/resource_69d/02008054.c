// differ 2: the first Func_02001ef0 call builds 0x1300000 with its shift
// last in native (after r1-r3); here the shift follows the movs. Pixel
// units, swapped helper parameters and a prototype did not move it.
#include "types.h"

void Func_02001e00();
void Func_02001df8();
void Func_02001ef0(s32 x, s32 y, s32 z, s32 w);
void Func_02001e70();
s32 GameFlag_Test();
void Func_02000724();

static inline void MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Func_02001e00(a, b, c, d, e, f);
}

static inline void MapCopyAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Func_02001df8(a, b, c, d, e, f);
}

static inline void ObjPut(s32 a, s32 b, s32 c, s32 d) {
  Func_02001ef0(a, b, c, d);
}

void Func_02000054(void) {
  MapCopy(17, 10, 1, 4, 19, 10);
  MapCopyAttr(17, 10, 1, 1, 19, 10);
  ObjPut(0x1300000, 0xa00000, 0, 254);
  Func_02001e70(14, 0x1f80000, 0xa80000);
  if (GameFlag_Test(0xa00)) {
    Func_02000724(14);
    ObjPut(0x1f00000, 0xa00000, 0, 254);
  }
}
