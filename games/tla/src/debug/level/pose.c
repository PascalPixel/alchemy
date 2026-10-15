#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

extern const u8 Data_02000f10[];
extern const u8 Data_02000ebc[];
u8 *UnitGet(s32 owner);
void UnitCalc(s32 owner);
void Func_02000724(void);
void Func_020006e4(void);
void Func_020006ec(void);
void Func_0200072c(s32 value);
void Func_020006fc(s32 id, s32 x, s32 z);
void Func_020006a4(void);
void Func_020006ac(void);

void Func_020005d8(void)
{
    s32 x;
    s32 z;
    s32 id;

    Func_020006e4();
    Func_0200072c(0);
    Object_SetModeById(10, 2);
    Object_SetModeById(11, 4);
    ObjectMotion_WaitForAnimationChange(10);
    x = 156;
    z = 208;
    id = 11;
    /* FAKEMATCH: ⚓️ sets the actor between the halves of the position */
    asm("" : "+l"(x), "+l"(z), "+l"(id));
    Func_020006fc(id, x << 17, z << 17);
    Func_020006ec();
}

void Func_02000610(void)
{
    *((u8 *)Ram_HeapSlots->effect_work + 53) = 1;
}
