/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * Every site here dispatches to the relocated IWRAM fill at 0x03000168.
 * Its signature is established, not guessed: semantic/main/080e15e8.c
 * documents it as (destination, size, value), and this file reaches it through
 * __call_via_sl and passes 64 bytes of 0x44444444, which corroborates the
 * (destination, size, value) shape directly.
 */
#include "types.h"

typedef void (*ArmFill)(void *destination, u32 size, u32 value);

extern u32 Data_08037250[];


void Func_08021848(void)
{
    s32 group;
    s32 row;
    s32 column;
    s32 index;
    u32 *block;
    u32 *word;

    group = 0;
    do {
        row = 0;
        block = (u32 *)(0x06006280 + group * 0x180);
        do {
            ((ArmFill)0x03000168)(block, 64, 0x44444444);
            column = 1;
            word = block + 1;
            do {
                index = row;
                if (group != 1 || column > 1) {
                    if (group == 0 && row > column - 2) {
                        index = column - 2;
                        if (index < 0)
                            index = 0;
                    }
                    word[0] ^= Data_08037250[index * 2];
                    word[8] ^= Data_08037250[index * 2 + 1];
                }
                column++;
                word++;
            } while (column <= 7);
            row++;
            block += 16;
        } while (row <= 5);
        group++;
    } while (group <= 1);
}
