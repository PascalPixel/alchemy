#include "FORTRESS.H"

extern const u32 gRunpaJoExits2[];
extern const u32 gRunpaJoExits3And4[];
extern const u32 gRunpaJoExitsOther[];

/* Where the fortress's exits lead; the third and fourth rows share theirs. */
const u32 *Scene_GetExits(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoExits2;
    }
    if (scene == (s32)&SceneId_RunpaJo3 || scene == (s32)&SceneId_RunpaJo4) {
        return gRunpaJoExits3And4;
    }
    return gRunpaJoExitsOther;
}
