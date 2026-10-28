// Draft, differ 106 (121 before statements reordered 2026-10-26).
// Near miss, differ 125 of 219 (146 before gMainRoutineWork's byte pointer was
// taken just before the coins, where the native loads it, 2026-10-25), written from the listing: takes
// over a The Broken Seal save (`save`, laid out as that game's: flag bits at
// 0x40, gMainRoutineWork at 0x240, the units at 0x520): its djinn flags and six
// flags from Data_080b1284 set or clear this game's flags from 0x800 and
// 0x820, flag 0x822 toggles, its coins go to 0x28c of gMainRoutineWork,
// unless `keepSettings` five settings bytes are copied; each of the four
// units is copied in keeping its first 15 bytes, items flagged 0x20 are
// removed, and the units recomputed; the djinn are set, flags
// 44 + (keepSettings & 1) and 47 set and the frame wait count restarted from
// the play time. The djinn flag loop is Func_080ae6e4's (counter k, empty asm
// on i, unsigned inner counter). Left: the native keeps the unit loop's bound
// 4 in sl with no entry test (count = 4 here is tested on entry, a constant
// bound keeps it in no register), the owner in r7 and the unit pointer in r8
// (swapped here); it keeps gMainRoutineWork in r4 through the settings copy (this
// reloads it), takes the djinn flag counter in sl and the base in r8
// (swapped), and orders the prologue's three pointers otherwise.
#include "types.h"
#include "string.h"
#include "main_routine_work.h"
#include "owner_state.h"
#include "item.h"
#include "iwram_call.h"
#include "frame.h"

s32 FlagBits_Test(u8 *bytes, s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_08038348(s32 a, s32 b);
s32 Inventory_Remove(s32 owner, s32 slot);
void Unit_Calculate(s32 owner);
void Djinn_ActivateOfferedForParty(void);
void Func_080ae16c(s32 mode);

struct SavedUnit {
  u8 bytes[0x14c];
};

extern const u16 Data_080b1284[6];

void Func_080ae410(u8 *save, s32 keepSettings) {
  u8 *flags = save + 0x40;
  u8 *party = save + 0x240;
  struct SavedUnit *units = (struct SavedUnit *)(save + 0x520);
  u8 *state;
  u8 name[16];
  s32 i;
  s32 k;
  s32 owner;
  s32 slot;
  s32 count;

  count = 4;
  for (i = 0, k = 0; i < 4; i++, k += 7) {
    u32 j;

    for (j = 0; j < 7; j++) {
      if (FlagBits_Test(flags, 0x30 + i * 20 + j))
        GameFlag_SetBit(0x800 + k + j);
      else
        GameFlag_ClearBit(0x800 + k + j);
    }
    /* FAKEMATCH: keeps i * 20 from being strength-reduced */
    asm("" : "+r"(i));
  }
  for (i = 0; i < 6; i++) {
    if (FlagBits_Test(flags, Data_080b1284[i]))
      GameFlag_SetBit(0x820 + i);
    else
      GameFlag_ClearBit(0x820 + i);
  }
  if (GameFlag_Test(0x822))
    GameFlag_ClearBit(0x822);
  else
    GameFlag_SetBit(0x822);

  state = (u8 *)&gMainRoutineWork;
  *(s32 *)(state + 0x28c) = *(s32 *)(party + 0x10);
  if (keepSettings == 0) {
    state[0x225] = party[0x225];
    state[0x226] = party[0x226];
    state[0x22c] = party[0x22c];
    state[0x22a] = party[0x22a];
    state[0x24a] = party[0x24a];
    Func_08038348(state[0x225], state[0x226]);
  }

  for (owner = 0; owner < count; owner++) {
    struct SavedUnit *unit = Unit_Get(owner);
    u16 *inventory;

    memcpy(name, &units[owner], 15);
    Iwram_CopyWords(unit, &units[owner], 0x14c);
    memcpy(unit, name, 15);
    if (keepSettings == 0)
      *(u16 *)&unit->bytes[0x14a] = unit->bytes[0x128];
    inventory = &((struct OwnerInventoryState *)unit)->inventory[14];
    for (slot = 14; slot >= 0; slot--) {
      s32 item = *inventory--;

      if (item != 0 && (Item_GetDirect(item)->flags & 0x20)) {
        while (Inventory_Remove(owner, slot) == 1)
          ;
      }
    }
    Unit_Calculate(owner);
  }
  Djinn_ActivateOfferedForParty();
  Func_080ae16c(1);
  GameFlag_SetBit((keepSettings & 1) + 44);
  GameFlag_SetBit(47);
  gFrameWaitCount = gMainRoutineWork.playTime;
}
