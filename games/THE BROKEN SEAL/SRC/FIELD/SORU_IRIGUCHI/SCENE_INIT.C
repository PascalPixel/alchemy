#include "SORU.H"

void SoruIriguchi_ApplyEntryState(void);
void FieldScene_RunSceneEntryHook(void);

/* The sanctum entrance's scene start: each of its two scenes runs its own
   entry setup. */
s32 Scene_Initialize(void)
{
    s32 scenario = gGameState.scene;

    if (scenario == (s32)&SceneId_SoruIriguchi2) {
        SoruIriguchi_ApplyEntryState();
    } else if (scenario == (s32)&SceneId_SoruIriguchi1) {
        FieldScene_RunSceneEntryHook();
    }
    return 0;
}
