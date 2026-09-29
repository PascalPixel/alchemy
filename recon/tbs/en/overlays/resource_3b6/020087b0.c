/* Draft of resource_3b6 0x020087b0 (FieldScene_RunScene3b6_020007b0): it
 * matches the ROM byte for byte now that the messages it loads from the
 * literal pool have catalogue names (MsgTorebiHeeHeeLook, MsgTorebiHello,
 * MsgTorebiHoHumFine). The listing keeps these rows until the draft is
 * adopted. */
#include "TOREBI.H"
extern u8 MsgTorebiHeeHeeLook[];
extern u8 MsgTorebiHello[];
extern u8 MsgTorebiHoHumFine[];

void FieldScene_RunScene3b6_020007b0(s32 a0)
{
    void Event_ShowMessage();

    u32 i;
    s32 record;
    s32 base5_2399;

    Event_Begin();
    if (GameFlag_IsSet(0x8bd) == 0) {
        base5_2399 = (s32)MsgTorebiHeeHeeLook;
        Event_SetMessage(base5_2399);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage((base5_2399 + 1));
        } else {
            Event_SetMessage((base5_2399 + 2));
        }
        Event_ShowMessage(a0, 0);
    } else {
        if (GameFlag_IsSet(0x8be) == 0) {
            GameFlag_Set(0x8be);
            Event_SetMessage((s32)MsgTorebiHello);
            Event_ShowMessage(a0, 0);
            Event_Wait(10);
            Actor_RunRepeatedMotion(a0, 2);
            Event_Wait(20);
        }
        Event_SetMessage((s32)MsgTorebiHoHumFine);
        Event_ShowMessage(a0, 0);
    }
    Event_End();
}
