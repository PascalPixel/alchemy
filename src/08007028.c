typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void Func_08006ac0(void *scratch);
extern s32 Func_080072f0(s32 mode, u8 *destination, u8 value, s32 status);

u16 Func_08007028(void)
{
    u8 scratch[64];
    u16 result;

    Func_08006ac0(scratch);
    {
        /* The flash info block is materialized only here, and only after the
         * wait-state word has been read and masked: hoisting it above the call
         * keeps it live across the call and forces a callee-saved register,
         * and opening the chain with it loads the base before the mask. */
        u16 wait = *(volatile u16 *)0x04000204;

        wait &= 0xFFFC;
        {
            u8 *info = (u8 *)0x08007C10;

            wait |= *(u16 *)(info + 36);
        }
        *(volatile u16 *)0x04000204 = wait;
    }
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x80;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x10;
    {
        /* The status word's ADDRESS is materialized before the 0x0E000000
         * destination is built; folding the load into the call argument
         * reverses the two. */
        s32 *status = (s32 *)0x02004C00;

        result = Func_080072f0(3, (u8 *)0x0E000000, 0xFF, *status);
    }
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;
    return result;
}
