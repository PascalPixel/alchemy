/* NONMATCHING: resource_3a1 at 0x02008504 (32 bytes with its pool), between
 * FIELD/SHIAN_HEYA/FLAGGED_OBJECT.C and SCENE_SETUP.C, stays listing.
 *
 * Remaining difference: the ROM loads message 0x1a40 from its literal pool,
 * so the source named it by a link-time symbol; the main image has no name
 * for 0x1a40, and a plain constant builds it with a move and a shift.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 LinkedMessage_DidMonstersInAltinSpit;

void SceneDialogue_RunActor12Dialogue(void)
{
    void Event_SetMessage(s32);
    s32 Event_AskYesNo(s32, s32);

    Event_Begin();
    Event_SetMessage((s32)&LinkedMessage_DidMonstersInAltinSpit);
    Event_AskYesNo(12, 0);
    Event_End();
}
