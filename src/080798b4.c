typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct State_080798b4 {
    u8 padding[0x128];
    u8 record;
};

struct Record_080798b4 {
    u8 padding[0x34];
    u8 value;
};

struct Table_080798b4 {
    s32 first;
    u8 padding[0x14];
};

struct Record_080798b4 *Func_080773d8(u8);
extern struct Table_080798b4 Data_08088e38[];

s32 Func_080798b4(struct State_080798b4 *state)
{
    u8 value = Func_080773d8(state->record)->value;

    if ((u32)value > 43)
        value = 0;
    return Data_08088e38[value].first;
}
