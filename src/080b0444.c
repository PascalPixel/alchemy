typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct State {
    u8 padding0[0x10];
    s32 value10;
    u8 padding14[0x108];
    s8 value11c;
};

extern struct State Data_02000240;

void Func_08077050(s32, s32);
void Func_080b0278(s32, s32);

s32 Func_080b0444(void)
{
    Data_02000240.value10 = 0x30d40;
    Data_02000240.value11c = 0x1c;
    Func_08077050(1, Func_08077028(1, 0x48d));
    Func_08077050(0, Func_08077028(0, 0x40b));
    Func_08077028(2, 0xe7);
    FIELD((void *)Func_08077008(3), s8 *, 0x131) = 1;
    FIELD((void *)Func_08077008(5), s8 *, 0x131) = 1;
    FIELD((void *)Func_08077008(2), s8 *, 0x140) = 1;
    Func_080b0278(1, 0x1e);
    return 0;
}
