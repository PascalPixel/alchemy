#include "types.h"
#include "frame.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
u32 Random16Far(void);
void Func_020009d0(void);

void Func_02000a80(void)
{
    if ((gFrameCount & 7) == 0) {
        struct ObjectRuntime *object = Object_Spawn(308, 0x02c20000, 0xcccc, 0x740000);

        if (object != NULL) {
            ObjectDispatch_SetSingleChildFlags(object, 0);
            object->update = Func_020009d0;
            object->motion_flags = 0;
            object->action = -(s32)((u32)(Random16Far() * 16) >> 16) * 2 + 20;
            object->unknown_66 = 0;
            object->gravity = 0x1999;
            object->velocity_y = 0x30000;
            object->angle = Random16Far();
        }
    }
}
