// Draft, differ 561 of 423 (ja 585 of 413): unpacks clear data into the four
// units, flags, Djinn, items and their counts. Left: loop forms (the native
// indexes its tables where this steps pointers) and register choice.
#include "types.h"
#include "adventure.h"
#include "iwram_call.h"

extern s32 gData_020026a0[];
extern u16 gData_020026b0[];
extern u16 gData_020026c0[];
extern u16 gData_02002490[];

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
u8 *Unit_Get(s32 id);
s32 Unit_GetExperienceForLevel(s32 id, s32 level);
u8 *Djinn_GetRecoveryTable(s32 id);
void Djinn_AddToLeastLoadedOwner(s32 element, s32 index);
void PartyInventory_Add(s32 item);
void Unit_UpdateClass(s32 id);
void Unit_Calculate(s32 id);

void Func_020018e0(s32 mode, u8 *in) {
  u32 levels = in[0] | in[1] << 8 | in[2] << 16 | (in[3] & 0xf0) << 20;
  u32 djinn = (in[3] & 15) | in[4] << 4 | in[5] << 12 | in[6] << 20;
  u8 flags = in[7];
  u8 found = in[8];
  u32 stats[8];
  s32 i, k, j;

  if (mode != 2) {
    u8 *p = in + 8 + (mode != 0);

    for (i = 0; i != 2; i++) {
      stats[i * 4] = p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
      stats[i * 4 + 1] = p[4] << 24 | p[5] << 16 | p[6] << 8 | p[7];
      stats[i * 4 + 2] =
          p[7] << 28 | p[8] << 20 | p[9] << 12 | p[10] << 4 | p[11] >> 4;
      stats[i * 4 + 3] = p[11] << 28 | p[12] << 20 | p[13] << 12 | p[14] << 4;
      p += 15;
    }
    for (i = 0; i != 4; i++) {
      s16 *stat = (s16 *)(Unit_Get(gData_020026a0[i]) + 16);
      u32 w = stats[i * 2];

      stat[0] = w >> 21;
      stat[4] = w & 0x1ff;
      stat[1] = (w << 11) >> 21;
      w = stats[i * 2 + 1];
      stat[5] = (w >> 22) & 0x1ff;
      stat[6] = (w >> 12) & 0x1ff;
      ((u8 *)stat)[14] = (w >> 4) & 127;
    }
  }
  for (i = 0; i != 6; i++) {
    if ((flags >> i) & 1)
      GameFlag_SetBit(gData_02002490[i]);
    else
      GameFlag_ClearBit(gData_02002490[i]);
  }
  for (i = 0, k = 0; i != 4; i++, k += 7) {
    u8 *unit = Unit_Get(gData_020026a0[i]);

    unit[15] = (levels >> k) & 127;
    *(s32 *)(unit + 292) = Unit_GetExperienceForLevel(i, unit[15]);
  }
  *(s32 *)(Djinn_GetRecoveryTable(0) + 296) = 0;
  for (i = 0; i != 4; i++) {
    u8 *unit = Unit_Get(gData_020026a0[i]);

    for (k = 0; k != 4; k++)
      Iwram_ClearWordsEntry(unit + 248, 40);
  }
  for (i = 0; i != 28; i++)
    GameFlag_ClearBit(i / 7 * 20 + i % 7 + 48);
  for (i = 0; i != 28; i++) {
    if ((djinn >> i) & 1)
      Djinn_AddToLeastLoadedOwner(i / 7, i % 7);
  }
  if (mode != 0) {
    Unit_Get(gData_020026a0[0]);
    for (i = 0; i != 8; i++) {
      if ((found >> i) & 1)
        PartyInventory_Add(gData_020026b0[i]);
    }
  }
  if (mode == 0) {
    s32 pos = 39;
    s32 bit = 0;
    s32 shift;

    for (i = 0; i != 4; i++) {
      u16 *items = (u16 *)(Unit_Get(gData_020026a0[i]) + 216);
      u8 *p = in + pos;

      for (k = 0; k != 15; k++) {
        *items++ = ((p[0] << (bit + 1)) | (p[1] >> (7 - bit))) & 0x1ff;
        bit++;
        p++;
        pos++;
        if (bit == 7) {
          bit = 0;
          p++;
          pos++;
        }
      }
    }
    pos = 107;
    shift = -1;
    for (i = 0; i != 4; i++) {
      u8 *unit = Unit_Get(gData_020026a0[i]);
      u8 *p = in + pos;

      for (k = 0; k != 23; k++) {
        u32 count = 0;

        if (shift < 0) {
          count = (*p << -shift) & 31;
          p++;
          pos++;
          shift += 8;
        }
        count += (*p >> shift) & 31;
        shift -= 5;
        if (shift == -5) {
          p++;
          pos++;
          shift = 3;
        }
        count &= 31;
        for (j = 0; j != 15; j++) {
          u16 *item = (u16 *)(unit + 216) + j;

          if ((*item & 0x1ff) == gData_020026c0[k])
            *item = count << 11 | *item;
        }
      }
    }
    *(u32 *)((u8 *)&gAdventure + 16) = in[165] << 16 | in[166] << 8 | in[167];
  }
  for (i = 0; i != 4; i++) {
    Unit_UpdateClass(gData_020026a0[i]);
    Unit_Calculate(gData_020026a0[i]);
  }
}
