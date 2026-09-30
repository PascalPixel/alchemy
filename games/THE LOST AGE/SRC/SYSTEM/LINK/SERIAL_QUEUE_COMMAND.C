#include "TYPES.H"
#include "RAM_BUFFER.H"

struct SerialWork {
    u8 unknown_0000[0x313c];
    u32 command;
    u16 progress;
    u16 state;
};

/* Queues a command word for the serial link: clears its progress and puts
   the exchange in state 3. Without a serial runtime it does nothing. */
void SerialRuntime_QueueCommand(u32 command)
{
    struct SerialWork *work = Ram_HeapSlots->serial_work;

    if (work == 0)
        return;
    work->command = command;
    work->progress = 0;
    work->state = 3;
}
