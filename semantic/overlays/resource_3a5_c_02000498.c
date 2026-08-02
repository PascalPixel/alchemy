extern void Func_0808a018();
extern int Func_080770c0();
extern void Func_080770c8();
extern void Func_080f9010();
extern void Func_0808a248();
extern void Func_0808a020();

/* Close the paired scene flags and restore the common presentation state. */
void Func_02000498(void)
{
    Func_0808a018();
    if (Func_080770c0(0x8b2) == 0 && Func_080770c0(0x8b3) == 0) {
        Func_080770c8(0x8b3);
        Func_080770c8(0x8b2);
    }
    Func_080f9010(123);
    Func_0808a248(3);
    Func_0808a020();
}
