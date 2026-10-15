#include "types.h"
#include "object_runtime.h"

// 速度を加え、1/8 ずつ減衰させる
void Func_020011b8(struct ObjectRuntime *object)
{
    object->x += object->bounce;
    object->y += object->gravity;
    object->z += object->unk4c;
    object->bounce -= object->bounce >> 3;
    object->gravity -= object->gravity >> 3;
    object->unk4c -= object->unk4c >> 3;
}
