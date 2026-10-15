#include "types.h"
#include "object_runtime.h"

extern u32 gFrameCount;
extern const u8 gData_02002498[];

struct ObjectRuntime *Func_02001f0c(s32 type, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02001efc(struct ObjectRuntime *object, s32 value);
void Func_02001f04(struct ObjectRuntime *object, const void *data);

void Func_02001d60(struct ObjectRuntime *object)
{
    struct ObjectRuntime *spawned;

    if ((gFrameCount & 7) == 0) {
        spawned = Func_02001f0c(14, object->x, object->y, object->z);
        if (spawned != 0) {
            ObjectDispatch_SetSingleChildFlags(spawned, 0);
            Func_02001efc(spawned, 1);
            Func_02001f04(spawned, gData_02002498);
        }
    }
}
