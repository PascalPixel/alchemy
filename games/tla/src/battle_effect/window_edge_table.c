#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "heap_release.h"
#include "macros.h"
#include "battle_effect.h"

struct SummonArgument;

struct FxRunWork {
  u8 unk0000[0x78e4];
  s32 active;
  u8 unk78e8[8];
  s32 *script;
};

struct BattleEffectArgument;
void BattleEffect_RunMeteor(struct BattleEffectArgument *effect);
void BattleEffect_RunRamses(struct BattleEffectArgument *effect);
void BattleEffect_RunNereid(struct BattleEffectArgument *effect);
void BattleEffect_RunKirin(struct BattleEffectArgument *effect);
void BattleEffect_RunAtalanta(struct BattleEffectArgument *effect);

void BattleEffect_RunNeptune(struct BattleEffectArgument *effect);
void BattleEffect_RunTiamat(s32 *arg);
void BattleEffect_RunProcne(struct BattleEffectArgument *effect);
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect);
void BattleEffect_RunBoreas(struct BattleEffectArgument *effect);
void BattleEffect_RunThor(struct BattleEffectArgument *effect);
void BattleEffect_RunZagan(struct BattleEffectArgument *effect);
void BattleEffect_RunMegaera(struct BattleEffectArgument *effect);
void BattleEffect_RunFlora(struct BattleEffectArgument *effect);
void BattleEffect_RunMoloch(struct BattleEffectArgument *effect);
void BattleEffect_RunUlysses(struct BattleEffectArgument *effect);
void BattleEffect_RunHaures(struct BattleEffectArgument *effect);
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect);
void BattleEffect_RunCoatlicue(struct BattleEffectArgument *effect);
void BattleEffect_RunDaedalus(struct SummonArgument *arg);
void BattleEffect_RunAzul(struct BattleEffectArgument *effect);
void BattleEffect_RunCatastrophe(struct BattleEffectArgument *effect);
void BattleEffect_RunCharon(struct BattleEffectArgument *arg);
void BattleEffect_RunIris(struct BattleEffectArgument *effect);
void BattleEffect_RunDaedalusAlt(struct BattleEffectArgument *effect);
void BattleEffect_RunTrident(struct BattleEffectArgument *effect);
void BattleEffect_RunCruelRuin(struct BattleEffectArgument *effect);
void BattleEffect_RunMeteorBlow(struct BattleEffectArgument *effect);

extern void (*gData_08197aa0[])(s32 *script);
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
      BattleEffect_RunTiamat(arg);
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
