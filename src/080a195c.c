typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001f2c;
s32 Func_08077158(void *);
void Func_08009038(void *);
s32 Func_08004278(s32);

void Func_080a195c(void)
{
    u32 buf[7];
    u8 *base = Data_03001f2c;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Func_08077158(buf);
    if (count != 0) {
        p = (void **)(base + 276);
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                Func_08009038(entry);
            }
        } while (--i != 0);
    }
    Func_08004278(0x080a19a1);
}
