typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/* 0x02004c08 が指すフラッシュ管理ブロック。control は待ち時間設定値。 */
struct FlashInfo08006cdc {
    u8 unknown_00[4];
    s32 size;
    u8 shift;
    u8 unknown_09[7];
    u16 control;
};

void Func_08006ac0(void *);
s32 Func_080072f0(s32, u8 *, u8, s32);

/* チップ全消去。WAITCNT の SRAM 待ちをフラッシュ側の値に替え、
   0x5555/0x2aaa へ解除列と消去命令 0x10 を書き、完了待ちの結果を返す。
   待ちは復帰前に 3 に戻す。 */
u16 Func_08006cdc(void)
{
    u8 savedCode[64];
    u16 result;
    s32 *status;

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) |
        (*(struct FlashInfo08006cdc **)0x02004C08)->control;

    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x80;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x10;

    Func_08006ac0(savedCode);

    status = (s32 *)0x02004C00;
    result = Func_080072f0(3, (u8 *)0x0E000000, 255, *status);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return result;
}
