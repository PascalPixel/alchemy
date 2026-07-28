typedef signed int s32;
typedef unsigned char u8;

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

extern struct Cell Data_02010000[];
extern u8 Data_0202c000[];

s32 Func_02000be4(s32 x, s32 z, s32 mode)
{
    s32 i;

    for (i = 0; i <= 3; i++) {
        struct Cell *cell = &Data_02010000[x + (z << 7)];

        if (cell->kind == 0xff || *(u8 *)((cell->type << 2) + (s32)Data_0202c000) != 0) {
            return -1;
        }
        if (mode == 0) {
            x++;
        } else {
            z++;
        }
    }
    return 0;
}
