/* Draft of resource_3b8 0x0200bfc4 (SceneDialogue_ShowMessage22a3Branch): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgTorebiWarriorsWhoStayed). The listing
 * keeps these rows until the draft is adopted. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgTorebiWarriorsWhoStayed[];
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

void SceneDialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)MsgTorebiWarriorsWhoStayed;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(k + 1);
        Event_ShowMessage(a, 0);
    } else {
        Event_SetMessage(k + 2);
        Event_ShowMessage(a, 0);
    }
}
