#include "battle_event.h"
#include "global_cells.h"

u32 Func_080bbabc(u32 opcode, u32 operand)
{
    u8 *base = *(u8 **)ADDR_03001E74;
    struct BattleEventQueue *queue =
        (struct BattleEventQueue *)(base + 1720);
    u32 *count = (u32 *)&queue->count;
    u32 index = *count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    *count = index + 1;
    return opcode;
}
