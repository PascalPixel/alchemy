#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8       gData_02003dba[];
struct ObjectRuntime *Object_GetById(s32 id);
void                  Func_02003bcc(s32 value);
void                  Func_02002e30(const u8 *data);
void                  Func_02003b3c(s32 id);
void                  Func_020036f8(s32 a, s32 b, s32 owner);

void Func_020030a4(s32 id)
{
    Object_GetById(id)->unk5c[0] = 3;
    Func_02003b3c(id);
}
