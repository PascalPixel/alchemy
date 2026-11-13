/// @file
/// Debug: the unit snapshot, the parameter sets and the battle test menu.
// Part of battle.c.

#include "../battle_internal.h"
#include "flag_call.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "module/battle/presentation.h"
#include "module/system/system.h"
#include "resource_ids.h"

void Func_08118430(void) {
  for (;;)
    Battle_RunEncounter(0);
}

void Func_0811843c(void) {
  s32 j;
  struct Snapshot *snap = &Data_0200ff58;
  // Set here, between snap and ids: the native sets i before it loads ids.
  s32 i = 0;
  u8 *ids = Data_0812a16c;
  struct SnapshotCopy *copies = Data_0200ff6c;

  for (; i <= 4; i++, copies++) {
    struct SnapshotUnit *unit = Unit_Get(ids[i]);

    snap->listed[i] = 0;
    if (BattleParty_IsUnitListed(ids[i])) {
      snap->listed[i] = 1;
      if (i > 2) {
        snap->vitals[i].hp = unit->hp > 0 ? unit->hp : 1;
        {
          struct SnapshotVitals *pv;
          /* FAKEMATCH(pin): the PP base held in r0; as a plain local, 4 of
           * 183 instructions differ in tla-en. */
          register struct SnapshotVitals *base asm("r0");
          u32 pp;

          /* FAKEMATCH(temporary): the PP value held in a local while pv is
           * built; as pv->pp = unit->unk3a, 121 of 183 instructions differ
           * in tla-en. */
          pp = unit->unk3a;
          base = Data_0200ffec;
          // The address as an integer sum, i * 4 first: &base[i] puts the
          // base first in the add.
          pv = (struct SnapshotVitals *)(i * 4 + (s32)base);
          /* FAKEMATCH(dead): a store the tag loop's init overwrites; its
           * test builds 300 in a reload register while pp and pv hold r2
           * and r3, which puts r1 among reload's registers; without it, 121
           * of 183 instructions differ in tla-en. */
          if (i == 300)
            j = 0;
          pv->pp = pp;
        }
      }
      if (i <= 2) {
        for (j = 0; j < 4; j++)
          snap->tags[i][j] = unit->tags[j];
      }
      copies->bytes[0] = unit->snapshot[0];
      copies->bytes[1] = unit->snapshot[1];
      copies->bytes[2] = unit->snapshot[2];
      copies->bytes[3] = unit->snapshot[3];
      copies->bytes[4] = unit->snapshot[4];
      copies->bytes[5] = unit->snapshot[5];
      copies->bytes[6] = unit->snapshot[6];
      copies->bytes[7] = unit->snapshot[7];
      copies->bytes[8] = unit->snapshot[8];
      copies->bytes[9] = unit->snapshot[9];
      copies->bytes[10] = unit->snapshot[10];
      copies->bytes[11] = unit->snapshot[11];
      copies->bytes[12] = unit->snapshot[12];
      copies->bytes[13] = unit->snapshot[13];
      copies->bytes[14] = unit->snapshot[14];
      copies->bytes[15] = unit->snapshot[15];
      copies->bytes[16] = unit->snapshot[16];
      copies->bytes[17] = unit->snapshot[17];
      copies->bytes[18] = unit->snapshot[18];
      copies->bytes[19] = unit->snapshot[19];
      copies->bytes[20] = unit->snapshot[20];
      copies->bytes[21] = unit->snapshot[21];
      copies->bytes[22] = unit->snapshot[22];
      copies->bytes[23] = unit->snapshot[23];
      copies->bytes[24] = unit->snapshot[24];
    }
  }
}

/// Restores the five units Func_0811843c snapshotted: for each one still in
/// the battle party and listed, the first three's four tag bytes and each
/// one's 25 snapshot bytes, and the last two's HP and PP through
/// Unit_SetHpFar and Unit_SetPpFar. PP is read through the global, which
/// reloads the base as the native does.
void Func_081185c4(void) {
  s32 i;
  s32 j;
  struct Snapshot *snap = &Data_0200ff58;
  struct SnapshotCopy *copies = Data_0200ff6c;

  for (i = 0; i <= 4; i++, copies++) {
    struct SnapshotUnit *unit = Unit_Get(Data_0812a16c[i]);

    if (BattleParty_IsUnitListed(Data_0812a16c[i]) && snap->listed[i] != 0) {
      if (i <= 2) {
        for (j = 0; j < 4; j++)
          unit->tags[j] = snap->tags[i][j];
      }
      unit->snapshot[0] = copies->bytes[0];
      unit->snapshot[1] = copies->bytes[1];
      unit->snapshot[2] = copies->bytes[2];
      unit->snapshot[3] = copies->bytes[3];
      unit->snapshot[4] = copies->bytes[4];
      unit->snapshot[5] = copies->bytes[5];
      unit->snapshot[6] = copies->bytes[6];
      unit->snapshot[7] = copies->bytes[7];
      unit->snapshot[8] = copies->bytes[8];
      unit->snapshot[9] = copies->bytes[9];
      unit->snapshot[10] = copies->bytes[10];
      unit->snapshot[11] = copies->bytes[11];
      unit->snapshot[12] = copies->bytes[12];
      unit->snapshot[13] = copies->bytes[13];
      unit->snapshot[14] = copies->bytes[14];
      unit->snapshot[15] = copies->bytes[15];
      unit->snapshot[16] = copies->bytes[16];
      unit->snapshot[17] = copies->bytes[17];
      unit->snapshot[18] = copies->bytes[18];
      unit->snapshot[19] = copies->bytes[19];
      unit->snapshot[20] = copies->bytes[20];
      unit->snapshot[21] = copies->bytes[21];
      unit->snapshot[22] = copies->bytes[22];
      unit->snapshot[23] = copies->bytes[23];
      unit->snapshot[24] = copies->bytes[24];
      if (i > 2) {
        Unit_SetHpFar((struct BattleUnit *)unit, snap->vitals[i].hp);
        Unit_SetPpFar((struct BattleUnit *)unit, Data_0200ff58.vitals[i].pp);
      }
    }
  }
}

