#include "types.h"

extern u8 gData_02002368[];
extern u8 gData_02002378[];

void Func_02000054(const u8 *data, s32 kind, s32 a);

void Func_020000c8(s32 kind) {
  const u8 *data = gData_02002368;

  if (kind == 3)
    data += 8;
  if (kind == 16)
    data = gData_02002378;
  Func_02000054(data, kind, 0);
}
