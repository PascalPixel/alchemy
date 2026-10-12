#include "types.h"
#include "btlwork.h"

void BtlEvPush(u32 opcode, u32 operand)
{
    struct BattleEventQueue *queue = &Ram_HeapSlots->battle_work->events.queue;
    u32 index = queue->count;

    queue->opcodes[index] = opcode;
    queue->operands[index] = operand;
    queue->count = index + 1;
}
