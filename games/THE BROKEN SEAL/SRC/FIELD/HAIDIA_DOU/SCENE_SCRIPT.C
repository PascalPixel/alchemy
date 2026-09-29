#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

void WaitFrames();
void HaidiaDou_ApplyEntryState();


s32 HaidiaDou_RunSceneScript(void)
{
    s32 *request = &gEventWork->start_transition;

    *request = 0x204;
    if (gGameState.scene == (s32)&SceneId_HaidiaDou1) {
        *request = 0x100;
        WaitFrames(1);
        Engine_ActorSetSpritePriority(11, 3);
        Engine_ActorSetSpritePriority(12, 3);
        Engine_GameFlagClear(0x12f);
    }
    HaidiaDou_ApplyEntryState();
    return 0;
}
