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
    s8 mode = state->doku;
    u32 flags = 0;

    if (mode == 1)
        flags = 1;
    if (mode == 2)
        flags |= mode;
    if (state->maboroshi != 0)
        flags |= 0x20;
    if (state->shibire != 0) {
        s32 kind;
        flags |= 4;
        kind = state->class_id;
        if (kind == 0x79 || kind == 0x94)
            flags &= ~4;
    }
    if (state->fuuji != 0)
        flags |= 8;
    if (state->akuryo != 0)
        flags |= 0x40;
    if (state->nemuri != 0)
        flags |= 0x10;
    if (state->shiryo != 0)
        flags |= 1 << (state->shiryo + 6);
    output->active_conditions = flags;
}
