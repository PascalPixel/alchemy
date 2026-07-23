typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08015080(s32, s32, s32, s32);

void Func_080a45cc(void *values, s32 destination)
{
    s32 missing;
    s32 value;
    u32 resource;

    Func_080150b8(0xf);
    value = FIELD(values, s8 *, 0);
    missing = -1;
    if (value == missing)
        Func_080150b8(0xe);

    resource = 0xb33;
    Func_08015080(resource, destination, 0, 0x18);
    Func_080150b8(0xf);
    if (FIELD(values, s8 *, 1) == missing)
        Func_080150b8(0xe);

    Func_08015080(resource + 1, destination, 0x20, 0x18);
    Func_080150b8(0xf);
    if (FIELD(values, s8 *, 3) == missing)
        Func_080150b8(0xe);

    Func_08015080(resource + 2, destination, 0, 0x20);
    Func_080150b8(0xf);
    if (FIELD(values, s8 *, 5) == missing)
        Func_080150b8(0xe);

    Func_08015080(resource + 3, destination, 0x50, 0x20);
    Func_080150b8(0xf);
    if (FIELD(values, s8 *, 2) == missing)
        Func_080150b8(0xe);

    Func_08015080(resource + 4, destination, 0x50, 0x18);
    Func_080150b8(0xf);
    if (FIELD(values, s8 *, 4) == missing)
        Func_080150b8(0xe);

    Func_08015080(resource + 5, destination, 0x20, 0x20);
    Func_080150b8(0xf);
}
