#include "types.h"

struct Work;

s32 Func_0808a5d0(s16, s16);
void Func_08016478(void *);
void Func_0801e9a0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_0801e858(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0801e74c(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 Value_0000099b[];
extern u8 Value_00000a07[];
extern u8 Data_08037428[];
void Func_08028ef0(struct Work *work, s16 first, const s16 *second) {
    s16 selected = first;
    s32 label = Func_0808a5d0(selected, *second) + (s32)Value_0000099b;
    Func_08016478(work);
    Func_0801e9a0(selected, 3, (s32) work, 0, 14);
    Func_0801e9a0(*second, 3, (s32) work, 0x52, 14);
    Func_0801e858(Data_08037428, (s32) work, 0x4A, 0);
    Func_0801e74c(selected + (s32)Value_00000a07, (s32)work, 0, 0);
    Func_0801e858(Data_08037428, (s32) work, 0x4A, 14);
    Func_0801e74c(label, (s32)work, 0x52, 0);
}
