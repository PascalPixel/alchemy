#include "ov662.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02001120(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 0;
    if (gPartyState.scene == (s32)&SceneId_Ov662Room1)
        Ov662_Call1848(43, 70, 4, 1, 55, 71);
    /* FAKEMATCH: the native reads the scene again here; without the barrier the read is moved up both paths (differ 18) */
    __asm__("" : : : "memory");
    if (gPartyState.scene == (s32)&SceneId_Ov662Room3) {
        Ov662_Call1848(90, 50, 5, 2, 110, 45);
        Ov662_Call1848(90, 52, 2, 1, 113, 47);
        Ov662_Call1848(90, 53, 3, 2, 107, 48);
    }
}
