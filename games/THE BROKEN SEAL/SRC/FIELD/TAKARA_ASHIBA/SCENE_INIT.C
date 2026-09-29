/* The platforms' scene start, entry veneer 0: the screen opens through the
 * window, and each platform runs its own opening. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FieldScene_RunScene3b4_02002188(void);
void FieldScene_RunScene3b4_02002290(void);
void FieldScene_RunScene3b4_02002334(void);

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba1) {
        FieldScene_RunScene3b4_02002188();
    }
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba2) {
        FieldScene_RunScene3b4_02002290();
    }
    if (gGameState.scene == (s32)&SceneId_TakaraAshiba3) {
        FieldScene_RunScene3b4_02002334();
    }
    return 0;
}
