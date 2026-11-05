// Draft, differ 210 of 423, skeleton gap 62 (tla-en, 2026-11-01): 30 removed,
// 32 added.
// Unpacks clear data, in the layout The Broken Seal's
// FileScreen_PackPasswordData packs (and Func_02001c7c here), into the four
// units' base stats, levels and Djinn, the story flags, the Psynergy items and
// the inventories with their counts. Control flow, calls and memory accesses
// are the native's, except as listed below.
// Loops: every loop but the two 28-count Djinn loops is a goto, so that loop.c
// leaves constants in them, with the native's strength reductions spelled out
// (offset, second, third, bytes, cursor, slot, slotOffset). Re-measured on
// 1 November with no gain from plain forms: the level field store (differ
// 248), no following (229), gData_02002490[i] in the flag loop (234), no
// second and third (240), packedStats[i * 4 - 3] (305), real for loops for the
// slots (304 to 345). Each of these is still needed at this differ.
// Gained on 1 November: gData_020026c0[m] with m stepping to 23 and no table
// local, as the native counts (differ 248 to 212), plain in[165] to in[167]
// for the coins, item | j << 11.
// Left, in order of size:
// - Allocation of the count and inventory sections, which rotates registers
//   through the whole function. Here: in r11, mode r9, djinnBits r10, slotUnit
//   r10, bitOffset r7, j r5, slotOffset r4, cursor r0, countMember ip, item
//   spilled to r11 or sp+0. Native: in r10, mode r11, djinnBits r9, slotUnit
//   r7, bitOffset r5, j r1, slotOffset r0, cursor ip, countMember r11, item r4,
//   and only item & 0x1ff spilled to sp+0 (native frame 40, here 36). The cause
//   read from the greg dump: item ranks first and takes r1, so j takes r5, n
//   r6, bitOffset r7, and slotUnit falls to a high register, which is what
//   pushes in to r11. Reload then spills r1 (item) where the native spills r2.
//   countMember's preference for r0 keeps slotOffset off r0 in global.c's first
//   pass. One i for the unit loops fails for a different reason: at 590 insns
//   of live length it ranks last, takes r8, and the function grows to 459.
// - PRE hoists m * 2 out of the slot loop (the native rebuilds it, lsls r2, r3,
//   #1, with gData_020026c0 hoisted to lr instead), and the 0x1ff load stays in
//   it here where it is the split form in the native.
// - The frame address sits above the unpack loop, native inside it.
// - clearMember->djinnAvailable is hoisted out of the clear loop by PRE.
//   Entering that loop at its bottom test stops the hoist (gap 63), at differ
//   221, so it is not used.
// Tried with no gain on 1 November: split or shared n, bitOffset, inputIndex,
// j (as count or high), member, cursor; a one-time copy of the frame address at
// the top of the unpack loop; bottom-entered unpack and slot loops.
// The rest looks like a compiler question (K1 to K3), out of scope here: an
// older note found -Os does what the native shows (constants left in real
// loops with givs still reduced, classic GCSE and hoisting in place of PRE, the
// flag loop's shared i + 1). That needs re-measuring and only Pascal can weigh it.
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
  s32 slotUnit;
  s32 inputIndex;
  s32 bitOffset;
  s32 j;
  s32 n;
  s32 m;
  s32 shift;
  s32 following;
  u8 *stream;
  u8 *bytes;
  u8 *cursor;
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
    s32 offset;
    s32 second;
    s32 third;

    i = 0;
    offset = 4;
    stream = statsOffset + in;
  unpack:
    packedStats[i * 4] =
          stream[0] << 24 | stream[1] << 16 | stream[2] << 8 | stream[3];
      i++;
      *(u32 *)((u8 *)packedStats + offset) =
          stream[4] << 24 | stream[5] << 16 | stream[6] << 8 | stream[7];
      second = offset + 4;
      *(u32 *)((u8 *)packedStats + second) =
          stream[7] << 28 | stream[8] << 20 | stream[9] << 12 |
          stream[10] << 4 | stream[11] >> 4;
      third = offset + 8;
      *(u32 *)((u8 *)packedStats + third) =
          stream[11] << 28 | stream[12] << 20 | stream[13] << 12 |
          stream[14] << 4;
      offset += 16;
      stream += 15;
    if (i != 2)
      goto unpack;
    i = 0;
  stats:
    {
      // baseHp, basePp, then baseAttack to baseAgility at 4 to 6 and baseLuck
      // at byte 14: the native addresses them from baseHp.
      s16 *stats = &((struct BattleUnit *)Unit_Get(gData_020026a0[i]))->baseHp;

      stats[0] = packedStats[i * 2] >> 21;
      stats[1] = packedStats[i * 2] << 11 >> 21;
      stats[4] = packedStats[i * 2] & 0x1ff;
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
    s32 bit = (flagBits >> i) & 1;
    u8 *table;
    s32 index;

    following = i + 1;
    table = (u8 *)gData_02002490;
    index = i * 2;
    if (bit)
      GameFlag_SetBit(*(u16 *)(table + index));
    else
      GameFlag_ClearBit(*(u16 *)(table + index));
  }
  i = following;
  if (i != 6)
    goto flags;

  i = 0;
  shift = 0;
