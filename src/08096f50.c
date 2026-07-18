typedef signed int s32;

extern volatile s32 Data_03001e40;
s32 Func_08002304(s32, s32);
void Func_08009240(s32, s32);
void Func_08096ddc(s32);

void Func_08096f50(s32 arg0)
{
    if ((Data_03001e40 & 1) != 0) {
        s32 value = Func_08002304((s32)((unsigned int)Data_03001e40 >> 1), 6);

        Func_08009240(arg0, value);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_08096ddc(arg0);
    }
}
