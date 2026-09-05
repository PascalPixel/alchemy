#include "types.h"

s32 Func_08096810(void);
s32 Func_08096960(void);
void Func_08096ab0(void);
void FieldEvent_RunTypeHandler(void);

void Field_DispatchTypeHandler(s32 kind) {
    /* 種別0から3に対応する初期化処理を呼ぶ。 */
    if (kind == 0) {
        Func_08096810();
    } else if (kind == 1) {
        Func_08096960();
    } else if (kind == 2) {
        Func_08096ab0();
    } else if (kind == 3) {
        FieldEvent_RunTypeHandler();
    }
}
