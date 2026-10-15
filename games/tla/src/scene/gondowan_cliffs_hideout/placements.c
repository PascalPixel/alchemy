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

const u8 *Func_0200004c(void)
{
    if (GameFlag_Test(2219))
        return Data_02000318;
    return Data_02000270;
}
