#include "layout_guard.h"
#include "types.h"

typedef struct CartridgeConfig_08006910 {
    u32 value_00;
    u32 value_04;
    u32 value_08;
    u32 value_0c;
    u32 value_10;
    u8 payload[0x14];
    u16 id;
} CartridgeConfig_08006910;

LAYOUT_OFFSET_GUARD(
    CartridgeConfig08006910_Payload,
    CartridgeConfig_08006910,
    payload,
    0x14);
LAYOUT_OFFSET_GUARD(
    CartridgeConfig08006910_Id,
    CartridgeConfig_08006910,
    id,
    0x28);

extern volatile u16 Data_04000204;
extern CartridgeConfig_08006910 *Data_08007a0c[];
extern volatile u32 Data_02004c00;
extern volatile u32 Data_02004c04;
extern void *volatile Data_02004c08;
extern volatile u32 Data_02004c10;
extern volatile u32 Data_02004c14;
extern volatile u32 Data_02004c18;

s32 Func_08006878(void);

/* Select and publish the configuration matching the detected cartridge ID. */
s32 Func_08006910(void)
{
    CartridgeConfig_08006910 **cursor = Data_08007a0c;
    CartridgeConfig_08006910 *config;
    u16 id;
    s32 missing = 1;

    Data_04000204 = (Data_04000204 & 0xfffc) | 3;
    id = (u16)Func_08006878();

    for (;;) {
        config = *cursor;
        if (config->id == 0)
            break;
        if (config->id == id) {
            missing = 0;
            break;
        }
        cursor++;
    }

    Data_02004c04 = config->value_00;
    Data_02004c10 = config->value_04;
    Data_02004c14 = config->value_08;
    Data_02004c00 = config->value_0c;
    Data_02004c18 = config->value_10;
    Data_02004c08 = config->payload;
    return missing;
}
