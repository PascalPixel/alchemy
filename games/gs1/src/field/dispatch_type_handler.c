#include "types.h"
#include "scene.h"

void FieldEvent_RunTypeHandler(void);

void Field_DispatchTypeHandler(s32 kind)
{
    /* 種別0から3に対応する初期化処理を呼ぶ。 */
    if (kind == 0) {
        Field_Check();
    } else if (kind == 1) {
        BattleFx_DispatchRequestKind();
    } else if (kind == 2) {
        Field_Run();
    } else if (kind == 3) {
        FieldEvent_RunTypeHandler();
    }
}
