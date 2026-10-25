#include "local.h"

void Func_0200255c(void) {
  s32 none = -1;
  s32 priority;

  gData_02006710 = none;
  priority = 0x480;
  Scheduler_AddOrUpdateCallback(Func_020024ac, priority);
}

// The cell stampers keep their last cell and the block they copy.
extern s32 Data_020067ec;
extern s32 Data_020067f0;
extern s32 Data_020067f4;
extern s32 Data_020067f8;

s32 Func_02004f00(void);
void Func_02004cf0(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  Func_02004cf0(srcX, srcZ, width, height, destX, destZ);
}

/// Saves the 1 by 2 cells 64 columns across from the controlled object at
/// (73, 72) and stamps the cells kept at (70, 72) in their place, once per
/// cell; Func_02003d40 of Air's Rock is the same with another source.
void Func_0200257c(void) {
  struct ObjectRuntime *object = Object_GetById(Func_02004f00());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006710) {
    gData_02006710 = key;
    Data_020067ec = 1;
    Data_020067f0 = 2;
    Data_020067f4 = x + 64;
    Data_020067f8 = z - y;
    /* FAKEMATCH(wrapper): both copies through the forwarding inline
     * CopyCells, which puts every argument in a register before the stack
     * arguments are stored; called directly, 12 of 59 instructions differ in
     * tla-en. */
    CopyCells(Data_020067f4, Data_020067f8, 1, 2, 73, 72);
    CopyCells(70, 72, Data_020067ec, Data_020067f0, Data_020067f4,
              Data_020067f8);
  }
}

/// As Func_0200257c, stamping the cells kept at (71, 72).
void Func_02002610(void) {
  struct ObjectRuntime *object = Object_GetById(Func_02004f00());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006710) {
    gData_02006710 = key;
    Data_020067ec = 1;
    Data_020067f0 = 2;
    Data_020067f4 = x + 64;
    Data_020067f8 = z - y;
    /* FAKEMATCH(wrapper): as in Func_0200257c; called directly, 12 of 59
     * instructions differ in tla-en. */
    CopyCells(Data_020067f4, Data_020067f8, 1, 2, 73, 72);
    CopyCells(71, 72, Data_020067ec, Data_020067f0, Data_020067f4,
              Data_020067f8);
  }
}
