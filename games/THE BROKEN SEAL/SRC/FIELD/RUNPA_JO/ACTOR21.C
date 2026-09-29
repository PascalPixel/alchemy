/* The Lunpa fortress: actor 21's line by the day's draw. */
#include "FORTRESS.H"

void FieldScene_SelectActorTwentyOneMessage(void)
{

    switch (gRunpaJoRandomPick) {
    case 0:
        Event_SetMessage(0x2414);
        Event_ShowMessage(21, 0);
        break;
    case 1:
        Event_SetMessage(0x2415);
        Event_ShowMessage(21, 0);
        break;
    case 2:
        Event_SetMessage(0x2416);
        Event_ShowMessage(21, 0);
        break;
    case 3:
        Event_SetMessage(0x2417);
        Event_ShowMessage(21, 0);
        break;
    case 4:
        Event_SetMessage(0x2418);
        Event_ShowMessage(21, 0);
        break;
    case 6:
        Event_SetMessage(0x241a);
        Event_ShowMessage(21, 0);
        break;
    case 7:
        Event_SetMessage(0x241b);
        Event_ShowMessage(21, 0);
        break;
    case 5:
        Actor_FaceDirection(21, 0xd000, 0);
        Event_Wait(50);
        Actor_FaceDirection(21, 0xb000, 0);
        Event_Wait(50);
        Actor_FaceDirection(21, 0x5000, 0);
        Event_Wait(50);
        Event_SetMessage(0x2419);
        Event_ShowMessage(21, 0);
        break;
    }
}
