#include "types.h"

struct FlashInfo08006f84 {
    u8 unknown_00[4];
    s32 size;
    u8 shift;
    u8 unknown_09[7];
    u16 control;
};

u16 Func_08006d50(u16);
void Func_08006ac0(void *);
u16 Func_08006dec(u8 *, u8 *);

extern u16 Data_02004c0c;

u16 Func_08006f84(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u8 *destination;

    if ((u32)sector > 15)
        return 0x80FF;

    result = Func_08006d50(sector);
    if (result != 0)
        return result;

    Func_08006ac0(savedCode);
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) |
        (*(struct FlashInfo08006f84 **)0x02004C08)->control;
    Data_02004c0c =
        (*(struct FlashInfo08006f84 **)0x02004C08)->size;
    destination = (u8 *)(
        (sector << (*(struct FlashInfo08006f84 **)0x02004C08)->shift) +
        0x0E000000);

    while (Data_02004c0c != 0) {
        result = Func_08006dec(source, destination);
        if (result != 0)
            break;
        Data_02004c0c -= 1;
        source++;
        destination++;
    }

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return result;
}
