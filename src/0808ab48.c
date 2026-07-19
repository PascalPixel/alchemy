typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Work {
    u8 unknown_000[448];
    s16 index;
};

extern struct Work Data_02000240;
extern u8 Data_0809f1a8[];
extern u8 Data_02008000[];

void Func_08002fb0(s32 value, void *destination);

void Func_0808ab48(void)
{
    u8 *table = Data_0809f1a8;
    s16 index = Data_02000240.index;
    s16 value = *(s16 *)(table + index * 8);
    Func_08002fb0(value, Data_02008000);
}
