/* Near miss of Func_02000ce4 of overlay 692 (21 instructions, draft 19): the native widens the
 * argument to s16 and then to u16 after the first call (four shifts); the draft folds the two
 * conversions into one. An s16 parameter gives all four shifts but the first before the call;
 * an s16 local and a u16 inline parameter fold. */
#include "local.h"

void Field_DispatchTypeHandler(s32 type);
void Func_02004e70(s32 a, s32 b);
void Func_02004e78(s32 id, s32 a);
void Func_02004e80(void);
void Func_02004e88(void);

static __inline__ void Message(s32 id, u16 message)
{
    Func_02004e78(id, message);
}

void Func_02000ce4(s32 value)
{
    Func_02004e70(151, 1);
    Message(gPartyState.current_owner, (s16)value);
    Field_DispatchTypeHandler(1);
    Func_02004e80();
    Func_02004e88();
}
