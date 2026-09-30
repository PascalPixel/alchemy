#include "../../../INCLUDE/SYSTEM/FLASH.H"

struct FlashInfo08007220 {
    u8 unknown_00[24];
    s32 size;
    u8 unknown_1c[8];
    u16 control;
};

extern struct FlashInfo08007220 Flash_ChipAtmel;
extern struct FlashInfo08007220 Flash_ChipAtmelLayout;
extern u16 gFlashNumRemainingBytes;

u16 ProgramAtmelFlashBlock(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u16 current;
    u16 retries;

    if ((u32)sector > 15)
        return 0x80ff;

    CopyFlashReadRoutineToRam(savedCode);
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xfffc) |
        Flash_ChipAtmelLayout.control;

    current = sector << 5;
    gFlashNumRemainingBytes = Flash_ChipAtmel.size;

    while (gFlashNumRemainingBytes != 0) {
        retries = 2;
        goto attempt;
        do {
retry_failed:
            retries -= 1;
            if (retries == 0)
                break;
attempt:
            result = ProgramAtmelFlashSector(current, source);
        } while (result != 0);

        if (result != 0)
            break;

        gFlashNumRemainingBytes -= Flash_ChipAtmelLayout.size;
        source += Flash_ChipAtmelLayout.size;
        current += 1;
    }

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xfffc) | 3;
    return result;
}
