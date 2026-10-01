/* NONMATCHING: Japanese compile-known old-couple dialogue, 2026-10-01.
 * The complete branch scene compiles with approved TBS flags. Message 5856
 * uses a shifted byte in place of the original pool load and shortens this
 * owner by four bytes. Production uses the canonical PO symbol instead.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "text/MSG_IDS.H"
TEXT_MESSAGE_ENUM(MsgImiruEveryoneCountsOnMaryThats);
TEXT_MESSAGE_ENUM(MsgImiruHaveVisitedOldCoupleWho);

void SceneDialogue_RunActor20BranchScene(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(3) != 0) {
        Engine_EventSetMessage(MsgImiruEveryoneCountsOnMaryThats);
        Event_ShowMessage(20, 0);
    } else {
        Engine_EventSetMessage(MsgImiruHaveVisitedOldCoupleWho);
        Event_AskYesNo(20, 0);
        GameFlag_Set(0x82a);
        GameFlag_Set(0x82c);
    }
    Engine_EventEnd();
}
