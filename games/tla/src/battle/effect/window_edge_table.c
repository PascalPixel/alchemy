#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "mem.h"

struct FxRunWork {
  u8 unk0000[0x78e4];
  s32 active;
  u8 unk78e8[8];
  s32 *script;
};

struct BattleEffectArgument;
void BattleFx_RunMeteor(struct BattleEffectArgument *effect);
void Func_08165ab4(s32 *arg);
void Func_08148458(s32 *arg);
void Func_08168a40(s32 *arg);
void Func_08153ebc(s32 *arg);
void Func_0814ce30(s32 *arg);
void Func_081693d0(s32 *arg);
void Func_08147ae0(s32 *arg);
void Func_08146e00(s32 *arg);
void Func_0816729c(s32 *arg);
void Func_0814db04(s32 *arg);
void Func_08158dc8(s32 *arg);
void Func_0814bee0(s32 *arg);
void Func_08170364(s32 *arg);
void Func_08179f18(s32 *arg);
void Func_08191d60(s32 *arg);
void Func_0815b764(s32 *arg);
void Func_08186dfc(s32 *arg);
void Func_0814153c(s32 *arg);
void Func_08182aa8(s32 *arg);
void Func_0817ea58(s32 *arg);
void Func_0815cde8(s32 *arg);
void Func_081843fc(s32 *arg);
void Func_08175f74(s32 *arg);
void Func_08192c9c(s32 *arg);
void Func_08180c94(s32 *arg);
void Func_0818cb74(s32 *arg);
void Func_0813fd84(s32 *arg);
void Func_0818df5c(s32 *arg);

extern void (*gData_08197aa0[])(s32 *script);
extern u8 gMapCellBuffer[];

// Runs the battle effect of mode *arg (1 to 29; 0 and 11 the default one)
// with the effect's three work blocks taken.
void Func_0814c994(s32 *arg) {
  struct FxRunWork *work;

  Runtime_AllocateHeapBlock(100, 0x302);
  Runtime_AllocateHeapBlock(92, 0x7b7c);
  Runtime_AllocateHeapBlock(96, 0x4000);
  /* FAKEMATCH: the one-pass block loads the work pointer before the 0x78f0
   * offset is started, as in Func_0814cb60 (differ 2 without it). */
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
      Func_08165ab4(arg);
      break;
    case 2:
      Func_08148458(arg);
      break;
    case 3:
      Func_08168a40(arg);
      break;
    case 4:
      Func_08153ebc(arg);
      break;
    case 5:
      Func_0814ce30(arg);
      break;
    case 6:
      Func_081693d0(arg);
      break;
    case 7:
      Func_08147ae0(arg);
      break;
    case 8:
      Func_08146e00(arg);
      break;
    case 9:
      Func_0816729c(arg);
      break;
    case 10:
      Func_0814db04(arg);
      break;
    case 11:
      BattleFx_RunMeteor((struct BattleEffectArgument *)arg);
      break;
    case 12:
      Func_08158dc8(arg);
      break;
    case 13:
      Func_0814bee0(arg);
      break;
    case 14:
      Func_08170364(arg);
      break;
    case 15:
      Func_08179f18(arg);
      break;
    case 16:
      Func_08191d60(arg);
      break;
    case 17:
      Func_0815b764(arg);
      break;
    case 18:
      Func_08186dfc(arg);
      break;
    case 19:
      Func_0814153c(arg);
      break;
    case 20:
      Func_08182aa8(arg);
      break;
    case 21:
      Func_0817ea58(arg);
      break;
    case 22:
      Func_0815cde8(arg);
      break;
    case 23:
      Func_081843fc(arg);
      break;
    case 24:
      Func_08175f74(arg);
      break;
    case 25:
      Func_08192c9c(arg);
      break;
    case 26:
      Func_08180c94(arg);
      break;
    case 27:
      Func_0818cb74(arg);
      break;
    case 28:
      Func_0813fd84(arg);
      break;
    case 29:
      Func_0818df5c(arg);
      break;
    }
  }
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(92);
  Runtime_ReleaseHeapBlock(100);
}

// Runs one battle effect script: takes the effect's three work blocks, calls
// the handler its first word selects, then gives the blocks back.
void Func_0814cb60(s32 *script) {
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
      if (right < 0)
        right = 0;
      if (right > 240)
        right = 240;
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
