#include "types.h"
#include "ram_buffer.h"

struct SerialCommandWork {
  u8 unk0000[0x313c];
  u32 command;
  u16 progress;
  u16 state;
};

void Serial_QueueCommand(u32 command) {
  struct SerialCommandWork *work = Ram_HeapSlots->serialWork;

  if (work == 0)
    return;
  work->command = command;
  work->progress = 0;
  work->state = 3;
}
