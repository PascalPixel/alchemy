#include "types.h"

#define GetXianScriptData Func_0200034c
#define GetXianInitialState Func_0200037c
#define GetXianMessageData Func_02000380
extern s16 Data_02000240[];
extern u8 Value_0000003c;
extern u8 Data_0200c7a8[];
extern u8 Data_0200c838[];
extern u8 Data_0200c8c8[];

s32 GetXianScriptData(void)
{
    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200c7a8;
    }
    return (s32)Data_0200c838;
}

s32 GetXianInitialState(void)
{
    return 0;
}

s32 GetXianMessageData(void)
{
    return (s32)Data_0200c8c8;
}
