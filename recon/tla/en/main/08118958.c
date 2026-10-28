// Draft, differ 62 of 158: the battle test menu, ☀️'s DebugBattle_RunMenu
// (games/tbs/src/battle/battle.c) in ⚓️'s form: Down is read once on entry,
// the first encounter is 69, Start calls Func_08118954 (empty), B marks the
// later battles with gAdventure.battleRule24b = 5 instead of
// Battle_SetStart, and the preset loads after A. Left: register choice and
// pool placement: the native keeps all eight values in registers (gInput in
// r5, the dead Down flag's r7 reused for 0x80, the A mask's 1 kept in r10 for
// gData_03001110 = 1, loadedPreset in r11); here loadedPreset spills to the
// stack and gInput moves to r9. The native also reaches battleRule24b as
// gAdventure's address plus 0x24b loaded early, where here the sum is one
// pooled constant, and its literal pool sits at the end rather than inside
// the loop.
#include "types.h"
#include "input.h"
#include "adventure.h"

extern u8 gData_03001110;
extern u16 Data_02000436;

void Func_080ad090(void);
void Ui_LoadWindowGraphics(void);
void Func_08014b70(void);
void Task_ResetAll(void);
void Runtime_InitializeHeap(void);
void VramCache_Reset(void);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void Func_08118954(void);
s32 DebugParty_LoadPreset(s32 preset);
void Unit_CalculateFar(s32 unitId);
void Func_081197f0(s32 encounter);

void DebugBattle_RunMenu(void) {
  s32 startModeSet = 0;
  s32 down = gInput.held & 0x80;
  s32 encounter;
  s32 preset;
  s32 loadedPreset;

  Func_080ad090();
restart:
  Ui_LoadWindowGraphics();
  Func_08014b70();
  Task_ResetAll();
  Runtime_InitializeHeap();
  VramCache_Reset();
  GameFlag_SetBit(362);
  encounter = 69;
  if (down) {
    loadedPreset = -1;
    preset = 0;
    GameFlag_ClearBit(362);
    for (;;) {
      GameFlag_ClearBit(32);
      WaitFrames(1);
      for (;;) {
        if (gInput.repeat & 0x10)
          encounter++;
        if (gInput.repeat & 0x20)
          encounter--;
        if (gInput.repeat & 0x40)
          encounter -= 10;
        if (gInput.repeat & 0x80)
          encounter += 10;
        if (gInput.repeat & 0x100)
          preset++;
        if (gInput.repeat & 0x200)
          preset--;
        if (gInput.repeat & 1)
          break;
        if (gInput.repeat & 8)
          Func_08118954();
        if ((gInput.repeat & 2) || startModeSet)
          startModeSet = 1;
        WaitFrames(1);
      }
      if (preset != loadedPreset) {
        Func_080ad090();
        DebugParty_LoadPreset(preset);
        loadedPreset = preset;
      }
      if (gInput.held & 0x80)
        GameFlag_SetBit(364);
      Unit_CalculateFar(0);
      Data_02000436 = 0x2b;
      if (encounter == 28)
        GameFlag_SetBit(366);
      GameFlag_SetBit(354);
      if (startModeSet)
        gAdventure.battleRule24b = 5;
      Func_081197f0(encounter);
      gData_03001110 = 1;
      Ui_LoadWindowGraphics();
      Func_08014b70();
      Task_ResetAll();
      Runtime_InitializeHeap();
      VramCache_Reset();
    }
  }
  GameFlag_SetBit(354);
  Func_081197f0(69);
  goto restart;
}
