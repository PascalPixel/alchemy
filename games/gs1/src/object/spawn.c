#include "types.h"
#include "scene.h"
#include "abi/object/spawn.h"
#include "global_cells.h"

u8 *Obj_Run(s32, s32, s32, s32);
void Object_Destroy(void);

void Object_SetMode(u8 *, s32);

u8 *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    u8 *base = *(u8 **)ADDR_03001F30;
    u8 *object;
    u8 *child;
    u8 flag;

    object = Obj_Run(kind, x, y, z);
    if (object != 0) {
        if (object[84] == 0) {
            Object_Destroy();
            return 0;
        }
        *(u32 *)(object + 20) = *(u32 *)(*(u8 **)(base + 16) + 20);
        flag = 4;
        object[85] = flag;
        object[35] = flag;
        child = *(u8 **)(object + 80);
        child[9] &= ~(flag + 8);
        Obj_Apply(object, 0);
        Object_SetMode(object, 1);
    }
    return object;
}
