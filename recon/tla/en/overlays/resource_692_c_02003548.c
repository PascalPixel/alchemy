/* Draft, differ 2 of 85: the native loads the first script address (ldr r6) before it builds the
 * flag 0x302 in r5; sched2 here puts the three-insn constant first (its chain is one longer than the
 * pool load's). The first flag call goes through call.h's Call1 so the block builds 0x302 again.
 * Tried: the script in a local behind a register-operand barrier, volatile or not (46), an inline
 * wrapper or Call3 for the eight calls (69, the flag is rebuilt at each). Overlay 688's
 * Func_02000060 shows the same pool-load-first order. */
#include "../../../../games/tla/src/field/overlay_692/local.h"
#include "call.h"

extern const u8 Data_0200525c[];
extern const u8 Data_020052bc[];
extern const u8 Data_0200531c[];
extern const u8 Data_0200537c[];

void Object_SetModeById(u32 id, s32 mode);
void Func_02000800(s32 id, const u8 *script, s32 flag);

void Func_02003548(void)
{
    Call1((void (*)())GameFlag_SetBit, 0x302);
    if (gPartyState.scene == (s32)&SceneId_Ov692Room4) {
        Func_02000800(15, Data_0200525c, 0x302);
        Func_02000800(16, Data_0200525c, 0x302);
        Func_02000800(17, Data_020052bc, 0x302);
        Func_02000800(18, Data_020052bc, 0x302);
        Func_02000800(19, Data_0200531c, 0x302);
        Func_02000800(20, Data_0200531c, 0x302);
        Func_02000800(21, Data_0200537c, 0x302);
        Func_02000800(22, Data_0200537c, 0x302);
        if (Object_GetById(16)->step == 0) {
            Object_GetById(16)->step = 10;
            Object_GetById(18)->step = 10;
            Object_GetById(20)->step = 10;
            Object_GetById(22)->step = 10;
        }
        Object_SetModeById(23, 4);
        Object_SetModeById(24, 4);
        Object_SetModeById(25, 4);
        Object_SetModeById(26, 2);
    }
}
