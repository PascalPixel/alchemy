/* Draft of RunActorScriptedSequenceB, resource_3bf at 0x02009c4c, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: a message number held across calls is a C constant,
 * which GCC's second scheduling pass hoists above the calls before its first
 * use; the ROM loads it after them, as it would a link-time value.
 * The listing keeps these rows. */
#include "FORTRESS.H"

/*
 * The resource run is taken as the address of 0x241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Actor_RunRepeatedMotion(handle, 1);
    id = 0x241e;
    Event_SetMessage((s32)id);
    Event_ShowMessage(handle, 0);
    Actor_ShowEmoteAt(handle);
    Event_SetMessage((s32)(id + 1));
    Event_ShowMessage(handle, 0);
    id += 2;
    Actor_SetAnimationAndWait(handle, 4);
    Event_SetMessage((s32)id);
    Event_ShowMessage(handle, 0);
}
