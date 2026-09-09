#include "types.h"

#define OverlayObject_UpdateOnFrameParity Func_02001580
#define OverlayObject_ApplyRandomSlotOnOddFrames Func_020015bc
s32 Func_02002da4(s32, s32);
void Func_02002e6c(u8 *, s32);
void Func_02002c3c(u8 *);
s32 Func_02002de0(s32, s32);
void Func_02002ea8(s32, s32);

void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        Func_02002e6c(obj, Func_02002da4((s32) (Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c3c(obj);
    }
}

void OverlayObject_ApplyRandomSlotOnOddFrames(s32 obj)
{
    extern volatile s32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_02002de0((u32)Data_03001e40 >> 1, 6);

        Func_02002ea8(obj, slot);
    }
}
