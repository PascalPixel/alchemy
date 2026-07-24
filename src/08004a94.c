typedef unsigned int u32;

struct Entry {
    u32 value;
    u32 zero4;
    u32 zero8;
    u32 zeroC;
};

void Func_08004a94(void)
{
    struct Entry *entries = (struct Entry *)0x03000350;

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
}
