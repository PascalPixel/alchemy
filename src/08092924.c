typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001ebc;
void *Func_0808ba1c(s32);
void Func_080090d0(void *);

void Func_08092924(s32 index)
{
    void *object = Func_0808ba1c(index);
    u8 *base = Data_03001ebc;
    s32 offset;

    if (object != 0) {
        Func_080090d0(object);
        offset = index * 4;
        offset += 20;
        *(s32 *)(base + offset) = 0;
    }
}

void Func_0809294c(void)
{
}
