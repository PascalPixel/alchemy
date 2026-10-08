#include "../../../include/system/flash.h"
#include "../../../include/system/io_reg.h"

struct FlashChipType {
    u32 handlers[5];
    u32 size;
    u16 sector_size;
    u16 unknown_1a;
    u8 sector_shift;
};

typedef s32 (*FlashVerifyCore)(s32 source, u8 *flash, u32 size);

extern const struct FlashChipType Flash_ChipUnknown;

s32 Flash_VerifySector(u16 sector, s32 source)
{
    u16 code[128];
    u16 *from;
    u16 *to;
    u16 count;
    FlashVerifyCore core;

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
    from = (u16 *)VerifyFlashCore;
    from = (u16 *)((u32)from ^ 1);
    to = code;
    count = ((u32)Flash_VerifySector - (u32)VerifyFlashCore) / 2;
    while (count != 0) {
        *to++ = *from++;
        count--;
    }
    core = (FlashVerifyCore)((u8 *)code + 1);
    return core(source, (u8 *)0x0e000000 + (sector << Flash_ChipUnknown.sector_shift), Flash_ChipUnknown.sector_size);
}
