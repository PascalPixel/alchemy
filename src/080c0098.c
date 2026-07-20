typedef signed int s32;
typedef unsigned int u32;

void Func_080072f0(s32 *, s32, s32, s32);

void Func_080c0098(s32 *destination)
{
    s32 value;
    s32 count;
    s32 target;

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while ((u32)count <= 0x3F);

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while ((u32)count <= 0x37);

    target = 0x03000168;
    Func_080072f0(destination, 0x220, -1, target);
}
