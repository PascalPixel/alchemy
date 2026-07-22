typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct Cell_08012078 {
    u8 filler0[2];
    u8 value;
    u8 filler3;
};

struct Descriptor_08012078 {
    struct Cell_08012078 *cells;
    u8 filler4[44];
};

struct State_08012078 {
    u8 filler0[304];
    struct Descriptor_08012078 descriptors[4];
};

extern struct State_08012078 *Data_03001e70;

void Func_08012078(u32 selector, s32 x, s32 y, u32 value)
{
    struct State_08012078 *state = Data_03001e70;

    x >>= 20;
    y >>= 20;
    if (state != 0) {
        struct Cell_08012078 *cells = state->descriptors[selector & 3].cells;
        u32 offset = (x + (y << 7)) * sizeof(struct Cell_08012078);
        u8 *cell = (u8 *)cells;

        cell += offset;
        cell[2] = value;
    }
}
