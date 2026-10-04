/* Ported from ☀️ games/THE BROKEN SEAL/SRC/BATTLE/BATTLE.C. */
#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_STATUS_ICON.H"
#include "BATTLE_RUNTIME.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "BATTLE_PRESENTATION.H"

s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObjectSlot *output)
{
    struct BattleUnit *state = Owner_GetStateFar(id);
    s8 mode = state->poison;
    u32 flags = 0;

    if (mode == 1)
        flags = 1;
    if (mode == 2)
        flags |= mode;
    if (state->delusion != 0)
        flags |= 0x20;
    if (state->stun != 0) {
        s32 kind;
        flags |= 4;
        kind = state->class_id;
        if (kind == 0x79 || kind == 0x94)
            flags &= ~4;
    }
    if (state->psy_seal != 0)
        flags |= 8;
    if (state->evil_spirit != 0)
        flags |= 0x40;
    if (state->sleep != 0)
        flags |= 0x10;
    if (state->death_count != 0)
        flags |= 1 << (state->death_count + 6);
    output->active_conditions = flags;
}
