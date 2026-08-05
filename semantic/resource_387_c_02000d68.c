#include "types.h"

extern u8 *Data_03001f30;






/* Configure the presentation layer and expose workspace feature 0x71c. */
extern void Func_02001f70(s32 value, s32 mode);
extern void Func_02001f80(s32 left, s32 right);
extern void Func_02001fa0(void);
extern void Func_02001f86(s32 enabled);
extern void Func_02001fa2(void);
void Func_02000d68(void)
{
    Func_02001f70(78, 1);
    Func_02001f80(2, 15);
    Data_03001f30[0x71c] |= 8;
    Func_02001fa0();
    Func_02001f86(1);
    Func_02001fa2();
}
