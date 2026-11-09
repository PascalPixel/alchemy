#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02000160(s32 owner, s32 delta);

void Func_0200017c(s32 delta) {
  u16 owners[16];
  s32 count;
  s32 i;

  count = FieldParty_ListMembers(owners);
  for (i = 0; i < count; i++)
    Func_02000160(owners[i], delta);
}
