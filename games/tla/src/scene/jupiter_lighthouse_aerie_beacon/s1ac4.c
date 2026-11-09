#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "main_routine_work.h"
#include "owner_state.h"
#include "event_inline.h"
s32 Func_020059c4();
s32 Func_020059dc();
s32 Func_020059d4();
s32 Func_020059cc();

void Func_02001ac4(s32 a0, s32 a1) {
  Func_020059c4();
  Func_020059dc(((s32)Actor_Get(a0)), 2);
  Event_PlaySound(0xc9);
  FieldEvent_Wait(60);
  Func_020059dc(((s32)Actor_Get(a0)), 0);
  Func_020059d4();
  Func_020059cc();
  Actor_SetAnimation(a1, 1);
}

