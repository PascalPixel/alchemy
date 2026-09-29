/*
 * main:08028e54 Menu_RunConfirmSelectionAt - draft; the range links as
 * disassembly (recon/tbs/raw/08028e54.s).
 *
 * Remaining: the reference loads the layout's last argument, 0x24, from the
 * literal pool before the first call and keeps it in r8 across the calls.
 * Written as the number below, GCC materialises it with mov r3, #36 at the
 * layout call, so the r8 save, restore and pool word disappear. The unit
 * matched only while 0x24 was the address of a link-time symbol.
 */
#include "TYPES.H"
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 arg0);
void Menu_LayoutResourceEntries(s32 a0, s32 a1, s32 a2, s32 a3);

s32 Menu_RunConfirmSelectionAt(s32 arg0, s32 arg1, s32 arg2)
{
    s32 ret;

    ret = arg2;
    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(5);
    Menu_AppendResourceEntry(6);
    Menu_LayoutResourceEntries(arg0, arg1, 3, 0x24);
    ret = Menu_RunResourceSelectionLoop(ret);
    Menu_EndResourceSelection();
    if (ret == -1) {
        ret = 1;
    }
    return ret;
}
