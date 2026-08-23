#include "flash.h"

struct FlashChipInfo {
    u8 unknown_00[4];
    s32 size;
    u8 shift;
    u8 unknown_09[7];
    u16 control;
};


extern u16 Data_02004c0c;

u16 ProgramFlashSector(u16 sector, u8 *source)
{
    u8 savedCode[64];
    u16 result;
    u8 *destination;

    if ((u32)sector > 15)
        return 0x80FF;

    result = EraseFlashSector(sector);
    if (result != 0)
        return result;

    CopyFlashReadRoutineToRam(savedCode);
    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) |
        (*(struct FlashChipInfo **)0x02004C08)->control;
    Data_02004c0c =
        (*(struct FlashChipInfo **)0x02004C08)->size;
    destination = (u8 *)(
        (sector << (*(struct FlashChipInfo **)0x02004C08)->shift) +
        0x0E000000);

    while (Data_02004c0c != 0) {
        result = ProgramFlashByte(source, destination);
        if (result != 0)
            break;
        Data_02004c0c -= 1;
        source++;
        destination++;
    }

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return result;
}
