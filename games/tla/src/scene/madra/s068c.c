#include "types.h"
#include "call.h"

void Func_02000670(void);
void Func_02002af8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_0200068c(void) {
  Call6((void (*)())Func_02002af8, 51, 10, 3, 3, 45, 10);
  Func_02000670();
}
