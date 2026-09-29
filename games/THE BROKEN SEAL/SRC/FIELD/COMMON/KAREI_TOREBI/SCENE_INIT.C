#include "KAREI.H"
#include "SCENE_IDS.H"

void FieldScene_ConfigureFlaggedActors(void);


s32 Scene_Initialize(void)
{
    u32 i;
    s32 record;

    if (gGameState.entrance == 90) {
        GameFlag_Set(0x950);
    }
    if (gGameState.scene == (s32)&SceneId_KareiTorebi1) {
        FieldScene_RunScene3ae_020008cc();
    } else {
        if (gGameState.scene == (s32)&SceneId_KareiTorebi3) {
            FieldScene_ConfigureFlaggedActors();
        } else {
            if (gGameState.scene == (s32)&SceneId_KareiTorebi2) {
                SceneState_SetRuntimeWord448To521AndSend303();
            }
        }
    }
    return 0;
}
