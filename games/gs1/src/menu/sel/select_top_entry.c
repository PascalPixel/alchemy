#include "types.h"
#include "scene.h"
#include "abi/menu/sel/select_top_entry.h"
#include "gs1_edition.h"

extern s8 gRom[];
extern s8 gRom2[];

void *Menu_Run(void);
void Menu_AppendResourceEntry(s32 arg0);

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

    if (Menu_Check(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    tbl = gRom;
    ofs = triple << 1;
    sel = TblGet(tbl, sel + ofs) - 1;
    if (sel < 0) {
        sel = 0;
    }

    Menu_Run();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Menu_Place(17, SELECT_MENU_WIDTH, 0);
    ret = Menu_Check2(sel);
    Menu_Run2();

    if (ret >= 0) {
        ret = gRom2[ret + ofs + 1];
    }

    return ret;
}
