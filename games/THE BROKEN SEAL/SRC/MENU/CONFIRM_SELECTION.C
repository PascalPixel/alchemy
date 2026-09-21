#include "TYPES.H"
void *Func_080284dc(void);
#define AffineEffect_InitializeWork Func_080284dc
void Func_0802851c(void);
#define Menu_EndResourceSelection Func_0802851c
s32 Func_08028574(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Func_08028808(s32, s32, s32);
extern u8 Data_00000024[];
void Func_080288a8(s32 a0, s32 a1, s32 a2, s32 a3);
#define Menu_LayoutResourceEntries Func_080288a8

s32 Menu_SelectEntry11To14(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x11);
    Menu_AppendResourceEntry(0x12);
    Menu_AppendResourceEntry(0x13);
    Menu_AppendResourceEntry(0x14);
    Func_08028808(0x11, 7, 0);
    ret = Func_08028574(arg0);
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
    Func_08028808(0x11, 0xA, 0);
    ret = Func_08028574(arg0);
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
    Func_08028808(flag, arg2, arg1);

    arg3 = Func_08028574(arg3);
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
    ret = Func_08028574(ret);
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
    Func_08028808(0x11, 9, 0);
    ret = Func_08028574(arg0);
    Menu_EndResourceSelection();
    return ret;
}
