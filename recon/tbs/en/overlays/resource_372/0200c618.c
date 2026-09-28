/* NONMATCHING: resource_372 at 0x0200c618, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_H.C, stays listing.
 *
 * Remaining difference: the ROM loads message 0x1120 from its literal pool, so the source named it by a link-time symbol; the main image has no name for it, and a plain constant builds it with a move and a shift.
 */

#include "GROUP_DEPARTURE.H"

void FieldScene_RunScriptedStep1120(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ITS_GERALD_CHEST_VALUABLES, 1);
    Event_End();
}
