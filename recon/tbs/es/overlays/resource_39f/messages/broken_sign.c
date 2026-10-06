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
