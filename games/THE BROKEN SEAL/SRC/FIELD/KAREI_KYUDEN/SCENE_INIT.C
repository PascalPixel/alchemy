#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FieldScene_DispatchSceneByIndex(void);

/* The palace's scene start: open with the window transition and, in the
   palace itself, run its scene. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)(u32)&SceneId_KareiKyuden) {
        FieldScene_DispatchSceneByIndex();
    }
    return 0;
}
