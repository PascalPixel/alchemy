typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

extern s32 Func_02002d0a();
extern s32 Func_02002d04();
extern s32 Func_02002cea();
extern s32 Func_02002de4();
extern s32 Func_02002df4();
extern s32 Func_02002d2e();
extern s32 Func_02002ed4();
extern s32 Func_02002d32();
extern s32 Func_02002d18();
extern s32 Func_02002d26();
extern s32 Func_02002e20();
extern s32 Func_02002e30();
extern s32 Func_02002e50();
extern s32 Func_02002e64();
extern s32 Func_02002d80();

void Func_02000abc(void) {
    u16 sel = *(u16 *)(Func_02002d0a(0) + 6);

    if ((u32)((sel + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_02002d04();
        if (Func_02002cea(0x82d) == 0) {
            Func_02002de4(0x1553);
            Func_02002df4(19, 0);
            Func_02002d0a(0x82d);
        }
        Func_02002d2e();
        Func_02002ed4(19);
    } else {
        Func_02002d32();
        if (Func_02002d18(0x881) != 0) {
            Func_02002e20(0x1671);
            Func_02002e30(19, 0);
        } else if (Func_02002d26(3) != 0) {
            Func_02002e20(0x1572);
            Func_02002e30(19, 0);
        } else {
            Func_02002e30(0x1554);
            Func_02002e50(19, 0);
            Func_02002e64(19, 0x3000, 10);
        }
        Func_02002d80();
    }
}
