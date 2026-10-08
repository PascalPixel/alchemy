#include "types.h"

struct InputState {
    u32 held;
    u32 pressed;
    u32 released;
    u32 repeat;
};

extern volatile struct InputState gInput;
s32 GameFlag_Test(s32);
void GameFlag_SetBit(s32);
void GameFlag_ClearBit(s32);

s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor)
{
    s32 *row = &cursor[1];

    if (gInput.repeat & 1) {
        s32 flag = ((*page << 4) + *row << 4) + cursor[0];

        if (GameFlag_Test(flag))
            GameFlag_ClearBit(flag);
        else
            GameFlag_SetBit(flag);
        return 1;
    }
    if ((gInput.pressed & 2) || (gInput.repeat & 4))
        return -1;
    if (gInput.repeat & 0x40) {
        if (--*row < 0)
            *row = 15;
    } else if (gInput.repeat & 0x80) {
        if (++*row > 15)
            *row = 0;
    } else if (gInput.repeat & 0x20) {
        if (--cursor[0] < 0)
            cursor[0] = 15;
    } else if (gInput.repeat & 0x10) {
        if (++cursor[0] > 15)
            cursor[0] = 0;
    } else if ((gInput.repeat & 0x200) && (gInput.repeat & 8)) {
        *page -= 10;
        if (*page < 0)
            *page = 15;
        return 1;
    } else if ((gInput.repeat & 0x100) && (gInput.repeat & 8)) {
        *page += 10;
        if (*page > 15)
            *page = 0;
        return 1;
    } else if (gInput.repeat & 0x200) {
        if (--*page < 0)
            *page = 15;
        return 1;
    } else if (gInput.repeat & 0x100) {
        if (++*page > 15)
            *page = 0;
        return 1;
    }
    return 0;
}

void Menu_ReservedNoOp(void)
{
}
