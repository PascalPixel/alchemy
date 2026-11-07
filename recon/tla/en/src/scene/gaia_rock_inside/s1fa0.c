#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "ram_buffer.h"
s32 Actor_GetById();
s32 GameFlag_ClearBit();
s32 Func_02004f8c();

struct GaiaRockInsideEventWork {
  u8 unk000[0x15a];
  u16 unk15a;
};

extern const u8 MsgCheckedTheAltar[];

void FieldMessage_Show(const u8 *message, s32 position);
s32 PartyInventory_FindOwner(s32 item);

void Func_02001fa0(void) {
  s32 t0;

  t0 = Actor_GetById(66);
  FIELD_AT(t0, s32, 8) = 0x2280000;
  FIELD_AT(t0, s32, 16) = 0x1280000;
  FIELD_AT(t0, s32, 12) = -1966080;
  FIELD_AT(t0, s32, 20) = -2097152;
  GameFlag_ClearBit(0x142);
  Func_02004f8c(0x1042);
}

/// Examines the altar.
void Func_02001fd8(void) {
  const u8 *message = MsgCheckedTheAltar;
  const u8 *next;
  s32 owner;

  FieldMessage_Show(message, 1);
  owner = PartyInventory_FindOwner(451);
  next = message + 2;
  if (owner != -1) {
    struct GaiaRockInsideEventWork *work = Ram_HeapSlots->eventWork;


    FieldMessage_Show(next, 1);
    work->unk15a = 1;
  } else {
    FieldMessage_Show(next, 1);
  }
}
