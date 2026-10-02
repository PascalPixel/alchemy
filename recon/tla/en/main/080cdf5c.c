/* Pending-action owner selector trial, 2026-10-02.
 * EN ordinary compiler: complete 36-byte owner, score 60: the gPartyState
 * literal load is after the first offset immediate instead of before it.
 * Four ordinary pointer/branch spellings retained the same reorder; the
 * conditional expression scored 240. Finite 128-rewrite search did not improve.
 * The signed halfword is observed at work+0x276; the storage owner remains
 * unresolved and the maintained PartyState declaration stops before it.
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

s32 Func_080cdf5c(void)
{
    s32 actor = 8;

    if (gPartyState.owner_override == 0)
        actor = gPartyState.current_owner;
    return actor;
}
