#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern struct Actor *gData_02003600[];
extern u16 gData_02003702;
void Func_02000060(s32 value);
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_0200013c(void) {
  s32 flag = 514;

  /* FAKEMATCH(barrier): an asm operand that says flag changes, which it does
   * not, so the flag is built again for the call; as the same statements with
   * no asm, not measured. */
  asm("" : "+l"(flag));
  if (!Event_GetFlag(flag)) {
    Event_SetFlag(514);
    Func_02000060(1);
  }
}

void Func_02000160(void) {
  s32 flag = 514;

  /* FAKEMATCH(barrier): an asm operand that says flag changes, which it does
   * not, so the flag is built again for the call; as the same statements with
   * no asm, not measured. */
  asm("" : "+l"(flag));
  if (Event_GetFlag(flag) == 1) {
    Event_ClearFlag(514);
    Func_020000b4();
  }
  Func_02000100();
}

void Func_02000184(void) {
  System_Wait(10);
  Func_02000100();
}
