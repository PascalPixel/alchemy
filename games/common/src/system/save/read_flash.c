#include "../../../include/system/flash.h"
#include "../../../include/system/io_reg.h"

struct FlashChipType {
    u32 handlers[5];
    u32 size;
    u16 sector_size;
    u16 unknown_1a;
    u8 sector_shift;
};

typedef void (*FlashReadCore)(u32 source, u8 *destination, s32 size);

extern const struct FlashChipType Flash_ChipUnknown;

void ReadFlash(u16 sector, u32 offset, u8 *destination, s32 size)
{
    u16 code[64];
    u16 *from;
    u16 *to;
    u16 count;
    FlashReadCore core;

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
    from = (u16 *)ReadFlashCore;
    from = (u16 *)((u32)from ^ 1);
    to = code;
    count = ((u32)ReadFlash - (u32)ReadFlashCore) / 2;
    while (count != 0) {
        *to++ = *from++;
        count--;
    }
    core = (FlashReadCore)((u8 *)code + 1);
    core((u32)((u8 *)0x0e000000 + (sector << Flash_ChipUnknown.sector_shift) + offset), destination, size);
}
