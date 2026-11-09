#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020000f4(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_0200132c(void);
void Func_02001334(void);

void Func_020004ac(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 effect = work->screenEffect;

  Func_02000178();
  Func_0200132c();
  wait(40);
  Func_02000158();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  wait(16);

  gMainRoutineWork.unk2c2 = 0;
  Actor_ConfigureSpriteColor(gMainRoutineWork.currentOwner, 0);
  work->screenEffect = effect;
}

void Func_02000508(void) {
  Func_020000f4();
  Func_02001334();

  gMainRoutineWork.unk2c2 = 1;
  Event_Wait(40);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         PIXELS(232), PIXELS(136));
  Func_020004ac();
}

void Func_02000544(void) {
  Func_020000f4();
  Func_02001334();

  gMainRoutineWork.unk2c2 = 1;
  Event_Wait(40);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                         PIXELS(240), PIXELS(480));
  Func_020004ac();
}
