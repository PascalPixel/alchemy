#include "types.h"

struct FlashInfo08007220 {
    u8 unknown_00[24];
    s32 size;
    u8 unknown_1c[8];
    u16 control;
};

extern struct FlashInfo08007220 Data_08007be4;
extern struct FlashInfo08007220 Data_08007c10;
extern u16 Data_02004c0c;

void Func_08006ac0(void *);
u16 Func_080071a8(u32, const u8 *);

u16 Func_08007220(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;

    if ((u32)sector > 15)
        return 0x80ff;

    Func_08006ac0(savedCode);
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xfffc) |
        Data_08007c10.control;

    current = sector << 5;
    Data_02004c0c = Data_08007be4.size;

    while (Data_02004c0c != 0) {
        retries = 2;
        goto attempt;
        do {
retry_failed:
            retries -= 1;
            if (retries == 0)
                break;
attempt:
            result = Func_080071a8(current, source);
        } while (result != 0);

        if (result != 0)
            break;

        Data_02004c0c -= Data_08007c10.size;
        source += Data_08007c10.size;
        current += 1;
    }

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xfffc) | 3;
    return result;
}
