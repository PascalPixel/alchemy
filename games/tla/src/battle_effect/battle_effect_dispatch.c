/// @file
/// Running summon and table effects, and shared target helpers.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "gamepak/far_call/battle.h"

/// Calls the empty BattleEffect_DoNothing count times: a busy delay.
void BattleEffect_CallDoNothing(s32 count) {
  s32 i;

  for (i = 0; i != count; i++)
    BattleEffect_DoNothing();
}

/// Runs one of the large battle effects, the summons and the scenes of a few
/// abilities: takes the effect's sprite sheet, state and canvas, runs the
/// effect of args->kind (enum SummonEffectId) and frees them again.
void BattleEffect_RunSummon(struct BattleEffectParams *args) {
  struct BattleEffectState *state;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  state = Ram_HeapSlots->battleEffect;
  state->args = args;
  state->vblanksSinceTransfer = 1;
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
    case SUMMON_EFFECT_DAEDALUS_STRIKE:
      BattleEffect_RunDaedalusStrike(args);
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

/// Runs an ordinary battle effect from gBattleEffect_Handlers, with the sprite
/// sheet, state and canvas it lends.
void BattleEffect_RunFromTable(struct BattleEffectParams *args) {
  struct BattleEffectState *state;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
  HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
  state = Ram_HeapSlots->battleEffect;
  state->args = args;
  state->vblanksSinceTransfer = 1;
  gBattleEffect_Handlers[args->kind - 1](args);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

/// Builds window 0's right edge for every line and restarts the DMA that writes
/// it.
void BattleEffect_BuildWindowEdgeTable(void) {
  u16 *baseRightEdge;
  u16 *scanlineEdges;
  u8 *edgeOffsets;
  s32 scanline;
  s32 rightEdge;

  baseRightEdge = WINDOW_EDGE_RIGHT;
  scanlineEdges = (u16 *)WINDOW_EDGE_LINES;
  edgeOffsets = WINDOW_EDGE_INSETS;
  for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
    if (scanline >= 8 && scanline < 8 + WINDOW_EDGE_INSET_LINES) {
      rightEdge = *baseRightEdge - edgeOffsets[scanline - 8];
      CLAMP(rightEdge, 0, DISPLAY_WIDTH);
      scanlineEdges[scanline] = rightEdge;
    } else {
      scanlineEdges[scanline] = WIN_RANGE(255, 241);
    }
  }
  {
    volatile u16 *channel = REG_DMA0;

    channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
    channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
    (void)channel[DMA_CONTROL_HALF];
    Dma_Set(WINDOW_EDGE_LINES, (void *)&REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)channel);
  }
}

/// Shows the battle objects of every target with HP left.
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *args) {
  s16 list[UNIT_MAX];
  s32 count;
  s32 i;

  count = 0;
  for (i = 0; i != args->targetCount; i++) {
    if (Unit_Get(args->targets[i])->hp > 0)
      list[count++] = args->targets[i];
  }
  list[count] = BATTLE_UNIT_LIST_END;
  BattleActor_SpawnObjectsForListFar(list, 0);
}
