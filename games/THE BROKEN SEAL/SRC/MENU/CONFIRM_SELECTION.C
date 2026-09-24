#include "TYPES.H"
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Menu_CenterResourceEntries(s32, s32, s32);
extern u8 Data_00000024[];
void Menu_LayoutResourceEntries(s32 a0, s32 a1, s32 a2, s32 a3);

s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
    Menu_CenterResourceEntries(0x11, 7, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

s32 Menu_SelectEntry19To1c(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x19);
    Menu_AppendResourceEntry(0x1A);
    Menu_AppendResourceEntry(0x1B);
    Menu_AppendResourceEntry(0x1C);
    Menu_CenterResourceEntries(0x11, 0xA, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 flag;

    flag = 0;

    AffineEffect_InitializeWork();

    if (arg2 == 0) {
        arg2 = 3;
    }
    if (arg0 != 0) {
        flag = 17;
    }

    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_CenterResourceEntries(flag, arg2, arg1);

    arg3 = Menu_RunResourceSelectionLoop(arg3);
    Menu_EndResourceSelection();
    if (arg3 == -1) {
        arg3 = 1;
    }
    return arg3;
}

s32 Menu_RunConfirmSelectionAt(s32 arg0, s32 arg1, s32 arg2)
{
    s32 ret;
    u8 *p;

    ret = arg2;
    p = Data_00000024;
    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_LayoutResourceEntries(arg0, arg1, 3, p);
    ret = Menu_RunResourceSelectionLoop(ret);
    Menu_EndResourceSelection();
    if (ret == -1) {
        ret = 1;
    }
    return ret;
}

s32 Menu_SelectEntry20To21(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Menu_CenterResourceEntries(0x11, 9, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}
