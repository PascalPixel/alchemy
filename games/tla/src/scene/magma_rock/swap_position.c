#include "types.h"
#include "actor.h"
#include "adventure.h"

extern const u8 gData_02003dba[];
struct Actor *Actor_GetById(s32 id);
void Func_02003bcc(s32 value);
void Func_02002e30(const u8 *data);
void Func_02003b3c(s32 id);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02003224(struct Actor *object, struct Actor *source) {
  Actor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_SetPositionAndResetMotion(source, 0, 0, 0);
}
