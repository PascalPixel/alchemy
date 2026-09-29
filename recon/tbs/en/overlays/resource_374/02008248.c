/* Draft of resource_374 0x02008248 (Villager_ShowOffPsynergy): it matches the
 * ROM byte for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgHaidiaArentWorriedCrossing,
 * MsgHaidiaBeholdPowerPsynergy, MsgHaidiaShownNewAbility). The listing keeps
 * these rows until the draft is adopted. */
#include "HAIDIA.H"
extern u8 MsgHaidiaArentWorriedCrossing[];
extern u8 MsgHaidiaBeholdPowerPsynergy[];
extern u8 MsgHaidiaShownNewAbility[];

void Villager_ShowOffPsynergy(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        base5_1197 = (s32)MsgHaidiaArentWorriedCrossing;
        Event_SetMessage(base5_1197);
        if (GameFlag_IsSet(2) != 0) {
            bump_step(1);
        }
        if (GameFlag_IsSet(3) != 0) {
            bump_step(1);
        }
        Event_OpenMessage(17, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((base5_1197 + 3));
        } else {
            Event_SetMessage((base5_1197 + 4));
        }
        Event_ShowMessage(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Event_SetMessage((s32)MsgHaidiaShownNewAbility);
        Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 0);
        Event_AskYesNo(17, 0);
        Event_Wait(20);
        Actor_StartRepeatedMotion(17, 2);
        Event_Wait(15);
        SceneState_ApplyPair140And0();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            OverlayObject_UpdateOnFrameBit1(((s32 (*)())Engine_ActorGet)(17));
            Task_Wait(1);
        }
        Value2(Engine_ScheduleCallback, 0x200a591, 0xc80);
        Audio_PlayCue(107);
        for (i = 0; i != 180; i++) {
            if (Value2(IwramUnsignedRemainder, i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Event_Wait(1);
        }
        Audio_PlayCue(0x121);
        Call1(Scheduler_RemoveCallback, 0x200a591);
        Task_Wait(1);
        FieldScene_Forward4dac();
        Actor_SetChildValue(17, 0);
        Event_Wait(40);
        Event_SetMessage((s32)MsgHaidiaBeholdPowerPsynergy);
        Event_ShowMessage(17, 0);
    }
    Event_End();
}
