typedef unsigned short u16;
typedef unsigned int u32;

void Func_080072f0(void *, u32, u32, void *);

extern char Value_06000290;
extern char Value_06000280;
extern char Value_0600028c;
extern char Value_03000164;

void Func_080b5ad4(void)
{
    u32 *dma = (u32 *)0x040000d4;
    u32 source = (u32)&Value_06000290;
    u32 destination = (u32)&Value_06000280;
    u32 control = 0x80000008;

    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    Func_080072f0(&Value_0600028c, 20, control, &Value_03000164);
}

void Func_080b5b08(void)
{
    u16 zero = 0;

    *(u16 *)0x04000012 = zero;
}
