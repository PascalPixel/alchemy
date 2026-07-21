typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

u8 *Func_0808ba1c(s32);
void Func_08009088(u8 *, s32);
void Func_0809ad71(void);

struct GlobalState {
    u8 unknown_000[0x249];
    u8 saved_byte;
    u8 unknown_24a[6];
    u32 saved_callback;
};

extern struct GlobalState Data_02000240;

void Func_0809ad90(s32 arg0) {
    u8 *object;
    u8 *entry;

    object = Func_0808ba1c(arg0);
    if (object != NULL) {
        Data_02000240.saved_callback = *(u32 *)(object + 0x6C);
        Data_02000240.saved_byte = 0;
        if (object[0x54] == 1) {
            entry = *(u8 **)(*(u8 **)(object + 0x50) + 0x28);
            if (entry != NULL) {
                Data_02000240.saved_byte = entry[5];
            }
        }
        *(u32 *)(object + 0x6C) = (u32)Func_0809ad71;
        object[0x5B] = 1;
        Func_08009088(object, 0);
    }
}
