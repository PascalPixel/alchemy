#include "gabomba_statue_chamber.h"
#include "io_reg.h"
#include "event_inline.h"

void Func_02000100(void) {
  volatile u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);
  struct GabombaStatueChamberEventWork *work;

  Actor_SetModeById(8, 2);
  PaletteFade_ApplySource(0x10000, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 2);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  work = Ram_HeapSlots->eventWork;
  Func_0803ce64((s16)work->field1c4++, 0, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 2);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Actor_SetModeById(8, 1);
}
