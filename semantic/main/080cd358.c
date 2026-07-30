typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001eec;

u32 Func_080072f0(s32, s32, u32, s32);
u32 Func_08005534(void *, void *, u32);
u32 Func_080054e4(void *, void *, u32);
u32 Func_08005490(void *, u32, void *, u32);

void Func_080cd358(void)
{
    u8 *base = Data_03001eec;

    if (*(s32 *)(base + 0x7824) == 1) {
        switch (*(s32 *)(base + 0x7780)) {
        case 0:
            Func_080072f0(0x06008000, 0x02010000, 0x7800, 0x03001388);
            break;
        case 1:
            Func_080072f0(0x06008000, 0x02010000, 0x7800, 0x03001388);
            Func_080072f0(0x02010000, 0x7800, *(u32 *)(base + 0x7784), 0x03000168);
            break;
        case 2:
            if (*(s32 *)(base + 0x7784) == 50) {
                Func_08005534((void *)0x02010000, (void *)0x06008000, 0x7800);
            } else {
                Func_080054e4((void *)0x02010000, (void *)0x06008000, 0x7800);
            }
            break;
        case 3:
            Func_08005490((void *)0x02010000, *(u32 *)(base + 0x7784), (void *)0x06008000, 0x7800);
            break;
        }
        *(s32 *)(base + 0x7824) = 0;
    }
}
