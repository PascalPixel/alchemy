typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void *Func_080048f4(s32, s32);
u16 *Func_08002f40(void *);
void Func_080053e8(void *, void *);
void Func_08009288(void *, s32);
void Func_08003fa4(s32, s32, void *);
void Func_08002dd8(s32);
extern unsigned char Data_000000f1[];

void Func_080216e8(s32 index, s32 value, s32 flag) {
    s32 size = 1024;
    void *buffer = Func_080048f4(14, size);
    u16 *base = Func_08002f40(Data_000000f1);

    if (value <= 95) {
        Func_080053e8((void *)((u32)base + base[index]), buffer);
        if (flag != 0)
            Func_08009288(buffer, 768);
        Func_08003fa4(value, size, buffer);
        Func_08002dd8(14);
    }
}
