typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Child_08096ab0 {
    u8 padding[91];
    u8 value;
};

struct State_08096ab0 {
    u8 padding_00[20];
    struct Child_08096ab0 *child;
    u8 padding_18[2];
    s16 value;
    u8 padding_1c[2];
    s16 mode;
};

struct Global_08096ab0 {
    u8 padding[0x24a];
    s16 value;
};

extern struct State_08096ab0 *Data_03001f30;
extern struct Global_08096ab0 Data_02000240;
void Func_08097608(void);

void Func_08096ab0(void)
{
    struct State_08096ab0 *state = Data_03001f30;

    if (state->mode == 2) {
        Func_08097608();
        if (Data_02000240.value != state->value) {
            state->child->value = 0;
        }
    }
}
