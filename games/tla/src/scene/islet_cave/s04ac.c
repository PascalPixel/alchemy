#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "scene_tables.h"

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_020000f4(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_0200129c(s32 id, s32 x, s32 z);
void Func_020012bc(s32 id, s32 a);
void Func_020012ec(s32 a, s32 b);
void Func_020012fc(s32 a);
void Func_0200132c(void);
void Func_02001334(void);

void Func_020004ac(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 effect = work->screenEffect;

  Func_02000178();
  Func_0200132c();
  wait(40);
  Func_02000158();
  Func_020012ec(0x10000, 1);
  Func_020012fc(16);
  wait(16);
  /* FAKEMATCH: the loop note ends the scheduling block: the native loads
   * gMainRoutineWork's address before it builds the field offset (2 differ
   * without it). */
  do {
  } while (0);
  gMainRoutineWork.unk2c2 = 0;
  Func_020012bc(gMainRoutineWork.currentOwner, 0);
  work->screenEffect = effect;
}

void Func_02000508(void) {
  Func_020000f4();
  Func_02001334();
  /* FAKEMATCH: the loop note ends the scheduling block: the native loads
   * gMainRoutineWork's address before it builds the field offset (one row late
   * without it). */
  do {
  } while (0);
  gMainRoutineWork.unk2c2 = 1;
  FieldEvent_Wait(40);
  Func_0200129c(gMainRoutineWork.currentOwner, PIXELS(232), PIXELS(136));
  Func_020004ac();
}

void Func_02000544(void) {
  Func_020000f4();
  Func_02001334();
  /* FAKEMATCH: the loop note ends the scheduling block: the native loads
   * gMainRoutineWork's address before it builds the field offset (one row late
   * without it). */
  do {
  } while (0);
  gMainRoutineWork.unk2c2 = 1;
  FieldEvent_Wait(40);
  Func_0200129c(gMainRoutineWork.currentOwner, PIXELS(240), PIXELS(480));
  Func_020004ac();
}
