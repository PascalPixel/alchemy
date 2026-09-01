/* Contiguous unnamed leaf-owner run for resource_38a. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001d;
extern u8 Data_020088d8[];
extern u8 Data_02008818[];

s32 Func_02000314(void) {
    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_020088d8;
    }
    return (s32)Data_02008818;
}

#include "types.h"

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 Func_02000344(void)
{
    return 0;
}

#include "types.h"

/* Complete eight-byte literal-address getter, including its sole pool word. */
extern u8 Data_02008920[];

u8 *Func_02000348(void)
{
    return Data_02008920;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001d;
extern u8 Data_02008978[];
extern u8 Data_02008948[];

s32 Func_02000350(void) {
    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_02008978;
    }
    return (s32)Data_02008948;
}