levels:
  levelMember = Unit_Get(gData_020026a0[i]);
  *(u8 *)&levelMember->level = levelBits >> shift & 0x7f;
#if LANGUAGE != LANGUAGE_JAPANESE
  levelMember->experience = Unit_GetExperienceForLevel(i, levelMember->level);
#endif
  i++;
  shift += 7;
  if (i != 4)
    goto levels;

  Djinn_GetRecoveryTable(0)->count = 0;
  i = 0;
clearDjinn:
  clearMember = Unit_Get(gData_020026a0[i]);
  // The native clears the same 40 bytes, djinnAvailable through
  // djinnActiveCounts, four times.
  n = 0;
clearDjinnFour:
  Iwram_ClearWordsEntry(clearMember->djinnAvailable, 40);
  n++;
  if (n != 4)
    goto clearDjinnFour;
  i++;
  if (i != 4)
    goto clearDjinn;

  for (i = 0; i != 28; i++) {
    s32 element = i / 7;
    s32 number = i % 7;

    GameFlag_ClearBit(element * 20 + number + 48);
  }

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
    slotUnit = 0;
  countUnits:
    {
      countMember = Unit_Get(gData_020026a0[slotUnit]);
      m = 0;
      cursor = in + inputIndex;
    countItems:
      j = 0;
      if (bitOffset < 0) {
        j = cursor[0] << -bitOffset & 0x1f;
        cursor++;
        inputIndex++;
        bitOffset += 8;
      }
      j += cursor[0] >> bitOffset & 0x1f;
      bitOffset -= 5;
      if (bitOffset == -5) {
        cursor++;
        inputIndex++;
        bitOffset = 3;
      }
      j &= 0x1f;
      {
        s32 slotOffset = 216;

        n = 0;
      countSlots:
        {
          s32 item = *(u16 *)((u8 *)countMember + slotOffset);

          if ((item & 0x1ff) == gData_020026c0[m])
            *(u16 *)((u8 *)countMember + slotOffset) = item | j << 11;
        }
        n++;
        slotOffset += 2;
        if (n != 15)
          goto countSlots;
      }
      m++;
      if (m != 23)
        goto countItems;
      slotUnit++;
    }
    if (slotUnit != 4)
      goto countUnits;

    gMainRoutineWork.coins = in[165] << 16 | in[166] << 8 | in[167];
  }

  i = 0;
calculate:
  Unit_UpdateClass(gData_020026a0[i]);
  Unit_Calculate(gData_020026a0[i]);
  i++;
  if (i != 4)
    goto calculate;
}
