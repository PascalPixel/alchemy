#include "types.h"
#include "battle_runtime.h"
#include "battle_types.h"
#include "battle_work.h"

#define BattleActor_ClearIndexedMaskBit Func_080c1ebc

void BattleActor_ClearIndexedMaskBit(s32 actor_id)
{
    struct BattleUnit *unit;
    u8 *battle;
    u8 class_id;
    s32 actor_count;
    s32 actor_index;
    s32 text_length;
    s32 bit_index;

    battle = BattleWorkPtr;
    actor_count = battle[0x40];
    unit = BattleUnit_Get(actor_id);

    if (unit->class_bonus_disabled != 0)
        return;

    class_id = unit->class_id;
    actor_index = 0;
    while (actor_index < actor_count
           && *(u16 *)(battle + 0x10 + actor_index * 2)
                  != class_id) {
        actor_index++;
    }

    if (actor_index == actor_count)
        return;

    actor_index *= 4;
    if (*(u32 *)(battle + 0x1c + actor_index) == 0)
        return;

    text_length = 0;
    if (((u8 *)unit)[0] != 0) {
        do {
            text_length++;
            if (text_length > 13)
                break;
        } while (((u8 *)unit)[text_length] != 0);
    }

    bit_index = 32;
    if (text_length > 0)
        bit_index = ((u8 *)unit)[text_length - 1] - '1';

    *(u32 *)(battle + 0x1c + actor_index) &= ~(1u << bit_index);
}
