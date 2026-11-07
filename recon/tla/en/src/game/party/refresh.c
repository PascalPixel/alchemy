#include "scene.h"
#include "game_flags.h"
#include "inventory.h"
#include "main_routine_work.h"
#include "types.h"
#include "owner_state.h"
#include "fixed_math.h"

s32 Unit_Calculate(s32);
s32 GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

// Party_RestoreHpPp from The Broken Seal.
void Func_080ae16c(s32 arg0) {
  s32 count;
  s32 n;
  u8 *obj;
  s32 t;
  s32 v14;
  s32 v16;
  s32 one;
  s16 v34;

  count = Party_CountActiveOwners();
  for (n = 0; n < count; n++) {
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      /* FAKEMATCH: ⚓️ loads gMainRoutineWork before it starts the owner list's
       * offset; read as a plain field the offset is started first (differ 2) */
      asm volatile("");
      obj = Unit_Get(party->activeOwners[n]);
    }

    do {
      *(u16 *)(obj + 0x38) = *(u16 *)(obj + 0x34);
      *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
    } while (0);

    v34 = *(s16 *)(obj + 0x34);
    t = __divsi3(v34 << 14, v34);
    v14 = 0x4000;
    if (t <= 0x4000) {
      v14 = 0;
      if (t >= 0) {
        v14 = t;
      }
    }
    *(s16 *)(obj + 0x14) = (s16)v14;
    if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x14) = (s16)one;
    }

    t = __divsi3(*(s16 *)(obj + 0x3A) << 14, *(s16 *)(obj + 0x36));
    v16 = 0x4000;
    if (t <= 0x4000) {
      v16 = 0;
      if (t >= 0) {
        v16 = t;
      }
    }
    *(s16 *)(obj + 0x16) = (s16)v16;
    if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x16) = (s16)one;
    }

    if (arg0 == 1) {
      *(s8 *)(obj + 0x131) = 0;
      *(s8 *)(obj + 0x140) = 0;
    }
  }
}

extern const u8 gCharacter_ElementGroupTable[];

// Party_RestorePpOfLitElements from The Broken Seal.
void Func_080ae220(void) {
  s32 count;
  s32 n;
  u8 ownerId;
  s32 group;
  s32 doRefresh;
  u8 *obj;
  s32 t;
  s32 v14;
  s32 v16;
  s32 v34;
  s32 v36;
  s32 v38;
  s32 v3A;
  s32 one;

  count = Party_CountActiveOwners();
  for (n = 0; n < count; n++) {
    {
      struct MainRoutineWork *party = &gMainRoutineWork;

      /* FAKEMATCH: ⚓️ loads gMainRoutineWork before it starts the owner list's
       * offset; read as a plain field the offset is started first (differ 2) */
      asm volatile("");
      ownerId = party->activeOwners[n];
    }
    group = gCharacter_ElementGroupTable[ownerId];
    doRefresh = 0;
    if (group == 0) {
      if (GameFlag_Test(0x110) || GameFlag_Test(0x112)) {
        doRefresh = 1;
      }
    } else {
      if (GameFlag_Test(0x111) || GameFlag_Test(0x113)) {
        doRefresh = 1;
      }
    }

    if (doRefresh == 0) {
      continue;
    }

    obj = Unit_Get(ownerId);
    do {
      *(u16 *)(obj + 0x3A) = *(u16 *)(obj + 0x36);
    } while (0);

    v38 = *(s16 *)(obj + 0x38);
    v34 = *(s16 *)(obj + 0x34);
    t = __divsi3(v38 << 14, v34);
    v14 = 0x4000;
    if (t <= 0x4000) {
      v14 = 0;
      if (t >= 0) {
        v14 = t;
      }
    }
    *(s16 *)(obj + 0x14) = (s16)v14;
    if (((v14 << 16) == 0) && (*(s16 *)(obj + 0x38) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x14) = (s16)one;
    }

    v3A = *(s16 *)(obj + 0x3A);
    v36 = *(s16 *)(obj + 0x36);
    t = __divsi3(v3A << 14, v36);
    v16 = 0x4000;
    if (t <= 0x4000) {
      v16 = 0;
      if (t >= 0) {
        v16 = t;
      }
    }
    *(s16 *)(obj + 0x16) = (s16)v16;
    if (((v16 << 16) == 0) && (*(s16 *)(obj + 0x3A) != 0)) {
      one = 1;
      *(s16 *)(obj + 0x16) = (s16)one;
    }
  }
}

struct UnitDjinnSets {
  u8 unk00[0xf8];
  u32 djinn[4];
};

/// Element and index of the last owned Djinni.
s32 Func_080ae2fc(s32 unit, s32 *element_out, s32 *index_out) {
  u32 *sets = ((struct UnitDjinnSets *)Unit_Get(unit))->djinn;
  s32 element = -1;
  s32 index = -1;
  s32 i;
  s32 j;

  for (i = 0; i < 20; i++) {
    for (j = 0; j < 4; j++) {
      if (sets[j] & (1 << i)) {
        element = j;
        index = i;
      }
    }
  }
  if (element == -1)
    return element;
  *element_out = element;
  *index_out = index;
  return 0;
}
