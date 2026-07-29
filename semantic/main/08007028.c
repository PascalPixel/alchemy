#include "types.h"

void Func_08006ac0(void *);
u16 Func_080072f0(s32, u32, s32, u32);

u16 Func_08007028(void)
{
    u8 buffer[64];
    volatile u16 *waitControl;
    volatile u8 *command;
    volatile u8 *confirm;
    u16 mask;
    u16 result;

    Func_08006ac0(buffer);
    waitControl = (volatile u16 *)0x04000204;
    mask = 0xfffc;
    *waitControl =
        (*waitControl & mask) | *(u16 *)(0x08007c10 + 36);

    command = (volatile u8 *)0x0e005555;
    confirm = (volatile u8 *)0x0e002aaa;
    *command = 0xaa;
    *confirm = 0x55;
    *command = 0x80;
    *command = 0xaa;
    *confirm = 0x55;
    *command = 0x10;

    result = Func_080072f0(3, 0x0e000000, 0xff, *(u32 *)0x02004c00);
    *waitControl = (*waitControl & mask) | 3;
    return result;
}
