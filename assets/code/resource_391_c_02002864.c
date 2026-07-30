typedef signed int s32;
typedef unsigned int u32;

extern u32 Data_03001e40;

extern void Func_02005524(s32 arg0, s32 arg1);
extern void Func_0200552c(s32 arg0, s32 arg1);

s32 Func_02002864(s32 arg0) {
    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02005524(arg0, 10);
    } else {
        Func_0200552c(arg0, 7);
    }
    return 0;
}
