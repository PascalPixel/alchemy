/* The door of truth: actors 8 and 9 are its two leaves. Flag 0x985 records
   it open; a switch opens it for one who sees with a true heart. */
#include "IRIGUCHI.H"
extern u8 MsgBabiTruthDoorOpenThoseSeeing[];
extern u8 MsgFieldFlippedSwitch[];

void FieldScene_RunActorEventSequence(void);

/* Slide the leaves apart and open the passage; the first time, the scene
   that follows plays. */
void BabiIriguchi_OpenTruthDoor(void)
{
    if (GameFlag_IsSet(0x985) == 0) {
        GameFlag_Set(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x118, 240);
        Actor_SetDestination(9, 0x148, 240);
        Iriguchi_WaitForMove(8);
        Iriguchi_WaitForMove(9);
        Iriguchi_CopyCellAttributes(81, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x989) == 0) {
            FieldScene_RunActorEventSequence();
        }
    }
}

/* Slide the leaves together, close the passage and toggle flag 0x301. */
void BabiIriguchi_CloseTruthDoor(void)
{
    if (GameFlag_IsSet(0x985) != 0) {
        GameFlag_Clear(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x128, 240);
        Actor_SetDestination(9, 0x138, 240);
        Iriguchi_WaitForMove(8);
        Iriguchi_WaitForMove(9);
        Iriguchi_CopyCellAttributes(0, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x301) != 0) {
            GameFlag_Clear(0x301);
        } else {
            GameFlag_Set(0x301);
        }
    }
}

/* The door's switch: while the word at +0xcb8 of the event work is set the
   leader flips it and the door opens; otherwise the door's words show. */
void BabiIriguchi_FlipTruthDoorSwitch(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 *seeing;

    Event_Begin();
    seeing = (s16 *)(work + 0xcb8);
    if (seeing[0] != 0) {
        if (GameFlag_IsSet(0x985) == 0) {
            /* FAKEMATCH: forced temporaries; the destination stays in two
               saved registers across both copies, where plain constants
               are built again for each call. */
            s32 dest_x = 17, dest_y = 78;

            Message_ShowCentered((s32)MsgFieldFlippedSwitch, 1);
            Audio_PlayCue(155);
            Engine_MapCopyCellsTo(35, 78, 1, 2, dest_x, dest_y);
            Iriguchi_Wait(10);
            Engine_MapCopyCellsTo(34, 78, 1, 2, dest_x, dest_y);
            Iriguchi_Wait(10);
            BabiIriguchi_OpenTruthDoor();
        }
    } else {
        Event_SetMessage((s32)MsgBabiTruthDoorOpenThoseSeeing);
        Event_ShowMessage(-1, 0);
    }
    Event_End();
}
