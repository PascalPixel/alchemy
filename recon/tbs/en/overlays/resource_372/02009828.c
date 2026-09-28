/* NONMATCHING: resource_372 at 0x02009828, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_E.C, stays listing.
 *
 * Remaining difference: the ROM keeps a message number in a saved register and loads it from the literal pool where the source named a link-time symbol; the main image has no name for it, and a plain constant is loaded ahead of the preceding call instead.
 */

#include "GROUP_DEPARTURE.H"

void FieldScene_RunScene372SequenceE(void)
{
    u32 i;
    s32 record;
    s32 base5_e74;
    s32 v6;

    if (GameFlag_IsSet(0x837) != 0) {
    } else {
        Event_Begin();
        Actor_SetAttachedEffect(22, 0x100);
        base5_e74 = MSG_HEY_ROBIN;
        Event_SetMessage(base5_e74);
        Event_ShowMessage(22, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Camera_SetSpeed(0x6666, 0xccc);
        Camera_MoveTo(0x1000000, -1, 0x24c0000, 1);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Value2(Func_02005ff6, 22, 0x200c934);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
        Event_Wait(30);
        Value2(Engine_ActorEnableActionCallback, 22, 0x200c984);
        Event_ShowMessage(22, 0);
        v6 = 128;
        record = Actor_Get(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Actor_RunRepeatedMotion(22, 1);
        Event_Wait(20);
        Event_AskYesNo(22, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage((base5_e74 + 5));
        Event_ShowMessageAndWait(22, 0, 20);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessage(22, 0);
        Actor_SetSpeed(22, (v6 << 9), 0x8000);
        Actor_SetAnimation(22, 2);
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Func_0200606c(1, 1);
        Actor_SetAnimation(21, 3);
        GameFlag_Set(0x837);
        Event_End();
    }
}
