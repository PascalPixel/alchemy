#ifndef ALCHEMY_PARTY_STATE_H
#define ALCHEMY_PARTY_STATE_H

#include "types.h"

struct PartyState {
    u8 unknown_000[0x1f8];
    u8 active_owners[8];
};

extern struct PartyState Data_02000240;

/*
 * Most callers use the true no-argument interface. Func_08079664's reference
 * object keeps its owner value live by presenting it at both calls, even
 * though the callee ignores r0, so that translation unit intentionally uses
 * the old-style call form for byte-exact register allocation.
 */
s32 Func_080795fc();
s32 Func_080796c4(s16 *owners);

#endif
