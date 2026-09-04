/* Contiguous unnamed leaf-owner run for resource_381. */

#include "types.h"

extern s32 Data_03001e40;
extern s32 Func_02005d48(void);
extern u32 Func_02005d2e(s32, s32);
extern void Func_020051a8(s32);
extern void Func_020051b0(s32);

#define FieldScene_RunRandomHalfBranch Func_0200293c

void FieldScene_RunRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d2e(Func_02005d48(), 100) > 50) {
            Func_020051a8(1);
        } else {
            Func_020051b0(0);
        }
    }
}

#include "types.h"

#define FieldScene_RunLateRandomHalfBranch Func_02002970

extern s32 Data_03001e40;
extern s32 Func_02005d7c(void);
extern u32 Func_02005d62(s32, s32);
extern void Func_0200526c(s32);
extern void Func_02005274(s32);

void FieldScene_RunLateRandomHalfBranch(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d62(Func_02005d7c(), 100) > 50) {
            Func_0200526c(1);
        } else {
            Func_02005274(0);
        }
    }
}
