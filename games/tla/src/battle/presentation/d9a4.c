#include "types.h"
#include "battle_party.h"
#include "iwram_call.h"

s32  BattleParty_ListActorIds();
void ObjMotRst();

struct ActionTargets
{
    u8  side;
    u8  count;
    u8  unk02;
    u8  ids[14];
    u8  stateA[14];
    u8  stateB[14];
    u8  stateC[14];
    u8  stateD[14];
    u16 value4a;
    s32 message;
    s32 value50;
    s32 value54;
    s32 cue;
    s32 unk5c;
    s32 extra;
    s32 unk64;
};

s32 BattleParty_PrepareReserveOwners(u16 *unit_ids);

s32 Func_0811a39c(s32 side, u16 *unit_ids);
s32 Func_0811f4d4();
s32 Battle_GetTaggedSlotValue();

void Func_0811d7e8(u16 *source, struct ActionTargets *targets)
{
    u16 ids[10];
    s32 count;
    s32 i;
    s32 side = 2;
    s32 own;

    if (source[0] > 7)
        side = 1;
    count = Func_0811a39c(side, ids);
    Iwram_ClearWords(targets, sizeof *targets);
    own = 0;
    if (source[0] <= 7)
        own = 1;
    own = Battle_GetTaggedSlotValue(Func_0811f4d4(own));
    targets->value50 = 2;
    targets->value54 = 14;
    {
        s32  half = source[3];
        u16 *value;

        /* FAKEMATCH: a one-pass block keeps the halfword read ahead of the field address; without it the address is begun before the read (2 differ). */
        do
        {
        } while (0);
        value = &targets->value4a;
        targets->side = own;
        *value = half;
    }
    for (i = 0; i < count; i++)
    {
        targets->ids[i] = ids[i];
        targets->stateA[i] = 0;
        targets->stateB[i] = 1;
        targets->stateC[i] = 0;
        targets->stateD[i] = 1;
    }
    targets->count = count;
    targets->message = 403;
    targets->cue = 0x801a;
}

void Func_0811d89c(struct ActionTargets *targets)
{
    u16 ids[10];
    s32 count;
    s32 i;
    s32 side = 1;

    if (targets->side > 7)
        side = 2;
    count = Func_0811a39c(side, ids);
    for (i = 0; i < count; i++)
    {
        targets->ids[i] = ids[i];
        targets->stateA[i] = 0;
        targets->stateB[i] = 1;
        targets->stateC[i] = 0;
        targets->stateD[i] = 1;
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

    if (targets->side <= 7)
    {
        count = BattleParty_PrepareActiveOwners(ids);
        count += BattleParty_PrepareReserveOwners(&ids[count]);
    }
    else
    {
        count = BattleParty_ListActorIds(2, ids);
    }
    for (i = 0; i < count; i++)
    {
        targets->ids[i] = ids[i];
        targets->stateA[i] = 0;
        targets->stateB[i] = 1;
        targets->stateC[i] = 0;
        targets->stateD[i] = 1;
    }
    {
        /* FAKEMATCH: a named zero takes r3 and leaves r2 to the two constants; as a literal the registers trade places (18 differ) */
        s32 zero = 0;

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
