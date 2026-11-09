#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
u8 *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);

s32 Func_02003604(void) {
  u8 *state = Unit_Get(gMainRoutineWork.currentOwner);
  s32 sum = 0;
  u32 i;

  for (i = 0; i < 14; i++)
    sum += state[i];
  return sum & 15;
}
