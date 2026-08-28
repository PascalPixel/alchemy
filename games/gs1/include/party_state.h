#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "layout_guard.h"

struct PartyState {
    u8 unknown_000[0x10];
    s32 money;                      /* 0x010 */
    u8 unknown_014[0x1e0];
    s32 current_owner;              /* 0x1f4 */
    u8 active_owners[8];            /* 0x1f8 */
};

LAYOUT_OFFSET_GUARD(
    PartyState_Money,
    struct PartyState,
    money,
    0x10);
LAYOUT_OFFSET_GUARD(
    PartyState_CurrentOwner,
    struct PartyState,
    current_owner,
    0x1f4);
LAYOUT_OFFSET_GUARD(
    PartyState_ActiveOwners,
    struct PartyState,
    active_owners,
    0x1f8);
LAYOUT_SIZE_GUARD(PartyState_Size, struct PartyState, 0x200);

extern struct PartyState Data_02000240;

#define PARTY_STATE Data_02000240

/*
 * Most callers use the true no-argument interface. Func_08079664's reference
 * object keeps its owner value live by presenting it at both calls, even
 * though the callee ignores r0, so that translation unit intentionally uses
 * the old-style call form for byte-exact register allocation.
 */
s32 Func_080795fc();
s32 Func_080796c4(s16 *owners);

#define Party_CountActiveOwners Func_080795fc
#define Party_ListActiveOwners  Func_080796c4

#endif
