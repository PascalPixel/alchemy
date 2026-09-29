#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void KareiMachi_SetupEntryActors(void);
void SceneState_CheckFlags941And940(void);
void SceneState_ApplyFlagGatedActorEightSetup(void);
void SceneState_SetWork448AndRunFlag915Step(void);
void FieldScene_RunMiddleSequence(void);

/* Kalay's scene start: set flag 0x87a and run the setup of the scene the
   party enters. */
s32 Scene_Initialize(void)
{
    s32 v;

    GameFlag_Set(0x87a);
    v = gGameState.scene;
    if (v == (s32)&SceneId_KareiMachi1) {
        KareiMachi_SetupEntryActors();
    } else if (v == (s32)&SceneId_KareiMachi2) {
        SceneState_CheckFlags941And940();
    } else if (v == (s32)&SceneId_KareiMachi3) {
        SceneState_ApplyFlagGatedActorEightSetup();
    } else if (v == (s32)&SceneId_KareiMachi5) {
        SceneState_SetWork448AndRunFlag915Step();
    } else if (v == (s32)&SceneId_KareiMachi6) {
        FieldScene_RunMiddleSequence();
    }
    return 0;
}
