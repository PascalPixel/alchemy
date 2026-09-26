/* NONMATCHING: complete 100-byte owner and pool; typed-table candidate is
 * 100 bytes, 13 aligned halfword edits. The signed division read absorbs
 * the unsigned position snapshot. Volatile views preserve it but add two
 * sign-extension instructions; stop this axis without a new source model. */
#include "TYPES.H"

s32 Func_080f07f0(void *resource, s32 offset, s32 mode);
extern u16 Data_02004c00;
extern s16 Data_02004c04;
extern s16 Data_02004c08;
extern void *Data_080f1220[];

void Func_080f0614(void)
{
    u16 current;
    s16 previous;
    s32 current_group;
    s32 previous_group;

    if (Data_02004c04 != 0)
        return;

    current_group = *(s16 *)&Data_02004c00 / 8;
    current = Data_02004c00;

    previous = Data_02004c08;
    previous_group = previous / 8;

    if (current_group != previous_group) {
        Data_02004c08 = current;
        Data_02004c04 = Func_080f07f0(
            Data_080f1220[current_group],
            ((current_group + 16) & 31) * 24,
            1);
    }
}
