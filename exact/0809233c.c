#include "object_runtime.h"

void Func_08092064(s32, s32, s32);
struct ObjectRuntime *Func_08092054(u32);
void Func_080923e4(u32, s32, s32);
void Func_08009080(struct ObjectRuntime *, s32);
void Func_0809228c(s32, s32, s32);
void Func_08009098(struct ObjectRuntime *, void *);

extern s32 Data_02000240[];
extern u8 Data_0809fbcc;

void Func_0809233c(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct ObjectRuntime *object = Func_0808ba1c(arg0);

    if (object != NULL) {
        struct ObjectRuntime *other;

        Func_08092064(arg0, 0x9999, 0x4CCC);
        other = Func_08092054(Data_02000240[125]);
        if (other != NULL)
            Func_080923e4(arg0, other->x, other->z);
        object->movement_state = 0;
        Func_08009080(object, 2);
        Func_0809228c(arg0, arg1, arg2);
        Func_08009098(object, &Data_0809fbcc);
        object->action = arg3;
    }
}
