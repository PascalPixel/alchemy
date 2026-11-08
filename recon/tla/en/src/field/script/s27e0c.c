#include "types.h"

s32 Field_CheckConfiguredKeys(void);

s32 Func_08027e0c(u16 *state) {
  Field_CheckConfiguredKeys();
  state[2]++;
  return 1;
}
