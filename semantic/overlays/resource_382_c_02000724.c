typedef unsigned char u8;
typedef unsigned short u16;
extern void Func_0808a018(void); extern void Func_0808a170(int); extern void Func_0200173c(int,int,int);
extern void Func_0808a178(int,int); extern int Func_0808a070(int,int); extern void Func_0808a180(int,int);
extern void Func_0808a020(void);

void Func_02000724(void)
{
    u8 *workspace;

    Func_0808a018();
    Func_0808a170(0x1232);
    Func_0200173c(18, 0, 2);
    Func_0808a178(18, 0);
    if (Func_0808a070(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_0808a180(18, 0);
    Func_0808a020();
}
