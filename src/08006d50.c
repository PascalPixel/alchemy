typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_08006ac0(void *scratch);
extern s32 Func_080072f0(s32 mode, u8 *destination, u8 value, s32 status);

u16 Func_08006d50(u16 sector)
{
    u8 scratch[64];
    u8 *cursor;
    u16 result;

    if (sector > 15) {
        return 0x80FF;
    }
    {
        u16 wait = *(volatile u16 *)0x04000204;

        wait &= 0xFFFC;
        {
            u8 *info = *(u8 **)0x02004C08;

            wait |= *(u16 *)(info + 16);
            *(volatile u16 *)0x04000204 = wait;
            cursor = (u8 *)(0x0E000000 + (sector << info[8]));
        }
    }
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x80;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *cursor = 0x30;
    Func_08006ac0(scratch);
    {
        s32 *status = (s32 *)0x02004C00;

        result = Func_080072f0(2, cursor, 0xFF, *status);
    }
    {
        u16 wait = *(volatile u16 *)0x04000204;

        wait &= 0xFFFC;
        wait |= 3;
        *(volatile u16 *)0x04000204 = wait;
    }
    return result;
}
