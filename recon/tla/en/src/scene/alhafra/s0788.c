#include "types.h"

extern const u8 gData_020027f4[];
extern const u8 gData_02002ad0[];
extern const u8 gData_02002e48[];
extern const u8 gData_02003154[];

s32 GameFlag_Test(s32 flag);

const u8 *Func_02000788(void) {
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x7a8, 255)))) {
    if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x810, 255))) == 0)
      return gData_02003154;
    return gData_02002e48;
  }
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x800, 171))))
    return gData_02002ad0;
  return gData_020027f4;
}
