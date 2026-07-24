typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 Data_000000f1[];

void *Func_080048b0(s32, s32);
u16 *Func_08002f40(void *);
void Func_080053e8(void *, void *);
s32 Func_080040d0(s32, s32);
void Func_08002dd8(s32);

s32 Func_08021c64(s32 arg0, s32 arg1)
{
    void *buffer = Func_080048b0(0x11, 0x608);
    u16 *base = Func_08002f40(Data_000000f1);
    void **slot = (void **)((u32)buffer + 0x604);
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Func_080053e8(target, buffer);
    ret = Func_080040d0(arg0, (s32)buffer);
    Func_08002dd8(0x11);
    return ret;
}
