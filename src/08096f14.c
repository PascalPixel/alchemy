typedef signed int s32;

extern s32 Data_03001e40;
void Func_08009240(s32, s32);
void Func_08096ddc(s32);

void Func_08096f14(s32 arg0)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_08009240(arg0, 7);
    } else {
        Func_08009240(arg0, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_08096ddc(arg0);
    }
}
