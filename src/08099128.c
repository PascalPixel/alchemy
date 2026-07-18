typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

struct Child_08099128 {
    u8 padding[35];
    u8 flags;
};

struct State_08099128 {
    u8 padding_00[20];
    struct Child_08099128 *child;
    u8 padding_18[8];
    u8 active;
    u8 padding_21[20];
    s8 mode;
};

extern struct State_08099128 *Data_03001f30;
void Func_08099160(void);

void Func_08099128(void)
{
    struct State_08099128 *state = Data_03001f30;
    struct Child_08099128 *child = state->child;

    if (child != 0) {
        if (state->mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        Func_08099160();
    }
}
