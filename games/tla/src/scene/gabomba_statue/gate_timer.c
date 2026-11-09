#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

struct GateEventWork {
  u8 unk000[362];
  u16 gateTimer;
};

extern const u8 gData_02005d7c[];
void Func_020018ec(s32 open);
void Func_02005428(const u8 *data, s32 a, s32 b);

void Func_02001990(s32 a, s32 b) {
  Func_02005428(gData_02005d7c, a, b);
  if (GameFlag_Test(0x8ce) && GameFlag_Test(0x8cf))
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 200;
}
