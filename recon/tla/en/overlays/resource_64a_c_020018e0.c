// Draft, differ 248 of 423, skeleton gap 82 (tla-en, 2026-11-01): 40 removed,
// 42 added.
// Unpacks clear data, in the layout The Broken Seal's
// FileScreen_PackPasswordData packs (and Func_02001c7c here), into the four
// units' base stats, levels and Djinn, the story flags, the Psynergy items and
// the inventories with their counts. Control flow, calls and memory accesses
// are the native's, except as listed below.
// Loops: unpack, stats, flags and the slot loops are gotos, so
// that loop.c leaves constants in them, with the native's strength reductions
// spelled out (offset, second, third, bytes, cursor, slot, slotOffset).
// Allocation hangs on what crosses calls. in reaches a register only when
// nothing hoisted lives beside it, which is why the draft has:
// - cursor split from bytes, and stream from both;
// - second and third as temporaries in place of one shared next;
// - itemOffset stepping by 2 to 46 where the native has m stepping by 1 to 23
//   and rebuilds m * 2 in the slot loop. PRE hoists m * 2 out of even a goto
//   loop, and that pseudo takes the register in needs. A copy of m at the top
//   of the loop is folded by cprop. This costs the two instructions of m * 2;
// - level stored through a u8 pointer, whose field store otherwise leaves a
//   zero that gcse carries across two calls in place of the count = 0 store's;
// - s32 item, because u16 gives a HImode 0x1ff that PRE carries across a call.
// Left, in order of size:
// - The count and inventory sections' labels. The draft has bitOffset r7, j r5,
//   slotUnit r9, m lr, table r10, in r11, mode r9, djinnBits r10; the native
//   has bitOffset r5, j r1, the unit r7, m r9, table lr, in r10, mode r11,
//   djinnBits r9. item is spilled here where the native spills item & 0x1ff
//   only: reload runs one low register short. The rest of those sections' lines
//   follow from these.
// - The frame address is hoisted out of the stats loop (the native rebuilds it
//   there, 4 more bytes of frame here), and that moves i++ and the second and
//   third temporaries by a few places.
// - clearMember->djinnAvailable is hoisted out of the clear loop by PRE.
// Tried with no gain: one counter for all loops or for the last three (differ
// 269 to 454), bitOffset, inputIndex and the unit split per section (equal or
// worse), do loops for stats and the clear (worse), a real unpack do loop (its
// reductions fold the frame into a pointer).
// What blocks a match is a compiler question (K1 to K3), so it stops here.
// alchemy-gcc's -Os does exactly what the native shows and -O2 cannot: loop.c
// skips move_movables but still reduces givs (constants stay in real loops),
// gcse.c runs classic GCSE and code hoisting in place of PRE (the flag loop's
// shared head, no hoisting of m * 2), and toplev.c leaves refs unweighted by
// loop depth (the allocation order above). An earlier hand trial of -Os reached
// 138 lines of register allocation. Only Pascal can weigh that.
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
  s32 inputIndex;
  s32 bitOffset;
  s32 j;
  s32 n;
  s32 itemOffset;
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
    do {
      countMember = Unit_Get(gData_020026a0[slotUnit]);
      itemOffset = 0;
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
        u16 *table = gData_020026c0;
        s32 slotOffset = 216;

        n = 0;
      countSlots:
        {
          s32 item = *(u16 *)((u8 *)countMember + slotOffset);

          if ((item & 0x1ff) == *(u16 *)((u8 *)table + itemOffset))
            *(u16 *)((u8 *)countMember + slotOffset) = j << 11 | item;
        }
        n++;
        slotOffset += 2;
        if (n != 15)
          goto countSlots;
      }
      itemOffset += 2;
      if (itemOffset != 46)
        goto countItems;
      slotUnit++;
    } while (slotUnit != 4);
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
