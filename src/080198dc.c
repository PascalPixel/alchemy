#include "types.h"

void Func_080198dc(void)
{
    s32 no;
    u32 *word_base;
    u16 *half_base;
    volatile u32 *word_ptr;
    volatile u16 *half_ptr;

    word_base = (u32 *)((u8 *)*(u32 *)0x03001e8c + 0x12bc);
    half_base = (u16 *)((u8 *)*(u32 *)0x03001e8c + 0x12dc);
    no = 0;

    /* 対応する値と識別子は同じ順序で消去する。 */
    do {
        word_ptr = word_base + no;
        half_ptr = half_base + no;
        *word_ptr = 0;
        *half_ptr = 0;
        no++;
    } while (no != 8);
}
