typedef unsigned char u8; typedef unsigned short u16;
extern u8 *Func_0808a080(int); extern void Func_0808a018(void); extern int Func_080770c0(int);
extern void Func_0808a170(int); extern void Func_02000938(int); extern void Func_0808a020(void);
void Func_02000cfc(void)
{
    u8 *actor = Func_0808a080(14); u16 *state = (u16 *)(actor + 100); *state |= 2;
    Func_0808a018();
    if (Func_080770c0(0x855) == 0) {
        Func_0808a170(0x123c);
    } else {
        Func_0808a170(0x1349);
        if (Func_080770c0(2) != 0) { u8 *workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    }
    Func_02000938(14); Func_0808a020(); actor = Func_0808a080(14); *(u16 *)(actor + 100) &= 1;
}
