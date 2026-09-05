#include "types.h"

extern void Func_02003a5e();

void OverlayObject_WaitUntilIdle(s32 *obj) {
    s32 i = 60;

    while (i != 0) {
        Func_02003a5e(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}
