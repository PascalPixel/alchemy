#include "types.h"
#include "btlpty.h"

s32 BattleParty_ListActorIds();
void ObjMotRst();

struct ActionTargets {
    u8 side;
    u8 count;
    u8 unknown_02;
    u8 ids[14];
    u8 state_a[14];
    u8 state_b[14];
    u8 state_c[14];
    u8 state_d[14];
    s32 message;
    s32 unknown_50[2];
    s32 cue;
    s32 unknown_5c;
    s32 extra;
};

s32 BattleParty_PrepareReserveOwners(u16 *unit_ids);

s32 Func_0811a39c(s32 side, u16 *unit_ids);

void Func_0811d89c(struct ActionTargets *targets)
{
    u16 ids[10];
    s32 count;
    s32 i;
    s32 side = 1;

    if (targets->side > 7)
        side = 2;
    count = Func_0811a39c(side, ids);
    for (i = 0; i < count; i++) {
        targets->ids[i] = ids[i];
        targets->state_a[i] = 0;
        targets->state_b[i] = 1;
        targets->state_c[i] = 0;
        targets->state_d[i] = 1;
    }
    targets->count = count;
    targets->message = 103;
    targets->cue = 0x10b5;
}

void Func_0811d914(struct ActionTargets *targets)
{
    u16 ids[10];
    s32 count;
    s32 i;

    if (targets->side <= 7) {
        count = BattleParty_PrepareActiveOwners(ids);
        count += BattleParty_PrepareReserveOwners(&ids[count]);
    } else {
        count = BattleParty_ListActorIds(2, ids);
    }
    for (i = 0; i < count; i++) {
        targets->ids[i] = ids[i];
        targets->state_a[i] = 0;
        targets->state_b[i] = 1;
        targets->state_c[i] = 0;
        targets->state_d[i] = 1;
    }
    {
        s32 zero = 0; /* FAKEMATCH: a named zero takes r3 and leaves r2 to the two constants; as a literal the registers trade places (18 differ) */

        targets->message = 298;
        targets->count = count;
        targets->cue = 0x120b6;
        targets->extra = zero;
    }
}

void Func_0811d9a4(void)
{
    u16 ids[14];
    s32 count = BattleParty_ListActorIds(3, ids);
    s32 i;

    for (i = 0; i < count; i++)
        ObjMotRst(ids[i]);
}
