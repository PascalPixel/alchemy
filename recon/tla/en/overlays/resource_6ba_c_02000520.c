// Near miss, differ 32 of 92: the same instructions; in both inventory loops
// the native keeps the slot index in r5 and the strength-reduced slot offset in
// r6, this form the other way round. Reusing the index for the stored 1, or a
// slot pointer, measured worse (58, 54).
#include "types.h"
#include "owner_state.h"

extern const u8 MsgTheBodyIs[];

void FieldMessage_Show(const u8 *message, s32 mode);
s32 Unit_AddHp(s32 owner, s32 amount);
void Unit_AddPp(s32 owner, s32 amount);
void Inventory_Break(s32 owner, s32 slot);
u8 *Func_02000d48(s32 item);
void Func_02000dc0(void);

static __inline__ void AddHp(s32 owner, s32 amount) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline keeps each
   * call's own constant; direct calls share -100 in one register. */
  Unit_AddHp(owner, amount);
}

void Func_02000520(void) {
  u8 *unit;
  u8 *item;
  s32 i;

  FieldMessage_Show(MsgTheBodyIs, 1);
  AddHp(4, -100);
  AddHp(5, -100);
  AddHp(6, -33);
  Unit_AddPp(4, -50);
  Unit_AddPp(5, -40);
  Unit_AddPp(6, -35);
  unit = Unit_Get(4);
  unit[0x131] = 1;
  unit[0x140] = 1;
  unit = Unit_Get(5);
  unit[0x130] = 1;
  unit[0x131] = 2;
  unit = Unit_Get(4);
  for (i = 0; i <= 14; i++) {
    item = Func_02000d48(((struct OwnerInventoryState *)unit)->inventory[i]);
    if (((struct OwnerInventoryState *)unit)->inventory[i] == 0)
      break;
    if (item[12] == 2) {
      Inventory_Break(4, i);
      break;
    }
  }
  unit = Unit_Get(5);
  for (i = 0; i <= 14; i++) {
    item = Func_02000d48(((struct OwnerInventoryState *)unit)->inventory[i]);
    if (((struct OwnerInventoryState *)unit)->inventory[i] == 0)
      break;
    if (item[12] == 2) {
      Inventory_Break(5, i);
      break;
    }
  }
  Func_02000dc0();
}
