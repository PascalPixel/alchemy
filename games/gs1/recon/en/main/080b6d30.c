#include "types.h"
#include "global_cells.h"

struct BattleActorDefinition {
    u8 reserved_000[296];
    u8 class_id;
    u8 unavailable;
};

struct Layout {
    u8 pad[4];
    s16 field[6];
};

struct BattleActorDefinition *Func_08077008(s32 actor_id);
s32 Func_080c23c0(s32 class_id);
s32 Func_080c2384(s32 class_id);
s32 Func_080c23a0(s32 class_id);
s32 Func_08009270(s32 slot, s32 buffer_addr, s32 value, s32 flag);

#define SLOT_BUFFER_BASE 0x02018000

s32 Func_080b6d30(s32 unit)
{
    struct Layout *table = *(struct Layout **)ADDR_03001E74;
    struct BattleActorDefinition *actor = Func_08077008(unit);
    s32 single_slot = Func_080c23c0(actor->class_id);
    s32 result = 0;
    s32 sprite_value = Func_080c2384(actor->class_id);
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
            s32 flag = Func_080c23a0(actor->class_id);
            s32 buffer_addr = (slot << 14) + SLOT_BUFFER_BASE;

            if (Func_08009270(slot, buffer_addr, sprite_value + pass, flag) == 0)
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
