#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct GateEventWork {
  u8 unk000[362];
  u16 gateTimer;
};

extern const u8 gData_02005d7c[];
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020018ec(s32 open);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_02005428(const u8 *data, s32 a, s32 b);

void Func_02001990(s32 a, s32 b) {
  Func_02005428(gData_02005d7c, a, b);
  if (GameFlag_Test(0x8ce) && GameFlag_Test(0x8cf))
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 200;
}
