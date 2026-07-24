typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *Data_03001e94;
extern u32 Data_08029ee4[];
void *Func_08077018(s32);
void Func_0801a5a4(struct State_0801a4c0 *, u32);

void Func_0801a370(s32 arg0)
{
    struct State_0801a4c0 *state = Data_03001e94;
    void *result = Func_08077018(0x1FF & arg0);

    if (arg0 != 0) {
        state->value = Data_08029ee4[*(u16 *)((u8 *)result + 6)];
    } else {
        state->value = Data_08029ee4[0];
    }
    state->first = 2;
    state->second = 2;
    Func_0801a5a4(state, 0);
}
