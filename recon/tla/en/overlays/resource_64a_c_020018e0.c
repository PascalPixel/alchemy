// Near miss, differ 496 of 423 (tla-en, 2026-10-31; tla-ja 516 of 413):
// unpacks clear data, in the layout The Broken Seal's
// FileScreen_PackPasswordData packs (and Func_02001c7c here), into the four
// units' base stats, levels and Djinn, the story flags, the Psynergy items and
// the inventories with their counts. The control flow, calls and memory
// accesses are the native's; what is left is register allocation and where
// invariants sit. The native rebuilds every table address and constant inside
// its loops (gData_*, 0x1ff, the packedStats frame address, IwramClearWords),
// yet strength-reduces their counters (the stats offsets, the level shift,
// the inventory pointers); it keeps in in r10, i in r7 and the flag bits in
// r6. At -O2, loop.c hoists those invariants out of any C loop and in gets no
// register (sp + 4). The flag loop's head also computes the table address and
// i + 1 once for both calls, which -O2 does in each arm. With every loop
// written as a for loop this C differs 526; compiled by hand with -Os in place
// of -O2 (the other options kept), a line diff against the listing falls from
// 484 to 138 lines, all register allocation (the flag bits on the stack, a
// 44-byte frame).
#include "types.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "battle/battle_unit.h"
#include "owner_state.h"

extern s32 gData_020026a0[];
extern u16 gData_020026b0[];
extern u16 gData_020026c0[];
extern u16 gData_02002490[];

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Unit_GetExperienceForLevel(s32 id, s32 level);
void PartyInventory_Add(s32 item);
void Unit_Calculate(s32 id);

void Func_020018e0(s32 mode, u8 *in) {
  u32 levelBits;
  u32 djinnBits;
  u8 flagBits;
  u8 keyItemBits;
  u32 packedStats[8];
  s32 i;
  s32 inputIndex;
  s32 bitOffset;
  s32 j;
  s32 n;

  levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  flagBits = in[7];
  keyItemBits = in[8];

  if (mode != 2) {
    // As a value: in + 8 + (mode != 0) becomes a conditional add.
    s32 statsOffset = 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      packedStats[i * 4] = in[statsOffset] << 24 | in[statsOffset + 1] << 16 |
                           in[statsOffset + 2] << 8 | in[statsOffset + 3];
      packedStats[i * 4 + 1] = in[statsOffset + 4] << 24 |
                               in[statsOffset + 5] << 16 |
                               in[statsOffset + 6] << 8 | in[statsOffset + 7];
      packedStats[i * 4 + 2] =
          in[statsOffset + 7] << 28 | in[statsOffset + 8] << 20 |
          in[statsOffset + 9] << 12 | in[statsOffset + 10] << 4 |
          in[statsOffset + 11] >> 4;
      packedStats[i * 4 + 3] =
          in[statsOffset + 11] << 28 | in[statsOffset + 12] << 20 |
          in[statsOffset + 13] << 12 | in[statsOffset + 14] << 4;
      statsOffset += 15;
    }
    // A goto loop: as a for loop, loop.c moves the table's address and 0x1ff
    // out of it and steps a pointer through packedStats (differ 514).
    i = 0;
  stats:
    {
      // baseHp, basePp, then baseAttack to baseAgility at 4 to 6 and baseLuck
      // at byte 14: the native addresses them from baseHp.
      s16 *stats = &((struct BattleUnit *)Unit_Get(gData_020026a0[i]))->baseHp;

      stats[0] = packedStats[i * 2] >> 21;
      stats[4] = packedStats[i * 2] & 0x1ff;
      stats[1] = packedStats[i * 2] << 11 >> 21;
      stats[5] = packedStats[i * 2 + 1] >> 22 & 0x1ff;
      stats[6] = packedStats[i * 2 + 1] >> 12 & 0x1ff;
      ((u8 *)stats)[14] = packedStats[i * 2 + 1] >> 4 & 0x7f;
    }
    i++;
    if (i != 4)
      goto stats;
  }

  for (i = 0; i != 6; i++) {
    if ((flagBits >> i) & 1)
      GameFlag_SetBit(gData_02002490[i]);
    else
      GameFlag_ClearBit(gData_02002490[i]);
  }

  for (i = 0; i != 4; i++) {
    struct BattleUnit *member = Unit_Get(gData_020026a0[i]);

    member->level = levelBits >> (i * 7) & 0x7f;
#if LANGUAGE != LANGUAGE_JAPANESE
    member->experience = Unit_GetExperienceForLevel(i, member->level);
#endif
  }

  Djinn_GetRecoveryTable(0)->count = 0;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *member = Unit_Get(gData_020026a0[i]);

    // The native clears the same 40 bytes, djinnAvailable through
    // djinnActiveCounts, four times.
    for (n = 0; n != 4; n++)
      Iwram_ClearWordsEntry(member->djinnAvailable, 40);
  }

  for (i = 0; i != 28; i++)
    GameFlag_ClearBit(i / 7 * 20 + i % 7 + 48);

  for (i = 0; i != 28; i++) {
    if ((djinnBits >> i) & 1)
      Djinn_AddToLeastLoadedOwner(i / 7, i % 7);
  }

  if (mode != 0) {
    Unit_Get(gData_020026a0[0]);
    // A goto loop: as a for loop, loop.c steps a pointer through
    // gData_020026b0 (differ 504).
    i = 0;
  keyItems:
    if ((keyItemBits >> i) & 1)
      PartyInventory_Add(gData_020026b0[i]);
    i++;
    if (i != 8)
      goto keyItems;
  }

  if (mode == 0) {
    inputIndex = 39;
    bitOffset = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *member = Unit_Get(gData_020026a0[i]);

      for (n = 0; n != 15; n++) {
        member->inventory[n] = (in[inputIndex] << (bitOffset + 1) |
                                in[inputIndex + 1] >> (7 - bitOffset)) &
                               0x1ff;
        bitOffset++;
        inputIndex++;
        if (bitOffset == 7) {
          bitOffset = 0;
          inputIndex++;
        }
      }
    }

    inputIndex = 107;
    bitOffset = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *member = Unit_Get(gData_020026a0[i]);
      s32 m;

      for (m = 0; m != 23; m++) {
        j = 0;
        if (bitOffset < 0) {
          j = in[inputIndex] << -bitOffset & 0x1f;
          inputIndex++;
          bitOffset += 8;
        }
        j += in[inputIndex] >> bitOffset & 0x1f;
        bitOffset -= 5;
        if (bitOffset == -5) {
          inputIndex++;
          bitOffset = 3;
        }
        j &= 0x1f;
        for (n = 0; n != 15; n++) {
          if ((member->inventory[n] & 0x1ff) == gData_020026c0[m])
            member->inventory[n] = j << 11 | member->inventory[n];
        }
      }
    }
    gMainRoutineWork.coins = in[165] << 16 | in[166] << 8 | in[167];
  }

  // A goto loop: as a for loop, loop.c moves the table's address out of it
  // and steps its index by 4 (differ 500).
  i = 0;
calculate:
  Unit_UpdateClass(gData_020026a0[i]);
  Unit_Calculate(gData_020026a0[i]);
  i++;
  if (i != 4)
    goto calculate;
}
