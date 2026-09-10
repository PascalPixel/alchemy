#include "types.h"

#define HexDigitCharacters Data_0200ce3c
#define WriteU32AsHex Func_02003c94
#define FindActorAtWholeTilePosition Func_020045e0

extern u8 Data_0200ce3c[];
extern u8 *Data_03001ebc;

void WriteU32AsHex(u8 *hex_text, u32 value)
{
    s32 digit_index;

    hex_text += 8;
    *hex_text = 0;
    hex_text--;
    for (digit_index = 7; digit_index >= 0; digit_index--) {
        *hex_text = HexDigitCharacters[value & 15];
        value >>= 4;
        hex_text--;
    }
}

s32 *FindActorAtWholeTilePosition(s32 *position)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 actor_index;

    for (actor_index = 8; actor_index <= 65; actor_index++) {
        s32 *actor = slots[actor_index];

        if ((position[0] >> 20) == (actor[2] >> 20)
            && (position[1] >> 20) == (actor[3] >> 20)
            && (position[2] >> 20) == (actor[4] >> 20)) {
            return actor;
        }
    }
    return 0;
}
