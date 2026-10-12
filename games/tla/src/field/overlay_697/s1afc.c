#include "ov697.h"

extern const u8 Data_02007734[];

void Func_02000924(s32 id);
void Func_0200095c(s32 id);
void Func_020042f0(const void *data);

void Func_02001afc(void)
{
    OBJECT_BYTE(Object_GetById(18), 98) = 1;
    OBJECT_BYTE(Object_GetById(19), 98) = 1;
    OBJECT_BYTE(Object_GetById(20), 98) = 1;
    OBJECT_BYTE(Object_GetById(21), 98) = 1;
    if (!GameFlag_Test(0x339))
        OBJECT_BYTE(Object_GetById(16), 98) = 1;
    else
        Func_020058e0(22, 0, 0);
    Func_020006b4(15);
    Func_020006b4(16);
    Func_020006b4(17);
    Func_020006b4(18);
    Func_020006b4(19);
    Func_020006b4(20);
    Func_020006b4(21);
    Func_0200062c(15);
    Func_0200062c(16);
    Func_0200062c(17);
    if (!GameFlag_Test(0x335)) {
        Func_0200095c(18);
        Func_02000924(19);
    } else {
        Func_0200095c(19);
        Func_02000924(18);
    }
    if (!GameFlag_Test(0x337)) {
        Func_0200095c(20);
        Func_02000924(21);
    } else {
        Func_0200095c(21);
        Func_02000924(20);
    }
}

void Func_02001bdc(void)
{
    Func_020042f0(Data_02007734);
}
