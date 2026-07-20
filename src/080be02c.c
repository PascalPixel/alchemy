typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(u32);
void Func_08004278(void *);
s32 Func_080bdfec(void);

s32 Func_080be02c(void) {
    s32 state;
    void *runtime;

    runtime = *(void **)0x03001E74;
    state = M2C_FIELD(runtime, s32 *, 0x800);
    if (state == 0) {
        M2C_FIELD(runtime, s32 *, 0x800) = 1;
        state = 1;
    }
    if (state != 4) {
        do {
            Func_080030f8(1U);
        } while (M2C_FIELD(runtime, s32 *, 0x800) != 4);
    }
    Func_08004278((void *)0x080BD899);
    return Func_080bdfec();
}
