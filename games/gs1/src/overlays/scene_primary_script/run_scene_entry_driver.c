#include "types.h"

#define FieldScene_RunSceneEntryDriver Func_020016f0

extern u8 Data_03001ebc[];
extern u8 Data_0000005d[];
void Func_02003332();
void Func_0200343a();
void Func_02003442();
void Func_020033c8();
void Func_020030b4();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/* Overlay loader entry point: the target of no bl inside the image
 * (regions.json), so it is reached only through the header veneer. */
s32 Func_020016f0(void)
{
    extern u8 Data_02000240[];
    u8 *base;

    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x204;
    base = Data_02000240;
    if (*(s16 *)(base + 0x1c0) == (s32)Data_0000005d) {
        *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x100;
        Func_02003332(1);
        Func_0200343a(11, 3);
        Func_02003442(12, 3);
        Call1(Func_020033c8, 0x12f);
    }
    Func_020030b4();
    return 0;
}
