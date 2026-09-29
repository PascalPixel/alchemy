/* Draft of FieldScene_RunScene3bf_02004794, resource_3bf at 0x0200c794, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: a message number held across calls is a C constant,
 * which GCC's second scheduling pass hoists above the calls before its first
 * use; the ROM loads it after them, as it would a link-time value.
 * The listing keeps these rows. */
#include "FORTRESS.H"

void FieldScene_RunScene3bf_02004794(void)
{
    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Event_Begin();
    if (GameFlag_IsSet(0x941) != 0) {
        Event_SetMessage(0x2566);
        Event_ShowMessage(18, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x313) != 0) {
            Event_SetMessage(0x2457);
            Event_OpenMessage(25, 0);
            Event_End();
        } else {
            Actor_ShowEmote(25, 0x102, 30);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            base5_244f = 0x244f;
            Event_SetMessage(base5_244f);
            Event_ShowMessage(25, 0);
            Actor_FaceActor(25, 24, 0);
            Camera_MoveToActor(24, 1);
            Camera_WaitForMove();
            Event_Wait(60);
            Camera_MoveToActor(0, 1);
            Event_Wait(20);
            Actor_ShowEmote(25, 0x105, 60);
            Event_SetMessage((base5_244f + 1));
            Event_ShowMessage(25, 0);
            Actor_ShowEmote(25, 0x107, 60);
            Event_SetMessage((base5_244f + 2));
            Event_ShowMessage(25, 0);
            Event_Wait(70);
            Actor_ShowEmote(25, 0x100, 60);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            Event_SetMessage((base5_244f + 3));
            Event_OpenMessage(25, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_SetMessage((base5_244f + 4));
                Event_OpenMessage(25, 0);
            } else {
                Event_SetMessage((base5_244f + 5));
                Event_OpenMessage(25, 0);
            }
            Event_Wait(60);
            Actor_ShowEmote(25, 0x105, 60);
            base5_2455 = 0x2455;
            Event_SetMessage(base5_2455);
            Event_OpenMessage(25, 0);
            Actor_RunRepeatedMotion(25, 1);
            Event_SetMessage((base5_2455 + 1));
            Event_OpenMessage(25, 0);
            Actor_SetAnimationAndWait(25, 3);
            Event_SetMessage((base5_2455 + 2));
            Event_OpenMessage(25, 0);
            GameFlag_Set(0x313);
            Event_End();
        }
    }
}
