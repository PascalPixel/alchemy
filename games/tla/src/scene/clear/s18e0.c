#include "types.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "battle/battle_unit.h"
#include "owner_state.h"
#include "version.h"
#include "event_inline.h"

extern s32 gData_020026a0[];
extern u16 gData_020026b0[];
extern u16 gData_020026c0[];
extern u16 gData_02002490[];

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
  djinnBits = (in[3] & 0x0f) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  flagBits = in[7];
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
#if TLA_PATCH_1
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
          if ((unit->inventory[n] & 0x1ff) == gData_020026c0[m])
            unit->inventory[n] |= j << 11;
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
