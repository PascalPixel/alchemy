typedef unsigned char u8; typedef unsigned short u16;
extern void Func_0808a018(void); extern void Func_0808a170(int); extern int Func_080770c0(int);
extern void Func_0200090c(int); extern void Func_0808a020(void);
void Func_020009f0(void)
{
    u8 *workspace;
    Func_0808a018(); Func_0808a170(0x1336);
    if (Func_080770c0(2) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Func_0200090c(12); Func_0808a020();
}
