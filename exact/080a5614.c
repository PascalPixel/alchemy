#include "far_runtime.h"
#include "types.h"

struct Selection_080a5614 {
    u8 padding0[8];
    s32 index;
    u8 paddingC[4];
    s32 selected;
    u8 padding14[4];
    s32 tile;
};

struct State_080a5614 {
    u8 padding0[0x20];
    u8 *object;
    u8 padding24[8];
    void *owner;
    u8 padding30[0x198];
    u16 tiles[1];
};

extern struct State_080a5614 *Data_03001f2c;
extern u8 Data_00000075[];

void Func_080030f8(s32);
void Func_08015080(s32, void *, s32, s32);
void Func_080a2268(const u8 *, s32, s32, s32, s32, u32);

s32 Func_080a5614(s32 unused0, s32 unused1, struct Selection_080a5614 *selection)
{
    struct State_080a5614 *state = Data_03001f2c;
    s32 row;
    s32 column;
    s32 height;
    s32 tile;
    u16 test;
    s32 condition;
    s32 graphic;

    selection->tile = selection->index * 5 + selection->selected;
    Func_08015270(state->owner);
    Func_080030f8(1);
    tile = state->tiles[selection->tile];
    test = tile;
    condition = test;
    if (condition != 0) {
        graphic = 0x1FF;
        graphic &= tile;
        graphic += (s32)Data_00000075;
        Func_08015080(
            graphic,
            state->owner,
            0,
            0);
    }

    height = 1;
    row = 0;
    column = 1;
    do {
        if (row == selection->selected)
            Func_080a2268(state->object, 1, column, 14, height, 14);
        else
            Func_080a2268(state->object, 1, column, 14, height, 15);
        row++;
        column += 2;
    } while (row <= 4);

    Func_080030f8(1);
    return 1;
}
