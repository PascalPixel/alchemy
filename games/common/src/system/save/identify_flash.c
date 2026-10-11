#include "../../../include/system/flash.h"
#include "../../../include/system/flash_data.h"
#include "../../../include/system/io_reg.h"

extern const struct FlashSetupInfo *const Flash_Chips[];
extern FlashWaitProc Flash_Handler3;
extern u32 Flash_Handler0;
extern u32 Flash_Handler1;
extern u32 gEraseFlashSector;
extern u16 *Flash_Handler4;

s32 IdentifyFlash(void)
{
    u16 missing;
    u16 id;
    const struct FlashSetupInfo *const *chip;

    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;
    id = ReadFlashId();
    chip = Flash_Chips;
    missing = 1;
    while (((u8 *)&(*chip)->type.id)[0] != 0) {
        if (id == (*chip)->type.id) {
            missing = 0;
            break;
        }
        chip++;
    }
    Flash_Handler0 = (*chip)->handlers[0];
    Flash_Handler1 = (*chip)->handlers[1];
    gEraseFlashSector = (*chip)->handlers[2];
    Flash_Handler3 = (*chip)->wait;
    Flash_Handler4 = (*chip)->max_time;
    gFlash = &(*chip)->type;
    return missing;
}
