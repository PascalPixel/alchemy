#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

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
  /* FAKEMATCH: ⚓️ sets the actor between the halves of the position */
  asm("" : "+l"(x), "+l"(z), "+l"(id));
  Actor_SetHorizontalPositionWithTerrain(id, x << 17, z << 17);
  Func_020006ec();
}

void Func_02000610(void) { *((u8 *)Ram_HeapSlots->effectWork + 53) = 1; }
