#include "TYPES.H"

/* 0xff-terminated index orders for the list modes; mode 0 is the default. */
extern u8 Menu_ListOrderDefault[];
extern u8 Menu_ListOrderMode0[];
extern u8 Menu_ListOrderMode1[];
extern u8 Menu_ListOrderMode2[];

/* Copies the index order for a list mode into order, terminator included,
   at most 32 entries. */
void Menu_CopyListOrder(s32 mode, u8 *order)
{
    u8 *src;
    s32 count;

    src = Menu_ListOrderDefault;
    switch (mode) {
    case 0:
        src = Menu_ListOrderMode0;
        break;
    case 1:
        src = Menu_ListOrderMode1;
        break;
    case 2:
        src = Menu_ListOrderMode2;
        break;
    }
    *order = *src;
    count = 0;
    if (*order != 0xff) {
        do {
            if (++count > 31)
                break;
            src++;
            order++;
            *order = *src;
        } while (*order != 0xff);
    }
}
