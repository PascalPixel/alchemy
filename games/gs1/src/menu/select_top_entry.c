#include "types.h"
#include "gs1_edition.h"

extern s8 Data_08037403[];
extern s8 Data_080373f7[];

s32 Func_08077290(s32);
void *Func_080284dc(void);
void Menu_AppendResourceEntry(s32 arg0);
void Func_08028808(s32, s32, s32);
s32 Func_08028574(s32);
void Func_0802851c(void);

static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
    return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
    s32 group = 0;
    s32 ofs;
    s32 triple;
    s32 ret;
    s8 *tbl;

    if (Func_08077290(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = Data_08037403;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    Func_080284dc();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Func_08028808(17, SELECT_MENU_WIDTH, 0);
    ret = Func_08028574(sel);
    Func_0802851c();

    if (ret >= 0) {
        ret = Data_080373f7[ret + ofs + 1];
    }

    return ret;
}
