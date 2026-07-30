typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern void Func_08006ac0(void *);
extern u8 Func_080072f8(s32);

s32 Func_08006878(void)
{
    struct {
        u8 buffer[64];
        u16 delay;
    } locals;
    s32 value;
    s32 result;

    Func_08006ac0(locals.buffer);
    {
        volatile u8 *first = (volatile u8 *)0x0e005555;
        volatile u8 *second = (volatile u8 *)0x0e002aaa;
        *first = 0xaa;
        *second = 0x55;
        *first = 0x90;
    }

    value = 20000;
    goto first_write;
first_decrement:
    value = *(volatile u16 *)&locals.delay;
    value--;
first_write:
    *(volatile u16 *)&locals.delay = value;
    if (*(volatile u16 *)&locals.delay != 0)
        goto first_decrement;

    result = (u16)Func_080072f8(0x0e000001) << 8;
    result |= Func_080072f8(0x0e000000);

    {
        volatile u8 *first = (volatile u8 *)0x0e005555;
        volatile u8 *second = (volatile u8 *)0x0e002aaa;
        *first = 0xaa;
        *second = 0x55;
        *first = 0xf0;
    }

    value = 20000;
    goto second_write;
second_decrement:
    value = *(volatile u16 *)&locals.delay;
    value--;
second_write:
    *(volatile u16 *)&locals.delay = value;
    if (*(volatile u16 *)&locals.delay != 0)
        goto second_decrement;
    return result;
}
