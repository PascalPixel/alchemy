#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

extern const u8 Data_020017d8[];
extern const u8 Data_020017a4[];
extern const u8 Data_0200171c[];
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const u8 *callback);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 value);
void Func_02001400(s32 a, s32 b, s32 c);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_020013f8(s32 value);
void Func_02000f38(void);

void Func_02000ffc(void)
{
    s32 back;
    s32 id;
    s32 x;

    Func_020012e0();
    Func_020013f8(0);
    back = 14;
    id = 4;
    x = 0;
    /* FAKEMATCH: ⚓️ sets the other arguments before it negates the step */
    asm("" : "+l"(back), "+l"(id), "+l"(x));
    ObjectMotion_CommitPositionAndActivate(id, x, -back);
    ObjectMotion_CommitPositionAndActivate(4, 16, 0);
    Battle_WaitMode0(10);
    Func_02000f38();
}

void Func_0200102c(void)
{
    s32 back;
    s32 id;
    s32 x;

    Func_020012e0();
    Func_020013f8(0);
    back = 14;
    id = 4;
    x = 0;
    /* FAKEMATCH: ⚓️ sets the other arguments before it negates the step */
    asm("" : "+l"(back), "+l"(id), "+l"(x));
    ObjectMotion_CommitPositionAndActivate(id, x, -back);
    ObjectMotion_CommitPositionAndActivate(4, -16, 0);
    Battle_WaitMode0(10);
    Func_02000f38();
}

void Func_0200105c(void)
{
    Func_020012e0();
    Func_020013f8(0);
    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Battle_WaitMode0(10);
    Func_02000f38();
}

void Func_02001080(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(20), 0);
}

void Func_020010a0(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 521;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
}
