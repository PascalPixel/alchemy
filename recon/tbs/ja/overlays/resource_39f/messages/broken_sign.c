/* NONMATCHING: broken sign message before the PO enum import, 2026-10-01.
 * The approved TBS flags compile the complete 36-byte extent. Only 2
 * literal-pool bytes differ: this partial localized scene retains the
 * English message number. Production imports the canonical PO constant.
 */
#include "../../../../../../games/THE BROKEN SEAL/SRC/FIELD/MOGORU_MORI/MORI.H"
#include "FIELD_EVENT.H"

enum WaypointMessage { MSG_BROKEN_SIGN_READS_NORTH_FUCHIN = 0x17e6 };

void FieldScene_RunScriptedSteps0And17E6(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered(MSG_BROKEN_SIGN_READS_NORTH_FUCHIN, 1);
    Engine_EventEnd();
}
