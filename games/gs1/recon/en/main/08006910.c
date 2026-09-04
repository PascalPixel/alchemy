#include "flash.h"

extern volatile u16 Data_04000204;
extern u32 Data_08007a0c[];
extern volatile u32 Data_02004c00;
extern volatile u32 Data_02004c04;
extern void *volatile Data_02004c08;
extern volatile u32 Data_02004c10;
extern volatile u32 Data_02004c14;
extern volatile u32 Data_02004c18;

/* Select and publish the configuration matching the detected cartridge ID. */
s32 IdentifyFlash(void)
{
    u32 *cursor;
    u16 id;
    s32 missing;
    u16 value;

    value = Data_04000204;
    value &= 0xfffc;
    value |= 3;
    Data_04000204 = value;
    id = (u16)ReadFlashId();
    cursor = Data_08007a0c;
    missing = 1;

    for (;;) {
        u8 *config = (u8 *)*cursor;
        u16 config_id;

        if (config[0x28] == 0)
            break;
        config_id = *(u16 *)(config + 0x28);
        if (id == config_id) {
            missing = 0;
            break;
        }
        cursor++;
    }

    Data_02004c04 = *(u32 *)(*cursor + 0);
    Data_02004c10 = *(u32 *)(*cursor + 4);
    Data_02004c14 = *(u32 *)(*cursor + 8);
    Data_02004c00 = *(u32 *)(*cursor + 12);
    Data_02004c18 = *(u32 *)(*cursor + 16);
    Data_02004c08 = (void *)(*cursor + 0x14);
    return missing;
}
