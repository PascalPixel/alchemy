typedef unsigned short u16;
typedef signed int s32;

void Func_0802281c(u16 *);
void Func_080b50e0(u16 *, s32);

extern void Func_080270ac(void)
    __attribute__((alias("Nested_080270ac.0")));

static __inline__ s32 Scope_080270ac(void)
{
    void Nested_080270ac(void)
    {
        u16 data[2];

        data[0] = 0xff;
        Func_0802281c(data);
        Func_080b50e0(data, 1);
    }

    return 0;
}
