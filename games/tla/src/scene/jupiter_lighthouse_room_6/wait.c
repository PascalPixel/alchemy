#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern s32 gData_020023c0;
extern s16 gData_0200242a;
extern struct Actor *gData_0200242c[];
extern u16 gData_0200252e;
s32 Func_020018bc(s32 value);
void Func_020000b4(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Scene_SetArrivalFlags(s32 value);
void Func_020003a4(s32 value);

void Func_02000190(void) {
  s32 flag = 512;

  /* FAKEMATCH(barrier): an asm operand that says flag changes, which it does
   * not, so the flag is built again for the call; as the same statements with
   * no asm, not measured. */
  asm("" : "+l"(flag));
  if (!Event_GetFlag(flag)) {
    Event_SetFlag(512);
    Func_020000b4(1);
  }
}

void Func_020001b0(void) {
  s32 flag = 512;

  /* FAKEMATCH(barrier): an asm operand that says flag changes, which it does
   * not, so the flag is built again for the call; as the same statements with
   * no asm, not measured. */
  asm("" : "+l"(flag));
  if (Event_GetFlag(flag) == 1) {
    Event_ClearFlag(512);
    Func_02000108();
  }
  Func_02000154();
}

void Func_020001d0(void) {
  System_Wait(10);
  Func_02000154();
}
