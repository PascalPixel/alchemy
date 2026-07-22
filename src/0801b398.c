typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Data_03001e98;
extern u32 Data_03001c94;
extern volatile u32 Data_03001b04;

void Func_0801b9ec(void *state, u32 index);
void Func_080030f8(u32 value);
void Func_0801b664(void *state);
void Func_0801b810(void *state);
u32 Func_0801be80(void *state);

s32 Func_0801b398(s32 mode)
{
    u8 *state = Data_03001e98;

    Func_0801b9ec(state, 0);
    for (;;) {
        Func_080030f8(1);
        if (*(u16 *)(state + 0x3a0) != 0) {
            continue;
        }
        if (mode != 0x3e7) {
            if (Data_03001b04 & 0x10) {
                Func_0801b664(state);
            } else if (Data_03001b04 & 0x20) {
                Func_0801b810(state);
            } else if (Data_03001c94 & 1) {
                return Func_0801be80(state);
            }
        }
        if (mode != 0 && (Data_03001c94 & 2)) {
            return -1;
        }
    }
}
