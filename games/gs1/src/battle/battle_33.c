#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* battle/actor/commit_placement.c */
void BattleActor_CommitPlacement(void)
{
    u8 actor_slots[28];

    Actor_Apply(3, actor_slots);
    Actor_Apply2(actor_slots, 1);
}

/* battle/party/list_all_units_and_submit.c */
void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    Battle_Apply(3, local);
    Battle_Apply2(local, 0);
}

/* battle/motion/get_slot_field_14.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *GetBattleObjectSlot();

s32 BattleMotion_GetSlotField14(void)
{
    return FIELD_AT_OFFSET(GetBattleObjectSlot(), s32 *, 0x14);
}

/* battle/summon/class_is_valid.c */
struct Layout {
    u8 pad[4];
    s16 field[6];
};

s32 Summon_ClassValid(s32 arg0)
{
    struct Layout *ptr;
    s32 retval;
    s32 i;

    retval = Battle_Check(arg0);
    ptr = *(struct Layout **)ADDR_03001E74;
    for (i = 0; i <= 5; i++) {
        if (ptr->field[i] != 0)
            continue;
        if (retval != 0)
            break;
        if (i <= 4 && ptr->field[i + 1] == 0)
            break;
    }
    return i != 6;
}
