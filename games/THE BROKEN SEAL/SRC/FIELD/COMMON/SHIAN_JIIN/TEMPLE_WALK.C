#include "TYPES.H"
#include "CALL.H"
extern u8 MsgShianAlreadyHeardTest[];

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_AudioPlayCue();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Effect_Spawn();
void Engine_EventWait();
void BattleFx_PlayQueuedSoundFar();
void Engine_MapCopyCellAttributes();
void Engine_GameFlagSet();
void Engine_EventEnd();

void ShianJiin_RunTempleWalkScene(void)
{
    s32 *p8;
    s32 rec8;
    s32 record;
    u8 slot16[40];

    rec8 = Engine_ActorGet(9);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgShianAlreadyHeardTest);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Call3(Engine_ActorWalkToAndWait, 0, 168, 0x188);
    Engine_ActorFaceDirection(0, 0xc000, 20);
    Engine_AudioPlayCue(132);
    record = Engine_ActorGet(9);
    *(s32 *)(record + 40) = 0x140000;
    record = Engine_ActorGet(9);
    *(s32 *)(record + 72) = 0x40000;
    Call3(Engine_ActorSetSpeed, 9, 0x30000, 0x18000);
    Call3(Engine_ActorSetDestination, 9, 152, 0x188);
    Engine_ActorWaitForMove(9);
    record = Engine_ActorGet(9);
    *(s32 *)(record + 72) = 0x10000;
    Engine_ActorFaceDirection(9, 0, 0);
    Engine_AudioPlayCue(132);
    p8 = (s32 *)slot16;
    p8[1] = 7;
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), 0x8000, 0, 0, 0x10000, p8);
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), 0, 0, 0, 0x10000, p8);
    Effect_Spawn(*(s32 *)(rec8 + 8), *(s32 *)(rec8 + 12), (*(s32 *)(rec8 + 16) + 0x40000), -0x8000, 0, 0, 0x10000, p8);
    Engine_EventWait(30);
    BattleFx_PlayQueuedSoundFar();
    Call6(Engine_MapCopyCellAttributes, 10, 24, 1, 1, 10, 22);
    Engine_GameFlagSet(0x892);
    Engine_EventEnd();
}
