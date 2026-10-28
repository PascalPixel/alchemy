#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void GameFlag_Test();
void Func_02001094();
void Func_020010ac();
void Func_02000d28(void);

void Func_02000d10(void) {
  s32 result0;

  result0 = Value1((s32 (*)())GameFlag_Test, 0x9af);
  if (result0 == 0)
    goto L_02008d24;
  ((void (*)())Func_02000d28)();
L_02008d24:
  return;
}

void Func_02000d28(void) {
  Call6((void (*)())Func_02001094, 35, 12, 18, 12, 3, 3);
  Call6((void (*)())Func_020010ac, 35, 12, 3, 3, 18, 12);
  Call6((void (*)())Func_02001094, 84, 13, 83, 13, 1, 1);
  return;
}
