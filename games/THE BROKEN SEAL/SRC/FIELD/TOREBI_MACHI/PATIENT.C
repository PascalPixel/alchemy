#include "MACHI.H"
extern u8 MsgTorebiPatientLittleGuy[];

void FieldScene_RunPatientTalk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiPatientLittleGuy);
    Call3(Engine_ActorFaceDirection, 25, 0xc000, 0);
    Engine_EventShowMessage(25, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 0);
    Engine_EventShowMessage(25, 0);
    Engine_EventEnd();
}
