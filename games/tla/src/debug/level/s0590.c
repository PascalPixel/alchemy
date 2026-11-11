/// @file
/// Overlay 0x6b7's code after level/finish.c, in ROM order.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

#include "scene_tables.h"

s32 Func_02000590(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  *(s32 *)(event + 428) = 516;
  *(s32 *)(event + 436) = 24;
  if (gMainRoutineWork.entrance <= 89) {
    Actor_GetById(11)->scaleY = 0x1b333;
    Actor_GetById(11)->scaleX = 0x1b333;
  }
  return 0;
}

extern const u8 gData_02000f10[];
extern const u8 gData_02000ebc[];
u8 *Unit_Get(s32 owner);
void Unit_Calculate(s32 owner);
void Func_020006e4(void);
void Func_020006ec(void);
void Func_020006a4(void);
void Func_020006ac(void);

void Func_020005d8(void) {
  s32 x;
  s32 z;
  s32 id;

  Func_020006e4();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 4);
  Actor_WaitForAnimationChange(10);
  x = 156;
  z = 208;
  id = 11;
  /* FAKEMATCH(barrier): x, z and the actor held at an empty asm, so the actor
   * is set between the halves of the position, as the ROM has it; as no asm,
   * 2 of 20 instructions differ in tla-en. */
  asm("" : "+l"(x), "+l"(z), "+l"(id));
  Actor_SetHorizontalPositionWithTerrain(id, x << 17, z << 17);
  Func_020006ec();
}

void Func_02000610(void) { *((u8 *)Ram_HeapSlots->effectWork + 53) = 1; }

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000624(void) { Func_020006a4(); }

void Func_0200062c(void) { Func_020006ac(); }
