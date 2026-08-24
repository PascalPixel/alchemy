#include "types.h"

extern u8 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200cb3c[];
extern u8 Data_0200ce6c[];
extern u8 Data_0200cd64[];
extern u8 Data_0200cb84[];
extern s32 Func_0200442e(s32);
extern s32 Func_0200443c(s32);

s32 Func_02000070(void)
{
    s16 *table = (s16 *)Data_02000240;

    if (table[0xe0] == (s32)&Value_0000008b)
        return (s32)Data_0200cb3c;
    if (Func_0200442e(0x950) != 0)
        return (s32)Data_0200ce6c;
    if (Func_0200443c(0x962) != 0)
        return (s32)Data_0200cd64;
    return (s32)Data_0200cb84;
}
