#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "object_runtime.h"

extern const u8 Data_02000318[];
extern const u8 Data_02000270[];
struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_AttachWorkTargetToObject(s32 id, s32 mode);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 mode);
void ObjectMotion_Launch(s32 id, s32 target, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_020001b4(void);
void Func_020001bc(void);
void Func_020001e4(s32 message);
void Func_020001ec(s32 id, s32 value);
void Func_020001f4(s32 id, s32 value);
void Func_02000204(s32 speed, s32 acceleration);
void Func_0200020c(void);
void Func_02000214(s32 value);
s32 Func_0200021c(void);

void Func_02000074(void)
{
    if (!GameFlag_Test(2219)) {
        Func_020001b4();
        Func_02000214(0);
        Func_020001e4((s32)MsgKibomboWarriorsFound);
        Func_02000204(0x10000, 0x2000);
        Object_AttachWorkTargetToObject(8, 1);
        Func_0200020c();
        ObjectMotion_SetAngleToward(10, 4, 2);
        {
            s32 half = 129;
            s32 id = 10;

            /* FAKEMATCH: ⚓️ sets the actor between the halves of the value */
            asm("" : "+l"(half), "+l"(id));
            Func_020001f4(id, half << 1);
        }
        ObjectMotion_Launch(10, 4, 15);
        Func_020001ec(10, 0);
        ObjectMotion_SetAngleToward(11, 4, 0);
        ObjectMotion_SetAngleToward(12, 4, 0);
        ObjectMotion_SetAngleToward(13, 4, 0);
        ObjectMotion_Launch(11, 2, 15);
        Func_020001ec(11, 0);
        ObjectMotion_SetAngleToward(12, 11, 0);
        Motion_SetModeAndWaitAnimation(12, 4);
        Func_020001ec(12, 0);
        ObjectMotion_SetAngleToward(12, 4, 0);
        Func_020001ec(12, 0);
        Motion_SetModeAndWaitAnimation(13, 3);
        Func_020001ec(13, 0);
        Func_020001bc();
    }
    GameFlag_SetBit(2447);
}

s32 Func_02000154(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 513;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    return 0;
}

s32 Func_02000184(void)
{
    Object_GetById(Func_0200021c())->priority_flags |= 32;
    return 0;
}
