#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_ESCAPE.H"
s32 Battle_CollectPartyCommandsFar(void *entries, u16 *excluded_units, s32 excluded_count);
void Runtime_BumpFree(void *ptr);
extern u8 Data_03001e74[];
s32 BattleParty_ListActorIds(s32 groups, u16 *ids);

/* battle/actor/clear_field_12b_for_group.c */
u8 *Owner_GetStateFar(s32);
void Owner_RecalculateStatsFar(u16 id);

struct ActorState_080b90ac {
    u8 padding_000[0x12b];
    u8 field_12b;
};

void BattleUnit_ClearField12bForGroup(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = BattleParty_ListActorIds(3, ids);
    for (index = 0; index < count; index++) {
        struct ActorState_080b90ac *actor;

        actor = (struct ActorState_080b90ac *)Owner_GetStateFar(ids[index]);
        actor->field_12b = 0;
        Owner_RecalculateStatsFar(ids[index]);
    }
}
