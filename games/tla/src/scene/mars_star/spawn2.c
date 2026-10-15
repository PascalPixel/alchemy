#include "types.h"
#include "object_runtime.h"
#include "call.h"

void Func_020041cc(struct ObjectRuntime *object, s32 mode);
struct ObjectRuntime *Func_020041e4();
void Func_020015ec(void);

void Func_02001928(struct ObjectRuntime *object, s32 priority);

void Func_0200196c(s32 *position, s32 speed, s32 flag)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Value4((s32 (*)())Func_020041e4, 366, position[0], position[1], position[2]);

    if (object != NULL) {
        object->bounce = 0;
        object->gravity = 0;
        object->unknown_4c = -speed;
        Func_02001928(object, 3);
        object->update = Func_020015ec;
        if (flag != 0)
            object->unknown_5c[0] = 2;
        Func_020041cc(object, 2);
    }
}
