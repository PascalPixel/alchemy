typedef signed int s32;
typedef unsigned int u32;

extern volatile s32 Data_03001e40;

extern s32 Func_0200811c(s32, s32);
extern void Func_02008204(s32, s32);

void Func_02003f88(s32 arg0)
{
    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_0200811c((u32)Data_03001e40 >> 1, 6);

        Func_02008204(arg0, slot);
    }
}
