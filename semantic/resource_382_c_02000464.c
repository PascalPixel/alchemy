typedef unsigned char u8;
typedef unsigned int u32;
extern void Func_080b5108(void); extern u8 *Func_08077008(int);
extern void Func_080771d0(int,int,int,int); extern void Func_080f9010(int); extern void Func_08077010(int);

void Func_02000464(void)
{
    Func_080b5108();
    if ((*(u32 *)(Func_08077008(2) + 248) & 1) != 0) {
        Func_080771d0(2, 0, 0, 0);
        Func_080f9010(126);
        Func_08077010(0);
        Func_08077010(2);
    }
}
