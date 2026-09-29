#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void RunGuardedSceneSetup(void);
void SceneState_SetRuntimeWord448To516(void);
void FieldScene_RunScene398SequenceC(void);

/* The cave's scene start: each of its three areas runs its own setup. */
s32 Scene_Initialize(void)
{
    s16 variant = gGameState.scene;

    if (variant == (s32)&SceneId_BiribinoDou3) {
        RunGuardedSceneSetup();
    } else if (variant == (s32)&SceneId_BiribinoDou2) {
        SceneState_SetRuntimeWord448To516();
    } else if (variant == (s32)&SceneId_BiribinoDou1) {
        FieldScene_RunScene398SequenceC();
    }
    return 0;
}
