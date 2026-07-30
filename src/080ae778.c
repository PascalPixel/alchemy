#include "types.h"

/* 手番配分の均衡判定。Func_080ae7fc で現在の手数表を作り、from から to へ
   1つ移した仮の配分について、全ての組の差が1以内に収まるかを返す。
   件数は毎回 work[0x219] を読み直す。内側の走査は j を先に進めてから
   範囲を判定する形で、外側の条件と同じ比較を保つ。 */

void Func_080ae7fc(s8 *);

extern u8 *Data_03001f2c;

s32 Func_080ae778(s32 from, s32 to)
{
    u8 *work;
    s8 counts[16];
    u8 i;
    u8 j;
    s32 diff;
    s32 ok;

    work = Data_03001f2c;
    Func_080ae7fc(counts);
    counts[from] -= 1;
    counts[to] += 1;
    ok = 1;
    for (i = 0; i < work[0x219]; i++) {
        j = i;
        if (i < work[0x219]) {
            while (1) {
                j++;
                if (j >= work[0x219])
                    break;
                diff = counts[i] - counts[j];
                if (diff < -1 || diff > 1) {
                    ok = 0;
                    break;
                }
            }
        }
    }
    return ok;
}
