#include "../../../INCLUDE/SYSTEM/FLASH.H"
#include "../../../INCLUDE/SYSTEM/FLASH_DATA.H"
#include "../../../INCLUDE/SYSTEM/IO_REG.H"

/* gFlash が指すフラッシュ管理ブロック。control は待ち時間設定値。 */
struct FlashChipInfo {
    u8 unknown_00[4];
    s32 size;
    u8 shift;
    u8 unknown_09[7];
    u16 control;
};

extern struct FlashChipInfo *gFlash;
extern FlashWaitProc Flash_Handler3;

s32 _call_via_r3(s32, u8 *, u8, s32);

/* チップ全消去。WAITCNT の SRAM 待ちをフラッシュ側の値に替え、
   0x5555/0x2aaa へ解除列と消去命令 0x10 を書き、完了待ちの結果を返す。
   待ちは復帰前に 3 に戻す。 */
u16 EraseFlashChip(void)
{
    u8 savedCode[64];
    u16 result;
    s32 *status;

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) |
        gFlash->control;

    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x80;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0x10;

    CopyFlashReadRoutineToRam(savedCode);

    status = (s32 *)&Flash_Handler3;
    result = _call_via_r3(3, (u8 *)0x0E000000, 255, *status);

    REG_WAITCNT =
        (REG_WAITCNT & 0xFFFC) | 3;

    return result;
}
