#include "types.h"

void Func_0813d098(void *object, s32 mode);

void Func_0813cf80(void *object) { Func_0813d098(object, 6); }

void Func_0813cf8c(void *object) { Func_0813d098(object, 3); }

void Func_0813cf98(void *object) { Func_0813d098(object, 1); }

void Func_0813cfa4(void *object) { Func_0813d098(object, 10); }

void Func_0813cfb0(void *object) { Func_0813d098(object, 5); }

void Func_0813cfbc(void *object) { Func_0813d098(object, 9); }

void Func_0813cfc8(void *object) { Func_0813d098(object, 4); }

void Func_0813cfd4(void *object) { Func_0813d098(object, 0); }

void Func_0813cfe0(void *object) { Func_0813d098(object, 8); }

void Func_0813cfec(void *object) { Func_0813d098(object, 7); }

void Func_0813cff8(void *object) { Func_0813d098(object, 2); }

void Func_0813d004(void *object) { Func_0813d098(object, 11); }

void Func_0813d010(void *object) { Func_0813d098(object, 16); }

void Func_0813d01c(void *object) { Func_0813d098(object, 14); }

void Func_0813d028(void *object) { Func_0813d098(object, 15); }

void Func_0813d034(void *object) { Func_0813d098(object, 17); }

void Func_0813d040(void *object) { Func_0813d098(object, 18); }

void Func_0813d04c(void *object) { Func_0813d098(object, 19); }

void Func_0813d058(void *object) { Func_0813d098(object, 20); }

void Func_08157530(s32 x, s32 y, s32 width, s32 height);
void WaitFrames(s32 frames);

void Func_0813d064(u8 *object) {
  Func_08157530(*(s32 *)(object + 8), *(s16 *)(object + 36), 4, 20);
  WaitFrames(2);
  if (*(s32 *)(object + 24) == 0)
    Func_0813d098(object, 12);
  else
    Func_0813d098(object, 13);
}
