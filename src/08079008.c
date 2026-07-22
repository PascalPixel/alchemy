typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct State_08079008 {
    u8 padding[0x128];
    u8 type;
    u8 enabled;
};

struct State_08079008 *Func_08077394(s32);
extern u32 Data_0807a830[];

u32 Func_08079008(s32 owner, s32 level)
{
    struct State_08079008 *state = Func_08077394(owner);

    if (state->enabled != 0) {
        if (level <= 0) {
            return 0;
        }
        if (level <= 99 && state->type <= 7) {
            return Data_0807a830[state->type * 99 + level - 1];
        }
    }
    return (u32)-1;
}
