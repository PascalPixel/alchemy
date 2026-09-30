#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
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

s32 SummonSlot_RegisterActorSprites(s32 unit)
{
    s32 pass;
    struct Layout *table = (struct Layout *)gBattleWork;
    struct BattleActorDefinition *actor = Owner_GetStateFar(unit);
    s32 single_slot = Summon_IsEntryFlagged(actor->class_id);
    s32 result = 0;
    s32 sprite_value = Summon_GetEntryValue(actor->class_id);

    /* FAKEMATCH: an opaque copy keeps cse from folding pass to the constant zero */
    asm("mov %0, %1" : "=r"(pass) : "r"(result));
    do {
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
            s32 buffer_addr = (slot << 14) + (s32)Ram_ActorSpriteSlots;

            /* FAKEMATCH: an empty use of sprite_value steers it into r7 as the ROM allocates it */
            asm("" : "+r"(sprite_value));
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
    } while (++pass <= 1);

    return result;
}
