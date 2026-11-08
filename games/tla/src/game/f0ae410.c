/// @file
/// Takes over a The Broken Seal save.

#include "types.h"
#include "string.h"
#include "main_routine_work.h"
#include "battle_unit.h"
#include "item.h"
#include "iwram_call.h"
#include "frame.h"

s32 FlagBits_Test(u8 *bytes, s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_08038348(s32 a, s32 b);
void *Unit_Get(s32 owner);
s32 Inventory_Remove(s32 owner, s32 slot);
void Unit_Calculate(s32 owner);
void Djinn_ActivateOfferedForParty(void);
void Func_080ae16c(s32 mode);

extern const u16 Data_080b1284[6];

/// Copies a The Broken Seal save's djinn, flags, coins, options (unless
/// keepSettings) and units into this game's state.
void Func_080ae410(u8 *save, s32 keepSettings) {
  u8 *flags = save + 0x40;
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  struct MainRoutineWork *state;
  u8 name[16];
  s32 i;
  s32 k;
  s32 slot;
  s32 count;
  s32 j;

  for (i = 0, k = 0; i < 4; i++, k += 7) {

    for (j = 0; j < 7; j++) {
      s32 flag = 0x800 + k + j;

      if (FlagBits_Test(flags, 0x30 + i * 20 + j))
        GameFlag_SetBit(flag);
      else
        GameFlag_ClearBit(flag);
      /* FAKEMATCH(barrier): an empty asm reading j, which keeps the counter
       * the native's one long-lived register; without it, 105 of 219
       * instructions differ in tla-en. */
      asm("" : : "r"(j));
    }
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

  state = &gMainRoutineWork;
  *(s32 *)&state->unk284[8] = party->coins;
  if (keepSettings == 0) {
    state->paletteGlow[0] = party->paletteGlow[0];
    state->paletteGlow[1] = party->paletteGlow[1];
    state->eventMode = party->eventMode;
    state->battleFxEnabled = party->battleFxEnabled;
    state->unk244[6] = party->unk244[6];
    Func_08038348(state->paletteGlow[0], state->paletteGlow[1]);
  }

  j = 4; // A local reused for a second value: the unit loop's bound.
  for (i = 0; i < (s32)j; i++) {
    struct BattleUnit *unit = Unit_Get(i);

    memcpy(name, &units[i], 15);
    Iwram_CopyWords(unit, &units[i], sizeof(struct BattleUnit));
    memcpy(unit, name, 15);
    if (keepSettings == 0)
      unit->enemyId = unit->classId;
    for (slot = 14; slot >= 0; slot--) {
      s32 item = unit->inventory[slot];

      if (item != 0 && (Item_GetDirect(item)->flags & 0x20)) {
        while (Inventory_Remove(i, slot) == 1)
          ;
      }
    }
    Unit_Calculate(i);
  }
  Djinn_ActivateOfferedForParty();
  Func_080ae16c(1);
  GameFlag_SetBit((keepSettings & 1) + 44);
  GameFlag_SetBit(47);
  gFrameWaitCount = gMainRoutineWork.playTime;
}
