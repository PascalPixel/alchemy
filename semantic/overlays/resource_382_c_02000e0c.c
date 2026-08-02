typedef unsigned char u8;
extern u8 *Func_0808a080(int); extern void Func_0808a018(void); extern int Func_080770c0(int);
extern void Func_0808a170(int); extern void Func_0808a100(int,int); extern void Func_0808a010(int);
extern void Func_0808a180(int,int); extern void Func_0808a020(void);
void Func_02000e0c(void)
{
    u8 *actor = Func_0808a080(19); actor[91] = 1; Func_0808a018();
    if (Func_080770c0(0x855) == 0) {
        Func_0808a170(0x1241); Func_0808a100(19, 0); Func_0808a010(2);
    } else if (Func_080770c0(0x858) != 0) {
        Func_0808a170(0x13ab);
    } else {
        Func_0808a170(0x134e);
    }
    Func_0808a180(19, 0); Func_0808a020(); actor[91] = 0;
}
