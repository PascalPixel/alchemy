#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct Ov6a1EventWork {
    u8 unknown_000[0x1ac];
    s32 screen_effect;
};

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Func_02002248(s32 id);
void Func_02000af0(s32 id);
void Func_020018f4(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_02002010(s32 x, s32 z, s32 a);

s32 Func_02000110(void)
{
    ((struct Ov6a1EventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x204;
    Object_GetById(8)->scale_x = 0xc000;
    Object_GetById(8)->scale_y = 0xc000;
    ObjectMotion_SetActionVariant(12, 3);
    ObjectMotion_SetActionVariant(14, 3);
    Func_02002248(11);
    Func_02002248(12);
    Func_02002248(13);
    Func_02002248(14);
    Func_02002248(15);
    Func_02000af0(11);
    Func_02000af0(12);
    Func_02000af0(13);
    Func_02000af0(14);
    Func_02000af0(15);
    Func_020018f4(33, 0x280, 0x380, 0, 1);
    if (gPartyState.entrance == 13)
        Func_02002010(19, 0, 0x204);
    else if (gPartyState.entrance == 14)
        Func_02002010(-20, -22, 0x204);
    return 0;
}
