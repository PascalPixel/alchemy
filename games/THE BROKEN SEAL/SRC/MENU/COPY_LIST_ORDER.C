#include "TYPES.H"

/* 0xff-terminated index orders for the list modes; mode 0 is the default. */
extern u8 Data_080af2a6[];
extern u8 Data_080af2d0[];
extern u8 Data_080af2bc[];
extern u8 Data_080af2b1[];

/* Copies the index order for a list mode into order, terminator included,
   at most 32 entries. */
void Menu_CopyListOrder(s32 mode, u8 *order)
{
    u8 *src;
    s32 count;

    src = Data_080af2a6;
    switch (mode) {
    case 0:
        src = Data_080af2d0;
        break;
    case 1:
        src = Data_080af2bc;
        break;
    case 2:
        src = Data_080af2b1;
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
