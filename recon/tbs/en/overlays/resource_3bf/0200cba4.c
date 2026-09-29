/* Draft of ConfigureSceneActor13, resource_3bf at 0x0200cba4, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: the ROM loads message 0x2440 from its literal pool, as a
 * link-time value; as a C constant GCC builds it with mov and lsl.
 * The listing keeps these rows. */
#include "FORTRESS.H"

void ConfigureSceneActor13(void)
{
    Actor_RunRepeatedMotion(13, 2);
    Event_SetMessage(0x2440);
    Event_ShowMessage(13, 0);
}
