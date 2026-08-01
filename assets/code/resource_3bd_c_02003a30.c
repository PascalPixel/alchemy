typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern s32 Func_02007670(void);
extern void Func_02007684(s32 first, s32 second, s32 *position);
extern s32 Func_02007898(void *object);
extern s32 Func_020078b0(void *object);
extern void Func_020078da(void *object);
extern void Func_02007904(s32 value);

void Func_02003a30(void *arg0)
{
    s32 position[3];
    s32 temp_r2;
    s32 temp_r3;
    u8 *state_ptr;
    s32 state;

    state_ptr = (u8 *)arg0 + 0x40;
    state = *(s8 *)state_ptr;
    if (state == 0) {
        temp_r3 = FIELD(arg0, s32, 0x18);
        temp_r2 = FIELD(arg0, s32, 0x14);
        FIELD(arg0, s32, 8) = temp_r3;
        position[2] = temp_r3;
        FIELD(arg0, s32, 4) = temp_r2;
        position[0] = temp_r2;
        Func_02007684(0x780000, Func_02007670(), position);
        FIELD(arg0, s32, 0xC) = position[0];
        FIELD(arg0, s32, 0x10) = position[2];
        FIELD(arg0, s32, 0x24) = 0x50000;
        FIELD(arg0, s32, 0x20) = 0x50000;
        FIELD(arg0, u8, 0x42) = state;
        (*state_ptr)++;
        if ((*(s32 *)0x03001800 & 3) == 0)
            Func_02007904(0x86);
    } else if (state == 1) {
        if (Func_02007898(arg0) == 0) {
            s32 value = *state_ptr;
            value--;
            *state_ptr = value;
        }
    } else if (state == 2) {
        if (Func_020078b0(arg0) == 0)
            Func_020078da(arg0);
    }
}
