#include "types.h"

/* 共有作業域の符号付き半語。添字225は src/0808b1d8.c と同じ位置を読む。 */
extern s16 Data_02000240[];

s32 Func_020000dc(void);

s32 Func_02000054(void)
{
    if (Data_02000240[225] == 15) {
        Func_020000dc();
    }
    return 0;
}
