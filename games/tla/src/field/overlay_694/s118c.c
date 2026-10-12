#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 mode);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_0200118c(void)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = Object_GetById(15);
    s16 angle = object->angle;

    Func_0200406c();
    Func_02004204(0);
    BYTE_AT(object, 99) = 1;
    if (*(s16 *)(event + 358) == 0) {
        ObjectMotion_SetAngleToward(15, gPartyState.current_owner, 0);
        if (GameFlag_Test(0x9b2) != 0)
            Func_0200412c((s32)MsgScene694No143);
        else
            Func_0200412c((s32)MsgScene694No42);
    } else {
        if (GameFlag_Test(0x9b2) != 0)
            Func_0200412c((s32)MsgScene694No149);
        else
            Func_0200412c((s32)MsgScene694No50);
    }
    Func_02004144(15, 0);
    if (*(s16 *)(event + 358) == 0)
        object->angle = angle;
    BYTE_AT(object, 99) = 0;
    Func_02004074();
}
