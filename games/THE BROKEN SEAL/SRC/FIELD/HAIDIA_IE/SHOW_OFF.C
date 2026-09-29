/* The villager who shows off his psynergy by shaking the view. */
#include "HAIDIA.H"
#include "MAP_SCROLL.H"
extern u8 MsgHaidiaArentWorriedCrossing[];
extern u8 MsgHaidiaBeholdPowerPsynergy[];
extern u8 MsgHaidiaShownNewAbility[];

/* Once the party has left the vale he only asks about the journey, one line
 * further for each of the two companions; before, he lifts the view up and
 * down for three seconds. */
void Villager_ShowOffPsynergy(void)
{
    u32 i;
    s32 message;
    s32 shakes;
    s32 *origin;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        message = (s32)MsgHaidiaArentWorriedCrossing;
        Event_SetMessage(message);
        if (GameFlag_IsSet(2) != 0) {
            bump_step(1);
        }
        if (GameFlag_IsSet(3) != 0) {
            bump_step(1);
        }
        Event_OpenMessage(17, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(message + 3);
        } else {
            Event_SetMessage(message + 4);
        }
        Event_ShowMessage(17, 0);
    } else {
        origin = Data_03001e70->origin;
        Event_SetMessage((s32)MsgHaidiaShownNewAbility);
        Actor_FaceEachOther(17, ACTOR_PARTY_LEADER, 0);
        Event_AskYesNo(17, 0);
        Event_Wait(20);
        Actor_StartRepeatedMotion(17, 2);
        Event_Wait(15);
        SceneState_ApplyPair140And0();
        shakes = 0;
        for (i = 0; i < 40; i++) {
            OverlayObject_UpdateOnFrameBit1((s32)Engine_ActorGet(17));
            Task_Wait(1);
        }
        Value2(Engine_ScheduleCallback, (s32)FieldScene_RunStep17, 0xc80);
        Audio_PlayCue(107);
        for (i = 0; i != 180; i++) {
            if (Value2(IwramUnsignedRemainder, i, 10) == 0) {
                if ((1 & shakes) != 0) {
                    *origin -= 0x10000;
                } else {
                    *origin += 0x10000;
                }
                shakes = shakes + 1;
            }
            Event_Wait(1);
        }
        Audio_PlayCue(0x121);
        Call1(Scheduler_RemoveCallback, (s32)FieldScene_RunStep17);
        Task_Wait(1);
        FieldScene_Forward4dac();
        Actor_SetChildValue(17, 0);
        Event_Wait(40);
        Event_SetMessage((s32)MsgHaidiaBeholdPowerPsynergy);
        Event_ShowMessage(17, 0);
    }
    Event_End();
}
