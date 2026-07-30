#include "types.h"

struct MatrixRow_08004c1c {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

struct Matrix_08004c1c {
    struct MatrixRow_08004c1c rows[3];
};

s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080072f0(struct Matrix_08004c1c *matrix, s32 scale, s32 mode,
                   u32 routine);

void Func_08004c1c(s32 angle)
{
    struct Matrix_08004c1c matrix;
    struct Matrix_08004c1c *destination;
    s32 sine = Func_08002322(angle);
    s32 cosine;

    destination = &matrix;
    cosine = Func_0800231c(angle);
    destination->rows[0].first = 0x10000;
    destination->rows[0].second = 0;
    destination->rows[0].third = 0;
    destination->rows[0].fourth = 0;
    destination->rows[1].first = 0x10000;
    destination->rows[1].second = 0;
    destination->rows[1].third = 0;
    destination->rows[1].fourth = 0;
    destination->rows[2].first = 0x10000;
    destination->rows[2].second = 0;
    destination->rows[2].third = 0;
    destination->rows[2].fourth = 0;

    destination->rows[0].third = -sine;
    destination->rows[0].first = cosine;
    destination->rows[1].third = sine;
    destination->rows[2].first = cosine;

    Func_080072f0(destination, 0x10000, 0, 0x030002c0);
}
