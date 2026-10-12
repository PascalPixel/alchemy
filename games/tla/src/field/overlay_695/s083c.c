#include "local.h"

extern char SceneId_Ov695;

void Func_0200083c(void)
{
    Func_02001080();
    Func_020011e0(0);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party->battle_rule_24b = 2;
        /* FAKEMATCH: the loop note ends the scheduling block after the store; without it the scene id is loaded before the store (two reordered rows). */
        do {
        } while (0);
    }
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov695, 20);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov695, 21);
    Func_020011a0(102, 2);
}
