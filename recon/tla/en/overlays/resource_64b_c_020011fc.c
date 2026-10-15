/* Draft of Func_020011fc (overlay 64B, 65 instructions), differ 2: the djinn
 * tutorial of the world map, ported from ☀️'s WorldMap_ShowDjinnTutorial. The one
 * difference: for the second message the native loads MsgNextILl before the
 * multiply (ldr r3; muls r0, r5); here the multiply comes first (sched2 priority
 * 76 against the load's 74, both waiting on the GameFlag_Test call).
 * Measured: the base before the product, (B - A) * flag, the message through an
 * inline helper (differ 22, the base kept in a saved register), two "+r" asm on a
 * base temporary (no change); a one-pass block or an asm volatile between a base
 * temporary and the product fixes the message but turns ObjectMotion_Launch's
 * argument setup to r1, r2, r0 (differ 2), with or without Call3. alchemy permute,
 * 4 minutes: nothing closer. */
#include "types.h"
#include "flag_call.h"
#include "text/msg_ids.h"

void Battle_WaitMode0(s32 frames);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void Menu_AnimateSelectionToEntry(s32 a, s32 b);
void ObjectMotion_Launch(s32 actor, s32 height, s32 frames);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004d48(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e00(void);
void Func_02004e28(void);
void Func_02004e30(s32 cue);

static __inline__ void Func_02004df8_(s32 value, s32 mode)
{
    Func_02004df8(value, mode);
}

void Func_020011fc(void)
{
    Func_02004db0();
    Func_02004df8_(0x10000, 6);
    Func_02004e00();
    Func_02004da8();
    Motion_SetVarCbAndRefresh(9, 2);
    Func_02004cd8(Flag_IsSet(0x175) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) + (s32)MsgAfterBringingA);
    Func_02004cf0(9, 0);
    Battle_WaitMode0(30);
    Func_02004e30(111);
    Menu_AnimateSelectionToEntry(0, 2);
    Flag_Clear(0x16f);
    Flag_Clear(0x171);
    Func_02004e28();
    ObjectMotion_Launch(9, 4, 30);
    Func_02004cd8(Flag_IsSet(0x175) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) + (s32)MsgNextILl);
    Func_02004cf0(9, 0);
    Flag_Clear(0x16f);
    Flag_Set(0x171);
    Func_02004e28();
    Battle_WaitMode0(30);
    Func_02004d48(9, 5);
}
