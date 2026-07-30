typedef unsigned int u32;
typedef signed int s32;

struct Entry_08004c6c {
    u32 value;
    u32 zero4;
    u32 zero8;
    u32 zeroC;
};

s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080072f0(struct Entry_08004c6c *, u32, u32, u32);

void Func_08004c6c(s32 angle)
{
    struct Entry_08004c6c entries[3];
    s32 sine;
    s32 cosine;

    sine = Func_08002322(angle);
    cosine = Func_0800231c(angle);
    entries[0].value = 0x10000;
    entries[0].zero4 = 0;
    entries[0].zero8 = 0;
    entries[0].zeroC = 0;
    entries[1].value = 0x10000;
    entries[1].zero4 = 0;
    entries[1].zero8 = 0;
    entries[1].zeroC = 0;
    entries[2].value = 0x10000;
    entries[2].zero4 = 0;
    entries[2].zero8 = 0;
    entries[2].zeroC = 0;
    entries[0].zero4 = sine;
    entries[0].value = cosine;
    entries[1].value = cosine;
    entries[0].zeroC = -sine;
    Func_080072f0(entries, 0x10000, 0, 0x030002c0);
}
