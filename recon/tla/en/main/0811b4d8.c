#include "types.h"
#include "btlpty.h"
#include "battle_status_icon.h"
#include "btlsys.h"
#include "motobj.h"
#include "animspr.h"
#include "btldisp.h"

s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *output)
{
    struct BtlUnit *state = UnitGetFar(id);
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
    if (state->haunt != 0)
        flags |= 0x40;
    if (state->sleep != 0)
        flags |= 0x10;
    if (state->grim_reaper != 0)
        flags |= 1 << (state->grim_reaper + 6);
    output->active_conditions = flags;
}
