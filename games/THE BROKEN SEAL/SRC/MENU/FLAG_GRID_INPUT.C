#include "TYPES.H"

extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);

/* Flag grid input (debug menu): the cursor picks a flag on a 16x16 page (column,
   row); A toggles it, B or Select leaves, the D-pad moves the cursor and L/R
   turn pages, ten at a time with Start held. Returns 1 when the page or a
   flag changed. */
s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor)
{
    s32 *row = &cursor[1];

    if (Data_03001b04 & 1) {
        s32 flag = ((*page << 4) + *row << 4) + cursor[0];

        if (GameFlag_TestFar(flag))
            GameFlag_ClearBitFar(flag);
        else
            GameFlag_SetBitFar(flag);
        return 1;
    }
    if ((Data_03001c94 & 2) || (Data_03001b04 & 4))
        return -1;
    if (Data_03001b04 & 0x40) {
        if (--*row < 0)
            *row = 15;
    } else if (Data_03001b04 & 0x80) {
        if (++*row > 15)
            *row = 0;
    } else if (Data_03001b04 & 0x20) {
        if (--cursor[0] < 0)
            cursor[0] = 15;
    } else if (Data_03001b04 & 0x10) {
        if (++cursor[0] > 15)
            cursor[0] = 0;
    } else if ((Data_03001b04 & 0x200) && (Data_03001b04 & 8)) {
        *page -= 10;
        if (*page < 0)
            *page = 15;
        return 1;
    } else if ((Data_03001b04 & 0x100) && (Data_03001b04 & 8)) {
        *page += 10;
        if (*page > 15)
            *page = 0;
        return 1;
    } else if (Data_03001b04 & 0x200) {
        if (--*page < 0)
            *page = 15;
        return 1;
    } else if (Data_03001b04 & 0x100) {
        if (++*page > 15)
            *page = 0;
        return 1;
    }
    return 0;
}

