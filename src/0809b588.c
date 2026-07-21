typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct GlobalState_0809b588 {
    u8 unknown_000[0x1F4];
    u32 object_id;
};

extern struct GlobalState_0809b588 Data_02000240;
extern u32 Data_03001e40;
u8 *Func_0808ba1c(u32);
s32 Func_08002304(u32, s32);

void Func_0809b588(void) {
    s32 result;
    u8 *state;
    u8 *child;
    register u8 *value_address;
    register u8 value;

    state = *(u8 **)(Func_0808ba1c(Data_02000240.object_id) + 0x50);
    child = *(u8 **)(state + 0x28);
    result = Func_08002304(Data_03001e40, 5);
    if (result == 0) {
        value_address = state + 0x25;
        *value_address = 1;
        value_address++;
        value = 3;
        goto write_value;
    }
    if (result == 2) {
        value_address = state + 0x25;
        child[5] = 0;
        value = 1;
        *value_address = value;
        value_address++;
write_value:
        *value_address = value;
    }
}
