typedef unsigned int u32;
typedef signed int s32;

extern s32 Data_03001e40;
extern s32 Func_02005d7c(void);
extern u32 Func_02005d62(s32, s32);
extern void Func_0200526c(s32);
extern void Func_02005274(s32);

void Func_02002970(void)
{
    if ((Data_03001e40 & 1) == 0) {
        if (Func_02005d62(Func_02005d7c(), 100) > 50) {
            Func_0200526c(1);
        } else {
            Func_02005274(0);
        }
    }
}
