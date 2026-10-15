#include "types.h"
#include "flag_call.h"

// Import stubs: the event runtime's begin and end, and the pending
// actions' resolution.
void Func_02000fcc(void);
void Func_02000fd4(void);
void Func_0200104c(s32 skip);
void Func_0200043c(void *cells, s32 index);
extern u32 Data_0200117c[];
extern u32 Data_020011cc[];

// For each of cells 0 to 2 of Data_020011cc whose flag (0x30a to 0x30c) is
// still clear, calls Func_0200043c and sets the flag; the event runtime is
// begun before the first and ended after the last.
void Func_02000684(void)
{
    s16 count = 0;

    if (!Flag_IsSet(0x30a)) {
        Func_02000fcc();
        Func_0200104c(0);
        Func_0200043c(Data_020011cc, 0);
        Flag_Set(0x30a);
        count = 1;
    }
    if (!Flag_IsSet(0x30b)) {
        if (count == 0)
            Func_02000fcc();
        Func_0200104c(0);
        Func_0200043c(Data_020011cc, 1);
        Flag_Set(0x30b);
        count++;
    }
    if (!Flag_IsSet(0x30c)) {
        if (count == 0)
            Func_02000fcc();
        Func_0200104c(0);
        Func_0200043c(Data_020011cc, 2);
        Flag_Set(0x30c);
        count++;
    }
    if (count != 0)
        Func_02000fd4();
}

void Func_02000728(void)
{
    if (!Flag_IsSet(0x307)) {
        Func_02000fcc();
        Func_0200104c(0);
        Func_0200043c(Data_0200117c, 4);
        Flag_Set(0x307);
        Func_02000fd4();
    }
}
