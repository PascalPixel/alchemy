#include "MENU_TEST.H"

u8 *SceneData_GetTable93c8(void)
{
    return MenuTest_CommandTable;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTable93f8(void)
{
    return MenuTest_CommandTableB;
}

u8 *SceneData_GetTable93fc(void)
{
    return MenuTest_CommandTableC;
}

void SceneDialogue_ShowMessageAndWait(s32 arg0)
{
    UiWork_FinalizePendingCore();
    UiText_OpenMessageWindow(arg0, 5, 0, 0x22);
    while (UiWork_IsComplete() == 0) {
        Engine_TaskWait(1);
    }
    Engine_TaskWait(1);
}

void CommandTable_RunDirectionalInput(s32 x, s32 cnt)
{
    s16 *tbl = Data_02000240;
    volatile s32 *key;
    s32 token;
    s32 i;

    *(u8 *)&tbl[262] = 2;
    token = UiWindow_CreateWithSideObject(125, 0, 0, 0);
    for (i = 0; i < cnt; i++) {
        key = &Data_03001ae8;
        UiWork_PushValueSlot(1, 1);
        UiWork_PushValueSlot(141, 2);
        UiWork_PushValueSlot(0x1e240, 5);
        SceneDialogue_ShowMessageAndWait(x);
        goto test;
retry:
        if (*key != 0) {
            goto next;
        }
        Engine_TaskWait(1);
test:
        if ((*key & 2) != 0) {
            goto end;
        }
        if ((*key & 1) != 0) {
            goto inc;
        }
        if ((*key & 0x80) == 0) {
            goto other;
        }
inc:
        x++;
        goto next;
other:
        if ((*key & 0x40) != 0) {
            x--;
            goto next;
        }
        goto retry;
next:;
    }
end:
    UiWork_FinalizePendingCore();
    Engine_DebugFinalizeWindow(token, 2);
}

void SceneState_ApplyBlockC9b(void)
{
    CommandTable_RunDirectionalInput((s32)&Value_00000c9b, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void SceneState_ApplyBlockCc6(void)
{
    CommandTable_RunDirectionalInput((s32)&Value_00000cc6, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}

void SceneState_ApplyBlockCf1(void)
{
    CommandTable_RunDirectionalInput((s32)&Value_00000cf1, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
