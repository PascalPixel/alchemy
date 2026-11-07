#include "types.h"
#include "input.h"
#include "main_routine_work.h"

extern u8 gData_03001110;
extern u16 Data_02000436;
extern char ResourceId_ColossoBackdrop;

void Func_080ad090(void);
void Window_LoadGraphics(void);
void Func_08014b70(void);
void Task_ResetAll(void);
void Heap_Initialize(void);
void VramCache_Reset(void);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void Func_08118954(void);
s32 DebugParty_LoadPreset(s32 preset);
void Unit_CalculateFar(s32 unitId);
void Func_081197f0(s32 encounter);

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
  Func_08014b70();
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
        /* FAKEMATCH(dead): the || startModeSet test kept; as if (input->repeat
         * & 2), 47 of 158 instructions differ in tla-en. */
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
      Func_081197f0(encounter);
      gData_03001110 = 1;
      Window_LoadGraphics();
      Func_08014b70();
      Task_ResetAll();
      Heap_Initialize();
      VramCache_Reset();
    }
  }
  GameFlag_SetBit(354);
  Func_081197f0(69);
  goto restart;
}
