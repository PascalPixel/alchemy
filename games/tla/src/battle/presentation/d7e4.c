#include "types.h"

s32 Func_0811d79c(s32 kind) {
  s32 value = kind;

  kind = 0;
  switch (value) {
  case 0:
  case 1:
  case 4:
  case 5:
  case 7:
    kind = 0x4001;
    break;
  case 2:
  case 3:
  case 6:
    kind = 0x4004;
    break;
  }
  return kind;
}

void Func_0811d7e4(void) {}
