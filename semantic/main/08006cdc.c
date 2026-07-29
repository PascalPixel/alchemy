typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_08006ac0(void *);
u16 Func_080072f0(s32, u32, s32, u32);

u16 Func_08006cdc(void)
{
    u8 buffer[64];
    volatile u16 *waitcnt;
    volatile u8 *command;
    volatile u8 *confirm;
    u16 mask;
    u16 result;
    u8 unlock;
    u8 acknowledge;

    waitcnt = (volatile u16 *)0x04000204;
    mask = 0xFFFC;
    *waitcnt = (*waitcnt & mask) | *(u16 *)(*(u8 **)0x02004c08 + 16);
    command = (volatile u8 *)0x0E005555;
    confirm = (volatile u8 *)0x0E002AAA;
    unlock = 0xAA;
    *command = unlock;
    acknowledge = 0x55;
    *confirm = acknowledge;
    *command = 0x80;
    *command = unlock;
    *confirm = acknowledge;
    *command = 0x10;
    Func_08006ac0(buffer);
    result = Func_080072f0(3, 0x0E000000, 255, *(u32 *)0x02004c00);
    *waitcnt = (*waitcnt & mask) | 3;
    return result;
}
