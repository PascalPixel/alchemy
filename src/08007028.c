typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/* 0x08007c10 のフラッシュ設定表。wait は WAITCNT の SRAM 待ち値。 */
struct FlashConfig08007028 {
    u8 unknown_00[0x24];
    u16 wait;
};

void Func_08006ac0(void *);
s32 Func_080072f0(s32, u8 *, u8, s32);

/* チップ全消去。復帰用の手続きを退避してから WAITCNT の SRAM 待ちを
   設定表の値に替え、0x5555/0x2aaa へ解除列と消去命令 0x10 を書く。
   0x2aaa の番地は最初の 0xAA を書いた後に作る。 */
u16 Func_08007028(void)
{
    u8 savedCode[64];
    u16 result;
    s32 *status;
    volatile u8 *command;
    volatile u8 *unlock;

    Func_08006ac0(savedCode);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) |
        ((struct FlashConfig08007028 *)0x08007C10)->wait;

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *command = 0x10;

    status = (s32 *)0x02004C00;
    result = Func_080072f0(3, (u8 *)0x0E000000, 255, *status);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return result;
}