void Unit_UpdateClassFar(s32 owner);

s32 DebugParty_LoadPreset(s32 preset) {
  s32 nextDjinn[4];
  void *state;
  s32 owner;
  s32 done;
  s32 row;
  s32 j;
  s32 set;
  s32 i;

  done = 0;
  Party_RemoveMemberFar(0);
  Party_RemoveMemberFar(1);
  Party_RemoveMemberFar(2);
  Party_RemoveMemberFar(3);
  Party_RemoveMemberFar(5);
  Party_RemoveMemberFar(4);
  Party_RemoveMemberFar(6);
  Party_RemoveMemberFar(7);
  ((struct TradeOfferState *)Djinn_GetRecoveryTableFar(0))->count = done;

  set = 0;
  for (i = 0; i < 4; i++)
    nextDjinn[i] = 0;

  for (row = 0;; row++) {
    owner = gData_0812a174[row].owner;
    if ((u32)row > 440) {
      done = 1;
      break;
    }
    if (owner == -1) {
      if (set <= 69) {
        if (set == preset)
          break;
      } else if (set == (preset | 1)) {
        break;
      }
      set++;
      continue;
    }
    if (set < preset)
      continue;

    Party_AddMemberFar(owner);
    Unit_LevelUpToFar(owner, gData_0812a174[row].level);
    state = Unit_Get(owner);
    for (i = 0; i < 4; i++) {
      ((struct OwnerDjinnState *)state)->counts.banks.owned[i] = 0;
      ((struct OwnerDjinnState *)state)->counts.banks.active[i] = 0;
      ((struct OwnerDjinnState *)state)->flags.banks.owned[i] = 0;
      ((struct OwnerDjinnState *)state)->flags.banks.active[i] = 0;
    }
    for (i = 0; i < 32; i++)
      // Through a u16 cast the zero loads from the literal pool, as the ROM has
      // it.
      *(u16 *)&((struct OwnerActionState *)state)->actionSlots[i] = 0;
    for (i = 0; i < 2; i++) {
      if (gData_0812a174[row].actions[i] != 0)
        Unit_LearnPsynergyFar(owner, gData_0812a174[row].actions[i]);
    }
    for (i = 0; i < 4; i++) {
      for (j = 0; j < gData_0812a174[row].djinnCounts[i]; j++) {
        Djinn_GiveFar(owner, i, (nextDjinn[i] + 7) % 18);
        Djinn_SetFar(owner, i, (nextDjinn[i] + 7) % 18);
        nextDjinn[i]++;
      }
    }
    for (i = 0; i < 16; i++)
      Inventory_RemoveFar(owner, 0);
    for (i = 0; i < 6; i++) {
      if (gData_0812a174[row].items[i] != 0)
        Inventory_EquipFar(
            owner, Inventory_AddItemFar(owner, gData_0812a174[row].items[i]));
    }
    Unit_UpdateClassFar(owner);
  }
  return done;
}

void Func_08118954(void) {}

extern u8 gData_03001110;

void Func_080ad090(void);

void Func_08118958(void) {
  s32 startModeSet = 0;
  s32 down = gInput.held & 0x80;
  s32 encounter;
  s32 preset;
  s32 loadedPreset;
  struct InputState *input;
  struct MainRoutineWork *m;
  u16 *backdropId;

  Func_080ad090();
restart:
  Window_LoadGraphics();
  DebugText_Clear();
  Task_ResetAll();
  Heap_Initialize();
  VramCache_Reset();
  encounter = 69;
  GameFlag_SetBit(362);
  if (down) {
    preset = 0;
    loadedPreset = -1;
    GameFlag_ClearBit(362);
    input = &gInput;
    // Held for the loop's life: the backdrop id's value loads before its
    // address.
    backdropId = &Data_02000436;
    for (;;) {
      GameFlag_ClearBit(32);
      wait(1);
      for (;;) {
        if (input->repeat & 0x10)
          encounter++;
        if (input->repeat & 0x20)
          encounter--;
        if (input->repeat & 0x40)
          encounter -= 10;
        if (input->repeat & 0x80)
          encounter += 10;
        if (input->repeat & 0x100)
          preset++;
        if (input->repeat & 0x200)
          preset--;
        if (input->repeat & 1)
          break;
        if (input->repeat & 8)
          Func_08118954();
        // DEAD: once startModeSet is set, setting it again changes nothing;
        // the ROM keeps the test.
        if ((input->repeat & 2) || startModeSet)
          startModeSet = 1;
        wait(1);
      }
      if (preset != loadedPreset) {
        Func_080ad090();
        DebugParty_LoadPreset(preset);
        loadedPreset = preset;
      }
      if (input->held & 0x80)
        GameFlag_SetBit(364);
      Unit_CalculateFar(0);
      *backdropId = (s32)&ResourceId_ColossoBackdrop;
      m = &gMainRoutineWork;
      if (encounter == 28)
        GameFlag_SetBit(366);
      GameFlag_SetBit(354);
      if (startModeSet)
        m->battleRule24b = 5;
      Battle_RunEncounter(encounter);
      gData_03001110 = 1;
      Window_LoadGraphics();
      DebugText_Clear();
      Task_ResetAll();
      Heap_Initialize();
      VramCache_Reset();
    }
  }
  GameFlag_SetBit(354);
  Battle_RunEncounter(69);
  goto restart;
}
