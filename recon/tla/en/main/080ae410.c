// Draft, differ 44 of 219, skeleton gap 8 (tla-en, 2026-11-01)
// Takes over a The Broken Seal save (`save`, laid out as that game's: flag bits
// at 0x40, its gMainRoutineWork at 0x240, the units at 0x520): its djinn flags
// and six flags from Data_080b1284 set or clear this game's flags from 0x800
// and 0x820, flag 0x822 toggles, its coins go to unk284[8] and, unless
// `keepSettings`, its options are copied; each of the four units is copied in
// keeping its first 15 bytes, items flagged 0x20 are removed and the units
// recomputed; the djinn are set, flags 44 + (keepSettings & 1) and 47 set and
// the frame wait count restarted from the play time.
// The empty asm on i is gone: i * 20 now comes out as the listing's with one
// i across all three loops. MainRoutineWork through a local `state`,
// BattleUnit fields and unit->inventory[slot] closed the settings copy and the
// unit loop.
// Left, all one cause: the listing's sl is one pseudo with a long life, the
// inner counter in the first loop and the unit loop's bound 4 afterwards (it
// outranks r8's neighbours; a set-once count has a constant equivalence, ranks
// last in .18.greg and is rematerialized as cmp #4, and 0x14c takes its
// register). Declaring `u32 j` at function scope, `j = 4` before the unit loop
// and `i < (s32)j` gives differ 8, skeleton gap 3: only the bls below and two
// scheduling places in the first loop's preheader. That is a local reused for
// a second value plus a cast, so it is not kept here; S2 would tag it
// `temporary`, which replaces nothing in this draft.
// The native's inner counter is signed (ble) and loop.c's second pass reverses
// a signed one once its givs are reduced ("Reversed loop" in the .08.loop dump),
// hence u32 and bls. do/while, for (;;) with break, ++j > 6, j++ in both arms,
// hand-stepped flag numbers with a for and with a goto (the goto reduces i * 20)
// all failed, as did a `// Default:` count, count declared first, and
// i < 4. Func_080ae6e4 keeps ble only with a tagged barrier; 080ae868 has the same bls.
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

  for (i = 0, k = 0; i < 4; i++, k += 7) {
    u32 j;

    for (j = 0; j < 7; j++) {
      if (FlagBits_Test(flags, 0x30 + i * 20 + j))
        GameFlag_SetBit(0x800 + k + j);
      else
        GameFlag_ClearBit(0x800 + k + j);
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
    Func_08038348(state->paletteGlow[0],
                  state->paletteGlow[1]);
  }

  count = 4;
  for (i = 0; i < count; i++) {
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
