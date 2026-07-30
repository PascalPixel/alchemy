typedef signed int s32;
typedef unsigned int u32;

extern volatile s32 Data_03001e40;

extern s32 Func_02002de0(s32, s32);
extern void Func_02002ea8(s32, s32);

void Func_020015bc(s32 arg0)
{
    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_02002de0((u32)Data_03001e40 >> 1, 6);

        Func_02002ea8(arg0, slot);
    }
}
