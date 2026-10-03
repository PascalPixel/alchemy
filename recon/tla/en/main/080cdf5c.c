/* Pending-action owner selector trial, 2026-10-02.
 * EN ordinary compiler: complete 36-byte owner, score 60: the gPartyState
 * literal load is after the first offset immediate instead of before it.
 * Four ordinary pointer/branch spellings retained the same reorder; the
 * conditional expression scored 240. Finite 128-rewrite search did not improve.
 * The signed halfword is observed at work+0x276. The current maintained
 * PartyState now names it owner_override; this draft retains its original
 * private view and measured mismatch. The name repair adds no byte proof.
 * Draft only; no full six-edition linked proof, steering device or credit. */
#include "TYPES.H"

struct PendingPartyState {
    u8 unknown_000[0x214];
    s32 current_owner;
    u8 unknown_218[0x54];
    s8 pending_third;
    u8 unknown_26d[9];
    s16 owner_override;
};
extern struct PendingPartyState gPartyState;

s32 EventRuntime_GetControlledOwner(void)
{
    s32 actor = 8;

    if (gPartyState.owner_override == 0)
        actor = gPartyState.current_owner;
    return actor;
}
