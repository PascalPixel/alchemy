#include "types.h"
#include "ram_buffer.h"

struct SerialWork {
    u8 unknown_0000[0x313c];
    u32 command;
    u16 progress;
    u16 state;
};

void SerialRuntime_QueueCommand(u32 command)
{
    struct SerialWork *work = Ram_HeapSlots->serial_work;

    if (work == 0)
        return;
    work->command = command;
    work->progress = 0;
    work->state = 3;
}
