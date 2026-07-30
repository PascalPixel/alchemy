typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_08006ac0(void *scratch);
extern u16 Func_08006d50(u16 sector);
extern u16 Func_08006dec(u8 *destination, u8 *source);
extern u16 Func_08006f48(void);
extern u16 Func_08006f6c(u8 *source, void *routine);

u16 Func_08006e24(u16 sector, u8 *destination)
{
    u8 scratch[96];
    u8 *source;
    u16 result;
    u8 attempts;
    u8 index;
    u8 limit;

    if (sector > 15) {
        return 0x80FF;
    }
    source = (u8 *)(0x0E000000 + (sector << (*(u8 **)0x02004C08)[8]));
    {
        u16 *from = (u16 *)((u32)Func_08006f48 ^ 1);
        u16 *to = (u16 *)scratch;
        u16 left = (u8 *)Func_08006f6c - (u8 *)Func_08006f48;

        while (left != 0) {
            *to = *from;
            from++;
            to++;
            left -= 2;
        }
    }
    attempts = 0;
    for (;;) {
        result = Func_08006d50(sector);
        if (result == 0) {
            result = Func_08006f6c(source, scratch + 1);
            if (result == 0) {
                break;
            }
        }
        attempts++;
        if (attempts == 81) {
            return result;
        }
    }
    limit = 1;
    if (attempts != 0) {
        limit = 6;
    }
    for (index = 1; index <= limit; index++) {
        Func_08006d50(sector);
    }
    Func_08006ac0(scratch);
    {
        u16 wait = *(volatile u16 *)0x04000204;

        wait &= 0xFFFC;
        {
            u8 *info = *(u8 **)0x02004C08;

            wait |= *(u16 *)(info + 16);
            *(volatile u16 *)0x04000204 = wait;
            *(volatile u16 *)0x02004C0C = *(u32 *)(info + 4);
        }
    }
    while (*(volatile u16 *)0x02004C0C != 0) {
        result = Func_08006dec(destination, source);
        if (result != 0) {
            break;
        }
        *(volatile u16 *)0x02004C0C -= 1;
        destination++;
        source++;
    }
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;
    return result;
}
