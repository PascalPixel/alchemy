/* Pending-action owner selector trial, 2026-10-02.
 * EN ordinary compiler: complete 36-byte owner, score 60: the gPartyState
 * literal load is after the first offset immediate instead of before it.
 * Four ordinary pointer/branch spellings retained the same reorder; the
 * conditional expression scored 240. Finite 128-rewrite search did not improve.
 * The signed halfword is observed at work+0x276. On 2026-10-03 the
 * competing private view was replaced by the maintained PARTY_STATE.H owner.
 * Fresh EN score after this ownership correction remains 60, with one
 * reordered instruction over the complete 36-byte owner. Earlier
 * measurements above remain trial records.
 * Draft only; no full six-edition linked proof, steering device or credit. */
#include "PARTY_STATE.H"

s32 EventRuntime_GetControlledOwner(void)
{
    s32 actor = 8;

    if (gPartyState.owner_override == 0)
        actor = gPartyState.current_owner;
    return actor;
}
