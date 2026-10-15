#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0();
void Func_020005c4();
void Func_020005cc();
void Func_02000604();
void Func_02000614();
void Func_02000624();
void Func_0200062c();
void GameFlag_SetBit();
void GameFlag_Test();
void Inventory_PromptAndSetObjectMode();
void ObjectMotion_SetAngleToward();
void Object_SetModeById();
void UiText_OpenMessageAtObject();
void Func_020004dc(void);

void Func_020004b0(void)
{
    s32 result0;

    result0 = Value1((s32 (*)())GameFlag_Test, 0xa39);
    if (result0 != 0)
        goto L_020084c6;
    ((void (*)())Func_020004dc)();
    goto L_020084d4;
L_020084c6:
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors22);
    Call2((void (*)())Func_02000614, 18, 0);
L_020084d4:
    return;
}

void Func_020004dc(void)
{
    s32 result0;

    ((void (*)())Func_020005c4)();
    Call1((void (*)())Func_0200062c, 0);
    Call1((void (*)())Func_02000604, (s32)MsgMikasallaInteriors19);
    Value3((s32 (*)())Func_02000624, 18, 0x100, 60);
    Call3((void (*)())ObjectMotion_SetAngleToward, 18, gPartyState.currentOwner, 0);
    Call2((void (*)())Func_02000614, 18, 0);
    Value3((s32 (*)())Func_02000624, gPartyState.currentOwner, 0x100, 60);
    Call2((void (*)())Object_SetModeById, 18, 4);
    Call1((void (*)())Battle_WaitMode0, 15);
    Call2((void (*)())Func_02000614, 18, 0);
    Value3((s32 (*)())Func_02000624, 18, 0x108, 60);
    Value2((s32 (*)())UiText_OpenMessageAtObject, 18, 0);
    result0 = Value2((s32 (*)())Inventory_PromptAndSetObjectMode, gPartyState.currentOwner, 0);
    if (result0 != 0)
        goto L_02008568;
    Call2((void (*)())Func_02000614, 18, 0);
    Call1((void (*)())GameFlag_SetBit, 0x340);
    goto L_02008582;
L_02008568:
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call2((void (*)())Func_02000614, 18, 0);
L_02008582:
    Call1((void (*)())GameFlag_SetBit, 0xa39);
    Call1((void (*)())GameFlag_SetBit, 0x341);
    ((void (*)())Func_020005cc)();
    return;
}
