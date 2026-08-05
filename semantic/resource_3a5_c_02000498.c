






/* Close the paired scene flags and restore the common presentation state. */
extern void Func_02002200();
extern int Func_020021e6();
extern int Func_020021f0();
extern void Func_02002202();
extern void Func_02002208();
extern void Func_02002306();
extern void Func_020022c4();
extern void Func_02002238();
void Func_02000498(void)
{
    Func_02002200();
    if (Func_020021e6(0x8b2) == 0 && Func_020021f0(0x8b3) == 0) {
        Func_02002202(0x8b3);
        Func_02002208(0x8b2);
    }
    Func_02002306(123);
    Func_020022c4(3);
    Func_02002238();
}
