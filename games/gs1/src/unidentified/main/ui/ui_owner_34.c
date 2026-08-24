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

static __inline__ s32 TblGet(s8 *table, s32 index)
{
    return table[index];
}

s32 Func_08028920(s32 input)
{
    s32 group = 0;
    s32 offset;
    s32 triple;
    s32 value;
    s8 *table;

    if (Func_08077290(-1) == 0) {
        group = 1;
    }

    triple = group * 3;
    table = Data_08037403;
    offset = triple << 1;
    input = TblGet(table, input + offset) - 1;
    if (input < 0) {
        input = 0;
    }

    Func_080284dc();
    Menu_AppendResourceEntry(1);
    if (group == 0) {
        Menu_AppendResourceEntry(15);
    }
    Menu_AppendResourceEntry(2);
    Menu_AppendResourceEntry(7);
    Func_08028808(17, SELECT_MENU_WIDTH, 0);
    value = Func_08028574(input);
    Func_0802851c();

    if (value >= 0) {
        value = Data_080373f7[value + offset + 1];
    }

    return value;
}
