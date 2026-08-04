#include "types.h"

s32 Func_08096810(void);
s32 Func_08096960(void);
void Func_08096ab0(void);
void Func_08096af0(void);

void Func_080967e4(s32 arg0) {
    /* 種別0から3に対応する初期化処理を呼ぶ。 */
    if (arg0 == 0) {
        Func_08096810();
    } else if (arg0 == 1) {
        Func_08096960();
    } else if (arg0 == 2) {
        Func_08096ab0();
    } else if (arg0 == 3) {
        Func_08096af0();
    }
}
