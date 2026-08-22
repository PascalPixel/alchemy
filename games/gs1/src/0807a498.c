#include "types.h"
#include "owner_state.h"
u32 Func_0807a2bc(s32 owner, s32 index, s32 bit);
s32 Func_0807a1b4(s32 owner, s32 index, s32 bit);
u32 Func_0807a350(s32 owner, s32 index, s32 bit);
s32 Func_0807a2e4(s32 owner, s32 index, s32 bit);
s32 Func_0807a3a8(s32 owner, s32 index, s32 bit);
u32 *Func_0807a458(u32 owner, u32 index, u32 bit);

s32 Func_0807a498(s32 source, s32 index, s32 bit, s32 target)
{
    struct OwnerTransferState *state = Func_08077394(source);
    /* Retained across calls for the two accesses to available[index]. */
    s32 availableOffset = index * 4 + 0xf8;
    u32 mask = 1U << bit;
    u32 present;

    if ((*(u32 *)((u8 *)state + availableOffset) & mask) != 0) {
        present = Func_0807a2bc(source, index, bit);
        if (Func_0807a1b4(target, index, bit) == 0) {
            Func_0807a350(source, index, bit);
            *(u32 *)((u8 *)state + availableOffset) &= ~mask;
            state->owned_counts[index]--;

            if (present != 0) {
                Func_0807a2e4(target, index, bit);
            } else {
                Func_0807a3a8(source, index, bit);
                Func_0807a458(target, index, bit);
            }
            return 0;
        }
    }
    return -1;
}
