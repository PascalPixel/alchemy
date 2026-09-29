/* 2026-09-29 alchemy permute: score 200 on the permuter's scorer (1
   inserted, 1 deleted), unchanged after 56,705 candidates in 10 minutes.
   The cse dump confirms the cause in the header: cse folds pass = result
   to the constant (a constant costs 0 against 1 for a pseudo, so it is not
   a tie), and the post-reload cselib pass does not turn it
   back into the copy (a high-to-low move costs 4). A do-while with the
   counter tested at the bottom and a chained initialisation of both give
   the same or worse. */
/* Draft, not exact (2026-09-24, names updated 2026-09-28): 256 of 256 bytes,
   1 differing halfword. The reference copies the zero result into the pass
   counter (mov r4, sl) where this spelling materialises movs #0: cse.c picks
   the constant on a cost tie, so the copy survives only where CSE cannot
   see the zero. Initialising the pair in either order or as one chained
   assignment gives 6 halfwords; BattlePres_RunEncounterOrUnitTrigger
   (080b9dc4) has the same residual. */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 *gBattleWork;


struct BattleActorDefinition {
    u8 reserved_000[296];
    u8 class_id;
    u8 unavailable;
};

struct Layout {
    u8 pad[4];
    s16 field[6];
};

struct BattleActorDefinition *Owner_GetStateFar(s32 actor_id);
s32 Summon_IsEntryFlagged(s32 class_id);
s32 Summon_GetEntryValue(s32 class_id);
s32 Summon_GetEntryFlag1Field(s32 class_id);
s32 ResourceSlot_LoadFar(s32 slot, s32 buffer_addr, s32 value, s32 flag);

#define SLOT_BUFFER_BASE 0x02018000

s32 SummonSlot_RegisterActorSprites(s32 unit)
{
    struct Layout *table = (struct Layout *)gBattleWork;
    struct BattleActorDefinition *actor = Owner_GetStateFar(unit);
    s32 single_slot = Summon_IsEntryFlagged(actor->class_id);
    s32 result = 0;
    s32 sprite_value = Summon_GetEntryValue(actor->class_id);
    s32 pass;

    for (pass = result; pass <= 1; pass++) {
        s32 slot;

        if (actor->unavailable != 0)
            continue;

        for (slot = 0; slot <= 5; slot++) {
            if (table->field[slot] != 0)
                continue;
            if (single_slot)
                break;
            if (slot <= 4 && table->field[slot + 1] == 0)
                break;
        }

        if (slot == 6)
            break;

        {
            s32 flag = Summon_GetEntryFlag1Field(actor->class_id);
            s32 buffer_addr = (slot << 14) + SLOT_BUFFER_BASE;

            if (ResourceSlot_LoadFar(slot, buffer_addr, sprite_value + pass, flag) == 0)
                return 0;
        }

        if (pass == 0)
            result = (slot << 12) | sprite_value;

        table->field[slot] = (s16)unit;
        if (!single_slot)
            table->field[slot + 1] = (s16)unit;

        if (sprite_value != 476 && sprite_value != 483)
            break;
    }

    return result;
}
