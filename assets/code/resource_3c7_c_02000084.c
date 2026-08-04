#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000b4;
extern u8 Data_02009734[];
extern u8 Data_02009974[];
extern u8 Data_0200989c[];

extern s32 Func_02001278(s32);

u8 *Func_02000084(void)
{
    if (Data_02000240[224] == (s32) &Value_000000b4) {
        if (Func_02001278(0x9A7) != 0) {
            return Data_02009974;
        }
        return Data_0200989c;
    }
    return Data_02009734;
}
