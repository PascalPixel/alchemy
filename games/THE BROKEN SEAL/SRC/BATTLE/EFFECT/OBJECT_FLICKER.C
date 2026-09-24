#include "TYPES.H"

extern volatile s32 Data_03001e40;
s32 Math_ModU(s32, s32);
void Animation_ApplyChildValuesFar(s32, s32);
void Func_08096ddc(s32);

void BattleFx_FlickerObjectAndTick(s32 arg0)
{
    if ((Data_03001e40 & 2) != 0) {
        Animation_ApplyChildValuesFar(arg0, 7);
    } else {
        Animation_ApplyChildValuesFar(arg0, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_08096ddc(arg0);
    }
}

void BattleFx_CycleObjectValueByCounter(s32 arg0)
{
    if ((Data_03001e40 & 1) != 0) {
        s32 value = Math_ModU((s32)((unsigned int)Data_03001e40 >> 1), 6);

        Animation_ApplyChildValuesFar(arg0, value);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_08096ddc(arg0);
    }
}
