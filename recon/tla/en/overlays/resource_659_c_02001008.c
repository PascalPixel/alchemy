/* Draft of overlay 659's Func_02001008, differ 2 of 292: after Object_GetById(4) the native
 * loads z before it builds the bound 0x5f0000, this draft builds it first (the load is
 * class 1 after the call, the constant class 2, at equal priority). The "+l" asm on the
 * object that fixes this in Func_02001310 moves the r2 argument of the earlier
 * ObjectMotion_ResetAndSetPositionInMode2 call instead (differ 2); with a second barrier
 * after that call, the r2 argument still comes late. Value and direct forms of the calls,
 * a z local, a one-pass block and alchemy permute (5 minutes) found nothing closer. */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0();
void Func_02001d50();
void Func_02001d58();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001eb8();
void Func_02001ec0();
void GameFlag_SetBit();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
struct ObjectRuntime *Object_GetById();
void Object_LinkObjectAndSetCallback();
void Object_LinkPair();
void Object_RefreshSelectorById();
void Object_SetModeById();
void PartyInventory_Remove();
extern u32 Data_02001f18[];
extern u32 Data_02001f4c[];
extern u32 Data_02001f90[];
extern u32 Data_02001fc4[];
extern u32 Data_02002054[];
extern u32 Data_020020e4[];

void Func_02001008(void)
{
    struct ObjectRuntime *object;

    Call1((void (*)())GameFlag_SetBit, 0x98a);
    Call1((void (*)())GameFlag_SetBit, 0x310);
    Call1((void (*)())PartyInventory_Remove, 0x1c0);
    ((void (*)())Func_02001d50)();
    Call1((void (*)())Func_02001eb8, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 11, 0x6666, 0x3333);
    Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 11, 0x19a, 109);
    object = Object_GetById(4);
    if (object->z > 0x5f0000)
        Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, object->x / 0x10000, 95);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 4, 0x16a, 95);
    Call2((void (*)())Func_02001e40, 4, 0x4000);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 11);
    Call1((void (*)())Func_02001e18, (s32)MsgScene659No110);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Value3((s32 (*)())Func_02001e48, 11, 0x100, 3);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 11, 0x17a, 109);
    Call3((void (*)())ObjectMotion_ArmCallback, 11, 0xa000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    Call3((void (*)())Object_LinkPair, 10, 11, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 4, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 10, 0x108, 30);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Value3((s32 (*)())Func_02001e48, 11, 0x102, 30);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    Call3((void (*)())ObjectMotion_SetAngleToward, 10, 11, 0);
    Call1((void (*)())Battle_WaitMode0, 30);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
    Call1((void (*)())Battle_WaitMode0, 30);
    Value3((s32 (*)())Func_02001e48, 10, 0x101, 30);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    Call2((void (*)())Func_02001e40, 11, 0x4000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Func_02001e40, 11, 0xc000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Call2((void (*)())Func_02001e40, 11, 0x8000);
    Call1((void (*)())Battle_WaitMode0, 10);
    Value3((s32 (*)())Func_02001e48, 11, 0x106, 30);
    Call1((void (*)())ObjectMotion_EnableActionAndResetMotion, 25);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x8000, 0x4000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 11, 0x8000, 0x4000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 25, 0x8000, 0x4000);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 11, (s32)Data_02001f18);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 4, (s32)Data_02001f4c);
    Call1((void (*)())Battle_WaitMode0, 60);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 10, (s32)Data_02001f90);
    Call1((void (*)())Object_RefreshSelectorById, 10);
    Call1((void (*)())Battle_WaitMode0, 120);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 11, (s32)Data_02001fc4);
    Call2((void (*)())Object_LinkObjectAndSetCallback, 4, 11);
    Call1((void (*)())Battle_WaitMode0, 60);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 10, (s32)Data_02002054);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 25, (s32)Data_020020e4);
    Call1((void (*)())Object_RefreshSelectorById, 10);
    Call1((void (*)())Object_RefreshSelectorById, 11);
    Call1((void (*)())Object_RefreshSelectorById, 25);
    Call2((void (*)())Motion_SetVarCbAndRefresh, 11, 2);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    Call3((void (*)())Func_02001ec0, 25, 2, 10);
    ((struct EventWork *)Ram_HeapSlots->event_work)->message_id += 1;
    Call2((void (*)())Object_SetModeById, 4, 1);
    Call3((void (*)())ObjectMotion_ArmCallback, 10, 0x4000, 0);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 10, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())ObjectMotion_ArmCallback, 4, 0, 0);
    Call3((void (*)())Func_02001e28, 10, 0, 10);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 11, 3);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call3((void (*)())Func_02001e28, 11, 0, 10);
    ((void (*)())Func_02001d58)();
}
