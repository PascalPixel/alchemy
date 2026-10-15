#include "ov662.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);

// Func_02001120's counterpart: the leader's terrain becomes 2, and in rooms 1
// and 3 the same calls run with their first and last pairs swapped, each
// followed by more.
void Func_02001040(void)
{
    Object_GetById(gPartyState.current_owner)->terrain_id = 2;
    if (gPartyState.scene == (s32)&SceneId_Ov662Room1) {
        Ov662_Call1848(55, 71, 4, 1, 43, 70);
        Ov662_Call1848(43, 71, 4, 1, 55, 71);
    }
    /* FAKEMATCH: the native reads the scene again here, as Func_02001120 does; without the barrier the read is moved up both paths */
    __asm__("" : : : "memory");
    if (gPartyState.scene == (s32)&SceneId_Ov662Room3) {
        Ov662_Call1848(110, 45, 5, 2, 90, 50);
        Ov662_Call1848(113, 47, 2, 1, 90, 52);
        Ov662_Call1848(107, 48, 3, 2, 90, 53);
        Ov662_Call1848(97, 58, 5, 2, 110, 45);
        Ov662_Call1848(97, 58, 2, 1, 113, 47);
        Ov662_Call1848(97, 58, 3, 2, 107, 48);
    }
}

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
