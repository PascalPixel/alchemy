#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

extern const u8 gData_02003dba[];
struct Actor *Actor_GetById(s32 id);
void Func_02003bcc(s32 value);
void Func_02002e30(const u8 *data);
void Func_02003b3c(s32 id);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02003784(s32 a, s32 b) {
  Func_020036f8(a, b, gSequenceParentWork.currentOwner);
}
