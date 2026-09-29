/* Draft of resource_3b5 0x02008528 (FieldScene_RunScene3b5_02000528): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgTorebiPatientLittleGuy). The listing
 * keeps these rows until the draft is adopted. */
#include "MACHI.H"
extern u8 MsgTorebiPatientLittleGuy[];

void FieldScene_RunScene3b5_02000528(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiPatientLittleGuy);
    Call3(Engine_ActorFaceDirection, 25, 0xc000, 0);
    Engine_EventShowMessage(25, 0);
    Call3(Engine_ActorFaceDirection, 25, 0x8000, 0);
    Engine_EventShowMessage(25, 0);
    Engine_EventEnd();
}
