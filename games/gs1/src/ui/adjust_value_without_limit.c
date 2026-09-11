#include "types.h"
#include "scene.h"
#include "abi/ui/adjust_value_without_limit.h"

/* 上限を指定しない呼出し。 */
void Ui_AdjustValueWithoutLimit(s32 no, s32 value)
{
    Ui_Place(no, value, 0x7fffffff);
}
