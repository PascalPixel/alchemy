typedef unsigned char u8;
typedef signed int s32;

struct Point {
    u8 unknown0[8];
    s32 x;
    u8 unknown1[4];
    s32 y;
};

struct Point **Func_080b7dd0(s32);
void Func_080c0cec(s32, s32, s32, s32);

void Func_080c0df4(s32 first, s32 second, s32 mode)
{
    struct Point *left = *Func_080b7dd0(first);
    struct Point *right = *Func_080b7dd0(second);
    s32 left_x = left->x;
    s32 right_x = right->x;
    s32 left_y = left->y;
    s32 right_y = right->y;
    s32 x = (right_x + left_x) / 2;
    s32 y = (right_y + left_y) / 2;

    Func_080c0cec(x, 0, y, mode);
}
