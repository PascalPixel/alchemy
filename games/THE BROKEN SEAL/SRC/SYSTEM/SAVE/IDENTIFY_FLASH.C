#include "FLASH.H"

/* The chip description a detected flash ID selects: five handlers, then the
   chip's geometry and timing, whose ID sits at offset 0x28. */
struct FlashChip {
    u32 handlers[5];
    u8 layout[20];
    u16 id;
};

extern const struct FlashChip *const Flash_Chips[];
extern u32 Flash_Handler3;
extern u32 Flash_Handler0;
extern const void *Flash_Layout;
extern u32 Flash_Handler1;
extern u32 Flash_Handler2;
extern u32 Flash_Handler4;

/* Select and publish the configuration matching the detected cartridge ID. */
s32 IdentifyFlash(void)
{
    u16 missing;
    u16 id;
    const struct FlashChip *const *chip;

    *(volatile u16 *)0x04000204 = (*(volatile u16 *)0x04000204 & ~3) | 3;
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
    Flash_Handler2 = (*chip)->handlers[2];
    Flash_Handler3 = (*chip)->handlers[3];
    Flash_Handler4 = (*chip)->handlers[4];
    Flash_Layout = (*chip)->layout;
    return missing;
}
