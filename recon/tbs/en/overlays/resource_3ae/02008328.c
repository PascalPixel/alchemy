/* Draft of resource_3ae 0x02008328 (SceneDialogue_RunActor12Event), built with
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/KAREI.H.
 * Remaining difference: none in its bytes, but the ROM holds message 0x1d20 as a link-time value and adds one and two to it; no source defines that value.
 * The listing keeps these rows. */
#include "KAREI.H"

extern u8 LinkedMessage_DoYouWantGoAshore[];

void SceneDialogue_RunActor12Event(void)
{
    s32 rec7;
    s32 rec8;
    u8 *record;
    s16 angle;

    record = Func_020018ce(0);
    angle = (u16)((*(u16 *)(record + 6) + 0x2000) & ~0x3fff);
    Event_Begin();
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
            Event_SetMessage(0x1d23);
            Event_OpenMessage(12, 0);
            goto L_02000496;
        }
        Event_SetMessage((s32)LinkedMessage_DoYouWantGoAshore);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(((s32)LinkedMessage_DoYouWantGoAshore + 1));
            Event_ShowMessage(12, 0);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            Event_Wait(20);
            GameFlag_Set(0x8a9);
            goto L_02000496;
        }
        Event_SetMessage(((s32)LinkedMessage_DoYouWantGoAshore + 2));
        Event_ShowMessage(12, 0);
    } else {
        if ((u16)angle != 0x8000) {
            goto L_0200049a;
        }
        Event_SetMessage(0x1d16);
        Event_ShowMessage(12, 0);
        if (GameFlag_IsSet(0x8a5) != 0) {
            rec7 = Func_0200194c(235);
            rec8 = Func_0200194c_next(rec7, 235);
            Actor_SetAnimationAndWait(12, 3);
            Actor_WalkToAndWait(12, 88, 0x508);
            Actor_FaceDirection(12, 0x4000, 0);
            bump_step(1);
            Event_ShowMessage(12, 0);
            Func_020019be(rec7, rec8);
            GameFlag_Set(0x8a7);
            record = Func_020019fa(0);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), 0x518);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 0x518);
            Actor_WalkToAndWait(12, 88, 0x518);
            Actor_FaceDirection(12, 0, 0);
        } else {
            Func_02001abc_scene_dialogue(12, 0);
        }
    }
    L_02000496:;
    Event_End();
    L_0200049a:;
}
