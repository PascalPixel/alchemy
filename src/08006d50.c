typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct FlashChip08006d50 {
    u8 unknown_00[8];
    u8 sectorShift;
    u8 unknown_09[7];
    u16 wait;
};

void Func_08006ac0(void *);
s32 Func_080072f0(s32, u8 *, u8, s32);

u16 Func_08006d50(u16 sector)
{
    u8 savedCode[64];
    u16 result;
    u16 setting;
    s32 *status;
    struct FlashChip08006d50 *chip;
    volatile u16 *waitcnt;
    volatile u8 *command;
    volatile u8 *unlock;
    volatile u8 *target;

    if (sector > 15) {
        return 0x80FF;
    }

    waitcnt = (volatile u16 *)0x04000204;
    setting = *waitcnt & 0xFFFC;
    chip = *(struct FlashChip08006d50 **)0x02004C08;
    *waitcnt = setting | chip->wait;

    target = (volatile u8 *)((sector << chip->sectorShift) + 0x0E000000);

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *target = 0x30;

    Func_08006ac0(savedCode);

    status = (s32 *)0x02004C00;
    result = Func_080072f0(2, (u8 *)target, 255, *status);

    *waitcnt = (*waitcnt & 0xFFFC) | 3;

    return result;
}
