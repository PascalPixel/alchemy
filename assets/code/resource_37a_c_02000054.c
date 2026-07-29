typedef signed int s32;

extern s32 Func_02002608(void);
extern s32 Func_02002a16(s32);
extern void Func_02002a3e(void);
extern void Func_02002b2e(s32, s32);
extern void Func_02002b44(s32);
extern void Func_02002a3a(s32);
extern void Func_02002a4a(s32);
extern void Func_02002a50(s32);
extern s32 Func_02002a46(s32);
extern void Func_02000a8e(void);
extern s32 Func_0200264e(void);
extern s32 Func_02002a5c(s32);
extern void Func_02001c98(void);
extern void Func_02002a90(void);
extern s32 Func_02002a72(s32);
extern void Func_02002a9a();
extern void Func_02002b8c(s32, s32);
extern void Func_02002ba2(s32);
extern void Func_02002aa8(s32);
extern void Func_02002aae(s32);
extern void Func_02002aca(void);

void Func_02000054(void)
{
    if (Func_02002608() != 0) {
        if (Func_02002a16(0x201) == 0) {
            Func_02002a3e();
            Func_02002b2e(0x2051cc, 1);
            Func_02002b44(20);
            Func_02002a3a(0x201);
            Func_02002a4a(0x200);
            Func_02002a50(0x202);
            if (Func_02002a46(0x80a) == 0) {
                Func_02000a8e();
            }
            if (Func_0200264e() != 0) {
                if (Func_02002a5c(0x811) == 0) {
                    Func_02001c98();
                }
            }
            Func_02002a90();
        }
    } else {
        if (Func_02002a72(0x200) == 0) {
            Func_02002a9a();
            Func_02002b8c(0x10000, 1);
            Func_02002ba2(20);
            Func_02002a9a(0x200);
            Func_02002aa8(0x201);
            Func_02002aae(0x202);
            Func_02002aca();
        }
    }
}
