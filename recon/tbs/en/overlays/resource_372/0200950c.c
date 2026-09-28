/* NONMATCHING: resource_372 at 0x0200950c, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_D.C, stays listing.
 *
 * Remaining difference: the ROM keeps a message number in a saved register and loads it from the literal pool where the source named a link-time symbol; the main image has no name for it, and a plain constant is loaded ahead of the preceding call instead.
 */

#include "GROUP_DEPARTURE.H"

void FieldScene_RunScene372SequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_e67;

    Event_Begin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x106, 0x32a);
    Actor_SetPosition(20, 0x1060000, 0x3250000);
    Actor_WalkTo(20, 0x106, 0x339);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x11a, 0x357);
    Actor_SetAnimation(20, 1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
    Func_02005e24();
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(20, 0x100, 20);
    base5_e67 = MSG_HUH_ROBIN;
    Event_SetMessage(base5_e67);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Event_AskYesNo(20, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_SetMessage((base5_e67 + 4));
    Event_ShowMessageAndWait(20, 0, 20);
    Call2(Func_02005d2e, 20, 0x200c8c0);
    GameFlag_Set(0x835);
    Event_End();
}
