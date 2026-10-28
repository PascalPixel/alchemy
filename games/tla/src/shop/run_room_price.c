#include "types.h"
#include "ram_buffer.h"

/// The inn's view of the shop work (heap slot 220).
struct InnRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[8];
  u8 unk4da[0x2a];
  s8 partyMemberCount;
  u8 unk505;
  u8 busy;
};

struct InnUnit {
  u8 unk00[0x38];
  s16 hp;
};

struct InnUnit *Unit_Get(s32 unitId);
void Func_08108148(void);
void Func_0810824c(void);

/// The price per guest of each inn.
extern const s8 gData_0810cf2c[];

/// Multiplies the inn's price per guest by the number of living party members.
// As ☀️'s Inn_RoomPrice.
s32 Inn_RoomPrice(s32 innId) {
  struct InnRuntime *inn;
  s32 guestCount;
  s32 pricePerGuest;
  s32 memberIndex;

  inn = Ram_HeapSlots->menuRuntime;
  guestCount = 0;
  pricePerGuest = gData_0810cf2c[innId];
  for (memberIndex = 0; memberIndex < inn->partyMemberCount; memberIndex++) {
    if (Unit_Get(inn->partyMemberIds[memberIndex])->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

s32 Func_0810b3ec(s32 value) {
  struct InnRuntime *work;

  Func_08108148();
  work = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH: ⚓️ loads the menu work before it builds the flag offset */
  asm volatile("");
  work->busy = 1;
  value = Inn_RoomPrice(value);
  Func_0810824c();
  return value;
}
