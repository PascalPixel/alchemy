#include "YAMA.H"

void ArutinYama_ApplyEntryState(void);
void ArutinYama_PlaceFlaggedActors(void);

/* Altin Peak: open the screen with the window transition and run the entry
 * scene of the area the party enters; the second and fourth areas have
 * none. */
s32 ArutinYama_RunAreaScene(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_ArutinYama1) {
        ArutinYama_ApplyEntryState();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama3) {
        FieldScene_RunScene3a4_02002310();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama4) {
        FieldScene_RunScene3a4_02002428();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama5) {
        FieldScene_RunScene3a4_02002490();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama6) {
        FieldScene_RunScene3a4_020025c0();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama7) {
        FieldScene_RunScene3a4_020026c0();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama9) {
        ArutinYama_PlaceFlaggedActors();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama10) {
        FieldScene_RunScene3a4_02002934();
    } else if (gGameState.scene == (s32)&SceneId_ArutinYama11) {
        FieldScene_RunScene3a4_020029dc();
    }
    return 0;
}
