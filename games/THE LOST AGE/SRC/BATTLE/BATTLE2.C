#include "TYPES.H"
#include "OWNER_STATE.H"

void Owner_RecalculateStatsFar(u16 id);
s32 BattleParty_ListActorIds(s32 groups, u16 *ids);

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

        actor = (struct ActorState_080b90ac *)Owner_GetState(ids[index]);
        actor->field_12b = 0;
        Owner_RecalculateStatsFar(ids[index]);
    }
}
