typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct Source_080b09fc {
    u8 filler0[6];
    u16 x;
    u16 y;
};

struct Entry_080b09fc {
    struct Source_080b09fc *source;
    u16 x;
    u16 y;
    u16 dx;
    u16 dy;
    s8 flag;
    s8 kind;
};

extern u8 Data_00000000[];

void Func_080b09fc(struct Entry_080b09fc *entry, u16 dx, u16 dy, s8 kind)
{
    struct Source_080b09fc *source = entry->source;
    s8 flag = (s8)(s32)Data_00000000;

    entry->x = source->x;
    entry->y = source->y;
    entry->dx = dx;
    entry->dy = dy;
    entry->kind = kind;
    entry->flag = flag;
}
