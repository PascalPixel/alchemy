#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "ram_buffer.h"
s32 Object_GetById();
s32 GameFlag_ClearBit();
s32 Func_02004f8c();

struct GaiaRockInsideEventWork {
  u8 unk000[0x15a];
  u16 unk15a;
};

extern const u8 MsgCheckedTheAltar[];

void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 position);
s32 PartyInventory_FindOwner(s32 item);

void Func_02001fa0(void) {
  s32 t0;

  t0 = Value1(Object_GetById, 0x42);
  FIELD_AT(t0, s32, 8) = 0x2280000;
  FIELD_AT(t0, s32, 16) = 0x1280000;
  FIELD_AT(t0, s32, 12) = -1966080;
  FIELD_AT(t0, s32, 20) = -2097152;
  Call1(GameFlag_ClearBit, 0x142);
  Call1(Func_02004f8c, 0x1042);
}

/// Examine the altar.
void Func_02001fd8(void) {
  const u8 *message = MsgCheckedTheAltar;
  const u8 *next;
  s32 owner;

  UiText_ShowPositionedMessageAndWait(message, 1);
  owner = PartyInventory_FindOwner(451);
  next = message + 2;
  if (owner != -1) {
    struct GaiaRockInsideEventWork *work = Ram_HeapSlots->eventWork;

    /* FAKEMATCH: as overlay 680's Func_02000314, a one-pass block keeps the
     * work pointer load before the message arguments (one reordered
     * instruction, differ 2). */
    do {
    } while (0);
    UiText_ShowPositionedMessageAndWait(next, 1);
    work->unk15a = 1;
  } else {
    UiText_ShowPositionedMessageAndWait(next, 1);
  }
}
