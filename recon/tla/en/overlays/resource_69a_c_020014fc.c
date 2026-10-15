/*
 * Draft of overlay 69a Func_020014fc (twins: 6a3 Func_0200413c, 6a5
 * Func_02005194). 2 instructions differ: the listing loads the object's x
 * before it copies the 0x40 mask (ldr r6, [r0, #8]; adds r3, r2), the draft
 * after. Tried: the OR and the AND with their operands swapped (26 to 47
 * differ, or the same 2).
 */
#include "types.h"
#include "party_state.h"
#include "field_event.h"
#include "call.h"

struct LightSwitch {
    u8 unk00[6];
    s16 x;
    s16 z;
};

struct FieldActor *Object_GetById();
void Func_02001474();
void Func_02001bd4();
void Func_02001adc();
void Battle_WaitMode0();

void Func_020014fc(struct LightSwitch *light)
{
    struct FieldActor *object = Object_GetById(gPartyState.currentOwner);
    s32 x = (object->x >> 20) | (s16)(0x40 & light->x);
    s32 z = (object->z >> 20) | (s16)(0x40 & light->z);

    Func_02001474();
    Func_02001bd4(161);
    Call6(Func_02001adc, light->x, light->z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}
