#include "types.h"

#include "object_runtime.h"

extern const s8 Data_02005940[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_0200561c(void *animation, s32 a);
void Func_02005624(struct ObjectRuntime *object, s32 mode);

void Func_020020b0(s32 id, s32 direction, s32 value)
{
    struct ObjectRuntime *object = Object_GetById(id);
    void *animation = object->animation;

    Func_02005624(object, Data_02005940[(direction + 1) & 3]);
    object->unknown_5c[0] = 2;
    Func_0200561c(animation, value);
}
