#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void SceneState_ClearSlotsBySubState(void);

/* The Kalay houses' scene start: open with the window transition; the first
   scene clears the slots its entrance calls for. */
s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)&SceneId_KareiHeya1) {
        SceneState_ClearSlotsBySubState();
    }
    return 0;
}
