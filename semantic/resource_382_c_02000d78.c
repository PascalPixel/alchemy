typedef unsigned char u8; typedef unsigned short u16;
extern u8 *Func_0808a080(int); extern void Func_0808a018(void); extern int Func_080770c0(int);
extern void Func_0808a170(int); extern void Func_02000938(int); extern void Func_0808a020(void);
void Func_02000d78(void)
{
    u8 *actor = Func_0808a080(15); *(u16 *)(actor + 100) |= 2; Func_0808a018();
    if (Func_080770c0(0x855) == 0) Func_0808a170(0x123d); else Func_0808a170(0x134b);
    Func_02000938(15); Func_0808a020(); actor = Func_0808a080(15); *(u16 *)(actor + 100) &= 1;
}
