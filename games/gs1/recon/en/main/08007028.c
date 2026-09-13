/* The wait callback is read from the flash runtime cell at 0x02004c00. */
#include "flash.h"
#include "flash_data.h"

/* 0x08007c10 のフラッシュ設定表。wait は WAITCNT の SRAM 待ち値。 */
struct FlashConfig08007028 {
    u8 unknown_00[0x24];
    u16 wait;
};


/* チップ全消去。復帰用の手続きを退避してから WAITCNT の SRAM 待ちを
   設定表の値に替え、0x5555/0x2aaa へ解除列と消去命令 0x10 を書く。
   0x2aaa の番地は最初の 0xAA を書いた後に作る。 */
u16 EraseAtmelFlashChip(void)
{
    u8 savedCode[64];
    u16 result;
    FlashWaitProc *wait;
    volatile u8 *command;
    volatile u8 *unlock;

    CopyFlashReadRoutineToRam(savedCode);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) |
        ((struct FlashConfig08007028 *)Data_08007c10)->wait;

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *command = 0x10;

    wait = (FlashWaitProc *)0x02004C00;
    result = (*wait)(3, (u8 *)0x0E000000, 255);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return result;
}
