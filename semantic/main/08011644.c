typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_08002f40(s32);
void Func_080053e8(s32, void *);
void Func_08005340(s32, void *);
void Func_080030f8(u32);
void Func_080113e4(void);
void Func_0800439c(s32);

void Func_08011644(void)
{
    s16 saved;
    u8 *work;
    s32 *sources;

    work = *(u8 **)0x03001E70;
    sources = FIELD(work, s32 *, 0x11C);
    saved = *(s16 *)0x05000000;

    Func_080053e8(Func_08002f40(sources[0]), (void *)0x02010000);
    *(s16 *)0x02010000 = saved;
    *(u32 *)0x040000D4 = 0x02010000;
    *(u32 *)0x040000D8 = 0x05000000;
    *(u32 *)0x040000DC = 0x84000070;

    Func_08005340(Func_08002f40(sources[1]), (void *)0x02038000);
    Func_08005340(Func_08002f40(sources[2]), (void *)0x0203A000);
    Func_08005340(Func_08002f40(sources[3]), (void *)0x0203C000);
    Func_08005340(Func_08002f40(sources[4]), (void *)0x0203E000);

    *(s32 *)0x03001CFC = 0x0801161D;
    FIELD(work, s16, 0x100) = 0;
    FIELD(work, s16, 0x102) = 0x9F;
    Func_080030f8(1);
    Func_08005340(Func_08002f40(0xD5), (void *)0x02010000);
    Func_080113e4();
    FIELD(work, u8, 0xFC) = 0;
    Func_0800439c(0x0801179D);
    Func_080030f8(1);
}
