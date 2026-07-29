typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Record_08019854 {
    u8 padding00[8];
    u16 left;
    u16 top;
    u16 right;
    u16 bottom;
    u8 padding10[2];
    u16 mode;
};

struct State_08019854 {
    struct Record_08019854 *record;
    u8 padding04[16];
    u16 counter;
};

void Func_080170f8(s32, s32, s32, s32);
void Func_08016178(s32, s32, s32, s32);

void Func_08019854(struct State_08019854 *state) {
    struct Record_08019854 *record;
    u32 bottom;
    u32 left;
    u32 right;
    u32 top;
    u32 mode;

    record = state->record;
    top = record->right;
    bottom = record->bottom;
    left = record->left;
    mode = record->mode;
    right = record->top;

    if (mode == 4) {
        u32 innerTop;
        u32 innerRight;
        u32 innerBottom;
        u32 innerLeft;

        innerLeft = left + 2;
        innerRight = top;
        innerBottom = bottom;
        innerRight--;
        innerBottom--;
        innerTop = right + 2;

        Func_080170f8(innerRight, innerBottom, innerLeft, innerTop);
        state->counter += 0xFFFF;
        if (state->counter == 0) {
            state->record->mode = 0;
            Func_08016178(innerRight, innerBottom, innerLeft, innerTop);
            Func_080170f8(top, bottom, left, right);
        }
    }
}
