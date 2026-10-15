#include "types.h"

extern const u8 MsgIsaac2[];
void            Func_020006e4(void);
void            Func_020006ec(void);
void            Func_0200072c(s32 value);
void            Func_02000714(s32 message);
void            Func_0200068c(s32 index, s32 flags);
void            Func_0200071c(s32 index, s32 flags);

void Func_020000fc(s32 index)
{
    Func_020006e4();
    Func_0200072c(0);
    Func_02000714(index + (s32)MsgIsaac2);
    Func_0200068c(index, 1);
    Func_0200071c(index, 0);
    if (index == 1)
        Func_0200071c(0, 0);
    Func_020006ec();
}
