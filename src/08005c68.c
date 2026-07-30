#include "types.h"

/* 表の先頭3枠を作り直す。各枠64バイトをDMAで0埋めし、索引が有効な枠だけ
   文字ブロックを割り当て、作れた枠数を返す。索引が無効な副参照(+56)は0で潰す。 */

/* 記述子の基底は転送ごとに作り直す。関数先頭に置くとループ不変値になり
   退避レジスタを1本余分に取る。source は0書き込みとDMA転送元の二用途を
   持つため低位レジスタに割り付く。 */

u32 Func_08005b24(s32 arg0);
void Func_08006ba8(u16 arg0, s32 arg1, void *arg2, s32 arg3);

s32 Func_08005c68(void)
{
    u8 *destination;
    u32 i;
    s32 count;
    s32 zero;
    s32 fill;
    s32 *source;

    destination = *(u8 **)0x03001f1c + 4160;
    count = 0;
    i = 0;
    source = &fill;
    zero = 0;
    do {
        u32 *dma = (u32 *)0x040000d4;
        u32 index;
        u32 index2;

        *source = zero;
        *dma++ = (u32)source;
        *dma++ = (u32)destination;
        *dma++ = 0x85000010;
        index = Func_08005b24(i);
        if (index <= 15) {
            Func_08006ba8((u16)index, 0, destination, 64);
            count++;
        }
        index2 = Func_08005b24(i + 3);
        if (index2 <= 15)
            Func_08006ba8((u16)index2, 272, destination + 56, 4);
        else
            *(s32 *)(destination + 56) = zero;
        i++;
        destination += 64;
    } while (i <= 2);
    return count;
}
