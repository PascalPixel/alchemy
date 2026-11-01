// Draft, differ 415 of 423, skeleton gap 141 (tla-en, 2026-11-01): 67 removed,
// 74 added.
// Unpacks clear data, in the layout The Broken Seal's
// FileScreen_PackPasswordData packs (and Func_02001c7c here), into the four
// units' base stats, levels and Djinn, the story flags, the Psynergy items and
// the inventories with their counts. Control flow, calls and memory accesses
// are the native's.
// The native keeps code in its loops that -O2 moves out: every table address,
// 0x1ff, the packedStats address and m * 2 stay inside, the flag loop's head
// computes table, index and i + 1 once for both calls, yet the counters are
// strength-reduced. So the loops are written as gotos (no loop notes: loop.c
// leaves them alone) with the reductions spelled out by hand (offset, next,
// shift, bytes, slot). Only unpack and counts stay do loops, which raises their
// pseudos' reference weights and closes more of the gap. table, index and next
// are plain locals that exist for this; on adoption each needs a FAKEMATCH tag
// or a better form.
// What is left is mostly one thing: in stays on the stack (sp + 12, frame 48
// against 40), where the native has it in r10, so every use costs a reload and
// the count section's registers shift. Global ranks in behind j, n and table,
// which take r5 to r7 and the high registers. The native's order is j (r1),
// the slot offset (r0), the item (r4), then i r7, n r6, bitOffset r5; goto
// loops lose the loop depth that would weight those refs. One counter for every
// loop, as the native's r7 suggests, spills it to a high register (gap 200 and
// up): i is shared by the first loops, the clear, inventory and count loops
// have their own. Greedy goto or do choices per loop and counter merges found
// nothing further.
// Not expressible at -O2: GCSE's PRE moves m * 2 out of even a goto loop
// (spilled to sp + 4, the native recomputes it), i + 1 before the flag branch
// needs another temporary and is left out. By hand with -Os in place of -O2
// the for-loop form fell to 138 lines of register allocation, which points at
// the native's optimization level; that is a compiler question (K1 to K3).
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
  s32 clearUnit;
  s32 slotUnit;
  s32 countUnit;
  s32 inputIndex;
  s32 bitOffset;
  s32 j;
  s32 n;
  s32 m;
  s32 shift;
  u8 *bytes;
  u8 *base;
  s32 next;
  struct BattleUnit *levelMember;
  struct BattleUnit *clearMember;
  struct BattleUnit *slotMember;
  struct BattleUnit *countMember;

  levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  flagBits = in[7];
  keyItemBits = in[8];

  if (mode != 2) {
    // As a value: in + 8 + (mode != 0) becomes a conditional add.
    s32 statsOffset = 8 + (mode != 0);
    s32 offset = 4;

    i = 0;
    bytes = in + statsOffset;
    do {
      base = (u8 *)packedStats;
      packedStats[i * 4] =
          bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
      i++;
      *(u32 *)(base + offset) =
          bytes[4] << 24 | bytes[5] << 16 | bytes[6] << 8 | bytes[7];
      next = offset + 4;
      *(u32 *)(base + next) = bytes[7] << 28 | bytes[8] << 20 | bytes[9] << 12 |
                              bytes[10] << 4 | bytes[11] >> 4;
      next = offset + 8;
      *(u32 *)(base + next) =
          bytes[11] << 28 | bytes[12] << 20 | bytes[13] << 12 | bytes[14] << 4;
      offset += 16;
      bytes += 15;
    } while (i != 2);
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

  i = 0;
flags:
  {
    u8 *table = (u8 *)gData_02002490;
    s32 index = i * 2;

    if ((flagBits >> i) & 1)
      GameFlag_SetBit(*(u16 *)(table + index));
    else
      GameFlag_ClearBit(*(u16 *)(table + index));
  }
  i++;
  if (i != 6)
    goto flags;

  i = 0;
  shift = 0;
levels:
  levelMember = Unit_Get(gData_020026a0[i]);
  levelMember->level = levelBits >> shift & 0x7f;
#if LANGUAGE != LANGUAGE_JAPANESE
  levelMember->experience = Unit_GetExperienceForLevel(i, levelMember->level);
#endif
  i++;
  shift += 7;
  if (i != 4)
    goto levels;

  Djinn_GetRecoveryTable(0)->count = 0;
  clearUnit = 0;
clearDjinn:
  clearMember = Unit_Get(gData_020026a0[clearUnit]);
  // The native clears the same 40 bytes, djinnAvailable through
  // djinnActiveCounts, four times.
  n = 0;
clearDjinnFour:
  Iwram_ClearWordsEntry(clearMember->djinnAvailable, 40);
  n++;
  if (n != 4)
    goto clearDjinnFour;
  clearUnit++;
  if (clearUnit != 4)
    goto clearDjinn;

  for (i = 0; i != 28; i++)
    GameFlag_ClearBit(i / 7 * 20 + i % 7 + 48);

  for (i = 0; i != 28; i++) {
    if ((djinnBits >> i) & 1)
      Djinn_AddToLeastLoadedOwner(i / 7, i % 7);
  }

  if (mode != 0) {
    Unit_Get(gData_020026a0[0]);
    i = 0;
  keyItems:
    if ((keyItemBits >> i) & 1)
      PartyInventory_Add(gData_020026b0[i]);
    i++;
    if (i != 8)
      goto keyItems;
  }

  if (mode == 0) {
    u16 *slot;

    inputIndex = 39;
    bitOffset = 0;
    slotUnit = 0;
  inventories:
    slotMember = Unit_Get(gData_020026a0[slotUnit]);
    slot = slotMember->inventory;
    bytes = in + inputIndex;
    n = 0;
  inventorySlots:
    *slot = (bytes[0] << (bitOffset + 1) | bytes[1] >> (7 - bitOffset)) & 0x1ff;
    slot++;
    bitOffset++;
    bytes++;
    inputIndex++;
    if (bitOffset == 7) {
      bitOffset = 0;
      bytes++;
      inputIndex++;
    }
    n++;
    if (n != 15)
      goto inventorySlots;
    slotUnit++;
    if (slotUnit != 4)
      goto inventories;

    inputIndex = 107;
    bitOffset = -1;
    countUnit = 0;
    do {
      countMember = Unit_Get(gData_020026a0[countUnit]);
      m = 0;
      bytes = in + inputIndex;
    countItems:
      j = 0;
      if (bitOffset < 0) {
        j = bytes[0] << -bitOffset & 0x1f;
        bytes++;
        inputIndex++;
        bitOffset += 8;
      }
      j += bytes[0] >> bitOffset & 0x1f;
      bitOffset -= 5;
      if (bitOffset == -5) {
        bytes++;
        inputIndex++;
        bitOffset = 3;
      }
      j &= 0x1f;
      {
        u16 *table = gData_020026c0;

        n = 0;
      countSlots:
        {
          u16 item = countMember->inventory[n];

          if ((item & 0x1ff) == table[m])
            countMember->inventory[n] = j << 11 | item;
        }
        n++;
        if (n != 15)
          goto countSlots;
      }
      m++;
      if (m != 23)
        goto countItems;
      countUnit++;
    } while (countUnit != 4);
    bytes = in + 165;
    gMainRoutineWork.coins = bytes[0] << 16 | bytes[1] << 8 | in[167];
  }

  i = 0;
calculate:
  Unit_UpdateClass(gData_020026a0[i]);
  Unit_Calculate(gData_020026a0[i]);
  i++;
  if (i != 4)
    goto calculate;
}
