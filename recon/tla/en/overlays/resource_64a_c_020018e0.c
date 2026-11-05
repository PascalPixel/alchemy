// Under -Os, 5 November: differ 132 of 423 (tla-en); older figures below predate it.
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
  s32 j;
  s32 n;
  s32 m;
  s32 position;
  s32 bit;

  levelBits = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  flagBits = in[7];
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  keyItemBits = in[8];

  if (mode != 2) {
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
      packedStats[i * 4 + 3] = in[statsOffset + 11] << 28 |
                               in[statsOffset + 12] << 20 |
                               in[statsOffset + 13] << 12 |
                               in[statsOffset + 14] << 4;
      statsOffset += 15;
    }
    for (i = 0; i != 4; i++) {
      s16 *stats = &((struct BattleUnit *)Unit_Get(gData_020026a0[i]))->baseHp;
      s32 at = i * 8;

#define WORD (*(u32 *)((u8 *)packedStats + at))
      stats[0] = WORD >> 21;
      stats[1] = WORD << 11 >> 21;
      stats[4] = WORD & 0x1ff;
      at += 4;
      stats[5] = WORD >> 22 & 0x1ff;
      stats[6] = WORD >> 12 & 0x1ff;
      ((u8 *)stats)[14] = WORD >> 4 & 0x7f;
#undef WORD
    }
  }

  for (i = 0; i != 6; i++) {
    if ((flagBits >> i) & 1)
      GameFlag_SetBit(gData_02002490[i]);
    else
      GameFlag_ClearBit(gData_02002490[i]);
  }

  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

    *(u8 *)&unit->level = levelBits >> (i * 7) & 0x7f;
#if LANGUAGE != LANGUAGE_JAPANESE
    unit->experience = Unit_GetExperienceForLevel(i, unit->level);
#endif
  }

  Djinn_GetRecoveryTable(0)->count = 0;
  for (i = 0; i != 4; i++) {
    struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

    // The native clears the same 40 bytes, djinnAvailable through
    // djinnActiveCounts, four times.
    for (n = 0; n != 4; n++)
      Iwram_ClearWordsEntry(unit->djinnAvailable, 40);
  }

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
    for (i = 0; i != 8; i++) {
      if ((keyItemBits >> i) & 1)
        PartyInventory_Add(gData_020026b0[i]);
    }
  }

  if (mode == 0) {
    position = 39;
    bit = 0;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

      for (n = 0; n != 15; n++) {
        unit->inventory[n] =
            (in[position] << (bit + 1) | in[position + 1] >> (7 - bit)) & 0x1ff;
        bit++;
        position++;
        if (bit == 7) {
          bit = 0;
          position++;
        }
      }
    }

    position = 107;
    bit = -1;
    for (i = 0; i != 4; i++) {
      struct BattleUnit *unit = Unit_Get(gData_020026a0[i]);

      for (m = 0; m != 23; m++) {
        j = 0;
        if (bit < 0) {
          j = in[position] << -bit & 0x1f;
          position++;
          bit += 8;
        }
        j += in[position] >> bit & 0x1f;
        bit -= 5;
        if (bit == -5) {
          position++;
          bit = 3;
        }
        j &= 0x1f;
        for (n = 0; n != 15; n++) {
          s32 item = unit->inventory[n];
          s32 masked = item & 0x1ff;

          if (masked == gData_020026c0[m])
            unit->inventory[n] = j << 11 | item;
        }
      }
    }

    gMainRoutineWork.coins = in[165] << 16 | in[166] << 8 | in[167];
  }

  for (i = 0; i != 4; i++) {
    Unit_UpdateClass(gData_020026a0[i]);
    Unit_Calculate(gData_020026a0[i]);
  }
}
