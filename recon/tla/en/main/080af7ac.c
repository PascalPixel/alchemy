// Near miss: differ 8 of 119, four reordered rows. The native loads the owner
// table before it opens the frame (ldr r1; sub sp, #32), reads the first owner
// before it builds -1 (ldr r0, [r3]; movs r1, #1), sets the clear loop counter
// before the Character_GetDefinition call (movs r5, #14; ldrh r0, [r2]; bl) and
// loads the halfword zero before it copies the record (ldr r2, =0; adds r3, r7,
// #0). Setting the counter before the call, in the same or in its own variable,
// swaps the first loop r5 and r6. Message 131 (the first character name) needs
// a name in the six catalogs: MsgCharacterName here is not defined yet.
#include "types.h"
#include "owner_state.h"
#include "party_state.h"

struct OwnerStart {
  u8 unk00[150];
  u8 level;
  u8 unk97;
  u16 items[13];
};

struct OwnerName {
  u8 name[14];
  u8 nameFlags;
  u8 unk0f[5];
  u16 hpRatio;
  u16 ppRatio;
};

extern u8 MsgCharacterName[];
extern s32 gData_080b2340[];

s32 Inventory_AddItem(s32 owner, s32 item);
void Inventory_Equip(s32 owner, s32 slot);
struct OwnerStart *Character_GetDefinition(s32 index);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Unit_Calculate(s32 owner);

void Party_InitCharacters(void) {
  struct OwnerInventoryState *state;
  struct OwnerStart *start;
  u16 nameBuf[16];
  s32 owner;
  s32 *remote = gData_080b2340;
  s32 i;
  s32 slot;
  u8 *name;

  for (owner = 0; owner <= 7; owner++) {
    state = Unit_Get(owner);
    Ui_AdjustValueWithoutLimitFar(owner + (s32)MsgCharacterName, nameBuf);
    name = ((struct OwnerName *)state)->name;
    name[0] = nameBuf[0];
    i = 0;
    if (nameBuf[0] != 0) {
      do {
        i++;
        if (i > 13)
          break;
        name[i] = nameBuf[i];
      } while (nameBuf[i] != 0);
    }
    ((struct OwnerName *)state)->nameFlags = 0;
  }

  if (*remote != -1) {
    do {
      state = Unit_Get(*remote);
      if (state != 0) {
        {
          s32 id = *remote;

          /* FAKEMATCH: the barrier keeps the id load ahead of the class field
           * offset; without it the offset is started first (one reordered row).
           */
          __asm__("");
          state->classId = id;
        }
        start = Character_GetDefinition(state->classId);
        for (i = 14; i >= 0; i--)
          state->inventory[i] = 0;

        for (i = 0; i < sizeof(start->items) / sizeof(start->items[0]); i++) {
          slot = Inventory_AddItem(*remote, start->items[i] & 0x1ff);
          Inventory_Equip(*remote, slot);
        }

        Unit_UpdateClass(*remote);
        ((struct OwnerName *)state)->ppRatio = 0x4000;
        ((struct OwnerName *)state)->hpRatio = 0x4000;
        Unit_LevelUpTo(*remote, start->level);
        Unit_UpdateClass(*remote);
        Unit_Calculate(*remote);
      }
      remote++;
    } while (*remote != -1);
  }
}
