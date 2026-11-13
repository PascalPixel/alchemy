/// @file
/// Running summon and table effects, and shared target helpers.
// Part of f13ba50.c.

#include "../f13ba50_internal.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/f13ba50.h"
#include "module/tla.h"

// Battle effects: repeat an empty effect.

/// Calls the empty BattleEffect_DoNothing cnt times without waiting for frames.
/// cnt: call count.
void BattleEffect_CallDoNothing(s32 cnt) {
  s32 n;

  n = 0;
  if (cnt != 0) {
    do {
      n += 1;
      BattleEffect_DoNothing();
    } while (n != cnt);
  }
}

struct SummonArgument;

struct BattleEffectArgument;

/* VIEW(f14b9e8): routine BattleEffect_RunTiamat as void BattleEffect_RunTiamat(s32 *arg); the definition has DEFINE_EFFECT_KIND(BattleEffect_RunDragonDriver, Func_08147aec, 1) DEFINE_EFFECT_KIND(BattleEffect_RunTiamat, Func_08147aec, 0) void Func_08.... */
extern void BattleEffect_RunTiamat__f14b9e8(s32 *arg) __asm__("BattleEffect_RunTiamat");

extern u8 gMapCellBuffer[];

/// Runs the battle effect of mode *arg (1 to 29; 0 and 11 the default one)
/// with the effect's three work blocks taken.
void BattleEffect_RunSceneEffect(s32 *arg) {
  struct FxRunWork *work;

  HeapSlot_Alloc(100, 0x302);
  HeapSlot_Alloc(92, 0x7b7c);
  HeapSlot_Alloc(96, 0x4000);
  work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  work->script = arg;
  work->active = 1;
  if (*arg == 0) {
    BattleEffect_RunMeteor((struct BattleEffectArgument *)arg);
  } else {
    switch (*arg) {
    case 1:
      BattleEffect_RunRamses(arg);
      break;
    case 2:
      BattleEffect_RunNereid(arg);
      break;
    case 3:
      BattleEffect_RunKirin(arg);
      break;
    case 4:
      BattleEffect_RunAtalanta(arg);
      break;
    case 5:
      BattleEffect_RunCybele(arg);
      break;
    case 6:
      BattleEffect_RunNeptune(arg);
      break;
    case 7:
      BattleEffect_RunTiamat__f14b9e8(arg);
      break;
    case 8:
      BattleEffect_RunProcne(arg);
      break;
    case 9:
      BattleEffect_RunJudgment(arg);
      break;
    case 10:
      BattleEffect_RunBoreas(arg);
      break;
    case 11:
      BattleEffect_RunMeteor((struct BattleEffectArgument *)arg);
      break;
    case 12:
      BattleEffect_RunThor(arg);
      break;
    case 13:
      BattleEffect_RunZagan(arg);
      break;
    case 14:
      BattleEffect_RunMegaera(arg);
      break;
    case 15:
      BattleEffect_RunFlora(arg);
      break;
    case 16:
      BattleEffect_RunMoloch(arg);
      break;
    case 17:
      BattleEffect_RunUlysses(arg);
      break;
    case 18:
      BattleEffect_RunHaures(arg);
      break;
    case 19:
      BattleEffect_RunEclipse(arg);
      break;
    case 20:
      BattleEffect_RunCoatlicue(arg);
      break;
    case 21:
      BattleEffect_RunDaedalus(arg);
      break;
    case 22:
      BattleEffect_RunAzul(arg);
      break;
    case 23:
      BattleEffect_RunCatastrophe(arg);
      break;
    case 24:
      BattleEffect_RunCharon(arg);
      break;
    case 25:
      BattleEffect_RunIris(arg);
      break;
    case 26:
      BattleEffect_RunDaedalusAlt(arg);
      break;
    case 27:
      BattleEffect_RunTrident(arg);
      break;
    case 28:
      BattleEffect_RunCruelRuin(arg);
      break;
    case 29:
      BattleEffect_RunMeteorBlow(arg);
      break;
    }
  }
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  HeapSlot_Release(100);
}

/// Runs one battle effect script: takes the effect's three work blocks, calls
/// the handler its first word selects, then gives the blocks back.
void BattleEffect_RunTableEffect(s32 *script) {
  struct FxRunWork *work;

  HeapSlot_Alloc(100, 0x302);
  HeapSlot_Alloc(92, 0x7b7c);
  HeapSlot_Alloc(96, 0x4000);
  work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  work->script = script;
  work->active = 1;
  gData_08197aa0[*script - 1](script);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  HeapSlot_Release(100);
}

void BattleEffect_BuildWindowEdgeTable(void) {
  u16 *edge;
  u16 *line;
  u8 *inset;
  s32 i;
  s32 right;

  edge = (u16 *)gMapCellBuffer;
  line = (u16 *)(Ram_MapCellBuffer + 0x82);
  inset = Ram_MapCellBuffer + 2;
  for (i = 0; i != 160; i++) {
    if ((u32)(i - 8) <= 127) {
      right = *edge - inset[i - 8];
      CLAMP(right, 0, 240);
      line[i] = right;
    } else {
      line[i] = 0xfff1;
    }
  }
  {
    volatile u16 *channel = REG_DMA0;
    u16 control;

    control = channel[5];
    channel[5] = control & 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(gMapCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}

void *Unit_Get(s32 owner);

/// Spawns the battle objects of each unit in the list that has HP left.
void BattleEffect_SelectLivingTargets(struct FxPartyList *party) {
  s16 list[14];
  s32 count = 0;
  s32 i;

  for (i = 0; i != party->count; i++) {
    if (((struct FxUnit *)Unit_Get(party->ids[i]))->hp > 0)
      list[count++] = party->ids[i];
  }
  list[count] = 0xff;
  BattleActor_SpawnObjectsForListFar(list, 0);
}
