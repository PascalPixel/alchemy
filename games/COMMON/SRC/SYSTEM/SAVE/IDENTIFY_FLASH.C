#include "../../../INCLUDE/SYSTEM/FLASH.H"
#include "../../../INCLUDE/SYSTEM/IO_REG.H"

struct FlashChip {
    u32 handlers[5];
    u8 layout[20];
    u16 id;
};

extern const struct FlashChip *const Flash_Chips[];
extern u32 Flash_Handler3;
extern u32 Flash_Handler0;
extern const void *gFlash;
extern u32 Flash_Handler1;
extern u32 gEraseFlashSector;
extern u32 Flash_Handler4;

s32 IdentifyFlash(void)
{
    u16 missing;
    u16 id;
    const struct FlashChip *const *chip;

    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;
    id = ReadFlashId();
    chip = Flash_Chips;
    missing = 1;
    while (((u8 *)&(*chip)->id)[0] != 0) {
        if (id == (*chip)->id) {
            missing = 0;
            break;
        }
        chip++;
    }
    Flash_Handler0 = (*chip)->handlers[0];
    Flash_Handler1 = (*chip)->handlers[1];
    gEraseFlashSector = (*chip)->handlers[2];
    Flash_Handler3 = (*chip)->handlers[3];
    Flash_Handler4 = (*chip)->handlers[4];
    gFlash = (*chip)->layout;
    return missing;
}
