#include "types.h"

s32 ObjectDispatch_SetSingleChildFlags();

s32 Func_02000038(s32 object) {
  ObjectDispatch_SetSingleChildFlags(object, 0);
  return 0;
}
