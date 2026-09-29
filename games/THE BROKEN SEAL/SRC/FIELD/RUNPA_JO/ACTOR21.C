/* The Lunpa fortress: actor 21's line by the day's draw. */
#include "FORTRESS.H"
extern u8 MsgRunpaDodonpasOrdersAbsolute[];
extern u8 MsgRunpaHammetGreatMerchant[];
extern u8 MsgRunpaSighBadCouldnt[];
extern u8 MsgRunpaStrangeSwearSomeone[];
extern u8 MsgRunpaTakeCareAnybody[];
extern u8 MsgRunpaToldStandGuard[];
extern u8 MsgRunpaWhoDisruptingSleep[];
extern u8 MsgRunpaZZZ[];

void FieldScene_SelectActorTwentyOneMessage(void)
{

    switch (gRunpaJoRandomPick) {
    case 0:
        Event_SetMessage((s32)MsgRunpaHammetGreatMerchant);
        Event_ShowMessage(21, 0);
        break;
    case 1:
        Event_SetMessage((s32)MsgRunpaZZZ);
        Event_ShowMessage(21, 0);
        break;
    case 2:
        Event_SetMessage((s32)MsgRunpaDodonpasOrdersAbsolute);
        Event_ShowMessage(21, 0);
        break;
    case 3:
        Event_SetMessage((s32)MsgRunpaSighBadCouldnt);
        Event_ShowMessage(21, 0);
        break;
    case 4:
        Event_SetMessage((s32)MsgRunpaStrangeSwearSomeone);
        Event_ShowMessage(21, 0);
        break;
    case 6:
        Event_SetMessage((s32)MsgRunpaToldStandGuard);
        Event_ShowMessage(21, 0);
        break;
    case 7:
        Event_SetMessage((s32)MsgRunpaWhoDisruptingSleep);
        Event_ShowMessage(21, 0);
        break;
    case 5:
        Actor_FaceDirection(21, 0xd000, 0);
        Event_Wait(50);
        Actor_FaceDirection(21, 0xb000, 0);
        Event_Wait(50);
        Actor_FaceDirection(21, 0x5000, 0);
        Event_Wait(50);
        Event_SetMessage((s32)MsgRunpaTakeCareAnybody);
        Event_ShowMessage(21, 0);
        break;
    }
}
