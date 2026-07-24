typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080030f8(u32);

void Func_080064b8(void)
{
    u32 i;

    i = 0;
    if (*(s32 *)0x02002080 != 0)
    {
        goto loop;
    }
    if (*(s32 *)0x020023AC != 0)
    {
        goto loop;
    }
    return;
loop:
    Func_080030f8(1);
    i++;
    if (i > 0x000927BF)
    {
        return;
    }
    if (*(s32 *)0x02002080 != 0)
    {
        goto loop;
    }
    if (*(s32 *)0x020023AC != 0)
    {
        goto loop;
    }
}
