#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void UiWork_Finalize(struct Work *work, s32 release);
s32 UiWork_IsIdle(void *arg0);
void WaitFrames(s32);
s32 Resource_ResetEntry(u32 index);
void Func_08002dd8(s32);

void Func_0801c954(void) {
    u8 *r5;
    u8 *r6;
    u16 *p;

    r5 = *(u8 **)ADDR_03001E9C;
    UiWork_Finalize(M2C_FIELD(r5, s32 *, 0xFF4), 0);
    while (UiWork_IsIdle(M2C_FIELD(r5, s32 *, 0xFF4)) == 0) {
        WaitFrames(1);
    }
    if (M2C_FIELD(r5, u16 *, 0x46) != 0) {
        Resource_ResetEntry(M2C_FIELD(r5, u16 *, 0x48));
    }
    p = (u16 *)(r5 + 0x352);
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Resource_ResetEntry(*p);
    }
    Func_08002dd8(0x13);
}
