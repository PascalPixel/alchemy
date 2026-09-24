#include "TYPES.H"

struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

extern u32 gBattleFxWork;
extern struct Position gBgScroll;

void BattleFx_AdvanceScrollOnInterval(void)
{
    u8 *base = (u8 *)gBattleFxWork;
    u32 *counter = (u32 *)(base + 0x7790);

    (*counter)++;
    if (*counter == *(u32 *)(base + 0x7794)) {
        gBgScroll.x += *(s32 *)(base + 0x7798);
        gBgScroll.y += *(s32 *)(base + 0x779C);
        *counter = 0;
    }
}
