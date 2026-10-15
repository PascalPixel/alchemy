#include "types.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02003b38(struct ObjectRuntime *object);
void Func_02003b70(struct ObjectRuntime *object);
void Func_02004e30(s32 value);

void Func_02003bb8(struct ObjectRuntime *object, s32 mode)
{
    if (mode == 0)
    {
        object->update = 0;
        Animation_ApplyChildValues(object, 0);
        Func_02004e30(0x186);
    }
    else if (mode == 1)
    {
        object->update = (void (*)(void))Func_02003b38;
    }
    else
    {
        object->update = (void (*)(void))Func_02003b70;
        Func_02004e30(0x123);
    }
}
