typedef unsigned char u8;
typedef signed int s32;

typedef void (*Entry02000188)(void);

extern s32 Data_0200d480;
extern s32 Data_0200d484;

extern void Func_020049ea(Entry02000188);

void Func_02000188(void)
{
    Entry02000188 entry;

    Data_0200d484 = 0;
    entry = (Entry02000188) 0x0200804D;
    Data_0200d480 = 0;
    Func_020049ea(entry);
    entry();
}
