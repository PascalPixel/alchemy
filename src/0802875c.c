typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

extern u8 Data_000000f1[];

u32 Func_08004938(s32 size);
u16 *Func_08002f40(void *);
void Func_080053e8(void *, void *);
void Func_08003fa4(s32, s32, void *);
void Func_08002df0(void *);

void Func_0802875c(s32 arg0, s32 index)
{
    s32 size = 1024;
    u32 buffer = Func_08004938(size);
    u16 *base = Func_08002f40(Data_000000f1);

    Func_080053e8((void *)((u32)base + base[index]), (void *)buffer);
    Func_08003fa4(arg0, size, (void *)buffer);
    Func_08002df0((void *)buffer);
}
