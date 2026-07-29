typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

extern s32 Func_02002c8a();
extern s32 Func_02002e38();
extern s32 Func_02002c8e();
extern s32 Func_02002d48();
extern s32 Func_02002c7e();
extern s32 Func_02002d78();
extern s32 Func_02002d88();
extern s32 Func_02002d98();
extern s32 Func_02002dbc();
extern s32 Func_02002cd8();

void Func_02000a3c(void) {
    u16 sel = *(u16 *)(Func_02002c8a(0) + 6);

    if ((u32)((sel + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_02002e38(4, 16);
    } else {
        Func_02002c8e();
        Func_02002d48(16, 0, 10);
        if (Func_02002c7e(0x881) != 0) {
            Func_02002d78(0x1653);
            Func_02002d98(16, 0);
        } else {
            Func_02002d88(0x154b);
            Func_02002d98(16, 0);
        }
        Func_02002dbc(16, 0x3000, 10);
        Func_02002cd8();
    }
}
