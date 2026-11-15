/// @file
/// Running summon and table effects, and shared target helpers.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "gamepak/far_call/battle.h"

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

/// Runs one of the large battle effects, the summons and the scenes of a few
/// abilities: takes the effect's sheet, work and canvas, runs the effect of
/// args->kind (enum SummonEffectId) and frees them again.
void BattleEffect_RunSummon(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  work = Ram_HeapSlots->battleEffect;
  work->args = args;
  work->vblanksSinceTransfer = 1;
  if (args->kind == SUMMON_EFFECT_NONE) {
    BattleEffect_RunMeteor(args);
  } else {
    switch (args->kind) {
    case SUMMON_EFFECT_RAMSES:
      BattleEffect_RunRamses(args);
      break;
    case SUMMON_EFFECT_NEREID:
      BattleEffect_RunNereid(args);
      break;
    case SUMMON_EFFECT_KIRIN:
      BattleEffect_RunKirin(args);
      break;
    case SUMMON_EFFECT_ATALANTA:
      BattleEffect_RunAtalanta(args);
      break;
    case SUMMON_EFFECT_CYBELE:
      BattleEffect_RunCybele(args);
      break;
    case SUMMON_EFFECT_NEPTUNE:
      BattleEffect_RunNeptune(args);
      break;
    case SUMMON_EFFECT_TIAMAT:
      BattleEffect_RunTiamat(args);
      break;
    case SUMMON_EFFECT_PROCNE:
      BattleEffect_RunProcne(args);
      break;
    case SUMMON_EFFECT_JUDGMENT:
      BattleEffect_RunJudgment(args);
      break;
    case SUMMON_EFFECT_BOREAS:
      BattleEffect_RunBoreas(args);
      break;
    case SUMMON_EFFECT_METEOR:
      BattleEffect_RunMeteor(args);
      break;
    case SUMMON_EFFECT_THOR:
      BattleEffect_RunThor(args);
      break;
    case SUMMON_EFFECT_ZAGAN:
      BattleEffect_RunZagan(args);
      break;
    case SUMMON_EFFECT_MEGAERA:
      BattleEffect_RunMegaera(args);
      break;
    case SUMMON_EFFECT_FLORA:
      BattleEffect_RunFlora(args);
      break;
    case SUMMON_EFFECT_MOLOCH:
      BattleEffect_RunMoloch(args);
      break;
    case SUMMON_EFFECT_ULYSSES:
      BattleEffect_RunUlysses(args);
      break;
    case SUMMON_EFFECT_HAURES:
      BattleEffect_RunHaures(args);
      break;
    case SUMMON_EFFECT_ECLIPSE:
      BattleEffect_RunEclipse(args);
      break;
    case SUMMON_EFFECT_COATLICUE:
      BattleEffect_RunCoatlicue(args);
      break;
    case SUMMON_EFFECT_DAEDALUS:
      BattleEffect_RunDaedalus(args);
      break;
    case SUMMON_EFFECT_AZUL:
      BattleEffect_RunAzul(args);
      break;
    case SUMMON_EFFECT_CATASTROPHE:
      BattleEffect_RunCatastrophe(args);
      break;
    case SUMMON_EFFECT_CHARON:
      BattleEffect_RunCharon(args);
      break;
    case SUMMON_EFFECT_IRIS:
      BattleEffect_RunIris(args);
      break;
    case SUMMON_EFFECT_DAEDALUS_ALT:
      BattleEffect_RunDaedalusAlt(args);
      break;
    case SUMMON_EFFECT_TRIDENT:
      BattleEffect_RunTrident(args);
      break;
    case SUMMON_EFFECT_CRUEL_RUIN:
      BattleEffect_RunCruelRuin(args);
      break;
    case SUMMON_EFFECT_METEOR_BLOW:
      BattleEffect_RunMeteorBlow(args);
      break;
    }
  }
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// Runs an ordinary battle effect from gBattleEffect_Handlers, with the sheet,
/// work and canvas it lends.
void BattleEffect_RunFromTable(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  work = Ram_HeapSlots->battleEffect;
  work->args = args;
  work->vblanksSinceTransfer = 1;
  gBattleEffect_Handlers[args->kind - 1](args);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// Builds window 0's right edge for every line and restarts the DMA that writes
/// it.
void BattleEffect_BuildWindowEdgeTable(void) {
  u16 *edge;
  u16 *line;
  u8 *inset;
  s32 i;
  s32 right;

  edge = WINDOW_EDGE_RIGHT;
  line = (u16 *)WINDOW_EDGE_LINES;
  inset = WINDOW_EDGE_INSETS;
  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    if ((u32)(i - 8) <= 127) {
      right = *edge - inset[i - 8];
      CLAMP(right, 0, DISPLAY_WIDTH);
      line[i] = right;
    } else {
      line[i] = WIN_RANGE(255, 241);
    }
  }
  {
    volatile u16 *channel = REG_DMA0;
    u16 control;

    control = channel[DMA_CONTROL_HALF];
    channel[DMA_CONTROL_HALF] = control & (u16)~DMA_START_TIMING_DRQ_REPEAT;
    channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
    (void)channel[DMA_CONTROL_HALF];
    Dma_Set(gMapGridCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}

/// Spawns the battle objects of each unit in the list that has HP left.
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *party) {
  s16 list[14];
  s32 count = 0;
  s32 i;

  for (i = 0; i != party->targetCount; i++) {
    if (Unit_Get(party->targets[i])->hp > 0)
      list[count++] = party->targets[i];
  }
  list[count] = 0xff;
  BattleActor_SpawnObjectsForListFar(list, 0);
}
