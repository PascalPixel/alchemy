#include "types.h"

#define OverlayObject_SelectValueByFrameBit1 Func_02002864

extern u32 Data_03001e40;

extern void Func_02005524(s32 arg0, s32 arg1);
extern void Func_0200552c(s32 arg0, s32 arg1);

s32 OverlayObject_SelectValueByFrameBit1(s32 obj) {
    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02005524(obj, 10);
    } else {
        Func_0200552c(obj, 7);
    }
    return 0;
}
