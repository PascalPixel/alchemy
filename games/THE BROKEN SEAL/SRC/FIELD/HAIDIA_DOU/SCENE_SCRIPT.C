#include "TYPES.H"
#include "SCENE_IDS.H"
extern struct EventWork *gEventWork;

extern u8 Data_02000240[];

void WaitFrames();
void Engine_ActorSetSpritePriority();
void Engine_GameFlagClear();
void HaidiaDou_ApplyEntryState();


s32 HaidiaDou_RunSceneScript(void)
{
    s32 off = 448;
    s32 *request = (s32 *)(*(u8 **)&gEventWork + off);

    *request = 0x204;
    if (*(s16 *)(Data_02000240 + off) == (s32)&SceneId_HaidiaDou1) {
        *request = 0x100;
        WaitFrames(1);
        Engine_ActorSetSpritePriority(11, 3);
        Engine_ActorSetSpritePriority(12, 3);
        Engine_GameFlagClear(0x12f);
    }
    HaidiaDou_ApplyEntryState();
    return 0;
}
