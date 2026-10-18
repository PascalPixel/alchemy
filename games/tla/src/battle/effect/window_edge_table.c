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
void BattleFx_RunMeteor(struct BattleEffectArgument *effect);
void BattleFx_RunRamses(s32 *arg);
void BattleFx_RunNereid(s32 *arg);
void BattleFx_RunKirin(s32 *arg);
void BattleFx_RunAtalanta(s32 *arg);
void BattleFx_RunCybele(s32 *arg);
void BattleFx_RunNeptune(s32 *arg);
void BattleFx_RunTiamat(s32 *arg);
void BattleFx_RunProcne(s32 *arg);
void BattleFx_RunJudgment(s32 *arg);
void BattleFx_RunBoreas(s32 *arg);
void BattleFx_RunThor(s32 *arg);
void BattleFx_RunZagan(s32 *arg);
void BattleFx_RunMegaera(s32 *arg);
void BattleFx_RunFlora(s32 *arg);
void BattleFx_RunMoloch(s32 *arg);
void BattleFx_RunUlysses(s32 *arg);
void BattleFx_RunHaures(s32 *arg);
void BattleFx_RunEclipse(s32 *arg);
void BattleFx_RunCoatlicue(s32 *arg);
void BattleFx_RunDaedalus(s32 *arg);
void BattleFx_RunAzul(s32 *arg);
void BattleFx_RunCatastrophe(s32 *arg);
void BattleFx_RunCharon(s32 *arg);
void BattleFx_RunIris(s32 *arg);
void BattleFx_RunDaedalusAlt(s32 *arg);
void BattleFx_RunTrident(s32 *arg);
void BattleFx_RunCruelRuin(s32 *arg);
void BattleFx_RunMeteorBlow(s32 *arg);

extern void (*gData_08197aa0[])(s32 *script);
extern u8 gMapCellBuffer[];

// Runs the battle effect of mode *arg (1 to 29; 0 and 11 the default one)
// with the effect's three work blocks taken.
void BattleFx_RunSceneEffect(s32 *arg) {
  struct FxRunWork *work;

  Runtime_AllocateHeapBlock(100, 0x302);
  Runtime_AllocateHeapBlock(92, 0x7b7c);
  Runtime_AllocateHeapBlock(96, 0x4000);
  /* FAKEMATCH: the one-pass block loads the work pointer before the 0x78f0
   * offset is started, as in BattleFx_RunTableEffect (differ 2 without it). */
  do {
    work = (struct FxRunWork *)Ram_HeapSlots->battleFxWork[0];
  } while (0);
  work->script = arg;
  work->active = 1;
  if (*arg == 0) {
    BattleFx_RunMeteor((struct BattleEffectArgument *)arg);
  } else {
    switch (*arg) {
    case 1:
      BattleFx_RunRamses(arg);
      break;
    case 2:
      BattleFx_RunNereid(arg);
      break;
    case 3:
      BattleFx_RunKirin(arg);
      break;
    case 4:
      BattleFx_RunAtalanta(arg);
      break;
    case 5:
      BattleFx_RunCybele(arg);
      break;
    case 6:
      BattleFx_RunNeptune(arg);
      break;
    case 7:
      BattleFx_RunTiamat(arg);
      break;
    case 8:
      BattleFx_RunProcne(arg);
      break;
    case 9:
      BattleFx_RunJudgment(arg);
      break;
    case 10:
      BattleFx_RunBoreas(arg);
      break;
    case 11:
      BattleFx_RunMeteor((struct BattleEffectArgument *)arg);
      break;
    case 12:
      BattleFx_RunThor(arg);
      break;
    case 13:
      BattleFx_RunZagan(arg);
      break;
    case 14:
      BattleFx_RunMegaera(arg);
      break;
    case 15:
      BattleFx_RunFlora(arg);
      break;
    case 16:
      BattleFx_RunMoloch(arg);
      break;
    case 17:
      BattleFx_RunUlysses(arg);
      break;
    case 18:
      BattleFx_RunHaures(arg);
      break;
    case 19:
      BattleFx_RunEclipse(arg);
      break;
    case 20:
      BattleFx_RunCoatlicue(arg);
      break;
    case 21:
      BattleFx_RunDaedalus(arg);
      break;
    case 22:
      BattleFx_RunAzul(arg);
      break;
    case 23:
      BattleFx_RunCatastrophe(arg);
      break;
    case 24:
      BattleFx_RunCharon(arg);
      break;
    case 25:
      BattleFx_RunIris(arg);
      break;
    case 26:
      BattleFx_RunDaedalusAlt(arg);
      break;
    case 27:
      BattleFx_RunTrident(arg);
      break;
    case 28:
      BattleFx_RunCruelRuin(arg);
      break;
    case 29:
      BattleFx_RunMeteorBlow(arg);
      break;
    }
  }
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(92);
  Runtime_ReleaseHeapBlock(100);
}

// Runs one battle effect script: takes the effect's three work blocks, calls
// the handler its first word selects, then gives the blocks back.
void BattleFx_RunTableEffect(s32 *script) {
  struct FxRunWork *work;

  Runtime_AllocateHeapBlock(100, 0x302);
  Runtime_AllocateHeapBlock(92, 0x7b7c);
  Runtime_AllocateHeapBlock(96, 0x4000);
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
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(92);
  Runtime_ReleaseHeapBlock(100);
}

void BattleFx_BuildWindowEdgeTable(void) {
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
    volatile u16 *channel = (volatile u16 *)0x040000b0;
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
    Dma_Set(gMapCellBuffer + 0x82, (void *)0x04000040, 0xa2600001,
            (volatile u32 *)channel);
  }
}
