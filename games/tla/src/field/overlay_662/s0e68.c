#include "ov662.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02000e68(s32 unused, s32 id)
{
    if (gPartyState.scene == (s32)&SceneId_Ov662Room2) {
        s32 row = 12;

        Func_02001848(98, 3, 3, 3, 97, row);
        Func_02001848(33, 12, 1, 1, 34, row);
    }
    /* FAKEMATCH: the native reads the scene again here; without the barrier the read is moved up both paths */
    __asm__("" : : : "memory");
    if (gPartyState.scene == (s32)&SceneId_Ov662Room3) {
        if (id == 12) {
            Ov662_Call1848(102, 15, 1, 1, 103, 15);
            Ov662_Call1848(43, 14, 1, 1, 39, 16);
        }
        if (id == 13) {
            struct ObjectRuntime *object = Object_GetById(13);

            s32 column = object->x >> 20;
            s32 row = object->z >> 20;

            Func_02001848(7, 10, 1, 1, column, row);
        }
    }
}
