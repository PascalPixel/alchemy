#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

void Battle_WaitMode0();
void Func_020028ac();
void Func_020028c4();
void Func_0200291c();
void Inventory_PromptAndSetObjectMode();
void UiText_OpenMessageAtObject();

void Func_02000154(s32 arg0)
{
    s32 result0;
    s32 result1;

    Call1((void (*)())Func_020028ac, (s32)MsgAreYouTheWarriors);
    Value2((s32 (*)())UiText_OpenMessageAtObject, arg0, 0);
    result0 = Value0((s32 (*)())Func_0200291c);
    result1 = Value2((s32 (*)())Inventory_PromptAndSetObjectMode, result0, 0);
    if (result1 != 0) goto L_02008184;
    Value1((s32 (*)())Battle_WaitMode0, 10);
    Call1((void (*)())Func_020028ac, ((s32)MsgAreYouTheWarriors + 1));
    goto L_02008190;
L_02008184:
    Value1((s32 (*)())Battle_WaitMode0, 20);
    Call1((void (*)())Func_020028ac, ((s32)MsgAreYouTheWarriors + 2));
L_02008190:
    Call2((void (*)())Func_020028c4, arg0, 0);
    return;
}

void Func_020001a0(s32 arg0)
{
    s32 result0;
    s32 result1;

    Call1((void (*)())Func_020028ac, (s32)MsgDoYouSeek);
    Value2((s32 (*)())UiText_OpenMessageAtObject, arg0, 0);
    result0 = Value0((s32 (*)())Func_0200291c);
    result1 = Value2((s32 (*)())Inventory_PromptAndSetObjectMode, result0, 0);
    if (result1 != 0) goto L_020081d0;
    Value1((s32 (*)())Battle_WaitMode0, 10);
    Call1((void (*)())Func_020028ac, ((s32)MsgDoYouSeek + 1));
    goto L_020081dc;
L_020081d0:
    Value1((s32 (*)())Battle_WaitMode0, 20);
    Call1((void (*)())Func_020028ac, ((s32)MsgDoYouSeek + 2));
L_020081dc:
    Call2((void (*)())Func_020028c4, arg0, 0);
    return;
}
