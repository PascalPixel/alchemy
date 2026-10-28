#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "mem.h"
#include "macros.h"

struct FxRunWork {
  u8 unk0000[0x78e4];
  s32 active;
  u8 unk78e8[8];
  s32 *script;
};

struct BattleEffectArgument;
void BattleEffect_RunMeteor(struct BattleEffectArgument *effect);
void BattleEffect_RunRamses(s32 *arg);
void BattleEffect_RunNereid(s32 *arg);
void BattleEffect_RunKirin(s32 *arg);
void BattleEffect_RunAtalanta(s32 *arg);
void BattleEffect_RunCybele(s32 *arg);
void BattleEffect_RunNeptune(s32 *arg);
void BattleEffect_RunTiamat(s32 *arg);
void BattleEffect_RunProcne(s32 *arg);
void BattleEffect_RunJudgment(s32 *arg);
void BattleEffect_RunBoreas(s32 *arg);
void BattleEffect_RunThor(s32 *arg);
void BattleEffect_RunZagan(s32 *arg);
void BattleEffect_RunMegaera(s32 *arg);
void BattleEffect_RunFlora(s32 *arg);
void BattleEffect_RunMoloch(s32 *arg);
void BattleEffect_RunUlysses(s32 *arg);
void BattleEffect_RunHaures(s32 *arg);
void BattleEffect_RunEclipse(s32 *arg);
void BattleEffect_RunCoatlicue(s32 *arg);
void BattleEffect_RunDaedalus(s32 *arg);
void BattleEffect_RunAzul(s32 *arg);
void BattleEffect_RunCatastrophe(s32 *arg);
void BattleEffect_RunCharon(s32 *arg);
void BattleEffect_RunIris(s32 *arg);
void BattleEffect_RunDaedalusAlt(s32 *arg);
void BattleEffect_RunTrident(s32 *arg);
void BattleEffect_RunCruelRuin(s32 *arg);
void BattleEffect_RunMeteorBlow(s32 *arg);

extern void (*gData_08197aa0[])(s32 *script);
extern u8 gMapCellBuffer[];

/// Runs the battle effect of mode *arg (1 to 29; 0 and 11 the default one)
/// with the effect's three work blocks taken.
void BattleEffect_RunSceneEffect(s32 *arg) {
  struct FxRunWork *work;

  HeapSlot_Alloc(100, 0x302);
  HeapSlot_Alloc(92, 0x7b7c);
  HeapSlot_Alloc(96, 0x4000);
  /* FAKEMATCH: the one-pass block loads the work pointer before the 0x78f0
   * offset is started, as in BattleEffect_RunTableEffect (differ 2 without it). */
  do {
    work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  } while (0);
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
  /* FAKEMATCH: the one-pass block loads the work pointer before the 0x78f0
   * offset is started (differ 2 without it). */
  do {
    work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  } while (0);
  work->script = script;
  work->active = 1;
  /* FAKEMATCH: the one-pass block loads the handler table before the script's
   * first word and sets the argument after the index (differ 2 without it). */
  do {
    gData_08197aa0[*script - 1](script);
  } while (0);
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

    /* FAKEMATCH: the one-pass blocks read the control word before the mask is
     * built and store it before the next mask (differ 4 without them). */
    do {
      control = channel[5];
    } while (0);
    channel[5] = control & 0xc5ff;
    channel[5] &= 0x7fff;
    /* FAKEMATCH: the native stores the control word before it starts the window
     * address (differ 2 without the barrier). */
    __asm__("");
    (void)channel[5];
    Dma_Set(gMapCellBuffer + 0x82, (void *)&REG_WIN0H,
            DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
  }
}
