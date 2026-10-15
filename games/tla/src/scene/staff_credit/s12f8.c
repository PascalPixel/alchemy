#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_0200209c(void);
void Func_02001274(struct ObjectRuntime *object, s32 mode);

void Func_020012f8(void) { Func_02001274(Object_GetById(Func_0200209c()), 0); }
