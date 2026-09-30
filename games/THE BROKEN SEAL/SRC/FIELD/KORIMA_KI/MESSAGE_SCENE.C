#include "TYPES.H"
#include "CALL.H"
extern u8 MsgKorimaWho[];
extern u8 MsgKorimaForestKolimaAlive[];
extern u8 MsgKorimaLeaveBeforeForest[];

void Engine_EventBegin();
void KorimaKi_PlayGesture(s32 actor, s32 gesture);
s32 Engine_GameFlagIsSet();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventShowMessageAndWait();
void Engine_EventWait();
void Engine_ActorSetAnimationAndWait();
void Engine_GameFlagSet();
void Engine_EventEnd();

void KorimaKi_RunMessageScene(void)
{
    Engine_EventBegin();
    KorimaKi_PlayGesture(11, 1);
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_EventSetMessage((s32)MsgKorimaForestKolimaAlive);
        Engine_EventShowMessage(9, 0);
    } else if (Value1(Engine_GameFlagIsSet, 0x84c) != 0) {
        Engine_EventSetMessage((s32)MsgKorimaLeaveBeforeForest);
        Engine_EventShowMessage(9, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaWho);
        Engine_EventShowMessageAndWait(9, 0, 20);
        KorimaKi_PlayGesture(11, 0);
        Engine_EventWait(60);
        KorimaKi_PlayGesture(11, 1);
        Engine_EventShowMessageAndWait(9, 0, 10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(40);
        Engine_EventShowMessage(9, 0);
        KorimaKi_PlayGesture(11, 0);
        Engine_EventWait(80);
        Engine_EventShowMessageAndWait(9, 0, 20);
        KorimaKi_PlayGesture(11, 1);
        Engine_EventShowMessageAndWait(9, 0, 20);
        Engine_GameFlagSet(0x84c);
    }
    KorimaKi_PlayGesture(11, 0);
    Engine_EventEnd();
}
