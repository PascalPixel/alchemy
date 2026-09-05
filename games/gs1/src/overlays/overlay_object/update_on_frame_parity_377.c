#include "types.h"

extern volatile u32 Data_03001e40;

extern s32 Func_02002da4(s32, s32);
extern void Func_02002e6c(u8 *, s32);
extern void Func_02002c3c(u8 *);

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    if ((Data_03001e40 & 1) != 0) {
        Func_02002e6c(obj, Func_02002da4((s32) (Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c3c(obj);
    }
}
